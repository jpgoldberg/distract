/*
 * This file, ross.c, is part of the distrat/askmap package
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

/* simple test of the rossify function */

#include "distrat.h"	/* needed only for version, getopt and exit states */

static void usage(char *);

int
main(int ac, char **av)
{
    unsigned char **rossify(int), **ross;
    int size, i;
    int opt;

    fprintf(stderr,
	"Ross ordering %s. (C) 1993-1995 by Jeff Goldberg\n", version);
    fprintf(stderr, "Use -L option for license information\n");

    while((opt= getopt(ac, av, "hL")) != EOF)
	switch(opt) {
	    case 'L': License(stdout); exit(0);
	    case 'h': usage(av[0]); exit(0);
	    case '?': usage(av[0]);  exit(EUSE);
	}
    switch(ac-optind) {
	case 0:
	    fprintf(stderr, "Not enough arguments\n");
	    usage(av[0]); exit(EUSE);
	case 1:
	    size = atoi(av[optind]); break;
	default:
	    fprintf(stderr, "Too many arguements\n");
	    usage(av[0]); exit(EUSE);
    }

    if (size < 3) {
	fprintf(stderr, "Must use number greater than 2\n");
	exit(EUSE);
    }


    if((ross=rossify(size)) == (unsigned char **)NULL) {
	fprintf(stderr, "Error getting ross array\n"); exit(1);
    }

    for(i=0; i < (size * (size-1))/2; ++i) 
	fprintf(stdout, "%d: %d %d\n", i+1, ross[i][0], ross[i][1]);
    
    exit(0);
#if defined (__TURBOC__)
#   pragma warn -rvl
#endif
}
#if defined (__TURBOC__)
#   pragma warn .rvl
#endif

static void   
usage(char *s)
{
    fprintf(stderr, "Usage: %s [hL] size\n", s);
    fprintf(stderr, "\tsize is number of elements\n");
    fprintf(stderr, "\tOptions:\n");
    fprintf(stderr, "\t\t-h\tdisplay this message\n");
    fprintf(stderr, "\t\t-L\tdisplay License information\n");
}
