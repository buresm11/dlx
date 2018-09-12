//===-- DLXISelLowering.cpp - DLX DAG Lowering Implementation ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the DLXTargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "DLXISelLowering.h"
#include "DLX.h"
#include "DLXCondCode.h"
#include "DLXMachineFunctionInfo.h"
#include "DLXSubtarget.h"
#include "DLXTargetObjectFile.h"
#include "MCTargetDesc/DLXBaseInfo.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RuntimeLibcalls.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/CodeGen/TargetCallingConv.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/KnownBits.h"
#include "llvm/Support/MachineValueType.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <utility>

#define DEBUG_TYPE "DLX-lower"

using namespace llvm;

// Limit on number of instructions the lowered multiplication may have before a
// call to the library function should be generated instead. The threshold is
// currently set to 14 as this was the smallest threshold that resulted in all
// constant multiplications being lowered. A threshold of 5 covered all cases
// except for one multiplication which required 14. mulsi3 requires 16
// instructions (including the prologue and epilogue but excluding instructions
// at call site). Until we can inline mulsi3, generating at most 14 instructions
// will be faster than invoking mulsi3.

DLXTargetLowering::DLXTargetLowering(const TargetMachine &TM,
                                         const DLXSubtarget &STI)
    : TargetLowering(TM) {
}

SDValue DLXTargetLowering::LowerOperation(SDValue Op,
                                            SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  default:
    llvm_unreachable("unimplemented operand");
  }
}

std::pair<unsigned, const TargetRegisterClass *>
DLXTargetLowering::getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                                                  StringRef Constraint,
                                                  MVT VT) const {

  return TargetLowering::getRegForInlineAsmConstraint(TRI, Constraint, VT);
}

SDValue
DLXTargetLowering::PerformDAGCombine(SDNode *N, DAGCombinerInfo &DCI) const
{
  return SDValue();
}

void
DLXTargetLowering::LowerAsmOperandForConstraint(SDValue Op, std::string &Constraint,
                                  std::vector<SDValue> &Ops,
                                  SelectionDAG &DAG) const
{

}

TargetLowering::ConstraintWeight
DLXTargetLowering::getSingleConstraintMatchWeight(AsmOperandInfo &Info,
                               const char *Constraint) const
{
  ConstraintWeight Weight = CW_Invalid;

  return Weight;
}

void
DLXTargetLowering::computeKnownBitsForTargetNode(const SDValue Op, KnownBits &Known,
                                   const APInt &DemandedElts,
                                   const SelectionDAG &DAG,
                                   unsigned Depth) const
{

}
