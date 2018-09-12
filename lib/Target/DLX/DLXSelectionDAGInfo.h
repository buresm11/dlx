//===-- DLXSelectionDAGInfo.h - DLX SelectionDAG Info -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the DLX subclass for TargetSelectionDAGInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DLX_DLXSELECTIONDAGINFO_H
#define LLVM_LIB_TARGET_DLX_DLXSELECTIONDAGINFO_H

#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class DLXSelectionDAGInfo : public SelectionDAGTargetInfo {
public:
  DLXSelectionDAGInfo() = default;

  SDValue EmitTargetCodeForMemcpy(SelectionDAG &DAG, const SDLoc &dl,
                                  SDValue Chain, SDValue Dst, SDValue Src,
                                  SDValue Size, unsigned Align, bool isVolatile,
                                  bool AlwaysInline,
                                  MachinePointerInfo DstPtrInfo,
                                  MachinePointerInfo SrcPtrInfo) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_DLX_DLXSELECTIONDAGINFO_H
