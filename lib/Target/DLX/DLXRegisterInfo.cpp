//===-- DLXRegisterInfo.cpp - DLX Register Information ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the DLX implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "DLXRegisterInfo.h"
#include "DLX.h"
#include "DLXSubtarget.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_REGINFO_TARGET_DESC
#include "DLXGenRegisterInfo.inc"

using namespace llvm;

DLXRegisterInfo::DLXRegisterInfo() : DLXGenRegisterInfo(DLX::RA) {}

const uint16_t *
DLXRegisterInfo::getCalleeSavedRegs(const MachineFunction * /*MF*/) const
{
  const uint16_t t = 5;
  return &t;
}

BitVector
DLXRegisterInfo::getReservedRegs(const MachineFunction &MF) const
{
  BitVector Reserved(getNumRegs());

  Reserved.set(DLX::RA);
  Reserved.set(DLX::FP);
  Reserved.set(DLX::SP);

  if (hasBasePointer(MF))
    Reserved.set(getBaseRegister());
  return Reserved;
}

bool DLXRegisterInfo::requiresRegisterScavenging(
    const MachineFunction & /*MF*/) const
{
  return true;
}

bool
DLXRegisterInfo::trackLivenessAfterRegAlloc(
    const MachineFunction & /*MF*/) const
{
  return true;
}

static bool
isALUArithLoOpcode(unsigned Opcode)
{
    return false;
}

static unsigned
getOppositeALULoOpcode(unsigned Opcode)
{
  switch (Opcode) {
  default:
    llvm_unreachable("Opcode has no RRM variant");
  }
}

static unsigned
getRRMOpcodeVariant(unsigned Opcode)
{
  switch (Opcode) {
  default:
    llvm_unreachable("Opcode has no RRM variant");
  }
}

void
DLXRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                            int SPAdj, unsigned FIOperandNum,
                                            RegScavenger *RS) const
{

}

bool
DLXRegisterInfo::hasBasePointer(const MachineFunction &MF) const
{

  return false;
}

unsigned DLXRegisterInfo::getRARegister() const
{
  return DLX::RA;
}

unsigned
DLXRegisterInfo::getFrameRegister(const MachineFunction & /*MF*/) const
{
  return DLX::FP;
}

unsigned
DLXRegisterInfo::getBaseRegister() const
{
   return DLX::R14;
}

const uint32_t *
DLXRegisterInfo::getCallPreservedMask(const MachineFunction & /*MF*/,
                                        CallingConv::ID /*CC*/) const
{
  return nullptr;
}
