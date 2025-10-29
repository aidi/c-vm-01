#include <stdio.h>
int regisers[256];     // 整数寄存器数组，一共256个寄存器
float fregisers[256];  // 浮点数寄存器数组，一共256个寄存器
int stack[1024];       // 栈，用于函数调用保存返回地址
int sp = 0;            // 栈指针
// 虚拟机指令 OpCode
typedef enum
{
    MOV = 1,      // 寄存器之间赋值
    ADD,          // 寄存器加法
    SUB,          // 寄存器减法
    MUL,          // 寄存器乘法
    DIV,          // 寄存器除法
    LOAD,         // 寄存器赋值
    PRINT_INT,    // 打印寄存器的值
    ASSERT_INT,   // 断言寄存器值是否符合预期
    CALL,         // 函数调用
    RET,          // 函数返回
    EXIT,         // 退出执行
    LABEL,        // 标签定义
    PUSH,         // 将寄存器值压入栈
    POP,          // 从栈弹出值到寄存器
    // 浮点数指令
    FMOV,         // 浮点数寄存器之间赋值
    FADD,         // 浮点数寄存器加法
    FSUB,         // 浮点数寄存器减法
    FMUL,         // 浮点数寄存器乘法
    FDIV,         // 浮点数寄存器除法
    FLOAD,        // 浮点数寄存器赋值
    PRINT_FLOAT,  // 打印浮点数寄存器的值
    ASSERT_FLOAT, // 断言浮点数寄存器值是否符合预期
    FPUSH,        // 将浮点数寄存器值压入栈
    FPOP          // 从栈弹出值到浮点数寄存器
} OpCode;
// 指令结构内部直接使用匿名联合体支持int和float类型

// 指令结构 Instruction
typedef struct
{
    OpCode op;   // 指令
    int dest;    // 目标寄存器
    union { 
        int src;  
        float src_float; 
    };
    char* label; // 标签名称（用于LABEL和CALL指令）
} Instruction;
// 创建带浮点数的指令辅助函数（仅用于运行时，不能用于全局变量初始化）
Instruction createFloatInst(OpCode op, int dest, float value) {
    Instruction inst = {op, dest};
    inst.src_float = value;
    return inst;
}

// 自定义程序 - 指令序列
#define INST_MAX 1024 // 指令序列数组大小
Instruction instructions[INST_MAX] = {
    {LOAD, 1, 100}, // R1=>a = 100
    {LOAD, 2, 99},  // R2=>b = 99
    {MOV, 3, 1},    // R3=>c = a ，让c的初始值等于 a
    {ADD, 3, 2},    // R3=>c = c + b , 此时 c 等于 a+b
    {PRINT_INT, 3}, // 打印 R3寄存器 也就是 c 的值
    {FLOAD, 1,  {.src_float = 3.1415926f}}, // R1=>a = 3.1415926
    {PRINT_FLOAT, 1}, // 打印浮点数寄存器的值
    {EXIT, 0},      // 退出执行
};

Instruction instructions2[INST_MAX] = {
    {LOAD, 1, 100}, // R1=>a = 100
    {LOAD, 2, 99},  // R2=>b = 99
    {MOV, 3, 1},    // R3=>c = a ，让c的初始值等于 a
    {ADD, 3, 2},    // R3=>c = c + b , 此时 c 等于 a+b
    {PRINT_INT, 3}, // 打印 R3寄存器 也就是 c 的值
    {ASSERT_INT, 3, 199}, // 断言R3的值为199
    
    // 测试减法指令
    {MOV, 4, 1},    // R4 = R1
    {SUB, 4, 2},    // R4 = R4 - R2
    {PRINT_INT, 4}, // 打印减法结果
    {ASSERT_INT, 4, 1},   // 断言R4的值为1
    
    // 测试乘法指令
    {MOV, 5, 1},    // R5 = R1
    {MUL, 5, 2},    // R5 = R5 * R2
    {PRINT_INT, 5}, // 打印乘法结果
    {ASSERT_INT, 5, 9900}, // 断言R5的值为9900
    
    // 测试除法指令
    {MOV, 6, 1},    // R6 = R1
    {DIV, 6, 2},    // R6 = R6 / R2
    {PRINT_INT, 6}, // 打印除法结果
    {ASSERT_INT, 6, 1},   // 断言R6的值为1
    
    // 测试函数调用 - 使用标准参数传递约定
    // 使用PUSH指令保存R1和R2的原始值
    {PUSH, 1},      // 保存R1原始值到栈
    {PUSH, 2},      // 保存R2原始值到栈
    
    // 测试加法函数
    {LOAD, 1, 5},   // 设置函数参数 R1 = 5
    {LOAD, 2, 3},   // 设置函数参数 R2 = 3
    {CALL, 0, 0, "add"},  // 调用加法函数
    {PRINT_INT, 0}, // 打印函数返回值（保存在R0）
    {ASSERT_INT, 0, 8},   // 断言函数返回值为8
    
    // 使用POP指令恢复R2和R1的原始值（注意顺序）
    {POP, 2},       // 恢复R2原始值
    {POP, 1},       // 恢复R1原始值
    
    // 验证寄存器是否被正确恢复
    {ASSERT_INT, 1, 100}, // 断言R1恢复为100
    {ASSERT_INT, 2, 99},  // 断言R2恢复为99
    
    // 测试减法函数
    {PUSH, 1},      // 保存R1原始值到栈
    {PUSH, 2},      // 保存R2原始值到栈
    {LOAD, 1, 10},  // 设置函数参数 R1 = 10
    {LOAD, 2, 4},   // 设置函数参数 R2 = 4
    {CALL, 0, 0, "sub"},  // 调用减法函数
    {PRINT_INT, 0}, // 打印函数返回值
    {ASSERT_INT, 0, 6},   // 断言函数返回值为6
    
    // 恢复寄存器值
    {POP, 2},       // 恢复R2原始值
    {POP, 1},       // 恢复R1原始值
    
    // 验证寄存器是否被正确恢复
    {ASSERT_INT, 1, 100}, // 断言R1恢复为100
    {ASSERT_INT, 2, 99},  // 断言R2恢复为99
    
    // 测试乘法函数
    {PUSH, 1},      // 保存R1原始值到栈
    {PUSH, 2},      // 保存R2原始值到栈
    {LOAD, 1, 6},   // 设置函数参数 R1 = 6
    {LOAD, 2, 7},   // 设置函数参数 R2 = 7
    {CALL, 0, 0, "mul"},  // 调用乘法函数
    {PRINT_INT, 0}, // 打印函数返回值
    {ASSERT_INT, 0, 42},  // 断言函数返回值为42
    
    // 恢复寄存器值
    {POP, 2},       // 恢复R2原始值
    {POP, 1},       // 恢复R1原始值
    
    // 验证寄存器是否被正确恢复
    {ASSERT_INT, 1, 100}, // 断言R1恢复为100
    {ASSERT_INT, 2, 99},  // 断言R2恢复为99
    
    // 测试除法函数
    {PUSH, 1},      // 保存R1原始值到栈
    {PUSH, 2},      // 保存R2原始值到栈
    {LOAD, 1, 20},  // 设置函数参数 R1 = 20
    {LOAD, 2, 5},   // 设置函数参数 R2 = 5
    {CALL, 0, 0, "div"},  // 调用除法函数
    {PRINT_INT, 0}, // 打印函数返回值
    {ASSERT_INT, 0, 4},   // 断言函数返回值为4
    
    // 恢复寄存器值
    {POP, 2},       // 恢复R2原始值
    {POP, 1},       // 恢复R1原始值
    
    // 验证寄存器是否被正确恢复
    {ASSERT_INT, 1, 100}, // 断言R1恢复为100
    {ASSERT_INT, 2, 99},  // 断言R2恢复为99
    
    // ===== 浮点数测试部分 =====
    // 测试浮点数基本指令
    {FLOAD, 1, {.src_float = 100.0f}},  // FR1 = 100.0
    {FLOAD, 2, {.src_float = 99.0f}},   // FR2 = 99.0
    {FMOV, 3, {.src = 1}},     // FR3 = FR1
    {FADD, 3, {.src = 2}},     // FR3 = FR3 + FR2
    {PRINT_FLOAT, 3, {.src = 0}}, // 打印浮点数加法结果
    {ASSERT_FLOAT, 3, {.src_float = 199.0f}}, // 断言FR3的值为199.0
    
    // 测试浮点数减法
      {FMOV, 4, {.src = 1}},     // FR4 = FR1
    {FSUB, 4, {.src = 2}},     // FR4 = FR4 - FR2
      {PRINT_FLOAT, 4, {.src = 0}}, // 打印浮点数减法结果
    {ASSERT_FLOAT, 4, {.src_float = 1.0f}},   // 断言FR4的值为1.0
    
    // 测试浮点数乘法
      {FMOV, 5, {.src = 1}},     // FR5 = FR1
    {FMUL, 5, {.src = 2}},     // FR5 = FR5 * FR2
      {PRINT_FLOAT, 5, {.src = 0}}, // 打印浮点数乘法结果
    {ASSERT_FLOAT, 5, {.src_float = 9900.0f}}, // 断言FR5的值为9900.0
    
    // 测试浮点数除法
    {FLOAD, 6, {.src_float = 10.0f}},   // FR6 = 10.0
    {FLOAD, 7, {.src_float = 2.0f}},    // FR7 = 2.0
    {FDIV, 6, {.src = 7}},     // FR6 = FR6 / FR7
      {PRINT_FLOAT, 6, {.src = 0}}, // 打印浮点数除法结果
    {ASSERT_FLOAT, 6, {.src_float = 5.0f}},  // 断言FR6的值为5.0
    
    // 测试浮点数函数调用
    // 测试浮点数加法函数
    {FPUSH, 1, {.src = 0}},       // 保存FR1原始值到栈
      {FPUSH, 2, {.src = 0}},       // 保存FR2原始值到栈
    {FLOAD, 1, {.src_float = 5.0f}},    // 设置函数参数 FR1 = 5.0
    {FLOAD, 2, {.src_float = 3.0f}},    // 设置函数参数 FR2 = 3.0
    {CALL, 0, {.src = 0}, "fadd"}, // 调用浮点数加法函数
      {PRINT_FLOAT, 0, {.src = 0}}, // 打印浮点数函数返回值
    {ASSERT_FLOAT, 0, {.src_float = 8.0f}},   // 断言函数返回值为8.0
    
    // 恢复浮点数寄存器值
    {FPOP, 2, {.src = 0}},        // 恢复FR2原始值
      {FPOP, 1, {.src = 0}},        // 恢复FR1原始值
    
    // 验证浮点数寄存器是否被正确恢复
    {ASSERT_FLOAT, 1, {.src_float = 100.0f}}, // 断言FR1恢复为100.0
    {ASSERT_FLOAT, 2, {.src_float = 99.0f}}, // 断言FR2恢复为99.0
    {EXIT, 3, {.src = 0}},     // 退出执行，返回值位R3寄存器里的值
    
    // 以下是函数标签定义
    // 函数定义：计算两个数的和
    // 输入：R1, R2
    // 输出：R0
    // 调用方法：LOAD R1, LOAD R2, CALL add, R0即为结果
    {LABEL, 0, 0, "add"},  // 定义add标签
    {MOV, 0, 1},    // R0 = R1
    {ADD, 0, 2},    // R0 += R2
    {RET, 0, 0},               // 返回
    
    // 函数定义：计算两个数的差
    // 输入：R1, R2
    // 输出：R0
    {LABEL, 0, 0, "sub"},  // 定义sub标签
    {MOV, 0, 1},    // R0 = R1
    {SUB, 0, 2},    // R0 -= R2
    {RET, 0, 0},    // 返回
    
    // 函数定义：计算两个数的积
    // 输入：R1, R2
    // 输出：R0
    {LABEL, 0, 0, "mul"},  // 定义mul标签
    {MOV, 0, 1},    // R0 = R1
    {MUL, 0, 2},    // R0 *= R2
    {RET, 0, 0},    // 返回
    
    // 函数定义：计算两个数的商
    // 输入：R1, R2
    // 输出：R0
    {LABEL, 0, 0, "div"},  // 定义div标签
    {MOV, 0, 1},    // R0 = R1
    {DIV, 0, 2},    // R0 /= R2
    {RET, 0, 0},    // 返回
    
    // 浮点数函数定义：计算两个浮点数的和
    // 输入：FR1, FR2
    // 输出：FR0
    {LABEL, 0, 0, "fadd"},  // 定义fadd标签
    {FMOV, 0, 1},    // FR0 = FR1
    {FADD, 0, 2},    // FR0 += FR2
    {RET, 0, 0}     // 返回
};
// 虚拟机执行 vm 函数 ，insts 为指令序列数组(传入数组名)，为 instMax数组大小
int vm(Instruction *insts, int instMax)
{
    printf("\n================== CVM-01 START ==================\n");
    
    // 第一遍扫描：收集标签并更新CALL指令的dest字段
    for (int i = 0; i < instMax; i++) {
        if (insts[i].op == LABEL && insts[i].label != NULL) {
            printf("LABEL: %s at position %d\n", insts[i].label, i);
            // 查找所有使用该标签的CALL指令并更新dest字段
            for (int j = 0; j < instMax; j++) {
                if (insts[j].op == CALL && insts[j].label != NULL && 
                    insts[j].label == insts[i].label) {
                    insts[j].dest = i; // 将标签位置保存到dest字段
                    printf("Updated CALL at position %d to use label '%s' at %d\n", 
                           j, insts[j].label, i);
                }
            }
        }
    }
    
    // 第二遍执行：实际运行指令
    for (int pc = 0; pc < instMax; pc++)
    {
        switch (insts[pc].op) // 判断指令
        {
        case LOAD: // 寄存器赋值
            printf("LOAD \tR%d = %d \n", insts[pc].dest, insts[pc].src);
            regisers[insts[pc].dest] = insts[pc].src;
            break;
        case MOV: // 寄存器之间赋值
            printf("MOV \tR%d = R%d \n", insts[pc].dest, insts[pc].src);
            regisers[insts[pc].dest] = regisers[insts[pc].src];
            break;
        case ADD: // 寄存器加法
            printf("ADD \tR%d += R%d \n", insts[pc].dest, insts[pc].src);
            regisers[insts[pc].dest] = regisers[insts[pc].dest] + regisers[insts[pc].src];
            break;
        case SUB: // 寄存器减法
            printf("SUB \tR%d -= R%d \n", insts[pc].dest, insts[pc].src);
            regisers[insts[pc].dest] = regisers[insts[pc].dest] - regisers[insts[pc].src];
            break;
        case MUL: // 寄存器乘法
            printf("MUL \tR%d *= R%d \n", insts[pc].dest, insts[pc].src);
            regisers[insts[pc].dest] = regisers[insts[pc].dest] * regisers[insts[pc].src];
            break;
        case DIV: // 寄存器除法
            printf("DIV \tR%d /= R%d \n", insts[pc].dest, insts[pc].src);
            regisers[insts[pc].dest] = regisers[insts[pc].dest] / regisers[insts[pc].src];
            break;
        case ASSERT_INT: // 断言寄存器值是否符合预期
            printf("ASSERT_INT \tR%d == %d ", insts[pc].dest, insts[pc].src);
            if (regisers[insts[pc].dest] == insts[pc].src) {
                printf("R%d = %d OK\n", insts[pc].dest, regisers[insts[pc].dest]);
            } else {
                printf("ERROR: R%d = %d != %d\n", insts[pc].dest, regisers[insts[pc].dest], insts[pc].src);
                printf("Assertion failed at instruction %d\n", pc);
                return -1;
            }
            break;
        case PUSH: // 将寄存器值压入栈
            printf("PUSH \tR%d = %d\n", insts[pc].dest, regisers[insts[pc].dest]);
            if (sp < 1024) {
                stack[sp++] = regisers[insts[pc].dest];
            } else {
                printf("Error: Stack overflow\n");
                return -1;
            }
            break;
        case POP: // 从栈弹出值到寄存器
            if (sp > 0) {
                regisers[insts[pc].dest] = stack[--sp];
                printf("POP \tR%d = %d\n", insts[pc].dest, regisers[insts[pc].dest]);
            } else {
                printf("Error: Stack underflow\n");
                return -1;
            }
            break;
        case CALL: // 函数调用
            // 保存返回地址到栈
            stack[sp++] = pc + 1;
            printf("CALL \tjump to position %d\n", insts[pc].dest);
            // 设置PC为目标位置
            pc = insts[pc].dest - 1;
            break;
        case RET: // 函数返回
            printf("RET \treturn from function\n");
            if (sp >= 1) {
                // 从栈中弹出返回地址并跳转
                pc = stack[--sp] - 1;
            } else {
                printf("Error: Stack underflow\n");
                return -1;
            }
            break;
        case PRINT_INT: // 打印寄存器的值
            printf("PRINT_INT \tR%d ==> %d \n", insts[pc].dest, regisers[insts[pc].dest]);
            break;
        case EXIT: // 退出执行
            printf("EXIT \tR%d ==> %d \n", insts[pc].dest, regisers[insts[pc].dest]);
            printf("================== CVM-01 EXIT  ==================\n\n");
            return regisers[insts[pc].dest];
        case LABEL: // 标签指令
            // 标签已经在扫描阶段处理过了，执行时直接跳过
            printf("SKIP LABEL: %s\n", insts[pc].label ? insts[pc].label : "unnamed");
            break;
        // 浮点数指令实现
        case FLOAD: // 浮点数寄存器赋值
            printf("FLOAD \tFR%d = %f \n", insts[pc].dest, insts[pc].src_float);
            fregisers[insts[pc].dest] = insts[pc].src_float;
            break;
        case FMOV: // 浮点数寄存器之间赋值
            printf("FMOV \tFR%d = FR%d \n", insts[pc].dest, insts[pc].src);
            fregisers[insts[pc].dest] = fregisers[insts[pc].src];
            break;
        case FADD: // 浮点数寄存器加法
            printf("FADD \tFR%d += FR%d \n", insts[pc].dest, insts[pc].src);
            fregisers[insts[pc].dest] = fregisers[insts[pc].dest] + fregisers[insts[pc].src];
            break;
        case FSUB: // 浮点数寄存器减法
            printf("FSUB \tFR%d -= FR%d \n", insts[pc].dest, insts[pc].src);
            fregisers[insts[pc].dest] = fregisers[insts[pc].dest] - fregisers[insts[pc].src];
            break;
        case FMUL: // 浮点数寄存器乘法
            printf("FMUL \tFR%d *= FR%d \n", insts[pc].dest, insts[pc].src);
            fregisers[insts[pc].dest] = fregisers[insts[pc].dest] * fregisers[insts[pc].src];
            break;
        case FDIV: // 浮点数寄存器除法
            printf("FDIV \tFR%d /= FR%d \n", insts[pc].dest, insts[pc].src);
            if (fregisers[insts[pc].src] != 0.0f) {
                fregisers[insts[pc].dest] = fregisers[insts[pc].dest] / fregisers[insts[pc].src];
            } else {
                printf("Error: Floating point division by zero\n");
                return -1;
            }
            break;
        case PRINT_FLOAT: // 打印浮点数寄存器的值
            printf("PRINT_FLOAT \tFR%d ==> %f \n", insts[pc].dest, fregisers[insts[pc].dest]);
            break;
        case ASSERT_FLOAT: // 断言浮点数寄存器值是否符合预期
            printf("ASSERT_FLOAT \tFR%d == %f ", insts[pc].dest, insts[pc].src_float);
            // 浮点数比较使用一个小的误差范围
            if (fregisers[insts[pc].dest] >= insts[pc].src_float - 0.0001f && 
                fregisers[insts[pc].dest] <= insts[pc].src_float + 0.0001f) {
                printf("FR%d = %f OK\n", insts[pc].dest, fregisers[insts[pc].dest]);
            } else {
                printf("ERROR: FR%d = %f != %f\n", insts[pc].dest, fregisers[insts[pc].dest], insts[pc].src_float);
                printf("Assertion failed at instruction %d\n", pc);
                return -1;
            }
            break;
        case FPUSH: // 将浮点数寄存器值压入栈（使用类型转换）
            printf("FPUSH \tFR%d = %f\n", insts[pc].dest, fregisers[insts[pc].dest]);
            if (sp < 1024 - sizeof(float) / sizeof(int)) {
                // 使用联合体或类型转换将float存储到int栈中
                union { float f; int i[2]; } converter;
                converter.f = fregisers[insts[pc].dest];
                stack[sp++] = converter.i[0];
                stack[sp++] = converter.i[1];
            } else {
                printf("Error: Stack overflow\n");
                return -1;
            }
            break;
        case FPOP: // 从栈弹出值到浮点数寄存器（使用类型转换）
            if (sp >= 2) {
                union { float f; int i[2]; } converter;
                converter.i[1] = stack[--sp];
                converter.i[0] = stack[--sp];
                fregisers[insts[pc].dest] = converter.f;
                printf("FPOP \tFR%d = %f\n", insts[pc].dest, fregisers[insts[pc].dest]);
            } else {
                printf("Error: Stack underflow\n");
                return -1;
            }
            break;
        default: // 不支持的指令，打印错误信息，并退出
            printf("Error Op=%d\n", insts[pc].op);
            return -1;
        }
    }

    printf("vm end exit\n");
    return 0;
}
void NasmWin64Output(Instruction insts[], int count, char *filename) {
        FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to open output file");
        return;
    }
    fprintf(fp, "section  .data\n");
    fprintf(fp, "    int_format db \"%%d\", 10, 0  ; Format string for printf\n");
    fprintf(fp, "    float_format db \"%%f\", 10, 0  ; Format string for printf\n");
    fprintf(fp, "    hello_vm_basic db \"hello vm-basic\", 10, 0  ; Format string for printf\n");
    fprintf(fp, "    freg times 256 dd 0.0 \n");
    
    fprintf(fp,"section .bss\n");
    fprintf(fp,"    reg resd 8 ; Reserve space for 8 double words (4 bytes each)\n");
    

    fprintf(fp, "section .text\n");
    fprintf(fp,"    extern printf           ; Declare external printf function\n");
    fprintf(fp,"    global main\n");

    fprintf(fp,"main:\n");
    fprintf(fp,"    push rbp                ; Save caller's base pointer\n");
    fprintf(fp,"    mov rbp, rsp            ; Set current function's base pointer\n");
    // hello vm-basic 
    fprintf(fp,"    mov rcx, hello_vm_basic  ; First parameter: format string\n");
    fprintf(fp,"    call printf\n");
    // print 666
    fprintf(fp,"    mov dword [rel reg + 4], 666  ; reg[1] = 1 (each element is 4 bytes, offset is 4)\n");
    fprintf(fp,"    ; Call printf to output reg[0]\n");
    fprintf(fp,"    mov rcx, int_format        ; First parameter: format string\n");
    fprintf(fp,"    mov edx, dword [rel reg+4*1]  ; Second parameter: value of reg[1]\n");
    fprintf(fp,"    call printf\n");
    // compiler 
    fprintf(fp,"; compiler generated code --start-----------------------------------------\n");
    int i = 0;
    while (i < count && insts[i].op != EXIT) {
        switch (insts[i].op) {
            case LOAD:
                fprintf(fp,"    ; LOAD: R%d = %d\n", insts[i].dest, insts[i].src);
                fprintf(fp, "    mov dword [rel reg+%d*4], %d           ; R%d = %d\n", 
                        insts[i].dest, insts[i].src, insts[i].dest, insts[i].src);
                break;
            case FLOAD:
                fprintf(fp,"    ; FLOAD: FR%d = %f\n", insts[i].dest, insts[i].src_float);
                fprintf(fp, "    mov dword [rel freg+%d*4], 0x%08X           ; FR%d = %f\n", 
                        insts[i].dest, insts[i].src, insts[i].dest, insts[i].src_float);
                break;
            case MOV:
                fprintf(fp,"    ; MOV: R%d = R%d\n", insts[i].dest, insts[i].src);
                fprintf(fp,"    mov eax, [rel reg+%d*4]    ; eax=R%d \n",insts[i].src,insts[i].src);
                fprintf(fp,"    mov [rel reg+%d*4], eax    ; R%d=eax\n", insts[i].dest,insts[i].dest);
                break;
            case ADD:
                fprintf(fp,"    ; ADD: R%d = R%d + R%d\n", insts[i].dest, insts[i].dest, insts[i].src);
                fprintf(fp,"    mov eax, [rel reg+%d*4]    ; eax=R%d \n",insts[i].dest,insts[i].dest);
                fprintf(fp,"    add eax, [rel reg+%d*4]    ; eax=eax+R%d \n", insts[i].src,insts[i].src);
                fprintf(fp,"    mov [rel reg+%d*4], eax    ; R%d=eax\n", insts[i].dest,insts[i].dest);
                break;
            case PRINT_INT:
                fprintf(fp,"    ; PRINT_INT:  R%d = ?\n", insts[i].dest);
                fprintf(fp,"    mov rcx, int_format        ; First parameter: format string\n");
                fprintf(fp,"    mov edx, [rel reg+%d*4]    ; second param \n",insts[i].dest);
                fprintf(fp,"    call printf\n");
                break;
            case PRINT_FLOAT:
                fprintf(fp,"    ; PRINT_FLOAT:  FR%d = ?\n", insts[i].dest);
                fprintf(fp,"    mov rcx, float_format        ; First parameter: format string\n");
                fprintf(fp,"    movss xmm0, [rel freg+%d*4]    ; second param \n",insts[i].dest);
                fprintf(fp,"    cvtss2sd xmm0, xmm0  \n");//float 转 double 
                fprintf(fp,"    movq rdx, xmm0   \n");
                fprintf(fp,"    call printf\n");
                break;
            default:
                printf("Error Op=%d\n", insts[i].op);
                return ;
            }
            i++;
    }
    fprintf(fp,"; compiler generated code --end---------------------------------------\n");
    
    //999 end 
    fprintf(fp,"    mov dword [rel reg + 4], 999  ; reg[1] = 1 (each element is 4 bytes, offset is 4)\n");
    fprintf(fp,"    ; Call printf to output reg[0]\n");
    fprintf(fp,"    mov rcx, int_format        ; First parameter: format string\n");
    fprintf(fp,"    mov edx, dword [rel reg+4*1]  ; Second parameter: value of reg[1]\n");
    fprintf(fp,"    call printf\n");

    fprintf(fp,"    ; Clean up stack and return\n");
    fprintf(fp,"    mov rsp, rbp            ; Restore stack pointer\n");
    fprintf(fp,"    pop rbp                 ; Restore base pointer\n");
    fprintf(fp,"    mov eax, 0              ; Set return value to 0\n");
    fprintf(fp,"    ret\n");
    fclose(fp);
}
void AsmOutput(Instruction insts[], int count, char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to open output file");
        return;
    }

    // Windows兼容的汇编头部
    fprintf(fp, "extern _printf\n");
    fprintf(fp, "extern _exit\n");
    fprintf(fp, "global _main\n\n");
    
    // .data段定义 - 使用更简单的数据结构
    fprintf(fp, "section  .data\n");
    fprintf(fp, "fmt_int:    db 'R%%d = %%d\\r\\n', 0\n");
    fprintf(fp, "fmt_float:  db 'FR%%d = %%f\\r\\n', 0\n");
    fprintf(fp, "fmt_assert: db 'ASSERTION FAILED\\r\\n', 0\n");
    
    // 为每个寄存器创建独立的变量，避免复杂的偏移寻址
    for (int i = 0; i < 16; i++) {
        fprintf(fp, "reg%d:      dd 0                   ; R%d\n", i, i);
    }
    for (int i = 0; i < 16; i++) {
        fprintf(fp, "freg%d:     dq 0                   ; FR%d\n", i, i);
    }
    
    // 为栈操作创建临时变量
    fprintf(fp, "stack:      times 1024 dd 0       ; 栈空间\n");
    fprintf(fp, "sp:         dd 0                  ; 栈指针\n");
    fprintf(fp, "temp_addr:  dd 0                  ; 临时地址变量\n\n");
    
    // .text段定义
    fprintf(fp, ".text\n");
    fprintf(fp, "_main:                           ; Windows入口点\n");
    
    int pc = 0;
    while (pc < count && insts[pc].op != EXIT) {
        switch (insts[pc].op) {
            case LOAD:
                
                fprintf(fp, "    mov dword [reg%d], %d           ; R%d = %d\n", 
                        insts[pc].dest, insts[pc].src, insts[pc].dest, insts[pc].src);
                break;
            case FLOAD:
                // 简化浮点数加载，避免复杂寻址
                fprintf(fp, "    ; 处理浮点数加载: FR%d = %f\n", insts[pc].dest, insts[pc].src_float);
                fprintf(fp, "    push dword %d                  ; 推送浮点数的整数表示部分\n", (int)(insts[pc].src_float * 1000));
                fprintf(fp, "    push dword 1000                ; 推送除数\n");
                fprintf(fp, "    fild dword [esp]               ; 加载除数\n");
                fprintf(fp, "    fild dword [esp+4]             ; 加载分子\n");
                fprintf(fp, "    fdivp st1, st0                 ; 执行除法得到浮点数\n");
                fprintf(fp, "    fstp qword [freg%d]            ; 存储到FR%d\n", insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    add esp, 8                     ; 清理栈\n");
                break;
            case MOV:
                // 简单的寄存器赋值
                fprintf(fp, "    mov eax, [reg%d]                ; 加载R%d\n", 
                        insts[pc].src, insts[pc].src);
                fprintf(fp, "    mov [reg%d], eax                ; R%d = R%d\n", 
                        insts[pc].dest, insts[pc].dest, insts[pc].src);
                break;
            case FMOV:
                // 浮点数寄存器操作
                fprintf(fp, "    fld qword [freg%d]             ; 加载FR%d\n", insts[pc].src, insts[pc].src);
                fprintf(fp, "    fstp qword [freg%d]            ; 存储到FR%d\n", 
                        insts[pc].dest, insts[pc].dest);
                break;
            case ADD:
                // 加法操作
                fprintf(fp, "    mov eax, [reg%d]                ; 加载R%d\n", 
                        insts[pc].src, insts[pc].src);
                fprintf(fp, "    add [reg%d], eax                ; R%d += R%d\n", 
                        insts[pc].dest, insts[pc].dest, insts[pc].src);
                break;
            case FADD:
                // 浮点数加法
                fprintf(fp, "    fld qword [freg%d]             ; 加载FR%d\n", 
                        insts[pc].src, insts[pc].src);
                fprintf(fp, "    fld qword [freg%d]             ; 加载FR%d\n", 
                        insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    faddp st1, st0                 ; FR%d += FR%d\n", 
                        insts[pc].dest, insts[pc].src);
                fprintf(fp, "    fstp qword [freg%d]            ; 存储结果到FR%d\n", 
                        insts[pc].dest, insts[pc].dest);
                break;
            case SUB:
                // 减法操作
                fprintf(fp, "    mov eax, [reg%d]                ; 加载R%d\n", 
                        insts[pc].src, insts[pc].src);
                fprintf(fp, "    sub [reg%d], eax                ; R%d -= R%d\n", 
                        insts[pc].dest, insts[pc].dest, insts[pc].src);
                break;
            case FSUB:
                // 浮点数减法
                fprintf(fp, "    fld qword [freg%d]             ; 加载FR%d\n", 
                        insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    fld qword [freg%d]             ; 加载FR%d\n", 
                        insts[pc].src, insts[pc].src);
                fprintf(fp, "    fsubp st1, st0                 ; FR%d -= FR%d\n", 
                        insts[pc].dest, insts[pc].src);
                fprintf(fp, "    fstp qword [freg%d]            ; 存储结果到FR%d\n", 
                        insts[pc].dest, insts[pc].dest);
                break;
            case MUL:
                // 乘法操作
                fprintf(fp, "    mov eax, [reg%d]                ; 加载R%d\n", 
                        insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    mov ebx, [reg%d]                ; 加载R%d\n", 
                        insts[pc].src, insts[pc].src);
                fprintf(fp, "    imul eax, ebx                  ; eax = R%d * R%d\n", 
                        insts[pc].dest, insts[pc].src);
                fprintf(fp, "    mov [reg%d], eax                ; R%d = 结果\n", 
                        insts[pc].dest, insts[pc].dest);
                break;
            case FMUL:
                // 浮点数乘法
                fprintf(fp, "    fld qword [freg%d]             ; 加载FR%d\n", 
                        insts[pc].src, insts[pc].src);
                fprintf(fp, "    fld qword [freg%d]             ; 加载FR%d\n", 
                        insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    fmulp st1, st0                 ; FR%d *= FR%d\n", 
                        insts[pc].dest, insts[pc].src);
                fprintf(fp, "    fstp qword [freg%d]            ; 存储结果到FR%d\n", 
                        insts[pc].dest, insts[pc].dest);
                break;
            case DIV:
                // 除法操作
                fprintf(fp, "    mov eax, [reg%d]                ; 加载R%d\n", 
                        insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    mov ebx, [reg%d]                ; 加载R%d\n", 
                        insts[pc].src, insts[pc].src);
                fprintf(fp, "    cdq                            ; 符号扩展eax到edx\n");
                fprintf(fp, "    idiv ebx                       ; eax = R%d / R%d\n", 
                        insts[pc].dest, insts[pc].src);
                fprintf(fp, "    mov [reg%d], eax                ; R%d = 结果\n", 
                        insts[pc].dest, insts[pc].dest);
                break;
            case FDIV:
                // 浮点数除法
                fprintf(fp, "    fld qword [freg%d]             ; 加载FR%d\n", 
                        insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    fld qword [freg%d]             ; 加载FR%d\n", 
                        insts[pc].src, insts[pc].src);
                fprintf(fp, "    fdivp st1, st0                 ; FR%d /= FR%d\n", 
                        insts[pc].dest, insts[pc].src);
                fprintf(fp, "    fstp qword [freg%d]            ; 存储结果到FR%d\n", 
                        insts[pc].dest, insts[pc].dest);
                break;
            case PRINT_INT:
                // 打印整数 - 简化的参数传递
                fprintf(fp, "    push dword [reg%d]              ; 压入R%d的值\n", 
                        insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    push dword %d                   ; 压入寄存器编号\n", insts[pc].dest);
                fprintf(fp, "    push dword fmt_int              ; 压入格式字符串\n");
                fprintf(fp, "    call _printf                    ; 调用printf\n");
                fprintf(fp, "    add esp, 12                    ; 清理栈\n");
                break;
            case PRINT_FLOAT:
                // 打印浮点数 - 简化的参数传递
                fprintf(fp, "    sub esp, 8                     ; 为浮点数参数预留空间\n");
                fprintf(fp, "    fld qword [freg%d]             ; 加载FR%d\n", insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    fstp qword [esp]               ; 存储到栈参数\n");
                fprintf(fp, "    push dword %d                   ; 压入寄存器编号\n", insts[pc].dest);
                fprintf(fp, "    push dword fmt_float            ; 压入格式字符串\n");
                fprintf(fp, "    call _printf                    ; 调用printf\n");
                fprintf(fp, "    add esp, 16                    ; 清理栈\n");
                break;
            case ASSERT_INT:
                // 整数断言
                fprintf(fp, "    mov eax, [reg%d]                ; 加载R%d\n", insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    cmp eax, %d                     ; 比较R%d和预期值\n", insts[pc].src, insts[pc].dest);
                fprintf(fp, "    je assert_int_ok_%d            ; 如果相等，继续执行\n", pc);
                fprintf(fp, "    push dword fmt_assert           ; 打印错误信息\n");
                fprintf(fp, "    call _printf                    ; 调用printf\n");
                fprintf(fp, "    add esp, 4                      ; 清理栈\n");
                fprintf(fp, "    push dword -1                   ; 设置错误退出码\n");
                fprintf(fp, "    call _exit                      ; 调用exit\n");
                fprintf(fp, "assert_int_ok_%d:\n", pc);
                break;
            case ASSERT_FLOAT:
                // 简化浮点数断言，使用整数比较方式
                fprintf(fp, "    ; 处理浮点数断言: FR%d == %f\n", insts[pc].dest, insts[pc].src_float);
                fprintf(fp, "    ; 转换为整数进行比较（乘以1000取整）\n");
                fprintf(fp, "    push dword %d                ; 推送期望值的整数表示部分\n", (int)(insts[pc].src_float * 1000));
                fprintf(fp, "    push dword 0                   ; 为实际值预留空间\n");
                fprintf(fp, "    fld qword [freg%d]             ; 加载FR%d\n", insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    fmul dword [esp]              ; 乘以1000\n");
                fprintf(fp, "    fistp dword [esp+4]           ; 转换为整数\n");
                fprintf(fp, "    mov eax, [esp]                ; 加载期望值\n");
                fprintf(fp, "    mov ebx, [esp+4]              ; 加载实际值\n");
                fprintf(fp, "    cmp eax, ebx                  ; 比较两个整数\n");
                fprintf(fp, "    je assert_float_ok_%d         ; 如果相等，跳转到成功\n", pc);
                fprintf(fp, "assert_float_error_%d:\n", pc);
                fprintf(fp, "    push dword fmt_assert          ; 打印错误信息\n");
                fprintf(fp, "    call _printf                   ; 调用printf\n");
                fprintf(fp, "    add esp, 8                     ; 清理栈\n");
                fprintf(fp, "    push dword -1                  ; 设置错误退出码\n");
                fprintf(fp, "    call _exit                     ; 调用exit\n");
                fprintf(fp, "assert_float_ok_%d:\n", pc);
                fprintf(fp, "    add esp, 8                     ; 清理栈\n");
                break;
            case PUSH:
                // 使用临时变量计算地址，避免复杂的寻址表达式
                fprintf(fp, "    mov eax, [sp]                  ; 获取栈指针\n");
                fprintf(fp, "    mov edx, 4                     ; 每个栈元素4字节\n");
                fprintf(fp, "    mul edx                        ; 计算偏移量\n");
                fprintf(fp, "    mov [temp_addr], eax           ; 保存计算的地址\n");
                fprintf(fp, "    mov ebx, [reg%d]                ; 加载R%d\n", insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    mov eax, [temp_addr]           ; 获取计算的地址\n");
                fprintf(fp, "    mov dword [stack + eax], ebx   ; 存储到栈中\n");
                fprintf(fp, "    inc dword [sp]                 ; 栈指针加1\n");
                break;
            case POP:
                // 使用临时变量计算地址
                fprintf(fp, "    dec dword [sp]                 ; 栈指针减1\n");
                fprintf(fp, "    mov eax, [sp]                  ; 获取栈指针\n");
                fprintf(fp, "    mov edx, 4                     ; 每个栈元素4字节\n");
                fprintf(fp, "    mul edx                        ; 计算偏移量\n");
                fprintf(fp, "    mov [temp_addr], eax           ; 保存计算的地址\n");
                fprintf(fp, "    mov eax, [temp_addr]           ; 获取计算的地址\n");
                fprintf(fp, "    mov ebx, dword [stack + eax]   ; 从栈中加载值\n");
                fprintf(fp, "    mov [reg%d], ebx                ; 存储到R%d\n", insts[pc].dest, insts[pc].dest);
                break;
            case FPUSH:
                // 浮点数PUSH，使用临时变量计算地址
                fprintf(fp, "    mov eax, [sp]                  ; 获取栈指针\n");
                fprintf(fp, "    mov edx, 4                     ; 每个栈元素4字节\n");
                fprintf(fp, "    mul edx                        ; 计算偏移量\n");
                fprintf(fp, "    mov [temp_addr], eax           ; 保存计算的地址\n");
                fprintf(fp, "    mov eax, [temp_addr]           ; 获取计算的地址\n");
                fprintf(fp, "    fld qword [freg%d]             ; 加载FR%d\n", insts[pc].dest, insts[pc].dest);
                fprintf(fp, "    fstp qword [stack + eax]       ; 存储到栈中\n");
                fprintf(fp, "    add dword [sp], 2              ; 浮点数占用两个栈位置\n");
                break;
            case FPOP:
                // 浮点数POP，使用临时变量计算地址
                fprintf(fp, "    sub dword [sp], 2              ; 浮点数占用两个栈位置\n");
                fprintf(fp, "    mov eax, [sp]                  ; 获取栈指针\n");
                fprintf(fp, "    mov edx, 4                     ; 每个栈元素4字节\n");
                fprintf(fp, "    mul edx                        ; 计算偏移量\n");
                fprintf(fp, "    mov [temp_addr], eax           ; 保存计算的地址\n");
                fprintf(fp, "    mov eax, [temp_addr]           ; 获取计算的地址\n");
                fprintf(fp, "    fld qword [stack + eax]        ; 从栈中加载浮点数\n");
                fprintf(fp, "    fstp qword [freg%d]            ; 存储到FR%d\n", insts[pc].dest, insts[pc].dest);
                break;
            case CALL:
                if (insts[pc].label) {
                    fprintf(fp, "    call %s                        ; 调用函数 %s\n", 
                            insts[pc].label, insts[pc].label);
                }
                break;
            case RET:
                fprintf(fp, "    ret                            ; 返回\n");
                break;
            case LABEL:
                if (insts[pc].label) {
                    fprintf(fp, "%s:\n", insts[pc].label);
                }
                break;
            case EXIT:
                // 这个会被while循环处理，不需要额外输出
                break;
            default:
                fprintf(fp, "    ; Unsupported instruction: %d\n", insts[pc].op);
                break;
        }
        pc++;
    }

    // 程序结束，Windows风格的退出
    fprintf(fp, "    push dword 0                   ; 设置正常退出码\n");
    fprintf(fp, "    call _exit                     ; 调用exit\n");

    fclose(fp);
    printf("NASM assembly code successfully generated to %s\n", filename);
    printf("Check file exists: %s\n", filename);
    // 尝试重新打开文件验证
    FILE *verify = fopen(filename, "r");
    if (verify) {
        printf("File verification successful: %s exists\n", filename);
        fclose(verify);
    } else {
        printf("Warning: Cannot reopen file for verification\n");
        perror("reopen error");
    }
}

int main(void)
{
    int ret = 0; // 虚拟机返回值
    printf("Hello VM-01 \n");
    
    // 生成NASM汇编代码
    printf("Generating NASM assembly code...\n");
    NasmWin64Output(instructions, INST_MAX, "out.asm");
    printf("Assembly generation complete.\n");
    
    // 运行虚拟机
    ret = vm(instructions, INST_MAX);
    printf("vm return = %d\n", ret);
    return ret;
}