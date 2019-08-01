#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "chip8.h"

// Instructions

// Null op
void cpuNULL(chip8 *c8);

// 0x00EX
void cpuClearScreen(chip8 *c8);
void cpuReturnFlow(chip8 *c8);

// 0x1NNN
void cpuJump(chip8 *c8);

// 0x3XNN
void cpuSkipNextEq(chip8 *c8);
// 0x4XNN
void cpuSkipNextNotEq(chip8 *c8);

// 0x6XNN
void cpuSetVx(chip8 *c8);

// 0xANNN
void cpuSetI(chip8 *c8);

// 0xDXYN
void cpuDrawSprite(chip8 *c8);

// 0xFX65
void cpuFillRegisters(chip8 *c8);

#endif
