# x86 assembly basics (16-bit real mode, NASM)

> AI declaration: this file was written by Claude (Sonnet 5.5, Anthropic) through Claude Code, at the project owner's request. The code examples and BIOS interrupt details were written from memory and have not been run or checked against a reference. Verify them before relying on them.

Everything here runs in a boot sector under QEMU. Exercises are in EXERCISES.md.

## Build and run

    nasm -f bin file.asm -o file.bin
    qemu-system-x86_64 -drive format=raw,file=file.bin

`-f bin` means "output raw bytes, no executable format". A boot sector is raw bytes.

## 1. Registers

Registers are the CPU's variables. In 16-bit mode you use these:

| Register | Usual use |
|----------|-----------|
| ax, bx, cx, dx | general purpose, 16 bits each |
| al, ah | low and high byte of ax (same for b, c, d) |
| si, di | source and destination index, used for strings and memory |
| sp | stack pointer |
| ip | instruction pointer (next instruction), you never write it directly |
| ds, es, ss, cs | segment registers, select a 64 KB window of memory |

`ax` is 16 bits. `al` is its low 8 bits. `ah` is its high 8 bits. Writing `al` changes `ax`.

## 2. Instructions

Format: `instruction destination, source`. The destination comes first.

    mov ax, 5        ; ax = 5
    mov bx, ax       ; bx = ax
    add ax, 3        ; ax = ax + 3
    sub ax, 1        ; ax = ax - 1
    inc cx           ; cx = cx + 1
    dec cx           ; cx = cx - 1

A semicolon starts a comment.

Constraints you will hit:
- You cannot move memory to memory in one instruction. Go through a register.
- The two operands must be the same size (`mov al, bx` is an error).
- You cannot `mov` an immediate value into a segment register. Go through ax.

## 3. Labels and jumps

A label names an address:

    start:
        dec cx
        jnz start    ; jump back if the result was not zero

`cmp a, b` subtracts b from a, throws away the result, and sets flags. A conditional jump then reads the flags:

| Jump | Meaning |
|------|---------|
| je / jz | equal / zero |
| jne / jnz | not equal / not zero |
| jl / jg | less / greater (signed) |
| jb / ja | below / above (unsigned) |
| jmp | always |

`jmp $` jumps to the current line, which is an infinite loop.

## 4. Data

    msg: db "Hello", 0       ; bytes in memory, 0 terminates the string
    num: dw 1234             ; one 16-bit word
    mov al, [num]            ; brackets mean "read memory at this address"
    mov ax, msg              ; no brackets means "the address itself"

Use `[bx]`, `[si]` or `[label]` to read memory. Brackets are the main thing to get right.

## 5. Where the code lives: org and segments

The BIOS loads your sector at address 0x7C00. Memory addresses in real mode are `segment * 16 + offset`. Start your program with:

    org 0x7C00       ; assemble labels as if the code lives at 0x7C00
    xor ax, ax       ; ax = 0
    mov ds, ax       ; data segment 0, so [label] means physical 0x7C00 + label
    mov es, ax

`xor ax, ax` is the usual way to zero a register.

## 6. Printing with the BIOS

BIOS services are called with `int`. The video service is `int 0x10`.

Teletype output, one character:

    mov ah, 0x0E     ; function: teletype
    mov al, 'A'      ; the character
    int 0x10

Keyboard read, waits for a key:

    mov ah, 0x00
    int 0x16         ; the ASCII code of the key is now in al

These only work in real mode. Once we switch to protected mode they are gone, and we write to video memory ourselves.

## 7. Strings

`lodsb` does two things: `al = [ds:si]` and then `si = si + 1`. A print loop uses it:

    mov si, msg
    .next:
        lodsb
        test al, al       ; sets zero flag if al == 0
        jz .done
        mov ah, 0x0E
        int 0x10
        jmp .next
    .done:

Labels that start with a dot are local to the label above them.

## 8. The stack, call and ret

    mov sp, 0x7C00    ; the stack grows downward from below your code
    push ax           ; sp -= 2, store ax
    pop ax            ; load ax, sp += 2
    call print_str    ; push the return address, jump
    ret               ; pop the return address into ip

Set `sp` yourself before using `push` or `call`. The BIOS does not guarantee a useful value.

## 9. Ending a boot sector

    times 510 - ($ - $$) db 0     ; pad to 510 bytes
    dw 0xAA55                      ; boot signature, bytes 0x55 then 0xAA

`$` is the current address. `$$` is the start of the section. The file must be exactly 512 bytes.

## 10. Stopping the CPU

    cli          ; disable interrupts
    hlt          ; stop until an interrupt arrives
    jmp $        ; if hlt ever wakes up, spin

## Reading

- NASM manual: https://www.nasm.us/doc/
- Ralf Brown's interrupt list (int 0x10, 0x16 details): https://www.ctyme.com/rbrown.htm
- OSDev wiki, Real Mode: https://wiki.osdev.org/Real_Mode
