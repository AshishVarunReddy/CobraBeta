global _start
section .text
_start:
push rbp
mov rbp, rsp
mov rax, 3
mov r10, rax
sub rsp, 4
mov dword [rbp-4], 3
mov rax, 4
add rax, 0
mov r10, rax
sub rsp, 4
mov dword [rbp-8], 4
mov r10, [rbp - 8]
exit:
mov rax, 60
mov rdi, r10
syscall
