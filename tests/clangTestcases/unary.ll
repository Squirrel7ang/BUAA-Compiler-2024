; ModuleID = 'unary.c'
source_filename = "unary.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i8, align 1
  store i32 0, ptr %1, align 4
  %5 = load i32, ptr %1, align 4
  %6 = icmp ne i32 %5, 0
  %7 = xor i1 %6, true
  %8 = zext i1 %7 to i32
  store i32 %8, ptr %2, align 4
  %9 = load i32, ptr %2, align 4
  %10 = load i32, ptr %1, align 4
  %11 = icmp ne i32 %9, %10
  %12 = zext i1 %11 to i32
  store i32 %12, ptr %3, align 4
  %13 = load i32, ptr %2, align 4
  %14 = load i32, ptr %1, align 4
  %15 = icmp eq i32 %13, %14
  %16 = zext i1 %15 to i32
  store i32 %16, ptr %3, align 4
  %17 = load i32, ptr %2, align 4
  %18 = load i32, ptr %1, align 4
  %19 = icmp slt i32 %17, %18
  %20 = zext i1 %19 to i32
  store i32 %20, ptr %3, align 4
  %21 = load i32, ptr %2, align 4
  %22 = load i32, ptr %1, align 4
  %23 = icmp sle i32 %21, %22
  %24 = zext i1 %23 to i32
  store i32 %24, ptr %3, align 4
  %25 = load i32, ptr %2, align 4
  %26 = load i32, ptr %1, align 4
  %27 = icmp sgt i32 %25, %26
  %28 = zext i1 %27 to i32
  store i32 %28, ptr %3, align 4
  %29 = load i32, ptr %2, align 4
  %30 = load i32, ptr %1, align 4
  %31 = icmp sge i32 %29, %30
  %32 = zext i1 %31 to i32
  store i32 %32, ptr %3, align 4
  %33 = load i32, ptr %3, align 4
  %34 = trunc i32 %33 to i8
  store i8 %34, ptr %4, align 1
  %35 = load i8, ptr %4, align 1
  %36 = sext i8 %35 to i32
  store i32 %36, ptr %3, align 4
  %37 = load i8, ptr %4, align 1
  %38 = sext i8 %37 to i32
  %39 = load i32, ptr %1, align 4
  %40 = icmp ne i32 %38, %39
  %41 = zext i1 %40 to i32
  store i32 %41, ptr %3, align 4
  %42 = load i8, ptr %4, align 1
  %43 = sext i8 %42 to i32
  %44 = load i32, ptr %1, align 4
  %45 = icmp eq i32 %43, %44
  %46 = zext i1 %45 to i32
  store i32 %46, ptr %3, align 4
  %47 = load i8, ptr %4, align 1
  %48 = sext i8 %47 to i32
  %49 = load i32, ptr %1, align 4
  %50 = icmp slt i32 %48, %49
  %51 = zext i1 %50 to i32
  store i32 %51, ptr %3, align 4
  %52 = load i8, ptr %4, align 1
  %53 = sext i8 %52 to i32
  %54 = load i32, ptr %1, align 4
  %55 = icmp sle i32 %53, %54
  %56 = zext i1 %55 to i32
  store i32 %56, ptr %3, align 4
  %57 = load i8, ptr %4, align 1
  %58 = sext i8 %57 to i32
  %59 = load i32, ptr %1, align 4
  %60 = icmp sgt i32 %58, %59
  %61 = zext i1 %60 to i32
  store i32 %61, ptr %3, align 4
  %62 = load i8, ptr %4, align 1
  %63 = sext i8 %62 to i32
  %64 = load i32, ptr %1, align 4
  %65 = icmp sge i32 %63, %64
  %66 = zext i1 %65 to i32
  store i32 %66, ptr %3, align 4
  ret i32 0
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
