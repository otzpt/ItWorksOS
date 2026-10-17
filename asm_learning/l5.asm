global _start
section .data
msg db "idk", 10, 0
msg2 db "idk2", 10, 0
section .text
print:
    mov rdx, 0
    .again:
        cmp byte [rsi+rdx], 0
        je .done
        inc rdx
        jmp .again
    .done:
        mov rax, 1
        mov rdi, 1
        syscall
        ret
_start:
    mov rsi, msg
    call print
    mov rsi, msg2
    call print
    mov rax, 60
    mov rdi, 0
    syscall
