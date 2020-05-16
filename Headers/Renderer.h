#include <SDL.h>
#include <SDL_image.h>
#ifndef SEABATTLE_RENDERER_H
#define SEABATTLE_RENDERER_H

#endif //SEABATTLE_RENDERER_H

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280

void init_renderer(char* window_caption);

SDL_Window* get_window();
SDL_Renderer* get_renderer();
void close_renderer();