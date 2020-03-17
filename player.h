//
// Created by max on 21/1/2020.
//

#ifndef SEABATTLE_PLAYER_H
#define SEABATTLE_PLAYER_H

#include "ship.h"
#include <cstring>

class CPlayer : public IDrawable {
    char name[15];

    int points;

    bool inited;
    int ship_quantity;

    CShip* get_battleship();
    CShip* get_destroyer();
    CShip* get_cruiser();
    CShip* get_submarine();
public:
    CShip* ships;
    CShip* current_ship;
    CMap map;

    CPlayer() {
        strcpy(name, "undefined");

        points = 0;

        inited = false;

        ship_quantity = 0;
        ships = new CShip[ship_quantity];
    }
    CPlayer(char name[], CMap& Map) {
        strcpy(this->name, name);

        points = 0;
        ship_quantity = 0;

        inited = false;
        this->map = Map;

        current_ship = get_new_ship();
        ships = new CShip[ship_quantity];
    }

    void add_ship_to_player_array();
    void set_name(char*);
    void increase_points();
    void change_inited();
    bool get_player_init_status();
    int get_points();

    CShip* get_new_array(CShip* added_ship);
    bool check_collision (CShip&, CShip);
    bool can_put_ship();
    bool any_ship_damaged_on_position(int&, int&);
    CShip* get_new_ship();
    void do_hit(int x, int y);
    CPlayer& operator =(const CPlayer& player_source) {
        strcpy(name, player_source.name);

        map = player_source.map;
        points = player_source.points;

        current_ship = player_source.current_ship;
        ship_quantity = player_source.ship_quantity;

        for(int i = 0 ; i < ship_quantity ; i++) {
            ships[i] = player_source.ships[i];
        }

        return *this;
    }

    void draw(SDL_Renderer*) override;
    int** generate_state_map();
};

#endif //SEABATTLE_PLAYER_H
