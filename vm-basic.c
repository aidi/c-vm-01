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
// 创建带浮点数的指令辅助函数
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
    createFloatInst(FLOAD, 1, 100.0f),  // FR1 = 100.0
    createFloatInst(FLOAD, 2, 99.0f),   // FR2 = 99.0
    {FMOV, 3, {.src = 1}},     // FR3 = FR1
      {FADD, 3, {.src = 2}},     // FR3 = FR3 + FR2
      {PRINT_FLOAT, 3, {.src = 0}}, // 打印浮点数加法结果
    createFloatInst(ASSERT_FLOAT, 3, 199.0f), // 断言FR3的值为199.0
    
    // 测试浮点数减法
      {FMOV, 4, {.src = 1}},     // FR4 = FR1
    {FSUB, 4, {.src = 2}},     // FR4 = FR4 - FR2
      {PRINT_FLOAT, 4, {.src = 0}}, // 打印浮点数减法结果
    createFloatInst(ASSERT_FLOAT, 4, 1.0f),   // 断言FR4的值为1.0
    
    // 测试浮点数乘法
      {FMOV, 5, {.src = 1}},     // FR5 = FR1
    {FMUL, 5, {.src = 2}},     // FR5 = FR5 * FR2
      {PRINT_FLOAT, 5, {.src = 0}}, // 打印浮点数乘法结果
    createFloatInst(ASSERT_FLOAT, 5, 9900.0f), // 断言FR5的值为9900.0
    
    // 测试浮点数除法
    createFloatInst(FLOAD, 6, 10.0f),   // FR6 = 10.0
    createFloatInst(FLOAD, 7, 2.0f),    // FR7 = 2.0
    {FDIV, 6, {.src = 7}},     // FR6 = FR6 / FR7
      {PRINT_FLOAT, 6, {.src = 0}}, // 打印浮点数除法结果
    createFloatInst(ASSERT_FLOAT, 6, 5.0f),  // 断言FR6的值为5.0
    
    // 测试浮点数函数调用
    // 测试浮点数加法函数
    {FPUSH, 1, {.src = 0}},       // 保存FR1原始值到栈
      {FPUSH, 2, {.src = 0}},       // 保存FR2原始值到栈
    createFloatInst(FLOAD, 1, 5.0f),    // 设置函数参数 FR1 = 5.0
    createFloatInst(FLOAD, 2, 3.0f),    // 设置函数参数 FR2 = 3.0
    {CALL, 0, {.src = 0}, "fadd"}, // 调用浮点数加法函数
      {PRINT_FLOAT, 0, {.src = 0}}, // 打印浮点数函数返回值
    createFloatInst(ASSERT_FLOAT, 0, 8.0f),   // 断言函数返回值为8.0
    
    // 恢复浮点数寄存器值
    {FPOP, 2, {.src = 0}},        // 恢复FR2原始值
      {FPOP, 1, {.src = 0}},        // 恢复FR1原始值
    
    // 验证浮点数寄存器是否被正确恢复
    createFloatInst(ASSERT_FLOAT, 1, 100.0f), // 断言FR1恢复为100.0
    createFloatInst(ASSERT_FLOAT, 2, 99.0f)  // 断言FR2恢复为99.0
    
    {EXIT, 3, {.src = 0}}      // 退出执行，返回值位R3寄存器里的值
    
    // 函数定义：计算两个数的和
    // 输入：R1, R2
    // 输出：R0
    {LABEL, 0, 0, "add"},  // 添加加法函数标签
    {MOV, 0, 1},    // R0 = R1
    {ADD, 0, 2},    // R0 = R0 + R2
    {RET, 0, 0},    // 返回
    
    // 函数定义：计算两个数的差
    // 输入：R1, R2
    // 输出：R0
    {LABEL, 0, 0, "sub"},  // 添加减法函数标签
    {MOV, 0, 1},    // R0 = R1
    {SUB, 0, 2},    // R0 = R0 - R2
    {RET, 0, 0},    // 返回
    
    // 函数定义：计算两个数的积
    // 输入：R1, R2
    // 输出：R0
    {LABEL, 0, 0, "mul"},  // 添加乘法函数标签
    {MOV, 0, 1},    // R0 = R1
    {MUL, 0, 2},    // R0 = R0 * R2
    {RET, 0, 0},    // 返回
    
    // 函数定义：计算两个数的商
    // 输入：R1, R2
    // 输出：R0
    {LABEL, 0, 0, "div"},  // 添加除法函数标签
    {MOV, 0, 1},    // R0 = R1
    {DIV, 0, 2},    // R0 = R0 / R2
    {RET, 0, 0},    // 返回
    
    // 浮点数函数定义：计算两个浮点数的和
    // 输入：FR1, FR2
    // 输出：FR0
    {LABEL, 0, 0, "fadd"},  // 添加浮点数加法函数标签
    {FMOV, 0, 1},    // FR0 = FR1
    {FADD, 0, 2},    // FR0 = FR0 + FR2
    {RET, 0, 0},     // 返回
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
int main(void)
{
    int ret = 0; // 虚拟机返回值
    printf("Hello VM-01 \n");
    ret = vm(instructions, INST_MAX);
    printf("vm return = %d\n", ret);
    return ret;
}