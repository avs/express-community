
/*------------------------------------------------------------------------------
 *
 *    TITLE: node_stats_vxp.c
 *  CREATED: 08/01/98
 * MODIFIED: 13/01/98
 *   AUTHOR: Chris Pudney (cpudney@alphapharm.pharm.uwa.edu.au)
 *           Biomedical Confocal Microscopy Research Centre
 *           Department of Pharmacology
 *           The University of Western Australia
 *           Nedlands, WA 6907, Australia
 *
 * Copyright (C) Chris Pudney, The University of Western Australia, 1998.
 * All rights reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation only for the purposes of teaching and research is hereby
 * granted without fee, provided that the above copyright notice and this
 * permission notice appear in all copies of this software/documentation and
 * that you do not sell the software.  No commercial use or distribution
 * of the software is permitted without the consent of the copyright
 * owners.  Commercial licensing is available by contacting the author(s).
 *
 * THIS SOFTWARE/DOCUMENTATION IS PROVIDED WITH NO WARRANTY, EXPRESS OR
 * IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <stdlib.h>

#include "express.h"
#include "data_field.h"
#include "avs_err.h"
#include "node_stats.h"


/*-node_stats_vxp---------------------------------------------------------------
 *  Description: Print out node statistics
 *   Parameters: nstats_id            module id
 *               event_mask           event_mask
 *               seq_num              seq_num
 * Return value: 0/1 on failure/success.
 */

int
node_stats_vxp(OMobj_id nstats_id, OMevent_mask event_mask, int seq_num)
{
  OMobj_id in_field_id;
  char *info;
  FIELD *input;


  /* Read into FIELD structure */
  in_field_id = OMfind_subobj(nstats_id, OMstr_to_name("input"), OM_OBJ_RD);
  input = (FIELD *)DATvxp_to_field(in_field_id);
  if (!input)
    {
      return 0;
    }
  
  
  /* Set output string */
  info = node_stats(input);
  if (!info)
    { 
      ERRverror("node_stats_vxp", ERR_INFO, avsErrStr);
      return 0;
    }
  OMset_name_str_val(nstats_id, OMstr_to_name("info"), info);
  free(info);
  

  /* Clean up and return */
  DATfree_field(input);
  return 1;
}
