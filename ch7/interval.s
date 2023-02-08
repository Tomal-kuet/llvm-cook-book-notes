	.text
	.file	"interval.c"
	.globl	donothing                       # -- Begin function donothing
	.p2align	4, 0x90
	.type	donothing,@function
donothing:                              # @donothing
# %bb.0:                                # %entry
	movl	%edi, -4(%rsp)
	retq
.Lfunc_end0:
	.size	donothing, .Lfunc_end0-donothing
                                        # -- End function
	.globl	func                            # -- Begin function func
	.p2align	4, 0x90
	.type	func,@function
func:                                   # @func
# %bb.0:                                # %entry
	subq	$24, %rsp
	movl	%edi, 20(%rsp)
	movl	$5, (%rsp)
	movl	(%rsp), %edi
	callq	donothing
	movl	(%rsp), %eax
	movl	%eax, 8(%rsp)
	movl	8(%rsp), %edi
	callq	donothing
	movl	$9, (%rsp)
	cmpl	$5, 20(%rsp)
	jge	.LBB1_2
# %bb.1:                                # %if.then
	movl	$3, 4(%rsp)
	movl	4(%rsp), %edi
	callq	donothing
	movl	4(%rsp), %eax
	movl	%eax, 16(%rsp)
	movl	16(%rsp), %edi
	callq	donothing
	jmp	.LBB1_3
.LBB1_2:                                # %if.else
	movl	(%rsp), %eax
	movl	%eax, 12(%rsp)
	movl	12(%rsp), %edi
	callq	donothing
.LBB1_3:                                # %if.end
	movl	8(%rsp), %eax
	addq	$24, %rsp
	retq
.Lfunc_end1:
	.size	func, .Lfunc_end1-func
                                        # -- End function
	.ident	"clang version 16.0.0 (https://github.com/llvm/llvm-project.git 435905ecf25ab9da0753931358414164352810f5)"
	.section	".note.GNU-stack","",@progbits
