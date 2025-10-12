global _start
section .text
_start:
push rbp
mov rbp, rsp
mov rax, 17
mov r10, rax
sub rsp, 4
mov dword [rbp-4], 17
mov rax, 1
mov r10, rax
sub rsp, 4
mov dword [rbp-8], 1
mov r10, [rbp - 4]
exit:
mov rax, 60
mov rdi, r10
syscall
