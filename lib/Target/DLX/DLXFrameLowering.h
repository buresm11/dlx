//===-- DLXFrameLowering.h - Define frame lowering for DLX --*- C++-*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class implements DLX-specific bits of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DLX_DLXFRAMELOWERING_H
#define LLVM_LIB_TARGET_DLX_DLXFRAMELOWERING_H

#include "DLX.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {

class BitVector;
class DLXSubtarget;

class DLXFrameLowering : public TargetFrameLowering {
private:
  void determineFrameLayout(MachineFunction &MF) const;
  void replaceAdjDynAllocPseudo(MachineFunction &MF) const;

protected:
  const DLXSubtarget &STI;

public:
  explicit DLXFrameLowering(const DLXSubtarget &Subtarget)
      : TargetFrameLowering(StackGrowsDown,
                            /*StackAlignment=*/8,
                            /*LocalAreaOffset=*/0),
        STI(Subtarget) {}

  // emitProlog/emitEpilog - These methods insert prolog and epilog code into
  // the function.
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;

  bool hasFP(const MachineFunction & /*MF*/) const override { return true; }

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS = nullptr) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_DLX_DLXFRAMELOWERING_H
