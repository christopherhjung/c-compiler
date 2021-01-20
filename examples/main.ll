; ModuleID = 'main.c'
source_filename = "main.c"

@Format = common global i8* null
@0 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8* %0, ...)

define i32 @main(i32 %argc, i8** %argv) {
entry:
  %0 = alloca i32, align 4
  %1 = alloca i8**, align 8
  %2 = alloca i32, align 4
  store i32 %argc, i32* %2, align 4
  store i8** %argv, i8*** %1, align 8
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i8** @Format, align 8
  store i32 1, i32* %0, align 4
  br label %while-header

while-header:                                     ; preds = %while-body, %entry
  %3 = load i32, i32* %0, align 4
  %4 = load i32, i32* %2, align 4
  %5 = icmp slt i32 %3, %4
  br i1 %5, label %while-body, label %while-end

while-body:                                       ; preds = %while-header
  %6 = load i32, i32* %0, align 4
  %7 = sext i32 %6 to i64
  %8 = load i8**, i8*** %1, align 8
  %9 = getelementptr i8*, i8** %8, i64 %7
  %10 = load i8*, i8** %9, align 8
  %11 = load i8*, i8** @Format, align 8
  %12 = call i32 (i8*, ...) @printf(i8* %11, i8* %10)
  %13 = load i32, i32* %0, align 4
  %14 = add i32 %13, 1
  store i32 %14, i32* %0, align 4
  br label %while-header

while-end:                                        ; preds = %while-header
  ret i32 0

DEAD_BLOCK:                                       ; No predecessors!
  ret i32 0
}
