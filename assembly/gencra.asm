global _start
section .text
_start:
push rbp
mov rbp, rsp
mov rax, 5
add rax, 3
mov r10, rax
sub rsp, 4
mov dword [rbp-4], 8
mov rax, 4
add rax, 9
mov r10, rax
sub rsp, 4
mov dword [rbp-8], 13
mov rax, 0
sub rax, 3
mov rax, -3
add rax, 5
mov r10, rax
exit:
mov rax, 60
mov rdi, r10
syscall
