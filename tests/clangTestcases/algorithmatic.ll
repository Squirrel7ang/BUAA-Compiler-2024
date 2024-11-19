; ModuleID = 'algorithmatic.c'
source_filename = "algorithmatic.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@__const.main.arr = private unnamed_addr constant [1 x i32] [i32 12], align 4

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
  %7 = alloca i32, align 4
  %8 = alloca i8, align 1
  %9 = alloca i8, align 1
  %10 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 0, ptr %2, align 4
  %11 = load i32, ptr %2, align 4
  %12 = add nsw i32 %11, 1
  store i32 %12, ptr %3, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr align 4 %4, ptr align 4 @__const.main.arr, i64 4, i1 false)

  %13 = load i32, ptr %2, align 4
  %14 = load i32, ptr %3, align 4
  %15 = add nsw i32 %13, %14
  store i32 %15, ptr %5, align 4

  %16 = load i32, ptr %2, align 4
  %17 = load i32, ptr %3, align 4
  %18 = sub nsw i32 %16, %17
  store i32 %18, ptr %5, align 4

  %19 = load i32, ptr %2, align 4
  %20 = load i32, ptr %3, align 4
  %21 = mul nsw i32 %19, %20
  store i32 %21, ptr %5, align 4

  %22 = load i32, ptr %2, align 4
  %23 = load i32, ptr %3, align 4
  %24 = sdiv i32 %22, %23
  store i32 %24, ptr %5, align 4

  %25 = load i32, ptr %2, align 4
  %26 = load i32, ptr %3, align 4
  %27 = srem i32 %25, %26
  store i32 %27, ptr %5, align 4

  %28 = load i32, ptr %2, align 4
  %29 = sub nsw i32 0, %28
  store i32 %29, ptr %5, align 4

  %30 = load i32, ptr %2, align 4
  %31 = icmp ne i32 %30, 0
  %32 = xor i1 %31, true
  %33 = zext i1 %32 to i32
  store i32 %33, ptr %5, align 4

  %34 = load i32, ptr %5, align 4
  store i32 %34, ptr %5, align 4

  %35 = load i32, ptr %2, align 4
  %36 = icmp ne i32 %35, 0
  br i1 %36, label %37, label %40

37:                                               ; preds = %0
  %38 = load i32, ptr %3, align 4
  %39 = icmp ne i32 %38, 0
  br label %40

40:                                               ; preds = %37, %0
  %41 = phi i1 [ false, %0 ], [ %39, %37 ]
  %42 = zext i1 %41 to i32
  store i32 %42, ptr %5, align 4
  %43 = load i32, ptr %2, align 4
  %44 = icmp ne i32 %43, 0
  br i1 %44, label %48, label %45

45:                                               ; preds = %40
  %46 = load i32, ptr %3, align 4
  %47 = icmp ne i32 %46, 0
  br label %48

48:                                               ; preds = %45, %40
  %49 = phi i1 [ true, %40 ], [ %47, %45 ]
  %50 = zext i1 %49 to i32
  store i32 %50, ptr %5, align 4
  %51 = load i32, ptr %2, align 4
  %52 = load i32, ptr %3, align 4
  %53 = icmp ne i32 %51, %52
  %54 = zext i1 %53 to i32
  store i32 %54, ptr %5, align 4
  %55 = load i32, ptr %2, align 4
  %56 = load i32, ptr %3, align 4
  %57 = icmp eq i32 %55, %56
  %58 = zext i1 %57 to i32
  store i32 %58, ptr %5, align 4
  %59 = load i32, ptr %2, align 4
  %60 = load i32, ptr %3, align 4
  %61 = icmp slt i32 %59, %60
  %62 = zext i1 %61 to i32
  store i32 %62, ptr %5, align 4
  %63 = load i32, ptr %2, align 4
  %64 = load i32, ptr %3, align 4
  %65 = icmp sle i32 %63, %64
  %66 = zext i1 %65 to i32
  store i32 %66, ptr %5, align 4
  %67 = load i32, ptr %2, align 4
  %68 = load i32, ptr %3, align 4
  %69 = icmp sgt i32 %67, %68
  %70 = zext i1 %69 to i32
  store i32 %70, ptr %5, align 4
  %71 = load i32, ptr %2, align 4
  %72 = load i32, ptr %3, align 4
  %73 = icmp sge i32 %71, %72
  %74 = zext i1 %73 to i32
  store i32 %74, ptr %5, align 4
  %75 = load i32, ptr %2, align 4
  %76 = trunc i32 %75 to i8
  store i8 %76, ptr %6, align 1
  %77 = load i8, ptr %6, align 1
  %78 = sext i8 %77 to i32
  store i32 %78, ptr %2, align 4
  %79 = load i32, ptr %2, align 4
  %80 = load i32, ptr %3, align 4
  %81 = load i32, ptr %2, align 4
  %82 = mul nsw i32 %80, %81
  %83 = sub nsw i32 %79, %82
  %84 = load i32, ptr %2, align 4
  %85 = add nsw i32 %84, 1
  %86 = mul nsw i32 2, %85
  %87 = load i32, ptr %3, align 4
  %88 = sdiv i32 %86, %87
  %89 = add nsw i32 %83, %88
  store i32 %89, ptr %2, align 4
  %90 = load i32, ptr %3, align 4
  %91 = sub nsw i32 0, %90
  %92 = call i32 @f(i32 noundef %91)
  %93 = sub nsw i32 0, %92
  %94 = load i32, ptr %2, align 4
  %95 = add nsw i32 %93, %94
  %96 = getelementptr inbounds [1 x i32], ptr %4, i64 0, i64 0
  %97 = load i32, ptr %96, align 4
  %98 = add nsw i32 %95, %97
  %99 = sub nsw i32 %98, 4
  %100 = load i32, ptr %2, align 4
  %101 = add nsw i32 %100, 2
  %102 = sdiv i32 %99, %101
  store i32 %102, ptr %3, align 4
  %103 = load i32, ptr %3, align 4
  %104 = sub nsw i32 0, %103
  %105 = call i32 @f(i32 noundef %104)
  %106 = sub nsw i32 0, %105
  %107 = load i32, ptr %2, align 4
  %108 = add nsw i32 %106, %107
  %109 = getelementptr inbounds [1 x i32], ptr %4, i64 0, i64 0
  %110 = load i32, ptr %109, align 4
  %111 = add nsw i32 %108, %110
  %112 = sub nsw i32 %111, 4
  %113 = load i32, ptr %2, align 4
  %114 = add nsw i32 %113, 2
  %115 = sdiv i32 %112, %114
  store i32 %115, ptr %7, align 4
  store i8 97, ptr %8, align 1
  store i8 98, ptr %9, align 1
  %116 = load i32, ptr %2, align 4
  %117 = add nsw i32 97, %116
  %118 = trunc i32 %117 to i8
  store i8 %118, ptr %8, align 1
  %119 = load i8, ptr %8, align 1
  %120 = sext i8 %119 to i32
  %121 = sub nsw i32 %120, 97
  store i32 %121, ptr %2, align 4
  %122 = load i32, ptr %2, align 4
  %123 = add nsw i32 %122, -1
  store i32 %123, ptr %2, align 4
  %124 = trunc i32 %123 to i8
  store i8 %124, ptr %8, align 1
  %125 = getelementptr inbounds [1 x i32], ptr %4, i64 0, i64 0
  %126 = load i32, ptr %125, align 4
  %127 = add nsw i32 %126, 97
  %128 = trunc i32 %127 to i8
  store i8 %128, ptr %8, align 1
  %129 = getelementptr inbounds [1 x i32], ptr %4, i64 0, i64 0
  %130 = load i32, ptr %129, align 4
  %131 = add nsw i32 %130, 97
  %132 = getelementptr inbounds [1 x i32], ptr %4, i64 0, i64 0
  store i32 %131, ptr %132, align 4
  %133 = getelementptr inbounds [1 x i32], ptr %4, i64 0, i64 0
  %134 = load i32, ptr %133, align 4
  %135 = trunc i32 %134 to i8
  store i8 %135, ptr %9, align 1
  store i32 1, ptr %10, align 4
  %136 = load i32, ptr %2, align 4
  %137 = load i32, ptr %3, align 4
  %138 = add nsw i32 %136, %137
  %139 = load i32, ptr %3, align 4
  %140 = load i32, ptr %2, align 4
  %141 = add nsw i32 %139, %140
  %142 = icmp sle i32 %138, %141
  br i1 %142, label %143, label %148

143:                                              ; preds = %48
  %144 = load i32, ptr %2, align 4
  %145 = load i32, ptr %2, align 4
  %146 = call i32 @f(i32 noundef %145)
  %147 = icmp ne i32 %144, %146
  br i1 %147, label %153, label %148

148:                                              ; preds = %143, %48
  %149 = load i32, ptr %3, align 4
  %150 = load i32, ptr %3, align 4
  %151 = sub nsw i32 %150, 1
  %152 = icmp sgt i32 %149, %151
  br i1 %152, label %153, label %154

153:                                              ; preds = %148, %143
  store i32 0, ptr %10, align 4
  br label %154

154:                                              ; preds = %153, %148
  %155 = load i32, ptr %10, align 4
  ret i32 %155
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 18.1.3 (1ubuntu1)"}
