/*              @(#)$RCSfile: vecfonts.h,v $ $Revision: 1.2 $ $AVS$ $Date: 1995/04/10 11:55:22 $     */
/*
			Copyright (c) 1993 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.
	
	This file is under sccs control at AVS in:
	/src1/sccs/xp_demo/mods/s.vecfonts.h
*/

/*******************************************************************
(From ftp.uu.net:/usenet/comp.sources.unix/volume4/herhsey)

This distribution is made possible through the collective encouragement
of the Usenet Font Consortium, a mailing list that sprang to life to get
this accomplished and that will now most likely disappear into the mists
of time... Thanks are especially due to Jim Hurt, who provided the packed
font data for the distribution, along with a lot of other help.

This file describes the Hershey Fonts in general, along with a description of
the other files in this distribution and a simple re-distribution restriction.

USE RESTRICTION:
	This distribution of the Hershey Fonts may be used by anyone for
	any purpose, commercial or otherwise, providing that:
		1. The following acknowledgements must be distributed with
			the font data:
			- The Hershey Fonts were originally created by Dr.
				A. V. Hershey while working at the U. S.
				National Bureau of Standards.
			- The format of the Font data in this distribution
				was originally created by
					James Hurt
					Cognition, Inc.
					900 Technology Park Drive
					Billerica, MA 01821
					(mit-eddie!ci-dandelion!hurt)
		2. The font data in this distribution may be converted into
			any other format *EXCEPT* the format distributed by
			the U.S. NTIS (which organization holds the rights
			to the distribution and use of the font data in that
			particular format). Not that anybody would really
			*want* to use their format... each point is described
			in eight bytes as "xxx yyy:", where xxx and yyy are
			the coordinate values as ASCII numbers.

*PLEASE* be reassured: The legal implications of NTIS' attempt to control
a particular form of the Hershey Fonts *are* troubling. HOWEVER: We have
been endlessly and repeatedly assured by NTIS that they do not care what
we do with our version of the font data, they do not want to know about it,
they understand that we are distributing this information all over the world,
etc etc etc... but because it isn't in their *exact* distribution format, they
just don't care!!! So go ahead and use the data with a clear conscience! (If
you feel bad about it, take a smaller deduction for something on your taxes
next week...)

The Hershey Fonts:
	- are a set of more than 2000 glyph (symbol) descriptions in vector 
		( <x,y> point-to-point ) format
	- can be grouped as almost 20 'occidental' (english, greek,
		cyrillic) fonts, 3 or more 'oriental' (Kanji, Hiragana,
		and Katakana) fonts, and a few hundred miscellaneous
		symbols (mathematical, musical, cartographic, etc etc)
	- are suitable for typographic quality output on a vector device
		(such as a plotter) when used at an appropriate scale.
	- were digitized by Dr. A. V. Hershey while working for the U.S.
		Government National Bureau of Standards (NBS).
	- are in the public domain, with a few caveats:
		- They are available from NTIS (National Technical Info.
			Service) in a computer-readable from which is *not*
			in the public domain. This format is described in
			a hardcopy publication "Tables of Coordinates for
			Hershey's Repertory of Occidental Type Fonts and
			Graphic Symbols" available from NTIS for less than
			$20 US (phone number +1 703 487 4763).
		- NTIS does not care about and doesn't want to know about
			what happens to Hershey Font data that is not
			distributed in their exact format.
		- This distribution is not in the NTIS format, and thus is
			only subject to the simple restriction described
			at the top of this file.

Hard Copy samples of the Hershey Fonts are best obtained by purchasing the
book described above from NTIS. It contains a sample of all of the Occidental
symbols (but none of the Oriental symbols).

This distribution:
	- contains
		* a complete copy of the Font data using the original
			glyph-numbering sequence
		* a set of translation tables that could be used to generate
			ASCII-sequence fonts in various typestyles
		* a couple of sample programs in C and Fortran that are
			capable of parsing the font data and displaying it
			on a graphic device (we recommend that if you
			wish to write programs using the fonts, you should
			hack up one of these until it works on your system)
			
	- consists of the following files...
		hershey.doc - details of the font data format, typestyles and
				symbols included, etc.
		hersh.oc[1-4] - The Occidental font data (these files can
					be catenated into one large database)
		hersh.or[1-4] - The Oriental font data (likewise here)
		*.hmp - Occidental font map files. Each file is a translation
				table from Hershey glyph numbers to ASCII
				sequence for a particular typestyle.
		hershey.f77 - A fortran program that reads and displays all
				of the glyphs in a Hershey font file.
		hershey.c   - The same, in C, using GKS, for MS-DOS and the
				PC-Color Graphics Adaptor.

Additional Work To Be Done (volunteers welcome!):

	- Integrate this complete set of data with the hershey font typesetting
		program recently distributed to mod.sources
	- Come up with an integrated data structure and supporting routines
		that make use of the ASCII translation tables
	- Digitize additional characters for the few places where non-ideal
		symbol substitutions were made in the ASCII translation tables.
	- Make a version of the demo program (hershey.c or hershey.f77) that
		uses the standard Un*x plot routines.
	- Write a banner-style program using Hershey Fonts for input and
		non-graphic terminals or printers for output.
	- Anything else you'd like!

********************************************************************************/
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
 * $Id: vecfonts.h,v 1.2 1995/04/10 11:55:22 ianc Exp $
 *
 * $Log: vecfonts.h,v $
 * Revision 1.2  1995/04/10 11:55:22  ianc
 * better normals, new rendering type
 *
 * Revision 1.2  92/12/29  15:50:53  chas
 * initial release, Chas Williams (chas@ra.nrl.navy.mil)
 * 
 * Revision 1.1  92/12/29  15:49:04  chas
 * Initial revision
 * 
 * 
 */

#define MAX_LABEL_SIZE 300
#define LINE 	0
#define STROKE 	1
#define THREED	2
#define CAPPED	3

extern int Vlabel();

extern float Vlabel_width();

extern int Vchar();

#include "hfont.h"


/*
 * ===== STRUCTS ========
 * temp storage in local process
 * for data, prior to posting to
 * output field.
 */
typedef struct _OUTmesh {
	/*
	 * Structure Storage Area
	 */
	int nnodes;             /* number of output nodes */
	float *xyz;             /* coord array */
	char *data;             /* node data storage */
	int   size;             /* size of node data array */
	int   nline;            /* number of lines */
	int   *line_conn;       /* line connectivity */
	int   line_size;        /* line array alloc buffer size */
	int   ntri;             /* number of tris */
	int   *tri_conn;        /* tri connectivity */
	int   tri_size;         /* tri array alloc buffer size */
	int   nquad;            /* number of quads */
	int   *quad_conn;       /* quad connectivity */
	int   quad_size;        /* quad array alloc buffer size */
        /*
	 * Specs for Output
	 */
        int nspace;            /* 1D, 2D, 3D, etc. */
        int dtype;             /* node data storage type */
        int veclen;            /* node data veclen */
        int comp;              /* which component to use */
	int null_flag;         /* null data present flag */
	double null_value;

} OUTmesh;

