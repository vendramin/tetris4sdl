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
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __stuff
#define __stuff

#include <stdlib.h>

#include "SDL.h"

#define FALSE		0
#define TRUE		1

#define LEFT		1
#define RIGHT		2
#define UP		3
#define DOWN		4
#define BOTTOM		5
#define DROP		6

#define min(a,b)	((a) > (b) ? (b) : (a))
#define max(a,b)	((a) < (b) ? (b) : (a))

#define SCREEN_WIDTH 	800
#define SCREEN_HEIGHT	600
					    
SDL_Surface 	*screen;

void 		GFX_init();							/* Inicializa SDL */
void 		GFX_quit();							/* Desinicializa SDL */
void 		loadbmp(char *filename);					/* Carga un bmp en pantalla */
#endif
