/*
 * This file, drsort.c, is part of the distrat/askmap package
 * by Jeff Goldberg Copyright (c) 1994
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

/*****************************************************************
 *
 * This file contains main() and all other functions necessary for
 * drsort program.  The program will organize the output of distrat
 * into a table possibly for use by statistical programs
 *
 *****************************************************************/

#include<stdio.h>
#include<math.h>
#include <string.h>
#include<stdlib.h>

#include "distrat.h"	/* only needed for version and License,
			   and exit states */


#define TRUE	1
#define FALSE	0

#define EPS   (.0001)	/* How close to an int to be an integer root */

struct fl_list {	/* structure for a list of floats */
    float item;
    struct fl_list *next; };

struct vn_list {	/* list of variable names */
    char *vname;
    struct vn_list *next;};

#define FLNULL (struct fl_list *)NULL
#define VLNULL (struct vn_list *)NULL

int debug = 0;
unsigned char upright, lowleft, full, diagzero, nameflg, countflg;

/**
 *  Unless someone has told us otherwise, assume we don't have
 *  double rint(double) which appears in some math libs
 **/

#define myrint(x)  floor((x) + 0.49999)


/** see()  get value of a half table
 *  Macro for looking at half table.  Note that arguements
 *  appear several times; so never use something like
 *  see(table, i++, j++) ! Not Ever!
 **/

#define see(w, a, b) ((a < b)? w[a][b] : ((a > b)? w[b][a]: 0.0))

static void usage(void);
static void printflist(struct fl_list *);

int
main(int ac, char **av)
{
    int i, j;
    struct fl_list *dlist=FLNULL, *dlist_end=FLNULL,
	 *dlist_new, *dlist_tmp, *dlist_otmp;
    char name1[37], name2[37];	    /* for scanf */
    struct vn_list *names=VLNULL, *namesroot=VLNULL;
    char *secondname=NULL, *thirdname=NULL;
    int namectr=0;
    int morenames=TRUE;
    int scanret, maps, line=0;
    float fmaps, temp_f;
    float **table;

    /* stuff for getopt */
    extern int optind;
    extern char *optarg;
    int opt;

    /* option flags */
    extern unsigned char upright, lowleft, full, diagzero, nameflg, countflg;

    /* set defaults */
    upright = FALSE; lowleft = TRUE; full = FALSE; diagzero = TRUE;
    nameflg=FALSE; countflg=FALSE;

    fprintf(stderr,
	"Sort distrat output %s: (C) Jeff Goldberg 1994-95\n",
	version);
    fprintf(stderr, "Use -L option for license information\n");

    while((opt=getopt(ac, av, "cnlufezd:hL")) != EOF) {
	switch(opt) {
	    case 'h': usage(); exit(0); break;
	    case 'L': License(stdout); exit(0);
	    case 'l': upright = FALSE; lowleft = TRUE; full=FALSE; break;
	    case 'u': upright = TRUE; lowleft = FALSE; full=FALSE; break;
	    case 'f': upright = FALSE; lowleft = FALSE; full=TRUE; break;
	    case 'z': diagzero = TRUE; break;
	    case 'e': diagzero = FALSE; break;
	    case 'n': nameflg = TRUE; break;
	    case 'c': countflg = TRUE; break;
	    case 'd': debug = atoi(optarg); break;
	    case '?':
	    default: usage(); exit(EUSE); break;
	}
    }
    if(full) diagzero=TRUE;
    if(optind != ac) {	/* should be no command line arguements */
	usage();
	exit(EUSE);
    }

    /** Read though input,  putting the number we want into   **
     ** the variable temp_f; until error or EOF		     **/
    while((scanret=scanf("DR(%s %s =%f\n", name1, name2, &temp_f))
		!= EOF) {
	++line;
	name1[strlen(name1)-1] = '\0';	/* kill final ',' */
	name2[strlen(name2)-1] = '\0';	/* kill final ')' */
	if(scanret != 3) {  /* somethig didn't parse right */
	    fprintf(stderr, "Error on input line %d\n", line);
	    fprintf(stderr, "\tscanret == %d\n", scanret);
	    fprintf(stderr, "\tname1 == %s\n", name1);
	    fprintf(stderr, "\tname2 == %s\n", name2);
	    fprintf(stderr, "\ttemp_f == %f\n", temp_f);
	    exit(EINPUT);
	}
	if (debug > 3) 
	    fprintf(stderr, "Read in %f from line %d\n", temp_f, line);
	if (morenames) {
	    /* deal with name list; first time through we alloc everything */
	    if (line==1) {
		names = (struct vn_list *)malloc(sizeof(struct vn_list));
		if (!names) {
		    fprintf(stderr, "Memory error at line %d\n", line);
		    exit(EMEM);
		}
		secondname=mystrdup(name2);
		if (!secondname) {
		    fprintf(stderr, "Memory error at line %d\n", line);
		    exit(EMEM);
		}
		names->vname=mystrdup(name1);
		if (!names->vname) {
		    fprintf(stderr, "Memory error at line %d\n", line);
		    exit(EMEM);
		}
		names->next=(struct vn_list *)malloc(sizeof(struct vn_list));
		if (!names->next) {
		    fprintf(stderr, "Memory error at line %d\n", line);
		    exit(EMEM);
		}
		namesroot=names;
		names=names->next;
		names->vname=secondname;
		namectr=2;
	    } else if (line==2) {   /* second line */
		thirdname=mystrdup(name2);
		if (!thirdname) {
		    fprintf(stderr, "Memory error at line %d\n", line);
		    exit(EMEM);
		}
		names->next=(struct vn_list *)malloc(sizeof(struct vn_list));
		if (!names->next) {
		    fprintf(stderr, "Memory error at line %d\n", line);
		    exit(EMEM);
		}
		names=names->next;
		names->vname=thirdname;
		names->next=VLNULL;
		++namectr;
	    } else { /* other lines, looking for name1==second && name2==third */
		if(!strcmp(name1,secondname) && !strcmp(name2,thirdname)) {
		    /* have reached end of names */
		    names->next=VLNULL;
		    morenames=FALSE;
		} else {
		    names->next=(struct vn_list *)malloc(sizeof(struct vn_list));
		    if (!names->next) {
			fprintf(stderr, "Memory error at line %d\n", line);
			exit(EMEM);
		    }
		    names=names->next;
		    names->vname=mystrdup(name2);
		    if (!names->vname) {
			fprintf(stderr, "Memory error at line %d\n", line);
			exit(EMEM);
		    }
		    names->next=VLNULL;
		    ++namectr;
		}
	    }
	} /* end for if morenames */
	/* get new link for new float */
	dlist_new = (struct fl_list *)malloc(sizeof(struct fl_list));
	if(dlist_new == FLNULL) {
	    fprintf(stderr, "Memory error at line %d\n", line);
	    exit(EMEM);
	}
	dlist_new->next = FLNULL;
	dlist_new->item = temp_f;
	if(dlist == FLNULL) dlist = dlist_new;	/* first link */
	else      dlist_end->next = dlist_new;	/* subsequent links */
	dlist_end = dlist_new;
    }

    if(debug > 4) printflist(dlist);

    /* check that number of pairs entails an integer number of
       maps */
    fmaps = (sqrt(8.0*(float)line +1.0) +1.0)/2.0;
    if ( fabs(fmaps - myrint(fmaps)) > EPS ) {
	fprintf(stderr,
	     "Error: bad number of distances (%d) suggesting %f maps\n",
	     line, fmaps);
	exit(EINPUT);
    }
    /* cast "(int) float" may yield warning, but is intended */
    maps = (int) myrint(fmaps);
    if (maps != namectr) {
	fprintf(stderr,
	    "Warning: variable names (%d) not equal maps (%d)\n",namectr,maps);
	if (debug > 0) {
	    fprintf(stderr, "secondname == %s; thridname == %s\n",
		secondname, thirdname);
	}
    }
    if (debug > 1) {
	fprintf(stderr, "There are %d distances and %d maps\n",
		    line, maps);
    }

    if(countflg)
	printf("%d\n", maps);
    
    /* row by row get a half matrix and copy stuff into it */

    /* table needs one row less then the number of maps */
    table = (float **) malloc(sizeof(float *)*(maps -1));
    if (table == (float **) NULL) {
	fprintf(stderr, "Error: Couldn't get memory for table\n");
	exit(EMEM);
    }
    table -= 1;	/* table will be a 1 off-set matrix */
    dlist_tmp = dlist;
    for(i=1; i < maps; ++i) {
	table[i] = (float *) malloc(sizeof(float) * (maps - i));
	if (table[i] == (float *) NULL) {
	    fprintf(stderr,
		 "Error: Couldn't get mem for %dth row of table\n",i);
	    exit(EMEM);
	}
	table[i] -= i+1;    /* row 1 starts at 2, row 2 starts at 3, ... */
	for(j=i+1; j <= maps; ++j) {
	    table[i][j] = dlist_tmp->item;
	    /* free up used stuff in linked list, I wouldn't
	     * need to use otmp if I could rely on free not
	     * really freeing until next call to malloc, but ANSI
	     * says no  */
	    dlist_otmp=dlist_tmp;
	    dlist_tmp = dlist_tmp->next;
	    free((char *)dlist_otmp);
	}
    }
    /* if name flag, print out names across the top */
    if(nameflg) {
	printf("%s",namesroot->vname);
	for(names=namesroot->next; names; names=names->next) {
	    printf(" %s", names->vname);
	}
	putchar('\n');
    }

    /* print out table depending on setting of output forms */
    
    if(full) {
	for(i=1; i <= maps; ++i) {
	    for(j=1; j <= maps; ++j)
		printf("%7.4f", see(table,i,j));
	    putchar('\n');
	}
    } else if (lowleft && diagzero) {
	for(i=1; i <= maps; ++i) {
	    for(j=1; j <= i; ++j)
		printf("%7.4f", see(table,i,j));
	    putchar('\n');
	}
    } else if (lowleft ) {  /* empty diagonal */
	for(i=2; i <= maps; ++i) {
	    for(j=1; j < i; ++j)
		printf("%7.4f", see(table,i,j));
	    putchar('\n');
	}
    } else if (upright && diagzero) {
	for(i=1; i <= maps; ++i) {
	    for(j=1; j <= maps; ++j) {
		if(j >= i)
		    printf("%7.4f", see(table,i,j));
		else
		    printf("       ");
	    }
	    putchar('\n');
	}
    } else {  /* upper right no empty diagonals */
	for(i=1; i < maps; ++i) {
	    for(j=1; j <= maps; ++j) {
		if(j > i)
		    printf("%7.4f", see(table,i,j));
		else
		    printf("       ");
	    }
	    putchar('\n');
	}
    }
    exit(0);
#if defined (__TURBOC__)
#   pragma warn -rvl
#endif
}
#if defined (__TURBOC__)
#   pragma warn .rvl
#endif


static void
printflist(struct fl_list * list)
{
    int i=1;
    struct fl_list *temp_list;

    for(temp_list=list;temp_list != FLNULL;temp_list=temp_list->next){
	fprintf(stderr,"%d:  %f\n", i++, temp_list->item);
    }
}

#define ONOFF(f)  ((f)? "ON": "OFF")
static void
usage(void)
{
    extern unsigned char upright, lowleft, full, diagzero, nameflg, countflg;

    fprintf(stderr, "Usage:\n\tdrsort [-lufezh] [-d N]\n");
    fprintf(stderr, "\t\t-h\tthis message\n");
    fprintf(stderr, "\t\t-L\tdisplay license information\n");
    fprintf(stderr, "\t\t-l\tLower left corner [%s]\n",ONOFF(lowleft));
    fprintf(stderr, "\t\t-u\tUpper right corner [%s]\n", ONOFF(upright));
    fprintf(stderr, "\t\t-f\tFull matrix (implies -z) [%s]\n", ONOFF(full));
    fprintf(stderr, "\t\t-z\tZero's in diagonal [%s]\n", ONOFF(diagzero));
    fprintf(stderr, "\t\t-e\tEmpty diagonals [%s]\n", ONOFF(!diagzero));
    fprintf(stderr, "\t\t-n\tPrint variable names [%s]\n",ONOFF(nameflg));
    fprintf(stderr, "\t\t-c\tPrint number of variables [%s]\n",
	 ONOFF(countflg));
    fprintf(stderr, "\t\t-d N\tDebug info level [%d]\n", debug);
}
