//===-- DLXMCAsmInfo.cpp - DLX asm properties -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the DLXMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "DLXMCAsmInfo.h"

#include "llvm/ADT/Triple.h"

using namespace llvm;

void DLXMCAsmInfo::anchor() {}

DLXMCAsmInfo::DLXMCAsmInfo(const Triple & /*TheTriple*/) {
  IsLittleEndian = false;
  PrivateGlobalPrefix = ".L";
  WeakRefDirective = "\t.weak\t";
  ExceptionsType = ExceptionHandling::DwarfCFI;

  // DLX assembly requires ".section" before ".bss"
  UsesELFSectionDirectiveForBSS = true;

  // Use the integrated assembler instead of system one.
  UseIntegratedAssembler = true;

  // Use '!' as comment string to correspond with old toolchain.
  CommentString = "!";

  // Target supports emission of debugging information.
  SupportsDebugInformation = true;

  // Set the instruction alignment. Currently used only for address adjustment
  // in dwarf generation.
  MinInstAlignment = 4;
}
