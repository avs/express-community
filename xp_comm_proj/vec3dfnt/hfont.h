/*		@(#)$RCSfile: hfont.h,v $ $Revision: 1.1 $ $AVS$ $Date: 1995/03/30 02:56:04 $	*/
/*
			Copyright (c) 1993 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.
	
	This file is under sccs control at AVS in:
	$Source: /net/build/cvs/repository/xp_demo/mods/hfont.h,v $
*/
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
 * $Id: hfont.h,v 1.1 1995/03/30 02:56:04 ianc Exp $
 *
 * $Log: hfont.h,v $
 * Revision 1.1  1995/03/30 02:56:04  ianc
 * Initial revision
 *
 * Revision 1.2  92/12/29  15:50:52  chas
 * initial release, Chas Williams (chas@ra.nrl.navy.mil)
 * 
 * Revision 1.1  92/12/29  15:49:03  chas
 * Initial revision
 * 
 * 
 */

#define NGLYPHS 3926

#define GET_COORD(x)  ( (int) (*x) - (int) 'R' )

extern char *hershey_fonts[NGLYPHS+1];

extern int Roman_Plain();
extern int Roman_Simplex();
extern int Roman_Duplex();
extern int Roman_Triplex();
extern int Roman_Complex();
extern int Script_Simplex();
extern int Script_Complex();
extern int Italics_Triplex();
extern int Italics_Complex();
extern int Gothic_English();
extern int Greek_Plain();
extern int Greek_Simple();
extern int Greek_Complex();

extern int ((*hfont_list[])());
