//===-- DLXMachineFuctionInfo.cpp - DLX machine function info ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "DLXMachineFunctionInfo.h"

using namespace llvm;

void DLXMachineFunctionInfo::anchor() {}

unsigned DLXMachineFunctionInfo::getGlobalBaseReg() {
  // Return if it has already been initialized.

    return GlobalBaseReg;
}
