extern _printf
extern _exit
global _main

section .data
stack:      times 1024 dd 0       ; 栈空间
sp_val:     dd 0                  ; 栈指针
reg1:       dd 100                ; R1 = 100

section .text
BITS 32

_main:
    mov eax, [sp_val]              ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    add eax, stack                 ; 计算实际地址
    mov ebx, [reg1]                ; 加载R1
    mov [eax], ebx                 ; 存储到栈中
    inc dword [sp_val]             ; 栈指针加1
    
    ; 正常退出
    push dword 0
    call _exit