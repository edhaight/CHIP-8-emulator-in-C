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
    emulateCycle(&myChip8);
    // SDL_Event event;
    // int running = 1;
    // int count = 0;
    // while (running)
    // {
    //     memset(myChip8.gfx, 0, sizeof(myChip8.gfx));
    //     myChip8.gfx[count] = ON_COLOR;
    //     myChip8.gfx[count + 1] = ON_COLOR;
    //     myChip8.gfx[count + 64] = ON_COLOR;
    //     myChip8.gfx[count + 64 + 1] = ON_COLOR;
    //     drawDisplay(&display, myChip8.gfx);
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
