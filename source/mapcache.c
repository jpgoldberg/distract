/*
 * This file, mapcache.c, is part of the distrat/askmap package
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

#include "distrat.h"

#define CENULL  ((struct celem *)NULL)
struct celem **cache;   /* the cache array */
int cachesize;
int opencount = 0;      /* debugging for counting call to fopen */

/* makecache should only be called once, it allocates the cache
   array and each cache element */
void
makecache(unsigned int n)
{
    int i;
    extern struct celem **cache;
    extern int cachesize;

    cache=(struct celem **)malloc(n*sizeof(struct celem *));
    if (cache == (struct celem **)NULL) {
        fprintf(stderr, "Couldn't get mem for map cache\n");
        exit(EMEM);
    }
    cachesize=n;
    --cache;            /* array from 1 to n */
    for(i=1; i <= cachesize; ++i) {
        cache[i]=(struct celem *) malloc(sizeof(struct celem));
        if (cache[i] == CENULL) {
            fprintf(stderr, "Couldn't get mem for cache element\n");
            exit(EMEM);
        }
        cache[i]->map=CMNULL;
        cache[i]->fname=NULL;
        cache[i]->status=EMPTY;
    }
}

/* Changes a maps status in the cache from inuse to something
   reallocable, SAVE means it is expected to be called again,
   RELEASE means its space will be given up at the next chance */
void freemap(struct cm *m, int type)
{
    switch (type) {
        case SAVE:
            m->connect->status=SAVE; break;
        case RELEASE:
            m->connect->status=RELEASE; break;
        default:
            fprintf(stderr, "Warning unknown type %d in freemap().",
                            type);
            fprintf(stderr, "   Using RELEASE\n");
            m->connect->status=RELEASE; break;
        }
}

/*  getmap() is the heart of the cache system.  Given pointer to
    a file name (containing a map) it sees whether that map has
    been read and is in the cache.  If it is in the cache that map
    will be returned and its status changed to INUSE.  If the
    map is not in the cache already it will be read from the
    disk (with readmap) and cache space will be given to it
    according to the following rules:

    if readmap returns null, then it is upto getmap
    will do the same.  That is, it is up to the calling function
    to make sure that a CMNULL hasn't been returned to it.

    If there are elements marked RELEASE, then the first
    such element will hold the new map.

    If nothing is marked RELEASE, but there is an element 
    marked EMPTY, then the first EMPTY will space will be used.

    If nothing is RELEASE or EMPTY then the first element marked
    SAVE will be replaced with the the new map, which will be
    marked INUSE.  It the the responsibility of the calling
    routines to make sure that there are at least as many elements
    in the cache as there will be simultaneous INUSE maps, and
    that freemap is properly called to take things out of use.

    The cache is a First In Last Out semi-stack, which is
    nearly best for the way the main loop goes through files.
    It is important, however, to free some maps with RELEASE.

    Also note that this routine does not compare file
    names but pointers to file names.  This is not very
    safe, but it much faster than strcmp() */

struct cm *
getmap(char *fname)
{
    extern struct cm * (*readmap)(FILE *);
    struct celem *tmpelem;
    int best, obest, i;
    FILE *fp;
    void emptymap(struct cm *m);

    best = obest= 0;
    for (i=1; i<= cachesize; ++i) {
        if( cache[i]->status==EMPTY)  {
            if(best) {
                if (cache[best]->status != RELEASE) 
                    best=i;
            }
            best=i;
            break;
        }
        /* note dirty trick!! see if pointers to file names are same */
        else if (fname  != cache[i]->fname) {
            switch((best) ? cache[best]->status: 0) {
                case 0:
                    if (cache[i]->status != INUSE)
                        best=i;
                    if (!obest) obest=i;
                    break;
                case SAVE:
                    if (cache[i]->status == RELEASE)
                        best=i;
                    if (!obest) obest=i;
                    break;
                case RELEASE: break;
                case EMPTY:
                case INUSE:
                default:
                    fprintf(stderr, "Can't happen at %d in %s\n",
                        __LINE__, __FILE__);
                    exit(ECANT);
                    break;
            }
        }
        else {
            cache[i]->status=INUSE;
            /* exchance cache[obest] and cache[i] */
            if(obest != 0 && obest != i) {
                tmpelem=cache[i];
                cache[i]=cache[obest];
                cache[obest]=tmpelem;
                return(cache[obest]->map);
            } else
                return(cache[i]->map);
        }
    }
    if (cache[best] == CENULL) {
        fprintf(stderr, "Shouldn't happen at %d in %s\n",
            __LINE__, __FILE__);
        exit(ECANT);
    }
    if (cache[best]->status != EMPTY) {
        emptymap(cache[best]->map);
    }

    ++opencount;                /* profiling */
    fp=fopen(fname, "r");
    if (fp == FNULL) {
        fprintf(stderr, "Can't open map file %s\n", fname);
        return(CMNULL);
    }
    cache[best]->map = (*readmap)(fp);
    fclose(fp);
    if(cache[best]->map == CMNULL) {
        cache[best]->status = EMPTY;
        cache[best]->map = CMNULL;
        cache[best]->fname=fname;
        return(CMNULL);
    }
    cache[best]->map->connect=cache[best];
    cache[best]->fname=fname;
    cache[best]->status=INUSE;
    /* exchange cache[best] with cache[obest] */
    if(obest != 0 && obest != best) {
        tmpelem=cache[best];
        cache[best]=cache[obest];
        cache[obest]=tmpelem;
        return(cache[obest]->map);
    } else
        return(cache[best]->map);
}


/* This function, emptymap(struct cm *), is the one that really
 * frees the map as far as alloc is concerned, and should not be
 * confused with freemap() which should be renamed releasemap() */
void
emptymap(struct cm *m)
{
    free_matrix(m->table, 1, m->size, 1, m->size);
    free_ivector(m->nodes, 1, m->size);
    free(m->name);
    free((char *)m);
}
