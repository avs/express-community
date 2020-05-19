/*
 * Nurbs Demo, Manchester University
 * Revised from AVS5 to Express by
 * Ian Curington, August, 1995
 *
 * Revision: 13 Oct 1995 ianc - added control point struct
 *           30 Oct 1995 ianc - added ptr object to nurbs_struct
 *                              reader section only
 */

/*-----------------------------------------------------------------------------
    (C)Copyright 1995, The University of Manchester, United Kingdom

    This program is produced by the Computer Graphics Unit, Manchester
    Computing, the Victoria University of Manchester.

    This program has been included for its instructional value. It has been 
    tested with care but is not guaranteed for any particular purpose. Neither
    the authors, nor the University of Manchester offer any warranties or 
    representations, nor do they accept any liabilities with respect to this 
    program. 

    This program must not be used for commmercial gain without the written 
    permission of the authors. 

    Fenqiang Lin, W T Hewitt
    Computer Graphics Unit
    Manchester Computing
    University of Manchester
    Manchester M13 9PL

    e'mail: cgu-info@mcc.ac.uk
    tel:    0161 275 6095
    fax:    0161 275 6040    
    
-----------------------------------------------------------------------------*/


#include <stdio.h>
#include <math.h>

#define XP_WIDE_API

#include "avs/om.h"
#include "avs/err.h"
#include "avs/gd_def.h"

#include "nurbs.h"

/*******************************/
/***  Main Express Entry Pnt ***/
/*******************************/

int
rd_nurbs(OMobj_id object, OMevent_mask event_mask, int seq_num)
{
   OMobj_id field_id, field_id2, local_id;
   OMobj_id filename_id, nurbs_ptr_id;
   Ui ui;
   int      ndims;
   xp_long dims[3], dims2[3]; 
   double   xd;
   float    *coords, *coords2;
   xp_long      nnodes, nnodes2;
   xp_long      size, size2, stat;

   Qnurbs_pch *pch=NULL;      /* pointer to a surface */    
   Qpoint4    *cp;            /* pointer to 4D control net of surface */
   Qpoint3    *mesh,          /* resulting mesh of surface tessellation */
              *cpnet;         /* control net of surface */

   xp_long        i, j, k;
   Qfloat     dtu, dtv, ttu, ttv;


   /*
    * Reset all values in ui block
    */

   /* just used for input trigger */
   RESOLVE_STR_PARM(object,"filename",OM_OBJ_RD,ui.nurbs_filename);
   filename_id = local_id;

   /*
    * get id of nurbs struct pointer object
    */
   nurbs_ptr_id = OMfind_subobj(object, OMstr_to_name("nurbs"), OM_OBJ_RD );
   stat = OMget_ptr_val( nurbs_ptr_id, (void **)&pch, OM_OBJ_RD );

   if( stat == OM_STAT_UNDEF )
   {
       pch = NULL;
   }


    /******************************************/
    /* Nurbs File/Evaluation Computation      */
    /******************************************/

    /* check if filename has changed this time */
    if (OMget_obj_seq(filename_id, OMnull_obj, OM_SEQ_VAL) > seq_num ||
	pch == NULL )
    {
        /* read NURBS patch data and construct a patch */
        pch = (Qnurbs_pch *) nrb_read_pch(ui.nurbs_filename);
    }

    /* Return failure if error happens in reading a NURBS pch */
    if (pch == NULL) return(0);
    
   /*
    * Save nurbs struct ptr for next time
    */
   nurbs_ptr_id = OMfind_subobj(object, OMstr_to_name("nurbs"), OM_OBJ_RW );
   OMset_ptr_val( nurbs_ptr_id, (void *)pch, 0 );

   pch = NULL; /* make sure it is not saved locally */
	printf("at end");
   return(1);
}

