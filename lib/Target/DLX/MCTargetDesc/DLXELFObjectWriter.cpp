//===-- DLXELFObjectWriter.cpp - DLX ELF Writer -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/DLXBaseInfo.h"
#include "MCTargetDesc/DLXFixupKinds.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {

class DLXELFObjectWriter : public MCELFObjectTargetWriter {
public:
  explicit DLXELFObjectWriter(uint8_t OSABI);

  ~DLXELFObjectWriter() override = default;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
  bool needsRelocateWithSymbol(const MCSymbol &SD,
                               unsigned Type) const override;
};

} // end anonymous namespace

DLXELFObjectWriter::DLXELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit_=*/false, OSABI, ELF::EM_DLX,
                              /*HasRelocationAddend=*/true) {}

unsigned DLXELFObjectWriter::getRelocType(MCContext & /*Ctx*/,
                                            const MCValue & /*Target*/,
                                            const MCFixup &Fixup,
                                            bool /*IsPCRel*/) const {
  unsigned Type;
  unsigned Kind = static_cast<unsigned>(Fixup.getKind());
  switch (Kind) {
  case DLX::FIXUP_DLX_21:
    Type = ELF::R_DLX_21;
    break;
  case DLX::FIXUP_DLX_21_F:
    Type = ELF::R_DLX_21_F;
    break;
  case DLX::FIXUP_DLX_25:
    Type = ELF::R_DLX_25;
    break;
  case DLX::FIXUP_DLX_32:
  case FK_Data_4:
    Type = ELF::R_DLX_32;
    break;
  case DLX::FIXUP_DLX_HI16:
    Type = ELF::R_DLX_HI16;
    break;
  case DLX::FIXUP_DLX_LO16:
    Type = ELF::R_DLX_LO16;
    break;
  case DLX::FIXUP_DLX_NONE:
    Type = ELF::R_DLX_NONE;
    break;

  default:
    llvm_unreachable("Invalid fixup kind!");
  }
  return Type;
}

bool DLXELFObjectWriter::needsRelocateWithSymbol(const MCSymbol & /*SD*/,
                                                   unsigned Type) const {
  switch (Type) {
  case ELF::R_DLX_21:
  case ELF::R_DLX_21_F:
  case ELF::R_DLX_25:
  case ELF::R_DLX_32:
  case ELF::R_DLX_HI16:
    return true;
  default:
    return false;
  }
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createDLXELFObjectWriter(uint8_t OSABI) {
  return llvm::make_unique<DLXELFObjectWriter>(OSABI);
}
