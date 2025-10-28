extern _printf
extern _exit
global _main

.data
stack:      times 1024 dd 0       ; 栈空间
sp:         dd 0                  ; 栈指针
reg1:       dd 100                ; R1 = 100

.text
BITS 32

_main:
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov ebx, [reg1]                ; 加载R1
    mov [stack + eax], ebx         ; 存储到栈中
    inc dword [sp]                 ; 栈指针加1
    
    ; 正常退出
    push dword 0
    call _exit