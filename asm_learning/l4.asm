global _start
section .data
msg db "this is bring printed using a function", 10
len equ $ - msg
section .text
print:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, len
    syscall
    ret

_start:
    call print
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
