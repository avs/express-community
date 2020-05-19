/*
 * Multiplexer.c
 *
 * Th.Eickermann & W.Frings (October 1999)
 *
 *                  Visit - Visualization Interface Toolkit 
 *                  Seap  - Service Announcement Protocol
 *
 *   Copyright (C) 2000, Forschungszentrum Juelich GmbH, Federal Republic of
 *   Germany. All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *     - Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     - Any publications that result from the use of this software shall
 *       reasonably refer to the Research Centre's development.
 *
 *     - All advertising materials mentioning features or use of this software
 *       must display the following acknowledgement:
 *
 *           This product includes software developed by Forschungszentrum
 *           Juelich GmbH, Federal Republic of Germany.
 *
 *     - Forschungszentrum Juelich GmbH is not obligated to provide the user with
 *       any support, consulting, training or assistance of any kind with regard
 *       to the use, operation and performance of this software or to provide
 *       the user with any updates, revisions or new versions.
 *
 *
 *   THIS SOFTWARE IS PROVIDED BY FORSCHUNGSZENTRUM JUELICH GMBH "AS IS" AND ANY
 *   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL FORSCHUNGSZENTRUM JUELICH GMBH BE LIABLE FOR
 *   ANY SPECIAL, DIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 *   RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 *   CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 *   CONNECTION WITH THE ACCESS, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "xp_comm_proj/visit/gen.h"

#define MAX_OUTVAL 10

int
Multiplex(OMobj_id Muliplexer_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  Inval;
   int  Out;
   char Ostr[6];

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get Inval's value */ 
   if (OMget_name_int_val(Muliplexer_id, OMstr_to_name("Inval"), &Inval) != 1) 
      Inval = 0;


   /***********************/
   /* Function's Body     */
   /***********************/
   /*    ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: Multiplex generated from method: Muliplexer.Multiplex\n"); */

   if( (Inval < 1) || (Inval > MAX_OUTVAL ) ) return 0;


   /***********************/
   /*  Set output values  */
   /***********************/
   sprintf(Ostr, "Out%d", Inval);
   /* Set Out??'s value */  
   OMset_name_int_val(Muliplexer_id, OMstr_to_name(Ostr), Inval);
   
   return(1);


}
 
