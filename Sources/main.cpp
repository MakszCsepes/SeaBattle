#include <iostream>
#include "CWorld.h"
#include <time.h>

using namespace luabridge;

void printMessage(const std::string& s) {
    std::cout << s << " ";
}
void init_list(list<coordinate>& l) {
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

    luaL_dofile(L, "Resources/Lua Scripts/script 101 .lua");
    lua_pcall(L, 0, 0, 0);
    return L;
}
CWorld* get_game(){
    // init user
    CCursor u_cursor(AI_MAP_OFFSET_X, AI_MAP_OFFSET_Y);
    CMap user_map(USER_MAP_OFFSET_X, USER_MAP_OFFSET_Y, u_cursor);
    char u_name[15];
    strcpy(u_name, "max");
    CPlayer user(u_name, user_map);

    // init ai
    CCursor ai_cursor(USER_MAP_OFFSET_X, USER_MAP_OFFSET_Y);
    CMap ai_map(AI_MAP_OFFSET_X, AI_MAP_OFFSET_Y, ai_cursor);
    char ai_name[15];
    strcpy(ai_name, "AI");
    CPlayer computer(ai_name, ai_map);

    int game_state(PUT_SHIPS);

    lua_State* L(get_new_script());

    list<coordinate> l;
    init_list(l);

    return new CWorld(user, computer, game_state, L, l);
}

gameT gameT1;
SDL_Rect tiles_rect[NUM_ISO_TILES];
textureT tilesTex;
CWorld* world_game;
void init() {
    gameT1.loop_done = 0;

    init_tile_clip(tiles_rect, &tilesTex, 64, 80, NUM_ISO_TILES);
    CShip::load_textures();
    init_IsoEngine(&gameT1.isoEngine);

    gameT1.isoEngine.scrollX = 200;
    gameT1.isoEngine.scrollY = -200;
    gameT1.mapScroll2Dpos.x = -200;
    gameT1.mapScroll2Dpos.y = 0;

    gameT1.mapScrollSpeed = 3;

    if(load_texture(&tilesTex, "Resources/Images/isotiles_sb_2.png") == 0) {
        fprintf(stderr, "Error, could not load texture : Images/isotiles_sb.png");
        exit(0);
    }
}

void select_state(CWorld* game) {
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
            game->draw(&gameT1.isoEngine);
            break;
        default:
            game->run_game = false;
    }
}

void update_input(CWorld* game, SDL_Event event) {
    if (game->turn == AI_TURN) {
        game->ai.assign_new_hit_coords_from(game->lua_state, game->user);
        if (!game->ai.was_ever_hit_on_the_position(game->user.map,game->ai.map.cursor.position_x,game->ai.map.cursor.position_y)) {

            game->ai.do_hit(game->user);

            if(!game->ai.get_aim_status()) {
                game->change_turn();
            }
            SDL_Delay(2000);
            return;
        }
        return;
    }
    
    while (SDL_PollEvent(&event)) {

        // KEYBOARD EVENTS
        switch (event.type) {
            case SDL_QUIT:
                game->change_run_game();
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT :

                        if (game->game_state == PUT_SHIPS) {
                            game->user.current_ship->move(game->user.current_ship->get_current_x() + 1,
                                                          game->user.current_ship->get_current_y());
                        } else if (game->game_state == PLAY_GAME) {
                            game->user.map.cursor.position_x++;
                            if (game->user.map.cursor.position_x > MAP_CELL_HEIGHT - 1) {
                                game->user.map.cursor.position_x = 0;
                            }
                        }

                        break;
                    case SDLK_LEFT :

                        if (game->game_state == PUT_SHIPS) {
                            game->user.current_ship->move(game->user.current_ship->get_current_x() - 1,
                                                          game->user.current_ship->get_current_y());
                        } else if (game->game_state == PLAY_GAME) {
                            game->user.map.cursor.position_x--;
                            if (game->user.map.cursor.position_x < 0) {
                                game->user.map.cursor.position_x = MAP_CELL_HEIGHT - 1;
                            }
                        }
                        break;
                    case SDLK_DOWN :

                        if (game->game_state == PUT_SHIPS) {
                            game->user.current_ship->move(game->user.current_ship->get_current_x(),
                                                          game->user.current_ship->get_current_y() + 1);
                        } else if (game->game_state == PLAY_GAME){
                            game->user.map.cursor.position_y++;
                            if (game->user.map.cursor.position_y > MAP_CELL_WIDTH - 1) {
                                game->user.map.cursor.position_y = 0;
                            }
                        }
                        break;
                    case SDLK_UP :

                        if (game->game_state == PUT_SHIPS) {
                            game->user.current_ship->move(game->user.current_ship->get_current_x(),
                                                          game->user.current_ship->get_current_y() - 1);
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

                        game->change_run_game();
                    case SDLK_SPACE:

                        if (game->game_state == PUT_SHIPS) {
                            if (game->user.can_put_ship()) {
                                game->user.add_ship_to_player_list();

                                game->user.get_new_ship_for_list();

                                if(game->user.current_ship == nullptr) {
                                    game->user.change_inited();
                                    game->user.map.cursor.change_hidden();

                                    game->init_ai();
                                }
                            }
                        } else if (game->game_state == PLAY_GAME) {
                            if(game->turn == USER_TURN) {
                                if (!game->user.was_ever_hit_on_the_position(game->ai.map,
                                                                            game->user.map.cursor.position_x,
                                                                            game->user.map.cursor.position_y)) {
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
    }

    select_state(game);
}

int main(int argc, char* argv[]) {
    srand(time(0));
    init_SDL("Sea_Battle");
    TTF_Init();
    init();

    SDL_Event event;

    world_game = new CWorld(get_game());
    Uint32 frames;
    Uint32 timeStamps;

    while (world_game->run_game) {
        world_game->frame_count++;

        frames = world_game->frame_count - world_game->old_frame_count;
        if(frames >= 100) {
            timeStamps = world_game->get_timestamp_now() - world_game->time_stamp;
            world_game->FPS = (frames * 1000.0) / timeStamps;

            world_game->old_frame_count = world_game->frame_count;

            world_game->time_stamp = world_game->get_timestamp_now();
        }

        update_input(world_game, event);
        world_game->draw(&gameT1.isoEngine);
    }

    delete world_game;
    close_down_SDL();
    return 0;
}