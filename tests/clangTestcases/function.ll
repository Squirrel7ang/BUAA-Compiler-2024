; ModuleID = 'function.c'
source_filename = "function.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@__const.main.a = private unnamed_addr constant [4 x i32] [i32 1, i32 2, i32 3, i32 4], align 16
@__const.main.b = private unnamed_addr constant [4 x i8] c"abcd", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @f1(i32 noundef %0, i8 noundef signext %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i8, align 1
  store i32 %0, ptr %3, align 4
  store i8 %1, ptr %4, align 1
  %5 = load i32, ptr %3, align 4
  %6 = load i8, ptr %4, align 1
  %7 = sext i8 %6 to i32
  %8 = add nsw i32 %5, %7
  ret i32 %8
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @f2(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store ptr %0, ptr %5, align 8
  store ptr %1, ptr %6, align 8
  store i32 %2, ptr %7, align 4
  store i32 %3, ptr %8, align 4
  store i32 0, ptr %9, align 4
  %10 = load ptr, ptr %5, align 8
  %11 = getelementptr inbounds i32, ptr %10, i64 0
  %12 = load i32, ptr %11, align 4
  %13 = load ptr, ptr %5, align 8
  %14 = getelementptr inbounds i32, ptr %13, i64 0
  %15 = load i32, ptr %14, align 4
  %16 = trunc i32 %15 to i8
  %17 = load ptr, ptr %6, align 8
  %18 = getelementptr inbounds i8, ptr %17, i64 0
  store i8 %16, ptr %18, align 1
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local signext i8 @f3() #0 {
  ret i8 121
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca [4 x i32], align 16
  %3 = alloca [4 x i8], align 1
  %4 = alloca i32, align 4
  %5 = alloca i8, align 1
  store i32 0, ptr %1, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr align 16 %2, ptr align 16 @__const.main.a, i64 16, i1 false)
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %3, ptr align 1 @__const.main.b, i64 4, i1 false)
  store i32 4, ptr %4, align 4
  store i8 97, ptr %5, align 1
  %6 = getelementptr inbounds [4 x i32], ptr %2, i64 0, i64 0
  %7 = load i32, ptr %6, align 16
  %8 = load i32, ptr %4, align 4
  %9 = trunc i32 %8 to i8
  %10 = call i32 @f1(i32 noundef %7, i8 noundef signext %9)
  %11 = load i8, ptr %5, align 1
  %12 = sext i8 %11 to i32
  %13 = add nsw i32 %12, 1
  %14 = trunc i32 %13 to i8
  %15 = call i32 @f1(i32 noundef 0, i8 noundef signext %14)
  %16 = getelementptr inbounds [4 x i32], ptr %2, i64 0, i64 0
  %17 = getelementptr inbounds [4 x i8], ptr %3, i64 0, i64 0
  %18 = load i32, ptr %4, align 4
  call void @f2(ptr noundef %16, ptr noundef %17, i32 noundef %18, i32 noundef 4)
  %19 = call signext i8 @f3()
  store i8 %19, ptr %5, align 1
  %20 = load i32, ptr %4, align 4
  %21 = call i32 @f1(i32 noundef %20, i8 noundef signext 0)
  store i32 %21, ptr %4, align 4
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
