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
    CEffect(): CAnimation() {
        *this = CEffect(0, 0, 0, 0, "Resources/Images/fires2.png", false);
    }
    CEffect(int x, int y, int offset_x, int offset_y, char* sprites_filename, bool is_animating): CAnimation(is_animating) {
        FrameTime = 0;
        effect.x = effect.y = 0;

        init_effect_position(x, y, offset_x, offset_y);

        init_effect(x, y, offset_x, offset_y, sprites_filename);
    }
    ~CEffect() {

    }

    void draw(isoEngineT* isoEngine);
    void set_sprites(char* sprites_filename) {
        if(load_texture(&effect_texture, sprites_filename) == 0) {
            fprintf(stderr, "Error, could not load texture : %s", sprites_filename);
            exit(0);
        }
        SDL_QueryTexture(effect_texture.texture, NULL, NULL, &effect_texture.width, &effect_texture.height);
    }
    void init_effect_position(int x, int y, int offset_x, int offset_y) {
        effect_position.x = x * TILESIZE + MAP_OFFSET_X + offset_x;
        effect_position.y = y * TILESIZE + MAP_OFFSET_Y + offset_y;

        effect_position.w = effect_position.h = EFFECT_SIZE;
    }
    void init_effect(int x, int y, int offset_x, int offset_y, char* sprites_filename) {

        set_sprites(sprites_filename);

        init_frame();
    }
    void init_frame() {
        frameWidth = effect_texture.width / FRAME_NUMBER_IN_WIDTH;
        frameHeight = effect_texture.height / FRAME_NUMBER_IN_HEIGTH;

        effect.w = frameWidth;
        effect.h = frameHeight;
    }
};


#endif //SEABATTLE_CEFFECT_H
