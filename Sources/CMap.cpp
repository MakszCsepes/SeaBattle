#include "CMap.h"

void put_cell (isoEngineT* isoEngine, int& map_offset_x, int& map_offset_y, int& i , int& j, int& tile_number) {
    point2DT point;

    point.x = (j*TILESIZE) + isoEngine->scrollX + MAP_OFFSET_X + map_offset_x;
    point.y = (i*TILESIZE) + isoEngine->scrollY + MAP_OFFSET_Y + map_offset_y;

    Converter2DToIso(&point);

    texture_renderer_XY_clip(&tilesTex, point.x, point.y, &tiles_rect[tile_number]);
}

void CMap::create_map()  {
    cells_array = new int*[MAP_CELL_HEIGHT];
    for (int i = 0; i < MAP_CELL_HEIGHT; i++) {
        this->cells_array[i] = new int[MAP_CELL_WIDTH];

        for (int j = 0; j < MAP_CELL_WIDTH; j++) {
            cells_array[i][j] = EMPTY_CELL;
        }
    }
}

void CMap::delete_map() {
    for(int i = 0 ; i < MAP_CELL_HEIGHT ; i++) {
        delete[] cells_array[i] ;
    }
    delete [] cells_array;
}

void CMap::draw(isoEngineT* isoEngine) {

    for (int i = 0 ; i < MAP_CELL_WIDTH ; i++) {
        for (int j = 0 ; j < MAP_CELL_HEIGHT ; j++) {

            put_cell(isoEngine, offset_x, offset_y, i, j, cells_array[i][j]);
        }
    }
}
