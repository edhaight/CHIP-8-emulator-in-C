#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "stdio.h"
#include "chip8.h"
#include "display.h"

#define ON 1
#define OFF 0
#define DEFAULT_FRAMERATE 1000 / 60
#define DEBUG_FRAMERATE 200

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

int main(int argc, char **argv)
{
    chip8 c8;
    chip8 c8Debug;
    display display;
    float frameRate = DEFAULT_FRAMERATE;
    char debug = OFF;
    testAndPrint("initialize chip8 cpu", "initialized chip8 cpu", initializeChip8(&c8));
    testAndPrint("load game", "loaded game", loadGame(&c8, "test/test_opcode.ch8"));
    testAndPrint("setup display", "setup display", setupDisplay(&display));

    if (argc > 1 && (strcmp("-d", argv[1]) == 0))
    {
        c8Debug = c8;
        debug = ON;
        frameRate = DEBUG_FRAMERATE;
    }

    SDL_Event event;
    char running = ON;
    while (running)
    {
        emulateCycle(&c8);

        if (debug)
        {
            printChip8Updates(c8Debug, c8);
            c8Debug = c8;
        }

        if (c8.drawFlag)
        {
            drawDisplay(&display, c8.gfx);
            c8.drawFlag = OFF;
        }

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = OFF;
            }
        }
        SDL_Delay(frameRate);
    }

    cleanupDisplay(&display);
    return 0;
}
