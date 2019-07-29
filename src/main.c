#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "stdio.h"
#include "chip8.h"
#include "display.h"

void testAndPrint(char *failMsg, char *successMsg, int failed)
{
    if (failed)
    {
        printf("Failed to %s!\n", failMsg);
        exit(1);
    }
    else
    {
        printf("Succesfully %s!\n", successMsg);
    }
}

void testPC(unsigned int expected, unsigned int actual)
{
    if (expected != actual)
    {
        printf("FAIL - %x != %x\n", expected, actual);
    }
    else
    {
        printf("PASS\n");
    }
}

int main()
{
    chip8 c8;
    display display;
    testAndPrint("initialize chip8 cpu", "initialized chip8 cpu", initializeChip8(&c8));
    testAndPrint("load game", "loaded game", loadGame(&c8, "test/test_opcode.ch8"));
    testAndPrint("setup display", "setup display", setupDisplay(&display));
    testPC(0x200, c8.pc);
    emulateCycle(&c8);
    testPC(0x262, c8.pc);
    emulateCycle(&c8);
    // SDL_Event event;
    // int running = 1;
    // int count = 0;
    // while (running)
    // {
    //     memset(c8.gfx, 0, sizeof(c8.gfx));
    //     c8.gfx[count] = ON_COLOR;
    //     c8.gfx[count + 1] = ON_COLOR;
    //     c8.gfx[count + 64] = ON_COLOR;
    //     c8.gfx[count + 64 + 1] = ON_COLOR;
    //     drawDisplay(&display, c8.gfx);
    //     count += 2;
    //     if (count % 64 == 0)
    //         count += 64;
    //     if (count >= 2048)
    //         count = 0;

    //     while (SDL_PollEvent(&event))
    //     {
    //         if (event.type == SDL_QUIT)
    //         {
    //             running = 0;
    //         }
    //     }
    //     SDL_Delay(1000 / 60);
    // }

    cleanupDisplay(&display);
    return 0;
}
