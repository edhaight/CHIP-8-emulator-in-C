#include "math.h"
#include "chip8.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

// void updatePosition(const Uint8 *keystate, SDL_Rect *dest)
// {
//     int x = 0;
//     int y = 0;
//     if (keystate[SDL_SCANCODE_UP])
//     {
//         y--;
//     }
//     if (keystate[SDL_SCANCODE_DOWN])
//     {
//         y++;
//     }
//     if (keystate[SDL_SCANCODE_LEFT])
//     {
//         x--;
//     }
//     if (keystate[SDL_SCANCODE_RIGHT])
//     {
//         x++;
//     }

//     if (x && y)
//     {
//         dest->x += x * 10 / sqrt(2);
//         dest->y += y * 10 / sqrt(2);
//     }
//     else
//     {
//         dest->x += x * 10;
//         dest->y += y * 10;
//     }
//     // right boundary
//     if (dest->x + dest->w > 1000)
//         dest->x = 1000 - dest->w;

//     // left boundary
//     if (dest->x < 0)
//         dest->x = 0;

//     // bottom boundary
//     if (dest->y + dest->h > 1000)
//         dest->y = 1000 - dest->h;

//     // upper boundary
//     if (dest->y < 0)
//         dest->y = 0;
// }

int main()
{
    chip8 myChip8 = initialize();
    loadGame(&myChip8, "test/test_opcode.ch8");

    // if (SDL_Init(SDL_INIT_VIDEO) != 0)
    // {
    //     printf("error initializing SDL: %s\n", SDL_GetError());
    //     exit(1);
    // }
    // else
    // {
    //     printf("Initialization successful\n");
    // }

    // SDL_Window *win = SDL_CreateWindow("GAME", // creates a window
    //                                    SDL_WINDOWPOS_CENTERED,
    //                                    SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);

    // // triggers the program that controls
    // // your graphics hardware and sets flags
    // Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // // creates a renderer to render our images
    // SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    // // creates a surface to load an image into the main memory
    // SDL_Surface *surface;

    // // please provide a path for your image
    // surface = IMG_Load("test/test.jpg");

    // // loads image to our graphics hardware memory.
    // SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);

    // // clears main-memory
    // SDL_FreeSurface(surface);

    // // let us control our image position
    // // so that we can move it with our keyboard.
    // SDL_Rect dest;
    // // connects our texture with dest to control position
    // SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

    // // adjust height and width of our image box.
    // dest.w /= 4;
    // dest.h /= 6;

    // // sets initial x-position of object
    // dest.x = (1000 - dest.w) / 2;

    // // sets initial y-position of object
    // dest.y = (1000 - dest.h) / 2;

    // // controls annimation loop
    // int close = 0;

    // // Keyboard state array
    // const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    // // annimation loop
    // while (!close)
    // {

    //     SDL_Event event;

    //     // Events mangement
    //     while (SDL_PollEvent(&event))
    //     {
    //         switch (event.type)
    //         {
    //         case SDL_QUIT:
    //             // handling of close button
    //             close = 1;
    //             break;
    //         case SDL_KEYDOWN:
    //             break;
    //         }
    //     }
    //     // Update position based on keyboard state
    //     updatePosition(keystate, &dest);
    //     // clears the screen
    //     SDL_RenderClear(rend);
    //     SDL_RenderCopy(rend, tex, NULL, &dest);

    //     // triggers the double buffers
    //     // for multiple rendering
    //     SDL_RenderPresent(rend);

    //     // calculates to 60 fps
    //     SDL_Delay(1000 / 60);
    // }

    // // destroy texture
    // SDL_DestroyTexture(tex);

    // // destroy renderer
    // SDL_DestroyRenderer(rend);

    // // destroy window
    // SDL_DestroyWindow(win);

    SDL_Quit();
    return 0;
}
