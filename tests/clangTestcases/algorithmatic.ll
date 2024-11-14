; ModuleID = 'algorithmatic.c'
source_filename = "algorithmatic.c"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @f(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  ret i32 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca [1 x i32], align 4
  %5 = alloca i32, align 4
  %6 = alloca i8, align 1
  %7 = alloca i8, align 1
  %8 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 0, ptr %2, align 4
  %9 = load i32, ptr %2, align 4
  %10 = add nsw i32 %9, 1
  store i32 %10, ptr %3, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr align 4 %4, ptr align 4 @__const.main.arr, i64 4, i1 false)
  %11 = load i32, ptr %2, align 4
  %12 = load i32, ptr %3, align 4
  %13 = load i32, ptr %2, align 4
  %14 = mul nsw i32 %12, %13
  %15 = sub nsw i32 %11, %14
  %16 = load i32, ptr %2, align 4
  %17 = add nsw i32 %16, 1
  %18 = mul nsw i32 2, %17
  %19 = load i32, ptr %3, align 4
  %20 = sdiv i32 %18, %19
  %21 = add nsw i32 %15, %20
  store i32 %21, ptr %2, align 4
  %22 = load i32, ptr %3, align 4
  %23 = sub nsw i32 0, %22
  %24 = call i32 @f(i32 noundef %23)
  %25 = sub nsw i32 0, %24
  %26 = load i32, ptr %2, align 4
  %27 = add nsw i32 %25, %26
  %28 = getelementptr inbounds [1 x i32], ptr %4, i64 0, i64 0
  %29 = load i32, ptr %28, align 4
  %30 = add nsw i32 %27, %29
  %31 = sub nsw i32 %30, 4
  %32 = load i32, ptr %2, align 4
  %33 = add nsw i32 %32, 2
  %34 = sdiv i32 %31, %33
  store i32 %34, ptr %3, align 4
  %35 = load i32, ptr %3, align 4
  %36 = sub nsw i32 0, %35
  %37 = call i32 @f(i32 noundef %36)
  %38 = sub nsw i32 0, %37
  %39 = load i32, ptr %2, align 4
  %40 = add nsw i32 %38, %39
  %41 = getelementptr inbounds [1 x i32], ptr %4, i64 0, i64 0
  %42 = load i32, ptr %41, align 4
  %43 = add nsw i32 %40, %42
  %44 = sub nsw i32 %43, 4
  %45 = load i32, ptr %2, align 4
  %46 = add nsw i32 %45, 2
  %47 = sdiv i32 %44, %46
  store i32 %47, ptr %5, align 4
  store i8 97, ptr %6, align 1
  store i8 98, ptr %7, align 1
  %48 = load i32, ptr %2, align 4
  %49 = add nsw i32 97, %48
  %50 = trunc i32 %49 to i8
  store i8 %50, ptr %6, align 1
  %51 = load i8, ptr %6, align 1
  %52 = sext i8 %51 to i32
  %53 = sub nsw i32 %52, 97
  store i32 %53, ptr %2, align 4
  %54 = load i32, ptr %2, align 4
  %55 = add nsw i32 %54, -1
  store i32 %55, ptr %2, align 4
  %56 = trunc i32 %55 to i8
  store i8 %56, ptr %6, align 1
  %57 = getelementptr inbounds [1 x i32], ptr %4, i64 0, i64 0
  %58 = load i32, ptr %57, align 4
  %59 = add nsw i32 %58, 97
  %60 = trunc i32 %59 to i8
  store i8 %60, ptr %6, align 1
  %61 = getelementptr inbounds [1 x i32], ptr %4, i64 0, i64 0
  %62 = load i32, ptr %61, align 4
  %63 = add nsw i32 %62, 97
  %64 = getelementptr inbounds [1 x i32], ptr %4, i64 0, i64 0
  store i32 %63, ptr %64, align 4
  %65 = getelementptr inbounds [1 x i32], ptr %4, i64 0, i64 0
  %66 = load i32, ptr %65, align 4
  %67 = trunc i32 %66 to i8
  store i8 %67, ptr %7, align 1
  store i32 1, ptr %8, align 4
  %68 = load i32, ptr %2, align 4
  %69 = load i32, ptr %3, align 4
  %70 = add nsw i32 %68, %69
  %71 = load i32, ptr %3, align 4
  %72 = load i32, ptr %2, align 4
  %73 = add nsw i32 %71, %72
  %74 = icmp sle i32 %70, %73
  br i1 %74, label %75, label %80

75:                                               ; preds = %0
  %76 = load i32, ptr %2, align 4
  %77 = load i32, ptr %2, align 4
  %78 = call i32 @f(i32 noundef %77)
  %79 = icmp ne i32 %76, %78
  br i1 %79, label %85, label %80

80:                                               ; preds = %75, %0
  %81 = load i32, ptr %3, align 4
  %82 = load i32, ptr %3, align 4
  %83 = sub nsw i32 %82, 1
  %84 = icmp sgt i32 %81, %83
  br i1 %84, label %85, label %86

85:                                               ; preds = %80, %75
  store i32 0, ptr %8, align 4
  br label %86

86:                                               ; preds = %85, %80
  %87 = load i32, ptr %8, align 4
  ret i32 %87
}