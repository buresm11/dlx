//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "DLXTargetObjectFile.h"

#include "DLXSubtarget.h"
#include "DLXTargetMachine.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

static cl::opt<unsigned> SSThreshold(
    "DLX-ssection-threshold", cl::Hidden,
    cl::desc("Small data and bss section threshold size (default=0)"),
    cl::init(0));

void DLXTargetObjectFile::Initialize(MCContext &Ctx,
                                       const TargetMachine &TM) {
  TargetLoweringObjectFileELF::Initialize(Ctx, TM);
  InitializeELF(TM.Options.UseInitArray);

  SmallDataSection = getContext().getELFSection(
      ".sdata", ELF::SHT_PROGBITS, ELF::SHF_WRITE | ELF::SHF_ALLOC);
  SmallBSSSection = getContext().getELFSection(".sbss", ELF::SHT_NOBITS,
                                               ELF::SHF_WRITE | ELF::SHF_ALLOC);
}

// A address must be loaded from a small section if its size is less than the
// small section size threshold. Data in this section must be addressed using
// gp_rel operator.
static bool isInSmallSection(uint64_t Size) {
  // gcc has traditionally not treated zero-sized objects as small data, so this
  // is effectively part of the ABI.
  return Size > 0 && Size <= SSThreshold;
}

// Return true if this global address should be placed into small data/bss
// section.
bool DLXTargetObjectFile::isGlobalInSmallSection(
    const GlobalObject *GO, const TargetMachine &TM) const {
  if (GO == nullptr) return TM.getCodeModel() == CodeModel::Small;

  // We first check the case where global is a declaration, because finding
  // section kind using getKindForGlobal() is only allowed for global
  // definitions.
  if (GO->isDeclaration() || GO->hasAvailableExternallyLinkage())
    return isGlobalInSmallSectionImpl(GO, TM);

  return isGlobalInSmallSection(GO, TM, getKindForGlobal(GO, TM));
}

// Return true if this global address should be placed into small data/bss
// section.
bool DLXTargetObjectFile::isGlobalInSmallSection(const GlobalObject *GO,
                                                   const TargetMachine &TM,
                                                   SectionKind Kind) const {
  return isGlobalInSmallSectionImpl(GO, TM);
}

// Return true if this global address should be placed into small data/bss
// section. This method does all the work, except for checking the section
// kind.
bool DLXTargetObjectFile::isGlobalInSmallSectionImpl(
    const GlobalObject *GO, const TargetMachine &TM) const {
  const auto *GVA = dyn_cast<GlobalVariable>(GO);

  // If not a GlobalVariable, only consider the code model.
  if (!GVA) return TM.getCodeModel() == CodeModel::Small;

  // Global values placed in sections starting with .ldata do not fit in
  // 21-bits, so always use large memory access for them. FIXME: This is a
  // workaround for a tool limitation.
  if (GVA->getSection().startswith(".ldata"))
    return false;

  if (TM.getCodeModel() == CodeModel::Small)
    return true;

  if (GVA->hasLocalLinkage())
    return false;

  if (((GVA->hasExternalLinkage() && GVA->isDeclaration()) ||
       GVA->hasCommonLinkage()))
    return false;

  Type *Ty = GVA->getValueType();
  return isInSmallSection(
      GVA->getParent()->getDataLayout().getTypeAllocSize(Ty));
}

MCSection *DLXTargetObjectFile::SelectSectionForGlobal(
    const GlobalObject *GO, SectionKind Kind, const TargetMachine &TM) const {
  // Handle Small Section classification here.
  if (Kind.isBSS() && isGlobalInSmallSection(GO, TM, Kind))
    return SmallBSSSection;
  if (Kind.isData() && isGlobalInSmallSection(GO, TM, Kind))
    return SmallDataSection;

  // Otherwise, we work the same as ELF.
  return TargetLoweringObjectFileELF::SelectSectionForGlobal(GO, Kind, TM);
}

/// Return true if this constant should be placed into small data section.
bool DLXTargetObjectFile::isConstantInSmallSection(const DataLayout &DL,
                                                     const Constant *CN) const {
  return isInSmallSection(DL.getTypeAllocSize(CN->getType()));
}

MCSection *DLXTargetObjectFile::getSectionForConstant(const DataLayout &DL,
                                                        SectionKind Kind,
                                                        const Constant *C,
                                                        unsigned &Align) const {
  if (isConstantInSmallSection(DL, C))
    return SmallDataSection;

  // Otherwise, we work the same as ELF.
  return TargetLoweringObjectFileELF::getSectionForConstant(DL, Kind, C, Align);
}
