#include <stdio.h>
int regisers[256]; // 寄存器数组，一共256个寄存器
// 虚拟机指令 OpCode
typedef enum
{
    MOV = 1,   // 寄存器之间赋值
    ADD,       // 寄存器加法
    SUB,       // 寄存器‍减法
    LOAD,      // 寄存器赋值
    PRINT_INT, // 打印寄存器的值
    EXIT,      // 退出执行
} OpCode;
// 指令结构 Instruction
typedef struct
{
    OpCode op; // 指令
    int dest;  // 目标寄存器
    int src;   // 源寄存器 或 赋值数，根据指令不同判断
} Instruction;
// 自定义程序 - 指令序列
#define INST_MAX 1024 // 指令序列数组大小
Instruction instructions[INST_MAX] = {
    {LOAD, 1, 100}, // R1=>a = 100
    {LOAD, 2, 99},  // R2=>b = 99
    {MOV, 3, 1},    // R3=>c = a ，让c的初始值等于 a
    {ADD, 3, 2},    // R3=>c = c + b , 此时 c 等于 a+b
    {PRINT_INT, 3}, // 打印 R3寄存器 也就是 c 的值
    {EXIT, 3},      // 退出执行，返回值位R3寄存器里的值
};
// 虚拟机执行 vm 函数 ，insts 为指令序列数组(传入数组名)，为 instMax数组大小
int vm(Instruction *insts, int instMax)
{
    printf("\n================== CVM-01 START ==================\n");
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
        case PRINT_INT: // 打印寄存器的值
            printf("PRINT_INT \tR%d ==> %d \n", insts[pc].dest, regisers[insts[pc].dest]);
            break;
        case EXIT: // 退出执行
            printf("EXIT \tR%d ==> %d \n", insts[pc].dest, regisers[insts[pc].dest]);
            printf("================== CVM-01 EXIT  ==================\n\n");
            return regisers[insts[pc].dest];
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