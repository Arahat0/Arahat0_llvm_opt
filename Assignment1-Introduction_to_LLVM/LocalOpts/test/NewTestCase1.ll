; ModuleID = './test/TestCase1.ll'
source_filename = "./test/TestCase1.ll"

@.str = private unnamed_addr constant [31 x i8] c"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\0A\00", align 1

define dso_local void @foo(i32 noundef %0) {
  %2 = mul nsw i32 %0, 16
  %3 = mul nsw i32 %2, %0
  %4 = sdiv i32 %3, %0
  %5 = sdiv i32 %3, 10
  %6 = mul nsw i32 54, %4
  %7 = sdiv i32 %5, 128
  %8 = sdiv i32 %6, 54
  %9 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %0, i32 noundef %2, i32 noundef %3, i32 noundef %4, i32 noundef %5, i32 noundef %6, i32 noundef %7, i32 noundef %8, i32 noundef %5, i32 noundef %8)
  ret void
}

declare i32 @printf(ptr noundef, ...)
