#include "chip8.h"

int initializeChip8(chip8 *myChip8)
{
    myChip8->pc = 0x200; // Program counter starts at 0x200
    myChip8->opcode = 0; // Reset Current
    myChip8->I = 0;      // Reset index register
    myChip8->sp = 0;     // Reset stack pointer

    if (memset(myChip8->gfx, 0, sizeof(myChip8->gfx)) == NULL) // Clear display
        return -1;
    if (memset(myChip8->stack, 0, sizeof(myChip8->stack)) == NULL) // Clear stack
        return -1;
    if (memset(myChip8->V, 0, sizeof(myChip8->V)) == NULL) // Clear Registers
        return -1;
    if (memset(myChip8->memory, 0, sizeof(myChip8->memory)) == NULL) // Clear Memory
        return -1;
    if (memcpy(myChip8->memory, chip8FontSet, sizeof(chip8FontSet)) == NULL) // Load fontset
        return -1;

    myChip8->delayTimer = 0; // Reset delay Timer
    myChip8->soundTimer = 0; // Reset sound Timer

    return 0;
}

int loadGame(chip8 *myChip8, char *filename)
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

    unsigned char *buffer = malloc(fsize + 1);      // Allocate space for the buffer
    fread(buffer, 1, fsize, fptr);                  // Read from file into buffer for fsize bytes
    memcpy(myChip8->memory + 0x200, buffer, fsize); // Copy buffer into chip memory

    free(buffer); // Free the memory allocated for the buffer
    fclose(fptr); // Close the file
    return 0;
}

void emulateCycle(chip8 *myChip8)
{
    // Fetch Opcode
    unsigned short pc = myChip8->pc;
    unsigned short opcode = myChip8->memory[pc] << 8 | myChip8->memory[pc + 1];
    // Decode Opcode
    switch (opcode & 0xF000)
    {
    // Execute Opcode
    case 0xA000: // ANNN: Sets I to the address NNN
        myChip8->I = opcode & 0x0FFF;
        pc += 2;
        break;
    // More opcodes //
    default:
        printf("Unkown opcode: 0x%X\n", opcode);
    }

    // Copy back pc to chip8 struct
    myChip8->pc = pc;

    // Update timers
    if (myChip8->delayTimer > 0)
        --myChip8->delayTimer;
    if (myChip8->soundTimer > 0)
    {
        --myChip8->soundTimer;
        if (myChip8->soundTimer == 0)
            printf("Alarm!\n");
    }
}
