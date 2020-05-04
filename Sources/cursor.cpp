#include "cursor.h"

bool CCursor::get_hidden() {
    return hidden;
}

// Reccent draw()
/*void CCursor::draw(SDL_Renderer *renderer) {
    SDL_Rect cursor;

    cursor.x = position_x * CELL_SIZE + MAP_OFFSET_X + offset_x - 1;
    cursor.y = position_y * CELL_SIZE + MAP_OFFSET_Y + offset_y - 1;
    cursor.w = CELL_SIZE;
    cursor.h = CELL_SIZE;

    SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);

    SDL_RenderDrawRect(renderer, &cursor);
}*/
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