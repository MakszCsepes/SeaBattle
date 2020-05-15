#ifndef SEABATTLE_CEFFECT_H
#define SEABATTLE_CEFFECT_H

#include "CAnimation.h"

const int EFFECT_SIZE = 32;
const int FRAME_NUMBER_IN_WIDTH = 8;
const int FRAME_NUMBER_IN_HEIGTH = 4;

class CEffect : public IDrawable, public CAnimation{
    int FrameTime;

    SDL_Rect effect;
    SDL_Rect effect_position;

    textureT effect_texture;

    int frameWidth;
    int frameHeight;
public:
    CEffect() = delete;
    CEffect(int x, int y, int offset_x, int offset_y, char* sprites_filename, bool is_animating): CAnimation(is_animating) {
        FrameTime = 0;
        effect.x = effect.y = 0;

        init_effect_position(x, y, offset_x, offset_y);

        init_effect(sprites_filename);
    }
    ~CEffect() {

    }

    void draw(isoEngineT* isoEngine);
    void set_sprites(char* sprites_filename);
    void init_effect_position(int x, int y, int offset_x, int offset_y);
    void init_effect(char* sprites_filename);
    void init_frame();
};


#endif //SEABATTLE_CEFFECT_H
