/*
 * This file, matutil.c, is part of the distrat/askmap package
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
 * NOTE: This file is partially based on material placed in the
 * Public Domain by Numerical Recipes Software.  The General Public
 * License Agreement here is indended to apply to my implementation
 * and modifications of their work.  If you wish to distribute code
 * based on something similar to this file in a mannar inconsistant
 * with the License Agreement you must first obtain a Public Domain
 * version without Jeff Goldberg's modifications and work from that.
 */

/* Most of these routines are BASED on routines placed in the Public
   Domain by Numerical Recipies Software.  For the best description of
   them, see "Numerical Recipes in C",   (first edtion) 1986,
   Cambridge University Press.  When I, Jeff Goldberg, sought
   permission to distribute this code I was informed in a letter
   from Numerical Recipes Software, ``Upon publication of the Second
   Edition of Numerical Recipes, we placed these memory allocation
   utilities in the public domain, so you may use them freely.  Of
   course, we accept no liability for such use.''

   My modifications to these routines are minimal.  It
   is really just in the nature of error handling and messages.

   The idea in these routines is so simple, it is a surprise
   the C programmers haven't been doing this for ages.  When
   a pointer, P,  to an array (possibly and array of arrays) is returned
   by malloc() one can actually decrement the pointer by some integer N so
   that P[N] points to the first space allocated, and N does NOT
   have to be zero!!  You won't realize how heavy the burden of always
   counting from zero is until you have be relieved of it.  Use these,
   modify these, include these routines in everything you write.

   Basic usage is simple, a function like
       float *vector(int nl, int nh)
   returns a pointer to an array of floats subscriptable from
   nl to nh.  Correspondingly, there is another
       void free_vector(float *p, int nl, int nh)
   which frees a vector allocated this way.  (Note that nh is
   unused in the free_XXXX routines.)

   There is a function
       float ** matrix(int nrl, int nrh, int ncl, int nch)
   which does the same thing in two dimensions.  nrl is the
   lower row subscript, nrh is the higher row subscript.  ncl
   and nch are the same for columns.  No sanity checking
   is performed (ie assert(nrl < nrh && ncl < nch));

   Corresponding functions, ivector (returns int *), and imatrix
   (returns int **) exist and the free_ivector, free_imatrix, etc.

   The book, Numerical Recipes in C, is something I would recommend
   to any C programmer.  Even if you don't do numerical analysis
   the general advice on programming is worth it.  Also, it is
   almost certain that you will need some of the recipes.  I purchased
   the book because I needed two statistical routines.
   It was worthwhile just for those two, but I have since then referred
   to it for many other problems.  I keep it beside K&R and Harbison
   and Steele ("C: A reference manual").  I should also add that
   the clarity, brevity and completeness of the description of
   various statistical operations exceeds what I have seen in most
   statistical textbooks.  My suspicion is that the same is true
   for other chapters as well.   */

/*   I have also added mystrdup to mimic strdup, which doesn't always
 *   exist
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

 float *vector(int nl, int nh)
 {
   float *v;

   v= (float *) malloc((unsigned) (nh-nl+1)*sizeof(float));
   if (v == (float *) NULL) {
     fprintf(stderr, "Couldn't get memory in vector()\n");
     exit(1);
   }
   return(v-nl);
 }

 int *ivector(int nl, int nh)
 {
   int *v;

   v= (int *) malloc((unsigned) (nh-nl+1)*sizeof(int));
   if (v == (int *) NULL) {
     fprintf(stderr, "Couldn't get memory in ivector()\n");
     exit(1);
   }
   return(v-nl);
 }

 float **matrix(int nrl, int nrh, int ncl, int nch)
 {
   int i;
   float **m;

   m= (float **) malloc((unsigned) (nrh-nrl+1)*sizeof(float *));
   if (m == (float **) NULL) {
     fprintf(stderr, "Couldn't get memory in matrix()\n");
     exit(1);
   }
   m -= nrl;
   for(i = nrl; i <=  nrh; ++i) {
     m[i] = (float *) malloc((unsigned) (nch-ncl+1)*sizeof(float));
     if (m[i] == (float *) NULL) {
       fprintf(stderr, "Couldn't get memory in matrix()\n");
       exit(1);
     }
     m[i] -= ncl;
   }
   return(m);
 }

 #if defined (__TURBOC__)
 #  pragma argsused
 #endif

 void
 free_vector(float *v, int nl, int nh)
 {
   free((char *) (v+nl));
 }

 #if defined (__TURBOC__)
 #  pragma argsused
 #endif

 void
 free_matrix(float **m, int nrl, int nrh, int ncl, int nch)
 {
   int i;

   for(i=nrl; i <= nrh; ++i) {
     free((char *) (m[i]+ncl));
   }
   free((char *) (m+nrl));
 }

 #if defined (__TURBOC__)
 #  pragma argsused
 #endif

 void
 free_ivector(int *v, int nl, int nh)
 {
   free((char *) (v+nl));
 }

 #if defined (__TURBOC__)
 #  pragma argsused
 #endif

 void
 free_imatrix(int *m, int nrl, int nrh, int ncl, int nch)
 {
   int i;

   for(i=nrl; i <= nrh; ++i) {
     free((char *) (m[i]+ncl));
   }
   free((char *) (m+nrl));
 }

 /** use my own strdup **/

 char *
 mystrdup(const char *str)
 {
   char *s;
   s=(char *)malloc((strlen(str)+1) * sizeof(char));
   if (s == (char *) NULL) return (s);
   (void) strcpy(s, str);
   return (s);
 }
