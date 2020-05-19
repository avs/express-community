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
int nurbs2f(OMobj_id object, OMevent_mask event_mask, int seq_num)
{
   OMobj_id field_id, field_id2, local_id;
   OMobj_id filename_id, nurbs_ptr_id;
   Ui ui;
   int      ndims;
   xp_long dims[3], dims2[3]; 
   double   xd;
   float    *coords, *coords2;
   xp_long      nnodes, nnodes2;
   xp_long      size, size2;
   int mode,stat;
   Qnurbs_pch *pch=NULL;      /* pointer to a surface */    
   Qpoint4    *cp;            /* pointer to 4D control net of surface */
   Qpoint3    *mesh,          /* resulting mesh of surface tessellation */
              *cpnet;         /* control net of surface */

   int        i, j, k;
   Qfloat     dtu, dtv, ttu, ttv;


   /*
    * Reset all values in ui block
    */

   /* just used for input trigger */
   RESOLVE_INT_PARM(object,"nu",OM_OBJ_RD,ui.nurbs_nu);
   RESOLVE_INT_PARM(object,"nv",OM_OBJ_RD,ui.nurbs_nv);

   ndims = nnodes = nnodes2 = size = size2 = stat = 0;

   /*
    * Get id of field (patch tessellated mesh)
    */
   field_id = OMfind_subobj(object, OMstr_to_name("out"), OM_OBJ_RW);
   if (OMis_null_obj(field_id)) {
      ERR_RETURN("Error searching for field");
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


   ndims = 2;
   dims[0] = ui.nurbs_nv;
   dims[1] = ui.nurbs_nu;
   nnodes =  dims[0] * dims[1];

   /*
    * set dimensions of mesh
    */
   if (FLDset_ndim(field_id,ndims) != 1) {
               ERR_RETURN("Error setting ndim of output field");
   }
#ifdef DBG_MESSAGE
   printf("nurbs: set ndim ok\n");
#endif

   if (FLDset_dims(field_id, dims) != 1) {
           ERR_RETURN("Error setting dims array of node data");
   }
#ifdef DBG_MESSAGE
   printf("nurbs: set dims ok\n");
#endif

   /*
    * Define number of nodes in the field
    */
   if (FLDset_nnodes(field_id, nnodes) != 1) {
      ERR_RETURN("Error setting nnodes");
   }
#ifdef DBG_MESSAGE
   printf("nurbs: set nnodes ok\n");
#endif

   /*
    * Define rspace/nspace of field
    */
   if (FLDset_nspace(field_id, 3) != 1) {
      ERR_RETURN("Error setting nspace");
   }
#ifdef DBG_MESSAGE
   printf("nurbs: set nspace ok\n");
#endif

   /*
    * Define coordinates of field
    */
   if (FLDget_coord(field_id, &coords, &size,
                    OM_GET_ARRAY_WR) != 1) {
      ERR_RETURN("Error setting coord");
   }
#ifdef DBG_MESSAGE
   printf("nurbs: got coord pointer ok\n");
#endif

    /******************************************/
    /* Nurbs File/Evaluation Computation      */
    /******************************************/

    
    /* allocate mesh array */
    mesh = (Qpoint3 *) malloc((ui.nurbs_nu)
			      * (ui.nurbs_nv) * sizeof(Qpoint3));
	#ifdef DBG_MESSAGE
   printf("malloc ok\n");
#endif
    /* tessellate surface to get mesh */
    
    dtu = 1.0 / (ui.nurbs_nu - 1);
    dtv = 1.0 / (ui.nurbs_nv - 1);
    ttu = ttv = 0.0;
    for (i = 0; i < ui.nurbs_nu; i++) {
        for (j = 0; j < ui.nurbs_nv; j++) {
            k = i * ui.nurbs_nv + j;
            nrb_pch_eval(pch, ttu, ttv, 0, &mesh[k]);	   
            ttv += dtv;
        }
        ttu += dtu;
        ttv = 0.0;
    }  
#ifdef DBG_MESSAGE
   printf("loop done ok\n");
#endif
   /*
    * copy over the coord array
    */
   for (i=0; i < nnodes; i++)
   {
       coords[i*3+0] =  mesh[i].x;
       coords[i*3+1] =  mesh[i].y;
       coords[i*3+2] =  mesh[i].z;
       
   }
ARRfree((char *)coords);
      
free(mesh);


#ifdef DBG_MESSAGE
   printf("nurbs: ARRfree on coords ok\n");
#endif

   return(1);
}

