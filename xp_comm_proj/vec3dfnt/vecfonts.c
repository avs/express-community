#ifdef SCCS
static char sccsid[]="@(#)$RCSfile: vecfonts.c,v $ $Revision: 1.3 $ $AVS$ $Date: 1995/04/10 11:57:02 $";
#endif
/*
			Copyright (c) 1993 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.
	
	This file is under sccs control at AVS in:
	/src1/sccs/xp_demo/mods/s.vecfonts.c
*/
/****************************************************************************
                  INTERNATIONAL AVS CENTER
	(This disclaimer must remain at the top of all files)

WARRANTY DISCLAIMER

This module and the files associated with it are distributed free of charge.
It is placed in the public domain and permission is granted for anyone to use,
duplicate, modify, and redistribute it unless otherwise noted.  Some modules
may be copyrighted.  You agree to abide by the conditions also included in
the AVS Licensing Agreement, version 1.0, located in the main module
directory located at the International AVS Center ftp site and to include
the AVS Licensing Agreement when you distribute any files downloaded from 
that site.

The International AVS Center, MCNC, the AVS Consortium and the individual
submitting the module and files associated with said module provide absolutely
NO WARRANTY OF ANY KIND with respect to this software.  The entire risk as to
the quality and performance of this software is with the user.  IN NO EVENT
WILL The International AVS Center, MCNC, the AVS Consortium and the individual
submitting the module and files associated with said module BE LIABLE TO
ANYONE FOR ANY DAMAGES ARISING FROM THE USE OF THIS SOFTWARE, INCLUDING,
WITHOUT LIMITATION, DAMAGES RESULTING FROM LOST DATA OR LOST PROFITS, OR ANY
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES.

This AVS module and associated files are public domain software unless
otherwise noted.  Permission is hereby granted to do whatever you like with
it, subject to the conditions that may exist in copyrighted materials. Should
you wish to make a contribution toward the improvement, modification, or
general performance of this module, please send us your comments:  why you
liked or disliked it, how you use it, and most important, how it helps your
work. We will receive your comments at avs@ncsc.org.

Please send AVS module bug reports to avs@ncsc.org.

******************************************************************************/
/*                        NRL/RCD Visualization
 *
 * Source code and documentation developed at NRL/RCD Visualization Lab
 * are in the public domain. Hence, no limitation exists on the right to use,
 * publish, or resale the source code and documentation.
 *
 * We ask, but do not require, that the following message be included in all
 * derived works:
 *
 * THE NAVAL RESEARCH LABORATORY GIVES NO WARRANTY, EXPRESSED OR IMPLIED, FOR
 * THE SOFTWARE AND/OR DOCUMENTATION PROVIDED, INCLUDING, WITHOUT LIMITATION,
 * WARRANTY OF MERCHANTABILITY AND WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE
 */

/* 
 * $Id: vecfonts.c,v 1.3 1995/04/10 11:57:02 ianc Exp $
 *
 * $Log: vecfonts.c,v $
 * Revision 1.3  1995/04/10 11:57:02  ianc
 * improved normals, new mixed type = capped
 *
 * Revision 1.2  92/12/29  15:50:50  chas
 * initial release, Chas Williams (chas@ra.nrl.navy.mil)
 * 
 * Revision 1.1  92/12/29  15:49:03  chas
 * Initial revision
 * 
 * AVS/Express Revision, March 13 1995, Ian Curington
 * 
 */

#include <stdio.h>
#include <math.h>

#include "gen.h"
#include "vecfonts.h"


struct pt {
	float x, y, z;
	};

int
Vlabel(obj, x, y, z, label, font, kern, width, flags )

    OUTmesh *obj;
    float *x, *y, *z;
    char *label;
    int  font;
    float kern, width;
    int flags;
{
        int (*fonttype)();
	char *p;
	int glyph;

        fonttype = hfont_list[ font ];

        for ( p=label; *p != '\0'; ++p ) {
		if (*p == '\\' && *(p+1) != '\0' ) {
			++p;
			if ( *p >= '0' && *p <= '9' ) {
			   	glyph = atoi( p );
			   	while ( *p!='\0' && *p>='0' && *p<='9' ) ++p;
				--p;
			} else if ( *p == 'n' ) {
				*y -= 30.0;
				*x = 0.0;
				glyph = 0; /* print null char */
			} else if ( *p == 'r' ) {
				*x = 0.0;
				glyph = 0;
			}
		} else
			glyph = fonttype( *p );

#ifdef DEBUG
	fprintf(stderr,"Vlabel processing char: %c\n", *p);
#endif

		Vchar(obj,x,y,z,glyph,kern,width,flags);
	}
	return 1;
}

float
Vlabel_width(label, fonttype)

    char *label;
    int (*fonttype)();
{
	char *p, *c;
	float width;

        for ( p=label; *p != '\0'; ++p ) {
		c = hershey_fonts[ fonttype(*p) ];
		width += (float) - GET_COORD(c++);
		width += (float) GET_COORD(c++);
	}

	return width;
}
		

Vchar(obj, x, y, z, glyph, kern, width, flags)

        OUTmesh *obj;
        float *x, *y, *z;
        int glyph;
        float kern;
        float width;
        int flags;
{
	char *p;

	int cur_x, cur_y, dx, dy;
	float left, right;

	struct {
		struct pt a, b;
		} line;

#ifdef DEBUG
	fprintf(stderr,"Vchar glyph: %d\n", glyph);
#endif
        if ( glyph >= NGLYPHS )
        {
#ifdef DEBUG
	fprintf(stderr,"Vchar glyph out of range!: %d\n", glyph);
#endif
	    return 0;
	}

	p = hershey_fonts[ glyph ];

#ifdef DEBUG
	fprintf(stderr,"Vchar fontstring: %s\n", p);
#endif

	if ( *p == '\0' ) return 0;

	left = (float) - GET_COORD(p++);
	right = (float) GET_COORD(p++);

	*x += left * kern;

	/* does this glyph have more than a bounding box? */
	if ( *p != '\0' )
	{
		/* moveto -- move to starting point */
		cur_x = GET_COORD(p++);
		cur_y = - GET_COORD(p++);

		/* process moves */
		while ( *p != '\0' )
		{
			dx = GET_COORD(p++);
			dy = - GET_COORD(p++);

			/* moveto */
			if ( dx == -50 )
			{
				dx = GET_COORD(p++);
				dy = - GET_COORD(p++);
			}
			/* lineto */
			else 
			{
				line.a.x = *x + (float) cur_x;
				line.a.y = *y + (float) cur_y;
				line.a.z = *z;
				line.b.x = *x + (float) dx;
				line.b.y = *y + (float) dy;
				line.b.z = *z;

			switch(flags)
				{
				case LINE:
					/***
					GEOMadd_disjoint_line(obj, &line, NULL,
						2, GEOM_COPY_DATA);
					***/
					VLcreate_line(obj,0,&line,NULL );

					break;
				case STROKE:
					stroke(obj,line.a.x,line.a.y,
						line.b.x,line.b.y,width);
					break;
				case THREED:
					draw3d(obj,line.a.x,line.a.y,
						line.b.x,line.b.y,width);
					break;
				case CAPPED:
					stroke(obj,line.a.x,line.a.y,
						line.b.x,line.b.y,width);
					draw3d(obj,line.a.x,line.a.y,
						line.b.x,line.b.y,width);
					break;
				}
			}
			cur_x = dx;
			cur_y = dy;
		}
	}
	*x += right * kern;
	return 1;
}

draw3d(obj, x1, y1, x2, y2, width)
OUTmesh *obj;
float x1, y1, x2, y2;
float width;
{
	struct {
		struct pt a, b, c, d;
		} quad;


	width = width * 10.0;

	quad.a.x = x1;
	quad.a.y = y1;
	quad.a.z = 0.0;

	quad.b.x = x1;
	quad.b.y = y1;
	quad.b.z = width;

	quad.c.x = x2;
	quad.c.y = y2;
	quad.c.z = width;

	quad.d.x = x2;
	quad.d.y = y2;
	quad.d.z = 0.0;


	/***
	GEOMadd_disjoint_polygon(obj, &quad, GEOM_NULL, GEOM_NULL, 
			4, GEOM_NOT_SHARED | GEOM_CONVEX, GEOM_COPY_DATA );
         ***/
        VLcreate_quad(obj,0,&quad,NULL);
	return 1;
}

stroke(obj, x1, y1, x2, y2, width)
OUTmesh *obj;
float x1, y1, x2, y2;
float width;
{
	struct {
		struct pt a, b, c, d;
		} quad;
		
	float dx, dy, l, nx, ny, vx, vy;

	/* find the normal vector perpendicular to the line */
	ny = x2 - x1;
	nx = y2 - y1;
	nx = - nx;
	l = sqrt( nx*nx + ny*ny );
	nx = nx / l;
	ny = ny / l;

	dx=nx*width;
	dy=ny*width;

	/* a fudge factor to make line joins look better */
	vx=dy*0.4*width;
	vy=dx*0.4*width;

	/* now give a little width */
	quad.a.x = (x1-vx) - dx;
	quad.a.y = (y1+vy) - dy;
	quad.a.z = 0.0;

	quad.b.x = (x2+vx) - dx;
	quad.b.y = (y2-vy) - dy;
	quad.b.z = 0.0;

	quad.c.x = (x2+vx) + dx;
	quad.c.y = (y2-vy) + dy;
	quad.c.z = 0.0;

	quad.d.x = (x1-vx) + dx;
	quad.d.y = (y1+vy) + dy;
	quad.d.z = 0.0;

	/**
	GEOMadd_disjoint_polygon(obj, &quad, GEOM_NULL, GEOM_NULL, 
			4, GEOM_NOT_SHARED | GEOM_CONVEX, GEOM_COPY_DATA );
	 **/
        VLcreate_quad(obj,0,&quad,NULL);
	return 1;
}

