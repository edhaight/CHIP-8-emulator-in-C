#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#ifndef CHIP8_H
#define CHIP8_H

struct chip8
{
    unsigned short opcode; // Opcode we are executing

    unsigned char memory[4096]; // 0x000-0x1FF - Chip 8 interpreter (contains font set)
                                // 0x050-0x0A0 - Used for the built in 4x5 pixel font set
                                // (0-F) 0x200-0xFFF ∫- Program ROM and work RAM

    unsigned char V[16]; // 15 8-bit general purpose registers - V0 -> VE
                         // 16th register is carry flag - VF

    unsigned short I;  // Index register 0x000 -> 0xFFF
    unsigned short pc; // Program counter 0x000 -> 0xFFF

    unsigned char gfx[64 * 32]; // 2048 Total pixels, black or white

    unsigned char delayTimer; // Ticks at 60Hz, if set above 0: count down to 0
    unsigned char soundTimer; // Ticks at 60Hz, if set above 0: count down to 0

    unsigned short stack[16]; // used to remember current location before a jump is performed
    unsigned short sp;        // Stack pointer
};
typedef struct chip8 chip8;

int initializeChip8(chip8 *myChip8);          // Initializes, resets, and returns a chip8 struct
int loadGame(chip8 *myChip8, char *filename); // Load game into memory
void emulateCycle(chip8 *myChip8);            // Emulate a single chip8 instruction

#endif
