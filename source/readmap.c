/*
 * This file, readmap.c, is part of the distrat/askmap package
 * by Jeff Goldberg Copyright (c) 1992, 1993, 1994
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "distrat.h"

/* rmap1 reads in a file from the disk, it will allocate 
a space needed and return a pointer to the (struct cm *)map
This function (and this one alone) defines the form of the
format of the map files on the disk, so only this fuction should
be modified for other forms of storing maps */

struct cm *
rmap1(FILE *fp)
{
    unsigned int i,j;
    struct cm *map;
    unsigned int v;	/* temp var */
    float fv;		/* temp var */
    char str[256];
    extern int Maxc;

    /* get memory for maps */
    if ((map = (struct cm *) malloc(sizeof(struct cm))) == CMNULL) {
	fprintf(stderr, "Couldn't get mem for map. Exiting.\n");
	exit(EMEM);
    }

    /* get space for name if it doesn't exist yet */
    map->name=malloc( (MAXSTR+1) * sizeof(char));
    if (map->name == NULL) {
	fprintf(stderr, "Can't get memory for map name\n");
	exit(EMEM);
    }

    if(fscanf(fp, "Map id: %s\n", map->name) != 1 && map->name[0]) {
	fprintf(stderr, "Error reading map ID.  Exiting.\n");
	exit (EINPUT);
    }
    /* read in number of constructs and construct numbers */
    fgets(str, 256, fp);
    map->size=atoi(str);
    if(map->size < 2 || map->size > Maxc) {
	fprintf(stderr,"Bad number of nodes (%d) ",map->size);
	fprintf(stderr,"in map %s.\n",map->name);
	fprintf(stderr,"Should be 1 < N <= %d\n", Maxc);
	exit(EINPUT);
    }
    /* get space for nodes if it doesn't exist yet */
    map->nodes=ivector(1, map->size);
    map->table=matrix(1, map->size, 1, map->size);
    map->min=Maxc;
    map->max=1;
    for(i=1; i <= map->size; ++i) {
	if(fscanf(fp, "%d", &v) != 1) {
	    fprintf(stderr,"Error reading node numbers ");
	    fprintf(stderr, "in map %s.  Exiting\n", map->name);
	    exit (EINPUT);
	}
	if( v < 1 || v > Maxc) {
	    fprintf(stderr, "Bad node number %d. Exiting.\n", v);
	    exit(ENODE);
	}
	map->nodes[i]=v;
	map->max=MAX(map->max, v);
	map->min=MIN(map->min, v);
    }
    for(i=1; i<=map->size; ++i) {
	for(j=1; j<=map->size; ++j) {
	    if(fscanf(fp, "%f", &fv) != 1) {
		fprintf(stderr, "Error reading map.  Exiting.\n");
		exit(EINPUT);
	    }
	    map->table[i][j]=fv;
	}
    }
    return(map);
}

/* If you wish to write your own map reading routines for
   maps files of froms other than what is in rmap1() use
   one of the functions below.  If your form is not something
   used by programs available generally, them please use a
   rmap[789].  If you are writing a form for maps that you
   expect to be in general use, please use a lower number
   rmap[3456].  In any case, try to get in touch with Jeff Goldberg
   so that these editions can be co-ordinated.  */

#if defined (__TURBOC__)
#  pragma argsused
#endif

struct cm *
rmap2(FILE *fp)
{
    fprintf(stderr, "Map form %d is not an available form.  ", 2);
    fprintf(stderr, "Map was not read\n");
    return(CMNULL);
}

#if defined (__TURBOC__)
#  pragma argsused
#endif


struct cm *
rmap3(FILE *fp)
{
    fprintf(stderr, "Map form %d is not an available form.  ", 3);
    fprintf(stderr, "Map was not read\n");
    return(CMNULL);
}


#if defined (__TURBOC__)
#  pragma argsused
#endif

struct cm *
rmap4(FILE *fp)
{
    fprintf(stderr, "Map form %d is not an available form.  ", 4);
    fprintf(stderr, "Map was not read\n");
    return(CMNULL);
}


#if defined (__TURBOC__)
#  pragma argsused
#endif

struct cm *
rmap5(FILE *fp)
{
    fprintf(stderr, "Map form %d is not an available form.  ", 5);
    fprintf(stderr, "Map was not read\n");
    return(CMNULL);
}


#if defined (__TURBOC__)
#  pragma argsused
#endif

struct cm *
rmap6(FILE *fp)
{
    fprintf(stderr, "Map form %d is not an available form.  ", 6);
    fprintf(stderr, "Map was not read\n");
    return(CMNULL);
}


#if defined (__TURBOC__)
#  pragma argsused
#endif

struct cm *
rmap7(FILE *fp)
{
    fprintf(stderr, "Map form %d is not an available form.  ", 7);
    fprintf(stderr, "Map was not read\n");
    return(CMNULL);
}


#if defined (__TURBOC__)
#  pragma argsused
#endif

struct cm *
rmap8(FILE *fp)
{
    fprintf(stderr, "Map form %d is not an available form.  ", 8);
    fprintf(stderr, "Map was not read\n");
    return(CMNULL);
}


#if defined (__TURBOC__)
#  pragma argsused
#endif

struct cm *
rmap9(FILE *fp)
{
    fprintf(stderr, "Map form %d is not an available form.  ", 9);
    fprintf(stderr, "Map was not read\n");
    return(CMNULL);
}
