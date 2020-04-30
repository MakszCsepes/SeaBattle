
#include "Renderer.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

void init_renderer(char* window_caption) {
    window = SDL_CreateWindow(window_caption, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        fprintf(stderr, "SDL CreateWindow failed: %s", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        fprintf(stderr, "SDL CreateRenderer failed %s", SDL_GetError());
        exit(1);
    }


}


SDL_Window* get_window() {
    return window;
}
SDL_Renderer* get_renderer() {
    return renderer;
}
void close_renderer() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
