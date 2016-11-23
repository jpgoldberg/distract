/*
 * This file, avmap.c, is part of the distrat/askmap package
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

/* Calculate the "average" of a number of maps
(see Marko1cy and Goldberg (1992)) */

#include "distrat.h"
#include <math.h>


/**
 *  Unless someone has told us otherwise, assume we don't have
 *  double rint(double) which appears in some math libs
 **/

#define myrint(x)  floor((x) + 0.49999)

#if !defined(DEF_PC)
#  define DEF_PC   50
#endif

#if !defined(DEF_N)
#  define DEF_N  "Average-map"
#endif

int debug = DEF_DB;		/* debug level */
int Maxc = MAXC;
int Maxf = MAXF;
struct cm * (*readmap)(FILE *);	/* ptr to func which reads maps */

/* from avmap.c  main file only used locally */
static void makeaver(struct cm *avr, struct cm *count);
static void addtomaps(struct cm *new, struct cm *avr, struct cm *sum);
static void addtonodes(int *nodelist, struct cm *map);
static struct cm *buildsummap(int *nodelist, int cutoff, char *name);
static struct cm *mapdup(struct cm *frommap);
static void usage(char *s);

int
main(int ac, char **av)
{
    struct cm *curr_map, *avr_map, *sum_map;
    extern struct cm * (*readmap)(FILE *infilep);
    FILE  *lfp;			/* input file pointer */
    extern int debug;		/* debug level */
    char lfile[MAXSTR], tmpstr[MAXSTR], *avname, **files;
    int i, opt, filecnt=0;
    int pcutoff =DEF_PC;	/* percent cutoff */
    int cutoff;			/* actual cutoff */
    int *allnodes;

    lfile[0]='\0';  /* this shouldn't be needed but it is for some
                       compilers */

    debug=DEF_DB;
    avname = mystrdup(DEF_N);
    readmap=rmap1;
    
    fprintf(stderr,
	"Average map %s. (C) 1992-95 by Jeff Goldberg\n",
	version);
    fprintf(stderr, "Use -L option for license information\n");
    /* parse command line with getopt */


    while((opt= getopt (ac, av, "n:vd:hf:p:L")) != EOF) 
	switch(opt) {
	    case 'd': debug = atoi(optarg); break;
	    case 'p': pcutoff = atoi(optarg); break;
	    case 'f': strcpy(lfile, optarg); break;
	    case 'n': free(avname);
		      avname=mystrdup(optarg); break;
	    case 'v': if(debug < 2) debug=2; break;
	    case 'h': usage(av[0]); exit(0);
	    case 'L': License(stdout); exit(0);
	    case '?':
	    default: usage(av[0]);  exit(EUSE);
	}

    
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
	files = (char **)malloc(sizeof(char *)*Maxf);
	if (files == (char **)NULL) {
	    fprintf(stderr,
		"Couldn't get mem for file names.  Exiting.\n");
	    exit(EMEM);
	}		
	for(i=0; i < Maxf; ++i) files[i]=NULL;
	for(i=0; i < Maxf; ++i) {
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
	files=(char **)malloc(sizeof(char *)*(1+ac-optind));
	if(files == (char **)NULL) {
	    fprintf(stderr,
		"Couldn't get mem for file names\n");
	    exit(EMEM);
	}
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

    if(debug > 4) {
	fprintf(stderr, "Read in all %d file names\n", filecnt);
    }
    if (debug > 5) {
	for(i=0; i < filecnt; ++i)
	    fprintf(stderr, "%3d: %s\n", i+1, files[i]);
    }
    
    allnodes=ivector(1, Maxc);
    if (allnodes == (int *)NULL) {
	fprintf(stderr, "Couldn't get mem for node array. Exiting.\n");
	exit(EMEM);
    }
    for(i=1; i <= Maxc; ++i)
	 allnodes[i]=0;

    makecache((unsigned)MIN(filecnt, 50));
    if(debug > 6) {
	fprintf(stderr, "allocated map cache size of %d\n",
	    MIN(filecnt, 50));
    }

    /* first pass through maps */
    for(i=0; i < filecnt; ++i) {
	curr_map = getmap(files[i]);
	if(curr_map == CMNULL) {
	    fprintf(stderr, "avmap: Couldn't get map. Exiting\n");
	    exit(EINPUT);
	}
	addtonodes(allnodes, curr_map);
	freemap(curr_map, SAVE);
    }
    if(debug > 5) {
	fprintf(stderr,  "Completed first pass\n");
    }
    
    cutoff = (filecnt*pcutoff)/100 +1;
    if(debug > 5) {
	fprintf(stderr, "cutoff == %d nodes\n", cutoff);
    }
    
    if((avr_map=buildsummap(allnodes, cutoff, avname)) == CMNULL) {
	fprintf(stderr, "Error building avr map.  Exiting\n");
	exit(EMEM);
    }
    if(debug > 6) {
	fprintf(stderr, "constructed avr_map ");
	fprintf(stderr, "named (%s) with size %d\n",
	     avr_map->name, avr_map->size);
    }
    
    /*	if map has no nodes then we know the average ahead of time  *\
    **  Note that we still do the calculation if the average map    **
    **  will just have 1 node, since people MAY have nonzeros	    **
    \*  in the diagonal						    */

    if(avr_map->size < 2) {
	wmap2(stdout, avr_map);
	fprintf(stderr,
	    "avmap warning: resulting map is too small: ");
	fprintf(stderr,
	    "contains only %s\n", avr_map->size ? "1 node" : "0 nodes");
	exit(0);
    }
    
    if((sum_map = mapdup(avr_map)) == CMNULL) {
	fprintf(stderr, "Error building counting map\n");
	exit(EMEM);
    }
    if(debug > 6) {
	fprintf(stderr, "constructed sum_map ");
	fprintf(stderr, "named (%s) with size %d\n",
	     sum_map->name, sum_map->size);
    }

    /* second pass */
    for(i=0; i < filecnt; ++i) {
	curr_map = getmap(files[i]);
	if(curr_map == CMNULL) {
	    fprintf(stderr, "avmap: Couldn't get map. Exiting\n");
	    exit(EINPUT);
	}
	addtomaps(curr_map, avr_map, sum_map);
	freemap(curr_map, RELEASE);
    }

    if(debug > 5) {
	fprintf(stderr, "completed second pass\n");
    }

    if(debug > 7) {
	fprintf(stderr, "avemap before division\n");
	wmap2(stderr, avr_map);
    }
    if(debug > 7) {
	fprintf(stderr, "counting map\n");
	wmap1(stderr, sum_map);
    }

    makeaver(avr_map, sum_map);

    if(debug > 6) {
	fprintf(stderr, "have calculated average\n");
    }
    wmap2(stdout, avr_map);
    exit(0);
#if defined (__TURBOC__)
#   pragma warn -rvl
#endif
}
#if defined (__TURBOC__)
#   pragma warn .rvl
#endif

static void
makeaver(struct cm *avr, struct cm *cnt)
{
    int i, j;

    for(i=1; i <= avr->size; ++i) {
	for(j=1; j <= avr->size; ++j) {
	    if (debug > 1) {
		if(myrint(cnt->table[i][j]) != cnt->table[i][j]) {
		    fprintf(stderr,
			"Strange error: cnt->table[%d][%d] = %e\n",
			i, j, cnt->table[i][j]);
		}
	    } 
	    if(myrint(cnt->table[i][j]) != 0.0)
		avr->table[i][j] /= myrint(cnt->table[i][j]);
	}
    }
}

static void
addtomaps(struct cm *new, struct cm *avr, struct cm *cnt)
{
    int i, j;
    int * nindex;
    static int *avindex=(int *)NULL;

    if (debug > 6) {
	fprintf(stderr, "In addtomaps(): line %d:\n", __LINE__);
	fprintf(stderr, "\tWill add %s to %s if possible\n",
	    new->name, avr->name);
    }
    
/* The index stuff can be a bit hairy.  Since the nodes vector
   is a packed array, we need both a quick way to find out whether
   a not is in a map and if so, what what column, row corresponds
   to it in the table and what position it will be in the nodes
   array.  (these are all the same).  So, we create an vector
   called index which we use for this look-up,  If the node
   5 appears in the map (lets say 5 == nodes[2]) then index[5]=2;
   this way, if we want to get the value of node 22's (lets say
   nodes[6]==22) influence on node 5 then we would use
   table[index[5]][index[22]].  Of course, these things like 5 and
   22 may come from the packed node list of another map, so we
   might have m1->table[index[m2->nodes[i]]][index[m2->nodes[j]]]
   to get get the value in m1 of the influence of the ith in m2 node
   on the jth in m2 node.  So if i is 1 and j 3 and m2's first
   node is 5 and m2's third node is 22, then continuing with the
   example we will get m1->table[index[5]][index[22]] which will
   be m1->table[2][6].

   Checking existance in a map is just seeing if these are zero
   or not.  So to see if the 3 node of m2 exists in m1 just
   see if index[m2->nodes[3]] is zero or not.

   Ok, hope that that is clear.  We need an index for the average
   map, which only needs to be calculated once, so it is a static.
   it will be avindex;  we also need an index for the map we are
   adding in, that is nindex.   */

   
    if(avindex==(int *)NULL) {
	avindex = ivector(avr->min, avr->max);
	if(avindex == (int *) NULL) {
	    fprintf(stderr, "Couldn't get mem for avindex\n");
	    exit(EMEM);
	}
	/* zero it */
	for(i=avr->min; i <= avr->max; ++i)
	    avindex[i]=0;
	
	for(i=1; i <= avr->size; ++i)
	    avindex[avr->nodes[i]] = i;
    }

    nindex = ivector(MIN(new->min, avr->min), MAX(new->max, avr->max));
    if(nindex == (int *) NULL) {
	fprintf(stderr, "Couldn't get mem for nindex\n");
	exit(EMEM);
    }
    /* zero it */
    for(i=MIN(new->min,avr->min); i <= MAX(new->max, avr->max); ++i)
	nindex[i]=0;
    for(i=1; i <= new->size; ++i)
	nindex[new->nodes[i]] = i;

    /** here is where the actual adding goes on **/
    for(i=1; i <= avr->size; ++i)
	for(j=1; j <= avr->size; ++j) {
	    if(nindex[avr->nodes[i]] && nindex[avr->nodes[j]]) {
		cnt->table[i][j] += 1.0;
		avr->table[i][j] +=
		    new->table[nindex[avr->nodes[i]]][nindex[avr->nodes[j]]];
		if(debug >1) {
		    if (fabs(avr->table[i][j]) > 40000.0) {
			fprintf(stderr, "Odd, avr->cell[%d][%d] is large %f",
			    i, j, avr->table[i][j]);
			fprintf(stderr, " after added %s\n", new->name);
			fprintf(stderr, "avr->nodes[%d] is %d\n",
			    i, avr->nodes[i]);
			fprintf(stderr, "avr->nodes[%d] is %d\n",
			    j, avr->nodes[j]);
			fprintf(stderr,
			    "nindex[%d] is %d\n",
			    avr->nodes[i], nindex[avr->nodes[i]]);
			fprintf(stderr,
			    "nindex[%d] is %d\n",
			    avr->nodes[j], nindex[avr->nodes[j]]);
			fprintf(stderr,
			    "so the cell in add map has %f\n",
		new->table[nindex[avr->nodes[i]]][nindex[avr->nodes[j]]]);

		    }
		}
	    }
	}
    if (debug > 6) {
	fprintf(stderr, "In addtomaps(): line %d:\n", __LINE__);
	fprintf(stderr, "\tHave added %s to %s hopefully\n",
	    new->name, avr->name);
    }
    free_ivector(nindex, new->min, new->max);
}
		
	
static void
addtonodes(int *nodesum, struct cm *map)
{
    int i;
    for(i=1; i <=map->size; ++i)
	 ++nodesum[map->nodes[i]];
}

static struct cm *
buildsummap(int *nodelist, int cutoff, char *name)
{
    int i, j, size=0;
    struct cm *map=CMNULL;

    if ((map = (struct cm *) malloc(sizeof(struct cm))) == CMNULL) 
	return(CMNULL);

    map->name=name;
    map->min = 0;
    
    for(i=1; i <= Maxc; ++i)
	if(nodelist[i] >= cutoff) {
	    ++size;
	    if(map->min == 0) map->min=i;
	    map->max=i;
	}

    map->size = size;
    if(map->size > 0) {
	if((map->nodes=ivector(1, map->size)) == (int *)NULL)
	    return (CMNULL);
	/* zero it */
	for(i=1; i <= map->size; ++i)
	    map->nodes[i]=0;
	
	if((map->table=matrix(1, map->size, 1, map->size)) == (float **)NULL)
	    return (CMNULL);
	
	/* make nodes */
	j=0;
	for(i=1; i <= Maxc ;++i) {
	    if(nodelist[i] >= cutoff) {
		map->nodes[++j]=i;
	    }
	}
	if(j != map->size) {
	    fprintf(stderr, "Impossible error in buildsummap():\n\tnumber");
	    fprintf(stderr, " of nodes %d should equal %d\n", map->size, j);
	    exit(ECANT);
	}
	/* zero table */
    } else { /* map of zero size */
	map->nodes = (int *)NULL;
	map->table = (float **)NULL;
	map->min=0;
	map->max=0;
    }
    for(i=1; i <= map->size; ++i)
	for(j=1; j <= map->size; ++j)
	    map->table[i][j]=0.0;
	
    return(map);
}

static struct cm *
mapdup(struct cm *from)
{
    int i,j;
    struct cm * map;
    
    if ((map = (struct cm *) malloc(sizeof(struct cm))) == CMNULL) 
	return(CMNULL);

    map->name=mystrdup(from->name);
    map->size=from->size;
    map->min=from->min;
    map->max=from->max;
    
    if((map->nodes=ivector(1, map->size)) == (int *)NULL)
	return (CMNULL);

    if((map->table=matrix(1, map->size, 1, map->size)) == (float **)NULL)
	return (CMNULL);

    for(i=1; i <= map->size; ++i) {
	map->nodes[i]=from->nodes[i];
	for(j=1; j <= map->size; ++j)
	    map->table[i][j]=from->table[i][j];
    }
    return(map);
}    

static void
usage(char *s)
{
    fprintf(stderr, "Usage: %s [options] file [files]\n", s);
    fprintf(stderr, "   or  %s [options] -f listfile\n", s);
    fprintf(stderr, "\tfile and files contain maps\n");
    fprintf(stderr, "\tlistfile contains a list of files\n");
    fprintf(stderr, "\tOptions:\n");
    fprintf(stderr, "\t\t-h\tdisplay this message\n");
    fprintf(stderr, "\t\t-L\tdisplay License infomation\n");
    fprintf(stderr, "\t\t-v\tset verbose flag [off]\n");
    fprintf(stderr, "\t\t-d N\tset debug level flag [%d]\n", DEF_DB);
    fprintf(stderr, "\t\t-p num\tset cutoff percent to num [%d]\n",
				    DEF_PC);
    fprintf(stderr, "\t\t-n name\tset map id to name [%s]\n",DEF_N);
}


