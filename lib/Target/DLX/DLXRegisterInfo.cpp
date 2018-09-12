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

DLXRegisterInfo::DLXRegisterInfo() : DLXGenRegisterInfo(DLX::RCA) {}

const uint16_t *
DLXRegisterInfo::getCalleeSavedRegs(const MachineFunction * /*MF*/) const {
  return CSR_SaveList;
}

BitVector DLXRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(DLX::R0);
  Reserved.set(DLX::R1);
  Reserved.set(DLX::PC);
  Reserved.set(DLX::R2);
  Reserved.set(DLX::SP);
  Reserved.set(DLX::R4);
  Reserved.set(DLX::FP);
  Reserved.set(DLX::R5);
  Reserved.set(DLX::RR1);
  Reserved.set(DLX::R10);
  Reserved.set(DLX::RR2);
  Reserved.set(DLX::R11);
  Reserved.set(DLX::RCA);
  Reserved.set(DLX::R15);
  if (hasBasePointer(MF))
    Reserved.set(getBaseRegister());
  return Reserved;
}

bool DLXRegisterInfo::requiresRegisterScavenging(
    const MachineFunction & /*MF*/) const {
  return true;
}

bool DLXRegisterInfo::trackLivenessAfterRegAlloc(
    const MachineFunction & /*MF*/) const {
  return true;
}

static bool isALUArithLoOpcode(unsigned Opcode) {
  switch (Opcode) {
  case DLX::ADD_I_LO:
  case DLX::SUB_I_LO:
  case DLX::ADD_F_I_LO:
  case DLX::SUB_F_I_LO:
  case DLX::ADDC_I_LO:
  case DLX::SUBB_I_LO:
  case DLX::ADDC_F_I_LO:
  case DLX::SUBB_F_I_LO:
    return true;
  default:
    return false;
  }
}

static unsigned getOppositeALULoOpcode(unsigned Opcode) {
  switch (Opcode) {
  case DLX::ADD_I_LO:
    return DLX::SUB_I_LO;
  case DLX::SUB_I_LO:
    return DLX::ADD_I_LO;
  case DLX::ADD_F_I_LO:
    return DLX::SUB_F_I_LO;
  case DLX::SUB_F_I_LO:
    return DLX::ADD_F_I_LO;
  case DLX::ADDC_I_LO:
    return DLX::SUBB_I_LO;
  case DLX::SUBB_I_LO:
    return DLX::ADDC_I_LO;
  case DLX::ADDC_F_I_LO:
    return DLX::SUBB_F_I_LO;
  case DLX::SUBB_F_I_LO:
    return DLX::ADDC_F_I_LO;
  default:
    llvm_unreachable("Invalid ALU lo opcode");
  }
}

static unsigned getRRMOpcodeVariant(unsigned Opcode) {
  switch (Opcode) {
  case DLX::LDBs_RI:
    return DLX::LDBs_RR;
  case DLX::LDBz_RI:
    return DLX::LDBz_RR;
  case DLX::LDHs_RI:
    return DLX::LDHs_RR;
  case DLX::LDHz_RI:
    return DLX::LDHz_RR;
  case DLX::LDW_RI:
    return DLX::LDW_RR;
  case DLX::STB_RI:
    return DLX::STB_RR;
  case DLX::STH_RI:
    return DLX::STH_RR;
  case DLX::SW_RI:
    return DLX::SW_RR;
  default:
    llvm_unreachable("Opcode has no RRM variant");
  }
}

void DLXRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                            int SPAdj, unsigned FIOperandNum,
                                            RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
  bool HasFP = TFI->hasFP(MF);
  DebugLoc DL = MI.getDebugLoc();

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

  int Offset = MF.getFrameInfo().getObjectOffset(FrameIndex) +
               MI.getOperand(FIOperandNum + 1).getImm();

  // Addressable stack objects are addressed using neg. offsets from fp
  // or pos. offsets from sp/basepointer
  if (!HasFP || (needsStackRealignment(MF) && FrameIndex >= 0))
    Offset += MF.getFrameInfo().getStackSize();

  unsigned FrameReg = getFrameRegister(MF);
  if (FrameIndex >= 0) {
    if (hasBasePointer(MF))
      FrameReg = getBaseRegister();
    else if (needsStackRealignment(MF))
      FrameReg = DLX::SP;
  }

  // Replace frame index with a frame pointer reference.
  // If the offset is small enough to fit in the immediate field, directly
  // encode it.
  // Otherwise scavenge a register and encode it into a MOVHI, OR_I_LO sequence.
  if ((isSPLSOpcode(MI.getOpcode()) && !isInt<10>(Offset)) ||
      !isInt<16>(Offset)) {
    assert(RS && "Register scavenging must be on");
    unsigned Reg = RS->FindUnusedReg(&DLX::GPRRegClass);
    if (!Reg)
      Reg = RS->scavengeRegister(&DLX::GPRRegClass, II, SPAdj);
    assert(Reg && "Register scavenger failed");

    bool HasNegOffset = false;
    // ALU ops have unsigned immediate values. If the Offset is negative, we
    // negate it here and reverse the opcode later.
    if (Offset < 0) {
      HasNegOffset = true;
      Offset = -Offset;
    }

    if (!isInt<16>(Offset)) {
      // Reg = hi(offset) | lo(offset)
      BuildMI(*MI.getParent(), II, DL, TII->get(DLX::MOVHI), Reg)
          .addImm(static_cast<uint32_t>(Offset) >> 16);
      BuildMI(*MI.getParent(), II, DL, TII->get(DLX::OR_I_LO), Reg)
          .addReg(Reg)
          .addImm(Offset & 0xffffU);
    } else {
      // Reg = mov(offset)
      BuildMI(*MI.getParent(), II, DL, TII->get(DLX::ADD_I_LO), Reg)
          .addImm(0)
          .addImm(Offset);
    }
    // Reg = FrameReg OP Reg
    if (MI.getOpcode() == DLX::ADD_I_LO) {
      BuildMI(*MI.getParent(), II, DL,
              HasNegOffset ? TII->get(DLX::SUB_R) : TII->get(DLX::ADD_R),
              MI.getOperand(0).getReg())
          .addReg(FrameReg)
          .addReg(Reg)
          .addImm(LPCC::ICC_T);
      MI.eraseFromParent();
      return;
    }
    if (isSPLSOpcode(MI.getOpcode()) || isRMOpcode(MI.getOpcode())) {
      MI.setDesc(TII->get(getRRMOpcodeVariant(MI.getOpcode())));
      if (HasNegOffset) {
        // Change the ALU op (operand 3) from LPAC::ADD (the default) to
        // LPAC::SUB with the already negated offset.
        assert((MI.getOperand(3).getImm() == LPAC::ADD) &&
               "Unexpected ALU op in RRM instruction");
        MI.getOperand(3).setImm(LPAC::SUB);
      }
    } else
      llvm_unreachable("Unexpected opcode in frame index operation");

    MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, /*isDef=*/false);
    MI.getOperand(FIOperandNum + 1)
        .ChangeToRegister(Reg, /*isDef=*/false, /*isImp=*/false,
                          /*isKill=*/true);
    return;
  }

  // ALU arithmetic ops take unsigned immediates. If the offset is negative,
  // we replace the instruction with one that inverts the opcode and negates
  // the immediate.
  if ((Offset < 0) && isALUArithLoOpcode(MI.getOpcode())) {
    unsigned NewOpcode = getOppositeALULoOpcode(MI.getOpcode());
    // We know this is an ALU op, so we know the operands are as follows:
    // 0: destination register
    // 1: source register (frame register)
    // 2: immediate
    BuildMI(*MI.getParent(), II, DL, TII->get(NewOpcode),
            MI.getOperand(0).getReg())
        .addReg(FrameReg)
        .addImm(-Offset);
    MI.eraseFromParent();
  } else {
    MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, /*isDef=*/false);
    MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
  }
}

bool DLXRegisterInfo::hasBasePointer(const MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  // When we need stack realignment and there are dynamic allocas, we can't
  // reference off of the stack pointer, so we reserve a base pointer.
  if (needsStackRealignment(MF) && MFI.hasVarSizedObjects())
    return true;

  return false;
}

unsigned DLXRegisterInfo::getRARegister() const { return DLX::RCA; }

unsigned
DLXRegisterInfo::getFrameRegister(const MachineFunction & /*MF*/) const {
  return DLX::FP;
}

unsigned DLXRegisterInfo::getBaseRegister() const { return DLX::R14; }

const uint32_t *
DLXRegisterInfo::getCallPreservedMask(const MachineFunction & /*MF*/,
                                        CallingConv::ID /*CC*/) const {
  return CSR_RegMask;
}
