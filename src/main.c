#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "stdio.h"
#include "chip8.h"
#include "display.h"

#define ON 1
#define OFF 0
#define DEFAULT_FRAMERATE 1000 / 60
#define DEBUG_FRAMERATE 1000 / 60

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
    display display;

    if (argc < 2)
    {
        printf("Usage: ./chip8 filename [-d]\n");
        return 1;
    }
    testAndPrint("initialize chip8 cpu", "initialized chip8 cpu", initializeChip8(&c8));
    testAndPrint("load game", "loaded game", loadGame(&c8, argv[1]));
    testAndPrint("setup display", "setup display", setupDisplay(&display));

    chip8 c8Debug = c8;
    char debug = OFF;

    if (argc > 2 && (strcmp("-d", argv[2]) == 0))
        debug = ON;

    float frameRate = debug ? DEBUG_FRAMERATE : DEFAULT_FRAMERATE;
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
            switch (event.type)
            {
            case SDL_QUIT:
                running = OFF;
                break;
            default:
                break;
            }
        }
        SDL_Delay(frameRate);
    }

    cleanupDisplay(&display);
    return 0;
}
