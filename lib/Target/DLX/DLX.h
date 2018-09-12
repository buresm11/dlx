//===-- DLX.h - Top-level interface for DLX representation --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// DLX back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DLX_DLX_H
#define LLVM_LIB_TARGET_DLX_DLX_H

#include "DLXAluCode.h"
#include "DLXCondCode.h"
#include "MCTargetDesc/DLXBaseInfo.h"
#include "MCTargetDesc/DLXMCTargetDesc.h"
#include "llvm/CodeGen/ISDOpcodes.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class FunctionPass;
class DLXTargetMachine;
class MachineFunctionPass;
class TargetMachine;
class formatted_raw_ostream;

// createDLXISelDag - This pass converts a legalized DAG into a
// DLX-specific DAG, ready for instruction scheduling.
FunctionPass *createDLXISelDag(DLXTargetMachine &TM);

// createDLXDelaySlotFillerPass - This pass fills delay slots
// with useful instructions or nop's
FunctionPass *createDLXDelaySlotFillerPass(const DLXTargetMachine &TM);

// createDLXMemAluCombinerPass - This pass combines loads/stores and
// arithmetic operations.
FunctionPass *createDLXMemAluCombinerPass();

// createDLXSetflagAluCombinerPass - This pass combines SET_FLAG and ALU
// operations.
FunctionPass *createDLXSetflagAluCombinerPass();

Target &getTheDLXTarget();
} // namespace llvm

#endif // LLVM_LIB_TARGET_DLX_DLX_H
