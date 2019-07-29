#include "chip8.h"
#include "instruction.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

static unsigned char chip8FontSet[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// Function pointer array declarations
static void (*execOpcode[])(chip8 *c8);
static void (*returnClearOpcode[])(chip8 *c8);
static void (*ArithmeticOpcode[])(chip8 *c8);
// Function pointer array accessors declarations
static void cpuRETURNCLEAR(chip8 *c8);
static void cpuARITHMETIC(chip8 *c8);

// array of function pointers for entire chip8 instruction set
static void (*execOpcode[])(chip8 *c8) = {
    cpuRETURNCLEAR, cpuJump, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuSetVx, cpuNULL,
    cpuARITHMETIC, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,
    cpuNULL};

// array of function pointers Corresponding to 0x0XXN
static void (*returnClearOpcode[])(chip8 *c8) = {
    cpuClearScreen, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuReturnFlow};

// array of function pointers Corresponding to 0x8XXN
static void (*ArithmeticOpcode[])(chip8 *c8) = {
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL};

static void cpuRETURNCLEAR(chip8 *c8)
{
    returnClearOpcode[(c8->opcode & 0x000F)](c8);
}

static void cpuARITHMETIC(chip8 *c8)
{
    ArithmeticOpcode[(c8->opcode & 0x000F)](c8);
}

int initializeChip8(chip8 *c8)
{
    c8->pc = 0x200;   // Program counter starts at 0x200
    c8->opcode = 0;   // Reset the opcode
    c8->I = 0;        // Reset index register
    c8->sp = 0;       // Reset stack pointer
    c8->drawFlag = 0; // Reset the draw flag

    if (memset(c8->gfx, 0, sizeof(c8->gfx)) == NULL) // Clear display
        return -1;
    if (memset(c8->stack, 0, sizeof(c8->stack)) == NULL) // Clear stack
        return -1;
    if (memset(c8->V, 0, sizeof(c8->V)) == NULL) // Clear Registers
        return -1;
    if (memset(c8->memory, 0, sizeof(c8->memory)) == NULL) // Clear Memory
        return -1;
    if (memcpy(c8->memory, chip8FontSet, sizeof(chip8FontSet)) == NULL) // Load fontset
        return -1;

    c8->delayTimer = 0; // Reset delay Timer
    c8->soundTimer = 0; // Reset sound Timer

    return 0;
}

int loadGame(chip8 *c8, char *filename)
{
    FILE *fptr;
    if ((fptr = fopen(filename, "rb")) == NULL) // Open in read binary mode
    {
        perror("Error");
        return -1;
    }
    fseek(fptr, 0, SEEK_END); // Move file ptr to end of file
    long fsize = ftell(fptr); // Store # of bytes from beginning of file ( to end )
    fseek(fptr, 0, SEEK_SET); // Move file ptr to beginning of file

    unsigned char *buffer = malloc(fsize + 1); // Allocate space for the buffer
    fread(buffer, 1, fsize, fptr);             // Read from file into buffer for fsize bytes
    memcpy(c8->memory + 0x200, buffer, fsize); // Copy buffer into chip memory

    free(buffer); // Free the memory allocated for the buffer
    fclose(fptr); // Close the file
    return 0;
}

void emulateCycle(chip8 *c8)
{
    // Fetch Opcode
    c8->opcode = c8->memory[c8->pc] << 8 | c8->memory[c8->pc + 1];
    // decode Opcode
    unsigned short decoded = (c8->opcode & 0xF000) >> 12;
    // Execute Opcode
    execOpcode[decoded](c8);

    // Update timers
    if (c8->delayTimer > 0)
    {
        --c8->delayTimer;
    }
    if (c8->soundTimer > 0)
    {
        --c8->soundTimer;
        if (c8->soundTimer == 0)
            printf("Alarm!\n");
    }
}
