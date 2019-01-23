	.text
	.file	"Filters.ll"
	.section	.rodata.cst4,"aM",@progbits,4
	.p2align	2               # -- Begin function MeanFilter_kernel
.LCPI0_0:
	.long	1092616192              # float 10
	.text
	.globl	MeanFilter_kernel
	.p2align	4, 0x90
	.type	MeanFilter_kernel,@function
MeanFilter_kernel:                      # @MeanFilter_kernel
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	pushq	%rax
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movl	%ecx, %r12d
	movl	%edx, %ebx
	movq	%rsi, %r14
	movq	%rdi, %r15
	xorl	%edi, %edi
	xorl	%eax, %eax
	callq	get_global_id
	movl	%eax, %r13d
	movl	$1, %edi
	xorl	%eax, %eax
	callq	get_global_id
                                        # kill: def %eax killed %eax def %rax
	addl	$-1, %ebx
	cmpl	%ebx, %eax
	jge	.LBB0_5
# %bb.1:
	testl	%r13d, %r13d
	jle	.LBB0_5
# %bb.2:
	leal	-1(%r12), %ecx
	cmpl	%ecx, %r13d
	jge	.LBB0_5
# %bb.3:
	testl	%eax, %eax
	jle	.LBB0_5
# %bb.4:
	leal	-1(%rax), %ecx
	imull	%r12d, %ecx
	leal	(%rcx,%r13), %edx
	addl	$-1, %edx
	movslq	%edx, %rdx
	movss	(%r15,%rdx,4), %xmm0    # xmm0 = mem[0],zero,zero,zero
	leal	(%rcx,%r13), %edx
	addl	$1, %edx
                                        # kill: def %ecx killed %ecx killed %rcx
	addl	%r13d, %ecx
	movslq	%ecx, %rcx
	addss	(%r15,%rcx,4), %xmm0
	movslq	%edx, %rcx
	addss	(%r15,%rcx,4), %xmm0
	movl	%eax, %ecx
	imull	%r12d, %ecx
	leal	(%rcx,%r13), %edx
	addl	$-1, %edx
	movslq	%edx, %rdx
	addss	(%r15,%rdx,4), %xmm0
	leal	(%rcx,%r13), %edx
	addl	$1, %edx
                                        # kill: def %ecx killed %ecx killed %rcx
	addl	%r13d, %ecx
	movslq	%ecx, %rcx
	movss	(%r15,%rcx,4), %xmm1    # xmm1 = mem[0],zero,zero,zero
	addss	%xmm1, %xmm1
	addss	%xmm0, %xmm1
	movslq	%edx, %rdx
	addss	(%r15,%rdx,4), %xmm1
	addl	$1, %eax
	imull	%r12d, %eax
	leal	(%rax,%r13), %edx
	addl	$-1, %edx
	movslq	%edx, %rdx
	addss	(%r15,%rdx,4), %xmm1
	movl	%eax, %edx
	addl	%r13d, %edx
	movslq	%edx, %rdx
	addss	(%r15,%rdx,4), %xmm1
	leal	(%rax,%r13), %eax
	addl	$1, %eax
	cltq
	addss	(%r15,%rax,4), %xmm1
	divss	.LCPI0_0(%rip), %xmm1
	movss	%xmm1, (%r14,%rcx,4)
.LBB0_5:
	addq	$8, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
.Lfunc_end0:
	.size	MeanFilter_kernel, .Lfunc_end0-MeanFilter_kernel
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst4,"aM",@progbits,4
	.p2align	2               # -- Begin function GaussianFilter_kernel
.LCPI1_0:
	.long	1082130432              # float 4
.LCPI1_1:
	.long	1031798784              # float 0.0625
	.text
	.globl	GaussianFilter_kernel
	.p2align	4, 0x90
	.type	GaussianFilter_kernel,@function
GaussianFilter_kernel:                  # @GaussianFilter_kernel
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	pushq	%rax
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movl	%ecx, %r12d
	movl	%edx, %ebx
	movq	%rsi, %r14
	movq	%rdi, %r15
	xorl	%edi, %edi
	xorl	%eax, %eax
	callq	get_global_id
	movl	%eax, %r13d
	movl	$1, %edi
	xorl	%eax, %eax
	callq	get_global_id
                                        # kill: def %eax killed %eax def %rax
	addl	$-1, %ebx
	cmpl	%ebx, %eax
	jge	.LBB1_5
# %bb.1:
	testl	%r13d, %r13d
	jle	.LBB1_5
# %bb.2:
	leal	-1(%r12), %ecx
	cmpl	%ecx, %r13d
	jge	.LBB1_5
# %bb.3:
	testl	%eax, %eax
	jle	.LBB1_5
# %bb.4:
	leal	-1(%rax), %ecx
	imull	%r12d, %ecx
	leal	(%rcx,%r13), %edx
	addl	$-1, %edx
	movslq	%edx, %rdx
	movl	%ecx, %esi
	addl	%r13d, %esi
	movslq	%esi, %rsi
	movss	(%r15,%rsi,4), %xmm0    # xmm0 = mem[0],zero,zero,zero
	addss	%xmm0, %xmm0
	addss	(%r15,%rdx,4), %xmm0
	leal	1(%rcx,%r13), %ecx
	movslq	%ecx, %rcx
	addss	(%r15,%rcx,4), %xmm0
	movl	%eax, %ecx
	imull	%r12d, %ecx
	leal	(%rcx,%r13), %edx
	addl	$-1, %edx
	movslq	%edx, %rdx
	movss	(%r15,%rdx,4), %xmm1    # xmm1 = mem[0],zero,zero,zero
	addss	%xmm1, %xmm1
	addss	%xmm0, %xmm1
	leal	(%rcx,%r13), %edx
	addl	$1, %edx
                                        # kill: def %ecx killed %ecx killed %rcx
	addl	%r13d, %ecx
	movslq	%ecx, %rcx
	movss	(%r15,%rcx,4), %xmm0    # xmm0 = mem[0],zero,zero,zero
	mulss	.LCPI1_0(%rip), %xmm0
	addss	%xmm1, %xmm0
	movslq	%edx, %rdx
	movss	(%r15,%rdx,4), %xmm1    # xmm1 = mem[0],zero,zero,zero
	addss	%xmm1, %xmm1
	addss	%xmm0, %xmm1
	addl	$1, %eax
	imull	%r12d, %eax
	leal	(%rax,%r13), %edx
	addl	$-1, %edx
	movslq	%edx, %rdx
	addss	(%r15,%rdx,4), %xmm1
	movl	%eax, %edx
	addl	%r13d, %edx
	movslq	%edx, %rdx
	movss	(%r15,%rdx,4), %xmm0    # xmm0 = mem[0],zero,zero,zero
	addss	%xmm0, %xmm0
	addss	%xmm1, %xmm0
	leal	1(%rax,%r13), %eax
	cltq
	addss	(%r15,%rax,4), %xmm0
	mulss	.LCPI1_1(%rip), %xmm0
	movss	%xmm0, (%r14,%rcx,4)
.LBB1_5:
	addq	$8, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
.Lfunc_end1:
	.size	GaussianFilter_kernel, .Lfunc_end1-GaussianFilter_kernel
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 3.8.0-2ubuntu4 (tags/RELEASE_380/final)"
	.section	".note.GNU-stack","",@progbits
