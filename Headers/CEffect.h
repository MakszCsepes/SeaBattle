
#ifndef SEABATTLE_CEFFECT_H
#define SEABATTLE_CEFFECT_H

#include "CAnimation.h"

const int EFFECT_SIZE = 32;
const int FRAME_NUMBER_IN_WIDTH = 8;
const int FRAME_NUMBER_IN_HEIGTH = 4;

class CEffect : public IDrawable{
    int FrameTime;
    SDL_Rect effect;
    SDL_Rect effect_position;

    textureT effect_texture;

    int frameWidth;
    int frameHeight;
public:
    CEffect() {
        effect.x = effect.y = 0;

        *this = CEffect(effect.x, effect.y, 0, 0);
    }
    CEffect(int x, int y, int offset_x, int offset_y) {
        FrameTime = 0;
        effect_position.x = x * TILESIZE + MAP_OFFSET_X + offset_x;
        effect_position.y = y * TILESIZE + MAP_OFFSET_Y + offset_y;

        effect_position.w = effect_position.h = EFFECT_SIZE;

        if(load_texture(&effect_texture, "Images/fires2.png") == 0) {
            fprintf(stderr, "Error, could not load texture : data/fires.png");
            exit(0);
        }
        SDL_QueryTexture(effect_texture.texture, NULL, NULL, &effect_texture.width, &effect_texture.height);

        frameWidth = effect_texture.width / FRAME_NUMBER_IN_WIDTH;
        frameHeight = effect_texture.height / FRAME_NUMBER_IN_HEIGTH;

        effect.x = effect.y = 0;

        effect.w = frameWidth;
        effect.h = frameHeight;
    }

    ~CEffect() {

    }
    void draw(isoEngineT* isoEngine);
};


#endif //SEABATTLE_CEFFECT_H
