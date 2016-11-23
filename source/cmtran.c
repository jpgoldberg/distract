/*
 * This file, cmtran.c, is part of the distrat/askmap package
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

/* translate from one file form to another, (restricted to
   those forms that require no more information than is
   int struct cm  */

#include "distrat.h"

int debug = DEF_DB;			/* debug level */
int Maxc = MAXC;
int Maxf = 200;
struct cm * (*readmap)(FILE *);		/* ptr to func which reads maps */
void (*writemap)(FILE *, struct cm *);	/* ptr to func which writes maps */
static void usage(char *progname);

int
main(int ac, char **av)
{
    struct cm *map;
    FILE  *lfp;			/* input file pointer */
    extern int optind;
    extern char *optarg;
    int opt;
    extern int debug;		/* debug level */
    char lfile[MAXSTR];
    char tmpstr[MAXSTR];
    int i;
    char **files;
    int filecnt=0;

    readmap=rmap1;
    writemap=wmap1;

    fprintf(stderr,
	"CM Translate %s. Copyright 1994-95 by Jeff Goldberg\n", version);
    fprintf(stderr, "Use -L option for license information\n");
    /* parse command line with getopt */
    lfile[0]='\0';
    while((opt= getopt(ac, av, "i:o:d:hf:L")) != EOF) 
	switch(opt) {
	    case 'd': debug = atoi(optarg); break;
	    case 'm': 	/* number of possible maps */
		Maxf = atoi(optarg);
		if  (Maxc < 2) {
		    fprintf(stderr, "Max maps must be > 2.");
		    fprintf(stderr, "  Proceeding with %d\n",
					MAXF);
		    Maxf=MAXF;
		}
		break;
	    case 'i':  /* which map reader */
		switch(optarg[0]) {
		    case '1': readmap = rmap1; break;
		    case '2': readmap = rmap2; break;
		    case '3': readmap = rmap3; break;
		    case '4': readmap = rmap4; break;
		    case '5': readmap = rmap5; break;
		    case '6': readmap = rmap6; break;
		    case '7': readmap = rmap7; break;
		    case '8': readmap = rmap8; break;
		    case '9': readmap = rmap9; break;
		    default:
			fprintf(stderr,
			    "intype (-i) must be between 1 and 9\n");
			fprintf(stderr, "Proceeding with type 1\n");
			break;
		}
		break;
	    case 'o':  /* which map writer */
		switch(optarg[0]) {
		    case '1': writemap = wmap1; break;
		    case '2': writemap = wmap2; break;
		    case '3': writemap = wmap3; break;
		    case '4': writemap = wmap4; break;
		    case '5': writemap = wmap5; break;
		    case '6': writemap = wmap6; break;
		    case '7': writemap = wmap7; break;
		    case '8': writemap = wmap8; break;
		    case '9': writemap = wmap9; break;
		    default:
			fprintf(stderr,
			    "ouptype (-o) must be between 1 and 9\n");
			fprintf(stderr, "Proceeding with type 1\n");
			break;
		}
		break;
	    case 'f': strcpy(lfile, optarg); break;
	    case 'L': License(stdout); exit(0);
	    case 'h': usage(av[0]); exit(0);
	    case '?':
		usage(av[0]);  exit(EUSE);
	}


    files=(char **) malloc(sizeof(char *) * (Maxf+1));
    /* check non-option arguments */
    if(lfile[0]) {
	switch(ac-optind) {
	    case 0: break;
	    default: usage(av[0]); exit(EUSE);
	}
	if((lfp=fopen(lfile, "r")) == FNULL) {
	    fprintf(stderr, "%s: Error reading file %s. Exiting\n",
		    av[0], lfile);
	    exit(EFILE);
	}
	for(i=0; i < MAXF; ++i) files[i]=NULL;
	for(i=0; i < MAXF; ++i) {
	    if((fscanf(lfp, "%s ", tmpstr)) == EOF) break;
	    tmpstr[MAXSTR-1] = '\0'; /* get rid of \n */
	    files[i]= (char *)malloc(sizeof(char) *(strlen(tmpstr)+1));
	    if(files[i] == NULL) {
		fprintf(stderr,
			"Couldn't get mem for file name. Exiting.\n");
		exit(EMEM);
	    }
	    strcpy(files[i], tmpstr);
	}
	filecnt=i;
	if(filecnt < 1) {
	    fprintf(stderr, "Not enough file names in %s.  Exiting\n",
			    lfile);
	    exit(EINPUT);
	}
    } else if(ac-optind < 1) {
	usage(av[0]);
	exit(EUSE);
    } else { /* build list from av */
	for(i=0; i < MAXF; ++i) files[i]=NULL;
	for(i=0; i < ac - optind; ++i) {
	    files[i]= (char *)malloc(sizeof(char) * (strlen(av[optind+i])+1));
	    if(files[i] == NULL) {
		fprintf(stderr,
		    "Couldn't get mem for file name. Exiting.\n");
		exit(EMEM);
	    }
	    strcpy(files[i], av[optind+i]);
	}
	filecnt=i;
	if(filecnt < 1) {
	    fprintf(stderr, "Not enough file names in %s.  Exiting\n",
		lfile);
	    exit(EINPUT);
	}
    }

    makecache(2);
    for(i=0; i < filecnt; ++i) {
	map=getmap(files[i]);
	if(map == CMNULL) {
	    fprintf(stderr, "problem getting %dth map...continuing\n", i+1);
	    continue;
	}
	writemap(stdout, map);
	freemap(map, RELEASE);
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
usage(char *program)
{
    fprintf(stderr, "%s: A usage message will go here someday\n", program);
}
