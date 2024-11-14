; ModuleID = 'array.c'
source_filename = "array.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@gbIntArrInit = dso_local global [4 x i32] [i32 0, i32 1, i32 2, i32 3], align 16
@gbCharArrInit = dso_local global [4 x i8] c"012\00", align 1
@gbConstIntArr = dso_local constant [4 x i32] [i32 4, i32 5, i32 6, i32 0], align 16
@gbConstCharArr = dso_local constant [4 x i8] c"321\00", align 1
@gbIntInit = dso_local global i32 1, align 4
@gbCharInit = dso_local global i8 97, align 1
@gbConstInt = dso_local constant i32 1, align 4
@gbConstChar = dso_local constant i8 97, align 1
@__const.main.intArrInit = private unnamed_addr constant [4 x i32] [i32 0, i32 1, i32 2, i32 3], align 16
@__const.main.charArrInit = private unnamed_addr constant [4 x i8] c"0123", align 1
@gbCharArrNonInit = dso_local global [4 x i8] zeroinitializer, align 1
@gbIntArrNonInit = dso_local global [4 x i32] zeroinitializer, align 16
@gbIntNonInit = dso_local global i32 0, align 4
@gbCharNonInit = dso_local global i8 0, align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca [6 x i32], align 16
  %3 = alloca [5 x i8], align 1
  %4 = alloca [4 x i32], align 16
  %5 = alloca [4 x i8], align 1
  store i32 0, ptr %1, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr align 16 %4, ptr align 16 @__const.main.intArrInit, i64 16, i1 false)
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %5, ptr align 1 @__const.main.charArrInit, i64 4, i1 false)
  %6 = getelementptr inbounds [6 x i32], ptr %2, i64 0, i64 0
  store i32 1, ptr %6, align 16
  %7 = getelementptr inbounds [6 x i32], ptr %2, i64 0, i64 0
  %8 = load i32, ptr %7, align 16
  %9 = sext i32 %8 to i64
  %10 = getelementptr inbounds [6 x i32], ptr %2, i64 0, i64 %9
  store i32 2, ptr %10, align 4
  %11 = getelementptr inbounds [6 x i32], ptr %2, i64 0, i64 2
  store i32 3, ptr %11, align 8
  %12 = getelementptr inbounds [6 x i32], ptr %2, i64 0, i64 3
  store i32 4, ptr %12, align 4
  store i8 48, ptr @gbCharArrNonInit, align 1
  store i8 49, ptr getelementptr inbounds ([4 x i8], ptr @gbCharArrNonInit, i64 0, i64 1), align 1
  store i8 50, ptr getelementptr inbounds ([4 x i8], ptr @gbCharArrNonInit, i64 0, i64 2), align 1
  store i8 51, ptr getelementptr inbounds ([4 x i8], ptr @gbCharArrNonInit, i64 0, i64 3), align 1
  %13 = load i32, ptr @gbConstIntArr, align 16
  %14 = getelementptr inbounds [6 x i32], ptr %2, i64 0, i64 0
  store i32 %13, ptr %14, align 16
  %15 = getelementptr inbounds [6 x i32], ptr %2, i64 0, i64 1
  %16 = load i32, ptr %15, align 4
  %17 = getelementptr inbounds [6 x i32], ptr %2, i64 0, i64 1
  store i32 %16, ptr %17, align 4
  %18 = getelementptr inbounds [4 x i8], ptr %5, i64 0, i64 2
  %19 = load i8, ptr %18, align 1
  %20 = sext i8 %19 to i32
  %21 = sub nsw i32 %20, 48
  %22 = getelementptr inbounds [6 x i32], ptr %2, i64 0, i64 2
  store i32 %21, ptr %22, align 8
  %23 = load i8, ptr @gbConstCharArr, align 1
  %24 = sext i8 %23 to i32
  %25 = getelementptr inbounds [6 x i32], ptr %2, i64 0, i64 3
  store i32 %24, ptr %25, align 4
  ret i32 0
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
