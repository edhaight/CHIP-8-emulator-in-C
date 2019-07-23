#include "chip8.h"

void resetChip(chip8 *myChip8)
{
    myChip8->pc = 0x200;                                 // Program counter starts at 0x200
    myChip8->opcode = 0;                                 // Reset Current
    myChip8->I = 0;                                      // Reset index register
    myChip8->sp = 0;                                     // Reset stack pointer
    memset(myChip8->gfx, 0, sizeof(myChip8->gfx));       // Clear display
    memset(myChip8->stack, 0, sizeof(myChip8->stack));   // Clear stack
    memset(myChip8->V, 0, sizeof(myChip8->V));           // Clear Registers
    memset(myChip8->memory, 0, sizeof(myChip8->memory)); // Clear Memory
    for (int i = 0; i < 80; ++i)                         // Load fontset
        myChip8->memory[i] = chip8_fontset[i];
}

chip8 initialize()
{
    chip8 *myChip8 = malloc(sizeof(*myChip8));
    if (!myChip8)
    {
        perror("malloc failed");
        exit(1);
    }
    resetChip(myChip8);
    return *myChip8;
}
