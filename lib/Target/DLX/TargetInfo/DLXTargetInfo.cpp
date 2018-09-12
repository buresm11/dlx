//===-- DLXTargetInfo.cpp - DLX Target Implementation -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

namespace llvm {
Target &getTheDLXTarget() {
  static Target TheDLXTarget;
  return TheDLXTarget;
}
} // namespace llvm

extern "C" void LLVMInitializeDLXTargetInfo() {
  RegisterTarget<Triple::dlx> X(getTheDLXTarget(), "dlx", "DLX",
                                  "DLX");
}
