/*
 * This file, distrat.h, is part of the distrat/askmap package
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

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef DOS
#   include <dos.h>
#endif

#include <stdlib.h>

/* set default number of Constructs, but let it be a compile
 * time option.  So one can compile with cc -DMCONST=45 if the max
 * node number will be <= 45 */

#if !defined(MAXC)
#  define   MAXC    300
#endif

#ifndef MAXCACHE
#  define MAXCACHE      40
#endif

#define MAXSTR          32      /* length of various strings */

#ifndef MAXF
#  define MAXF          200     /* maximum number of comparisons */
#endif

/* Exit states */
#define EUSE            1       /* Usage Error */
#define EFILE           2       /* File opening error */
#define ECANT           3       /* Impossible error */
#define ENODE           4       /* node out of range */
#define EMEM            5       /* memory error */
#define EINPUT          6       /* error with input data */

/* various defaults */

#if !defined(DEF_A)
#   define DEF_A        1       /* default alpha */
#endif
#if !defined(DEF_B)
#   define DEF_B        3       /* default beta */
#endif
#if !defined(DEF_G)
#   define DEF_G        1       /* default gamma */
#endif
#if !defined(DEF_D)
#   define DEF_D        0       /* default delta */
#endif
#if !defined(DEF_E)
#   define DEF_E        2       /* default epsilon */
#endif
#define DEF_R1  0       /* default weighting ratio */
#define DEF_R2  0       /* default weighting ratio */

#if !defined(DEF_DB)
#   define DEF_DB       0       /* default debug (make 0 in prod version) */
#endif


#define MAX(a,b)        ((a < b) ? b : a)
#define MIN(a,b)        ((a > b) ? b : a)

#define CMNULL  ((struct cm *) NULL)
#define FNULL   ((FILE *) NULL)

/* structure of a tabular cm */
struct cm {
    float **table;              /* the table [1][1]..[size][size] */
    int size;                   /* num of nodes in map */
    int min, max;               /* the minimum, maximum node num */
    int *nodes;                 /* list of nodes [1..size] in the true map */
    char *name;                 /* name of map */
    struct celem *connect;      /* pointer to cache element */
};

/* note that celem also has space for a link.  The current caching
   doesn't use a link listed, but other caching systems might */
struct celem {          /* Cache element */
    struct cm   *map;           /* pointer to map */
    char        *fname;         /* file name for look-up */
    int         status;         /* status of cache element */
    struct celem *next;         /* forward link */
};

/* Status of cache element */
#define SAVE    1
#define RELEASE 2
#define EMPTY   3
#define INUSE   4

extern char version[];	/* defined in version.c */

/** prototypes of public functions  **/

/* from amlang.c */
extern int getlang(char *langname);

/* from getopt.c */
extern int getopt(int argc, char *const *argv, const char *optstring);
extern int optind;
extern char *optarg;

/* from matutil.c */
extern float *vector(int low, int high);
extern int *ivector(int low, int high);
extern float **matrix(int rowlow, int rowhigh, int collow, int colhigh);
extern void free_vector(float *vec, int rowlow, int rowhigh);
extern void free_matrix(float **arr, int rlow, int rhigh, int clow, int chigh);
extern void free_ivector(int *vec, int rowlow, int rowhigh);
extern void free_imatrix(int **arr, int rlow, int rhigh, int clow, int chigh);
extern char * mystrdup(const char *str);

/* from mapcache.c */
extern void makecache(unsigned int cachesize);
extern void freemap(struct cm *map, int type);
extern struct cm *getmap(char *filename);
extern void emptymap(struct cm *map);


/* from readmap.c */
extern struct cm *rmap1(FILE *infile);
extern struct cm *rmap2(FILE *infile);
extern struct cm *rmap3(FILE *infile);
extern struct cm *rmap4(FILE *infile);
extern struct cm *rmap5(FILE *infile);
extern struct cm *rmap6(FILE *infile);
extern struct cm *rmap7(FILE *infile);
extern struct cm *rmap8(FILE *infile);
extern struct cm *rmap9(FILE *infile);

/* from rossify.c */
extern unsigned char **rossify(int size);

/* from version.c */
void License(FILE *fp);

/* from writemap.c  */
extern void wmap1(FILE *outfile, struct cm *map);
extern void wmap2(FILE *outfile, struct cm *map);
extern void wmap3(FILE *outfile, struct cm *map);
extern void wmap4(FILE *outfile, struct cm *map);
extern void wmap5(FILE *outfile, struct cm *map);
extern void wmap6(FILE *outfile, struct cm *map);
extern void wmap7(FILE *outfile, struct cm *map);
extern void wmap8(FILE *outfile, struct cm *map);
extern void wmap9(FILE *outfile, struct cm *map);

