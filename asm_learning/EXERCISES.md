# Exercises

> AI declaration: this file was written by Claude (Sonnet 5.5, Anthropic) through Claude Code, at the project owner's request. The exercises have not been run. The solutions are to be written by the project owner.

Each exercise is one boot sector: `ex1.asm`, `ex2.asm`, and so on, in this folder.
Build and run each one as shown in LESSONS.md. Do them in order. Every one must be exactly 512 bytes (`ls -l ex1.bin`).
Write them yourself. Paste the code when you want a review. If you are stuck, ask for a hint, not the answer.

## 1. Hang

Boot, do nothing, and stay running. Pad to 512 bytes with the 0xAA55 signature.
Check: QEMU says it booted from the hard disk and does not reboot in a loop.
Question: why does the signature look like `55 AA` in the file but is written `0xAA55`? (`xxd ex1.bin | tail -n 2`)

## 2. One character

Print the single character `A` using int 0x10.
Then change it to print `Z`. Then print your initial.

## 3. A string

Store `"hello tinyOS"` with `db` and print it with a `lodsb` loop, ending on the 0 byte.
Check: you see the full text, no extra characters.

## 4. Counting

Print the characters `0` to `9` in order, using a register as a counter and a loop with `cmp` and a conditional jump.
Hint: the ASCII code for `'0'` is 48, and the digits are consecutive.

## 5. A function

Move your print loop into a function `print_str` that takes the address in `si`. Call it three times with three different strings.
You must set `sp` before the first `call`.
Question: what would happen if you forgot to set `sp`? Predict first, then try it.

## 6. Echo

Read a key with int 0x16 and print it back, forever.
Extra: when the key is Enter (ASCII 13), print a newline. In teletype mode a newline needs both 13 and 10.

## 7. Backspace

Extend exercise 6 so Backspace (ASCII 8) erases the previous character. That means moving the cursor back, printing a space, and moving back again.
Extra: do not let backspace erase past where input began.

## 8. Tiny prompt (the real goal)

Print a prompt like `> `, read a line into a buffer (use `db` or `times N db 0` for space), stop on Enter, then print the buffer back on the next line.
This is the first piece of the shell.

## Rules of thumb

- If QEMU shows garbage or reboots in a loop, the bug is almost always `org`, a missing `ds` setup, or a missing size pad.
- Debug by printing one character at a known point to see how far the code gets.
- When two instructions have different-size operands, NASM will tell you. Read the error.
