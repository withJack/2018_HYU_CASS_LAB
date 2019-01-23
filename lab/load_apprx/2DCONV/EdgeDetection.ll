; ModuleID = 'EdgeDetection.cl'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define void @SobelOperator_kernel(float* nocapture readonly %A, float* nocapture %B, i32 %ni, i32 %nj) #0 {
  %1 = tail call i32 (i32, ...) bitcast (i32 (...)* @get_global_id to i32 (i32, ...)*)(i32 0) #3
  %2 = tail call i32 (i32, ...) bitcast (i32 (...)* @get_global_id to i32 (i32, ...)*)(i32 1) #3
  %3 = add nsw i32 %ni, -1
  %4 = icmp slt i32 %2, %3
  br i1 %4, label %5, label %75

; <label>:5                                       ; preds = %0
  %6 = add nsw i32 %nj, -1
  %7 = icmp slt i32 %1, %6
  %8 = icmp sgt i32 %2, 0
  %or.cond = and i1 %7, %8
  %9 = icmp sgt i32 %1, 0
  %or.cond3 = and i1 %9, %or.cond
  br i1 %or.cond3, label %10, label %75

; <label>:10                                      ; preds = %5
  %11 = add nsw i32 %2, -1
  %12 = mul nsw i32 %11, %nj
  %13 = add nsw i32 %1, -1
  %14 = add nsw i32 %12, %13
  %15 = sext i32 %14 to i64
  %16 = getelementptr inbounds float, float* %A, i64 %15
  %17 = load float, float* %16, align 4, !tbaa !7
  %18 = add nsw i32 %12, %1
  %19 = sext i32 %18 to i64
  %20 = getelementptr inbounds float, float* %A, i64 %19
  %21 = load float, float* %20, align 4, !tbaa !7
  %22 = fmul float %21, 0.000000e+00
  %23 = tail call float @llvm.fmuladd.f32(float -1.000000e+00, float %17, float %22)
  %24 = add nsw i32 %1, 1
  %25 = add nsw i32 %12, %24
  %26 = sext i32 %25 to i64
  %27 = getelementptr inbounds float, float* %A, i64 %26
  %28 = load float, float* %27, align 4, !tbaa !7
  %29 = tail call float @llvm.fmuladd.f32(float 1.000000e+00, float %28, float %23)
  %30 = mul nsw i32 %2, %nj
  %31 = add nsw i32 %30, %13
  %32 = sext i32 %31 to i64
  %33 = getelementptr inbounds float, float* %A, i64 %32
  %34 = load float, float* %33, align 4, !tbaa !7
  %35 = tail call float @llvm.fmuladd.f32(float -2.000000e+00, float %34, float %29)
  %36 = add nsw i32 %30, %1
  %37 = sext i32 %36 to i64
  %38 = getelementptr inbounds float, float* %A, i64 %37
  %39 = load float, float* %38, align 4, !tbaa !7
  %40 = tail call float @llvm.fmuladd.f32(float 0.000000e+00, float %39, float %35)
  %41 = add nsw i32 %30, %24
  %42 = sext i32 %41 to i64
  %43 = getelementptr inbounds float, float* %A, i64 %42
  %44 = load float, float* %43, align 4, !tbaa !7
  %45 = tail call float @llvm.fmuladd.f32(float 2.000000e+00, float %44, float %40)
  %46 = add nsw i32 %2, 1
  %47 = mul nsw i32 %46, %nj
  %48 = add nsw i32 %47, %13
  %49 = sext i32 %48 to i64
  %50 = getelementptr inbounds float, float* %A, i64 %49
  %51 = load float, float* %50, align 4, !tbaa !7
  %52 = tail call float @llvm.fmuladd.f32(float -1.000000e+00, float %51, float %45)
  %53 = add nsw i32 %47, %1
  %54 = sext i32 %53 to i64
  %55 = getelementptr inbounds float, float* %A, i64 %54
  %56 = load float, float* %55, align 4, !tbaa !7
  %57 = tail call float @llvm.fmuladd.f32(float 0.000000e+00, float %56, float %52)
  %58 = add nsw i32 %47, %24
  %59 = sext i32 %58 to i64
  %60 = getelementptr inbounds float, float* %A, i64 %59
  %61 = load float, float* %60, align 4, !tbaa !7
  %62 = tail call float @llvm.fmuladd.f32(float 1.000000e+00, float %61, float %57)
  %63 = fmul float %21, 2.000000e+00
  %64 = tail call float @llvm.fmuladd.f32(float 1.000000e+00, float %17, float %63)
  %65 = tail call float @llvm.fmuladd.f32(float 1.000000e+00, float %28, float %64)
  %66 = tail call float @llvm.fmuladd.f32(float 0.000000e+00, float %34, float %65)
  %67 = tail call float @llvm.fmuladd.f32(float 0.000000e+00, float %39, float %66)
  %68 = tail call float @llvm.fmuladd.f32(float 0.000000e+00, float %44, float %67)
  %69 = tail call float @llvm.fmuladd.f32(float -1.000000e+00, float %51, float %68)
  %70 = tail call float @llvm.fmuladd.f32(float -2.000000e+00, float %56, float %69)
  %71 = tail call float @llvm.fmuladd.f32(float -1.000000e+00, float %61, float %70)
  %72 = fmul float %71, %71
  %73 = tail call float @llvm.fmuladd.f32(float %62, float %62, float %72)
  %sqrtf = tail call float @sqrtf(float %73) #4
  %74 = getelementptr inbounds float, float* %B, i64 %37
  store float %sqrtf, float* %74, align 4, !tbaa !7
  br label %75

; <label>:75                                      ; preds = %10, %5, %0
  ret void
}

declare i32 @get_global_id(...) #1

; Function Attrs: nounwind readnone
declare float @llvm.fmuladd.f32(float, float, float) #2

declare float @sqrtf(float)

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone }
attributes #3 = { nounwind }
attributes #4 = { nounwind readnone "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!opencl.kernels = !{!0}
!llvm.ident = !{!6}

!0 = !{void (float*, float*, i32, i32)* @SobelOperator_kernel, !1, !2, !3, !4, !5}
!1 = !{!"kernel_arg_addr_space", i32 0, i32 0, i32 0, i32 0}
!2 = !{!"kernel_arg_access_qual", !"none", !"none", !"none", !"none"}
!3 = !{!"kernel_arg_type", !"DATA_TYPE*", !"DATA_TYPE*", !"int", !"int"}
!4 = !{!"kernel_arg_base_type", !"float*", !"float*", !"int", !"int"}
!5 = !{!"kernel_arg_type_qual", !"", !"", !"", !""}
!6 = !{!"clang version 3.8.0-2ubuntu4 (tags/RELEASE_380/final)"}
!7 = !{!8, !8, i64 0}
!8 = !{!"float", !9, i64 0}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C/C++ TBAA"}
