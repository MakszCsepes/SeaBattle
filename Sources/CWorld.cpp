#include "CWorld.h"

const int R_back = 63;
const int G_back = 141;
const int B_back = 219;

coordinate get_rand_from_list(list<coordinate>& l) {
    auto iter = l.begin();
    int p = rand () % l.size();
    int i = 0;
    for(coordinate v : l) {
        if(i == p) {
//            cout << v.i << " " << v.j << endl;
            return v;
        }
        i++;
        iter++;
    }
}
void delete_by_value(list<coordinate>& l, coordinate val) {
    auto iter = l.begin();
    for(coordinate v : l) {
        if(v.i == val.i && v.j == val.j) {
            l.erase(iter);
            return;
        }
        iter++;
    }
}
coordinate get_rand_coords(list<coordinate>& l) {

    coordinate rand_coord = get_rand_from_list(l);
    delete_by_value(l, rand_coord);

    return rand_coord;
}
bool can_move (CShip* moved_ship ,coordinate& ship_coords) {

    if (moved_ship->get_inverse()) {
        if ((ship_coords.j < 0 || ship_coords.j + moved_ship->get_size() > MAP_CELL_HEIGHT) || (ship_coords.i < 0 || ship_coords.i > MAP_CELL_WIDTH) ) {
            return false;
        }
    } else {
        if ((ship_coords.j < 0 || ship_coords.j > MAP_CELL_HEIGHT) || (ship_coords.i + moved_ship->get_size() > MAP_CELL_WIDTH || ship_coords.i < 0) ) {
            return false;
        }
    }

    return true;
}

void CWorld::draw_text(char* text) {
    SDL_Color text_color = {0, 0, 0};

    surface = TTF_RenderText_Solid(font, text, text_color);
    texture = SDL_CreateTextureFromSurface(get_renderer(),surface);

    int w = 10;
    int h = 10;

    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    SDL_Rect dstrect = {10 + 50 , 10 + 50, w, h};

    SDL_RenderCopy(get_renderer(), texture, NULL, &dstrect);
}

void CWorld::draw_script_version(lua_State* L) {
    LuaRef version = getGlobal(L, "script_version");

    string vers =  version.cast<std::string>();
    char v[vers.length() + 1];
    strcpy(v, vers.c_str());

    draw_text(v);
}

void CWorld::draw(isoEngineT* isoEngine){
    SDL_SetRenderDrawColor(get_renderer(), R_back, G_back, B_back, 255);
    SDL_RenderClear(get_renderer());

    if (game_state == PUT_SHIPS) {

        user.draw(isoEngine);

        user.current_ship->draw(isoEngine);

    } else if (game_state == PLAY_GAME) {

        // first of all we draw maps and ships
        // PLAYER
        user.draw(isoEngine);
        // COMPUTER
        ai.draw(isoEngine);

        // then we put cursor on a map
        if (!user.map.cursor.get_hidden()) {
            user.map.cursor.draw(isoEngine);
        }

    } else if (game_state == ENDGAME) {

        // PLAYER
        user.draw(isoEngine);

        // COMPUTER
        ai.draw(isoEngine);
    }

//    draw_script_version(lua_state);

    string st = to_string(FPS);
    char* fps = new char[st.size() + 1];
    strcpy(fps, st.c_str());
    draw_text(fps);
    SDL_RenderPresent(get_renderer());
}

void CWorld::init_ai() {
    coordinate head_ship_coordinate = {0, 0};
    int inverse(HORIZONTAL);

    while (ai.get_init_status() == false) {

        if ((ai.current_ship = ai.get_new_ship())) {
            ai.current_ship->change_hidden();

            head_ship_coordinate = get_rand_coords(l);

            inverse = 0 + rand() % (1 - 0 + 1); // 1(true) - HORIZONTAL, 0(false) - VERTICAL
            if (inverse == VERTICAL) {
                ai.current_ship->change_inverse();
            }

            if (can_move(ai.current_ship, head_ship_coordinate)) {
                ai.current_ship->head_coordinate_x = head_ship_coordinate.j;
                ai.current_ship->head_coordinate_y = head_ship_coordinate.i;

                if (ai.can_put_ship()) {
                    ai.add_ship_to_player_array();
                }
            }
        } else {
            ai.change_inited();
        }

    }
}

void CWorld::change_turn() {
    turn = !turn;
}

Uint32 CWorld::get_timestamp_now() {
    return SDL_GetTicks();
}
// AI