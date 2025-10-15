global _start
section .text
_start:
push rbp
mov rbp, rsp
mov rax, 3
mov r10, rax
sub rsp, 4
mov dword [rbp-4], 3
mov rax, 2
mov r10, rax
sub rsp, 4
mov dword [rbp-8], 2
mov rax, 4
mov r10, rax
sub rsp, 4
mov dword [rbp-12], 4
mov rax, 3
mov r10, rax
sub rsp, 4
mov dword [rbp-16], 3
mov rax, 6
mov r10, rax
sub rsp, 4
mov dword [rbp-20], 6
mov r10, [rbp - 20]
exit:
mov rax, 60
mov rdi, r10
syscall
