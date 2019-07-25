#include "math.h"
#include "chip8.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

struct display
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
};
typedef struct display display;

int main()
{
    chip8 myChip8;
    if (initializeChip8(&myChip8)) // Instantiate chip8 struct
    {
        printf("Failed to initialize chip8 cpu!\n");
        exit(1);
    }
    else
    {
        printf("Successfully initialized chip8 cpu!\n");
    }

    if (loadGame(&myChip8, "test/test_opcode.ch8")) // Load test game
    {
        printf("Failed to load game!\n");
        exit(1);
    }
    else
    {
        printf("Successfully loaded game!\n");
    }
    // for (int i = 0; i < 10; ++i)                // Emulate 10 cycles
    // {
    //     emulateCycle(&myChip8);
    //     myChip8.pc += 2;
    // }

    // if (SDL_Init(SDL_INIT_VIDEO) != 0) // Try to initialize an SDL video component
    // {
    //     printf("error initializing SDL: %s\n", SDL_GetError());
    //     exit(1);
    // }
    // else
    // {
    //     printf("Initialization successful\n");
    // }

    // int baseWidth = 64;
    // int baseHeight = 32;
    // int windowScale = 10;
    // int windowWidth = baseWidth * windowScale;
    // int windowHeight = baseHeight * windowScale;

    // SDL_Init(SDL_INIT_EVERYTHING);

    // display->window = SDL_CreateWindow(
    //     "Chip 8 Emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    //     windowWidth, windowHeight, SDL_WINDOW_SHOWN);
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
