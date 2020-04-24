#include <iostream>
#include "World.h"

using namespace luabridge;

void printMessage(const std::string& s) {
    std::cout << s << " ";
}
void init_list(list<coordinate>& l) {
    list<coordinate> :: iterator iter;
    for(int i = 0 ; i < SHIP_QUANTITY ; i++) {
        for(int j = 0 ; j < SHIP_QUANTITY ; j++) {
            coordinate p = {i, j};
            l.push_back(p);
        }
    }
}
lua_State* get_new_script() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    getGlobalNamespace(L).addFunction("printMessage", printMessage);

    luaL_dofile(L, "../script 101 .lua");
    lua_pcall(L, 0, 0, 0);
    return L;
}
CWorld* get_game(){
    CCursor u_cursor(AI_MAP_OFFSET_X, USER_MAP_OFFSET_Y);
    CMap user_map(USER_MAP_OFFSET_X, USER_MAP_OFFSET_Y, 0, 0, u_cursor);
    char name[15];
    strcpy(name, "undefined");
    CPlayer user(name, user_map);

    CCursor ai_cursor(USER_MAP_OFFSET_X, USER_MAP_OFFSET_Y);
    CMap ai_map(AI_MAP_OFFSET_X, AI_MAP_OFFSET_Y, 0, 0, ai_cursor);
    strcpy(name, "AI");
    CPlayer computer(name, ai_map);

    int game_state(PUT_SHIPS);

    lua_State* L(get_new_script());

    list<coordinate> l;
    init_list(l);

    return new CWorld(user, computer, game_state, L, l);
}

int main() {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return 0;
    }

    srand(time(0));
/*
    SDL_Window *window = SDL_CreateWindow("NavalBattle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);*/
    SDL_Window *window = SDL_CreateWindow("NavalBattle",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_Event event;

    CWorld* game = new CWorld(get_game());
    bool run_game = true;

    while (run_game) {

        while (SDL_PollEvent(&event)) {
            game->draw(renderer);
            // KEYBOARD EVENTS
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_RIGHT :

                            if (game->game_state == PUT_SHIPS) {
                                game->user.current_ship->head_coordinate_x++;
                                if (game->user.current_ship->get_inverse() == HORIZONTAL) {
                                    if (game->user.current_ship->head_coordinate_x + game->user.current_ship->get_size() > MAP_CELL_WIDTH) {
                                        game->user.current_ship->head_coordinate_x = 0;
                                    }
                                } else {
                                    if (game->user.current_ship->head_coordinate_x > MAP_CELL_WIDTH - 1) {
                                        game->user.current_ship->head_coordinate_x = 0;
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME) {
                                game->user.map.cursor.position_x++;
                                if ( game->user.map.cursor.position_x > MAP_CELL_HEIGHT - 1) {
                                    game->user.map.cursor.position_x = 0;
                                }
                            }
                            break;
                        case SDLK_LEFT :

                            if (game->game_state == PUT_SHIPS) {
                                game->user.current_ship->head_coordinate_x--;

                                if (game->user.current_ship->head_coordinate_x < 0) {
                                    if (game->user.current_ship->get_inverse() == HORIZONTAL) {
                                        game->user.current_ship->head_coordinate_x = MAP_CELL_WIDTH - game->user.current_ship->get_size();
                                    } else {
                                        game->user.current_ship->head_coordinate_x = MAP_CELL_WIDTH - 1;
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME) {
                                game->user.map.cursor.position_x--;
                                if (game->user.map.cursor.position_x < 0) {
                                    game->user.map.cursor.position_x = MAP_CELL_HEIGHT - 1;
                                }
                            }
                            break;
                        case SDLK_DOWN :

                            if (game->game_state == PUT_SHIPS) {
                                game->user.current_ship->head_coordinate_y++;


                                if (game->user.current_ship->get_inverse() == HORIZONTAL) {
                                    if (game->user.current_ship->head_coordinate_y > MAP_CELL_WIDTH - 1) {
                                        game->user.current_ship->head_coordinate_y = 0;
                                    }
                                } else {
                                    if (game->user.current_ship->head_coordinate_y + game->user.current_ship->get_size() > MAP_CELL_WIDTH ) {
                                        game->user.current_ship->head_coordinate_y = 0;
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME){
                                game->user.map.cursor.position_y++;
                                if (game->user.map.cursor.position_y > MAP_CELL_WIDTH - 1) {
                                    game->user.map.cursor.position_y = 0;
                                }
                            }
                            break;
                        case SDLK_UP :

                            if (game->game_state == PUT_SHIPS) {
                                game->user.current_ship->head_coordinate_y--;
                                if (game->user.current_ship->get_inverse() == HORIZONTAL) {
                                    if ( game->user.current_ship->head_coordinate_y < 0) {
                                        game->user.current_ship->head_coordinate_y = MAP_CELL_WIDTH - 1;
                                    }
                                } else {
                                    if (game->user.current_ship->head_coordinate_y < 0) {
                                        game->user.current_ship->head_coordinate_y = MAP_CELL_WIDTH - game->user.current_ship->get_size();
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME) {
                                game->user.map.cursor.position_y--;
                                if (game->user.map.cursor.position_y < 0) {
                                    game->user.map.cursor.position_y = MAP_CELL_WIDTH - 1;
                                }
                            }
                            break;
                        case SDLK_r:
                            game->lua_state = get_new_script();
                            break;
                        case SDLK_i:
                            if (game->game_state == PUT_SHIPS) {
                                game->user.current_ship->change_inverse();
                            }
                            break;
                        case SDLK_ESCAPE:
                            return 0;
                        case SDLK_SPACE:
                            if (game->game_state == PUT_SHIPS) {
                                if (game->user.can_put_ship()) {
                                    game->user.add_ship_to_player_array();

                                    if((game->user.current_ship = game->user.get_new_ship()) == nullptr) {
                                        game->user.change_inited();
                                        game->user.map.cursor.change_hidden();

                                        game->init_ai();
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME) {
                                if(game->turn == USER_TURN) {
                                    if (game->user.was_ever_hit_on_the_position(game->ai.map,game->user.map.cursor.position_x,game->user.map.cursor.position_y) == false) {
                                        game->user.do_hit(game->ai);
                                        if (!game->user.get_aim_status()) {
                                            game->change_turn();
                                        }
                                    }
                                }
                            }
                            break;
                        default :
                            break;
                    }
                    break;
                default:
                    break;
            }

            if (game->turn == AI_TURN) {
                game->ai.assign_new_hit_coords_from(game->lua_state, game->user);
                if (game->ai.was_ever_hit_on_the_position(game->user.map,game->ai.map.cursor.position_x,game->ai.map.cursor.position_y) == false) {
                    game->ai.do_hit(game->user);

                    if(!game->ai.get_aim_status()) {
                        SDL_Delay(200);
                        game->change_turn();
                    }
                }
            }
            // SELECT STATE
            switch (game->game_state) {
                case PUT_SHIPS:
                    if (game->user.get_init_status() and game->ai.get_init_status()) {
                        game->game_state = PLAY_GAME;
                    }
                    break;
                case PLAY_GAME:
                    if(game->user.get_points() == 20 or game->ai.get_points() == 20) {
                        game->game_state = ENDGAME;
                        break;
                    }
                    break;
                case ENDGAME:
                    game->draw(renderer);
                    break;
                default:
                    run_game = false;
            }
        }

    }
    return 0;
}