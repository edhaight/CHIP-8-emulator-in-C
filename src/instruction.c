#include "stdio.h"
#include "string.h"
#include "instruction.h"

// Instructions definitions

// NULL function
void cpuNULL(chip8 *c8)
{
    printf("Unsupported opcode: %x\n", c8->opcode);
}

// Clears the screen
void cpuClearScreen(chip8 *c8)
{
    memset(c8->gfx, 0, sizeof(c8->gfx));
    c8->drawFlag = 1;
    c8->pc += 2;
}

// return from subroutine, decrement stack pointer
void cpuReturnFlow(chip8 *c8)
{
    --c8->sp;
    c8->pc = c8->stack[c8->sp];
    c8->pc += 2;
}

// Jump instruction, jump to NNN for opcodes 0x1NNN
void cpuJump(chip8 *c8)
{
    c8->pc = c8->opcode & 0x0FFF;
}

// Set register Vx to value NN for opcodes 0x6XNN
void cpuSetVx(chip8 *c8)
{
    unsigned char value = 0x00FF & c8->opcode;
    unsigned char index = (0x0F00 & c8->opcode) >> 8;
    c8->V[index] = value;
    c8->pc += 2;
}
