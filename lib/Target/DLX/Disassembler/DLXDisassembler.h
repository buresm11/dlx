//===- DLXDisassembler.cpp - Disassembler for DLX -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file is part of the DLX Disassembler.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DLX_DISASSEMBLER_DLXDISASSEMBLER_H
#define LLVM_LIB_TARGET_DLX_DISASSEMBLER_DLXDISASSEMBLER_H

#define DEBUG_TYPE "DLX-disassembler"

#include "llvm/MC/MCDisassembler/MCDisassembler.h"

namespace llvm {

class DLXDisassembler : public MCDisassembler {
public:
  DLXDisassembler(const MCSubtargetInfo &STI, MCContext &Ctx);

  ~DLXDisassembler() override = default;

  // getInstruction - See MCDisassembler.
  MCDisassembler::DecodeStatus
  getInstruction(MCInst &Instr, uint64_t &Size, ArrayRef<uint8_t> Bytes,
                 uint64_t Address, raw_ostream &VStream,
                 raw_ostream &CStream) const override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DLX_DISASSEMBLER_DLXDISASSEMBLER_H
