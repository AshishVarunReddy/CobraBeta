global _start
section .text
_start:
push rbp
mov rbp, rsp
mov rax, 1
mov rdi, 1
mov rsi, m0
mov rdx, 4
syscall

section .data
m0 db "Hlo", 10
section .text
mov rax, 1
mov rdi, 1
mov rsi, m1
mov rdx, 22
syscall

section .data
m1 db "Surprise Morthrforker", 10
section .text
mov rax, 3
mov r10, rax
sub rsp, 4
mov dword [rbp-4], 3
mov r10, [rbp - 4]
mov rax, 7
mov r10, rax
mov dword [rbp-4], 7
mov r10, [rbp - 4]
exit:
mov rax, 60
mov rdi, r10
syscall
