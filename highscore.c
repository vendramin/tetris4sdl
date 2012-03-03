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

#include <string.h>
#include <stdio.h>
#include "highscore.h"

////////////
//
// TODO: sacarla de aca
//
////////////
int filesize(FILE *stream)
{

	int size, save_position;
	save_position = ftell(stream);
	fseek(stream, 0L, SEEK_END);
	size = ftell(stream);
	fseek(stream, save_position, SEEK_SET);
	return size;

}

/*
 *   	HIGHSCORE *HIGHSCORE_create(char *filename)
 *   	
 *	Crea un nuevo HIGHSCORE. Se intenta leer los datos del archivo enviado 
 *	como parámetro. En caso de no existir el archivo, se crea un HIGHSCORE por 
 *	default con la función HIGHSCORE_default pero NO se grabará en disco hasta 
 *	no utilizarse la función HIGHSCORE_save 
 *
 *	NO es necesario reservar memoria con malloc o similares.
 */
HIGHSCORE *HIGHSCORE_create(char *filename)
{

	HIGHSCORE *highscore = HIGHSCORE_load(filename);
	if (highscore == NULL) {
		printf("El archivo %s no existe. Se creará uno con datos por default\n", filename);
		return HIGHSCORE_default();
	}
	else 
		return highscore;

}

/*
 * 	HIGHSCORE *HIGHSCORE_load(char *filename)
 *   
 *  	Devuelve un puntero a la estructura que contiene los datos leidos 
 *  	del archivo "filename". Si el archivo no existe, devuelve NULL. 
 *  	Esta función es utilizada en HIGHSCORE_create. 
 *  	
 *  	NO es necesario reservar memoria con malloc o similares.
 *	FIXME: Agregar chequeo en malloc
 */   	
HIGHSCORE *HIGHSCORE_load(char *filename)
{
    
	FILE *fp;
	if ((fp = fopen(filename, "rb")) == NULL) 
		return NULL;
	else {
		int length = filesize(fp)/sizeof(SCORE);
		HIGHSCORE *highscore = (HIGHSCORE *)malloc(sizeof(HIGHSCORE));
		if (highscore == NULL) return NULL;
		highscore->length = length;
		highscore->score = (SCORE *)malloc(highscore->length*sizeof(SCORE));
		fread(highscore->score, sizeof(SCORE), highscore->length, fp);
		fclose(fp);
		return highscore;
	}
	    
}

/*
 * 	HIGHSCORE *HIGHSCORE_default()
 *   
 * 	Devuelve un puntero a la estructura que contiene datos 
 * 	por default. Esta función es utilizada en HIGHSCORE_create 
 *	en caso de no existir un archivo de dónde leer los datos.
 *
 *	NO es necesario reservar memoria con malloc o similares.
 *	FIXME: Agregar chequeos en malloc
 */
HIGHSCORE *HIGHSCORE_default()
{

	HIGHSCORE *highscore = (HIGHSCORE *)malloc(sizeof(HIGHSCORE));
	
	highscore->length = HIGHSCORE_LENGTH;
	highscore->score = (SCORE *)malloc(sizeof(SCORE)*highscore->length);

	int i;
	for (i=0; i<highscore->length; i++) {
		strcpy(highscore->score[i].name, HIGHSCORE_NAME);
		strcpy(highscore->score[i].date, HIGHSCORE_DATE);
		highscore->score[i].points = 10000-1000*i;
		highscore->score[i].lines = 100-10*i;
		highscore->score[i].level = 10-i;
	}
		
	return highscore;

}



/*
 *	void HIGHSCORE_quit(HIGHSCORE *highscore)
 *	
 *  	Libera la memoria utilizada por la variable HIGHSCORE. 
 *  	IMPORTANTE: para guardar el HIGHSCORE en un archivo debe
 *  	utilizarse la función HIGHSCORE_save.
 */
void HIGHSCORE_destroy(HIGHSCORE *highscore)
{

	free(highscore->score);
	free(highscore);

}

/*
 *  	void HIGHSCORE_save(HIGHSCORE *highscore, char *filename)
 *
 *  	Se crea un archivo y se vuelca el contenido del HIGHSCORE. En caso de 
 *  	existir el archivo, se sobreescribe.
 *	FIXME: Agregar chequeo al crear el archivo. Debe devolver -1 
 *	en caso de existir algún error
 */
void HIGHSCORE_save(HIGHSCORE *highscore, char *filename)
{

	FILE *fp;
	fp = fopen(filename, "wb");
	fwrite(highscore->score, sizeof(SCORE), highscore->length, fp);
	fclose(fp);
  
}

/*
 *   	void HIGHSCORE_swap(HIGHSCORE *highscore, int i, int j)
 *   	
 *   	Intercambia los registros i, j del HIGHSCORE.
 */
void HIGHSCORE_swap(HIGHSCORE *highscore, int i, int j)
{

	SCORE tmp;
	memcpy(&tmp, &highscore->score[i-1], sizeof(SCORE));
	memcpy(&highscore->score[i-1], &highscore->score[j-1], sizeof(SCORE));
	memcpy(&highscore->score[j-1], &tmp, sizeof(SCORE));

}

/*
 *   	void HIGHSCORE_put(HIGHSCORE *highscore, SCORE *score, int j)
 *
 *   	Pone SCORE en HIGHSCORE en la posición j-ésima. El último de los 
 *   	registros en HIGHSCORE se pierde.
 *	FIXME: Agregar chequeo en realloc. La función debería devolver 
 *	-1 en caso de existir algún error.
 */
void HIGHSCORE_put(HIGHSCORE *highscore, SCORE *score, int j)
{
	
	int k;
	int l=HIGHSCORE_where(highscore, score);
	
	/* Se agrega score al final del listado */
	highscore->score = (SCORE *)realloc(highscore->score, (highscore->length+1)*sizeof(SCORE));
	memcpy((highscore->score)+highscore->length-1, score, sizeof(SCORE));

	/* Ahora se corre score hasta la posición en la que tiene que estar */
	for (k=highscore->length-1; k>l; k--) 
		HIGHSCORE_swap(highscore, k, k+1);

}

/*	void HIGHSCORE_show(HIGHSCORE *highscore)
 *
 *	Muestra el lista con los mejores puntajes.
 */
void HIGHSCORE_show(HIGHSCORE *highscore)
{
	int j;
	printf("Nombre--------------#-Fecha----# Points   # Nivel    # Lineas\n");
	for (j=0; j<highscore->length; j++)
		printf("%20s, %10s, %6d, %3d, %3d\n", highscore->score[j].name,
			highscore->score[j].date, highscore->score[j].points,
			highscore->score[j].level, highscore->score[j].lines);
	printf("-------------------------------------------------------------\n");
}

/*
 *   	int HIGHSCORE_where(HIGHSCORE *highscore, SCORE *score)
 *   	
 *   	Devuelve la posición dentro del HIGHSCORE en la que debe ubicarse
 *   	la variable SCORE (ordenado en forma decreciente con respecto al 
 *   	puntaje). En caso de que el SCORE no entre dentro del HIGHSCORE 
 *   	esta función devuelve -1 (NOT_RANKED).
 *
 *   	El primero del HIGHSCORE es el número cero.
 */
int HIGHSCORE_where(HIGHSCORE *highscore, SCORE *score)
{
	
	int j;
	for (j=0; j<highscore->length; j++) 
		if (highscore->score[j].points < score->points) return j;
	return highscore->length;
}
