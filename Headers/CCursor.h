#ifndef SEABATTLE_CCURSOR_H
#define SEABATTLE_CCURSOR_H

#include "IInterface.h"

const int MAP_OFFSET_X = 50;
const int MAP_OFFSET_Y = 100;
const int CELL_SIZE = 40;

class CCursor : public IDrawable{
    int offset_x;
    int offset_y;

    bool hidden;
public:
    int position_x;
    int position_y;

    CCursor() {
        position_x = 0;
        position_y = 0;

        offset_x = 0;
        offset_y = 0;

        hidden = true;
    }
    CCursor(int off_x, int off_y) {
        position_y = 0;
        position_x = 0;

        offset_x = off_x;
        offset_y = off_y;

        hidden = true;
    }

    void draw(isoEngineT*);
    void change_hidden();
    bool get_hidden();

    CCursor& operator =(const CCursor& cursor_source) {
        offset_x = cursor_source.offset_x;
        offset_y = cursor_source.offset_y;

        position_y = cursor_source.position_y;
        position_x = cursor_source.position_x;

        hidden = cursor_source.hidden;

        return *this;
    }
};

#endif //SEABATTLE_CCURSOR_H
