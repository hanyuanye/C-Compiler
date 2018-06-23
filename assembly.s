.globl func
func:
push	%ebp
movl	%esp, %ebp
sub   $0, %esp
movl	8(%ebp), %eax
leave
ret
.globl main
main:
push	%ebp
movl	%esp, %ebp
sub   $0, %esp
movl  $2, %eax
push    %eax
movl  $3, %eax
push	%eax
call	func
pop    %ecx
imul	%ecx, %eax
leave
ret
