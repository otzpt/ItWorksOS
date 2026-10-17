; global _start
; section .data
; msg: db "hello", 10
; len equ $ - msg
; section .text
;  _start:
;     mov rax, 1
;     mov rdi, 1
;     mov rsi, msg
;     mov rdx, 3
;     syscall
;     mov rax, 60
;     mov rdi, 0
;     syscall

global _start
section .data
msg: db "hello", 10, "world", 10
len equ $ - msg
section .text
_start:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, len
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
