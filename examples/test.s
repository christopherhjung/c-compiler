	.text
	.file	"fac.c"
	.globl	fac                             # -- Begin function fac
	.p2align	4, 0x90
	.type	fac,@function
fac:                                    # @fac
	.cfi_startproc
# %bb.0:                                # %entry
	movl	%edi, -8(%rsp)
	movl	$1, -4(%rsp)
	cmpl	$0, -8(%rsp)
	je	.LBB0_3
	.p2align	4, 0x90
.LBB0_2:                                # %while-body
                                        # =>This Inner Loop Header: Depth=1
	movl	-8(%rsp), %eax
	movl	-4(%rsp), %ecx
	imull	%eax, %ecx
	movl	%ecx, -4(%rsp)
	decl	%eax
	movl	%eax, -8(%rsp)
	cmpl	$0, -8(%rsp)
	jne	.LBB0_2
.LBB0_3:                                # %while-end
	movl	-4(%rsp), %eax
	retq
.Lfunc_end0:
	.size	fac, .Lfunc_end0-fac
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
