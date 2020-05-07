#include "CEffect.h"

void CEffect::draw(isoEngineT* isoEngine) {
    point2DT point;
    FrameTime++;


    if(FPS / FrameTime == 12) {
        FrameTime = 0;

        fire.x += frameWidth;
        if(fire.x >= fire_texture.width) {
            fire.y += frameHeight;
            fire.x = 0;

            if (fire.y >= fire_texture.height) {
                fire.y = 0;
            }
        }
    }

    point.x = fire_position.x + isoEngine->scrollX;
    point.y = fire_position.y + isoEngine->scrollY;

    Converter2DToIso(&point);

    fire_position.x = point.x;
    fire_position.y = point.y;

    SDL_RenderCopy(get_renderer(), fire_texture.texture, &fire, &fire_position);
}