
#ifndef SEABATTLE_CEFFECT_H
#define SEABATTLE_CEFFECT_H

#include "CAnimation.h"

const int FPS = 60;


class CEffect : public IDrawable{
    int FrameTime;
    SDL_Rect fire;
    SDL_Rect fire_position;

    textureT fire_texture;

    int frameWidth;
    int frameHeight;
public:
    CEffect() {
        FrameTime = 0;
        fire.x = fire.y = 0;

        // todo why 32
        fire_position.w = fire_position.h = 32;

        if(load_texture(&fire_texture, "Images/fires2.png") == 0) {
            fprintf(stderr, "Error, could not load texture : data/fires.png");
            exit(0);
        }
        SDL_QueryTexture(fire_texture.texture, NULL, NULL, &fire_texture.width, &fire_texture.height);

        // todo why 8 and 4
        frameWidth = fire_texture.width / 8;
        frameHeight = fire_texture.height / 4;

        fire.x = fire.y = 0;
        fire.w = frameWidth;
        fire.h = frameHeight;
    }
    CEffect(int x, int y , int map_off_x, int map_off_y, int off_x, int off_y) {
        FrameTime = 0;
        fire.x = y * TILESIZE + map_off_x + off_x;
        fire.y = x * TILESIZE + map_off_y + off_y;

        // todo why 32
        fire_position.w = fire_position.h = 32;

        if(load_texture(&fire_texture, "Images/fires2.png") == 0) {
            fprintf(stderr, "Error, could not load texture : data/fires.png");
            exit(0);
        }
        SDL_QueryTexture(fire_texture.texture, NULL, NULL, &fire_texture.width, &fire_texture.height);

        // todo why 8 and 4
        frameWidth = fire_texture.width / 8;
        frameHeight = fire_texture.height / 4;

        fire.x = fire.y = 0;
        fire.w = frameWidth;
        fire.h = frameHeight;
    }
    void draw(isoEngineT* isoEngine);
};


#endif //SEABATTLE_CEFFECT_H
