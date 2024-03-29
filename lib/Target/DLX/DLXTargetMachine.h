//===-- DLXTargetMachine.h - Define TargetMachine for DLX --- C++ ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the DLX specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DLX_DLXTARGETMACHINE_H
#define LLVM_LIB_TARGET_DLX_DLXTARGETMACHINE_H

#include "DLXFrameLowering.h"
#include "DLXISelLowering.h"
#include "DLXInstrInfo.h"
#include "DLXSelectionDAGInfo.h"
#include "DLXSubtarget.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class formatted_raw_ostream;

class DLXTargetMachine : public LLVMTargetMachine {
  DLXSubtarget Subtarget;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;

public:
  DLXTargetMachine(const Target &TheTarget, const Triple &TargetTriple,
                     StringRef Cpu, StringRef FeatureString,
                     const TargetOptions &Options,
                     Optional<Reloc::Model> RelocationModel,
                     Optional<CodeModel::Model> CodeModel,
                     CodeGenOpt::Level OptLevel, bool JIT);

  const DLXSubtarget *
  getSubtargetImpl(const llvm::Function & /*Fn*/) const override {
    return &Subtarget;
  }

  TargetTransformInfo getTargetTransformInfo(const Function &F) override;

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &pass_manager) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  bool isMachineVerifierClean() const override {
    return false;
  }
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_DLX_DLXTARGETMACHINE_H
