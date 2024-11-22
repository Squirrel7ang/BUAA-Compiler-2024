; ModuleID = 'ifFor.c'
source_filename = "ifFor.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 0, ptr %2, align 4
  store i32 1, ptr %3, align 4
  %14 = load i32, ptr %4, align 4
  %15 = icmp ne i32 %14, 0
  br i1 %15, label %16, label %22

16:                                               ; preds = %0
  %17 = load i32, ptr %5, align 4
  %18 = icmp ne i32 %17, 0
  br i1 %18, label %19, label %22

19:                                               ; preds = %16
  %20 = load i32, ptr %6, align 4
  %21 = icmp ne i32 %20, 0
  br i1 %21, label %40, label %22

22:                                               ; preds = %19, %16, %0
  %23 = load i32, ptr %7, align 4
  %24 = icmp ne i32 %23, 0
  br i1 %24, label %25, label %31

25:                                               ; preds = %22
  %26 = load i32, ptr %8, align 4
  %27 = icmp ne i32 %26, 0
  br i1 %27, label %28, label %31

28:                                               ; preds = %25
  %29 = load i32, ptr %9, align 4
  %30 = icmp ne i32 %29, 0
  br i1 %30, label %40, label %31

31:                                               ; preds = %28, %25, %22
  %32 = load i32, ptr %10, align 4
  %33 = icmp ne i32 %32, 0
  br i1 %33, label %34, label %41

34:                                               ; preds = %31
  %35 = load i32, ptr %11, align 4
  %36 = icmp ne i32 %35, 0
  br i1 %36, label %37, label %41

37:                                               ; preds = %34
  %38 = load i32, ptr %12, align 4
  %39 = icmp ne i32 %38, 0
  br i1 %39, label %40, label %41

40:                                               ; preds = %37, %28, %19
  br label %41

41:                                               ; preds = %40, %37, %34, %31
  %42 = load i32, ptr %2, align 4
  %43 = add nsw i32 %42, 1
  %44 = load i32, ptr %3, align 4
  %45 = icmp eq i32 %43, %44
  br i1 %45, label %49, label %46

46:                                               ; preds = %41
  %47 = load i32, ptr %2, align 4
  %48 = icmp ne i32 %47, 0
  br i1 %48, label %49, label %50

49:                                               ; preds = %46, %41
  store i32 1, ptr %2, align 4
  br label %50

50:                                               ; preds = %49, %46
  %51 = load i32, ptr %2, align 4
  %52 = load i32, ptr %3, align 4
  %53 = icmp sgt i32 %51, %52
  br i1 %53, label %54, label %59

54:                                               ; preds = %50
  %55 = load i32, ptr %2, align 4
  %56 = load i32, ptr %3, align 4
  %57 = icmp eq i32 %55, %56
  br i1 %57, label %58, label %59

58:                                               ; preds = %54
  br label %60

59:                                               ; preds = %54, %50
  store i32 2, ptr %2, align 4
  br label %60

60:                                               ; preds = %59, %58
  br label %61

61:                                               ; preds = %60
  br label %62

62:                                               ; preds = %61
  store i32 0, ptr %13, align 4
  store i32 0, ptr %13, align 4
  br label %63

63:                                               ; preds = %75, %62
  %64 = load i32, ptr %13, align 4
  %65 = icmp slt i32 %64, 4
  br i1 %65, label %66, label %78

66:                                               ; preds = %63
  %67 = load i32, ptr %2, align 4
  %68 = add nsw i32 %67, 1
  store i32 %68, ptr %2, align 4
  %69 = load i32, ptr %2, align 4
  %70 = load i32, ptr %2, align 4
  %71 = sub nsw i32 %70, 1
  %72 = icmp sgt i32 %69, %71
  br i1 %72, label %73, label %74

73:                                               ; preds = %66
  br label %75

74:                                               ; preds = %66
  br label %75

75:                                               ; preds = %74, %73
  %76 = load i32, ptr %13, align 4
  %77 = add nsw i32 %76, 1
  store i32 %77, ptr %13, align 4
  br label %63, !llvm.loop !6

78:                                               ; preds = %63
  %79 = load i32, ptr %2, align 4
  %80 = sub nsw i32 %79, 6
  ret i32 %80
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
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
