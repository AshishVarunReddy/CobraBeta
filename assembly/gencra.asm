global _start
section .text
_start:
mov rax, 2
add rax, 3
mov rax, 5
mov r9, 4
imul r9
mov rax, 5
mov r12, 4
div r12
mov rax, rdx
mov rax, 2
add rax, 13
mov rax, 1
sub rax, 15
mov rax, 12
add rax, -14
mov rax, 20
sub rax, -2
mov r12, rax
exit:
mov rax, 60
mov rdi, r12
syscall
