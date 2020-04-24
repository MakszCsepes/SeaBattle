#ifndef SEABATTLE_MAP_H
#define SEABATTLE_MAP_H

#include "cursor.h"

#define MAP_CELL_WIDTH 10
#define MAP_CELL_HEIGHT 10

#define WATER_AREA 0
#define MISHIT  1
#define HIT 2

#define USER_MAP_OFFSET_X 0
#define USER_MAP_OFFSET_Y 100
#define AI_MAP_OFFSET_X 435
#define AI_MAP_OFFSET_Y 100

const int EMPTY_CELL = 0;
const int PALUBA_CELL = 1;
const int MISHIT_CELL = 2;
const int HIT_PALUBA_CELL = 3;
const int KILLED_PALUBA_CELL = 4;

class CMap : public IDrawable {
public:
    int offset_x;
    int offset_y;

    CCursor cursor;

    int** cells_array;

    CMap() {
        create_map();

        cursor.position_y = 0;
        cursor.position_x = 0;

        offset_x = 0;
        offset_y = 0;
    }
    CMap(int offset_x, int offset_y, int map_x, int map_y, CCursor& cur) {
        this->offset_x = offset_x;
        this->offset_y = offset_y;

        cursor = cur;
        create_map();
    }
    ~CMap() {
        delete_map();
    }

    void create_map();
    void delete_map();
    void draw(SDL_Renderer*) override;
    void draw_mishit(SDL_Renderer*, int&, int&);

    CMap& operator = (const CMap& obj) {
        cursor = obj.cursor;
        cursor = obj.cursor;

        offset_y = obj.offset_y;
        offset_x = obj.offset_x;

        for(int i = 0 ; i < MAP_CELL_HEIGHT ; i++) {
            for(int j = 0 ; j < MAP_CELL_WIDTH ; j++) {
                cells_array[i][j] = obj.cells_array[i][j];
            }
        }

        return *this;
    }
    int* operator [] (int i) {
        return cells_array[i];
    }
};


#endif //SEABATTLE_MAP_H
