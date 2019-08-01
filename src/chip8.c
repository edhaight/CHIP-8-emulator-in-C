#include "chip8.h"
#include "instruction.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

// Fontset used for chip8 cpu
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
static void (*opcode0XXN[])(chip8 *c8);
static void (*opcode8XXN[])(chip8 *c8);
static void (*opcodeFXNN[])(chip8 *c8);

// Function pointer array accessors declarations
static void cpu0XXN(chip8 *c8);
static void cpu8XXN(chip8 *c8);
static void cpuFXNN(chip8 *c8);

// array of function pointers for entire chip8 instruction set
static void (*execOpcode[])(chip8 *c8) = {
    cpu0XXN, cpuJump, cpuNULL, cpuSkipNextEq, cpuSkipNextNotEq, cpuSkipNextRegEq, cpuSetVx, cpuNULL, // 0x0XXX - 0x7XXX
    cpu8XXN, cpuNULL, cpuSetI, cpuNULL, cpuNULL, cpuDrawSprite, cpuNULL, cpuFXNN                     // 08XXX - 0xFXXX
};

// array of function pointers Corresponding to 0x0XXN
static void (*opcode0XXN[])(chip8 *c8) = {
    cpuClearScreen, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, // 0x0XX0 - 0x0XX7
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuReturnFlow            // 0x0XX7 - 0x0XXE
};

// array of function pointers Corresponding to 0x8XXN
static void (*opcode8XXN[])(chip8 *c8) = {
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, // 0x8XX0 - 0x8XX7
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL  // 0x8XX0 - 0x8XXF
};

// array of function pointers Corresponding to 0xFXNN
static void (*opcodeFXNN[])(chip8 *c8) = {
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX00-0xFX07
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX08-0xFX0F
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX10-0xFX17
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX18-0xFX1F
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX20-0xFX27
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX28-0xFX2F
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX30-0xFX37
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX38-0xFX3F
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX40-0xFX47
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX48-0xFX4F
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX50-0xFX57
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX58-0xFX5F
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuFillRegisters, cpuNULL, cpuNULL, //0xFX60-0xFX67
    cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,          //0xFX68-0xFX6F
};

static void cpu0XXN(chip8 *c8)
{
    opcode0XXN[(c8->opcode & 0x000F)](c8);
}

static void cpu8XXN(chip8 *c8)
{
    opcode8XXN[(c8->opcode & 0x000F)](c8);
}

static void cpuFXNN(chip8 *c8)
{
    opcodeFXNN[(c8->opcode & 0x00FF)](c8);
}

// Debugging functions

/* 
Prints a sprite, on is '*' char, off is ' ' char
gets height based on opcode (assumes draw opcode)
populates char array binaryFormat and prints it.
*/
static void printSprite(chip8 c8)
{
    unsigned char height = c8.opcode & 0x000F;
    char binaryFormat[9];
    binaryFormat[8] = '\0';
    printf("\n--Sprite--\n");
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < 8; ++j)
            if ((0x80 >> j) & c8.memory[c8.I + i])
                binaryFormat[j] = '*';
            else
                binaryFormat[j] = ' ';
        printf("%s\n", binaryFormat);
    }
}

/*
prints all fields of the chip8 structure
*/
void printChip8(chip8 c8)
{
    printf("\n----Chip 8 State----\n\n");
    printf("PC: %x\n", c8.pc);
    printf("Opcode: %x\n", c8.opcode);
    printf("Index register: %x\n", c8.I);

    printf("\n---REGISTERS----\n");
    for (int i = 0; i < 16; ++i)
        printf("V[%d] = %x\n", i, c8.V[i]);

    printf("\nDraw Flag: %d\n", c8.drawFlag);

    printf("Delay Timer: %d\n", c8.delayTimer);
    printf("Sound Timer: %d\n", c8.soundTimer);

    printf("Stack pointer: %d\n", c8.sp);
    printf("Stack[%d] = %x\n", c8.sp, c8.stack[c8.sp]);

    printf("Instruction count: %d\n", c8.instructionCount);

    printf("\n");
}

/*
prints differences between previous state of chip8 cpu (prev)
and the current state of chip8 cpu (new)
*/
void printChip8Updates(chip8 prev, chip8 new)
{
    printf("\n----Chip 8 Update # %d----\n\n", new.instructionCount);
    if (prev.pc != new.pc)
        printf("PC: %x -> %x\n", prev.pc, new.pc);
    else
        printf("Warning: PC same as prev instruction (possible infinite loop)\n");

    printf("Opcode: %x\n", new.opcode);

    if (prev.I != new.I)
        printf("Index Register: %x -> %x\n", prev.I, new.I);

    for (int i = 0; i < 16; ++i)
        if (prev.V[i] != new.V[i])
            printf("V[%d]: %x -> %x\n", i, prev.V[i], new.V[i]);

    if (new.drawFlag)
        printSprite(new);

    if (prev.sp != new.sp)
    {
        printf("Stack pointer: %d -> %d\n", prev.sp, new.sp);
        printf("Stack[%d] = %x\n", new.sp, new.stack[new.sp]);
    }

    printf("\n");
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

    c8->instructionCount = 0;
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
    ++c8->instructionCount;
}
