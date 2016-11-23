/*
 * This file, writemap.c, is part of the distrat/askmap package
 * by Jeff Goldberg Copyright (c) 1993, 1994
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

void
wmap1(fp, m)   /* writes as int */
FILE *fp;
struct cm *m;
{
    int i, j;

    fprintf(fp, "Map id: %s\n%d\n", m->name, m->size);
    for(i=1; i <= m->size; ++i)
	fprintf(fp, "%2d%c", m->nodes[i], i == m->size ? '\n': ' ');
    for(i=1; i <= m->size; ++i) 
	for(j=1; j <= m->size; ++j) 
	    fprintf(fp, "%c%2d",
		(j % m->size == 1)? '\n' : ' ', (int) m->table[i][j]);
    fputc('\n', fp);
}

void
wmap2(fp, m)   /* writes as float X.XX */
FILE *fp;
struct cm *m;
{
    int i, j;

    fprintf(fp, "Map id: %s\n%d\n", m->name, m->size);
    for(i=1; i <= m->size; ++i)
	fprintf(fp, "%6d%c", m->nodes[i], i == m->size ? '\n': ' ');
    for(i=1; i <= m->size; ++i) 
	for(j=1; j <= m->size; ++j) 
	    fprintf(fp, "%c%6.3f",
		(j % m->size == 1)? '\n' : ' ', m->table[i][j]);
    fputc('\n', fp);
}

#if defined (__TURBOC__)
#  pragma argsused
#endif

void
wmap3(FILE *outfp, struct cm *map)
{
    fprintf(stderr, "This writing form does not exist\n");
}

#if defined (__TURBOC__)
#  pragma argsused
#endif

void
wmap4(FILE *outfp, struct cm *map)
{
    fprintf(stderr, "This writing form does not exist\n");
}

#if defined (__TURBOC__)
#  pragma argsused
#endif

void
wmap5(FILE *outfp, struct cm *map)
{
    fprintf(stderr, "This writing form does not exist\n");
}

#if defined (__TURBOC__)
#  pragma argsused
#endif

void
wmap6(FILE *outfp, struct cm *map)
{
    fprintf(stderr, "This writing form does not exist\n");
}

#if defined (__TURBOC__)
#  pragma argsused
#endif

void
wmap7(FILE *outfp, struct cm *map)
{
    fprintf(stderr, "This writing form does not exist\n");
}

#if defined (__TURBOC__)
#  pragma argsused
#endif

void
wmap8(FILE *outfp, struct cm *map)
{
    fprintf(stderr, "This writing form does not exist\n");
}

#if defined (__TURBOC__)
#  pragma argsused
#endif

void
wmap9(FILE *outfp, struct cm *map)
{
    fprintf(stderr, "This writing form does not exist\n");
}
