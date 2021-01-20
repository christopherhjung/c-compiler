; ModuleID = 'fac.c'
source_filename = "fac.c"

define i32 @fac(i32 %n) {
entry:
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  store i32 %n, i32* %1, align 4
  store i32 1, i32* %0, align 4
  br label %while-header

while-header:                                     ; preds = %while-body, %entry
  %while-cond-n-load = load i32, i32* %1, align 4
  %while-condition = icmp ne i32 %while-cond-n-load, 0
  br i1 %while-condition, label %while-body, label %while-end

while-body:                                       ; preds = %while-header
  %2 = load i32, i32* %0, align 4
  %3 = load i32, i32* %1, align 4
  %4 = mul i32 %2, %3
  store i32 %4, i32* %0, align 4
  %5 = load i32, i32* %1, align 4
  %6 = sub i32 %5, 1
  store i32 %6, i32* %1, align 4
  br label %while-header

while-end:                                        ; preds = %while-header
  %7 = load i32, i32* %0, align 4
  ret i32 %7

DEAD_BLOCK:                                       ; No predecessors!
  ret i32 0
}
