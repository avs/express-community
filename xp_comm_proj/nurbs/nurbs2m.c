/*
 * Nurbs Demo, Manchester University
 * Revised from AVS5 to Express by
 * Ian Curington, August, 1995
 *
 * Revision: 13 Oct 1995 ianc - added control point struct
 *           30 Oct 1995 ianc - added ptr object to nurbs_struct
 *                              nurbs to field version only
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
nurbs2m(OMobj_id object, OMevent_mask event_mask, int seq_num)
{
   OMobj_id field_id2, local_id;
   OMobj_id filename_id, nurbs_ptr_id;
   Ui ui;
   int      ndims;
   xp_long dims[3], dims2[3]; 
   double   xd;
   float    *coords, *coords2;
   xp_long      nnodes, nnodes2;
   xp_long      size, size2;
   int stat;

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
   RESOLVE_INT_PARM(object,"show_ctrl",OM_OBJ_RD,ui.nurbs_show_ctrl);

   ndims = nnodes = nnodes2 = size = size2 = stat = 0;

   /*
    * Get id of field (control point lines)
    */
   field_id2 = OMfind_subobj(object, OMstr_to_name("out2"), OM_OBJ_RW);
   if (OMis_null_obj(field_id2)) {
      ERR_RETURN("Error searching for field 2");
   }

   /*
    * get id of nurbs struct pointer object
    */
   nurbs_ptr_id = OMfind_subobj(object, OMstr_to_name("nurbs"), OM_OBJ_RD );
   stat = OMget_ptr_val( nurbs_ptr_id, (void **)&pch, OM_OBJ_RD );

   if( stat == OM_STAT_UNDEF )
   {
       pch = NULL;
   }

   /* Return failure if no valid nurbs input */
   if (pch == NULL) return(0);



    /******************************************/
    /* Control Mesh as output field           */
    /******************************************/

   if ( ui.nurbs_show_ctrl )
   {
       ndims = 2;
       dims2[0] = pch->nu;
       dims2[1] = pch->nv;
       nnodes2 =  dims2[0] * dims2[1];

       if (FLDset_ndim(field_id2,ndims) != 1) {
                   ERR_RETURN("Error setting ndim of output field");
       }
       if (FLDset_dims(field_id2, dims2) != 1) {
               ERR_RETURN("Error setting dims array of node data");
       }
       if (FLDset_nnodes(field_id2, nnodes2) != 1) {
          ERR_RETURN("Error setting nnodes");
       }
       if (FLDset_nspace(field_id2, 3) != 1) {
          ERR_RETURN("Error setting nspace");
       }
       if (FLDget_coord(field_id2, &coords, &size2,
                        OM_GET_ARRAY_WR) != 1) {
          ERR_RETURN("Error setting coord");
       }

       mesh = (Qpoint3 *) malloc((pch->nu) * (pch->nv) * sizeof(Qpoint3));

       /* set cp pointing to control net of the patch */
       cp = (Qpoint4 *) &pch->d[pch->ku + pch->nu + pch->kv + pch->nv];

       for (i = 0; i < pch->nv; i++) {
        for (j = 0; j < pch->nu; j++) {
            k = i * pch->nu + j;
            coords[k*3+0] = cp[k].x/cp[k].w;
            coords[k*3+1] = cp[k].y/cp[k].w;
            coords[k*3+2] = cp[k].z/cp[k].w;
        }
       }

       ARRfree((char *) coords);
       free (mesh);
   }
   else
   {
       dims2[0] = dims2[1] = 0;
       if (FLDset_dims(field_id2, dims2) != 1) {
               ERR_RETURN("Error setting dims array of node data");
       }
       if (FLDset_ndim(field_id2, 0) != 1) {
          ERR_RETURN("Error setting ndim of output field");
       }
       if (FLDset_nnodes(field_id2, 0) != 1) {
          ERR_RETURN("Error resetting nnodes");
       }
   }

   return(1);
}

