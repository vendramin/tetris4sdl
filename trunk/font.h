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

#ifndef __font
#define __font

#include "SDL.h"
#include "SDL_ttf.h"

typedef struct {
	TTF_Font *ttf_font;
	int fcolor;
	int scolor;
} FONT;

FONT*		FONT_create(char *filename, int size, int style);
void		FONT_destoy(FONT *font);
void		FONT_setcolor(FONT *font, int color, int shadow);
void 		FONT_writexy(FONT *font, char *string, int x, int y, int update_screen);
void 		FONT_center(FONT *font, char *string, SDL_Rect *rectangle, int update_screen);
void		FONT_right(FONT *font, char *string, SDL_Rect *rectangle, int update_screen);
void 		FONT_inputxy(FONT *font, char *string, int x, int y, int w);
#endif
