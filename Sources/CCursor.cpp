#include "CCursor.h"

bool CCursor::get_hidden() {
    return hidden;
}

void CCursor::draw(isoEngineT* isoEngine) {
    point2DT point;
    point.x = (position_x*TILESIZE) + isoEngine->scrollX + MAP_OFFSET_X + offset_x;
    point.y = (position_y*TILESIZE) + isoEngine->scrollY + MAP_OFFSET_Y + offset_y;

    Converter2DToIso(&point);

    texture_renderer_XY_clip(&tilesTex, point.x, point.y,&tiles_rect[0]);
}
void CCursor::change_hidden(){
    hidden = !hidden;
}