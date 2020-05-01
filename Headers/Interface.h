#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "InitClose.h"
#include "Texture.h"
#include "IsoEngine.h"


#ifndef SEABATTLE_INTERFACE_H
#define SEABATTLE_INTERFACE_H

#endif //SEABATTLE_INTERFACE_H

#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1000

#define NUM_ISO_TILES 5
extern SDL_Rect tiles_rect[NUM_ISO_TILES];
extern textureT tilesTex;
extern unsigned int TILESIZE;

class IDrawable {
public:
    virtual void draw(SDL_Renderer* ) {

    }
};

