; ModuleID = 'test.updated.bc'
source_filename = "test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [13 x i8] c"Hello world\0A\00", align 1
@.str.1 = private unnamed_addr constant [8 x i8] c"x is 2\0A\00", align 1
@hello_world_string = private constant [42 x i8] c"(TEST)Number of Instruction Executed: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define i32 @main() #0 {
  %alloca.variable = alloca i32
  store i32 0, i32* %alloca.variable
  %load.variable = load i32, i32* %alloca.variable
  %add.variable = add i32 %load.variable, 1
  store i32 %add.variable, i32* %alloca.variable
  %1 = alloca i32, align 4
  %load.variable1 = load i32, i32* %alloca.variable
  %add.variable2 = add i32 %load.variable1, 1
  store i32 %add.variable2, i32* %alloca.variable
  %2 = alloca i32, align 4
  %load.variable3 = load i32, i32* %alloca.variable
  %add.variable4 = add i32 %load.variable3, 1
  store i32 %add.variable4, i32* %alloca.variable
  store i32 0, i32* %1, align 4
  %load.variable5 = load i32, i32* %alloca.variable
  %add.variable6 = add i32 %load.variable5, 1
  store i32 %add.variable6, i32* %alloca.variable
  store i32 3, i32* %2, align 4
  %load.variable7 = load i32, i32* %alloca.variable
  %add.variable8 = add i32 %load.variable7, 1
  store i32 %add.variable8, i32* %alloca.variable
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i32 0, i32 0))
  %load.variable9 = load i32, i32* %alloca.variable
  %add.variable10 = add i32 %load.variable9, 1
  store i32 %add.variable10, i32* %alloca.variable
  %4 = load i32, i32* %2, align 4
  %load.variable11 = load i32, i32* %alloca.variable
  %add.variable12 = add i32 %load.variable11, 1
  store i32 %add.variable12, i32* %alloca.variable
  %5 = icmp eq i32 %4, 2
  %load.variable13 = load i32, i32* %alloca.variable
  %add.variable14 = add i32 %load.variable13, 1
  store i32 %add.variable14, i32* %alloca.variable
  %6 = load i32, i32* %alloca.variable
  %printf_call_tmp = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([42 x i8], [42 x i8]* @hello_world_string, i32 0, i32 0), i32 %6)
  br i1 %5, label %7, label %10

; <label>:7:                                      ; preds = %0
  %load.variable15 = load i32, i32* %alloca.variable
  %add.variable16 = add i32 %load.variable15, 1
  store i32 %add.variable16, i32* %alloca.variable
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.1, i32 0, i32 0))
  %load.variable17 = load i32, i32* %alloca.variable
  %add.variable18 = add i32 %load.variable17, 1
  store i32 %add.variable18, i32* %alloca.variable
  store i32 7, i32* %2, align 4
  %load.variable19 = load i32, i32* %alloca.variable
  %add.variable20 = add i32 %load.variable19, 1
  store i32 %add.variable20, i32* %alloca.variable
  store i32 39, i32* %2, align 4
  %load.variable21 = load i32, i32* %alloca.variable
  %add.variable22 = add i32 %load.variable21, 1
  store i32 %add.variable22, i32* %alloca.variable
  store i32 77, i32* %2, align 4
  %load.variable23 = load i32, i32* %alloca.variable
  %add.variable24 = add i32 %load.variable23, 1
  store i32 %add.variable24, i32* %alloca.variable
  %9 = load i32, i32* %alloca.variable
  %printf_call_tmp25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([42 x i8], [42 x i8]* @hello_world_string, i32 0, i32 0), i32 %9)
  br label %10

; <label>:10:                                     ; preds = %7, %0
  %load.variable26 = load i32, i32* %alloca.variable
  %add.variable27 = add i32 %load.variable26, 1
  store i32 %add.variable27, i32* %alloca.variable
  %11 = load i32, i32* %alloca.variable
  %printf_call_tmp28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([42 x i8], [42 x i8]* @hello_world_string, i32 0, i32 0), i32 %11)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (tags/RELEASE_601/final)"}
