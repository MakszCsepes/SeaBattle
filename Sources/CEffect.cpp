#include "CEffect.h"

void CEffect::draw(isoEngineT* isoEngine) {
    point2DT point;
    FrameTime++;

    if(FrameTime == 2) {
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