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

int main()
{
    chip8 myChip8;
    display display;
    testAndPrint("initialize chip8 cpu", "initialized chip8 cpu", initializeChip8(&myChip8));
    testAndPrint("load game", "loaded game", loadGame(&myChip8, "test/test_opcode.ch8"));
    testAndPrint("setup display", "setup display", setupDisplay(&display));

    for (int i = 0; i < 2047; ++i)
    {
        myChip8.gfx[i] = ON_COLOR;
    }

    SDL_Event event;
    int running = 1;
    while (running)
    {
        drawDisplay(&display, myChip8.gfx);
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }
    }

    cleanupDisplay(&display);
    return 0;
}
