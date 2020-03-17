#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "draw.h"

using namespace std;

void put_cell (SDL_Renderer *renderer, int& x , int& y ) {
    SDL_Rect r;

    r.x = x;
    r.y = y;
    r.w = 34;
    r.h = 34;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &r);
}
void draw_cursor (SDL_Renderer *renderer, world game) {
    SDL_Rect cursor;

    cursor.x = game.map_position_x * FRAME_SIZE + MAP_OFFSET_X + game.ai.offset - 1;
    cursor.y = game.map_position_y * FRAME_SIZE + MAP_OFFSET_Y - 1;
    cursor.w = FRAME_SIZE;
    cursor.h = FRAME_SIZE;

    SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);

    SDL_RenderDrawRect(renderer, &cursor);

}

// HITS
void draw_hit (SDL_Renderer *renderer, int& x , int& y , int offset) {
    SDL_Rect hit_left ;

    hit_left.x = x * FRAME_SIZE + MAP_OFFSET_X + offset + 2;
    hit_left.y = y * FRAME_SIZE + MAP_OFFSET_Y + 2;
    hit_left.h = 3;
    hit_left.w = 3;

    SDL_Rect hit_right;

    hit_right.x = x * FRAME_SIZE + MAP_OFFSET_X + offset + FRAME_SIZE - 8;
    hit_right.y = y * FRAME_SIZE + MAP_OFFSET_Y + 2;
    hit_right.h = 3;
    hit_right.w = 3;

    SDL_SetRenderDrawColor(renderer , 255 , 0 , 0 , 255);

    for (int i = 0 ; i < 10 ; i++ , hit_left.x += hit_left.h , hit_left.y += hit_left.w ) {

        SDL_RenderFillRect(renderer , &hit_left);
        SDL_RenderFillRect(renderer , &hit_right);

        hit_right.x -= hit_right.h;
        hit_right.y += hit_right.w;
    }
}
void draw_mishit (SDL_Renderer *renderer, int& x , int& y , int offset) {
    SDL_Rect low = {x * FRAME_SIZE + MAP_OFFSET_X + offset + 2, y * FRAME_SIZE + MAP_OFFSET_Y + 18 , 3 , 3};
    SDL_Rect low_1 = {x * FRAME_SIZE + MAP_OFFSET_X + offset + 2, y * FRAME_SIZE + MAP_OFFSET_Y + 10 , 3 , 3};;
    SDL_Rect mid = {x * FRAME_SIZE + MAP_OFFSET_X + offset + 2, y * FRAME_SIZE + MAP_OFFSET_Y + 2 , 3 , 3};
    SDL_Rect up_1 = {x * FRAME_SIZE + MAP_OFFSET_X + offset + 10, y * FRAME_SIZE + MAP_OFFSET_Y + 2 , 3 , 3};
    SDL_Rect up = {x * FRAME_SIZE + MAP_OFFSET_X + offset + 18, y * FRAME_SIZE + MAP_OFFSET_Y + 2 , 3 , 3};;

    SDL_SetRenderDrawColor(renderer ,  93 , 102 , 111 , 255);;

    for (int i = 0 ; i < 10 ; i++ , mid.x += mid.h , mid.y += mid.w ) {
        SDL_RenderFillRect(renderer , &mid);
    }

    for (int i = 0 ; i < 8 ; i++ , up_1.x += up_1.h , up_1.y += up_1.w) {
        SDL_RenderFillRect(renderer , &up_1);
    }

    for (int i = 0 ; i < 5 ; i++ , up.x += up.h , up.y += up.w) {
        SDL_RenderFillRect(renderer , &up);
    }

    for (int i = 0 ; i < 8 ; i++ , low_1.x += low_1.h , low_1.y += low_1.w) {
        SDL_RenderFillRect(renderer , &low_1);
    }

    for (int i = 0 ; i < 5 ; i++ , low.x += low.h , low.y += low.w) {
        SDL_RenderFillRect(renderer , &low);
    }
}
void draw_hits (SDL_Renderer *renderer, const world& game) {

    for (int i = 0 ; i < MARGIN_WIDTH ; i++) {
        for (int j = 0 ; j < MARGIN_HEIGHT ; j++) {
            if (game.user_map[i][j] == HIT) {

                draw_hit(renderer, j, i, game.user.offset);

            } else if (game.user_map[i][j] == MISHIT) {

                draw_mishit(renderer , j , i , game.user.offset);

            }

        }
    }

    for (int i = 0 ; i < MARGIN_WIDTH ; i++) {
        for (int j = 0 ; j < MARGIN_HEIGHT ; j++) {
            if (game.ai_map[i][j] == HIT) {

                draw_hit(renderer, j, i, game.ai.offset);

            } else if (game.ai_map[i][j] == MISHIT) {
                draw_mishit(renderer , j , i , game.ai.offset);
            }

        }
    }
}

// SHIPS
void draw_current_ship (SDL_Renderer *renderer, const ship& ship1) {
    SDL_Rect ship;
    SDL_Rect ship_head;

    ship.x = MAP_OFFSET_X + ship1.coord_x * FRAME_SIZE ;
    ship.y = MAP_OFFSET_Y + ship1.coord_y * FRAME_SIZE ;
    ship.h = FRAME_SIZE;
    ship.w = FRAME_SIZE;

    SDL_SetRenderDrawColor(renderer , 200 , 10 , 10 , 0);

    ship_head.x = MAP_OFFSET_X + ship1.coord_x * FRAME_SIZE - 1 + 12;
    ship_head.y = MAP_OFFSET_Y + ship1.coord_y * FRAME_SIZE - 1 + 12;
    ship_head.w = 12;
    ship_head.h = 12;

    if (ship1.inverse) {
        for (ship.x ; ship.x < ship1.size*FRAME_SIZE + MAP_OFFSET_X + ship1.coord_x * 36 ; ship.x += FRAME_SIZE + 1) {
            SDL_RenderFillRect(renderer , &ship);
        }
    } else {
        for (ship.y ; ship.y < ship1.size*FRAME_SIZE + MAP_OFFSET_Y + ship1.coord_y * 36 ; ship.y += FRAME_SIZE + 1) {
            SDL_RenderFillRect(renderer , &ship);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderFillRect(renderer , &ship_head);
}
void draw_ship (SDL_Renderer *renderer , const ship& ship1 , int& shift) {

    SDL_Rect ship_part;
    SDL_Rect ship_head;

    ship_part.x = MAP_OFFSET_X + ship1.coord_x * FRAME_SIZE + shift;
    ship_part.y = MAP_OFFSET_Y + ship1.coord_y * FRAME_SIZE;
    ship_part.h = FRAME_SIZE ;
    ship_part.w = FRAME_SIZE ;

    ship_head.x = MAP_OFFSET_X + ship1.coord_x * FRAME_SIZE - 1 + shift + 12;
    ship_head.y = MAP_OFFSET_Y + ship1.coord_y * FRAME_SIZE - 1 + 12;
    ship_head.w = 12;
    ship_head.h = 12;

    for (int i = 0 ; i < ship1.size; i++) {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderFillRect(renderer , &ship_part);

        if (i == 0) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            SDL_RenderFillRect(renderer , &ship_head);
        }

        if (ship1.inverse) {
            ship_part.x += FRAME_SIZE;
        } else {
            ship_part.y += FRAME_SIZE;
        }
    }

}
void draw_ships (SDL_Renderer *renderer, player player1) {

    ship *p = new ship;

    p = player1.ships_list_head;

    while (p) {
        draw_ship(renderer, *p , player1.offset);
        p = p->next;
    }
}

// MAP
void draw_map (SDL_Renderer *renderer, player player1) {
    for (int y = MAP_OFFSET_Y , i = 0 ; i < MARGIN_WIDTH ; i++ , y += FRAME_SIZE) {
        for (int x = MAP_OFFSET_X + player1.offset, j = 0 ; j < MARGIN_HEIGHT ; j++ , x += FRAME_SIZE) {

            put_cell(renderer, x, y);
        }
    }
}

// TEXT
void draw_text(SDL_Renderer *renderer, char* text) {
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/opentype/noto/NotoSansCJK.ttc", 25);
    SDL_Color text_color = {250, 250, 250};
    SDL_Surface * surface = TTF_RenderText_Solid(font, text, text_color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,surface);

    int w = 10;
    int h = 10;

    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    SDL_Rect dstrect = {10 + 50 , 10 + 50, w, h};

    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
//
    SDL_RenderPresent(renderer);
}
void draw_script_version(SDL_Renderer *renderer, lua_State* L) {
    LuaRef version = getGlobal(L, "script_version");

    string vers =  version.cast<std::string>();
    char v[vers.length() + 1];
    strcpy(v, vers.c_str());

    draw_text(renderer, v);
}

// DRAW EVERYTHING
void draw_world (SDL_Renderer *renderer, const world& game) {
    int R_back = 63;
    int G_back = 141;
    int B_back = 219;

    SDL_SetRenderDrawColor(renderer, R_back, G_back, B_back, 255);
    SDL_RenderClear(renderer);

    draw_script_version(renderer, game.lua_state);
    if (game.game_state == PUT_SHIPS) {
        draw_map(renderer, game.user);

        if (game.user.inited == false) {
            draw_ships(renderer, game.user);

            draw_current_ship(renderer, *game.current_ship);

        } else {
            draw_ships(renderer, game.ai);

            draw_map(renderer, game.ai);
        }
    } else if (game.game_state == PLAY_GAME) {
        destroyed(game.user.ships_list_head, game.ai.ships_list_head, game.user_map, game.ai_map);

        draw_map(renderer, game.user);
        draw_ships(renderer, game.user);
        // PLAYER

        draw_map(renderer, game.ai);

        // COMPUTER
        draw_cursor(renderer , game);

        draw_hits(renderer, game);
    } else if (game.game_state == ENDGAME) {
        destroyed(game.user.ships_list_head, game.ai.ships_list_head, game.user_map, game.ai_map);

        draw_map(renderer , game.user);
        draw_ships(renderer , game.user);
        // PLAYER

        draw_map(renderer , game.ai);
        draw_ships(renderer, game.ai);
        // COMPUTER
    }

    SDL_RenderPresent(renderer);
}