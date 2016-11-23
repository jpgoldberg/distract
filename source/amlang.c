/*
 * This file, amlang.h, is part of the distrat/askmap package
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

static char *deflang[] ={
	"English",
	"yes",
	"no",
	"Can't open recover file %s\n",
	"Couldn't get last pair info from file\n",
	"Old map read in: will query from pair %d\n",
	"What will be the map file's name? ",
	"Such a file exists. Overwrite it? [y/n] ",
	"Error opening file %s\n",
	"Number of nodes in the map? ",
	"Must be greater than 2\n",
	"Couldn't get memory for map\n",
	"Enter the map-id:  ",
	"Couldn't get memory for map id!\n",
	"Input nodes, [type return after each node]\n",
	"Node %s: ",
	"Node %d out of range.  Try again\n",
	"%d already been selected.  Try again\n",
	"Wrote map to file %s\n",
	"Usage:\naskmap [-h] [-L language] [-r recfile]\n",
	"\t-h\t\tDisplay this message\n",
	"\t-r recfile\tRead (recover) partially completed map\n",
	"Error generating Ross array\n",
	"%dth pair %d%% done\n",
	"Influence of node %d on %d [-3 to 3]: ",
	"Quit: Save state first? [y/n]? ",
	"Quitting\n",
	"Not a valid number (%s): re-enter\n",
	"Bad number of constructs (%d)\n",
	"Saving map in file askmap.tmp\n",
	"Error: couldn't open write to save file.  Tough!\n",
	"\t-L language\tUse language for most user interactions\n",
	"Error: couldn't get memory for node names.  Exiting\n",
	"Error: couldn't get memory for node name.  Exiting\n",
	"Enter text for node %d (<RETURN> for none): ",
	"\"%s\" ---> \"%s\"\n",
	"\t-C\t\tDisplay License information\n",
	"UNKNOWN"
};


#include "distrat.h"
#include "lang.h"
#if !defined(TRUE)
#   define TRUE	1
#endif
#if !defined(FALSE)
#   define FALSE	0
#endif
char *L[LSTRNUM];
static int Lloaded = FALSE;
static void parsestr(char *, char *);

int
getlang(char *s)
{
    FILE *lfp;
    char lfname[30];
    char tempstr[128];
    int i, len;
    extern char *L[];
    extern int Lloaded;

    if(s == (char *)NULL) {
	for(i=0; i < LSTRNUM; ++i) {
	    L[i]=deflang[i];
	}
	return(i);
    }

    /* we will set up reasonable searching later */
    strcpy(lfname, s);
    strcat(lfname, ".aml");
    lfp=fopen(lfname, "r");
    if (lfp == (FILE *)NULL) {
	fprintf(stderr,
	    "can't open language file %s\n", s);
	exit(EFILE);
    }
    if(Lloaded == TRUE) {
	for(i=0; i < LSTRNUM; ++i) {
	    free(L[i]);
	}
    }
    i=0;
    while(fgets(tempstr, 128, lfp) != NULL) {
	len = strlen(tempstr);
	tempstr[len-1]= '\0';  /* get rid of '\n' */
	L[i] = (char *) malloc((len+1)*sizeof(char));
	if(L[i] == (char *)NULL) {
	    fprintf(stdout, "Error with language memory\n");
	    exit(EMEM);
	}
	parsestr(L[i], tempstr);
	++i;
	if (i > LSTRNUM ) {
	    fprintf(stderr,
	       "language file is too long\n");
	    exit(1);
	}
    }
    fclose(lfp);

    if(i != LSTRNUM) {
	fprintf(stderr,
	   "language file is too short (%d lines)\n", i);
	exit(1);
    }
    Lloaded = TRUE;
    return(i);
}

static
void parsestr(s1, s2)  /* parse s2, put output in s1 */
char *s1, *s2;
{
    int i1, i2, state, len;
    int tmpcnt, tmpval;


    len = strlen(s2);

    i1=0;
    state = 1;
    for(i2 = 0; i2 < len; ++i2) {
	switch(state) {
	    case 1:  
		if(s2[i2] != '\\') s1[i1++]=s2[i2];
		else state =2;
		break;
	    case 2:
		switch(s2[i2]) {
		    /* "\a" is ANSI only, may yield warning, but OK */
		    case 'a': s1[i1++] = '\a'; state =1; break;
		    case 'b': s1[i1++] = '\b'; state =1; break;
		    case 'f': s1[i1++] = '\f'; state =1; break;
		    case 'n': s1[i1++] = '\n'; state =1; break;
		    case 't': s1[i1++] = '\t'; state =1; break;
		    case 'r': s1[i1++] = '\f'; state =1; break;
		    case 'v': s1[i1++] = '\v'; state =1; break;
		    case '\\': s1[i1++] = '\\'; state =1; break;
		    case '\'': s1[i1++] = '\''; state =1; break;
		    case '\"': s1[i1++] = '\"'; state =1; break;
		    case '0': case '1': case '2': case '3': case '4':
		    case '5': case '6': case '7': case '8': case '9':
			tmpcnt=1;
			tmpval=s2[i2]-'0';
			while(isdigit(s2[i2+tmpcnt]) && tmpcnt < 3) {
			    tmpval *= 8;
			    tmpval += s2[i2+tmpcnt] - '0';
			    ++tmpcnt;
			}
			s1[i1++]=tmpval;
			i2 += tmpcnt - 1;
			state=1;
			break;
		    default: s1[i1++] = s2[i2]; state=1; break;
		}
	    break;
	}
    }
    s1[i1++]='\0';
}
