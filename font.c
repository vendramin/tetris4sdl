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

#include "stuff.h"
#include "font.h"

extern SDL_Surface *screen;

/*
 *   	FONT *FONT_Create(char *filename, int size, int style)
 *	
 *	Crea un nuevo FONT TrueType. Los datos se leen del archivo 
 *	enviado como parámetro. El tamaño y el estilo también se 
 *	mandan como parámetro. 
 *	
 *	NO es necesario reservar memoria con malloc o similares.
 *	IMPORTANTE: DEBE estar inicializado el soporte para fonts TTF.
 */ 
FONT *FONT_create(char *filename, int size, int style)
{

	/* Se carga la TTF font */
	FONT *font = (FONT *)malloc(sizeof(FONT));
	font->ttf_font = TTF_OpenFont(filename, size);

	if ((font  == NULL) || (font->ttf_font == NULL)) return NULL;

	/* Estilo */
	TTF_SetFontStyle(font->ttf_font, style);

 	return font;	
	
}

/*
 * 	void FONT_destroy(FONT *font)
 *
 * 	Libera la memoria utilizada por FONT
 */
void FONT_destroy(FONT *font)
{
	TTF_CloseFont(font->ttf_font);	
	free(font);
}

void FONT_setcolor(FONT *font, int color, int shadow)
{
	font->fcolor = color;
	font->scolor = shadow;
}

/*	
 *	void FONT_writexy(FONT *font, char *string, int x, int y, 
 *				int color, int shadow, int update_screen)
 *
 *	Escribe en la posición (x,y) la cadena enviada como parámetro. 
 *	El número de color (8 bits) es enviado como parámetro y se lee 
 *	la paleta activa. Si update es TRUE, no será necesario 
 *	actualizar la pantalla.
 */
void FONT_writexy(FONT *font, char *string, int x, int y, int update_screen)
{

	SDL_Rect dest;
	SDL_Surface *buffer;
	
	buffer = TTF_RenderText_Blended(font->ttf_font, string, screen->format->palette->colors[font->scolor]);
	
	dest.x = x+2;
	dest.y = y+2;
	dest.w = buffer->w;
	dest.h = buffer->h;
	SDL_BlitSurface(buffer, NULL, screen, &dest);
	
	buffer = TTF_RenderText_Blended(font->ttf_font, string, screen->format->palette->colors[font->fcolor]);

	dest.x = x;
	dest.y = y;

	SDL_BlitSurface(buffer, NULL, screen, &dest);
	if (update_screen == TRUE) SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);

	return;
		
}

/*
 *	void FONT_center(FONT *font, char *string, 
 *				SDL_Rect *rectangle, int update_screen)
 *
 *	Escribe el string CENTRADO en el rectángulo 
 *	enviado como parámetro. 
 */
void FONT_center(FONT *font, char *string, SDL_Rect *rectangle, int update_screen)
{
	SDL_Rect dest;
	SDL_Surface *buffer;

	buffer = TTF_RenderText_Blended(font->ttf_font, string, screen->format->palette->colors[font->scolor]);
	
	dest.x = rectangle->x + (rectangle->w - buffer->w)/2;
	dest.y = rectangle->y + (rectangle->h - buffer->h)/2;
	dest.w = buffer->w;
	dest.h = buffer->h;
	SDL_BlitSurface(buffer, NULL, screen, &dest);
	
	buffer = TTF_RenderText_Blended(font->ttf_font, string, screen->format->palette->colors[font->fcolor]);

	dest.x = rectangle->x + (rectangle->w - buffer->w)/2 - 2;
	dest.y = rectangle->y + (rectangle->h - buffer->h)/2 - 2;
	
	SDL_BlitSurface(buffer, NULL, screen, &dest);
	if (update_screen == TRUE) SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);

}

/*
 * 	void FONT_right(FONT *font, char *string, SDL_Rect *rectangle, int update_screen)
 *
 * 	Escribe ALINEANDO a derecha el texto dentro del rectángulo
 * 	enviado como parámetro.
 */
void FONT_right(FONT *font, char *string, SDL_Rect *rectangle, int update_screen)
{
	SDL_Rect dest;
	SDL_Surface *buffer;

	buffer = TTF_RenderText_Blended(font->ttf_font, string, screen->format->palette->colors[font->scolor]);
	
	dest.x = rectangle->x + rectangle->w - buffer->w;
	dest.y = rectangle->y + (rectangle->h - buffer->h)/2;
	dest.w = buffer->w;
	dest.h = buffer->h;
	SDL_BlitSurface(buffer, NULL, screen, &dest);
	
	buffer = TTF_RenderText_Blended(font->ttf_font, string, screen->format->palette->colors[font->fcolor]);

	dest.x = rectangle->x + rectangle->w - buffer->w-2;
	dest.y = rectangle->y + (rectangle->h - buffer->h)/2-2;
	
	SDL_BlitSurface(buffer, NULL, screen, &dest);
	if (update_screen == TRUE) SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);

}

/*
 * 	void FONT_input(FONT *font, char *string, int x, int y, int w)
 *
 * 	Espera la entrada de un string de longitud n 
 * 	en la posición (x,y).
 * 	TODO: mostrar cursor
 * 	TODO: guardar imagen de fondo y escribir sobre eso
 */
void FONT_inputxy(FONT *font, char *string, int x, int y, int w)
{
	
	SDL_Event event;
	SDL_Surface *buffer;
	SDL_Rect buffer_rectangle;

	char *str = (char *)malloc(w+1);
	
	int done = FALSE;
	int changed = TRUE;

	int cursor = 0;
	int ch=-1;
	int unicode = SDL_EnableUNICODE(TRUE);

	if (string == NULL) str[0] = '\0'; 
	else strcpy(str, string);
	
	cursor = strlen(str);
	
	buffer_rectangle.x = x;
	buffer_rectangle.y = y;	
	buffer_rectangle.w = w * TTF_FontHeight(font->ttf_font);		// mmm...
	buffer_rectangle.h = TTF_FontHeight(font->ttf_font);

	while (done == FALSE) {
		if (event.type == SDL_KEYDOWN) {
			ch = event.key.keysym.unicode;

			switch(ch) {
				case SDLK_ESCAPE:
					done = TRUE;
					break;
				case SDLK_BACKSPACE:
					if (cursor > 0) {
						strcpy(str + cursor - 1, str + cursor);
						cursor--;
						changed = TRUE;
					}
					SDL_WaitEvent(&event);
					break;
				case SDLK_RETURN:
    					str[strlen(str)]='\0';
					strcpy(string, str);
					printf("ingresó: %s\n", str);
					done = TRUE;
					break;
				default:
					break;
			}
			
			if ((ch>31) && (ch<128) && (cursor < 20)) {
				sprintf(str, "%s%c", str, ch);
				str[++cursor] = '\0';
				SDL_WaitEvent(&event);
				changed = TRUE;
			}
			
		if (changed == TRUE) {
			printf("%s\n", str);
			SDL_FillRect(screen, &buffer_rectangle, 5);
			if (str[0] != '\0') FONT_writexy(font, str, x, y, FALSE);
			SDL_UpdateRect(screen, buffer_rectangle.x, buffer_rectangle.y, buffer_rectangle.w, buffer_rectangle.h);
			changed = FALSE;
		}
	}
	SDL_Delay(1);
	SDL_PollEvent(&event);
    }
    SDL_EnableUNICODE(unicode);  
}
