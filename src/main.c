#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "stdio.h"
#include "../include/chip8.h"
#include "../include/display.h"

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

int main(int argc, char **argv)
{
    chip8 c8;
    chip8 c8Debug;
    display display;
    testAndPrint("initialize chip8 cpu", "initialized chip8 cpu", initializeChip8(&c8));
    testAndPrint("load game", "loaded game", loadGame(&c8, "test/test_opcode.ch8"));
    testAndPrint("setup display", "setup display", setupDisplay(&display));

    float frameRate = 1000 / 60;
    if (argc > 1 && (strcmp("-d", argv[1]) == 0))
    {
        c8Debug = c8;
        c8.debug = 1;
        frameRate = 400;
    }

    SDL_Event event;
    int running = 1;

    while (running)
    {

        if (c8.debug)
        {
            c8Debug = c8;
            emulateCycle(&c8);
            printChip8Updates(c8Debug, c8);
        }
        else
        {
            emulateCycle(&c8);
        }

        if (c8.drawFlag)
        {
            drawDisplay(&display, c8.gfx);
            c8.drawFlag = 0;
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }
        SDL_Delay(frameRate);
    }

    cleanupDisplay(&display);
    return 0;
}
