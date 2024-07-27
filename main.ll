; ModuleID = 'main.c'
source_filename = "main.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @add(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  store i32 %1, i32* %4, align 4
  %5 = load i32, i32* %3, align 4
  %6 = load i32, i32* %4, align 4
  %7 = add nsw i32 %5, %6
  ret i32 %7
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @sub(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  store i32 %1, i32* %4, align 4
  %5 = load i32, i32* %3, align 4
  %6 = load i32, i32* %4, align 4
  %7 = sub nsw i32 %5, %6
  ret i32 %7
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @equal(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  store i32 %1, i32* %4, align 4
  %5 = load i32, i32* %3, align 4
  %6 = load i32, i32* %4, align 4
  %7 = icmp eq i32 %5, %6
  %8 = zext i1 %7 to i32
  ret i32 %8
}

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
  %11 = alloca [5 x i32], align 16
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  %14 = alloca i32, align 4
  %15 = alloca [2 x [3 x i32]], align 16
  %16 = alloca i32, align 4
  %17 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 10, i32* %2, align 4
  store i32 -1, i32* %3, align 4
  %18 = load i32, i32* %2, align 4
  %19 = load i32, i32* %2, align 4
  %20 = call i32 @add(i32 noundef %18, i32 noundef %19)
  %21 = load i32, i32* %2, align 4
  %22 = load i32, i32* %3, align 4
  %23 = sub nsw i32 %21, %22
  store i32 %23, i32* %4, align 4
  %24 = load i32, i32* %2, align 4
  %25 = load i32, i32* %3, align 4
  %26 = mul nsw i32 %24, %25
  store i32 %26, i32* %5, align 4
  %27 = load i32, i32* %2, align 4
  %28 = load i32, i32* %3, align 4
  %29 = add nsw i32 %28, 4
  %30 = sdiv i32 %27, %29
  store i32 %30, i32* %6, align 4
  %31 = load i32, i32* %2, align 4
  %32 = load i32, i32* %3, align 4
  %33 = add nsw i32 %32, 4
  %34 = srem i32 %31, %33
  store i32 %34, i32* %7, align 4
  %35 = load i32, i32* %2, align 4
  %36 = load i32, i32* %3, align 4
  %37 = icmp slt i32 %35, %36
  br i1 %37, label %38, label %42

38:                                               ; preds = %0
  %39 = load i32, i32* %3, align 4
  %40 = load i32, i32* %2, align 4
  %41 = call i32 @sub(i32 noundef %39, i32 noundef %40)
  br label %55

42:                                               ; preds = %0
  %43 = load i32, i32* %2, align 4
  %44 = load i32, i32* %3, align 4
  %45 = icmp sgt i32 %43, %44
  br i1 %45, label %50, label %46

46:                                               ; preds = %42
  %47 = load i32, i32* %2, align 4
  %48 = load i32, i32* %3, align 4
  %49 = icmp eq i32 %47, %48
  br i1 %49, label %50, label %54

50:                                               ; preds = %46, %42
  %51 = load i32, i32* %2, align 4
  %52 = load i32, i32* %3, align 4
  %53 = call i32 @sub(i32 noundef %51, i32 noundef %52)
  br label %54

54:                                               ; preds = %50, %46
  br label %55

55:                                               ; preds = %54, %38
  %56 = load i32, i32* %2, align 4
  %57 = load i32, i32* %3, align 4
  %58 = icmp eq i32 %56, %57
  br i1 %58, label %59, label %63

59:                                               ; preds = %55
  %60 = load i32, i32* %2, align 4
  %61 = load i32, i32* %3, align 4
  %62 = call i32 @equal(i32 noundef %60, i32 noundef %61)
  br label %63

63:                                               ; preds = %59, %55
  store i32 0, i32* %8, align 4
  store i32 10, i32* %9, align 4
  br label %64

64:                                               ; preds = %67, %63
  %65 = load i32, i32* %8, align 4
  %66 = icmp slt i32 %65, 10
  br i1 %66, label %67, label %70

67:                                               ; preds = %64
  %68 = load i32, i32* %8, align 4
  %69 = add nsw i32 %68, 1
  store i32 %69, i32* %8, align 4
  br label %64, !llvm.loop !6

70:                                               ; preds = %64
  store i32 0, i32* %8, align 4
  store i32 0, i32* %10, align 4
  br label %71

71:                                               ; preds = %77, %70
  %72 = load i32, i32* %10, align 4
  %73 = icmp slt i32 %72, 20
  br i1 %73, label %74, label %80

74:                                               ; preds = %71
  %75 = load i32, i32* %10, align 4
  %76 = sub nsw i32 %75, 1
  store i32 %76, i32* %10, align 4
  br label %77

77:                                               ; preds = %74
  %78 = load i32, i32* %10, align 4
  %79 = add nsw i32 %78, 2
  store i32 %79, i32* %10, align 4
  br label %71, !llvm.loop !8

80:                                               ; preds = %71
  store i32 0, i32* %12, align 4
  br label %81

81:                                               ; preds = %89, %80
  %82 = load i32, i32* %12, align 4
  %83 = icmp slt i32 %82, 5
  br i1 %83, label %84, label %92

84:                                               ; preds = %81
  %85 = load i32, i32* %12, align 4
  %86 = load i32, i32* %12, align 4
  %87 = sext i32 %86 to i64
  %88 = getelementptr inbounds [5 x i32], [5 x i32]* %11, i64 0, i64 %87
  store i32 %85, i32* %88, align 4
  br label %89

89:                                               ; preds = %84
  %90 = load i32, i32* %12, align 4
  %91 = add nsw i32 %90, 1
  store i32 %91, i32* %12, align 4
  br label %81, !llvm.loop !9

92:                                               ; preds = %81
  store i32 0, i32* %13, align 4
  br label %93

93:                                               ; preds = %102, %92
  %94 = load i32, i32* %13, align 4
  %95 = icmp slt i32 %94, 5
  br i1 %95, label %96, label %105

96:                                               ; preds = %93
  %97 = load i32, i32* %8, align 4
  %98 = sext i32 %97 to i64
  %99 = getelementptr inbounds [5 x i32], [5 x i32]* %11, i64 0, i64 %98
  %100 = load i32, i32* %99, align 4
  %101 = add nsw i32 %100, 1
  store i32 %101, i32* %14, align 4
  br label %102

102:                                              ; preds = %96
  %103 = load i32, i32* %13, align 4
  %104 = add nsw i32 %103, 1
  store i32 %104, i32* %13, align 4
  br label %93, !llvm.loop !10

105:                                              ; preds = %93
  store i32 0, i32* %16, align 4
  br label %106

106:                                              ; preds = %128, %105
  %107 = load i32, i32* %16, align 4
  %108 = icmp slt i32 %107, 2
  br i1 %108, label %109, label %131

109:                                              ; preds = %106
  store i32 0, i32* %17, align 4
  br label %110

110:                                              ; preds = %124, %109
  %111 = load i32, i32* %17, align 4
  %112 = icmp slt i32 %111, 3
  br i1 %112, label %113, label %127

113:                                              ; preds = %110
  %114 = load i32, i32* %16, align 4
  %115 = mul nsw i32 %114, 10
  %116 = load i32, i32* %17, align 4
  %117 = add nsw i32 %115, %116
  %118 = load i32, i32* %16, align 4
  %119 = sext i32 %118 to i64
  %120 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %15, i64 0, i64 %119
  %121 = load i32, i32* %17, align 4
  %122 = sext i32 %121 to i64
  %123 = getelementptr inbounds [3 x i32], [3 x i32]* %120, i64 0, i64 %122
  store i32 %117, i32* %123, align 4
  br label %124

124:                                              ; preds = %113
  %125 = load i32, i32* %17, align 4
  %126 = add nsw i32 %125, 1
  store i32 %126, i32* %17, align 4
  br label %110, !llvm.loop !11

127:                                              ; preds = %110
  br label %128

128:                                              ; preds = %127
  %129 = load i32, i32* %16, align 4
  %130 = add nsw i32 %129, 1
  store i32 %130, i32* %16, align 4
  br label %106, !llvm.loop !12

131:                                              ; preds = %106
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
