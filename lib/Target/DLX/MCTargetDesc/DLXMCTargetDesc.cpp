//===-- DLXMCTargetDesc.cpp - DLX Target Descriptions -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides DLX specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "DLXMCTargetDesc.h"
#include "InstPrinter/DLXInstPrinter.h"
#include "DLXMCAsmInfo.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/Triple.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"
#include <cstdint>
#include <string>

#define GET_INSTRINFO_MC_DESC
#include "DLXGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "DLXGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "DLXGenRegisterInfo.inc"

using namespace llvm;

static MCInstrInfo *createDLXMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitDLXMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createDLXMCRegisterInfo(const Triple & /*TT*/) {
  MCRegisterInfo *X = new MCRegisterInfo();
  //InitDLXMCRegisterInfo(X, DLX::RCA, 0, 0, DLX::PC);
  return X;
}

static MCSubtargetInfo *
createDLXMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  std::string CPUName = CPU;
  if (CPUName.empty())
    CPUName = "generic";

  return createDLXMCSubtargetInfoImpl(TT, CPUName, FS);
}

static MCStreamer *createMCStreamer(const Triple &T, MCContext &Context,
                                    std::unique_ptr<MCAsmBackend> &&MAB,
                                    std::unique_ptr<MCObjectWriter> &&OW,
                                    std::unique_ptr<MCCodeEmitter> &&Emitter,
                                    bool RelaxAll) {
  if (!T.isOSBinFormatELF())
    llvm_unreachable("OS not supported");

  return createELFStreamer(Context, std::move(MAB), std::move(OW),
                           std::move(Emitter), RelaxAll);
}

static MCInstPrinter *createDLXMCInstPrinter(const Triple & /*T*/,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new DLXInstPrinter(MAI, MII, MRI);
  return nullptr;
}

static MCRelocationInfo *createDLXElfRelocation(const Triple &TheTriple,
                                                  MCContext &Ctx) {
  return createMCRelocationInfo(TheTriple, Ctx);
}

namespace {

class DLXMCInstrAnalysis : public MCInstrAnalysis {
public:
  explicit DLXMCInstrAnalysis(const MCInstrInfo *Info)
      : MCInstrAnalysis(Info) {}

  bool evaluateBranch(const MCInst &Inst, uint64_t Addr, uint64_t Size,
                      uint64_t &Target) const override {
    if (Inst.getNumOperands() == 0)
      return false;

    if (Info->get(Inst.getOpcode()).OpInfo[0].OperandType ==
        MCOI::OPERAND_PCREL) {
      int64_t Imm = Inst.getOperand(0).getImm();
      Target = Addr + Size + Imm;
      return true;
    } else {
      int64_t Imm = Inst.getOperand(0).getImm();

      // Skip case where immediate is 0 as that occurs in file that isn't linked
      // and the branch target inferred would be wrong.
      if (Imm == 0)
        return false;

      Target = Imm;
      return true;
    }
  }
};

} // end anonymous namespace

static MCInstrAnalysis *createDLXInstrAnalysis(const MCInstrInfo *Info) {
  return new DLXMCInstrAnalysis(Info);
}

extern "C" void LLVMInitializeDLXTargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfo<DLXMCAsmInfo> X(getTheDLXTarget());

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(getTheDLXTarget(),
                                      createDLXMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(getTheDLXTarget(),
                                    createDLXMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(getTheDLXTarget(),
                                          createDLXMCSubtargetInfo);

  // Register the MC code emitter
  TargetRegistry::RegisterMCCodeEmitter(getTheDLXTarget(),
                                        createDLXMCCodeEmitter);

  // Register the ASM Backend
  TargetRegistry::RegisterMCAsmBackend(getTheDLXTarget(),
                                       createDLXAsmBackend);

  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(getTheDLXTarget(),
                                        createDLXMCInstPrinter);

  // Register the ELF streamer.
  TargetRegistry::RegisterELFStreamer(getTheDLXTarget(), createMCStreamer);

  // Register the MC relocation info.
  TargetRegistry::RegisterMCRelocationInfo(getTheDLXTarget(),
                                           createDLXElfRelocation);

  // Register the MC instruction analyzer.
  TargetRegistry::RegisterMCInstrAnalysis(getTheDLXTarget(),
                                          createDLXInstrAnalysis);
}
