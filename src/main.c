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

    printf("Chip8 PC: %d\n", myChip8.pc);

    // for (int i = 0; i < 10; ++i)                // Emulate 10 cycles
    // {
    //     emulateCycle(&myChip8);
    //     myChip8.pc += 2;
    // }

    //

    // int baseWidth = 64;
    // int baseHeight = 32;
    // int windowScale = 10;
    // int windowWidth = baseWidth * windowScale;
    // int windowHeight = baseHeight * windowScale;

    // SDL_Init(SDL_INIT_EVERYTHING);

    // if (display_check_error("window", display->window))
    //     return -1;

    // SDL_Renderer *renderer = SDL_CreateRenderer(MainWindow, -1, 0);

    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // SDL_RenderClear(renderer);

    // SDL_Texture *Tile = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
    //                                       SDL_TEXTUREACCESS_STREAMING, 8, 8);

    // //Clean up
    // SDL_DestroyTexture(Tile);
    // SDL_DestroyWindow(MainWindow);
    // SDL_Quit();

    return 0;
}
