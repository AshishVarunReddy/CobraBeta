global _start
section .text
_start:
mov rax, 0
sub rax, 3
mov rax, 0
sub rax, 3
mov rax, -3
sub rax, 2
mov rax, 20
mov r10, -5
idiv r10
mov rax, -3
sub rax, -4
mov rax, 60
mov r10, 1
idiv r10
push rax
exit:
mov rax, 60
pop rdi
syscall
