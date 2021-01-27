	.text
	.file	"test.c"
	.globl	fibbonacci                      # -- Begin function fibbonacci
	.p2align	4, 0x90
	.type	fibbonacci,@function
fibbonacci:                             # @fibbonacci
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -16
	movl	%edi, 12(%rsp)
	testl	%edi, %edi
	je	.LBB0_1
# %bb.3:                                # %if-false
	cmpl	$1, 12(%rsp)
	jne	.LBB0_5
# %bb.4:                                # %if-true3
	movl	$1, %eax
	jmp	.LBB0_2
.LBB0_1:                                # %if-true
	xorl	%eax, %eax
	jmp	.LBB0_2
.LBB0_5:                                # %if-false6
	movl	12(%rsp), %edi
	decl	%edi
	callq	fibbonacci
	movl	%eax, %ebx
	movl	12(%rsp), %edi
	addl	$-2, %edi
	callq	fibbonacci
	addl	%ebx, %eax
.LBB0_2:                                # %if-true
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	fibbonacci, .Lfunc_end0-fibbonacci
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	%edi, 20(%rsp)
	movq	%rsi, 8(%rsp)
	movq	8(%rsp), %rax
	movq	8(%rax), %rdi
	callq	atoi
	movl	%eax, %edi
	callq	fibbonacci
	movl	$.L__unnamed_1, %edi
	movl	%eax, %esi
	callq	printf
	xorl	%eax, %eax
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.L__unnamed_1,@object           # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"%d\n"
	.size	.L__unnamed_1, 4

	.section	".note.GNU-stack","",@progbits
