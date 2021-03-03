define i32 @upper(i32 %0, i32 %1) {
entry:

i32 %0
; ModuleID = 'upper.c'
source_filename = "upper.c"

define i32 @upper(i32 %a, i32 %b) {
entry:
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  store i32 %a, i32* %1, align 4
  store i32 %b, i32* %0, align 4
  br label %if-header

if-header:                                        ; preds = %entry
  %2 = load i32, i32* %1, align 4
  %3 = load i32, i32* %0, align 4
  %if-condition = icmp sge i32 %2, %3
  br i1 %if-condition, label %if-consequence, label %if-alternative

if-consequence:                                   ; preds = %if-header
  %4 = load i32, i32* %1, align 4
  ret i32 %4

if-alternative:                                   ; preds = %if-header
  %5 = load i32, i32* %0, align 4
  ret i32 %5

if-end:                                           ; preds = %DEAD_BLOCK1, %DEAD_BLOCK
  ret i32 0

DEAD_BLOCK:                                       ; No predecessors!
  br label %if-end

DEAD_BLOCK1:                                      ; No predecessors!
  br label %if-end
}
