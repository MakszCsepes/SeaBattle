#ifndef SEABATTLE_CMAP_H
#define SEABATTLE_CMAP_H

#include "CEffect.h"
#include <iostream>

using namespace std;
#define MAP_CELL_WIDTH 10
#define MAP_CELL_HEIGHT 10

#define WATER_AREA 0
#define MISHIT  1
#define HIT 2

#define USER_MAP_OFFSET_X 0
#define USER_MAP_OFFSET_Y 0
#define AI_MAP_OFFSET_X 400
#define AI_MAP_OFFSET_Y 0

const int EMPTY_CELL = 0;
const int MISHIT_CELL = 1;
const int PALUBA_CELL = 2;
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
    CMap(int offset_x, int offset_y, CCursor& cur) {
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
    void draw(isoEngineT*);
    void draw_mishit(SDL_Renderer*, int&, int&);

    CMap& operator = (const CMap& obj) {
        cursor = obj.cursor;

        offset_y = obj.offset_y;
        offset_x = obj.offset_x;

        int val;
        for(int i = 0 ; i < MAP_CELL_HEIGHT ; i++) {
            for(int j = 0 ; j < MAP_CELL_WIDTH ; j++) {
                val = obj.cells_array[i][j];
                cells_array[i][j] = val;
            }
        }

        return *this;
    }
    int* operator [] (int i) {
        return cells_array[i];
    }
};


#endif //SEABATTLE_CMAP_H
