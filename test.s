	.text
	.file	"test.c"
	.globl	factorial                       # -- Begin function factorial
	.p2align	4, 0x90
	.type	factorial,@function
factorial:                              # @factorial
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -16
	movl	%edi, 12(%rsp)
	cmpl	$-1, %edi
	je	.LBB0_3
# %bb.1:                                # %if-true
	movl	$1, %eax
	jmp	.LBB0_2
.LBB0_3:                                # %if-false
	movl	12(%rsp), %ebx
	leal	-1(%rbx), %edi
	callq	factorial
	imull	%ebx, %eax
.LBB0_2:                                # %if-true
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	factorial, .Lfunc_end0-factorial
	.cfi_endproc
                                        # -- End function
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
	cmpl	$-1, %edi
	je	.LBB1_3
# %bb.1:                                # %if-true
	xorl	%eax, %eax
	jmp	.LBB1_2
.LBB1_3:                                # %if-false
	cmpl	$1, 12(%rsp)
	jne	.LBB1_5
# %bb.4:                                # %if-true3
	movl	$1, %eax
	jmp	.LBB1_2
.LBB1_5:                                # %if-false6
	movl	12(%rsp), %edi
	decl	%edi
	callq	fibbonacci
	movl	%eax, %ebx
	movl	12(%rsp), %edi
	addl	$-2, %edi
	callq	fibbonacci
	addl	%ebx, %eax
.LBB1_2:                                # %if-true
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	fibbonacci, .Lfunc_end1-fibbonacci
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$5, (%rsp)
	movl	$0, 4(%rsp)
	movl	$.L__unnamed_1, %edi
	movl	$-2, %esi
	callq	printf
	movl	(%rsp), %edi
	callq	factorial
	movl	$.L__unnamed_2, %edi
	movl	%eax, %esi
	callq	printf
	movl	(%rsp), %esi
	movl	$.L__unnamed_3, %edi
	callq	printf
	.p2align	4, 0x90
.LBB2_1:                                # %while-condition
                                        # =>This Inner Loop Header: Depth=1
	movl	4(%rsp), %eax
	cmpl	(%rsp), %eax
	jge	.LBB2_3
# %bb.2:                                # %while-body
                                        #   in Loop: Header=BB2_1 Depth=1
	movl	4(%rsp), %edi
	callq	fibbonacci
	movl	$.L__unnamed_4, %edi
	movl	%eax, %esi
	callq	printf
	incl	4(%rsp)
	jmp	.LBB2_1
.LBB2_3:                                # %while-end
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	main, .Lfunc_end2-main
	.cfi_endproc
                                        # -- End function
	.type	.L__unnamed_1,@object           # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"Factorial of %d"
	.size	.L__unnamed_1, 16

	.type	.L__unnamed_2,@object           # @1
.L__unnamed_2:
	.asciz	": %d\n"
	.size	.L__unnamed_2, 6

	.type	.L__unnamed_3,@object           # @2
.L__unnamed_3:
	.asciz	"Fibbonacci of %d: "
	.size	.L__unnamed_3, 19

	.type	.L__unnamed_4,@object           # @3
.L__unnamed_4:
	.asciz	"%d "
	.size	.L__unnamed_4, 4

	.section	".note.GNU-stack","",@progbits
