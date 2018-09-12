//===-- DLXFixupKinds.h - DLX Specific Fixup Entries --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DLX_MCTARGETDESC_DLXFIXUPKINDS_H
#define LLVM_LIB_TARGET_DLX_MCTARGETDESC_DLXFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace DLX {
// Although most of the current fixup types reflect a unique relocation
// one can have multiple fixup types for a given relocation and thus need
// to be uniquely named.
//
// This table *must* be in the save order of
// MCFixupKindInfo Infos[DLX::NumTargetFixupKinds]
// in DLXAsmBackend.cpp.
//
enum Fixups {
  // Results in R_DLX_NONE
  FIXUP_DLX_NONE = FirstTargetFixupKind,

  FIXUP_DLX_21,   // 21-bit symbol relocation
  FIXUP_DLX_21_F, // 21-bit symbol relocation, last two bits masked to 0
  FIXUP_DLX_25,   // 25-bit branch targets
  FIXUP_DLX_32,   // general 32-bit relocation
  FIXUP_DLX_HI16, // upper 16-bits of a symbolic relocation
  FIXUP_DLX_LO16, // lower 16-bits of a symbolic relocation

  // Marker
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
} // namespace DLX
} // namespace llvm

#endif // LLVM_LIB_TARGET_DLX_MCTARGETDESC_DLXFIXUPKINDS_H
