#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "chip8.h"

// Instructions

// Null op
void cpuNULL(chip8 *c8);

// 0x00E0
void cpuClearScreen(chip8 *c8);
// 0x00EE
void cpuReturnFlow(chip8 *c8);
// 0x1NNN
void cpuJump(chip8 *c8);
// 0x2NNN
void cpuCallFlow(chip8 *c8);
// 0x3XNN
void cpuSkipNextEq(chip8 *c8);
// 0x4XNN
void cpuSkipNextNotEq(chip8 *c8);
// 0x5XY0
void cpuSkipNextRegEq(chip8 *c8);
// 0x6XNN
void cpuSetVx(chip8 *c8);
// 0x7XNN
void cpuIncrementVx(chip8 *c8);
// 0x8XY0
void cpuSetVxToVy(chip8 *c8);
// 0x8XY1
void cpuSetVxORVxVy(chip8 *c8);
// 0x8XY2
void cpuSetVxANDVxVy(chip8 *c8);
// 0x8XY3
void cpuSetVxXORVxVy(chip8 *c8);
// 0x8XY4
void cpuIncrementVxVy(chip8 *c8);
// 0x8XY5
void cpuDecrementVxVy(chip8 *c8);
// 0x8XY6
void cpuLSBVxVf(chip8 *c8);
// 0x8XYE
void cpuMSBVxVf(chip8 *c8);
// 0x9XY0
void cpuSkipNextRegNotEq(chip8 *c8);
// 0xANNN
void cpuSetI(chip8 *c8);

// 0xCXNN
void cpuSetVxRandAndNN(chip8 *c8);
// 0xDXYN
void cpuDrawSprite(chip8 *c8);

// 0xFX07
void cpuGetDelayTimerVx(chip8 *c8);
// 0xFX15
void cpuSetDelayTimerVx(chip8 *c8);
// 0xFX33
void cpuSetBCD(chip8 *c8);
// 0xFX55
void cpuStoreRegisters(chip8 *c8);
// 0xFX65
void cpuFillRegisters(chip8 *c8);

#endif
