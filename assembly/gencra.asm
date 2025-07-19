global _start
section .text
_start:
mov rax, 3
add rax, 4
mov rax, 3
imul rax, 4
mov rax, 7
add rax, 12
push rax
mov rax, 4
imul rax, 3
mov rax, 12
imul rax, 10
mov rax, 3
add rax, 120
push rax
exit:
mov rax, 60
pop rdi
syscall
