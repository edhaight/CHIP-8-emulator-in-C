#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#ifndef DISPLAY_H
#define DISPLAY_H

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

int setupDisplay(display *display);
void cleanupDisplay(display *display);

#endif
