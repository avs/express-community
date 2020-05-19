#/* =S+H= */ ifdef SCCS
/* =S+H= */ static char sccsid[]="@(#)vec_lab.c	1.4 AVS 95/04/05";
#/* =S+H= */ endif
/*
			Copyright (c) 1993 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.
	
	This file is under sccs control at AVS in:
	/src1/sccs/xp_demo/mods/s.vec_lab.c
*/
#ifdef SCCS
static char sccsid[]="@(#)vec_lab.c	1.2 AVS 95/03/06";
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
	/src1/sccs/xp_demo/mods/s.veclab.c
*/
/*
 * ===== HEADER ========
 * veclab.c
 *
 * Cell Set Generator Test Module
 * This module is a test bed for creating new
 * multi-cell_set output fields, with or without
 * node_data.
 *
 * The generation code is structured such that
 * each cell is added incrementally,
 * and you don't need to know how may cells you
 * are going to add ahead of time.
 *
 * This is also a memory efficient method,
 * where ARRalloc and ARRrealloc are used
 * to define a local (expandable) buffer.
 * When complete, these buffers are handed
 * to the OM as part of the output field.
 * In this way no data need be copied.
 *
 * Each cell is added by copying over data
 * from a small local buffer, for
 * application design simplicity, but this
 * could be changed without a large re-write.
 *
 * Revision History:
 * 24 Nov 94 IC Original
 * 30 Nov 94 IC Tested with Tri's, +node data
 *  6 Dec 94 IC added glyph, viariable cylinder example
 * 13 Mar 95 IC, converted from "CellGen" to Vector Label,
 *              pulled in IAC code.
 *
 */

/******** headers from included AVS5 demo code: **************/

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
 * $Id: vec_lab.c,v 1.5 1995/10/27 07:35:45 ianc Exp $
 *
 * $Log: vec_lab.c,v $
 * Revision 1.5  1995/10/27 07:35:45  ianc
 * Version 16, initial post V2.0 update into CVS
 *
 * Revision 1.2  92/12/29  15:50:28  chas
 * initial release, Chas Williams (chas@ra.nrl.navy.mil)
 * 
 * Revision 1.1  92/12/29  15:49:02  chas
 * Initial revision
 * 
 * 
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


/*
 * ===== INCLUDES ========
 */
#include <avs/port.h>
#include <avs/err.h>
#include <avs/om.h>
#include <math.h>

#include "vecfonts.h"

/*
 * ===== MACROS ========
 */
#define ERR_RETURN(A) \
      { ERRerror("veclab",1,ERR_ORIG, A); return(0);}

#define MAX_NAME_SIZE 1024

#define  ALLOC_SIZE 1000


/*
 * Macro Utility to resolve a real arg
 */
#define RESOLVE_OBJ(elem_id,string,flag)                     \
  {local_id = OMfind_subobj((elem_id),                       \
              OMstr_to_name((string)),flag);                 \
  if ( OMis_null_obj(local_id))                              \
  {                                                          \
      ERRerror("Vector Label",1,ERR_ORIG,                      \
               "search failed for subobj name");             \
      return(0);                                             \
  }}

#define RESOLVE_INT_PARM(elem_id,string,flag,ii)             \
  {local_id = OMfind_subobj((elem_id),                       \
              OMstr_to_name((string)),flag);                 \
  if ( OMis_null_obj(local_id))                              \
  {                                                          \
      ERRerror("Vector Label",1,ERR_ORIG,                      \
               "search failed for subobj int param name");   \
      return(0);                                             \
  }                                                          \
  if ( OMget_int_val(local_id,&(ii)) != 1 )                  \
  {                                                          \
      ERRerror("Vector Label",1,ERR_ORIG,                      \
               "search failed for subobj int param value");  \
      return(0);                                             \
  }                                                          \
  } 

#define RESOLVE_FLT_PARM(elem_id,string,flag,xx)             \
  {local_id = OMfind_subobj((elem_id),                       \
              OMstr_to_name((string)),flag);                 \
  if ( OMis_null_obj(local_id))                              \
  {                                                          \
      ERRerror("Vector Label",1,ERR_ORIG,                      \
               "search failed for subobj flt param name");   \
      return(0);                                             \
  }                                                          \
  if ( OMget_real_val(local_id,&(xd)) != 1 )                 \
  {                                                          \
      ERRerror("Vector Label",1,ERR_ORIG,                      \
               "search failed for subobj flt param value");  \
      return(0);                                             \
  }                                                          \
  (xx) = xd;                                                 \
  }


#define RESOLVE_STR_PARM(elem_id,string,flag,cc)             \
  {local_id = OMfind_subobj((elem_id),                       \
              OMstr_to_name((string)),flag);                 \
  if ( OMis_null_obj(local_id))                              \
  {                                                          \
      ERRerror("Vector Label",1,ERR_ORIG,                      \
               "search failed for subobj str param name");   \
      return(0);                                             \
  }                                                          \
  if ( OMget_str_val(local_id,(char **) &(cc),0) != 1 )      \
  {                                                          \
      ERRerror("Vector Label",1,ERR_ORIG,                      \
               "search failed for subobj str param value");  \
      return(0);                                             \
  }                                                          \
  }



/*
 * ==================================
 * ===== PRIMARY ENTRY POINT ========
 * ==================================
 */

int vector_label(elem)
    OMobj_id elem;
{
	/*
 	 * ===== PRIVATE LOCAL DATA ========
 	 */
	OMobj_id dst_id, local_id;
	OUTmesh  hh, *h;
	char *Label;
	float Kerning, Stroke_Width;
	int Font, LineStyle;
	double xd;
	
	/*
 	 * ===== RESOLVE POINTERS to FIELDS ========
 	 */
        h = &hh;
        RESOLVE_OBJ(elem,"out",OM_OBJ_RW); dst_id = local_id;

	/*
 	 * ===== RESOLVE POINTERS to PARAMETERS ========
 	 */
        RESOLVE_STR_PARM(elem,"Label",OM_OBJ_RD,Label);
        RESOLVE_FLT_PARM(elem,"Kerning",OM_OBJ_RD,Kerning);
        RESOLVE_FLT_PARM(elem,"Stroke_Width",OM_OBJ_RD,Stroke_Width);
        RESOLVE_INT_PARM(elem,"Font",OM_OBJ_RD,Font);
        RESOLVE_INT_PARM(elem,"LineStyle",OM_OBJ_RD,LineStyle);
		
	/* ===== TEST FONT-VALUE ======== */
	if(Font<0 || Font >12) return(0);

#ifdef DEBUG
        printf ("vec_lab.c: input label string is: %s\n",Label);
#endif

	/*
 	 * ===== CALL OPERATOR FUNCTION ========
 	 */
	if (FUNCveclab(h,
		       Label,
		       Kerning,
		       Stroke_Width,
		       Font,
		       LineStyle,
		       dst_id) != 1)
		ERR_RETURN("FUNCveclab error return status");
		return(0);
	
	return(1);
}

/*
 * ===== FUNCTION to COMPUTE STRUCTURE ========
 */
int FUNCveclab (h,
		Label,
		Kerning,
		Stroke_Width,
		Font,
		LineStyle,
		out)

    OMobj_id out;
    OUTmesh  *h;

	char *Label;
	float Kerning, Stroke_Width;
	int Font, LineStyle;
{
	/*
 	 * ===== PRIVATE LOCAL DATA ========
 	 */
	char  units[MAX_NAME_SIZE], label[MAX_NAME_SIZE];
	int data_flag;
	float x, y, z;

        /**********
	 * Begin  *
	 **********/

        /*
	 * Set Output Field Characteristics
	 */
        data_flag = 0; /* controls if node_data is generated or not */
	h->comp = 0;
	h->veclen = 1;
	h->dtype = DTYPE_BYTE;
	h->nspace = 3;
	h->null_flag = 0;
	h->null_value = -999.0;

	strcpy(label, "veclab_label");
	strcpy(units, "font_pts");

        /*
	 * Initialize internal data structures
	 */
        if (FUNCinit_veclab(h,out,data_flag) != 1)
		ERR_RETURN("Error in FUNCinit_veclab");

	/********************************/
	/*   Now Populate Cell Sets     */
	/********************************/



        /*
         * Call top level font gen routine
         */
        x = y = z = 0.0;


        Vlabel(h,&x,&y,&z,Label,Font,Kerning,Stroke_Width,LineStyle);


	/*
         * VLprintoutstruct( h );
	 */


        /*
	 * Complete the Output Field structures from
	 * the template local structure.
	 */
        if ( FUNCcomp_veclab(h, out, data_flag, label, units ) != 1 )
		ERR_RETURN("Error in FUNCcomp_veclab");


	return(1);
}

/*
 * ===== INITIALIZE INTERNALS  ========
 */
int FUNCinit_veclab(h,        /* internal structure storage */
                     out,      /* elem_id of output field */
		     data_flag )  /* flag to allocate node data or not */
    OUTmesh *h;
    OMobj_id out;
    int data_flag;
{

	/********************************/
	/*   Free pre-allocated arrays  */
	/********************************/
	if (FLDset_ncell_sets (out, 0) != 1) {
		ERR_RETURN("Error setting ncell_sets");
	}
	if (FLDset_nnodes (out, 0) != 1) {
		ERR_RETURN("Error setting nnodes");
	}
	h->xyz = NULL;
	h->data = NULL;
	if (FLDset_coord(out, h->xyz, 0, OM_SET_ARRAY_FREE) != 1) {
		ERR_RETURN("Error setting coord");
	}
	FLDset_node_data(out, 0, (char *)h->data, h->dtype, 
			 0, OM_SET_ARRAY_FREE);

	/********************************/
	/*Initially Allocate Temp arrays*/
	/********************************/

	h->nnodes = 0;
	h->size = ALLOC_SIZE;
	h->xyz = (float *)ARRalloc(NULL,
		      DTYPE_FLOAT, h->nspace*h->size, NULL);
	if (data_flag)
		h->data = (char *)ARRalloc(NULL,
		      DTYPE_CHAR,
		      DTYPEtype_size[h->dtype]*h->size*h->veclen, NULL);
	else
		h->data = NULL;
	h->nline = 0;
	h->ntri  = 0;
	h->nquad = 0;
	h->line_size = ALLOC_SIZE;
	h->line_conn = (int *)ARRalloc(NULL, DTYPE_INT, h->line_size, NULL);
	h->tri_size  = ALLOC_SIZE;
	h->tri_conn  = (int *)ARRalloc(NULL, DTYPE_INT, h->tri_size, NULL);
	h->quad_size = ALLOC_SIZE;
	h->quad_conn = (int *)ARRalloc(NULL, DTYPE_INT, h->quad_size, NULL);
	
	return(1);
}

/*
 * ===== COMPLETE OUTPUT FIELD  ========
 */
int FUNCcomp_veclab(h,        /* internal structure storage */
                     out,      /* elem_id of output field */
		     data_flag,/* flag to allocate node data or not */
		     label,    /* node data labels */
		     units)    /* coord and node data units */

    OUTmesh *h;
    OMobj_id out;
    int data_flag;
    char *label;
    char *units;
{

	OMobj_id cell_set;
        int set, out_nsets;

	out_nsets = 0;
	set = 0;

	/********************************/
	/*   Transfer Temp Storage      */
	/*     Nodes, Coordinates       */
	/*     into Output Field        */
	/********************************/
	if (FLDset_coord_units (out, units) != 1) {
		ERR_RETURN("Error setting units");
	}
	if (FLDset_nspace (out, h->nspace) != 1) {
		ERR_RETURN("Error setting nspace");
	}
	if (FLDset_nnodes (out, h->nnodes) != 1) {
		ERR_RETURN("Error setting nnodes");
	}
	if (h->nnodes)
		if (FLDset_coord(out, h->xyz,
		      h->nspace*h->nnodes, OM_SET_ARRAY_FREE) != 1) {
			ERR_RETURN("Error setting coord");
		}
	if (FLDset_ncell_sets (out, out_nsets) != 1) {
		ERR_RETURN("Error setting ncell_sets");
	}

	/********************************/
	/* Add Polyline to Output Field */
	/********************************/
	if (h->nline) {
		/******
		if (FLDadd_cell_set(out, "Polyline") != 1) {
		 ******/
		if (FLDadd_cell_set(out, "Line") != 1) {
			ERR_RETURN("Error setting line cell type");
		}
		if (FLDget_cell_set(out, set, &cell_set) != 1) {
			ERR_RETURN("Error getting line cell set");
		}

                /*
                 * Change over from V1.1 to V2.0
                 * FLD routines for polyhedra and macro cell support
                 */
                /********
                if (FLDset_npolys(cell_set, h->nline) != 1) {
                ********/
                if (FLDset_ncells(cell_set, h->nline) != 1) {
                        ERR_RETURN("Error setting polyline ncells");
                }

                /*********
                if (FLDset_poly_connect(cell_set, h->line_conn, 2*h->nline,
                                        OM_SET_ARRAY_FREE) != 1) {
                **********/
                if (FLDset_node_connect(cell_set, h->line_conn, 2*h->nline,
                                        OM_SET_ARRAY_FREE) != 1) {
                        ERR_RETURN("Error setting polyline cell connect list");
                }
		set++;
	}
	else {
		ARRfree((char *)h->line_conn);
	}

	/********************************/
	/* Add Tri Cells to Output     */
	/********************************/
	if (h->ntri) {
		if (FLDadd_cell_set(out, "Tri") != 1) {
			ERR_RETURN("Error setting Tri cell type");
		}
		if (FLDget_cell_set(out, set, &cell_set) != 1) {
			ERR_RETURN("Error getting Tri cell set");
		}
		if (FLDset_ncells(cell_set, h->ntri) != 1) {
			ERR_RETURN("Error setting Tri ncells");
		}
		if (FLDset_node_connect(cell_set, h->tri_conn, 3*h->ntri,
					OM_SET_ARRAY_FREE) != 1) {
			ERR_RETURN("Error setting Tri cell connect list");
		}
		set++;
	}
	else {
		ARRfree((char *)h->tri_conn);
	}

	/********************************/
	/* Add Quad Cells to Output     */
	/********************************/
	if (h->nquad) {
		if (FLDadd_cell_set(out, "Quad") != 1) {
			ERR_RETURN("Error setting quad cell type");
		}
		if (FLDget_cell_set(out, set, &cell_set) != 1) {
			ERR_RETURN("Error getting quad cell set");
		}
		if (FLDset_ncells(cell_set, h->nquad) != 1) {
			ERR_RETURN("Error setting quad ncells");
		}
		if (FLDset_node_connect(cell_set, h->quad_conn, 4*h->nquad,
					OM_SET_ARRAY_FREE) != 1) {
			ERR_RETURN("Error setting quad cell connect list");
		}
		set++;
	}
	else {
		ARRfree((char *)h->quad_conn);
	}



	/********************************************/
	/* Set Node Data on Field Output            */
	/********************************************/
	if (h->nnodes && data_flag ) {
		if (FLDset_node_data_ncomp (out, 1) != 1) {
			ERR_RETURN("Error setting nnode_data");
		}
		if (FLDset_node_data_comp (out,0,h->veclen,label,units) != 1) {
			ERR_RETURN("Error setting output node component");
		}

		/***
		 *** only use this for special things like texture,
		 *** or RGB, etc.
		 ***
	        data_id = 0;
		FLDset_node_data_id(out, 0, data_id);
		 **/

		if (FLDset_node_data(out, 0, (char *)h->data, h->dtype, 
		     h->veclen*h->nnodes, OM_SET_ARRAY_FREE) != 1) {
			ERR_RETURN("Error setting output node data");
		}
		if (h->null_flag) {
			if (FLDset_node_null_data(out, 0,
			      (char *)&(h->null_value), h->dtype) != 1) {
			  ERR_RETURN("Error setting output null value");
			}
		}
		else {
			if (FLDset_node_null_flag(out, 0, 0) != 1) {
				ERR_RETURN("Error setting output null flag");
			}
		}
	}
	else {
		if (FLDset_node_data_ncomp (out, 0) != 1) {
			ERR_RETURN("Error setting output nnode_data");
		}
	}
	return(1);
}

/*
 * ===== REALLOC NODES  ========
 */
int VLbump_nodes   ( h,        /* internal structure storage */
                     new,      /* requested number of verts to add */
		     data )    /* flag to allocate node data or not */
    OUTmesh *h;
    int new;
    int data;
{

    if (h->nnodes+new*h->nspace >= h->size)
    {
	h->size += ALLOC_SIZE;
	h->xyz = (float *)ARRrealloc(h->xyz,
			DTYPE_FLOAT, h->nspace*h->size, NULL);
        if ( h->xyz == NULL ) 
	    ERR_RETURN("could not realloc xyz coord array");
	if (data)
	{
	    h->data = (char *)ARRrealloc(h->data, DTYPE_CHAR, 
			DTYPEtype_size[h->dtype]*h->size*h->veclen, NULL);
            if ( h->data == NULL ) 
		ERR_RETURN("could not realloc local data array");
        }

    }

    return(1);
}


/*
 * ===== ADD NODE DATA  ========
 */
int VLadd_node_data( h,        /* internal structure storage */
                     new,      /* requested number of verts to add */
		     ldata )   /* new node data */
    OUTmesh *h;
    int new;
    char *ldata;
{
    int i,n;
    char *p;
    int  v;
    
    char      *p_char, *q_char;
    int       *p_int,  *q_int;
    float     *p_flt,  *q_flt;
    short     *p_sht,  *q_sht;
    double    *p_dbl,  *q_dbl;

        v = (h->nnodes) * (h->veclen);
        p = ((char *)h->data) + DTYPEtype_size[h->dtype] * v;
	n = new * h->veclen;

	/*
	 * Set Node Data
	 */
        switch ( h->dtype )
	{
	    case DTYPE_CHAR:
	    case DTYPE_BYTE:
		q_char = (char *)ldata;
		p_char = (char *)p;
                for (i=0; i<n; i++)
                    p_char[i] = q_char[i];
                break;

	    case DTYPE_INT:
		q_int = (int *)ldata;
		p_int = (int *)p;
                for (i=0; i<n; i++)
                     p_int[i] = q_int[i];
                break;

	    case DTYPE_FLOAT:
		q_flt = (float *)ldata;
		p_flt = (float *)p;
                for (i=0; i<n; i++)
                    p_flt[i] = q_flt[i];
                break;

	    case DTYPE_SHORT:
		q_sht = (short *)ldata;
		p_sht = (short *)p;
                for (i=0; i<n; i++)
                    p_sht[i] = q_sht[i];
                break;

	    case DTYPE_DOUBLE:
		q_dbl = (double *)ldata;
		p_dbl = (double *)p;
                for (i=0; i<n; i++)
                    p_dbl[i] = q_dbl[i];
                break;

            default:
	        ERR_RETURN("data type not recognized in VLadd_node_data");
                break;
         }
	 return(1);
}



/*
 * ===== REALLOC LINE CONN  ========
 */
int VLbump_line_conn ( h )        /* internal structure storage */
    OUTmesh *h;
{
	static int line_conn_sz = 2;

	/*
	 * estimate connectivity array size requirements,
	 * bump temp storage if needed.
	 */
	if ((h->nline+1)*line_conn_sz >= (h->line_size)) {
		h->line_size += ALLOC_SIZE;
		h->line_conn = (int *)ARRrealloc(h->line_conn,
		    DTYPE_FLOAT,
		    h->line_size, NULL);
	}

        if ( h->line_conn == NULL )
	  ERR_RETURN("could not realloc local line conn array");
	return(1);
}
/*
 * ===== REALLOC TRI CONN  ========
 */
int VLbump_tri_conn ( h )        /* internal structure storage */
    OUTmesh *h;
{
	static int tri_conn_sz = 3;

	/*
	 * estimate connectivity array size requirements,
	 * bump temp storage if needed.
	 */
	if ((h->ntri+1)*tri_conn_sz >= (h->tri_size))
	{
		h->tri_size += ALLOC_SIZE;
		h->tri_conn = (int *)ARRrealloc(h->tri_conn,
			DTYPE_FLOAT, h->tri_size, NULL);

            if ( h->tri_conn == NULL )
	  	ERR_RETURN("could not realloc local tri conn array");
	}

	return(1);
}

/*
 * ===== REALLOC QUAD CONN  ========
 */
int VLbump_quad_conn ( h )        /* internal structure storage */
    OUTmesh *h;
{
	static int quad_conn_sz = 4;

	/*
	 * estimate connectivity array size requirements,
	 * bump temp storage if needed.
	 */
	if ((h->nquad+1)*quad_conn_sz >= (h->quad_size))
	{
		h->quad_size += ALLOC_SIZE;
		h->quad_conn = (int *)ARRrealloc(h->quad_conn,
			DTYPE_FLOAT, h->quad_size, NULL);

            if ( h->quad_conn == NULL )
	       ERR_RETURN("could not realloc local quad conn array");
	}

	/*
         * VLprintoutstruct( h );
	 */

	return(1);
}


/*
 * ===== ADD A LINE TO THE STRUCTURE ========
 */
int VLcreate_line    ( h,        /* internal structure storage */
		     data,     /* control flag, save data or not */
		     points,   /* line coord array */
		     ldata )   /* line node data */
    OUTmesh *h;
    int data;
    float *points;
    char *ldata;
{
	int  i, n, nnew;

        nnew = 2;

        if (VLbump_nodes( h, nnew, data ) != 1)
	    ERR_RETURN("could not bump_nodes for line");

	/*
	 * Set Coordinates
	 */
        for (i=0; i < nnew*h->nspace; i++ ) {
		h->xyz[h->nnodes * h->nspace + i] = points[i];
	}

	/*
	 * Set Node Data
	 */
	if (data) {
            if (VLadd_node_data( h, nnew, ldata ) != 1)
	        ERR_RETURN("could not add_node_data for lines");
        }

        if ( VLbump_line_conn ( h ) != 1)
	    ERR_RETURN("could not bump_line_conn");

	/*
	 * Store Connectivity for one line.
	 */
	n = h->nnodes;
	h->line_conn[nnew*h->nline  ] = n;
	h->line_conn[nnew*h->nline+1] = n+1;
	(h->nline)++;
	h->nnodes += nnew;

	return(1);
}

/*
 * ===== ADD A TRI TO THE STRUCTURE ========
 */
int VLcreate_tri     ( h,         /* local storage struct */
		     data,      /* flag to include data or not */
		     points,   /* tri corners coord array */
		     qdata )   /* tri node data */
    OUTmesh *h;
    int  data;
    float *points;
    char *qdata;
{
	int  i, n, nnew;

        nnew = 3;

        if (VLbump_nodes( h, nnew, data ) != 1)
	    ERR_RETURN("could not bump_nodes for tri");

	/*
	 * Set Coordinates
	 */
        for (i=0; i < nnew*h->nspace; i++ ) {
		h->xyz[h->nnodes * h->nspace + i] = points[i];
	}

	/*
	 * Set Node Data
	 */
	if (data) {
            if (VLadd_node_data( h, nnew, qdata ) != 1)
	        ERR_RETURN("could not add_node_data for triangles");
	}

        if ( VLbump_tri_conn ( h ) != 1)
	    ERR_RETURN("could not bump_tri_conn");

	/*
	 * Store Connectivity for one Tri cell.
	 */
	n = h->nnodes;
	h->tri_conn[nnew*h->ntri]   = n;
	h->tri_conn[nnew*h->ntri+1] = n+1;
	h->tri_conn[nnew*h->ntri+2] = n+2;
	(h->ntri)++;
	h->nnodes += nnew;
	return(1);
}


/*
 * ===== ADD A QUAD TO THE STRUCTURE ========
 */
int VLcreate_quad    ( h,         /* local storage struct */
		     data,      /* flag to include data or not */
		     points,   /* quad corners coord array */
		     qdata )   /* quad node data */
    OUTmesh *h;
    int data;
    float *points;
    char *qdata;
{
	int  i, n, nnew;

        nnew = 4;

        if (VLbump_nodes( h, nnew, data ) != 1)
	    ERR_RETURN("could not bump_nodes for quad");

	/*
	 * Set Coordinates
	 */
        for (i=0; i < nnew*h->nspace; i++ ) {
		h->xyz[h->nnodes * h->nspace + i] = points[i];
	}

	/*
	 * Set Node Data
	 */
	if (data) {
            if (VLadd_node_data( h, nnew, qdata ) != 1)
	        ERR_RETURN("could not add_node_data for quads");
	}

        if ( VLbump_quad_conn ( h ) != 1)
	    ERR_RETURN("could not bump_quad_conn");

	/*
	 * Store Connectivity for one Quad cell.
	 */
	n = h->nnodes;
	h->quad_conn[nnew*h->nquad]   = n;
	h->quad_conn[nnew*h->nquad+1] = n+1;
	h->quad_conn[nnew*h->nquad+2] = n+2;
	h->quad_conn[nnew*h->nquad+3] = n+3;
	(h->nquad)++;
	h->nnodes += nnew;

	return(1);
}

/*
 * ===== ADD A POLYLINE TO THE STRUCTURE ========
 */
int VLcreate_polyline( h,        /* internal structure storage */
		     data,     /* control flag, save data or not */
		     points,   /* line coord array */
		     ldata,    /* line node data */
		     numpoints)/* number of points in polyline array */
    OUTmesh *h;
    int data;
    float *points;
    char *ldata;
    int  numpoints;
{
	int  i, n, nnew;

        nnew = 2;

        if (VLbump_nodes( h, numpoints, data ) != 1)
	    ERR_RETURN("could not bump_nodes for polyline");

	/*
	 * Set Coordinates
	 */
        for (i=0; i < numpoints*h->nspace; i++ ) {
		h->xyz[h->nnodes * h->nspace + i] = points[i];
	}

	/*
	 * Set Node Data
	 */
	if (data) {
            if (VLadd_node_data( h, numpoints, ldata ) != 1)
	        ERR_RETURN("could not add_node_data for polylines");
        }

        if ( VLbump_line_conn ( h ) != 1)
	    ERR_RETURN("could not bump_line_conn in polyline");

	/*
	 * Store Connectivity for one line.
	 */
	n = h->nnodes;
	h->line_conn[nnew*h->nline  ] = n;
	h->line_conn[nnew*h->nline+1] = n+numpoints-1;
	(h->nline)++;
	h->nnodes += numpoints;

	return(1);
}


/*
 * ===== PRINT OUT CONTENTS OF THE STRUCTURE ========
 */

VLprintoutstruct( h )

    OUTmesh *h;
{
    int i;

	printf("\ncontents of local struct:\n");
	printf(" h->nnodes    = %d\n", h->nnodes);
	printf(" h->size      = %d\n", h->size);
	printf(" h->nline     = %d\n", h->nline);
	printf(" h->line_size = %d\n", h->line_size);
	printf(" h->ntri      = %d\n", h->ntri);
	printf(" h->tri_size  = %d\n", h->tri_size);
	printf(" h->nquad     = %d\n", h->nquad);
	printf(" h->quad_size = %d\n", h->quad_size);

    for (i=0; i< h->nnodes; i++)
    {
	printf(" h->xyz[%d] = %f %f %f\n",
		 i,
		 h->xyz[3*i+0],
		 h->xyz[3*i+1],
		 h->xyz[3*i+2] );
    }
    for (i=0; i< h->ntri; i++)
    {
	printf(" h->tri_conn[%d] = %d %d %d\n",
		 i,
		 h->tri_conn[3*i+0],
		 h->tri_conn[3*i+1],
		 h->tri_conn[3*i+2] );
    }
	return 1;
}


