//===-- DLXISelLowering.h - DLX DAG Lowering Interface -....-*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that DLX uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DLX_DLXISELLOWERING_H
#define LLVM_LIB_TARGET_DLX_DLXISELLOWERING_H

#include "DLX.h"
#include "DLXRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
namespace DLXISD {
enum {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,

  ADJDYNALLOC,

  // Return with a flag operand. Operand 0 is the chain operand.
  RET_FLAG,

  // CALL - These operations represent an abstract call instruction, which
  // includes a bunch of information.
  CALL,

  // SELECT_CC - Operand 0 and operand 1 are selection variable, operand 3
  // is condition code and operand 4 is flag operand.
  SELECT_CC,

  // SETCC - Store the conditional code to a register.
  SETCC,

  // SET_FLAG - Set flag compare.
  SET_FLAG,

  // SUBBF - Subtract with borrow that sets flags.
  SUBBF,

  // BR_CC - Used to glue together a conditional branch and comparison
  BR_CC,

  // Wrapper - A wrapper node for TargetConstantPool, TargetExternalSymbol,
  // and TargetGlobalAddress.
  Wrapper,

  // Get the Higher/Lower 16 bits from a 32-bit immediate.
  HI,
  LO,

  // Small 21-bit immediate in global memory.
  SMALL
};
} // namespace DLXISD

class DLXSubtarget;

class DLXTargetLowering : public TargetLowering {
public:
  DLXTargetLowering(const TargetMachine &TM, const DLXSubtarget &STI);

  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;


 std::pair<unsigned, const TargetRegisterClass *>
  getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                               StringRef Constraint, MVT VT) const override;
  ConstraintWeight
  getSingleConstraintMatchWeight(AsmOperandInfo &Info,
                                 const char *Constraint) const override;

  void LowerAsmOperandForConstraint(SDValue Op, std::string &Constraint,
                                    std::vector<SDValue> &Ops,
                                    SelectionDAG &DAG) const override;

  SDValue PerformDAGCombine(SDNode *N, DAGCombinerInfo &DCI) const override;

  void computeKnownBitsForTargetNode(const SDValue Op, KnownBits &Known,
                                     const APInt &DemandedElts,
                                     const SelectionDAG &DAG,
                                     unsigned Depth = 0) const override;

};
} // namespace llvm

#endif // LLVM_LIB_TARGET_DLX_DLXISELLOWERING_H
