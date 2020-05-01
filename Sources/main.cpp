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
    char u_name[15];
    strcpy(u_name, "undefined");
    CPlayer user(u_name, user_map);

    CCursor ai_cursor(USER_MAP_OFFSET_X, USER_MAP_OFFSET_Y);
    CMap ai_map(AI_MAP_OFFSET_X, AI_MAP_OFFSET_Y, 0, 0, ai_cursor);
    char ai_name[15];
    strcpy(ai_name, "AI");
    CPlayer computer(ai_name, ai_map);

    int game_state(PUT_SHIPS);

    lua_State* L(get_new_script());

    list<coordinate> l;
    init_list(l);

    return new CWorld(user, computer, game_state, L, l);
}

#define MAP_HEIGHT 10
#define MAP_WIDTH 10

int worldTest[MAP_HEIGHT][MAP_WIDTH] = {
        {1, 1, 2, 2, 3, 1, 4, 1, 2, 3},
        {1, 1, 2, 2, 3, 1, 4, 1, 2, 3},
        {1, 1, 2, 2, 3, 1, 4, 1, 2, 3},
        {1, 1, 2, 2, 3, 1, 4, 1, 2, 3},
        {1, 1, 2, 2, 3, 1, 4, 1, 2, 3},
        {1, 1, 2, 2, 3, 1, 4, 1, 2, 3},
        {1, 1, 2, 2, 3, 1, 4, 1, 2, 3},
        {1, 1, 2, 2, 3, 1, 4, 1, 2, 3},
        {1, 1, 2, 2, 3, 1, 4, 1, 2, 3},
        {1, 1, 2, 2, 3, 1, 4, 1, 2, 3},
};
typedef struct gameT {
    SDL_Event event;
    int loop_done;
    SDL_Rect mouseRect;
    point2DT mousePoint;
    point2DT mapScroll2Dpos;
    int mapScrollSpeed;
    isoEngineT isoEngine;
    int lastTiledClick;
}gameT;
gameT gameT1;

SDL_Rect tiles_rect[NUM_ISO_TILES];
textureT tilesTex;


void setupRect(SDL_Rect* rect, int x, int y, int w, int h) {
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}
void init_tile_clip() {
    int x = 0;
    int y = 0;

    texture_init(&tilesTex, 0, 0, 0, NULL, NULL, SDL_FLIP_NONE);
    for (int i = 0; i < NUM_ISO_TILES; i++) {
        setupRect(&tiles_rect[i], x, y, 64, 80);
        x += 64;
    }
}
void init() {
    gameT1.loop_done = 0;

    init_tile_clip();
    init_IsoEngine(&gameT1.isoEngine, 32);
    IsoEngineSetMapSize(&gameT1.isoEngine, MAP_WIDTH, MAP_HEIGHT);

    gameT1.isoEngine.scrollX = 77;
    gameT1.isoEngine.scrollY = -77;
    gameT1.mapScroll2Dpos.x = -77;
    gameT1.mapScroll2Dpos.y = 0;

    gameT1.mapScrollSpeed = 3;

    gameT1.lastTiledClick = -1;

    if(load_texture(&tilesTex, "data/isotiles.png") == 0) {
        fprintf(stderr, "Error, could not load texture : data/isotiles.png");
        exit(0);
    }
}

void drawIsoMouse() {
    int correct_X = (gameT1.mapScroll2Dpos.x%(TILESIZE/2)) * 2;
    int correct_Y = (gameT1.mapScroll2Dpos.y%(TILESIZE/2)) ;

    gameT1.mousePoint.x = (gameT1.mouseRect.x/TILESIZE)*TILESIZE;
    gameT1.mousePoint.y = (gameT1.mouseRect.y/TILESIZE)*TILESIZE;

    // For very other x`position on the map
    if((gameT1.mousePoint.x/TILESIZE)%2) {
        // Move mouse down by half a tile
        // so we can pick isometric tiles on that row as well
        gameT1.mousePoint.y += TILESIZE*0.5;
    }
    texture_renderer_XY_clip(&tilesTex, gameT1.mousePoint.x - correct_X, gameT1.mousePoint.y + correct_Y, &tiles_rect[0]);
}
void drawIsoMap(isoEngineT* isoEngine) {
    int tile;
    point2DT point;

    // loop through the map
    for (int i = 0 ; i < isoEngine->mapHeight; i++) {
        for (int j = 0; j < isoEngine->mapWidth ; j++) {
            point.x = (j*TILESIZE) + isoEngine->scrollX;
            point.y = (i*TILESIZE) + isoEngine->scrollY;

            tile = worldTest[i][j];

            Converter2DToIso(&point);

            texture_renderer_XY_clip(&tilesTex, point.x, point.y, &tiles_rect[tile]);
        }
    }
}
void get_mouseTile_click(isoEngineT* isoEngine) {
    point2DT point;
    point2DT tileshift;
    point2DT mouse2IsoPoint;

    int correct_X = (gameT1.mapScroll2Dpos.x%(TILESIZE/2)) * 2;
    int correct_Y = (gameT1.mapScroll2Dpos.y%(TILESIZE/2)) ;

    //copy mouse point;
    mouse2IsoPoint = gameT1.mousePoint;
    ConverterIsoTo2D(&mouse2IsoPoint);
    //get tile coord
    GetTileCoordinates(&mouse2IsoPoint, &point);

    tileshift.x = correct_X;
    tileshift.y = correct_Y;
    ConverterIsoTo2D(&tileshift);

    point.x -= ((float)isoEngine->scrollX + (float)tileshift.x) / (float)TILESIZE;
    point.y -= ((float)isoEngine->scrollY - (float)tileshift.y) / (float)TILESIZE;

    if(point.x >= 0 && point.y >= 0 && point.x < MAP_WIDTH && point.y < MAP_HEIGHT) {
        gameT1.lastTiledClick = worldTest[point.y][point.x];
    }

}
void draw() {
    SDL_SetRenderDrawColor(get_renderer(), 0, 0, 0, 0);
    SDL_RenderClear(get_renderer());

    drawIsoMap(&gameT1.isoEngine);
    drawIsoMouse();

    if(gameT1.lastTiledClick != -1) {
        texture_renderer_XY_clip(&tilesTex, 0, 0, &tiles_rect[gameT1.lastTiledClick]);
    }
    SDL_RenderPresent(get_renderer());

    SDL_Delay(10);
}

void update() {
    SDL_GetMouseState(&gameT1.mouseRect.x, &gameT1.mouseRect.y);
}
void update_input() {
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&gameT1.event) != 0) {
        switch(gameT1.event.type) {
            case SDL_QUIT:
                gameT1.loop_done = 1;
                break;
            case SDL_KEYDOWN:
                switch (gameT1.event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        gameT1.loop_done = 1;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(gameT1.event.button.button == SDL_BUTTON_LEFT) {
                    get_mouseTile_click(&gameT1.isoEngine);
                }
                break;
            default:
                break;
        }
    }

    if(keystate[SDL_SCANCODE_W]) {
        gameT1.isoEngine.scrollX += gameT1.mapScrollSpeed;
        gameT1.isoEngine.scrollY += gameT1.mapScrollSpeed;
        gameT1.mapScroll2Dpos.y += gameT1.mapScrollSpeed;

        if(gameT1.mapScroll2Dpos.y > 0) {
            gameT1.mapScroll2Dpos.y = 0;
            gameT1.isoEngine.scrollX -= gameT1.mapScrollSpeed;
            gameT1.isoEngine.scrollY -= gameT1.mapScrollSpeed;
        }
    }
    if(keystate[SDL_SCANCODE_A]) {
        gameT1.isoEngine.scrollX += gameT1.mapScrollSpeed;
        gameT1.isoEngine.scrollY -= gameT1.mapScrollSpeed;
        gameT1.mapScroll2Dpos.x -= gameT1.mapScrollSpeed;
    }
    if(keystate[SDL_SCANCODE_S]) {
        gameT1.isoEngine.scrollX -= gameT1.mapScrollSpeed;
        gameT1.isoEngine.scrollY -= gameT1.mapScrollSpeed;
        gameT1.mapScroll2Dpos.y -= gameT1.mapScrollSpeed;
    }
    if(keystate[SDL_SCANCODE_D]) {
        gameT1.isoEngine.scrollX -= gameT1.mapScrollSpeed;
        gameT1.isoEngine.scrollY += gameT1.mapScrollSpeed;
        gameT1.mapScroll2Dpos.x += gameT1.mapScrollSpeed;
    }
}

int main(int argc, char* argv[]) {
    srand(time(0));

    init_SDL("Naval_Battle");
    init();
/*
    while (!game.loop_done) {
        update();
        update_input();
        draw();
    }

    return 0;*/
    SDL_Event event;

    CWorld* game = new CWorld(get_game());
    bool run_game = true;

    while (run_game) {

        while (SDL_PollEvent(&event)) {

            game->draw(&gameT1.isoEngine);

            // KEYBOARD EVENTS
            switch (event.type) {
                case SDL_QUIT:
                    run_game = !run_game;
                    break;
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
                            run_game = !run_game;
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
                    game->draw(&gameT1.isoEngine);
                    break;
                default:
                    run_game = false;
            }
        }

    }

    close_down_SDL();

    return 0;
}