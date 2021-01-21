; ModuleID = 'shsh'
source_filename = "shsh"

@0 = private unnamed_addr constant [12 x i8] c"hello111 %d\00", align 1
@1 = private unnamed_addr constant [12 x i8] c"hello222 %d\00", align 1

declare i32 @printf(i8* %0, ...)

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %0 = alloca i32, align 4
  %1 = alloca i8**, align 8
  %2 = alloca i32, align 4
  store i32 %argc, i32* %2, align 4
  store i8** %argv, i8*** %1, align 8
  br label %new-block

new-block:                                        ; preds = %entry
  store i32 99, i32* %0, align 4
  br label %if-condition

if-condition:                                     ; preds = %new-block
  %3 = load i32, i32* %0, align 4
  %4 = icmp ne i32 %3, 0
  br i1 %4, label %if-true, label %if-false

if-true:                                          ; preds = %if-condition
  br label %new-block1

if-end:                                           ; preds = %new-block2, %new-block1
  ret i32 0

new-block1:                                       ; preds = %if-true
  %5 = load i32, i32* %0, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @0, i32 0, i32 0), i32 %5)
  br label %if-end

if-false:                                         ; preds = %if-condition
  br label %new-block2

new-block2:                                       ; preds = %if-false
  %7 = load i32, i32* %0, align 4
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @1, i32 0, i32 0), i32 %7)
  br label %if-end
}
