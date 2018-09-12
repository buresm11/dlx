//===- DLX.td - Describe the DLX Target Machine --------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "DLXSchedule.td"
include "DLXRegisterInfo.td"
include "DLXCallingConv.td"
include "DLXInstrInfo.td"

def DLXInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// DLX processors supported.
//===----------------------------------------------------------------------===//

def : ProcessorModel<"generic", DLXSchedModel, []>;
def : ProcessorModel<"v11", DLXSchedModel, []>;

def DLXInstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def DLX : Target {
  // Pull in Instruction Info:
  let InstructionSet = DLXInstrInfo;
  let AssemblyWriters = [DLXInstPrinter];
}
