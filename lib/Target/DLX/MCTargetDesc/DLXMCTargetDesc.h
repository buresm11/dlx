//===-- DLXMCTargetDesc.h - DLX Target Descriptions ---------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_DLX_MCTARGETDESC_DLXMCTARGETDESC_H
#define LLVM_LIB_TARGET_DLX_MCTARGETDESC_DLXMCTARGETDESC_H

#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCInstrAnalysis;
class MCObjectTargetWriter;
class MCRelocationInfo;
class MCSubtargetInfo;
class Target;
class Triple;
class StringRef;
class raw_pwrite_stream;

Target &getTheDLXTarget();

MCCodeEmitter *createDLXMCCodeEmitter(const MCInstrInfo &MCII,
                                        const MCRegisterInfo &MRI,
                                        MCContext &Ctx);

MCAsmBackend *createDLXAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                    const MCRegisterInfo &MRI,
                                    const MCTargetOptions &Options);

std::unique_ptr<MCObjectTargetWriter> createDLXELFObjectWriter(uint8_t OSABI);
} // namespace llvm

// Defines symbolic names for DLX registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "DLXGenRegisterInfo.inc"

// Defines symbolic names for the DLX instructions.
#define GET_INSTRINFO_ENUM
#include "DLXGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "DLXGenSubtargetInfo.inc"

#endif // LLVM_LIB_TARGET_DLX_MCTARGETDESC_DLXMCTARGETDESC_H
