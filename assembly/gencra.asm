global _start
section .text
_start:
mov rax, 0
sub rax, 4
mov rax, 4
add rax, 6
mov rax, 10
imul rax, 5
mov rax, -4
add rax, 50
push rax
mov rax, 0
sub rax, 3
mov rax, -3
add rax, 5
push rax
exit:
mov rax, 60
pop rdi
syscall
