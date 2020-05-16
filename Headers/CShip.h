//
// Created by max on 21/1/2020.
//

#ifndef SEABATTLE_CSHIP_H
#define SEABATTLE_CSHIP_H

#include "CMap.h"
#include <list>

#define HORIZONTAL true
#define VERTICAL false

#define SHIP_QUANTITY 10

#define SUBMARINE_QUANTITY 4
#define SUBMARINE_SIZE 1
extern SDL_Rect submarine_rect[2];
extern textureT submarineTex;

#define DESTROYER_QUANTITY 3
#define DESTROYER_SIZE 2
extern SDL_Rect destroyer_rect[2];
extern textureT destroyerTex;

#define CRUISER_QUANTITY 2
#define CRUISER_SIZE 3
extern SDL_Rect cruiser_rect[2];
extern textureT cruiserTex;

#define BATTLESHIP_QUANTITY 1
#define BATTLESHIP_SIZE 4
extern SDL_Rect battleship_rect[2];
extern textureT battleshipTex;

static const int SEC_0 = 0b0000;
static const int SEC_1 = 0b0001;
static const int SEC_2 = 0b0010;
static const int SEC_3 = 0b0100;
static const int SEC_4 = 0b1000;

class CShip : public IDrawable, public CAnimation {
    int size;
    int damage_level;
    int offset_x;
    int offset_y;

    bool is_selected;
    bool inverse;
    bool hidden;

    list<CEffect> effects;
public:
    int head_coordinate_x;
    int head_coordinate_y;
    static int section_array[];

    CShip() {
        head_coordinate_x = 0;
        head_coordinate_y = 0;

        inverse = HORIZONTAL;
        is_selected = true;
        hidden = false;

        damage_level = SEC_0;

        size = 0;
        offset_x = 0;
        offset_y = 0;
    }
    CShip(int size, int offset_x, int offset_y): CShip() {
        this->offset_x = offset_x;
        this->offset_y = offset_y;

        this->size = size;
    }
    CShip(const CShip& obj) {
        size = obj.size;
        is_selected = obj.is_selected;
        inverse = obj.inverse;
        hidden = obj.hidden;

        damage_level = obj.damage_level;

        head_coordinate_x = obj.head_coordinate_x;
        head_coordinate_y = obj.head_coordinate_y;
    }

    bool get_hidden();
    bool get_inverse();
    bool is_dead();
    int get_size();
    int get_damage();

    int get_current_x();
    int get_current_y();

    void change_selected();
    void change_inverse();
    void change_hidden();
    bool has_the_coordinate(int& x, int& y);

    int get_palub_number(int x, int y);
    void put_ship_on_map(int** map_array);
    void add_hit_palub(int x, int y);
    void change_effects();
    void update_state(int) override ;
    CShip& operator =(const CShip& ship_source) {
        this->size = ship_source.size;

        this->offset_x = ship_source.offset_x;
        this->offset_y = ship_source.offset_y;
        this->inverse = ship_source.inverse;
        this->is_selected = ship_source.is_selected;
        this->hidden = ship_source.hidden;
        this->damage_level = ship_source.damage_level;

        this->head_coordinate_x = ship_source.head_coordinate_x;
        this->head_coordinate_y = ship_source.head_coordinate_y;

        return *this;
    }

    void move(int, int);
    void add_effect(const CEffect&);
    void draw(isoEngineT*);
    void draw_effects(isoEngineT*);
};

#endif //SEABATTLE_CSHIP_H
