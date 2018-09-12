//===- DLXCallingConv.td - Calling Conventions DLX -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the DLX architectures.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Return Value Calling Conventions
//===----------------------------------------------------------------------===//

def CC_DLX : CallingConv<[
  // Promote i8/i16 arguments to i32.
  CCIfType<[i1, i8, i16], CCPromoteToType<i32>>,

  // Integer values get stored in stack slots that are 4 bytes in
  // size and 4-byte aligned.
  CCIfType<[i32, f32], CCAssignToStack<4, 4>>
]>;

// Only the return rules are defined here for O32. The rules for argument
// passing are defined in MipsISelLowering.cpp.
def RetCC_DLX : CallingConv<[
  // Promote i1/i8/i16 return values to i32.
  CCIfType<[i1, i8, i16], CCPromoteToType<i32>>,

  // i32 are returned in registers V0, V1, A0, A1, unless the original return
  // type was a vector of floats.
  CCIfType<[i32],  CCAssignToReg<[R1]>>
]>;
