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
#include "font.h"
#include "stuff.h"
#include "highscore.h"

/*
 * 	debug()
 * 	
 * 	Imprime el tablero para DEBUG
 */	
void debug()
{

	int i,j;
	for (j=0; j<20; j++) {
	    for (i=0; i<10; i++) {
		if (i==9) printf("(%d,%d,%d)\n", i,j,player->board[10*j+i]);
		else printf("(%d,%d,%d) ", i,j,player->board[10*j+i]);
	    }
	}
	
	for (j=0; j<20; j++) {
	    for (i=0; i<10; i++) {
		if (i==9) printf("%d\n", player->board[10*j+i]);
		else printf("%d", player->board[10*j+i]);
	    }
	}
}

int main()
{

 	TETRIS_init();
	TETRIS_run();

	TETRIS_quit();
	
	printf("Programa finalizado.\n");
  
  	return TRUE;

}



