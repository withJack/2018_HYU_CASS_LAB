9
; ModuleID = 'kernel_membuf'
source_filename = "./2DCONV/EdgeDetection.cl"
target datalayout = "e-i64:64-i128:128-v16:16-v32:32-n16:32:64"
target triple = "nvptx64-nvidia-nvcl-"

; Function Attrs: convergent noinline nounwind
define spir_kernel void @SobelOperator_kernel(float addrspace(1)* nocapture readonly, float addrspace(1)* nocapture, i32, i32) local_unnamed_addr #0 !kernel_arg_addr_space !4 !kernel_arg_access_qual !5 !kernel_arg_type !6 !kernel_arg_base_type !7 !kernel_arg_type_qual !8 {
  %5 = tail call i64 @get_global_id(i32 0) #3
  %6 = trunc i64 %5 to i32
  %7 = tail call i64 @get_global_id(i32 1) #3
  %8 = trunc i64 %7 to i32
  %9 = add nsw i32 %2, -1
  %10 = icmp sgt i32 %9, %8
  br i1 %10, label %11, label %83

; <label>:11:                                     ; preds = %4
  %12 = add nsw i32 %3, -1
  %13 = icmp sgt i32 %12, %6
  %14 = icmp sgt i32 %8, 0
  %15 = and i1 %13, %14
  %16 = icmp sgt i32 %6, 0
  %17 = and i1 %16, %15
  br i1 %17, label %18, label %83

; <label>:18:                                     ; preds = %11
  %19 = add nsw i32 %8, -1
  %20 = mul nsw i32 %19, %3
  %21 = add nsw i32 %6, -1
  %22 = add nsw i32 %20, %21
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds float, float addrspace(1)* %0, i64 %23
  %25 = load float, float addrspace(1)* %24, align 4, !tbaa !9
  %26 = add nsw i32 %20, %6
  %27 = sext i32 %26 to i64
  %28 = getelementptr inbounds float, float addrspace(1)* %0, i64 %27
  %29 = fmul float %25, 0.000000e+00
  %30 = tail call float @llvm.fmuladd.f32(float %25, float -1.000000e+00, float %29)
  %31 = add nsw i32 %6, 1
  %32 = add nsw i32 %20, %31
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds float, float addrspace(1)* %0, i64 %33
  %35 = load float, float addrspace(1)* %34, align 4, !tbaa !9
  %36 = fadd float %35, %30
  %37 = mul nsw i32 %8, %3
  %38 = add nsw i32 %37, %21
  %39 = sext i32 %38 to i64
  %40 = getelementptr inbounds float, float addrspace(1)* %0, i64 %39
  %41 = load float, float addrspace(1)* %40, align 4, !tbaa !9
  %42 = tail call float @llvm.fmuladd.f32(float %41, float -2.000000e+00, float %36)
  %43 = add nsw i32 %37, %6
  %44 = sext i32 %43 to i64
  %45 = getelementptr inbounds float, float addrspace(1)* %0, i64 %44
  %46 = load float, float addrspace(1)* %45, align 4, !tbaa !9
  %47 = tail call float @llvm.fmuladd.f32(float %46, float 0.000000e+00, float %42)
  %48 = add nsw i32 %37, %31
  %49 = sext i32 %48 to i64
  %50 = getelementptr inbounds float, float addrspace(1)* %0, i64 %49
  %51 = load float, float addrspace(1)* %50, align 4, !tbaa !9
  %52 = tail call float @llvm.fmuladd.f32(float %51, float 2.000000e+00, float %47)
  %53 = add nsw i32 %8, 1
  %54 = mul nsw i32 %53, %3
  %55 = add nsw i32 %54, %21
  %56 = sext i32 %55 to i64
  %57 = getelementptr inbounds float, float addrspace(1)* %0, i64 %56
  %58 = load float, float addrspace(1)* %57, align 4, !tbaa !9
  %59 = tail call float @llvm.fmuladd.f32(float %58, float -1.000000e+00, float %52)
  %60 = add nsw i32 %54, %6
  %61 = sext i32 %60 to i64
  %62 = getelementptr inbounds float, float addrspace(1)* %0, i64 %61
  %63 = load float, float addrspace(1)* %62, align 4, !tbaa !9
  %64 = tail call float @llvm.fmuladd.f32(float %63, float 0.000000e+00, float %59)
  %65 = add nsw i32 %54, %31
  %66 = sext i32 %65 to i64
  %67 = getelementptr inbounds float, float addrspace(1)* %0, i64 %66
  %68 = load float, float addrspace(1)* %67, align 4, !tbaa !9
  %69 = fadd float %68, %64
  %70 = fmul float %25, 2.000000e+00
  %71 = fadd float %25, %70
  %72 = fadd float %35, %71
  %73 = tail call float @llvm.fmuladd.f32(float %41, float 0.000000e+00, float %72)
  %74 = tail call float @llvm.fmuladd.f32(float %46, float 0.000000e+00, float %73)
  %75 = tail call float @llvm.fmuladd.f32(float %51, float 0.000000e+00, float %74)
  %76 = tail call float @llvm.fmuladd.f32(float %58, float -1.000000e+00, float %75)
  %77 = tail call float @llvm.fmuladd.f32(float %63, float -2.000000e+00, float %76)
  %78 = tail call float @llvm.fmuladd.f32(float %68, float -1.000000e+00, float %77)
  %79 = fmul float %78, %78
  %80 = tail call float @llvm.fmuladd.f32(float %69, float %69, float %79)
  %81 = tail call float @_Z4sqrtf(float %80) #3
  %82 = getelementptr inbounds float, float addrspace(1)* %1, i64 %44
  store float %81, float addrspace(1)* %82, align 4, !tbaa !9
  br label %83

; <label>:83:                                     ; preds = %18, %11, %4
  ret void
}

; Function Attrs: convergent
declare i64 @get_global_id(i32) local_unnamed_addr #1

; Function Attrs: nounwind readnone speculatable
declare float @llvm.fmuladd.f32(float, float, float) #2

; Function Attrs: convergent
declare float @_Z4sqrtf(float) local_unnamed_addr #1

attributes #0 = { convergent noinline nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-features"="-satom" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { convergent "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-features"="-satom" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone speculatable }
attributes #3 = { convergent nounwind }

!nvvm.annotations = !{!0}
!llvm.module.flags = !{!1}
!opencl.ocl.version = !{!2}
!llvm.ident = !{!3}

!0 = !{void (float addrspace(1)*, float addrspace(1)*, i32, i32)* @SobelOperator_kernel, !"kernel", i32 1}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 1, i32 0}
!3 = !{!"clang version 6.0.1 (tags/RELEASE_601/final)"}
!4 = !{i32 1, i32 1, i32 0, i32 0}
!5 = !{!"none", !"none", !"none", !"none"}
!6 = !{!"DATA_TYPE*", !"DATA_TYPE*", !"int", !"int"}
!7 = !{!"float*", !"float*", !"int", !"int"}
!8 = !{!"", !"", !"", !""}
!9 = !{!10, !10, i64 0}
!10 = !{!"float", !11, i64 0}
!11 = !{!"omnipotent char", !12, i64 0}
!12 = !{!"Simple C/C++ TBAA"}
