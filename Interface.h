#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#ifndef SEABATTLE_INTERFACE_H
#define SEABATTLE_INTERFACE_H

#endif //SEABATTLE_INTERFACE_H


#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1000


class IDrawable {
public:
    virtual void draw(SDL_Renderer* ) {

    }
};

