; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -S -inline -functionattrs -indvars  < %s | FileCheck %s

; Check that the invalidation happens correctly and the test does not crash.
define void @f2() {
; CHECK-LABEL: @f2(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    br label [[FOR_COND:%.*]]
; CHECK:       for.cond:
; CHECK-NEXT:    br label [[FOR_COND]]
;
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.cond, %entry
  %a.0 = phi i32 [ 1, %entry ], [ 0, %for.cond ]
  call void @f1(i32 %a.0)
  br label %for.cond
}

define internal void @f1(i32 %p1) noinline {
entry:
  ret void
}
