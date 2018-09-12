//===- DLXSubtarget.cpp - DLX Subtarget Information -----------*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the DLX specific subclass of TargetSubtarget.
//
//===----------------------------------------------------------------------===//

#include "DLXSubtarget.h"

#include "DLX.h"

#define DEBUG_TYPE "DLX-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "DLXGenSubtargetInfo.inc"

using namespace llvm;

void DLXSubtarget::initSubtargetFeatures(StringRef CPU, StringRef FS) {
  std::string CPUName = CPU;
  if (CPUName.empty())
    CPUName = "generic";

  ParseSubtargetFeatures(CPUName, FS);
}

DLXSubtarget &DLXSubtarget::initializeSubtargetDependencies(StringRef CPU,
                                                                StringRef FS) {
  initSubtargetFeatures(CPU, FS);
  return *this;
}

DLXSubtarget::DLXSubtarget(const Triple &TargetTriple, StringRef Cpu,
                               StringRef FeatureString, const TargetMachine &TM,
                               const TargetOptions & /*Options*/,
                               CodeModel::Model /*CodeModel*/,
                               CodeGenOpt::Level /*OptLevel*/)
    : DLXGenSubtargetInfo(TargetTriple, Cpu, FeatureString),
      FrameLowering(initializeSubtargetDependencies(Cpu, FeatureString)),
      InstrInfo(), TLInfo(TM, *this), TSInfo() {}
