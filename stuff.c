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

#include "SDL.h"
#include "stuff.h"

extern SDL_Surface *screen;		/* Puntero a memoria de video */

/*
 * 	void GFX_init()
 *
 * 	Inicializa SDL 800x600x8.
 */
void GFX_init()
{

	printf("Inicializando SDL...\n");
	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		printf("ERROR: %s\n",SDL_GetError());
		SDL_Quit();
		exit(EXIT_FAILURE);
 	}

      	screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,8,SDL_HWSURFACE|SDL_HWPALETTE); //SDL_HWSURFACE o SDL_FULLSCREEN?
       	if (screen == NULL) {
	    	printf("ERROR: %s\n",SDL_GetError());
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

}

/*
 * 	void GFX_quit()
 *
 * 	Restaura el modo de video normal y desinstala SDL
 */
void GFX_quit()
{
	
	SDL_FreeSurface(screen);
	SDL_Quit();

}

/*
 *	void showbmp(char *filename)
 * 
 * 	Carga el BMP enviado como parámetro, lo pone en pantalla 
 * 	y carga su paleta de colores. 
 */
void showbmp(char *filename)
{

	SDL_Surface *bmp;
	bmp = SDL_LoadBMP(filename);
	if (bmp == NULL) {
		printf("ERROR: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
  	}

	if (bmp->format->palette && screen->format->palette) {
		SDL_SetColors(screen, bmp->format->palette->colors, 0,bmp->format->palette->ncolors);
	    	printf("%d colores\n", bmp->format->palette->ncolors);
	    	printf("%d colores (screen)\n", screen->format->palette->ncolors);
 	}
	
  	if (SDL_BlitSurface(bmp, NULL, screen, NULL) < 0) {
    		printf("ERROR: %s\n", SDL_GetError());
    		exit(EXIT_FAILURE);
  	}
	
	SDL_UpdateRect(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  	SDL_FreeSurface(bmp);

}

