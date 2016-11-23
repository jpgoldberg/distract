/*
 * This file, distrat.c, is part of the distrat/askmap package
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

#include <math.h>
#include "distrat.h"

int Maxc;               /* max constructs */
int Maxf;               /* max map files */
int Maxcache;
int debug;              /* debug level */
struct cm * (*readmap)(FILE *);       /* ptr to func which reads maps */

static void usage(char *);
static float dist_ratio(struct cm *m1, struct cm *m2, int *params);

int
main(int ac, char **av)
{
    struct cm *map_A=CMNULL, *map_B = CMNULL;   /* the maps */
    /* map reading functions (in distrat.h) */
    FILE *fp, *lfp;                     /* input file pointer */
    extern int optind;
    extern char *optarg;
    int opt;
    int params[7];              /* parameters (alpha...delta, wratio) */
    extern int debug;           /* debug level */
    extern int opencount;       /* check caching savings */
    char lfile[MAXSTR];
    char tmpstr[MAXSTR];
    int i,j;
    char **files = (char **)NULL;
    int filecnt=0;

    /*  These defaults, except for readmap, are specified
        in distrat.h; so (except for readmap) these should
        not be changed for modifying defaults */

    /* set L&W defaults for alpha, beta, and gamma */
    params[0]=DEF_A; params[1]=DEF_B; params[2]=DEF_G; params[3]=DEF_D;
    params[4]=DEF_E, params[5]=DEF_R1, params[6]=DEF_R2;
    /* other defaults */
    debug=DEF_DB;
    Maxc=MAXC;  Maxcache=MAXCACHE; Maxf=MAXF;
    readmap=rmap1;

    fprintf(stderr,
        "CM Distance Ratio %s. (C) 1992--1995 by Jeff Goldberg\n",
	version);
    fprintf(stderr, "Use -L option for license information\n");
    /* parse command line with getopt */
    lfile[0]='\0';
    while((opt= getopt(ac, av, "A:B:G:D:E:R:n:t:m:vd:hf:c:L")) != EOF) 
        switch(opt) {
            case 'A':
                params[0] = atoi(optarg);
                if ((params[0] == 0 && optarg[0] != '0') ||
                  (params[0] != 0 && params[0] != 1) ) {
                    fprintf(stderr, "alpha must be 1 or 0.  ");
                    fprintf(stderr, "Proceeding with %d\n", DEF_A);
                    params[0]=DEF_A;
                }
                break;
            case 'B': 
                params[1] = atoi(optarg);
                if ( params[1] < 1 ) {
                    fprintf(stderr, "beta must > 0.  ");
                    fprintf(stderr, "Proceeding with %d\n", DEF_B);
                    params[1]=DEF_B;
                }
                break;
            case 'G':
                params[2] = atoi(optarg);
                if (params[2] == 0 && optarg[0] != '0') {
                    fprintf(stderr, "gamma must be integer.  ");
                    fprintf(stderr, "Proceeding with %d\n", DEF_G);
                    params[2]=DEF_G;
                }
                break;
            case 'D':
                params[3] = atoi(optarg);
                if ((params[3] == 0 && optarg[0] != '0') ||
                  (params[3] < 0 || params[3] >  2) ) {
                    fprintf(stderr, "delta must be 0, 1 or 2.  ");
                    fprintf(stderr, "Proceeding with %d\n", DEF_D);
                    params[3]=DEF_D;
                }
                break;
            case 'E':
                params[4] = atoi(optarg);
                if ((params[4] == 0 && optarg[0] != '0') ||
                  (params[4] < 1 || params[4] >  2) ) {
                    fprintf(stderr, "epsilon must be 1 or 2.  ");
                    fprintf(stderr, "Proceeding with %d\n", DEF_E);
                    params[4]=DEF_E;
                }
                break;
            case 'R':
                sscanf(optarg, "%d:%d", params+5, params+6);
                if (params[5] < 0 || params[6] < 0) {
                    fprintf(stderr,
                        "R is ratio between two positive integers");
                    fprintf(stderr, "Proceeding with %d\n", DEF_E);
                    params[4]=DEF_E;
                }
                break;
            case 'd': debug = atoi(optarg); break;
            case 'n':   /* number of possible nodes */
                Maxc = atoi(optarg);
                if  (Maxc < 1) {
                    fprintf(stderr, "Max nodes must be positive ");
                    fprintf(stderr, "integer.  Proceeding with %d\n",
                                        MAXC);
                    Maxc=MAXC;
                }
                break;
            case 'm':   /* number of possible maps */
                Maxf = atoi(optarg);
                if  (Maxc < 2) {
                    fprintf(stderr, "Max maps must be > 2.");
                    fprintf(stderr, "  Proceeding with %d\n",
                                        MAXF);
                    Maxf=MAXF;
                }
                break;
            case 'c':   /* map cache size */
                Maxcache = atoi(optarg);
                if  (Maxc < 2) {
                    fprintf(stderr, "Max cache must be greater");
                    fprintf(stderr, "than 2.  Proceeding with %d\n",
                                        MAXCACHE);
                    Maxcache=MAXCACHE;
                }
                break;
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
            case 'h': usage(av[0]); exit(0);
	    case 'L': License(stdout); exit(0);
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
/*      for(i=0; i < Maxf; ++i) files[i]=NULL; */
        for(i=0; i < Maxf; ++i) {
            if(fscanf(lfp, "%s ", tmpstr) == EOF) break;
            tmpstr[MAXSTR-1] = '\0'; /* make sure it is terminated */
            files[i]= (char *)malloc(sizeof(char) *strlen(tmpstr));
            if(files[i] == NULL) {
                fprintf(stderr,
                        "Couldn't get mem for file name. Exiting.\n");
                exit(EMEM);
            }
            strcpy(files[i], tmpstr);
        }
        filecnt=i;
        if(filecnt < 2) {
            fprintf(stderr, "Not enough file names in %s.  Exiting\n",
                            lfile);
            exit(EINPUT);
        }
    } else {
        switch(ac-optind) {
            case 2: break;
            default:
                fprintf(stderr, "Wrong number (%d) of arguments.\n",
                                                            ac-optind);
                usage(av[0]); exit(EUSE);
        }
    }

    if(lfile[0]) {
        makecache((unsigned)MIN(filecnt, Maxcache));
        for(i=0; i < filecnt -1; ++i) {
            /* read first file */
            map_A=getmap(files[i]);
            if (map_A==CMNULL) continue;

            for(j=i+1; j < filecnt; ++j) {
                /* read second file */
                map_B = getmap(files[j]);
                if (map_B==CMNULL) continue;
                fprintf(stdout, "DR(%s, %s) = %.4f\n", map_A->name,
                    map_B->name, dist_ratio(map_A, map_B, params));
                freemap(map_B, SAVE);
            }
            freemap(map_A, RELEASE);
        }
        if(debug >= 1) {
            fprintf(stderr, "Total of %d map file openings\n",
                            opencount);
            fprintf(stderr, "out of %d possible\n",
                    (int)((2+filecnt)*((filecnt - 1.0)/2.0)));
        }
    } else {    /* just two maps  on command line */
        /* open first file */
        if ((fp = fopen(av[optind], "r")) == FNULL) {
            fprintf(stderr, "%s: Error reading file %s. Exiting\n",
                    av[0], av[optind]);
            exit(EFILE);
        } 

        /* read first file */
        map_A =(*readmap)(fp);
        if(map_A == CMNULL) {
            fprintf(stderr, "Error getting map, exiting\n");
            exit(EINPUT);
        }
        fclose(fp);  /* close file */

        /* open second file */
        if ((fp = fopen(av[optind+1], "r")) == FNULL) {
            fprintf(stderr, "%s: Error reading file %s. Exiting\n",
                    av[0], av[optind+1]);
            exit(EFILE);
        } 

        /* read second file */
        map_B=(*readmap)(fp);
        if(map_B == CMNULL) {
            fprintf(stderr, "Error getting map.  Exiting\n");
            exit(EINPUT);
        }
        fclose(fp);  /* close file */

        fprintf(stdout, "DR(%s, %s) = %.4f\n", map_A->name, map_B->name,
            dist_ratio(map_A, map_B, params));
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
    fprintf(stderr, "Usage: %s [options] file1 file2\n", s);
    fprintf(stderr, "   or  %s [options] -f listfile\n", s);
    fprintf(stderr, "\tfiles 1 and 2 contain maps\n");
    fprintf(stderr, "\tlistfile contains a list of files\n");
    fprintf(stderr, "\tOptions:\n");
    fprintf(stderr, "\t\t-h\tdisplay this message\n");
    fprintf(stderr, "\t\t-L\tdisplay License information\n");
    fprintf(stderr, "\t\t-v\tset verbose flag [off]\n");
    fprintf(stderr, "\t\t-d num\tset debug flag to num [%d]\n",DEF_DB);
    fprintf(stderr, "\t\t-n num\tset max. distinct nodes [%d]\n",MAXC);
    fprintf(stderr, "\t\t-m num\tset max. maps in list [%d]\n",MAXF);
    fprintf(stderr, "\t\t-c num\tset map cache size [%d]\n",MAXCACHE);
    fprintf(stderr, "\t\t-t num\tset type of map file [1]\n");
    fprintf(stderr, "\t\t-A num\tset alpha to num [%d]\n",DEF_A);
    fprintf(stderr, "\t\t-B num\tset beta to num [%d]\n",DEF_B);
    fprintf(stderr, "\t\t-G num\tset gamma to num [%d]\n",DEF_G);
    fprintf(stderr, "\t\t-D num\tset delta to num [%d]\n",DEF_D);
    fprintf(stderr, "\t\t-E num\tset epsilon to num [%d]\n",DEF_E);
}

/* define preds for testing for nodes in maps */
#define in(n, v)                (v[n])
#define pair_in(i, j, v)        (v[i] && v[j])
#define grid1(m, i, j)  (m->table[Nodes1[i]][Nodes1[j]])
#define grid2(m, i, j)  (m->table[Nodes2[i]][Nodes2[j]])

/* This is the function that does the calculation.  It does not
   closely follow the structure of the definition in M & G.
   Instead it works from a more compact tabular form instead
   of an association matrix.  The two arrays Nodes{1,2} are
   created for quick lookup of arcs in table. */

static float
dist_ratio(
    struct cm *m1,  /* ptr to map 1 */
    struct cm *m2,  /* prt to map 2 */
    int *pars	    /* list of parameters */
)
{
    unsigned char i, j;
    unsigned char p_c, p_1, p_2;
    int *Pc, *P1, *P2;
    char alpha, beta, gamma, delta, epsilon, D_weight, DR_weight;
    unsigned int denom;
    float Diff, DR, nodediff;
    extern int debug;
    int g_prime;
    int max, min;
    int *Nodes1, *Nodes2;

    alpha=pars[0]; beta=pars[1]; gamma=pars[2]; delta=pars[3];
    epsilon=pars[4], D_weight=pars[5], DR_weight=pars[6];
    p_c=p_1=p_2=0;

    g_prime= gamma ? 1 : 0;

    /* the minimum and maximum node nums will be used for loops */
    min=MIN(m1->min, m2->min);
    max=MAX(m1->max, m2->max);

    /* indecies to packed node arrays */
    Nodes1=ivector(min, max);
    Nodes2=ivector(min, max);

    /* packed array of Common nodes */
    Pc=ivector(1, MIN(m1->size, m2->size));
    P1=ivector(1, m1->size);    /* packed array unique to  m1 */
    P2=ivector(1, m2->size);    /* packed array unique to  m2 */

    /* Create index Node lists */
    for(i=min; i <= max; ++i)
        Nodes1[i]=Nodes2[i]=0;
    for(i=1; i<=m1->size; ++i)
        Nodes1[m1->nodes[i]]=i;
    for(i=1; i<=m2->size; ++i)
        Nodes2[m2->nodes[i]]=i;
    /* Calculate p_c, Pc, p_1, and p_2, P1 and P2 */
    for(i=min; i <= max; ++i) {
        if(in(i, Nodes1) && in(i, Nodes2)) {
            Pc[++p_c]=i;
        }
        else if(in(i, Nodes1)) P1[++p_1]=i;
        else if(in(i, Nodes2)) P2[++p_2]=i;
    }

    denom= (epsilon*beta+delta)*p_c*p_c +
        g_prime * (2*p_c*(p_1+p_2) + p_1*p_1 + p_2*p_2) -
        alpha*((epsilon*beta+delta)*p_c + g_prime *(p_1 +p_2));
    
    if(debug >= 2) {
        fprintf(stderr, "Maps %s and %s: ", m1->name, m2->name);
        fprintf(stderr,"p_c = %d, p_1 = %d, p_2 = %d",p_c,p_1,p_2);
        fprintf(stderr, "  Common:");
        for(i=1; i <= p_c; ++i) 
            fprintf(stderr, " %d", Pc[i]);
        putc('\n', stderr);
    }
            
    if(denom == 0) {
        free_ivector(Pc, 1, MIN(m1->size, m2->size));
        free_ivector(P1, 1, m1->size);
        free_ivector(P2, 1, m2->size);
        if(D_weight == 0 || DR_weight == 0)  {
            if(debug >= 2) fprintf(stderr, "%ld/%ld\n", 0L, 0L);
            return(-1.0);
        }
        else {
            return(1.0-
                pow(log(p_c+1.0)/log(p_c+p_1+p_2+1.0),
                1.0/(float)D_weight));
        }
    }
    Diff=0.0;
    /* first loop through common nodes */
    for(i=1; i <= p_c; ++i) {
        for(j=1; j <= p_c; ++j) {
            if (i==j && alpha == 1) ;   /* on diagnol */
            else 
                if((grid1(m1,Pc[i],Pc[j]))*(grid2(m2,Pc[i],Pc[j]))<0.0)
                    Diff += fabs(grid1(m1, Pc[i], Pc[j])
                            - grid2(m2, Pc[i],Pc[j])) + (float)delta;
            else
                    Diff +=fabs(grid1(m1,Pc[i],Pc[j])-grid2(m2,Pc[i],Pc[j]));
        }
    }
    /* now deal with one map arcs */
    switch (gamma) { 
        case 0: break;
        case 1: /* arc only in map one */
            /* for arcs only in map1 */
            for(i=1; i<=p_1; ++i) {
                /* between unique nodes */
                for(j=1; j <= p_1; ++j) {
                    if (i != j || alpha == 0)
                        if(grid1(m1, P1[i], P1[j]) != 0.0)
                            Diff += 1.0;
                }
                /* between unique and common nodes */
                for(j=1; j <= p_c; ++j) {
                    if(grid1(m1, P1[i], Pc[j]) != 0.0)
                            Diff += 1.0;
                    if(grid1(m1, Pc[j], P1[i]) != 0.0)
                            Diff += 1.0;
                }
            }
            /* for arcs only in second map */
            for(i=1; i<=p_2; ++i) {
                /* between unique nodes */
                for(j=1; j <= p_2; ++j) {
                    if (i != j || alpha == 0)
                        if(grid2(m2, P2[i], P2[j]) != 0.0)
                            Diff += 1.0;
                }
                /* between unique and common nodes */
                for(j=1; j <= p_c; ++j) {
                    if(grid2(m2, P2[i], Pc[j]) != 0.0)
                            Diff += 1.0;
                    if(grid2(m2, Pc[j], P2[i]) != 0.0)
                            Diff += 1.0;
                }
            }
            break;
        case 2:
            Diff += (float)((p_1*p_1)+(p_2*p_2) + 2*p_c*(p_1+p_2) -
                    alpha*(p_1+p_2));
            ; break;
        default: 
            fprintf(stderr, "Shouldn't happen (%d)\n",
                            __LINE__);
            exit(ECANT);
            break; /* make lint happy */
    }

    free_ivector(Pc, 1, MIN(m1->size, m2->size));
    free_ivector(P1, 1, m1->size);
    free_ivector(P2, 1, m2->size);
    free_ivector(Nodes1, min, max);
    free_ivector(Nodes2, min, max);
    if (debug > 2) {
        fprintf(stderr, "Maps %s and %s: ", m1->name, m2->name);
        fprintf(stderr, "%f/%d\n", Diff, denom);
    }
    DR = Diff/(float)denom;
    if(gamma==0 && DR_weight*D_weight != 0) { /* with node count */
        nodediff= log(p_c+1.0)/log(p_c+p_1+p_2+1.0);
        return(1-
            pow(pow(nodediff, 1.0/(float)D_weight)*
            pow(1.0-DR, 1.0/(float)DR_weight),
            ((float)D_weight+(float)DR_weight)/2.0));
    }
    return((float)DR);
}
