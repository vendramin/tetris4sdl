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

#ifndef __highscore
#define __highscore

/***************************************************************************
 *   Constantes generales sobre HIGHSCORE				   *
 ***************************************************************************/
#define NOT_RANKED		-1

#define NAME_LENGTH		20	   	/* Longitud del nombre */
#define DATE_LENGTH		10		/* Longitud de la fecha */
#define HIGHSCORE_LENGTH	10		/* Cantidad de jugadores */
#define HIGHSCORE_NAME		"Leandro"	/* Nombre por default */
#define HIGHSCORE_DATE		"02/10/1976"	/* Fecha por default */


/***************************************************************************
 *   Estructuras                					   *
 ***************************************************************************/
typedef struct {
	char name[NAME_LENGTH];		/* Nombre del jugador */
	char date[DATE_LENGTH];		/* Fecha */
	int points;			/* Puntaje */
	int level;			/* Nivel alcanzado */
	int lines;			/* Cantidad de líneas */
} SCORE;

typedef struct {
	int length;			/* Longitud */
	SCORE *score;			/* Score */
} HIGHSCORE;


/***************************************************************************
 *   Funciones                 						   *
 ***************************************************************************/
HIGHSCORE*	HIGHSCORE_create(char *filename);
HIGHSCORE*	HIGHSCORE_load(char *filename);
HIGHSCORE*	HIGHSCORE_default();
void 		HIGHSCORE_destroy(HIGHSCORE *highscore);
void		HIGHSCORE_save(HIGHSCORE *highscore, char *filename);
void		HIGHSCORE_swap(HIGHSCORE *highscore, int i, int j);
void		HIGHSCORE_put(HIGHSCORE *highscore, SCORE *score, int j);
void		HIGHSCORE_show(HIGHSCORE *highscore);
int 		HIGHSCORE_where(HIGHSCORE *highscore, SCORE *score);

#endif
