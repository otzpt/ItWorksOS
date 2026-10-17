global _start
section .data
msg db "hello, how are you doing", 10, 0
section .text
print:
    mov rxd, 0
    .again:
        cmp byte [rsi+rdx], 0
        je .done
        inc rdx
        jmp .again
    .done
        mov rax, 1
        mov rdi, 1
        syscall
        ret
read:
    mov rdx, 0
    .again:
        cmp byte [rsi+rdx], 0
        je .done
        inc rdx
        jmp .again
    .done
        mov rax, 0
        mov rdi, 0
        syscall
        ret
_start:
    call read
