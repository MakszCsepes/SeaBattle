#include "InitClose.h"


void init_SDL(char* window_name) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL Init faile %s", SDL_GetError());
        exit(1);
    }

//    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");

    if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0)) {
        fprintf(stdout, "Warning: Linear texture filtering was not enabled");
    }

    init_renderer(window_name);

    if ( !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Could not initialize SDL_Image, SDL_image_Err : %s", IMG_GetError());
        exit(1);
    }


}

void close_down_SDL() {
    close_renderer();
    IMG_Quit();

    SDL_Quit();
}
