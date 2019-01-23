; ModuleID = 'hello.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@i = global i32 3, align 4
@c = global i8 97, align 1
@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  %a = alloca i8, align 1
  %b = alloca i8, align 1
  store i32 0, i32* %1, align 4
  store i8 97, i8* %a, align 1
  store i8 98, i8* %b, align 1
  store i32 100, i32* %j, align 4
  %2 = load i32, i32* @i, align 4
  %3 = load i32, i32* %j, align 4
  %4 = call i32 @func(i32 %2, i32 %3)
  store i32 %4, i32* %k, align 4
  %5 = load i8, i8* %a, align 1
  %6 = sext i8 %5 to i32
  %7 = load i8, i8* %b, align 1
  %8 = sext i8 %7 to i32
  %9 = icmp eq i32 %6, %8
  br i1 %9, label %10, label %13

; <label>:10                                      ; preds = %0
  %11 = load i32, i32* %k, align 4
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %11)
  br label %18

; <label>:13                                      ; preds = %0
  %14 = load i8, i8* %a, align 1
  %15 = load i8, i8* %b, align 1
  %16 = call i32 @func2(i8 signext %14, i8 signext %15)
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %16)
  br label %18

; <label>:18                                      ; preds = %13, %10
  ret i32 0
}

; Function Attrs: nounwind uwtable
define i32 @func(i32 %a, i32 %b) #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 %a, i32* %1, align 4
  store i32 %b, i32* %2, align 4
  %3 = load i32, i32* %1, align 4
  %4 = load i32, i32* %2, align 4
  %5 = add nsw i32 %3, %4
  ret i32 %5
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: nounwind uwtable
define i32 @func2(i8 signext %a, i8 signext %b) #0 {
  %1 = alloca i8, align 1
  %2 = alloca i8, align 1
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i8 %a, i8* %1, align 1
  store i8 %b, i8* %2, align 1
  store i32 9, i32* %i, align 4
  store i32 10, i32* %j, align 4
  %3 = load i32, i32* %i, align 4
  %4 = load i32, i32* %j, align 4
  %5 = call i32 @func(i32 %3, i32 %4)
  %6 = load i8, i8* %1, align 1
  %7 = sext i8 %6 to i32
  %8 = load i8, i8* %2, align 1
  %9 = sext i8 %8 to i32
  %10 = icmp eq i32 %7, %9
  %11 = zext i1 %10 to i32
  ret i32 %11
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0-2ubuntu4 (tags/RELEASE_380/final)"}
