; ModuleID = 'sample.c'
source_filename = "sample.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @foo(i32 noundef %n, i32 noundef %m) #0 {
entry:
  %n.addr = alloca i32, align 4
  %m.addr = alloca i32, align 4
  %sum = alloca i32, align 4
  %c0 = alloca i32, align 4
  %c1 = alloca i32, align 4
  store i32 %n, ptr %n.addr, align 4
  store i32 %m, ptr %m.addr, align 4
  store i32 0, ptr %sum, align 4
  %0 = load i32, ptr %n.addr, align 4
  store i32 %0, ptr %c0, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc5, %entry
  %1 = load i32, ptr %c0, align 4
  %cmp = icmp sgt i32 %1, 0
  br i1 %cmp, label %for.body, label %for.end7

for.body:                                         ; preds = %for.cond
  %2 = load i32, ptr %m.addr, align 4
  store i32 %2, ptr %c1, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %3 = load i32, ptr %c1, align 4
  %cmp2 = icmp sgt i32 %3, 0
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %4 = load i32, ptr %c0, align 4
  %5 = load i32, ptr %c1, align 4
  %cmp4 = icmp sgt i32 %4, %5
  %6 = zext i1 %cmp4 to i64
  %cond = select i1 %cmp4, i32 1, i32 0
  %7 = load i32, ptr %sum, align 4
  %add = add nsw i32 %7, %cond
  store i32 %add, ptr %sum, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %8 = load i32, ptr %c1, align 4
  %dec = add nsw i32 %8, -1
  store i32 %dec, ptr %c1, align 4
  br label %for.cond1, !llvm.loop !6

for.end:                                          ; preds = %for.cond1
  br label %for.inc5

for.inc5:                                         ; preds = %for.end
  %9 = load i32, ptr %c0, align 4
  %dec6 = add nsw i32 %9, -1
  store i32 %dec6, ptr %c0, align 4
  br label %for.cond, !llvm.loop !8

for.end7:                                         ; preds = %for.cond
  %10 = load i32, ptr %sum, align 4
  ret i32 %10
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 16.0.0 (https://github.com/llvm/llvm-project.git 435905ecf25ab9da0753931358414164352810f5)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
