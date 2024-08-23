global _start
_start:
    mov rax, 33
    push rax
    mov rax, 60
    pop rdi
    syscall