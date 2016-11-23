/*
 * This file, rossify.h, is part of the distrat/askmap package
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

#include <stdio.h>
#ifdef DOS
#   include <dos.h>
#endif

#include <stdlib.h>

/* if we have limits.h then just include it */
#if !defined(NOLIMITS)
#   include <limits.h>
#else	    /* set minimum limits */
#  define UCHAR_MAX	255
#  define UINT_MAX	65535
#endif

#if !defined(ANYCHARPTR)
#   define ANYPTR  void *
#else
#   define ANYPTR  char *
#endif

struct pair {
    unsigned char first;
    unsigned char second;
    short status; };
typedef struct pair PAIR;

#define OMIT	0
#define ASSIGN	1

/* the rossify function is not very clean.  It will return an
   array of pairs in a sequence.  Internally it first works
   in a table and then copies pairs from that table to the
   array to be returned, although this costs time and
   memory doubles memory requirement, some positions in the
   table are not placed in the returned array;
   so the relationship between array positions and table cells
   is not transparent until the entire table is generated */

/* On machines where it is a problem to allocate more than 64K
   into one region (with one call to malloc), we hit  an array
   size limit.  Since we are allocating an array of pointers
   (usually each is four bytes long, that means that our total
   array of pointers can only be 16K long.  Since the array size
   is (E^2-E)/2 this usually leads to a maximum E of 181.

   I will also assume that any machine which has 8 byte pointers
   doesn't have the 64K array size problem.

   It would be possible to put in a bunch of tests for machine types,
   but then the code wouldn't always function on particular machines
   that I haven't anticipated.  I will assume that any machine will be
   able to at least allocated a single array 16384 * sizeof(char *).
   This makes for an element size of 181.  If these limits mean that
   you aren't getting the full power out of your Cray, well tough.

   Finally, for the purposes that Ross ordering is usually used,
   numbers above 20 or 30 are unlikely to be used.  A limit
   of 181 should not be constraining for practical purposes.
   ***********************************************************/

#if !defined(MAXELEM)
#   define MAXELEM	181
#endif

unsigned char **
rossify(int size)  /* this function will return a Ross ordering table */
{
    int i, j;  /* will be needed later */
    unsigned int asize;
    unsigned int wsize; /* working size */
    unsigned int rows, cols;
    unsigned char **rarr;
    PAIR **table;
    int f;
    int count;

    if (size < 2) {
	fprintf(stderr, "rossify(): must have more than 1 element\n");
	return((unsigned char **)NULL);
    }
    if (size > MAXELEM) {
	fprintf(stderr,
	    "rossify(): must be no more than %d elements\n", MAXELEM);
	return((unsigned char **)NULL);
    }

    asize= ((size-1)*size)/2;

    rarr=(unsigned char **) malloc(asize * sizeof(char *));
    if (rarr==(unsigned char **) NULL) {
	fprintf(stderr, "Error getting memory for Ross array\n");
	return((unsigned char **)NULL);
    }
    for(i=0; i < asize; ++i) {
	rarr[i] = (unsigned char *) malloc(2 * sizeof(char));
	if (rarr[i]==(unsigned char *) NULL) {
	    fprintf(stderr, "Error getting memory for Ross array\n");
	    return((unsigned char **)NULL);
	}
    }

    /* wsize must be odd so add one if size even */
    wsize = size + (size % 2 == 0);
    rows = (wsize + 1)/2; cols = wsize -1;
    /* get memory for table */
    table = (PAIR **) malloc(rows * (sizeof(struct pair *)));
    if(table == (PAIR **)NULL) {
	fprintf(stderr, "Error getting memory for Ross table\n");
	return((unsigned char **)NULL);
    }
    table -= 1;  /* to start from 1, not 0; Call to free() must
		    be offset as well */
    for(i = 1; i <= rows; ++i) {
	table[i] = (PAIR *) malloc(cols * (sizeof(struct pair)));
	if(table[i] == (PAIR *)NULL) {
	    fprintf(stderr, "Error getting memory for Ross table\n");
	    return((unsigned char **)NULL);
	}
	table[i] -= 1; /*subscripts from 1; adjust call to free() */
    }

    /* See Ross 1934 for this, the table is his table V */
    /* Different rules seem to apply to the odd and even
       columns of Ross's table V, also to the first and final
       rows of the table.  There must be a more general statement
       of the table or sequence then what I could induce from
       Ross, but the following algorithm works and is not
       dramatically wasteful.  Some parts can obviously
       be combined, but it may be clearer to leave things
       in their constituent parts this way */

    /* first row, odd columns */
    for(i=1; i <= cols; i += 2) {
	table[1][i].first=1;
	table[1][i].second=(i+3)/2;
	table[1][i].status=ASSIGN;
    }

    /* first row, even columns */
    for(i=2; i <= cols; i += 2) {
	table[1][i].second=(i+2)/2;
	table[1][i].first=table[1][i].second+1;
	table[1][i].status=ASSIGN;
    }

    /* last row, odd columns */
    /* Note that this is not exactly Ross's table V since I put
       the 1 in the second element and not the first.  This makes
       the array easier to balance */
    for(i=1; i <= cols; i += 2) {
	table[rows][i].second=1;
	table[rows][i].first=rows + (i+1)/2;
	table[rows][i].status=ASSIGN;
    }

    /* last row, even columns */
    for(i=2; i <= cols; i += 2)
	table[rows][i].status=OMIT;

    /* rows 2 through rows-1  */
    /* this steps through odd columns, but calculates the
       values for the subsequent even numbered column as well */
    for(i=1; i <= cols; i += 2) {
	for(j=2; j <= rows -1; ++j) {
	    if ((f=(i+1)/2 - (j -2)) > 1)
		table[j][i].first =f;
	    else
		table[j][i].first =(wsize-1)+f;
	    table[j][i].second =(i-1)/2 +3 + (j-2);
	    table[j][i].status=ASSIGN;

	    /* for one column over (even columns) */
	    table[j][i+1].second = table[j][i].first;
	    table[j][i+1].first = table[j][i].second+1;
	    table[j][i+1].status=ASSIGN;
	}
    }

    /* this next piece (which cuts things out when
       we only have an even number of elements) could be
       built into the above two `paragraphs', but is
       a bit more transparent here at little cost */
    if(size != wsize) { /* OMIT last items */
	for(i=1; i <= cols; ++i)
	    for(j=1; j <=rows; ++j)
		if(table[j][i].first == wsize ||
			table[j][i].second == wsize)
		    table[j][i].status=OMIT;
    }

    /* Ross does not explain how to balence the table (ensure
       that each number comes up equally as first or second item)
       But by inspection it looks like he inverts pairs in the
       odd numbered columns of Table V, the last column,
       and table V should be generated with 1 as the second
       element of final row even columns.  I have generated these
       that way already except of the final column which I
       still need to `flip'. */

    count = 0;
    for(i=1; i < cols; ++i)  /* all but last column */
	for(j=1; j <=rows; ++j) {
	    if(table[j][i].status != OMIT) {
		rarr[count][0] = table[j][i].first;
		rarr[count][1] = table[j][i].second;
		++count;
	    }
	}
    /* last column with first into [1] and second into [0] */
    for(j=1; j <=rows; ++j) {
	if(table[j][i].status != OMIT) {
	    rarr[count][1] = table[j][i].first;
	    rarr[count][0] = table[j][i].second;
	    ++count;
	}
    }
    
    if(count != asize) {
	fprintf(stderr, "Impossible error at %d in %s (count == %d)\n",
	    __LINE__, __FILE__, count);
	return((unsigned char **)NULL);
    }

    /* free stuff, remember, the -1 offset for the table */
    for(i=1; i <= rows; ++i)
	free((ANYPTR)(table[i]+1));
    free((ANYPTR)(table+1));

    return(rarr);
}
