section  .data
    int_format db "%d", 10, 0  ; Format string for printf
    float_format db "%f", 10, 0  ; Format string for printf
    freg times 256 dd 0.0 
section .bss
    reg resd 8 ; Reserve space for 8 double words (4 bytes each)
section .text
    extern printf           ; Declare external printf function
    global main
main:
    push rbp                ; Save caller's base pointer
    mov rbp, rsp            ; Set current function's base pointer
    mov dword [rel reg + 4], 666  ; reg[1] = 1 (each element is 4 bytes, offset is 4)
    ; Call printf to output reg[0]
    mov rcx, int_format        ; First parameter: format string
    mov edx, dword [rel reg+4*1]  ; Second parameter: value of reg[1]
    call printf
; compiler generated code --start-----------------------------------------
    ; LOAD: R1 = 100
    mov dword [rel reg+1*4], 100           ; R1 = 100
    ; LOAD: R2 = 99
    mov dword [rel reg+2*4], 99           ; R2 = 99
    ; MOV: R3 = R1
    mov eax, [rel reg+1*4]    ; 先加载到寄存器
    mov [rel reg+3*4], eax    ; 再从寄存器存储
    ; ADD: R3 = R3 + R2
    mov eax, [rel reg+3*4]    ; 先加载到寄存器
    add eax, [rel reg+2*4]    ; 再从寄存器存储
    mov [rel reg+3*4], eax    ; 再从寄存器存储
    ; PRINT_INT:  R3 = ?
    mov rcx, int_format        ; First parameter: format string
    mov edx, [rel reg+3*4]    ; second param 
    call printf
    ; FLOAD: FR1 = 3.141593
    mov dword [rel freg+1*4], 0x40490FDA           ; FR1 = 3.141593
    ; PRINT_FLOAT:  R1 = ?
    mov rcx, float_format        ; First parameter: format string
    movss xmm0, [rel freg+1*4]    ; second param 
    cvtss2sd xmm0, xmm0  
    movq rdx, xmm0   
    call printf
; compiler generated code --end---------------------------------------
    mov dword [rel reg + 4], 999  ; reg[1] = 1 (each element is 4 bytes, offset is 4)
    ; Call printf to output reg[0]
    mov rcx, int_format        ; First parameter: format string
    mov edx, dword [rel reg+4*1]  ; Second parameter: value of reg[1]
    call printf
    ; Clean up stack and return
    mov rsp, rbp            ; Restore stack pointer
    pop rbp                 ; Restore base pointer
    mov eax, 0              ; Set return value to 0
    ret
