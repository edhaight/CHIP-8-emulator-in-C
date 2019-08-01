#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "instruction.h"

// Instructions definitions

// NULL cpu op
void cpuNULL(chip8 *c8)
{
    printf("Unsupported opcode: %x\n", c8->opcode);
    printf("Instruction count: %d\n", c8->instructionCount);
    exit(1);
}

/*
0x00E0
clears the screen
*/
void cpuClearScreen(chip8 *c8)
{
    memset(c8->gfx, 0, sizeof(c8->gfx));
    c8->drawFlag = 1;
    c8->pc += 2;
}

/*
0x00EE
return from subroutine, decrement stack pointer
*/
void cpuReturnFlow(chip8 *c8)
{
    --c8->sp;
    c8->pc = c8->stack[c8->sp];
    c8->pc += 2;
}

/*
0x1NNN
Jump instruction, jump to NNN for opcodes
*/
void cpuJump(chip8 *c8)
{
    c8->pc = c8->opcode & 0x0FFF;
}

/* 
0x2NNN
Calls subroutine at NNN.
*/
void cpuCallFlow(chip8 *c8)
{
    c8->stack[c8->sp] = c8->pc;
    ++c8->sp;
    c8->pc = c8->opcode & 0x0FFF;
}

/*
0x3XNN
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
0x4XNN
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
0x5XY0
Skips the next instruction if VX equals VY. 
(Usually the next instruction is a jump to skip a code block)
*/
void cpuSkipNextRegEq(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    unsigned char Y = (0x00F0 & c8->opcode) >> 4;
    if (c8->V[X] == c8->V[Y])
        c8->pc += 4;
    else
        c8->pc += 2;
}

/*
0x6XNN
Set register Vx to value NN for opcodes 
*/
void cpuSetVx(chip8 *c8)
{
    unsigned char value = 0x00FF & c8->opcode;
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    c8->V[X] = value;
    c8->pc += 2;
}

/*
0x7XNN
Adds NN to VX. (Carry flag is not changed)
*/
void cpuIncrementVx(chip8 *c8)
{
    unsigned char value = 0x00FF & c8->opcode;
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    c8->V[X] += value;
    c8->pc += 2;
}

/*
0x8XY0
Sets VX to the value of VY.
*/
void cpuSetVxToVy(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    unsigned char Y = (0x00F0 & c8->opcode) >> 4;
    c8->V[X] = c8->V[Y];
    c8->pc += 2;
}

/*
0x8XY1
Sets VX to VX | VY. (Bitwise OR operation)
*/
void cpuSetVxORVxVy(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    unsigned char Y = (0x00F0 & c8->opcode) >> 4;
    c8->V[X] = c8->V[X] | c8->V[Y];
    c8->pc += 2;
}

/*
0x8XY2
Sets VX to VX & VY. (Bitwise AND operation)
*/
void cpuSetVxANDVxVy(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    unsigned char Y = (0x00F0 & c8->opcode) >> 4;
    c8->V[X] = c8->V[X] & c8->V[Y];
    c8->pc += 2;
}

/*
0x8XY3
Sets VX to VX ^ VY. (Bitwise XOR operation)
*/
void cpuSetVxXORVxVy(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    unsigned char Y = (0x00F0 & c8->opcode) >> 4;
    c8->V[X] = c8->V[X] ^ c8->V[Y];
    c8->pc += 2;
}

/*
0x8XY4
Adds VY to VX. VF is set to 1 when there's a carry, 
and to 0 when there isn't.
*/
void cpuIncrementVxVy(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    unsigned char Y = (0x00F0 & c8->opcode) >> 4;
    if (c8->V[X] > 0x00FF - c8->V[Y])
        c8->V[0xF] = 1;
    else
        c8->V[0xF] = 0;
    c8->V[X] += c8->V[Y];
    c8->pc += 2;
}

/*
0x8XY5
VY is subtracted from VX. VF is set to 0 when there's 
a borrow, and 1 when there isn't.
*/
void cpuDecrementVxVy(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    unsigned char Y = (0x00F0 & c8->opcode) >> 4;
    if (c8->V[Y] > c8->V[X])
        c8->V[0xF] = 0;
    else
        c8->V[0xF] = 1;
    c8->V[X] -= c8->V[Y];
    c8->pc += 2;
}

/*
0x8XY6
Stores the least significant bit of VX in VF and then 
shifts VX to the right by 1.
*/
void cpuLSBVxVf(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    c8->V[0xF] = c8->V[X] & 0x1;
    c8->V[X] >>= 1;
    c8->pc += 2;
}

/*
0x8XYE
Stores the most significant bit of VX in VF and then 
shifts VX to the left by 1.
*/
void cpuMSBVxVf(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    c8->V[0xF] = c8->V[X] >> 7;
    c8->V[X] <<= 1;
    c8->pc += 2;
}

/*
0x9XNN
Skips the next instruction if VX doesn't equal VY. 
(Usually the next instruction is a jump to skip a code block)
*/
void cpuSkipNextRegNotEq(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    unsigned char Y = (0x00F0 & c8->opcode) >> 4;
    if (c8->V[X] != c8->V[Y])
        c8->pc += 4;
    else
        c8->pc += 2;
}

/* 
0xANNN 
Sets I to the address NNN for opcodes 
*/
void cpuSetI(chip8 *c8)
{
    unsigned short value = 0x0FFF & c8->opcode;
    c8->I = value;
    c8->pc += 2;
}
/* 
0xCXNN
Sets VX to the result of a bitwise and operation on a 
random number (Typically: 0 to 255) and NN.
*/
void cpuSetVxRandAndNN(chip8 *c8)
{
    unsigned char X = (c8->opcode & 0x0F00) >> 8;
    unsigned char value = c8->opcode & 0x00FF;
    unsigned char randValue = (rand() % 0xFF);
    c8->V[X] = randValue & value;
    c8->pc += 2;
}

/*
DXYN 
Draws a sprite at coordinate (VX, VY) that has a width of 
8 pixels and a height of N pixels. Each row of 8 pixels is 
read as bit-coded starting from memory location I; 
I value doesn’t change after the execution of this instruction. 
As described above, VF is set to 1 if any screen pixels are 
flipped from set to unset when the sprite is drawn, and to
0 if that doesn’t happen
*/

void cpuDrawSprite(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    unsigned char Y = (0x00F0 & c8->opcode) >> 4;
    if (X > 16 || Y > 16)
        printf("Error! index too large!!\n");
    unsigned char x = c8->V[X];
    unsigned char y = c8->V[Y];

    unsigned char height = c8->opcode & 0x000F;
    unsigned char pixel;
    unsigned short fill;

    c8->V[0xF] = 0x00;
    for (int yline = 0; yline < height; ++yline)
    {
        pixel = c8->memory[c8->I + yline];
        for (int xline = 0; xline < 8; ++xline)
        {
            if (((0x80 >> xline) & pixel) != 0)
            {
                fill = (x + xline + ((y + yline) * 64)) % 2048;
                if (c8->gfx[fill] == 0xFF)
                    c8->V[0xF] = 1;
                c8->gfx[fill] ^= 0xFF;
            }
        }
    }
    c8->drawFlag = 1;
    c8->pc += 2;
}

/*
FX07
Stores the delay timer in Vx.
*/
void cpuGetDelayTimerVx(chip8 *c8)
{
    unsigned char X = (c8->opcode & 0x0F00) >> 8;
    c8->V[X] = c8->delayTimer;
    c8->pc += 2;
}

/*
FX15
Sets the delay timer to VX.
*/
void cpuSetDelayTimerVx(chip8 *c8)
{
    unsigned char X = (c8->opcode & 0x0F00) >> 8;
    c8->delayTimer = c8->V[X];
    c8->pc += 2;
}

/*
FX1E
Adds VX to I
*/
void cpuIncrementIVx(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    c8->I += c8->V[X];
    c8->pc += 2;
}

/*
FX33
Stores the binary-coded decimal representation of VX, with the 
most significant of three digits at the address in I, the middle 
digit at I plus 1, and the least significant digit at I plus 2. 
(In other words, take the decimal representation of VX, place the 
hundreds digit in memory at location in I, the tens digit at 
location I+1, and the ones digit at location I+2.)
*/
void cpuSetBCD(chip8 *c8)
{
    unsigned char X = (c8->opcode & 0x0F00) >> 8;
    unsigned char value = c8->V[X];
    c8->memory[c8->I] = value / 100;
    c8->memory[c8->I + 1] = (value / 10) % 10;
    c8->memory[c8->I + 2] = value % 10;
    c8->pc += 2;
}

/*
FX55
Stores V0 to VX (including VX) in memory starting at address I. 
The offset from I is increased by 1 for each value written, but 
I itself is left unmodified.
*/
void cpuStoreRegisters(chip8 *c8)
{
    unsigned char X = (0x0F00 & c8->opcode) >> 8;
    for (int i = 0; i <= X; ++i)
    {
        c8->memory[c8->I + i] = c8->V[i];
    }
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
