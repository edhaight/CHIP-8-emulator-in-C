#include "math.h"
#include "chip8.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define WINDOW_WIDTH 64
#define WINDOW_HEIGHT 32
#define WINDOW_SCALE 10

struct display
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
};
typedef struct display display;

// https : //github.com/scanlong/c8/blob/master/src/display.c
int checkDisplayError(char *component, void *pointer)
{
    if (pointer == NULL)
    {
        printf("Could not create %s: %s\n", component, SDL_GetError());
        return -1;
    }
    return 0;
}
int setupDisplay(display *display)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) // Try to initialize an SDL video component
    {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return -1;
    }

    display->window = SDL_CreateWindow(
        "Chip 8 Emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH * WINDOW_SCALE, WINDOW_HEIGHT * WINDOW_SCALE, SDL_WINDOW_SHOWN);

    if (checkDisplayError("window", display->window))
        return -1;

    display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);

    if (checkDisplayError("renderer", display->renderer))
        return -1;

    return 0;
}

void checkFailure(char *failMsg, char *successMsg, int failed)
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
    checkFailure("initialize chip8 cpu", "initialized chip8 cpu", initializeChip8(&myChip8));
    checkFailure("load game", "loaded game", loadGame(&myChip8, "test/test_opcode.ch8"));
    checkFailure("setup display", "setup display", setupDisplay(&display));

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
