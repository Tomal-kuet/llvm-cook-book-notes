; ModuleID = 'interval.c'
source_filename = "interval.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone
define dso_local void @donothing(i32 noundef %a) #0 {
entry:
  %a.addr = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define dso_local i32 @func(i32 noundef %i) #0 {
entry:
  %i.addr = alloca i32, align 4
  %a = alloca i32, align 4
  %m = alloca i32, align 4
  %b = alloca i32, align 4
  %z = alloca i32, align 4
  %k = alloca i32, align 4
  store i32 %i, ptr %i.addr, align 4
  store i32 5, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  call void @donothing(i32 noundef %0)
  %1 = load i32, ptr %a, align 4
  store i32 %1, ptr %m, align 4
  %2 = load i32, ptr %m, align 4
  call void @donothing(i32 noundef %2)
  store i32 9, ptr %a, align 4
  %3 = load i32, ptr %i.addr, align 4
  %cmp = icmp slt i32 %3, 5
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i32 3, ptr %b, align 4
  %4 = load i32, ptr %b, align 4
  call void @donothing(i32 noundef %4)
  %5 = load i32, ptr %b, align 4
  store i32 %5, ptr %z, align 4
  %6 = load i32, ptr %z, align 4
  call void @donothing(i32 noundef %6)
  br label %if.end

if.else:                                          ; preds = %entry
  %7 = load i32, ptr %a, align 4
  store i32 %7, ptr %k, align 4
  %8 = load i32, ptr %k, align 4
  call void @donothing(i32 noundef %8)
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %9 = load i32, ptr %m, align 4
  ret i32 %9
}

attributes #0 = { noinline nounwind optnone "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+cx8,+mmx,+sse,+sse2,+x87" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 16.0.0 (https://github.com/llvm/llvm-project.git 435905ecf25ab9da0753931358414164352810f5)"}
