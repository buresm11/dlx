//===-- DLXInstrInfo.cpp - DLX Instruction Information ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the DLX implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "DLX.h"
#include "DLXInstrInfo.h"
#include "DLXMachineFunctionInfo.h"
#include "DLXTargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "DLXGenInstrInfo.inc"

DLXInstrInfo::DLXInstrInfo()
    : DLXGenInstrInfo(DLX::R1, DLX::R2),
      RegisterInfo() {}


bool
DLXInstrInfo::areMemAccessesTriviallyDisjoint(
    MachineInstr &MIa, MachineInstr &MIb, AliasAnalysis * /*AA*/) const
{

      return false;
}

unsigned
DLXInstrInfo::isLoadFromStackSlot(const MachineInstr &MI,
                             int &FrameIndex) const
{
    return 1;
}

unsigned
DLXInstrInfo::isLoadFromStackSlotPostFE(const MachineInstr &MI,
                                   int &FrameIndex) const
{
  return 1;
}

unsigned
DLXInstrInfo::isStoreToStackSlot(const MachineInstr &MI,
                            int &FrameIndex) const
{
return 1;

}

void
DLXInstrInfo::copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator Position,
                 const DebugLoc &DL, unsigned DestinationRegister,
                 unsigned SourceRegister, bool KillSource) const
{

}

void
DLXInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                    MachineBasicBlock::iterator Position,
                    unsigned SourceRegister, bool IsKill, int FrameIndex,
                    const TargetRegisterClass *RegisterClass,
                    const TargetRegisterInfo *RegisterInfo) const
{

}

void
DLXInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                     MachineBasicBlock::iterator Position,
                     unsigned DestinationRegister, int FrameIndex,
                     const TargetRegisterClass *RegisterClass,
                     const TargetRegisterInfo *RegisterInfo) const
{

}

bool
DLXInstrInfo::expandPostRAPseudo(MachineInstr &MI) const
{
  return false;
}

bool
DLXInstrInfo::getMemOpBaseRegImmOfs(MachineInstr &LdSt, unsigned &BaseReg,
                           int64_t &Offset,
                           const TargetRegisterInfo *TRI) const
{
  return false;
}

std::pair<unsigned, unsigned>
DLXInstrInfo::decomposeMachineOperandsTargetFlags(unsigned TF) const
{
  return std::make_pair(TF, 0u);
}

ArrayRef<std::pair<unsigned, const char *>>
DLXInstrInfo::getSerializableDirectMachineOperandTargetFlags() const
{
  static const std::pair<unsigned, const char *> TargetFlags[] = {
      {1, "lanai-hi"},
      {2, "lanai-lo"},
      {3, "lanai-nf"}};
  return makeArrayRef(TargetFlags);
}

bool
DLXInstrInfo::analyzeBranch(MachineBasicBlock &MBB, MachineBasicBlock *&TrueBlock,
                   MachineBasicBlock *&FalseBlock,
                   SmallVectorImpl<MachineOperand> &Condition,
                   bool AllowModify) const
{
  return false;
}

unsigned
DLXInstrInfo::removeBranch(MachineBasicBlock &MBB,
                      int *BytesRemoved) const
{
  return false;

}

// For a comparison instruction, return the source registers in SrcReg and
// SrcReg2 if having two register operands, and the value it compares against
// in CmpValue. Return true if the comparison instruction can be analyzed.
bool
DLXInstrInfo::analyzeCompare(const MachineInstr &MI, unsigned &SrcReg,
                    unsigned &SrcReg2, int &CmpMask,
                    int &CmpValue) const
{

return false;
}

// See if the comparison instruction can be converted into something more
// efficient. E.g., on DLX register-register instructions can set the flag
// register, obviating the need for a separate compare.
bool
DLXInstrInfo::optimizeCompareInstr(MachineInstr &CmpInstr, unsigned SrcReg,
                          unsigned SrcReg2, int CmpMask, int CmpValue,
                          const MachineRegisterInfo *MRI) const
{
return false;

}

// Analyze the given select instruction, returning true if it cannot be
// understood. It is assumed that MI->isSelect() is true.
//
// When successful, return the controlling condition and the operands that
// determine the true and false result values.
//
//   Result = SELECT Cond, TrueOp, FalseOp
//
// DLX can optimize certain select instructions, for example by predicating
// the instruction defining one of the operands and sets Optimizable to true.
bool
DLXInstrInfo::analyzeSelect(const MachineInstr &MI,
                   SmallVectorImpl<MachineOperand> &Cond, unsigned &TrueOp,
                   unsigned &FalseOp, bool &Optimizable) const
{
 return false;
}

// Given a select instruction that was understood by analyzeSelect and
// returned Optimizable = true, attempt to optimize MI by merging it with one
// of its operands. Returns NULL on failure.
//
// When successful, returns the new select instruction. The client is
// responsible for deleting MI.
//
// If both sides of the select can be optimized, the TrueOp is modifed.
// PreferFalse is not used.
MachineInstr*
DLXInstrInfo::optimizeSelect(MachineInstr &MI,
                             SmallPtrSetImpl<MachineInstr *> &SeenMIs,
                             bool PreferFalse) const
{
  return nullptr;
}

bool
DLXInstrInfo::reverseBranchCondition(
    SmallVectorImpl<MachineOperand> &Condition) const
{
  return false;

}


unsigned
DLXInstrInfo::insertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TrueBlock,
                      MachineBasicBlock *FalseBlock,
                      ArrayRef<MachineOperand> Condition,
                      const DebugLoc &DL,
                      int *BytesAdded) const
{
  return 1;
}
