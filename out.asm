section  .data
    format db "%d", 10, 0  ; Format string for printf
section .bss
    reg resd 8 ; Reserve space for 8 double words (4 bytes each)
section .text
    extern printf           ; Declare external printf function
    global main
main:
    push rbp                ; Save caller's base pointer
    mov rbp, rsp            ; Set current function's base pointer
    mov dword [rel reg + 4], 1  ; reg[1] = 1 (each element is 4 bytes, offset is 4)
    ; Call printf to output reg[0]
    mov rcx, format        ; First parameter: format string
    mov edx, dword [rel reg+4*1]  ; Second parameter: value of reg[1]
    call printf
    ; Clean up stack and return
    mov rsp, rbp            ; Restore stack pointer
    pop rbp                 ; Restore base pointer
    mov eax, 0              ; Set return value to 0
    ret
