#include "CEffect.h"

void CEffect::draw(isoEngineT* isoEngine) {
    point2DT point;
    FrameTime++;

    if(FrameTime == 12) {
        FrameTime = 0;

        effect.x += frameWidth;
        if(effect.x >= effect_texture.width) {
            effect.y += frameHeight;
            effect.x = 0;

            if (effect.y >= effect_texture.height) {
                effect.y = 0;
            }
        }
    }

    point.x = effect_position.x + isoEngine->scrollX;
    point.y = effect_position.y + isoEngine->scrollY;


    Converter2DToIso(&point);
    SDL_Rect eff_pos;
    eff_pos.h = effect_position.h;
    eff_pos.w = effect_position.w;
    eff_pos.x = point.x + 15;
    eff_pos.y = point.y + 15;

    SDL_RenderCopy(get_renderer(), effect_texture.texture, &effect, &eff_pos);
}

void CEffect::set_sprites(char *sprites_filename) {
    if(load_texture(&effect_texture, sprites_filename) == 0) {
        fprintf(stderr, "Error, could not load texture : %s", sprites_filename);
        exit(0);
    }
    SDL_QueryTexture(effect_texture.texture, NULL, NULL, &effect_texture.width, &effect_texture.height);
}

void CEffect::init_effect_position(int x, int y, int offset_x, int offset_y) {
    effect_position.x = x * TILESIZE + MAP_OFFSET_X + offset_x;
    effect_position.y = y * TILESIZE + MAP_OFFSET_Y + offset_y;

    effect_position.w = effect_position.h = EFFECT_SIZE;
}

void CEffect::init_effect(char *sprites_filename) {
    set_sprites(sprites_filename);

    init_frame();
}

void CEffect::init_frame() {
    frameWidth = effect_texture.width / FRAME_NUMBER_IN_WIDTH;
    frameHeight = effect_texture.height / FRAME_NUMBER_IN_HEIGTH;

    effect.w = frameWidth;
    effect.h = frameHeight;
}