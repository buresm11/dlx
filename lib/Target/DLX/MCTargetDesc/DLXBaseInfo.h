//===-- DLXBaseInfo.h - Top level definitions for DLX MC ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone helper functions and enum definitions for
// the DLX target useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DLX_MCTARGETDESC_DLXBASEINFO_H
#define LLVM_LIB_TARGET_DLX_MCTARGETDESC_DLXBASEINFO_H

#include "DLXMCTargetDesc.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {

// DLXII - This namespace holds all of the target specific flags that
// instruction info tracks.
namespace DLXII {
// Target Operand Flag enum.
enum TOF {
  //===------------------------------------------------------------------===//
  // DLX Specific MachineOperand flags.
  MO_NO_FLAG,

  // MO_ABS_HI/LO - Represents the hi or low part of an absolute symbol
  // address.
  MO_ABS_HI,
  MO_ABS_LO,
};
} // namespace DLXII

static inline unsigned getDLXRegisterNumbering(unsigned Reg) {
  switch (Reg) {
  case DLX::R0:
    return 0;
  case DLX::R1:
    return 1;
  case DLX::R2:
    return 2;
  case DLX::R3:
    return 3;
  case DLX::R4:
    return 4;
  case DLX::R5:
    return 5;
  case DLX::R6:
    return 6;
  case DLX::R7:
    return 7;
  case DLX::R8:
    return 8;
  case DLX::R9:
    return 9;
  case DLX::R10:
    return 10;
  case DLX::R11:
    return 11;
  case DLX::R12:
    return 12;
  case DLX::R13:
    return 13;
  case DLX::R14:
    return 14;
  case DLX::R15:
    return 15;
  case DLX::R16:
    return 16;
  case DLX::R17:
    return 17;
  case DLX::R18:
    return 18;
  case DLX::R19:
    return 19;
  case DLX::R20:
    return 20;
  case DLX::R21:
    return 21;
  case DLX::R22:
    return 22;
  case DLX::R23:
    return 23;
  case DLX::R24:
    return 24;
  case DLX::R25:
    return 25;
  case DLX::R26:
    return 26;
  case DLX::R27:
    return 27;
  case DLX::R28:
    return 28;
  case DLX::FP:
    return 29;
  case DLX::SP:
    return 30;
  case DLX::RA:
    return 31;
  default:
    llvm_unreachable("Unknown register number!");
  }
}
} // namespace llvm
#endif // LLVM_LIB_TARGET_DLX_MCTARGETDESC_DLXBASEINFO_H
