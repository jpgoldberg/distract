/*
 * This file, version.h, is part of the distrat/askmap package
 * by Jeff Goldberg Copyright (c) 1992, 1993, 1994, 1995
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

char version[]="version 1.1b";	/* used in all programs */


void
License(FILE *fp)
{
    fputs("\nThis program is free software; you can redistribute",fp);
    fputs(" it and/or modify\n",fp);
    fputs("it under the terms of the GNU General Public License", fp);
    fputs(" as published by\n", fp);
    fputs("the Free Software Foundation; either version 2 of", fp);
    fputs(" the License, or\n", fp);
    fputs("(at your option) any later version.\n\n", fp);

    
    fputs("This program is distributed in the hope that", fp);
    fputs(" it will be useful,\n", fp);    
    fputs("but WITHOUT ANY WARRANTY; without even the", fp);
    fputs(" implied warranty of\n", fp);    
    fputs("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.", fp);
    fputs("  See the\n", fp);    
    fputs("GNU General Public License for more details.\n\n", fp);

    
    fputs("You should have received a copy of the GNU General", fp);
    fputs(" Public License\n", fp);    
    fputs("along with this program; if not, write to", fp);
    fputs(" the Free Software\nFoundation,", fp);
    fputs(" Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n\n", fp);

    return;
}
