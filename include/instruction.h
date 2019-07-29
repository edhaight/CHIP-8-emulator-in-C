#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "chip8.h"

// Instructions
void cpuNULL(chip8 *c8);
// returnClear functions
void cpuClearScreen(chip8 *c8);
void cpuReturnFlow(chip8 *c8);
void cpuSetVx(chip8 *c8);

void cpuJump(chip8 *c8);

#endif
