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
void CCursor::draw(SDL_Renderer *renderer) {
    SDL_Rect cursor;

    cursor.x = position_x * CELL_SIZE + MAP_OFFSET_X + offset_x - 1;
    cursor.y = position_y * CELL_SIZE + MAP_OFFSET_Y + offset_y - 1;
    cursor.w = CELL_SIZE;
    cursor.h = CELL_SIZE;

    SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);

//    texture_renderer_XY_clip(&tilesTex, game.mousePoint.x - correct_X, game.mousePoint.y + correct_Y, &tiles_rect[0]);
    SDL_RenderDrawRect(renderer, &cursor);
}
void CCursor::change_hidden(){
    hidden = !hidden;
}