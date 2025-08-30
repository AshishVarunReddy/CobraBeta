global _start
section .text
_start:
push rbp
mov rbp, rsp
mov r10, rax
sub rsp, 4
mov dword [rbp-4], 5
mov rax, 3
add rax, 3
mov r10, rax
sub rsp, 4
mov dword [rbp-8], 6
mov r10, [rbp - 8]
exit:
mov rax, 60
mov rdi, r10
syscall
