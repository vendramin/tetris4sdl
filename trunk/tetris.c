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

#include "tetris.h"

/***************************************************************************
 *   Variables globales 						   *
 ***************************************************************************/
extern PLAYER*		player;
extern SDL_Surface* 	blocks;

/*
 *   	void TETRIS_init()
 *   	
 *   	Inicializa las variables globales del TETRIS.   	
 */
void TETRIS_init()
{

	GFX_init();
	showbmp("background.bmp");
	
	SDL_WM_SetCaption("Tetris", NULL);
	
	srand(time(NULL));

	TTF_Init();
	font = FONT_create("font.ttf", 30, TTF_STYLE_NORMAL);
	FONT_setcolor(font, font_color, font_shadow);
			
	blocks = SDL_LoadBMP("blocks.bmp");

	highscore = HIGHSCORE_create("highscores");
	player = PLAYER_create();
	
}

/*
 *   	void TETRIS_quit()
 *   
 *   	Libera toda la memoria ocupada por el TETRIS. 
 *	El HIGHSCORE se guarda en el archivo "highscores".
 */
void TETRIS_quit()
{
	HIGHSCORE_save(highscore, "highscores");
	HIGHSCORE_destroy(highscore);
	PLAYER_destroy(player);
	FONT_destroy(font);
	GFX_quit();
}


/*
 * 	void TETRIS_run()
 *
 * 	Loop principal. Maneja el teclado tanto dentro como
 * 	fuera del partido de TETRIS. 
 */
void TETRIS_run()
{
	
	player->lastdrop = SDL_GetTicks();
	PLAYER_putpiece(player, player->piece);
	
	while (player->running) {
		TETRIS_eventhandler();	
	
		if (player->gameover == FALSE) {
			if (player->paused == FALSE) {
				int ticks = SDL_GetTicks();
				int linestodrop = (ticks - player->lastdrop) / player->dropdelay;
				if (linestodrop > 0) {
					player->lastdrop += linestodrop * player->dropdelay;
					PLAYER_movepiece(player, DROP);
				}
			
				if (player->moving) {
					PLAYER_movepiece(player, player->moveto);
					player->moving = FALSE;
				}
				PLAYER_draw(player);
				SDL_Delay(SDL_TIMESLICE);
			}
		}
	}
		
}

/*
 * 	void TETRIS_eventhandler()
 *
 * 	Manejador de eventos. Utilizado en TETRIS_run().
 * 	Si la ventana deja de ser activa juego se pone 
 * 	automáticamente en pausa. 
 */
void TETRIS_eventhandler()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_ACTIVEEVENT:
				if (event.active.gain == FALSE) {
					if ((event.active.state & SDL_APPINPUTFOCUS) || (event.active.state & SDL_APPACTIVE)) {
						PLAYER_pause(player, TRUE);
						PLAYER_draw(player);
					}
				}
				break;
			case SDL_QUIT: 
				player->running = FALSE;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_q:
					case SDLK_ESCAPE:
						player->running = FALSE;
						break;
					case SDLK_h:
 						//memcpy(tmp, highscore, sizeof(highscore));
						//HIGHSCORE_put(tmp, &player->score, player->rank); 
						HIGHSCORE_show(highscore);
						break;
					case SDLK_n:
						PLAYER_init(player);
						player->lastdrop = SDL_GetTicks();
						PLAYER_putpiece(player, player->piece);
						break;
					case SDLK_RETURN:
						if (event.key.keysym.mod & KMOD_ALT) 
							SDL_WM_ToggleFullScreen(SDL_GetVideoSurface());
						break;
					case SDLK_SPACE:
						player->moving = TRUE;
						player->moveto = BOTTOM;
						break;
					case SDLK_p:
						PLAYER_pause(player, (player->paused+1)%2);
						PLAYER_draw(player);
						break;
					case SDLK_RIGHT:
						player->moving = TRUE;
						player->moveto = RIGHT;
						break;
					case SDLK_UP:
						if (player->paused == FALSE)
							PLAYER_rotatepiece(player, LEFT);
						break;
					case SDLK_LEFT:
						player->moving = TRUE;
						player->moveto = LEFT;
						break;
					case SDLK_DOWN:
						player->moving = TRUE;
						player->moveto = DOWN;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	
	}
}

/*
 *   	PLAYER *player_create()
 *   	
 *   	Crea un nuevo jugador de TETRIS. Esta función devuelve un puntero 
 *   	a la estructura del PLAYER (lista para usar) o NULL en caso de 
 *   	haberse encontrado un error. 
 *   	
 *   	NO es necesario reservar memoria con malloc o similareas.
 */
PLAYER *PLAYER_create() 
{
	
	PLAYER *player = (PLAYER *)malloc(sizeof(PLAYER));
	if (player == NULL) return NULL;

	player->board = (char *)calloc(board_height, board_width);
	player->old_board = (char *)calloc(board_height, board_width);
	
	PLAYER_init(player);
	return player;
}

/*
 * 	void PLAYER_destroy(PLAYER *player)
 *
 * 	Libera la memoria ocupada por la estructura PLAYER
 */
void PLAYER_destroy(PLAYER *player)
{
	free(player->board);
	free(player->old_board);
	free(player);
}

/*
 * 	void PLAYER_init(PLAYER *player)
 *
 * 	Inicializa la estructura para el JUGADOR. Esta función 
 * 	es utilizada para empezar un nuevo partido de TETRIS.
 */
void PLAYER_init(PLAYER *player)
{

	memset(player->board, 0, board_height*board_width);
	memset(player->old_board, 0, board_height*board_width);
	
	player->score.points = 0;
	player->score.level = 1;
	player->score.lines = 0;
	strcpy(player->score.name, "Leandro");
	strcpy(player->score.date, "01/01/1981");

	strcpy(player->message, "Ok, let's play...");
	
	/* Por default, el JUGADOR no está rankeado
	 * NOT_RANKED es -1. Al darle el valor -2 es seguro que
	 * siempre va a escribirse la posición del JUGADOR */
	player->rank = highscore->length+1;		
	
	player->draw_next = TRUE;
	player->draw_board = TRUE;
	player->draw_score = TRUE;
	player->draw_message = TRUE;

	player->dropdelay = 1000;
	player->lastdrop = SDL_GetTicks();
	
	player->paused = FALSE;
	player->running = TRUE;
	player->gameover = FALSE;
	player->x = board_rectangle.x;
	player->y = board_rectangle.y;
	
	int i;
	for (i=0; i<8; i++) player->total[i] = 0;

	/* Creo y elijo una FICHA para empezar el juego 
	 * y la FICHA siguiente */
	player->piece = PIECE_create();
	player->next = PIECE_create();
	PIECE_set(player->piece, BLOCK_random());
	PIECE_set(player->next, BLOCK_random());
}

/*
 * 	void PLAYER_draw(PLAYER *player)
 *
 * 	Si fuera necesario, redibuja el tablero del jugador 
 * 	PLAYER, reescribe el SCORE, escribe mensajes y muestra
 * 	la FICHA siguiente.
 */
void PLAYER_draw(PLAYER *player)
{

	/* Si fuera necesario, redibuja el tablero */ 
	if (player->draw_board == TRUE) {
		int x,y,number;
		int i,j;
		for (j=0; j<board_height; j++) {
			for (i=0; i<board_width; i++) {
				number = player->board[j*board_width+i];
				x = block_width*i+player->x;
				y = block_height*j+player->y;
				if (number != 0)
					BLOCK_draw(number, x, y);
				else {
					SDL_Rect tmp;
					tmp.x = x;
					tmp.y = y;
					tmp.w = block_width;
					tmp.h = block_height;
					SDL_FillRect(screen, &tmp, 21);
				}
			}
		}
		SDL_UpdateRect(screen, player->x, player->y, block_width*board_width, block_height*board_height);
		player->draw_board = FALSE;
	}

	/* Chequear si hubo un ascenso en la posición del highscore
	 * y en ese caso, escribir el lugar en pantalla */
	int rank = HIGHSCORE_where(highscore, &player->score);
	if (rank < player->rank) {
		SDL_Rect dest;
		char string[10];
		memcpy(&dest, &rank_rectangle, sizeof(SDL_Rect));
		SDL_FillRect(screen, &dest, 0);
		if (rank == highscore->length) 
 			sprintf(string, "Not ranked");
		else 
			sprintf(string, "Rank: %d", rank+1);
		FONT_center(font, string, &dest, FALSE);
		SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);
	}
	player->rank = rank;
		
	/* Si fuera necesario, escribe puntos, 
	 * nivel y cantidad de lineas */
	if (player->draw_score == TRUE) {
		SDL_Rect dest;
		char string[10];

		/* Puntos */
		memcpy(&dest, &points_rectangle, sizeof(SDL_Rect));
		SDL_FillRect(screen, &dest, 0);
		sprintf(string, "%6d", player->score.points);
		FONT_right(font, string, &dest, FALSE);
		SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);

		/* Nivel y líneas 
		 * FIXME: ¿Utilizar una función?*/
		memcpy(&dest, &lines_rectangle, sizeof(SDL_Rect));
		SDL_FillRect(screen, &dest, 0);
		sprintf(string, "%3d", player->score.lines);
		FONT_center(font, string, &dest, FALSE);
		SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);

		memcpy(&dest, &level_rectangle, sizeof(SDL_Rect));
		SDL_FillRect(screen, &dest, 0);
		sprintf(string, "%3d", player->score.level);
		FONT_center(font, string, &dest, FALSE);
		SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);

		player->draw_score = FALSE;

	}

	/* Si fuera necesario, muestra la ficha siguiente */
	if (player->draw_next == TRUE) {
		SDL_Rect dest;
		memcpy(&dest, &next_rectangle, sizeof(SDL_Rect));
		SDL_FillRect(screen, &dest, 0);
		
		/* Para centrar la FICHA siguiente */
		int w = block_width*(1+PIECE_getmaxx(player->next)-PIECE_getminx(player->next));
		int h = block_height*(1+PIECE_getmaxy(player->next)-PIECE_getminy(player->next));

		int x,y,i,j;
		for (j=0; j<4; j++) {
			for (i=0; i<4; i++) {
				x = dest.x + (dest.w-w)/2 + block_width*i;
				y = dest.y + (dest.h-h)/2 + block_height*j;
				if (shape[player->next->number-1][0][j][i] != 0)
					BLOCK_draw(player->next->number, x, y);
			}
		}	
		SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);
		player->draw_next = FALSE;
	}

	/* Si fuera necesario, muestra un mensaje */
	if (player->draw_message == TRUE) {
		SDL_Rect dest;
		memcpy(&dest, &message_rectangle, sizeof(SDL_Rect));
		SDL_FillRect(screen, &dest, 0);
		FONT_center(font, player->message, &dest, FALSE);

		SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);
		player->draw_message = FALSE;
	}
	
}

/*
 * 	int PLAYER_putpiece(PLAYER *player, PIECE *piece)
 *
 * 	Ubica la FICHA en el tablero. Devuelve TRUE si pudo ubicarse
 * 	correctamente y FALSE en caso contrario. 
 */
int PLAYER_putpiece(PLAYER *player, PIECE *piece)
{

	char *tmp = (char *)calloc(board_height, board_width);
	memcpy(tmp, player->old_board, board_width*board_height);

	/* Ubico la ficha en el tablero temporal y verifico */
	int i,j=0;
	for (i=0; i<4; i++) {
		j+=tmp[piece->mask[i].y*board_width+piece->mask[i].x];
		tmp[board_width*piece->mask[i].y+piece->mask[i].x] = piece->number;	
	}
	
	/* Si se pudo ubicar la ficha en el tablero temporal, 
	 * actualizo buffer, dibujo y devuelvo TRUE. En caso 
	 * contrario, recupero el buffer guardado y devuelvo FALSE */
	if (j==0) {
		memcpy(player->board, tmp, board_width*board_height);
		player->draw_board = TRUE;
		return TRUE;
	}
		
	return FALSE;	
}

/*	int PLAYER_movepiece(PLAYER *player, int direction)
 *
 *	Mueve la FICHA activa en la dirección indicada 
 *	como parámetro. Devuelve el estado del juego después de
 *	la movida.
 */
int PLAYER_movepiece(PLAYER *player, int direction)
{
	PIECE *tmp = PIECE_create();
	
	if (direction == BOTTOM) {
		while (((tmp = PIECE_move(player->piece, DOWN)) != NULL) && (PLAYER_putpiece(player, tmp) == TRUE)) {
			player->score.points++;
			memcpy(player->piece, tmp, sizeof(PIECE));
			player->draw_board = TRUE;
			player->draw_score = TRUE;
			PLAYER_draw(player);
		}
		return TRUE;
	}
	
	if (((tmp = PIECE_move(player->piece, direction)) != NULL) && (PLAYER_putpiece(player, tmp) == TRUE)) {
			memcpy(player->piece, tmp, sizeof(PIECE));
			if (direction == DOWN) { 
				player->score.points++;
				player->draw_score = TRUE;
			}
	}
	else {
		if ((direction == DOWN) || (direction == DROP)) {

			/* Nueva ficha */
			memcpy(player->old_board, player->board, block_width*block_height);
			PLAYER_check(player);
			player->draw_next = TRUE;
			player->draw_board = TRUE;
			memcpy(player->piece, player->next, sizeof(PIECE));
			PIECE_set(player->next, BLOCK_random());

			/* ¿Se perdió el partido? 
			 * En ese caso, además de lo obvio, hacer un efecto simpaticón 
			 * en el tablero y ocultar la que era la ficha siguiente */
			if (PLAYER_putpiece(player, player->piece) == FALSE) {
				
				/* ...para la ficha siguiente */
				int x,y,i,j,k=BLOCK_random();
				SDL_Rect dest;
				memcpy(&dest, &next_rectangle, sizeof(SDL_Rect));
				for (j=0; j<4; j++) {
					for (i=0; i<4; i++) {
						x = 10+dest.x + block_width*i;
						y = 10+dest.y + block_height*j;
						BLOCK_draw(k, x, y);
					}
				}	
				SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);
				player->draw_next = FALSE;
		
				/* ...para el tablero */
				for (k=0; k<board_height; k++) {
					memset(player->board+k*board_width, 1+(k%7), board_width);
					player->draw_board = TRUE;
					PLAYER_draw(player);
					SDL_Delay(50);
				}
				
				PLAYER_setmessage(player, "Game over");
				PLAYER_draw(player);

				/* Para el highscore 
				 * FIXME: falta corregir la función input */
				
				//char name[20];
				//name[0] = '\0';
				//FONT_inputxy(font, name, 50, 50, 20);
				
				/* Algunas consideraciones generales */
				HIGHSCORE_put(highscore, &player->score, player->rank);
				HIGHSCORE_save(highscore, "highscores");
				player->draw_board = FALSE;
				player->gameover = TRUE;
				return FALSE;
		
			}
			
		}
	}
	return TRUE;
}

/* 	void PLAYER_check(PLAYER *player)
 *
 *  	Chequea si el JUGADOR hizo lineas. Calcula el puntaje
 *  	correspondiente y se cambia de nivel si fuera requerido.
 */
void PLAYER_check(PLAYER *player)
{

	int i,j,k;
	int old_lines = player->score.lines;
	
	for (j=0; j<board_height; j++) {
		k = 1;
		for (i=0; i<board_width; i++) k *= player->old_board[j*board_width+i];
		if (k != 0) {
		
			/* Primero, se hace un efecto simpaticón */
			memset(player->board+j*board_width, 8, board_width);
			player->draw_board = TRUE;
			PLAYER_draw(player);
			SDL_Delay(50);
			
			/* Ahora sí se quitan las líneas */
			for (k=j; k>0; k--) 
				memcpy(player->old_board+k*board_width, player->old_board+board_width*(k-1), board_width);
			
			memset(player->old_board, 0, board_width);
			memcpy(player->board, player->old_board, board_width*board_height);
			player->score.lines++;
	
			player->draw_board = TRUE;
			player->lastdrop = SDL_GetTicks();
		
			/* Cada 10 líneas se avanza un nivel. 
			 * El mínimo drop_delay posible es 12. */ 
			if (player->score.lines > player->score.level * 10) {
				player->score.level++;
				PLAYER_setmessage(player, "Level up!");
				player->draw_message = TRUE;
				player->dropdelay = max((int)((float)(player->dropdelay * 3.0f / 4.0f)), 12);
			} 
		}
	}

	/* Se consideran los puntajes correspondientes... */
	char message[20];
	int points_to_add = (player->score.lines-old_lines) * player->score.level * 100;
	if (player->score.lines-old_lines != 0) {
		player->score.points += points_to_add;
		player->draw_score = TRUE;
		sprintf(message, "%d points", points_to_add);
		PLAYER_setmessage(player, message);
	}

}

void PLAYER_rotatepiece(PLAYER *player, int direction)
{
	PIECE *tmp = PIECE_create();
	if ((tmp = PIECE_rotate(player->piece, direction)) != NULL) {
		if (PLAYER_putpiece(player, tmp) == TRUE)
			memcpy(player->piece, tmp, sizeof(PIECE));
	}
}


/*
 * 	void PLAYER_setmessage(PLAYER *player, char *string)
 *
 * 	Setea el mensaje a mostrar en pantalla
 */
void PLAYER_setmessage(PLAYER *player, char *string)
{
	strcpy(player->message, string);
	player->draw_message = TRUE;
}

/*
 * 	void PLAYER_pause(PLAYER *player, int pause)
 * 	
 * 	Setea el modo pausa
 */
void PLAYER_pause(PLAYER *player, int pause)
{

	SDL_WM_SetCaption(pause ? "Tetris (paused)" : "Tetris", NULL);
	PLAYER_setmessage(player, pause ? "Paused" : "Ok, let's play...");
	player->paused = pause;	
}


/*
 *	PIECE *PIECE_create()
 *
 *	Crea una ficha de TETRIS.
 */
PIECE *PIECE_create()
{
	return (PIECE *)malloc(sizeof(PIECE));
}

/*
 * 	void PIECE_destroy(PIECE *piece)
 *
 * 	Libera la memoria ocupada por la ficha.
 */
void PIECE_destroy(PIECE *piece)
{
	free(piece);
}

/*	
 *	void PIECE_set(PIECE *piece, int number)
 *
 *	Inicializa la ficha. 
 */	
void PIECE_set(PIECE *piece, int number)
{
	piece->x = (board_width-4)/2;	
	piece->y = 0;
	piece->number = number;
	piece->rotation = 0;
	PIECE_getmask(piece);
}

/*
 * 	void PIECE_rotate(PIECE *piece, int direction)
 *
 * 	Rota la FICHA hacia la izquierda. Esta función no es para ubicar
 * 	la ficha en el tablero. Solamente calcula la máscara de la ficha
 * 	rotada. 
 * 	FIXME: ¿direction?
 */
PIECE* PIECE_rotate(PIECE *piece, int direction)
{
	
	PIECE *tmp = PIECE_create();
	memcpy(tmp, piece, sizeof(PIECE));
	
	if (direction == LEFT) tmp->rotation++;
//	else tmp->rotation += 3;
	
	tmp->rotation = tmp->rotation % 4;
	PIECE_getmask(tmp);

	/* Si la ficha quedara fuera del tablero devuelvo NULL */
	if ((PIECE_getmaxx(tmp) >= board_width) || 
			(PIECE_getminx(tmp) < 0) || 
			(PIECE_getmaxy(tmp) >= board_height)) return NULL;
	
	return tmp;
}

/*	PIECE *PIECE_move(PIECE *piece, int direction)
 *
 *	Devuelve la FICHA ontenida al mover la FICHA ingresada 
 *	como parámetro en la dirección especificada.
 *	Esta función no ubica la ficha en el tablero. Solamente 
 *	calcula la nueva máscara. 
 */
PIECE *PIECE_move(PIECE *piece, int direction)
{

	PIECE *tmp = PIECE_create();
	memcpy(tmp, piece, sizeof(PIECE));
			
	int j;
	switch (direction)
	{
		case DOWN:
		case DROP:
			if (PIECE_getmaxy(tmp) < board_height-1) {
	 			for (j=0; j<4; j++) tmp->mask[j].y++;
				tmp->y++;
			}
			else return NULL;
			break;
		case LEFT:
			if (PIECE_getminx(tmp) > 0) {
				for (j=0; j<4; j++) tmp->mask[j].x--;
				tmp->x--;
			}
			else return NULL;
			break;
		case RIGHT: 
			if (PIECE_getmaxx(tmp) < board_width-1) {
				for (j=0; j<4; j++) tmp->mask[j].x++;
				tmp->x++;
			}
			else return NULL;
			break;
	}
	return tmp;	
		
}

/*
 * 	void PIECE_getmask(PIECE *piece)
 *
 * 	Extrae la ubicación de los puntos de cada ladrillo en base
 * 	a la pieza y a su rotación.
 */
void PIECE_getmask(PIECE *piece)
{
	
	/* Copio la forma correspondiente */
	memcpy(piece->shape, shape[piece->number-1][piece->rotation], 4*4);
	
	int i,j,k=0;
	for (j=0; j<4; j++) {
		for (i=0; i<4; i++) {
			if (piece->shape[j][i] != 0) {
				piece->mask[k].x = i+piece->x;
				piece->mask[k].y = j+piece->y;
				k++;
			}
		}
	}
       	
}

/*
 * 	int PIECE_getmaxx(PIECE *piece)
 * 	int PIECE_getminx(PIECE *piece)
 * 	int PIECE_getmaxy(PIECE *piece)
 * 	int PIECE_getminy(PIECE *piece)
 *
 * 	Devuelven los extremos de la máscara de la ficha.
 * 	Es necesario TENER la máscara cargada en la estructura.
 */
int PIECE_getmaxx(PIECE *piece) 
{ 
	return max(max(piece->mask[0].x, piece->mask[1].x), max(piece->mask[2].x, piece->mask[3].x)); 
}
	
int PIECE_getminx(PIECE *piece) 
{ 
	return min(min(piece->mask[0].x, piece->mask[1].x), min(piece->mask[2].x, piece->mask[3].x)); 
}

int PIECE_getmaxy(PIECE *piece) 
{ 
	return max(max(piece->mask[0].y, piece->mask[1].y), max(piece->mask[2].y, piece->mask[3].y)); 
}

int PIECE_getminy(PIECE *piece) 
{ 
	return min(min(piece->mask[0].y, piece->mask[1].y), min(piece->mask[2].y, piece->mask[3].y)); 
}



/*
 * 	void BLOCK_draw(int number, int x, int y)
 *
 * 	Dibuja el ladrillo number en la posición (x,y) de la pantalla. 
 * 	Los ladrillos están en el archivo "blocks.bmp". Los primeros 
 * 	7 ladrillos son de color (rojo, rosa, azul, celeste, verde,
 * 	amarillo y beige). Los 7 restantes son grises.
 * 	Esta función necesita ser seguida de un UPDATE.
 *
 */
void BLOCK_draw(int number, int x, int y)
{

	SDL_Rect from;				
	SDL_Rect dest;
	
	/* Extraer datos del bloque seleccionado */
	memcpy(&from, &block_rectangle, sizeof(SDL_Rect));
	from.x = block_width * (number-1);

	dest.x = x;
	dest.y = y;
	dest.w = block_width;
	dest.h = block_height;

	SDL_BlitSurface(blocks, &from, screen, &dest);
	
}

/*
 * 	int BLOCK_random()
 *
 * 	Devuelve un número entre 1 y 7
 */
int BLOCK_random()
{
	int number = 1+(unsigned int)rand() % 7;
	return number; 
}
