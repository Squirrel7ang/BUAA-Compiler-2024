; ModuleID = 'unary.c'
source_filename = "unary.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8, align 1
  store i32 0, ptr %1, align 4
  store i32 0, ptr %2, align 4

  %6 = load i32, ptr %2, align 4
  %7 = icmp ne i32 %6, 0
  %8 = xor i1 %7, true
  %9 = zext i1 %8 to i32
  store i32 %9, ptr %3, align 4

  %10 = load i32, ptr %2, align 4
  %11 = sub nsw i32 0, %10
  store i32 %11, ptr %4, align 4

  %12 = load i32, ptr %3, align 4
  %13 = load i32, ptr %2, align 4
  %14 = icmp ne i32 %12, %13
  %15 = zext i1 %14 to i32
  store i32 %15, ptr %4, align 4
  %16 = load i32, ptr %3, align 4
  %17 = load i32, ptr %2, align 4
  %18 = icmp eq i32 %16, %17
  %19 = zext i1 %18 to i32
  store i32 %19, ptr %4, align 4
  %20 = load i32, ptr %3, align 4
  %21 = load i32, ptr %2, align 4
  %22 = icmp slt i32 %20, %21
  %23 = zext i1 %22 to i32
  store i32 %23, ptr %4, align 4
  %24 = load i32, ptr %3, align 4
  %25 = load i32, ptr %2, align 4
  %26 = icmp sle i32 %24, %25
  %27 = zext i1 %26 to i32
  store i32 %27, ptr %4, align 4
  %28 = load i32, ptr %3, align 4
  %29 = load i32, ptr %2, align 4
  %30 = icmp sgt i32 %28, %29
  %31 = zext i1 %30 to i32
  store i32 %31, ptr %4, align 4
  %32 = load i32, ptr %3, align 4
  %33 = load i32, ptr %2, align 4
  %34 = icmp sge i32 %32, %33
  %35 = zext i1 %34 to i32
  store i32 %35, ptr %4, align 4
  %36 = load i32, ptr %2, align 4
  %37 = icmp ne i32 %36, 0
  br i1 %37, label %38, label %41

38:                                               ; preds = %0
  %39 = load i32, ptr %3, align 4
  %40 = icmp ne i32 %39, 0
  br label %41

41:                                               ; preds = %38, %0
  %42 = phi i1 [ false, %0 ], [ %40, %38 ]
  %43 = zext i1 %42 to i32
  store i32 %43, ptr %4, align 4
  %44 = load i32, ptr %2, align 4
  %45 = icmp ne i32 %44, 0
  br i1 %45, label %49, label %46

46:                                               ; preds = %41
  %47 = load i32, ptr %3, align 4
  %48 = icmp ne i32 %47, 0
  br label %49

49:                                               ; preds = %46, %41
  %50 = phi i1 [ true, %41 ], [ %48, %46 ]
  %51 = zext i1 %50 to i32
  store i32 %51, ptr %4, align 4
  %52 = load i32, ptr %4, align 4
  %53 = trunc i32 %52 to i8
  store i8 %53, ptr %5, align 1
  %54 = load i8, ptr %5, align 1
  %55 = sext i8 %54 to i32
  store i32 %55, ptr %4, align 4
  %56 = load i8, ptr %5, align 1
  %57 = sext i8 %56 to i32
  %58 = load i32, ptr %2, align 4
  %59 = icmp ne i32 %57, %58
  %60 = zext i1 %59 to i32
  store i32 %60, ptr %4, align 4
  %61 = load i8, ptr %5, align 1
  %62 = sext i8 %61 to i32
  %63 = load i32, ptr %2, align 4
  %64 = icmp eq i32 %62, %63
  %65 = zext i1 %64 to i32
  store i32 %65, ptr %4, align 4
  %66 = load i8, ptr %5, align 1
  %67 = sext i8 %66 to i32
  %68 = load i32, ptr %2, align 4
  %69 = icmp slt i32 %67, %68
  %70 = zext i1 %69 to i32
  store i32 %70, ptr %4, align 4
  %71 = load i8, ptr %5, align 1
  %72 = sext i8 %71 to i32
  %73 = load i32, ptr %2, align 4
  %74 = icmp sle i32 %72, %73
  %75 = zext i1 %74 to i32
  store i32 %75, ptr %4, align 4
  %76 = load i8, ptr %5, align 1
  %77 = sext i8 %76 to i32
  %78 = load i32, ptr %2, align 4
  %79 = icmp sgt i32 %77, %78
  %80 = zext i1 %79 to i32
  store i32 %80, ptr %4, align 4
  %81 = load i8, ptr %5, align 1
  %82 = sext i8 %81 to i32
  %83 = load i32, ptr %2, align 4
  %84 = icmp sge i32 %82, %83
  %85 = zext i1 %84 to i32
  store i32 %85, ptr %4, align 4
  %86 = load i32, ptr %1, align 4
  ret i32 %86
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 18.1.3 (1ubuntu1)"}
