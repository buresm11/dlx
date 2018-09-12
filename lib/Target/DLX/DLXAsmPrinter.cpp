//===-- DLXAsmPrinter.cpp - DLX LLVM assembly writer ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the DLX assembly language.
//
//===----------------------------------------------------------------------===//

#include "InstPrinter/DLXInstPrinter.h"
#include "DLX.h"
#include "DLXInstrInfo.h"
#include "DLXMCInstLower.h"
#include "DLXTargetMachine.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "asm-printer"

using namespace llvm;

namespace {
class DLXAsmPrinter : public AsmPrinter {
public:
  explicit DLXAsmPrinter(TargetMachine &TM,
                           std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "DLX Assembly Printer"; }

  void printOperand(const MachineInstr *MI, int OpNum, raw_ostream &O);
  bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                       unsigned AsmVariant, const char *ExtraCode,
                       raw_ostream &O) override;
  void EmitInstruction(const MachineInstr *MI) override;
  bool isBlockOnlyReachableByFallthrough(
      const MachineBasicBlock *MBB) const override;

private:
  void customEmitInstruction(const MachineInstr *MI);
  void emitCallInstruction(const MachineInstr *MI);
};
} // end of anonymous namespace

void DLXAsmPrinter::printOperand(const MachineInstr *MI, int OpNum,
                                   raw_ostream &O) {
  const MachineOperand &MO = MI->getOperand(OpNum);

  switch (MO.getType()) {
  case MachineOperand::MO_Register:
    O << DLXInstPrinter::getRegisterName(MO.getReg());
    break;

  case MachineOperand::MO_Immediate:
    O << MO.getImm();
    break;

  case MachineOperand::MO_MachineBasicBlock:
    O << *MO.getMBB()->getSymbol();
    break;

  case MachineOperand::MO_GlobalAddress:
    O << *getSymbol(MO.getGlobal());
    break;

  case MachineOperand::MO_BlockAddress: {
    MCSymbol *BA = GetBlockAddressSymbol(MO.getBlockAddress());
    O << BA->getName();
    break;
  }

  case MachineOperand::MO_ExternalSymbol:
    O << *GetExternalSymbolSymbol(MO.getSymbolName());
    break;

  case MachineOperand::MO_JumpTableIndex:
    O << MAI->getPrivateGlobalPrefix() << "JTI" << getFunctionNumber() << '_'
      << MO.getIndex();
    break;

  case MachineOperand::MO_ConstantPoolIndex:
    O << MAI->getPrivateGlobalPrefix() << "CPI" << getFunctionNumber() << '_'
      << MO.getIndex();
    return;

  default:
    llvm_unreachable("<unknown operand type>");
  }
}

// PrintAsmOperand - Print out an operand for an inline asm expression.
bool DLXAsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                                      unsigned /*AsmVariant*/,
                                      const char *ExtraCode, raw_ostream &O) {
  // Does this asm operand have a single letter operand modifier?
  if (ExtraCode && ExtraCode[0]) {
    if (ExtraCode[1])
      return true; // Unknown modifier.

    switch (ExtraCode[0]) {
    // The highest-numbered register of a pair.
    case 'H': {
      if (OpNo == 0)
        return true;
      const MachineOperand &FlagsOP = MI->getOperand(OpNo - 1);
      if (!FlagsOP.isImm())
        return true;
      unsigned Flags = FlagsOP.getImm();
      unsigned NumVals = InlineAsm::getNumOperandRegisters(Flags);
      if (NumVals != 2)
        return true;
      unsigned RegOp = OpNo + 1;
      if (RegOp >= MI->getNumOperands())
        return true;
      const MachineOperand &MO = MI->getOperand(RegOp);
      if (!MO.isReg())
        return true;
      unsigned Reg = MO.getReg();
      O << DLXInstPrinter::getRegisterName(Reg);
      return false;
    }
    default:
      return true; // Unknown modifier.
    }
  }
  printOperand(MI, OpNo, O);
  return false;
}

//===----------------------------------------------------------------------===//
void DLXAsmPrinter::emitCallInstruction(const MachineInstr *MI) {
  /*assert((MI->getOpcode() == DLX::CALL || MI->getOpcode() == DLX::CALLR) &&
         "Unsupported call function");

  DLXMCInstLower MCInstLowering(OutContext, *this);
  MCSubtargetInfo STI = getSubtargetInfo();
  // Insert save rca instruction immediately before the call.
  // TODO: We should generate a pc-relative mov instruction here instead
  // of pc + 16 (should be mov .+16 %rca).
  OutStreamer->EmitInstruction(MCInstBuilder(DLX::ADD_I_LO)
                                   .addReg(DLX::RCA)
                                   .addReg(DLX::PC)
                                   .addImm(16),
                               STI);

  // Push rca onto the stack.
  //   st %rca, [--%sp]
  OutStreamer->EmitInstruction(MCInstBuilder(DLX::SW_RI)
                                   .addReg(DLX::RCA)
                                   .addReg(DLX::SP)
                                   .addImm(-4)
                                   .addImm(LPAC::makePreOp(LPAC::ADD)),
                               STI);

  // Lower the call instruction.
  if (MI->getOpcode() == DLX::CALL) {
    MCInst TmpInst;
    MCInstLowering.Lower(MI, TmpInst);
    TmpInst.setOpcode(DLX::BT);
    OutStreamer->EmitInstruction(TmpInst, STI);
  } else {
    OutStreamer->EmitInstruction(MCInstBuilder(DLX::ADD_R)
                                     .addReg(DLX::PC)
                                     .addReg(MI->getOperand(0).getReg())
                                     .addReg(DLX::R0)
                                     .addImm(LPCC::ICC_T),
                                 STI);
  }*/
}

void DLXAsmPrinter::customEmitInstruction(const MachineInstr *MI) {
  DLXMCInstLower MCInstLowering(OutContext, *this);
  MCSubtargetInfo STI = getSubtargetInfo();
  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);
  OutStreamer->EmitInstruction(TmpInst, STI);
}

void DLXAsmPrinter::EmitInstruction(const MachineInstr *MI) {
  MachineBasicBlock::const_instr_iterator I = MI->getIterator();
  MachineBasicBlock::const_instr_iterator E = MI->getParent()->instr_end();

  do {
    if (I->isCall()) {
      emitCallInstruction(&*I);
      continue;
    }

    customEmitInstruction(&*I);
  } while ((++I != E) && I->isInsideBundle());
}

// isBlockOnlyReachableByFallthough - Return true if the basic block has
// exactly one predecessor and the control transfer mechanism between
// the predecessor and this block is a fall-through.
// FIXME: could the overridden cases be handled in AnalyzeBranch?
bool DLXAsmPrinter::isBlockOnlyReachableByFallthrough(
    const MachineBasicBlock *MBB) const {
  // The predecessor has to be immediately before this block.
  const MachineBasicBlock *Pred = *MBB->pred_begin();

  // If the predecessor is a switch statement, assume a jump table
  // implementation, so it is not a fall through.
  if (const BasicBlock *B = Pred->getBasicBlock())
    if (isa<SwitchInst>(B->getTerminator()))
      return false;

  // Check default implementation
  if (!AsmPrinter::isBlockOnlyReachableByFallthrough(MBB))
    return false;

  // Otherwise, check the last instruction.
  // Check if the last terminator is an unconditional branch.
  MachineBasicBlock::const_iterator I = Pred->end();
  while (I != Pred->begin() && !(--I)->isTerminator()) {
  }

  return !I->isBarrier();
}

// Force static initialization.
extern "C" void LLVMInitializeDLXAsmPrinter() {
  RegisterAsmPrinter<DLXAsmPrinter> X(getTheDLXTarget());
}
