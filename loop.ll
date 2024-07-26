; ModuleID = 'main.c'
source_filename = "main.c"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4 ; &len
  %3 = alloca [10 x i32], align 16 ; arr
  %4 = alloca i32, align 4 ; &i
  %5 = alloca i32, align 4
  %6 = alloca i32*, align 8
  store i32 0, i32* %1, align 4
  store i32 10, i32* %2, align 4 ; len = 10;
  store i32 0, i32* %4, align 4 ; i = 0;
  br label %7

7:                                                ; preds = %16, %0
  %8 = load i32, i32* %4, align 4 ; == i
  %9 = load i32, i32* %2, align 4 ; == len
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %19

11:                                               ; preds = %7
  %12 = load i32, i32* %4, align 4 ; == i, the value to be store into arr[i];
  %13 = load i32, i32* %4, align 4 ; == i
  %14 = sext i32 %13 to i64 ; sign extend
  %15 = getelementptr inbounds [10 x i32], [10 x i32]* %3, i64 0, i64 %14 ; == arr+i
  store i32 %12, i32* %15, align 4
  br label %16

16:                                               ; preds = %11
  %17 = load i32, i32* %4, align 4
  %18 = add nsw i32 %17, 1
  store i32 %18, i32* %4, align 4
  br label %7, !llvm.loop !6

; ================= while loop ================
19:                                               ; preds = %7
  store i32 9, i32* %5, align 4
  br label %20

20:                                               ; preds = %23, %19
  %21 = load i32, i32* %5, align 4
  %22 = icmp sge i32 %21, 0
  br i1 %22, label %23, label %30

23:                                               ; preds = %20
  %24 = load i32, i32* %5, align 4
  %25 = sext i32 %24 to i64
  %26 = getelementptr inbounds [10 x i32], [10 x i32]* %3, i64 0, i64 %25
  %27 = load i32, i32* %26, align 4
  store i32 %27, i32* %5, align 4
  %28 = load i32, i32* %5, align 4
  %29 = add nsw i32 %28, -1
  store i32 %29, i32* %5, align 4
  br label %20, !llvm.loop !8

30:                                               ; preds = %20
  store i32* %5, i32** %6, align 8
  %31 = load i32*, i32** %6, align 8
  store i32 1, i32* %31, align 4
  ret i32 0
}
