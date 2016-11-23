/*
 * This file, iodeg.c, is part of the distrat/askmap package
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

int Maxc=MAXC;		/* max constructs */
int Maxf=MAXF;		/* max map files */
int debug=DEF_DB;	/* debug level */
struct cm * (*readmap)(FILE *);	/* ptr to func which reads maps */

static void usage(char *);
static void outdeg(struct cm *);
static void indeg(struct cm *);

int
main(int ac, char **av)
{
    struct cm *map_A=CMNULL;	/* the maps */
    /* map reading functions (decl in distrat.h) */
    FILE *fp, *lfp, *mfp;			/* input file pointer */
    extern int optind;
    extern char *optarg;
    int opt;
    int alpha;		/* parameters (alpha...delta, wratio) */
    extern int debug;		/* debug level */
    char lfile[MAXSTR];
    char tmpstr[MAXSTR];
    int i;
    char **files = (char **)NULL;
    int filecnt=0;

    /*  These defaults, except for readmap, are specified
	in distrat.h; so (except for readmap) these should
	not be changed for modifying defaults */

    /* set L&W defaults for alpha, beta, and gamma */
    alpha=DEF_A;

    /* other defaults */
    readmap=rmap1;

    fprintf(stderr, "CM IOdeg %s. (C) 1994-95 by Jeff Goldberg\n",
	version);
    fprintf(stderr, "Use -L option for license information\n");

    /* parse command line with getopt */
    lfile[0]='\0';
    while((opt= getopt(ac, av, "A:n:t:m:vd:hf:L")) != EOF)
	switch(opt) {
	    case 'A':
		alpha = atoi(optarg);
		if ((alpha == 0 && optarg[0] != '0') ||
		  (alpha != 0 && alpha != 1) ) {
		    fprintf(stderr, "alpha must be 1 or 0.  ");
		    fprintf(stderr, "Proceeding with %d\n", DEF_A);
		    alpha=DEF_A;
		} break;
	    case 'd': debug = atoi(optarg); break;
	    case 'n': 	/* number of possible nodes */
		Maxc = atoi(optarg);
		if  (Maxc < 1) {
		    fprintf(stderr, "Max nodes must be positive ");
		    fprintf(stderr, "integer.  Proceeding with %d\n",
					MAXC);
		    Maxc=MAXC;
		} break;
	    case 'm': 	/* number of possible maps */
		Maxf = atoi(optarg);
		if  (Maxc < 2) {
		    fprintf(stderr, "Max maps must be > 2.");
		    fprintf(stderr, "  Proceeding with %d\n",
					MAXF);
		    Maxf=MAXF;
		} break;
	    case 't':  /* which map reader */
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
			    "type (-t) must be between 1 and 9\n");
			fprintf(stderr, "Proceeding with type 1\n");
			break;
		}
		break;
	    case 'f': strcpy(lfile, optarg); break;
	    case 'v': if(debug < 2) debug=2; break;
	    case 'L': License(stdout); exit(0);
	    case 'h': usage(av[0]); exit(0);
	    case '?':
		usage(av[0]);  exit(EUSE);
	}
    /* check non-option arguments */
    if(lfile[0]) {
	switch(ac-optind) {
	    case 0: break;
	    default: fprintf(stderr, "Too many arguments.\n");
	    usage(av[0]); exit(EUSE);
	}
	if((lfp=fopen(lfile, "r")) == FNULL) {
	    fprintf(stderr, "%s: Error reading file %s. Exiting\n",
		    av[0], lfile);
	    exit(EFILE);
	}
	/* get memory for files[0...Maxf-1]  */
	files= (char **) malloc((unsigned) Maxf * sizeof(char *));
	if (files == (char **) NULL) {
	    fprintf(stderr, "Couldn't get mem for file name array.\n");
	    exit(EMEM);
	}
	for(i=0; i < Maxf; ++i) {
	    if(fscanf(lfp, "%s ", tmpstr) == EOF) break;
	    tmpstr[MAXSTR-1] = '\0'; /* make sure it is terminated */
	    files[i]= malloc(sizeof(char) *strlen(tmpstr));
	    if(files[i] == NULL) {
		fprintf(stderr,
			"Couldn't get mem for file name. Exiting.\n");
		exit(EMEM);
	    }
	    strcpy(files[i], tmpstr);
	}
	filecnt=i;
    } else {
	switch(ac-optind) {
	    case 1: break;
	    default:
		fprintf(stderr, "Wrong number (%d) of arguments.\n",
							    ac-optind);
		usage(av[0]); exit(EUSE);
	}
    }

    if(lfile[0]) {
	for(i=0; i < filecnt; ++i) {
	    /* read first file */
	    mfp=fopen(files[i], "r");
	    if (mfp == FNULL) {
		fprintf(stderr, "Couldn't read map %s\n", files[i]);
		continue;
	    }
	    map_A=readmap(mfp);
	    fclose(mfp);
	    if (map_A==CMNULL) continue;
	    indeg(map_A);
	    outdeg(map_A);
	    emptymap(map_A);
	}
    } else {	/* just one map  on command line */
	/* open first file */
	if ((fp = fopen(av[optind], "r")) == FNULL) {
	    fprintf(stderr, "%s: Error reading file %s. Exiting\n",
		    av[0], av[optind]);
	    exit(EFILE);
	}

	/* read first file */
	map_A =(*readmap)(fp);
	if(map_A == CMNULL) {
	    fprintf(stderr, "Problem reading map\n");
	    exit(EINPUT);
	}
	fclose(fp);  /* close file */

	indeg(map_A);
	outdeg(map_A);
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
usage(char *s)
{
    fprintf(stderr, "Usage: %s [options] file\n", s);
    fprintf(stderr, "   or  %s [options] -f listfile\n", s);
    fprintf(stderr, "\tfiles contains a map\n");
    fprintf(stderr, "\tlistfile contains a list of files\n");
    fprintf(stderr, "\tOptions:\n");
    fprintf(stderr, "\t\t-h\tdisplay this message\n");
    fprintf(stderr, "\t\t-L\tdisplay license information\n");
    fprintf(stderr, "\t\t-v\tset verbose flag [off]\n");
    fprintf(stderr, "\t\t-d num\tset debug flag to num [%d]\n",DEF_DB);
    fprintf(stderr, "\t\t-n num\tset max. distinct nodes [%d]\n",MAXC);
    fprintf(stderr, "\t\t-m num\tset max. maps in list [%d]\n",MAXF);
    fprintf(stderr, "\t\t-t num\tset type of map file [1]\n");
    fprintf(stderr, "\t\t-A num\tset alpha to num [%d]\n",DEF_A);
}

/* careful not to use things like ABS(--i) !!! */
#define ABS(x)    (((x) < 0) ? -(x) : (x))

static void
outdeg(struct cm *m1)
{
    unsigned char i, j;
    extern int debug;
    float sum;
    int deg;

    printf("Out-degrees for map %s\n", m1->name);
    for(i=1; i <= m1->size; ++i) {
	sum=0.0; deg=0;
	for(j=1; j <= m1->size; ++j) {
	    if(m1->table[i][j] != 0.0) ++deg;
	    sum += ABS(m1->table[i][j]);
	}
	printf("\t\t%5d: %5d  (%+6.3f)\n", m1->nodes[i], deg, sum);
    }
    if (debug > 2) {
	fprintf(stderr, "Map %s: ", m1->name);
    }
      return;
}

static void
indeg(struct cm *m1)
{
    unsigned char i, j;
    extern int debug;
    float sum;
    int deg;

    printf("In-degrees for map %s:\n", m1->name);

    for(i=1; i <= m1->size; ++i) {
	sum=0.0; deg=0;
	for(j=1; j <= m1->size; ++j) {
	    if(m1->table[j][i] != 0.0) ++deg;
	    sum += ABS(m1->table[j][i]);
	}
	printf("\t\t%5d: %5d  (%+6.3f)\n", m1->nodes[i], deg, sum);
    }
    if (debug > 2) {
	fprintf(stderr, "Map %s: ", m1->name);
    }
      return;
}
