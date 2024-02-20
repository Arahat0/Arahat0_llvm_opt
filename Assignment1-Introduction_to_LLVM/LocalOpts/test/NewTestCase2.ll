; ModuleID = './test/TestCase2.ll'
source_filename = "./test/TestCase2.ll"

@.str = private unnamed_addr constant [31 x i8] c"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\0A\00", align 1

define dso_local void @foo(i32 noundef %0) {
  %2 = shl i32 %0, 4
  %3 = icmp ne i32 %0, 0
  br i1 %3, label %4, label %7

4:                                                ; preds = %1
  %5 = mul nsw i32 %2, %0
  %6 = sdiv i32 %5, %0
  br label %9

7:                                                ; preds = %1
  %8 = sdiv i32 0, 10
  br label %9

9:                                                ; preds = %7, %4
  %.03 = phi i32 [ %5, %4 ], [ 0, %7 ]
  %.02 = phi i32 [ %6, %4 ], [ 0, %7 ]
  %.01 = phi i32 [ 0, %4 ], [ %8, %7 ]
  %.0 = phi i32 [ 0, %4 ], [ 54, %7 ]
  %10 = ashr i32 %2, 4
  %11 = sdiv i32 %.0, 54
  %12 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %0, i32 noundef %2, i32 noundef %.03, i32 noundef %.02, i32 noundef %.01, i32 noundef %.0, i32 noundef %10, i32 noundef %11, i32 noundef %.01, i32 noundef %11)
  ret void
}

declare i32 @printf(ptr noundef, ...)
