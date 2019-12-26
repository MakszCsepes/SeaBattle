#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "draw.h"

using namespace std;

void put_kletka (SDL_Renderer *renderer, int x , int y ) {
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

    cursor.x = game.cursor_x * FRAME_SIZE + map_offset_x + game.computer.offset - 1;
    cursor.y = game.cursor_y * FRAME_SIZE + map_offset_y - 1;
    cursor.w = FRAME_SIZE;
    cursor.h = FRAME_SIZE;

    SDL_SetRenderDrawColor(renderer, 250, 0, 0, 255);

    SDL_RenderDrawRect(renderer, &cursor);

}

void draw_current_ship (SDL_Renderer *renderer, const ship &ship1) {
    SDL_Rect ship;
    SDL_Rect ship_head;

    ship.x = map_offset_x + ship1.coord_x * FRAME_SIZE ;
    ship.y = map_offset_y + ship1.coord_y * FRAME_SIZE ;
    ship.h = FRAME_SIZE;
    ship.w = FRAME_SIZE;

    SDL_SetRenderDrawColor(renderer , 200 , 10 , 10 , 0);

    ship_head.x = map_offset_x + ship1.coord_x * FRAME_SIZE - 1 + 12;
    ship_head.y = map_offset_y + ship1.coord_y * FRAME_SIZE - 1 + 12;
    ship_head.w = 12;
    ship_head.h = 12;

    if (ship1.inverse) {
        for (ship.x ; ship.x < ship1.size*FRAME_SIZE + map_offset_x + ship1.coord_x * 36 ; ship.x += FRAME_SIZE + 1) {
            SDL_RenderFillRect(renderer , &ship);
        }
    } else {
        for (ship.y ; ship.y < ship1.size*FRAME_SIZE + map_offset_y + ship1.coord_y * 36 ; ship.y += FRAME_SIZE + 1) {
            SDL_RenderFillRect(renderer , &ship);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderFillRect(renderer , &ship_head);
}

void draw_hit (SDL_Renderer *renderer, int x , int y , int offset) {
    SDL_Rect hit_left ;

    hit_left.x = x * FRAME_SIZE + map_offset_x + offset + 2;
    hit_left.y = y * FRAME_SIZE + map_offset_y + 2;
    hit_left.h = 3;
    hit_left.w = 3;

    SDL_Rect hit_right;

    hit_right.x = x * FRAME_SIZE + map_offset_x + offset + FRAME_SIZE - 8;
    hit_right.y = y * FRAME_SIZE + map_offset_y + 2;
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

void draw_mishit (SDL_Renderer *renderer, int x , int y , int offset) {
    SDL_Rect low = {x * FRAME_SIZE + map_offset_x + offset + 2, y * FRAME_SIZE + map_offset_y + 18 , 3 , 3};
    SDL_Rect low_1 = {x * FRAME_SIZE + map_offset_x + offset + 2, y * FRAME_SIZE + map_offset_y + 10 , 3 , 3};;
    SDL_Rect mid = {x * FRAME_SIZE + map_offset_x + offset + 2, y * FRAME_SIZE + map_offset_y + 2 , 3 , 3};
    SDL_Rect up_1 = {x * FRAME_SIZE + map_offset_x + offset + 10, y * FRAME_SIZE + map_offset_y + 2 , 3 , 3};
    SDL_Rect up = {x * FRAME_SIZE + map_offset_x + offset + 18, y * FRAME_SIZE + map_offset_y + 2 , 3 , 3};;

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

void draw_hits (SDL_Renderer *renderer, const world &game) {

    for (int i = 0 ; i < margin_width ; i++) {
        for (int j = 0 ; j < margin_heigth ; j++) {
            if (game.map_A[i][j] == HIT) {

                draw_hit(renderer, j, i, game.player1.offset);

            } else if (game.map_A[i][j] == MISHIT) {

                draw_mishit(renderer , j , i , game.player1.offset);

            }

        }
    }

    for (int i = 0 ; i < margin_width ; i++) {
        for (int j = 0 ; j < margin_heigth ; j++) {
            if (game.map_B[i][j] == HIT) {

                draw_hit(renderer, j, i, game.computer.offset);

            } else if (game.map_B[i][j] == MISHIT) {

                draw_mishit(renderer , j , i , game.computer.offset);
            }

        }
    }

}
void draw_ship (SDL_Renderer *renderer , const ship &ship1 , int shift) {

    SDL_Rect ship_part;
    SDL_Rect ship_head;

    ship_part.x = map_offset_x + ship1.coord_x * FRAME_SIZE+ shift;
    ship_part.y = map_offset_y + ship1.coord_y * FRAME_SIZE;
    ship_part.h = FRAME_SIZE ;
    ship_part.w = FRAME_SIZE ;

    ship_head.x = map_offset_x + ship1.coord_x * FRAME_SIZE - 1 + shift + 12;
    ship_head.y = map_offset_y + ship1.coord_y * FRAME_SIZE - 1 + 12;
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
    int count = 0;

    p = player1.head;

    while (p) {
        draw_ship(renderer, *p , player1.offset);
        p = p->next;
        count++;
    }

}

void draw_map (SDL_Renderer *renderer, player player1) {
    for (int y = map_offset_y , i = 0 ; i < margin_width ; i++ , y += FRAME_SIZE) {
        for (int x = map_offset_x + player1.offset, j = 0 ; j < margin_heigth ; j++ , x += FRAME_SIZE) {

            put_kletka(renderer, x, y );
        }
    }
}

void draw_results (SDL_Renderer *renderer, world game) {
    int paddingW = 10;
    int paddingH = 10;

    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/opentype/noto/NotoSansCJK.ttc", 25);

    char text[] = "The results are";

    int w = 0;
    int h = 0;

    SDL_Rect rectangle;

    if ( TTF_SizeText(font, text, &w, &h) != 0) {
        return;
    }

    rectangle.w = w + 2*paddingW;
    rectangle.h = h + 2*paddingH;
    rectangle.x = 380;
    rectangle.y = 100;

    SDL_Color text_color = {250, 250, 250};

    SDL_Surface * surface = TTF_RenderText_Solid(font,text, text_color);

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,surface);

    SDL_RenderDrawRect(renderer , &rectangle);

    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    SDL_Rect dstrect = {380 + paddingW , 100 + paddingH , w, h};

    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    //-------------------------------------------------

    char point = game.player1.points;

    w = 0;
    h = 0;

    if ( TTF_SizeText(font, &point, &w, &h) != 0) {
        return;
    }


    rectangle.w = w + 2*paddingW;
    rectangle.h = h + 2*paddingH;
    rectangle.x = 380;
    rectangle.y = 170;

    SDL_Surface * surface_p = TTF_RenderText_Solid(font,&point, text_color);

    SDL_Texture * texture_p = SDL_CreateTextureFromSurface(renderer,surface_p);

    SDL_RenderDrawRect(renderer , &rectangle);

    SDL_QueryTexture(texture_p, NULL, NULL, &w, &h);

    dstrect = {380 + paddingW , 150 + paddingH , w, h};

    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
}
void draw_world (SDL_Renderer *renderer, const world &game) {

    int R_back = 63;
    int G_back = 141;
    int B_back = 219;

    SDL_SetRenderDrawColor(renderer, R_back, G_back, B_back, 255);

    SDL_RenderClear(renderer);

    if (game.state == INIT_PLAYERS_SHIP) {
        draw_map(renderer , game.player1);

        draw_results(renderer , game);
        if (game.player1.init == false) {
            draw_ships(renderer, game.player1);

            draw_current_ship(renderer, *game.current_ship);

        } else {
            draw_ships(renderer,game.computer);

            draw_map(renderer , game.computer);
        }
    } else if (game.state == PLAY_GAME) {
        destroyed(game.player1.head , game.computer.head , game.map_A , game.map_B);

        draw_map(renderer , game.player1);
        draw_ships(renderer , game.player1);
        // PLAYER

        draw_map(renderer , game.computer);

        // COMPUTER
        draw_cursor(renderer , game);

        draw_hits(renderer , game);
    } else if (game.state == ENDGAME) {
        destroyed(game.player1.head , game.computer.head , game.map_A , game.map_B);

        draw_map(renderer , game.player1);
        draw_ships(renderer , game.player1);
        // PLAYER

        draw_map(renderer , game.computer);
        draw_ships(renderer, game.computer);
        // COMPUTER


        draw_results(renderer , game);
    }

    SDL_RenderPresent(renderer);
}
