#ifndef SEABATTLE_TEXTURE_H
#define SEABATTLE_TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
typedef struct textureT {
    int x;
    int y;
    int width;
    int height;
    double angle;

    SDL_Point* center;
    SDL_Rect* cliprect;
    SDL_RendererFlip fliptype;
    SDL_Texture* texture;
}textureT;



int load_texture(textureT* texture, char* filename);
void texture_init(textureT* texture, int x, int y, double angle, SDL_Point* center, SDL_Rect* cliprect, SDL_RendererFlip fliptype);
void texture_renderer_XY_clip(textureT* texture, int x, int y, SDL_Rect* cliprect);
void init_tile_clip(SDL_Rect* tiles_rect, textureT* tilesTex, int w, int h, int iso_size);

#endif //SEABATTLE_TEXTURE_H