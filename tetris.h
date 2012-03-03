/***************************************************************************
 *   Copyright (C) 2005 by Leandro Vendramin                               *
 *   lvendramin@gmx.net                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PUmaskE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __tetris
#define __tetris

#include "SDL.h"
#include "SDL_ttf.h"

#include "highscore.h"
#include "font.h"
#include "stuff.h"

/***************************************************************************
 * Definiciones								   *
 ***************************************************************************/
#define NOT_RANKED	-1

/***************************************************************************
 *   Estructuras                         	 			   *
 ***************************************************************************/
typedef struct {
	int x, y;
} POINT;

typedef struct {
	int x, y;
	int number;
	char rotation;
	POINT mask[4];
	char shape[4][4];
} PIECE;

typedef struct {
	int left;		/* Mover ficha a la izquierda */
	int right;		/* Mover ficha a la derecha */
	int rot_left;		/* Rotar ficha hacia la izquierda */
	int rot_right;		/* Rotar ficha hacia la derecha */
	int down;		/* Mover ficha hacia abajo */
	int drop;		/* Dejar caer la ficha */
} CONTROLS;

typedef struct {
	CONTROLS controls;	/* Teclas */
	PIECE *piece;		/* Ficha activa */
	PIECE *next;		/* Ficha siguiente */

	int x,y;		/* Posición del tablero */
//	int w,h;		/* Dimensiones del tablero */
	char *board;		/* Tablero (considerando la ficha activa) */
	char *old_board;	/* Tablero (SIN considerar la ficha activa) */

	char message[20];	/* Para mensajes en pantalla */
	SCORE score;		/* Puntaje, nivel y lineas */
	int rank;		/* Posición en el ranking */
	int total[8];		/* Cantidad de fichas (para estadísticas) */
				/* NOTA: el último es el total */
    	int moving;
        int moveto;
	int paused;
	int running;
	int gameover;

	int draw_board;					
	int draw_score;
	int draw_next;
	int draw_message;
	int draw_rank;

	int dropdelay;
	int lastdrop;
} PLAYER;

/***************************************************************************
 *   Constantes generales               	 			   *
 ***************************************************************************/
static const SDL_Rect	board_rectangle = {66, 65, 200, 400};
static const SDL_Rect  	block_rectangle = {0, 0, 20, 20}; 
static const SDL_Rect	points_rectangle = {331, 220, 129, 59};
static const SDL_Rect	level_rectangle = {401, 340, 69, 39};
static const SDL_Rect	lines_rectangle = {401, 300, 69, 39};
static const SDL_Rect	next_rectangle = {350, 80, 100, 100};
static const SDL_Rect	message_rectangle = {32, 530, 278, 49};
static const SDL_Rect 	rank_rectangle = {331,530,139,49};
static const int	block_width = 20;
static const int 	block_height = 20;
static const int 	board_width = 10;
static const int 	board_height = 20;
static const int 	font_color = 248; 
static const int 	font_shadow = 189;

/* 	
 *  	Forma de cada ficha (y sus rotaciones)
 *  	 
 *	Cada fila representa una ficha en alguna posición. Por ejemplo, la fila 1 corresponde 
 *	a la "T" es un posición inicial. Las 3 filas siguientes corresponden a sus rotaciones
 *	Las fichas son:
 *		1. La "T"
 *		2. El "cuadrado"
 * 		3. El "largo"
 *		4. La "escalera descendente" :-)
 *		5. La "escalera ascendente" :-)
 *		6. El "ángulo superior izquiedo" 
 *		7. El "ángulo superior derecho"
 */		
static const char shape[7][4][4][4] = {

       	1,1,1,0,  0,1,0,0,  0,0,0,0,  0,0,0,0,
        0,1,0,0,  0,1,1,0,  0,1,0,0,  0,0,0,0,
        0,1,0,0,  1,1,1,0,  0,0,0,0,  0,0,0,0,
        0,1,0,0,  1,1,0,0,  0,1,0,0,  0,0,0,0,

	1,1,0,0,  1,1,0,0,  0,0,0,0,  0,0,0,0,
	1,1,0,0,  1,1,0,0,  0,0,0,0,  0,0,0,0,
	1,1,0,0,  1,1,0,0,  0,0,0,0,  0,0,0,0,
	1,1,0,0,  1,1,0,0,  0,0,0,0,  0,0,0,0,

	0,1,0,0,  0,1,0,0,  0,1,0,0,  0,1,0,0,
	0,0,0,0,  0,0,0,0,  1,1,1,1,  0,0,0,0,
	0,1,0,0,  0,1,0,0,  0,1,0,0,  0,1,0,0,
	0,0,0,0,  0,0,0,0,  1,1,1,1,  0,0,0,0,

	0,1,0,0,  1,1,0,0,  1,0,0,0,  0,0,0,0,
	1,1,0,0,  0,1,1,0,  0,0,0,0,  0,0,0,0,
	0,1,0,0,  1,1,0,0,  1,0,0,0,  0,0,0,0,
	1,1,0,0,  0,1,1,0,  0,0,0,0,  0,0,0,0,
		
	1,0,0,0,  1,1,0,0,  0,1,0,0,  0,0,0,0,
	0,1,1,0,  1,1,0,0,  0,0,0,0,  0,0,0,0,
	1,0,0,0,  1,1,0,0,  0,1,0,0,  0,0,0,0,
	0,1,1,0,  1,1,0,0,  0,0,0,0,  0,0,0,0,

	1,1,1,0,  1,0,0,0,  0,0,0,0,  0,0,0,0,
	0,1,0,0,  0,1,0,0,  0,1,1,0,  0,0,0,0,
	0,0,1,0,  1,1,1,0,  0,0,0,0,  0,0,0,0,
	1,1,0,0,  0,1,0,0,  0,1,0,0,  0,0,0,0,

	1,1,1,0,  0,0,1,0,  0,0,0,0,  0,0,0,0,
	0,1,1,0,  0,1,0,0,  0,1,0,0,  0,0,0,0,
	0,0,0,0,  1,0,0,0,  1,1,1,0,  0,0,0,0,
	0,1,0,0,  0,1,0,0,  1,1,0,0,  0,0,0,0
				
};



/***************************************************************************
 *   Variables globales      						   *
 ***************************************************************************/
PLAYER*		player;			/* Jugador */
HIGHSCORE*	highscore;		/* Highscore */
SDL_Surface*	blocks;			/* Ladrillos 20x20 */
FONT*		font;			/* Font para mensajes */


/***************************************************************************
 *   Funciones                 						   *
 ***************************************************************************/
PLAYER*		PLAYER_create();
void		PLAYER_destroy(PLAYER *player);
void		PLAYER_init(PLAYER *player);
void 		PLAYER_draw(PLAYER *player);
void 		PLAYER_check(PLAYER *player);
int 		PLAYER_movepiece(PLAYER *player, int direction);
void		PLAYER_rotatepiece(PLAYER *player, int direction);
void		PLAYER_pause(PLAYER *player, int pause);
void 		PLAYER_showscore(PLAYER *player);
int		PLAYER_putpiece(PLAYER *player, PIECE *piece);
void		PLAYER_setmessage(PLAYER *player, char *string);

PIECE*		PIECE_create();
void		PIECE_destroy(PIECE *piece);
void 		PIECE_set(PIECE *piece, int number);
PIECE*		PIECE_rotate(PIECE *piece, int rotation);
PIECE* 		PIECE_move(PIECE *piece, int direction);
void		PIECE_drop(PIECE *piece);
void		PIECE_draw(PIECE *piece, int x, int y);
void		PIECE_getmask();
int		PIECE_getmaxx(PIECE *piece);
int 		PIECE_getminx(PIECE *piece);
int 		PIECE_getmaxy(PIECE *piece);
int		PIECE_getminy(PIECE *piece);

void 		TETRIS_init();
void		TETRIS_quit();
void 		TETRIS_run();
void 		TETRIS_eventhandler();

void 		BLOCK_draw(int number, int x, int y);
int		BLOCK_random();

#endif
