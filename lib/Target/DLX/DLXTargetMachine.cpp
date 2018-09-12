//===-- DLXTargetMachine.cpp - Define TargetMachine for DLX ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about DLX target spec.
//
//===----------------------------------------------------------------------===//

#include "DLXTargetMachine.h"

#include "DLX.h"
#include "DLXTargetObjectFile.h"
#include "DLXTargetTransformInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

namespace llvm {
void initializeDLXMemAluCombinerPass(PassRegistry &);
} // namespace llvm

extern "C" void LLVMInitializeDLXTarget() {
  // Register the target.
  RegisterTargetMachine<DLXTargetMachine> registered_target(
      getTheDLXTarget());
}

static std::string computeDataLayout() {
  // Data layout (keep in sync with clang/lib/Basic/Targets.cpp)
  return "E"        // Big endian
         "-m:e"     // ELF name manging
         "-p:32:32" // 32-bit pointers, 32 bit aligned
         "-i64:64"  // 64 bit integers, 64 bit aligned
         "-a:0:32"  // 32 bit alignment of objects of aggregate type
         "-n32"     // 32 bit native integer width
         "-S64";    // 64 bit natural stack alignment
}

static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  if (!RM.hasValue())
    return Reloc::PIC_;
  return *RM;
}

static CodeModel::Model getEffectiveCodeModel(Optional<CodeModel::Model> CM) {
  if (CM)
    return *CM;
  return CodeModel::Medium;
}

DLXTargetMachine::DLXTargetMachine(const Target &T, const Triple &TT,
                                       StringRef Cpu, StringRef FeatureString,
                                       const TargetOptions &Options,
                                       Optional<Reloc::Model> RM,
                                       Optional<CodeModel::Model> CodeModel,
                                       CodeGenOpt::Level OptLevel, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(), TT, Cpu, FeatureString, Options,
                        getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CodeModel), OptLevel),
      Subtarget(TT, Cpu, FeatureString, *this, Options, getCodeModel(),
                OptLevel),
      TLOF(new DLXTargetObjectFile()) {
  initAsmInfo();
}

TargetTransformInfo
DLXTargetMachine::getTargetTransformInfo(const Function &F) {
  return TargetTransformInfo(DLXTTIImpl(this, F));
}

namespace {
// DLX Code Generator Pass Configuration Options.
class DLXPassConfig : public TargetPassConfig {
public:
  DLXPassConfig(DLXTargetMachine &TM, PassManagerBase *PassManager)
      : TargetPassConfig(TM, *PassManager) {}

  DLXTargetMachine &getDLXTargetMachine() const {
    return getTM<DLXTargetMachine>();
  }

  bool addInstSelector() override;
  void addPreSched2() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *
DLXTargetMachine::createPassConfig(PassManagerBase &PassManager) {
  return new DLXPassConfig(*this, &PassManager);
}

// Install an instruction selector pass.
bool DLXPassConfig::addInstSelector() {
  addPass(createDLXISelDag(getDLXTargetMachine()));
  return false;
}

// Implemented by targets that want to run passes immediately before
// machine code is emitted.
void DLXPassConfig::addPreEmitPass() {
  addPass(createDLXDelaySlotFillerPass(getDLXTargetMachine()));
}

// Run passes after prolog-epilog insertion and before the second instruction
// scheduling pass.
void DLXPassConfig::addPreSched2() {
  addPass(createDLXMemAluCombinerPass());
}
