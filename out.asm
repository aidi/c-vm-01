extern _printf
extern _exit
global _main

.data
fmt_int:    db 'R%d = %d\r\n', 0
fmt_float:  db 'FR%d = %f\r\n', 0
fmt_assert: db 'ASSERTION FAILED\r\n', 0
reg0:      dd 0                   ; R0
reg1:      dd 0                   ; R1
reg2:      dd 0                   ; R2
reg3:      dd 0                   ; R3
reg4:      dd 0                   ; R4
reg5:      dd 0                   ; R5
reg6:      dd 0                   ; R6
reg7:      dd 0                   ; R7
reg8:      dd 0                   ; R8
reg9:      dd 0                   ; R9
reg10:      dd 0                   ; R10
reg11:      dd 0                   ; R11
reg12:      dd 0                   ; R12
reg13:      dd 0                   ; R13
reg14:      dd 0                   ; R14
reg15:      dd 0                   ; R15
freg0:     dq 0                   ; FR0
freg1:     dq 0                   ; FR1
freg2:     dq 0                   ; FR2
freg3:     dq 0                   ; FR3
freg4:     dq 0                   ; FR4
freg5:     dq 0                   ; FR5
freg6:     dq 0                   ; FR6
freg7:     dq 0                   ; FR7
freg8:     dq 0                   ; FR8
freg9:     dq 0                   ; FR9
freg10:     dq 0                   ; FR10
freg11:     dq 0                   ; FR11
freg12:     dq 0                   ; FR12
freg13:     dq 0                   ; FR13
freg14:     dq 0                   ; FR14
freg15:     dq 0                   ; FR15
stack:      times 1024 dd 0       ; 栈空间
sp:         dd 0                  ; 栈指针
temp_addr:  dd 0                  ; 临时地址变量

.text
_main:                           ; Windows入口点
    mov dword [reg1], 100           ; R1 = 100
    mov dword [reg2], 99           ; R2 = 99
    mov eax, [reg1]                ; 加载R1
    mov [reg3], eax                ; R3 = R1
    mov eax, [reg2]                ; 加载R2
    add [reg3], eax                ; R3 += R2
    push dword [reg3]              ; 压入R3的值
    push dword 3                   ; 压入寄存器编号
    push dword fmt_int              ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 12                    ; 清理栈
    mov eax, [reg3]                ; 加载R3
    cmp eax, 199                     ; 比较R3和预期值
    je assert_int_ok_5            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_5:
    mov eax, [reg1]                ; 加载R1
    mov [reg4], eax                ; R4 = R1
    mov eax, [reg2]                ; 加载R2
    sub [reg4], eax                ; R4 -= R2
    push dword [reg4]              ; 压入R4的值
    push dword 4                   ; 压入寄存器编号
    push dword fmt_int              ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 12                    ; 清理栈
    mov eax, [reg4]                ; 加载R4
    cmp eax, 1                     ; 比较R4和预期值
    je assert_int_ok_9            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_9:
    mov eax, [reg1]                ; 加载R1
    mov [reg5], eax                ; R5 = R1
    mov eax, [reg5]                ; 加载R5
    mov ebx, [reg2]                ; 加载R2
    imul eax, ebx                  ; eax = R5 * R2
    mov [reg5], eax                ; R5 = 结果
    push dword [reg5]              ; 压入R5的值
    push dword 5                   ; 压入寄存器编号
    push dword fmt_int              ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 12                    ; 清理栈
    mov eax, [reg5]                ; 加载R5
    cmp eax, 9900                     ; 比较R5和预期值
    je assert_int_ok_13            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_13:
    mov eax, [reg1]                ; 加载R1
    mov [reg6], eax                ; R6 = R1
    mov eax, [reg6]                ; 加载R6
    mov ebx, [reg2]                ; 加载R2
    cdq                            ; 符号扩展eax到edx
    idiv ebx                       ; eax = R6 / R2
    mov [reg6], eax                ; R6 = 结果
    push dword [reg6]              ; 压入R6的值
    push dword 6                   ; 压入寄存器编号
    push dword fmt_int              ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 12                    ; 清理栈
    mov eax, [reg6]                ; 加载R6
    cmp eax, 1                     ; 比较R6和预期值
    je assert_int_ok_17            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_17:
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov ebx, [reg1]                ; 加载R1
    mov eax, [temp_addr]           ; 获取计算的地址
    mov dword [stack + eax], ebx   ; 存储到栈中
    inc dword [sp]                 ; 栈指针加1
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov ebx, [reg2]                ; 加载R2
    mov eax, [temp_addr]           ; 获取计算的地址
    mov dword [stack + eax], ebx   ; 存储到栈中
    inc dword [sp]                 ; 栈指针加1
    mov dword [reg1], 5           ; R1 = 5
    mov dword [reg2], 3           ; R2 = 3
    call add                        ; 调用函数 add
    push dword [reg0]              ; 压入R0的值
    push dword 0                   ; 压入寄存器编号
    push dword fmt_int              ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 12                    ; 清理栈
    mov eax, [reg0]                ; 加载R0
    cmp eax, 8                     ; 比较R0和预期值
    je assert_int_ok_24            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_24:
    dec dword [sp]                 ; 栈指针减1
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov eax, [temp_addr]           ; 获取计算的地址
    mov ebx, dword [stack + eax]   ; 从栈中加载值
    mov [reg2], ebx                ; 存储到R2
    dec dword [sp]                 ; 栈指针减1
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov eax, [temp_addr]           ; 获取计算的地址
    mov ebx, dword [stack + eax]   ; 从栈中加载值
    mov [reg1], ebx                ; 存储到R1
    mov eax, [reg1]                ; 加载R1
    cmp eax, 100                     ; 比较R1和预期值
    je assert_int_ok_27            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_27:
    mov eax, [reg2]                ; 加载R2
    cmp eax, 99                     ; 比较R2和预期值
    je assert_int_ok_28            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_28:
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov ebx, [reg1]                ; 加载R1
    mov eax, [temp_addr]           ; 获取计算的地址
    mov dword [stack + eax], ebx   ; 存储到栈中
    inc dword [sp]                 ; 栈指针加1
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov ebx, [reg2]                ; 加载R2
    mov eax, [temp_addr]           ; 获取计算的地址
    mov dword [stack + eax], ebx   ; 存储到栈中
    inc dword [sp]                 ; 栈指针加1
    mov dword [reg1], 10           ; R1 = 10
    mov dword [reg2], 4           ; R2 = 4
    call sub                        ; 调用函数 sub
    push dword [reg0]              ; 压入R0的值
    push dword 0                   ; 压入寄存器编号
    push dword fmt_int              ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 12                    ; 清理栈
    mov eax, [reg0]                ; 加载R0
    cmp eax, 6                     ; 比较R0和预期值
    je assert_int_ok_35            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_35:
    dec dword [sp]                 ; 栈指针减1
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov eax, [temp_addr]           ; 获取计算的地址
    mov ebx, dword [stack + eax]   ; 从栈中加载值
    mov [reg2], ebx                ; 存储到R2
    dec dword [sp]                 ; 栈指针减1
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov eax, [temp_addr]           ; 获取计算的地址
    mov ebx, dword [stack + eax]   ; 从栈中加载值
    mov [reg1], ebx                ; 存储到R1
    mov eax, [reg1]                ; 加载R1
    cmp eax, 100                     ; 比较R1和预期值
    je assert_int_ok_38            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_38:
    mov eax, [reg2]                ; 加载R2
    cmp eax, 99                     ; 比较R2和预期值
    je assert_int_ok_39            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_39:
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov ebx, [reg1]                ; 加载R1
    mov eax, [temp_addr]           ; 获取计算的地址
    mov dword [stack + eax], ebx   ; 存储到栈中
    inc dword [sp]                 ; 栈指针加1
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov ebx, [reg2]                ; 加载R2
    mov eax, [temp_addr]           ; 获取计算的地址
    mov dword [stack + eax], ebx   ; 存储到栈中
    inc dword [sp]                 ; 栈指针加1
    mov dword [reg1], 6           ; R1 = 6
    mov dword [reg2], 7           ; R2 = 7
    call mul                        ; 调用函数 mul
    push dword [reg0]              ; 压入R0的值
    push dword 0                   ; 压入寄存器编号
    push dword fmt_int              ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 12                    ; 清理栈
    mov eax, [reg0]                ; 加载R0
    cmp eax, 42                     ; 比较R0和预期值
    je assert_int_ok_46            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_46:
    dec dword [sp]                 ; 栈指针减1
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov eax, [temp_addr]           ; 获取计算的地址
    mov ebx, dword [stack + eax]   ; 从栈中加载值
    mov [reg2], ebx                ; 存储到R2
    dec dword [sp]                 ; 栈指针减1
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov eax, [temp_addr]           ; 获取计算的地址
    mov ebx, dword [stack + eax]   ; 从栈中加载值
    mov [reg1], ebx                ; 存储到R1
    mov eax, [reg1]                ; 加载R1
    cmp eax, 100                     ; 比较R1和预期值
    je assert_int_ok_49            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_49:
    mov eax, [reg2]                ; 加载R2
    cmp eax, 99                     ; 比较R2和预期值
    je assert_int_ok_50            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_50:
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov ebx, [reg1]                ; 加载R1
    mov eax, [temp_addr]           ; 获取计算的地址
    mov dword [stack + eax], ebx   ; 存储到栈中
    inc dword [sp]                 ; 栈指针加1
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov ebx, [reg2]                ; 加载R2
    mov eax, [temp_addr]           ; 获取计算的地址
    mov dword [stack + eax], ebx   ; 存储到栈中
    inc dword [sp]                 ; 栈指针加1
    mov dword [reg1], 20           ; R1 = 20
    mov dword [reg2], 5           ; R2 = 5
    call div                        ; 调用函数 div
    push dword [reg0]              ; 压入R0的值
    push dword 0                   ; 压入寄存器编号
    push dword fmt_int              ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 12                    ; 清理栈
    mov eax, [reg0]                ; 加载R0
    cmp eax, 4                     ; 比较R0和预期值
    je assert_int_ok_57            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_57:
    dec dword [sp]                 ; 栈指针减1
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov eax, [temp_addr]           ; 获取计算的地址
    mov ebx, dword [stack + eax]   ; 从栈中加载值
    mov [reg2], ebx                ; 存储到R2
    dec dword [sp]                 ; 栈指针减1
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov eax, [temp_addr]           ; 获取计算的地址
    mov ebx, dword [stack + eax]   ; 从栈中加载值
    mov [reg1], ebx                ; 存储到R1
    mov eax, [reg1]                ; 加载R1
    cmp eax, 100                     ; 比较R1和预期值
    je assert_int_ok_60            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_60:
    mov eax, [reg2]                ; 加载R2
    cmp eax, 99                     ; 比较R2和预期值
    je assert_int_ok_61            ; 如果相等，继续执行
    push dword fmt_assert           ; 打印错误信息
    call _printf                    ; 调用printf
    add esp, 4                      ; 清理栈
    push dword -1                   ; 设置错误退出码
    call _exit                      ; 调用exit
assert_int_ok_61:
    ; 处理浮点数加载: FR1 = 100.000000
    push dword 100000                  ; 推送浮点数的整数表示部分
    push dword 1000                ; 推送除数
    fild dword [esp]               ; 加载除数
    fild dword [esp+4]             ; 加载分子
    fdivp st1, st0                 ; 执行除法得到浮点数
    fstp qword [freg1]            ; 存储到FR1
    add esp, 8                     ; 清理栈
    ; 处理浮点数加载: FR2 = 99.000000
    push dword 99000                  ; 推送浮点数的整数表示部分
    push dword 1000                ; 推送除数
    fild dword [esp]               ; 加载除数
    fild dword [esp+4]             ; 加载分子
    fdivp st1, st0                 ; 执行除法得到浮点数
    fstp qword [freg2]            ; 存储到FR2
    add esp, 8                     ; 清理栈
    fld qword [freg1]             ; 加载FR1
    fstp qword [freg3]            ; 存储到FR3
    fld qword [freg2]             ; 加载FR2
    fld qword [freg3]             ; 加载FR3
    faddp st1, st0                 ; FR3 += FR2
    fstp qword [freg3]            ; 存储结果到FR3
    sub esp, 8                     ; 为浮点数参数预留空间
    fld qword [freg3]             ; 加载FR3
    fstp qword [esp]               ; 存储到栈参数
    push dword 3                   ; 压入寄存器编号
    push dword fmt_float            ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 16                    ; 清理栈
    ; 处理浮点数断言: FR3 == 199.000000
    ; 转换为整数进行比较（乘以1000取整）
    push dword 199000                ; 推送期望值的整数表示部分
    push dword 0                   ; 为实际值预留空间
    fld qword [freg3]             ; 加载FR3
    fmul dword [esp]              ; 乘以1000
    fistp dword [esp+4]           ; 转换为整数
    mov eax, [esp]                ; 加载期望值
    mov ebx, [esp+4]              ; 加载实际值
    cmp eax, ebx                  ; 比较两个整数
    je assert_float_ok_67         ; 如果相等，跳转到成功
assert_float_error_67:
    push dword fmt_assert          ; 打印错误信息
    call _printf                   ; 调用printf
    add esp, 8                     ; 清理栈
    push dword -1                  ; 设置错误退出码
    call _exit                     ; 调用exit
assert_float_ok_67:
    add esp, 8                     ; 清理栈
    fld qword [freg1]             ; 加载FR1
    fstp qword [freg4]            ; 存储到FR4
    fld qword [freg4]             ; 加载FR4
    fld qword [freg2]             ; 加载FR2
    fsubp st1, st0                 ; FR4 -= FR2
    fstp qword [freg4]            ; 存储结果到FR4
    sub esp, 8                     ; 为浮点数参数预留空间
    fld qword [freg4]             ; 加载FR4
    fstp qword [esp]               ; 存储到栈参数
    push dword 4                   ; 压入寄存器编号
    push dword fmt_float            ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 16                    ; 清理栈
    ; 处理浮点数断言: FR4 == 1.000000
    ; 转换为整数进行比较（乘以1000取整）
    push dword 1000                ; 推送期望值的整数表示部分
    push dword 0                   ; 为实际值预留空间
    fld qword [freg4]             ; 加载FR4
    fmul dword [esp]              ; 乘以1000
    fistp dword [esp+4]           ; 转换为整数
    mov eax, [esp]                ; 加载期望值
    mov ebx, [esp+4]              ; 加载实际值
    cmp eax, ebx                  ; 比较两个整数
    je assert_float_ok_71         ; 如果相等，跳转到成功
assert_float_error_71:
    push dword fmt_assert          ; 打印错误信息
    call _printf                   ; 调用printf
    add esp, 8                     ; 清理栈
    push dword -1                  ; 设置错误退出码
    call _exit                     ; 调用exit
assert_float_ok_71:
    add esp, 8                     ; 清理栈
    fld qword [freg1]             ; 加载FR1
    fstp qword [freg5]            ; 存储到FR5
    fld qword [freg2]             ; 加载FR2
    fld qword [freg5]             ; 加载FR5
    fmulp st1, st0                 ; FR5 *= FR2
    fstp qword [freg5]            ; 存储结果到FR5
    sub esp, 8                     ; 为浮点数参数预留空间
    fld qword [freg5]             ; 加载FR5
    fstp qword [esp]               ; 存储到栈参数
    push dword 5                   ; 压入寄存器编号
    push dword fmt_float            ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 16                    ; 清理栈
    ; 处理浮点数断言: FR5 == 9900.000000
    ; 转换为整数进行比较（乘以1000取整）
    push dword 9900000                ; 推送期望值的整数表示部分
    push dword 0                   ; 为实际值预留空间
    fld qword [freg5]             ; 加载FR5
    fmul dword [esp]              ; 乘以1000
    fistp dword [esp+4]           ; 转换为整数
    mov eax, [esp]                ; 加载期望值
    mov ebx, [esp+4]              ; 加载实际值
    cmp eax, ebx                  ; 比较两个整数
    je assert_float_ok_75         ; 如果相等，跳转到成功
assert_float_error_75:
    push dword fmt_assert          ; 打印错误信息
    call _printf                   ; 调用printf
    add esp, 8                     ; 清理栈
    push dword -1                  ; 设置错误退出码
    call _exit                     ; 调用exit
assert_float_ok_75:
    add esp, 8                     ; 清理栈
    ; 处理浮点数加载: FR6 = 10.000000
    push dword 10000                  ; 推送浮点数的整数表示部分
    push dword 1000                ; 推送除数
    fild dword [esp]               ; 加载除数
    fild dword [esp+4]             ; 加载分子
    fdivp st1, st0                 ; 执行除法得到浮点数
    fstp qword [freg6]            ; 存储到FR6
    add esp, 8                     ; 清理栈
    ; 处理浮点数加载: FR7 = 2.000000
    push dword 2000                  ; 推送浮点数的整数表示部分
    push dword 1000                ; 推送除数
    fild dword [esp]               ; 加载除数
    fild dword [esp+4]             ; 加载分子
    fdivp st1, st0                 ; 执行除法得到浮点数
    fstp qword [freg7]            ; 存储到FR7
    add esp, 8                     ; 清理栈
    fld qword [freg6]             ; 加载FR6
    fld qword [freg7]             ; 加载FR7
    fdivp st1, st0                 ; FR6 /= FR7
    fstp qword [freg6]            ; 存储结果到FR6
    sub esp, 8                     ; 为浮点数参数预留空间
    fld qword [freg6]             ; 加载FR6
    fstp qword [esp]               ; 存储到栈参数
    push dword 6                   ; 压入寄存器编号
    push dword fmt_float            ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 16                    ; 清理栈
    ; 处理浮点数断言: FR6 == 5.000000
    ; 转换为整数进行比较（乘以1000取整）
    push dword 5000                ; 推送期望值的整数表示部分
    push dword 0                   ; 为实际值预留空间
    fld qword [freg6]             ; 加载FR6
    fmul dword [esp]              ; 乘以1000
    fistp dword [esp+4]           ; 转换为整数
    mov eax, [esp]                ; 加载期望值
    mov ebx, [esp+4]              ; 加载实际值
    cmp eax, ebx                  ; 比较两个整数
    je assert_float_ok_80         ; 如果相等，跳转到成功
assert_float_error_80:
    push dword fmt_assert          ; 打印错误信息
    call _printf                   ; 调用printf
    add esp, 8                     ; 清理栈
    push dword -1                  ; 设置错误退出码
    call _exit                     ; 调用exit
assert_float_ok_80:
    add esp, 8                     ; 清理栈
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov eax, [temp_addr]           ; 获取计算的地址
    fld qword [freg1]             ; 加载FR1
    fstp qword [stack + eax]       ; 存储到栈中
    add dword [sp], 2              ; 浮点数占用两个栈位置
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov eax, [temp_addr]           ; 获取计算的地址
    fld qword [freg2]             ; 加载FR2
    fstp qword [stack + eax]       ; 存储到栈中
    add dword [sp], 2              ; 浮点数占用两个栈位置
    ; 处理浮点数加载: FR1 = 5.000000
    push dword 5000                  ; 推送浮点数的整数表示部分
    push dword 1000                ; 推送除数
    fild dword [esp]               ; 加载除数
    fild dword [esp+4]             ; 加载分子
    fdivp st1, st0                 ; 执行除法得到浮点数
    fstp qword [freg1]            ; 存储到FR1
    add esp, 8                     ; 清理栈
    ; 处理浮点数加载: FR2 = 3.000000
    push dword 3000                  ; 推送浮点数的整数表示部分
    push dword 1000                ; 推送除数
    fild dword [esp]               ; 加载除数
    fild dword [esp+4]             ; 加载分子
    fdivp st1, st0                 ; 执行除法得到浮点数
    fstp qword [freg2]            ; 存储到FR2
    add esp, 8                     ; 清理栈
    call fadd                        ; 调用函数 fadd
    sub esp, 8                     ; 为浮点数参数预留空间
    fld qword [freg0]             ; 加载FR0
    fstp qword [esp]               ; 存储到栈参数
    push dword 0                   ; 压入寄存器编号
    push dword fmt_float            ; 压入格式字符串
    call _printf                    ; 调用printf
    add esp, 16                    ; 清理栈
    ; 处理浮点数断言: FR0 == 8.000000
    ; 转换为整数进行比较（乘以1000取整）
    push dword 8000                ; 推送期望值的整数表示部分
    push dword 0                   ; 为实际值预留空间
    fld qword [freg0]             ; 加载FR0
    fmul dword [esp]              ; 乘以1000
    fistp dword [esp+4]           ; 转换为整数
    mov eax, [esp]                ; 加载期望值
    mov ebx, [esp+4]              ; 加载实际值
    cmp eax, ebx                  ; 比较两个整数
    je assert_float_ok_87         ; 如果相等，跳转到成功
assert_float_error_87:
    push dword fmt_assert          ; 打印错误信息
    call _printf                   ; 调用printf
    add esp, 8                     ; 清理栈
    push dword -1                  ; 设置错误退出码
    call _exit                     ; 调用exit
assert_float_ok_87:
    add esp, 8                     ; 清理栈
    sub dword [sp], 2              ; 浮点数占用两个栈位置
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov eax, [temp_addr]           ; 获取计算的地址
    fld qword [stack + eax]        ; 从栈中加载浮点数
    fstp qword [freg2]            ; 存储到FR2
    sub dword [sp], 2              ; 浮点数占用两个栈位置
    mov eax, [sp]                  ; 获取栈指针
    mov edx, 4                     ; 每个栈元素4字节
    mul edx                        ; 计算偏移量
    mov [temp_addr], eax           ; 保存计算的地址
    mov eax, [temp_addr]           ; 获取计算的地址
    fld qword [stack + eax]        ; 从栈中加载浮点数
    fstp qword [freg1]            ; 存储到FR1
    ; 处理浮点数断言: FR1 == 100.000000
    ; 转换为整数进行比较（乘以1000取整）
    push dword 100000                ; 推送期望值的整数表示部分
    push dword 0                   ; 为实际值预留空间
    fld qword [freg1]             ; 加载FR1
    fmul dword [esp]              ; 乘以1000
    fistp dword [esp+4]           ; 转换为整数
    mov eax, [esp]                ; 加载期望值
    mov ebx, [esp+4]              ; 加载实际值
    cmp eax, ebx                  ; 比较两个整数
    je assert_float_ok_90         ; 如果相等，跳转到成功
assert_float_error_90:
    push dword fmt_assert          ; 打印错误信息
    call _printf                   ; 调用printf
    add esp, 8                     ; 清理栈
    push dword -1                  ; 设置错误退出码
    call _exit                     ; 调用exit
assert_float_ok_90:
    add esp, 8                     ; 清理栈
    ; 处理浮点数断言: FR2 == 99.000000
    ; 转换为整数进行比较（乘以1000取整）
    push dword 99000                ; 推送期望值的整数表示部分
    push dword 0                   ; 为实际值预留空间
    fld qword [freg2]             ; 加载FR2
    fmul dword [esp]              ; 乘以1000
    fistp dword [esp+4]           ; 转换为整数
    mov eax, [esp]                ; 加载期望值
    mov ebx, [esp+4]              ; 加载实际值
    cmp eax, ebx                  ; 比较两个整数
    je assert_float_ok_91         ; 如果相等，跳转到成功
assert_float_error_91:
    push dword fmt_assert          ; 打印错误信息
    call _printf                   ; 调用printf
    add esp, 8                     ; 清理栈
    push dword -1                  ; 设置错误退出码
    call _exit                     ; 调用exit
assert_float_ok_91:
    add esp, 8                     ; 清理栈
    push dword 0                   ; 设置正常退出码
    call _exit                     ; 调用exit
