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

#define NUM_ISO_TILES 7
extern SDL_Rect tiles_rect[NUM_ISO_TILES];
extern textureT tilesTex;



typedef struct gameT {
    SDL_Event event;
    int loop_done;
    SDL_Rect mouseRect;
    point2DT mousePoint;
    point2DT mapScroll2Dpos;
    int mapScrollSpeed;
    isoEngineT isoEngine;
    int lastTiledClick;
}gameT;
extern gameT gameT1;

extern SDL_Rect submarine_rect[1];
extern textureT submarineTex;

extern SDL_Rect cruiser_rect[1];
extern textureT cruiserTex;

extern SDL_Rect destroyer_rect[1];
extern textureT destroyerTex;

extern SDL_Rect battleship_rect[1];
extern textureT battleshipTex;

extern unsigned int TILESIZE;

class IDrawable {
public:
    virtual void draw(SDL_Renderer* ) {

    }
};

