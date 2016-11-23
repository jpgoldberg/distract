/*
 * This file, lang.h, is part of the distrat/askmap package
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

#define LSTRNUM		38

#define	L_LNAME	0	/* English */
#define L_YES	1	/* yes */
#define L_NO	2	/* no */
#define L_CORF	3	/* Can't open recover file %s\n */
#define L_CGLP	4	/* Couldn't get last pair info from file\n */
#define L_ORQFP	5	/* Old map read in: will query from pair %d\n */
#define L_MFN	6	/* What will be the map file's name?  */
#define L_OVERW	7	/* Such a file exists. Overwrite it? [y/n]  */
#define L_EFO	8	/* Error opening file %s\n */
#define L_NoNum	9	/* Number of nodes in the map?  */
#define L_GT2	10	/* Must be greater than 2\n */
#define L_CGMM	11	/* Couldn't get memory for map\n */
#define L_MID	12	/* Enter the map-id:   */
#define L_CGMMID	13	/* Couldn't get memory for map id!\n */
#define L_INNOD	14	/* Input nodes, [type return ... */
#define L_NODEQ	15	/* Node %s:  */
#define L_NORNG	16	/* Node %d out of range.  Try again\n */
#define L_NSEL	17	/* %d already been selected.  Try again\n */
#define L_WMF	18	/* Wrote map to file %s\n */
#define L_UsCMD	19	/* Usage:\naskmap [-h] [-r recfile]\n */
#define L_Us_h	20	/* \t-h\t\tDisplay this message\n */
#define L_Us_r	21	/* \t-r recfile\tRead (recover) ... */
#define L_ERoss	22	/* Error generating Ross array\n */
#define L_NDONE	23	/* %dth pair %d%% done\n */
#define L_INFL	24	/* Influence of node %d on %d [-3 to 3]:  */
#define L_QSAVE	25	/* Quit: Save state first? [y/n]?  */
#define L_QUIT	26	/* Quitting\n */
#define L_NVNUM	27	/* Not a valid number (%s): re-enter\n */
#define L_BCNST	28	/* Bad number of constructs (%d)\n */
#define L_SAVE	29	/* Saving map in file askmap.tmp\n */
#define L_ESVOP	30	/* Error: couldn't open write to save file... */
#define L_Us_L	31	/* \t-L language\tUse language for most ... */
#define L_ENNMEMS 32	/* Error: couldn't get mem for node names */
#define L_ENNMEM 33	/* Error: couldn't get mem for node name */
#define L_NText	34	/*  Input text to go with node %d: ... */
#define L_TINFL	35	/* Infl. of \"%s\" on \"%s\"\n"  */
#define L_Us_C	36	/* \t-C\t\tDisplay license informatio */
#define L_UNKNW 37	/* UNKNWON */
