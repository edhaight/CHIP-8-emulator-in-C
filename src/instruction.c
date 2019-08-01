#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "instruction.h"

// Instructions definitions

// NULL function
void cpuNULL(chip8 *c8)
{
    printf("Unsupported opcode: %x\n", c8->opcode);
    printf("Instruction count: %d\n", c8->instructionCount);
    exit(1);
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

// Sets I to the address NNN for opcodes 0xaNNN
void cpuSetI(chip8 *c8)
{
    unsigned short value = 0x0FFF & c8->opcode;
    c8->I = value;
    c8->pc += 2;
}

/* 
Draws a sprite at coordinate (VX, VY) that has a width of 
8 pixels and a height of N pixels. Each row of 8 pixels is 
read as bit-coded starting from memory location I; 
I value doesn’t change after the execution of this instruction. 
As described above, VF is set to 1 if any screen pixels are 
flipped from set to unset when the sprite is drawn, and to
0 if that doesn’t happen

DXYN

D004

0xA0 0x40 0xA0 0xA0
*/

void cpuDrawSprite(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    unsigned char Y = (0x00F0 & c8->opcode) >> 4;
    unsigned char x = c8->V[X];
    unsigned char y = c8->V[Y];
    unsigned char height = c8->opcode & 0x000F;
    unsigned char pixel;

    c8->V[0xF] = 0x00;
    for (int yline = 0; yline < height; ++yline)
    {
        pixel = c8->memory[c8->I + yline];
        for (int xline = 0; xline < 8; ++xline)
        {
            if (((0x80 >> xline) & pixel) != 0)
            {
                if (c8->gfx[x + xline + ((y + yline) * 64)] == 0xFF)
                    c8->V[0xF] = 1;
                c8->gfx[x + xline + ((y + yline) * 64)] ^= 0xFF;
            }
        }
    }
    c8->drawFlag = 1;
    c8->pc += 2;
}

/*
3XNN
Skips the next instruction if VX equals NN.
(Usually the next instruction is a jump to skip a code block)
*/
void cpuSkipNextEq(chip8 *c8)
{
    unsigned char value = 0x00FF & c8->opcode;
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    if (c8->V[X] == value)
        c8->pc += 4;
    else
        c8->pc += 2;
}

/*
4XNN
Skips the next instruction if VX does not equals NN.
(Usually the next instruction is a jump to skip a code block)
*/
void cpuSkipNextNotEq(chip8 *c8)
{
    unsigned short value = 0x00FF & c8->opcode;
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    if (c8->V[X] != value)
        c8->pc += 4;
    else
        c8->pc += 2;
}

/*
FX65
Fills V0 to VX (including VX) with values from memory starting 
at address I. The offset from I is increased by 1 for each value 
written, but I itself is left unmodified.
*/
void cpuFillRegisters(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    for (int i = 0; i <= X; ++i)
    {
        c8->V[i] = c8->memory[c8->I + i];
    }
    c8->pc += 2;
}
