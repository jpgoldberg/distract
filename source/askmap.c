/*
 * This file, askmap.c, is part of the distrat/askmap package
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
#ifdef HAVE_UNISTD_H	/* this is defined set (or not) in the makefile) */
#   include <unistd.h>
#endif
#include "lang.h"
#define UNINFL		-99	/* for undefined influences */
#define TRUE 1
#define FALSE 0
int Maxc=MAXC;
#define DEBUG 3

static struct cm *allocmap(int);
static void usage(void);
static void querymap(struct cm *, int, char **);
static char *itostr(char * str, int n);
static void savestate(struct cm *, int);

int
main(int ac, char **av)
{
    struct cm *map=CMNULL;
    char mfilename[50], tempstr[50], nodelab[5];
    FILE *mfileptr, *recfptr;
    int i,j;
    int size;
    extern int optind;
    extern char *optarg;
    int opt;
    int recover=FALSE;
    int recpair=0;
    extern char *L[];
    char lang[20];
    char **nodenames=(char**) NULL;

    fprintf(stderr, "Askmap %s. (C) Jeff Goldberg 1993--1995\n",
	version);
    fprintf(stderr, "Use -C option for license information\n");

    getlang((char *)NULL);  /* load default language */

    mfileptr = recfptr = (FILE *)NULL;

    while((opt=getopt(ac, av, "hr:L:C"))!= EOF) {
	switch(opt) {
	    case 'h': usage(); exit(0); break;
	    case 'r':
		recover=TRUE;
		recfptr=fopen(optarg, "r");
		if (recfptr==FNULL) {
		    fprintf(stderr, L[L_CORF],
			optarg);
		    exit(EFILE); return(EFILE);
		}
		break;
	    case 'L': strcpy(lang, optarg);
		getlang(lang); break;
	    case 'C': License(stdout); exit(0);
	    case '?':
	    default:  usage(); exit(EUSE); break;
	}
    }
    if(optind != ac) {
	usage();
	exit(EUSE); return(EUSE);
    }
    if(recfptr != FNULL) {
	recpair = -1;
	map = rmap1(recfptr);
	fscanf(recfptr, "\nAM-Recover-from-pair: %d\n", &recpair);
	if (recpair == -1) {
	    fprintf(stderr, L[L_CGLP]);
	    exit(EFILE); return(EFILE);
	}
	fprintf(stdout, L[L_ORQFP], recpair);
	fclose(recfptr);
	/* now get memory for node labels */
	nodenames = (char **) malloc(sizeof(char *) * map->size);
	if(nodenames == (char **)NULL) {
	    fprintf(stderr, L[L_ENNMEMS]);
	    exit(EMEM);
	}
	--nodenames; /* use a 1 offset array */
	for(i=1; i <= map->size; ++i) {
	    /* get nodenames */
	    fprintf(stdout, L[L_NText], map->nodes[i]);
	    gets(tempstr);
	    nodenames[i] = (char *) malloc((strlen(tempstr)+1)*sizeof(char));
	    if(nodenames[i] == (char *)NULL) {
		fprintf(stderr, L[L_ENNMEM]);
		exit(EMEM);
	    }
	    strcpy(nodenames[i], tempstr);
	}
    }
    while(mfileptr == FNULL) {
	fprintf(stdout, L[L_MFN]);
	gets(mfilename);

	if(!access(mfilename, 00)) {
	    fprintf(stdout, L[L_OVERW]);
	    gets(tempstr);
	    if(tolower(tempstr[0]) != L[L_YES][0]) continue;
	}
	mfileptr = fopen(mfilename, "w");
	if (mfileptr == FNULL)
	    fprintf(stdout, L[L_EFO], mfilename);
    }
    if(recover == FALSE) {
	size = 0;
	while(size == 0) {
	    fprintf(stdout, L[L_NoNum]);
	    gets(tempstr);
	    size=atoi(tempstr);

	    if(size < 2) {
		fprintf(stdout, L[L_GT2]);
		size = 0;
	    }
	}
	if ((map=allocmap(size)) == CMNULL) {
	    fprintf(stderr, L[L_CGMM]);
	    exit(EMEM);
	}

	fprintf(stdout, L[L_MID]);
	gets(tempstr);
	map->name = (char *)malloc((strlen(tempstr) +1) * sizeof(char));
	if (map->name == NULL) {
	    fprintf(stderr, L[L_CGMMID]);
	    exit(EMEM);
	}
	strcpy(map->name, tempstr);
	/* now get memory for node labels */
	nodenames = (char **) malloc(sizeof(char *) * size);
	if(nodenames == (char **)NULL) {
	    fprintf(stderr, L[L_ENNMEMS]);
	    exit(EMEM);
	}
	--nodenames; /* use a 1 offset array */
	fprintf(stdout, L[L_INNOD]);
	for(i = 1; i <= size; ++i) {
	    map->nodes[i]=0;
	    nodenames[i] = (char *)NULL;
	    while(map->nodes[i] == 0) {
		if(size < 27) {
		    nodelab[0] = 'A' -1 + i;
		    nodelab[1]='\0';
		}
		else {
		    nodelab[0]=(i%26)? 'A' -1 +(i%26) : 'Z';
		    nodelab[1]='\0';
		    strcat(nodelab, itostr(tempstr, ((i-1)/26) + 1));
		}
		fprintf(stdout, L[L_NODEQ], nodelab);
		gets(tempstr);
		map->nodes[i]=atoi(tempstr);
		if(map->nodes[i] < 1 || map->nodes[i] > MAXC) {
		    fprintf(stdout, L[L_NORNG], map->nodes[i]);
		    map->nodes[i]=0;
		}
		/* loop through looking for duplicates */
		for(j=1; j < i; ++j)
		    if(map->nodes[i] == map->nodes[j]) {
		    fprintf(stdout, L[L_NSEL], map->nodes[i]);
		    map->nodes[i]=0;
		}

	    }
	    /* get nodenames */
	    fprintf(stdout, L[L_NText], map->nodes[i]);
	    gets(tempstr);
	    nodenames[i] = (char *) malloc((strlen(tempstr)+1)*sizeof(char));
	    if(nodenames[i] == (char *)NULL) {
		fprintf(stderr, L[L_ENNMEM]);
		exit(EMEM);
	    }
	    strcpy(nodenames[i], tempstr);
	}
    }
    querymap(map,recpair, nodenames);
    wmap1(mfileptr, map);
    fprintf(stdout, L[L_WMF], mfilename);
    fclose(mfileptr);
    exit(0);
#if defined (__TURBOC__)
#   pragma warn -rvl
#endif
}
#if defined (__TURBOC__)
#   pragma warn .rvl
#endif

static void usage(void)
{
    extern char *L[];

    fprintf(stderr, L[L_UsCMD]);
    fprintf(stderr, L[L_Us_h]);
    fprintf(stderr, L[L_Us_C]);
    fprintf(stderr, L[L_Us_r]);
    fprintf(stderr, L[L_Us_L]);

    return;
}

static void
querymap(
    struct cm *m,
    int start,
    char **nn	/* nodenames */
)
{
    int i;
    char tempstr[30];
    unsigned char **ross, **rossify(int);
    int saved=FALSE;
    extern char *L[];

    ross = rossify(m->size);
    if (ross == (unsigned char **) NULL) {
	fprintf(stderr, L[L_ERoss]);
	exit(EMEM);
    }
    for(i=start; i < (m->size*(m->size-1))/2; ++i) {
	m->table[ross[i][0]][ross[i][1]] = UNINFL;
	while(m->table[ross[i][0]][ross[i][1]] == UNINFL) {
	    fprintf(stdout, L[L_NDONE], i+1,
		    (i*100)/((m->size*(m->size-1))/2));
	    fprintf(stdout, L[L_TINFL],
		nn[ross[i][0]][0] ? nn[ross[i][0]] : L[L_UNKNW],
		nn[ross[i][1]][0] ? nn[ross[i][1]] : L[L_UNKNW]);
	    fprintf(stdout, L[L_INFL], m->nodes[ross[i][0]],
		    m->nodes[ross[i][1]]);
	    gets(tempstr);
	    switch(tempstr[0]) {
		case 'Q':
		case 'q':
		    if(saved == FALSE) {
			fprintf(stdout, L[L_QSAVE]);
			gets(tempstr);
			if (tolower(tempstr[0]) != L[L_NO][0])
			    savestate(m,i);
		    }
		    fprintf(stdout, L[L_QUIT]);
		    exit(0); break;
		case 's':
		case 'S': savestate(m,i); saved=TRUE; break;
		default:
		    if(!(isdigit(tempstr[0]) || tempstr[0] == '-' ||
			      tempstr[0] == '+')) {
			 fprintf(stdout, L[L_NVNUM], tempstr);
		    } else  /* cast "(float) int" may yield warning, but OK */
			m->table[ross[i][0]][ross[i][1]] =
					    (float) atoi(tempstr);
		    break;
	    }
	}
	m->table[ross[i][1]][ross[i][0]] = UNINFL;
	while(m->table[ross[i][1]][ross[i][0]] == UNINFL) {
	    fprintf(stdout, L[L_TINFL],
		nn[ross[i][1]][0] ? nn[ross[i][1]] : L[L_UNKNW],
		nn[ross[i][0]][0] ? nn[ross[i][0]] : L[L_UNKNW]);
	    fprintf(stdout, L[L_INFL],
		m->nodes[ross[i][1]], m->nodes[ross[i][0]]);
	    gets(tempstr);
	    if(!(isdigit(tempstr[0]) || tempstr[0] == '-' ||
		      tempstr[0] == '+')) {
		 fprintf(stdout, L[L_NVNUM], tempstr);
	    } else  /* cast "(float) int" may yield warning, but OK */
		m->table[ross[i][1]][ross[i][0]] =
				    (float) atoi(tempstr);
	}
	puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	saved=FALSE;
    }
    free(ross);

    return;

}

static struct cm *
allocmap(int s)
{
    unsigned int i,j;
    struct cm *map;
    extern char *L[];

    /* get memory for maps */
    if ((map = (struct cm *) malloc(sizeof(struct cm))) == CMNULL) {
	fprintf(stderr, L[L_CGMM]);
	exit(EMEM);
    }

    map->name=(char *)NULL;

    map->size=s;
    if(map->size < 1 || map->size > MAXC) {
	fprintf(stderr, L[L_BCNST], map->size);
	return(CMNULL);
    }
    /* get space for nodes, and then for arcs */
    map->nodes=ivector(1, map->size);
    map->table=matrix(1, map->size, 1, map->size);

    /* put in undefineds for seeing where we are */
    for(i=1; i <= map->size; ++i)
	for(j=1; j <= map->size; ++j)
	    map->table[i][j] = (i==j) ? 0.0 : UNINFL;
    return(map);
}

static void
savestate(struct cm *m, int p)
{
    FILE *fp;
    extern char *L[];

    fprintf(stdout, L[L_SAVE]);

    fp = fopen("askmap.tmp", "w");
    if (fp == (FILE *)NULL) {
	fprintf(stderr, L[L_ESVOP]);
	return;
    }
    wmap1(fp, m);
    fprintf(fp, "AM-Recover-from-pair: %d\n", p);
    fclose(fp);
}

/* integer to string  (but why didn't use sprintf?) */
static char *
itostr(char *a, int n)
{
    int len, negative=0;

    if(n < 0) negative = 1;

    len=(n/10)+1+negative;

    a[len--] = '\0';
    do 
	a[len--] = '0' + (n%10);
    while(n /= 10);
    if (negative) a[0]= '-';

    return(a);
}
