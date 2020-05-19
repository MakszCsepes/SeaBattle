#include "CWorld.h"

const int R_back = 63;
const int G_back = 141;
const int B_back = 219;

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

void CWorld::draw_text(char* text, int x, int y, SDL_Color text_color) {
    text_surface = TTF_RenderText_Solid(font, text, text_color);
    text_texture = SDL_CreateTextureFromSurface(get_renderer(), text_surface);

    int w = 1;
    int h = 1;

    SDL_QueryTexture(text_texture, NULL, NULL, &w, &h);

    SDL_Rect text_rect = {x, y, w, h};

    SDL_RenderCopy(get_renderer(), text_texture, NULL, &text_rect);
}
void CWorld::draw_FPS() {
    SDL_Color text_color = {50, 50, 10};

    string st = to_string(float(FPS));
    char* fps = new char[st.size() + 1];
    strcpy(fps, st.c_str());

    draw_text(fps, 50, 50, text_color);
}
void CWorld::draw_script_version(lua_State* L) {
    LuaRef version = getGlobal(L, "script_version");

    string vers =  version.cast<std::string>();
    char v[vers.length() + 1];
    strcpy(v, vers.c_str());

    SDL_Color text_color = {250, 250, 250};
    draw_text(v, SCRIPT_VERSION_POSITION_X, SCRIPT_VERSION_POSITION_Y, text_color);
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

    draw_script_version(lua_state);
    draw_FPS();

    SDL_RenderPresent(get_renderer());
}

void CWorld::change_turn() {
    turn = !turn;
}
void CWorld::change_run_game() {
    run_game = !run_game;
}

Uint32 CWorld::get_timestamp_now() {
    return SDL_GetTicks();
}

int CWorld::get_current_frame() {
    return frame_count;
}

// AI
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
coordinate get_rand_from_list(list<coordinate>& l) {
    auto iter = l.begin();
    int p = rand () % l.size();
    int i = 0;
    for(coordinate v : l) {
        if(i == p) {
            return v;
        }
        i++;
        iter++;
    }
}
coordinate get_rand_coords(list<coordinate>& l) {

    coordinate rand_coord = get_rand_from_list(l);
    delete_by_value(l, rand_coord);

    return rand_coord;
}

void CWorld::init_ai() {
    coordinate head_ship_coordinate = {0, 0};
    int inverse(HORIZONTAL);

    while (!ai.get_init_status()) {

        ai.get_new_ship_for_list();
        if ((ai.current_ship)) {
            ai.current_ship->change_hidden();

            head_ship_coordinate = get_rand_coords(l);

            inverse = 0 + rand() % (1 - 0 + 1); // 1(true) - HORIZONTAL, 0(false) - VERTICAL
            if (inverse == VERTICAL) {
                ai.current_ship->change_inverse();
            }

            if (can_move(ai.current_ship, head_ship_coordinate)) {
                ai.current_ship->move(head_ship_coordinate.j, head_ship_coordinate.i);

                if (ai.can_put_ship()) {
                    ai.add_ship_to_player_list();
                }
            }
        } else {
            ai.change_inited();
        }
    }
}