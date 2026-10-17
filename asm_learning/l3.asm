global _start
section .data
msg: db "hi, this is my first loop in assembly", 10
len equ $ - msg
section .text
_start:
    mov r12, 0
    cmp r12, 0
    je done
again:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, len
    syscall
    dec r12
    jnz again
    mov rax, 60
    mov rdi, 0
    syscall
done:
    mov rax, 60
    mov rdi, 0
    syscall
