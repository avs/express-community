/*
 * VisitServer.c
 *
 * Th.Eickermann & W.Frings (October 2000)
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

#include <avs/om.h>
#include <avs/om_att.h>
#include <avs/event.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>

#include "xp_comm_proj/visit/gen.h"
#include "visit_srv.h"

#define DEBUG 1
#if DEBUG==2
#define DPRINTF(A) printf A 
#else
#define DPRINTF(A)
#endif

void _Visit_disconnect_cb(visit_srv_connection *vscon, void *internal);
void _Visit_shutdown_cb(visit_srv_connection *vscon, void *internal);

typedef enum {AFTERINIT, AFTERDEINST, READDATA, READDATAREADY, 
	      INTERFACECHANGED,PASSWDCHANGED, SERVICECHANGED, 
	      ALREADYLISTEN, NOTHING} MSG_CODE;
typedef enum {NOLISTEN, LISTENING, CONNECTED} ACTION_CODE;

typedef struct
{
  OMobj_id module_id;
  int vscd;
  int action;
  int lastcode;
  int lastid;
  int num;
  int EVcsdregistered;
  int IdDescriptions_size;
  char  **IdDescriptions;
} VisitServer_internal_t;

void set_status(VisitServer_internal_t *internal) {
  char  *Status = NULL;
  char  newstatus[256];
  int   updateStatus;

  /* Get Listen's value */ 
  if (OMget_name_int_val(internal->module_id, OMstr_to_name("updateStatus"), &updateStatus) != 1) 
    updateStatus = 0;

  if(!updateStatus) return;

  sprintf(newstatus,"%s%s%s  - lastid=%d (%-10s) - #%d - %s%s%s%s%s%s%s%s",
	  ((internal->action==NOLISTEN)   ?"not listening":""),
	  ((internal->action==LISTENING)  ?"listening... ":""),
	  ((internal->action==CONNECTED)  ?"connected    ":""),

	  internal->lastid,
	  (internal->lastid<=internal->IdDescriptions_size) &&
	  (internal->lastid>0)?
	  (
	   (internal->IdDescriptions[internal->lastid-1] != NULL)?
	   internal->IdDescriptions[internal->lastid-1]:"?"
	   ):"?",
	   internal->num,

	  ((internal->lastcode==AFTERINIT)        ?"init ready":""),
	  ((internal->lastcode==AFTERDEINST)      ?"deinstalled":""),
	  ((internal->lastcode==READDATA)         ?"read data...":""),
	  ((internal->lastcode==READDATAREADY)    ?"":""),
	  ((internal->lastcode==INTERFACECHANGED) ?"interf. changed":""),
	  ((internal->lastcode==PASSWDCHANGED)    ?"passwd changed":""),
	  ((internal->lastcode==SERVICECHANGED)   ?"service changed":""),
	  ((internal->lastcode==ALREADYLISTEN)    ?"already listen":"")

	  );
  

  OMset_name_str_val(internal->module_id, OMstr_to_name("Status"), newstatus);
  OMset_name_int_val (internal->module_id, OMstr_to_name ("Action"), internal->action);
  
#if DEBUG==1
  printf("set_status: %s \n",newstatus );
#endif
  DPRINTF(("set_status: %s \n",newstatus ));

}

void VisitServer_read_cb(VisitServer_internal_t *internal);
void VisitServer_connection_cb(VisitServer_internal_t *internal);

/********************************************************************************************/
int VisitServer_inst(OMobj_id VisitServer_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  Port;
   char  *Interface = NULL;
   int  Listen;
   char  *SeapService = NULL;
   char  *SeapPasswd = NULL;

   int    Vscd;
   VisitServer_internal_t *internal;

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get Port's value */ 
   if (OMget_name_int_val(VisitServer_id, OMstr_to_name("Port"), &Port) != 1) 
      Port = 0;

   /* Get Interface's value */
   if (OMget_name_str_val(VisitServer_id, OMstr_to_name("Interface"), &Interface, 0) != 1)
      Interface = NULL;

   /* Get Listen's value */ 
   if (OMget_name_int_val(VisitServer_id, OMstr_to_name("Listen"), &Listen) != 1) 
      Listen = 0;

   /* Get SeapService's value */
   if (OMget_name_str_val(VisitServer_id, OMstr_to_name("SeapService"), &SeapService, 0) != 1)
      SeapService = NULL;

   /* Get SeapPasswd's value */
   if (OMget_name_str_val(VisitServer_id, OMstr_to_name("SeapPasswd"), &SeapPasswd, 0) != 1)
      SeapPasswd = NULL;


   /***********************/
   /* Function's Body     */
   /***********************/

   /* Allocate our internal data structure */
   if( ! (internal = malloc (sizeof *internal)) ) {
     fprintf(stderr,"VisitServer_inst: malloc(internal) failed\n");
     exit(1);
   }

   if(Listen) {
     Vscd = visit_srv_init_socket(SeapService, SeapPasswd, Interface, Port,
				  VISIT_SRV_SEAP_TOGGLE,10000,
				  _Visit_disconnect_cb, internal,
				  _Visit_shutdown_cb, internal);

     /* add our connection handler to the Express event loop */
     DPRINTF(("VisitServer_inst: EVadd_select(%d, VisitServer_connection_cb, %x) Vscd=%d\n",
	      visit_srv_socket_lsd(Vscd),(char *)internal,Vscd));
     EVadd_select (EV_SELECT0,visit_srv_socket_lsd(Vscd),
		   (void (*)(char*)) VisitServer_connection_cb, NULL,
		   (char *)internal, EV_SEL_IN);
   } else {
     Vscd=-1;
   }

  internal->module_id           = VisitServer_id;
  internal->vscd                = Vscd;
  internal->action              = Listen?LISTENING:NOLISTEN;
  internal->lastcode            = AFTERINIT;
  internal->lastid              =  0;
  internal->num                 =  0;
  internal->IdDescriptions      = NULL;
  internal->IdDescriptions_size = -1;

   /***********************/
   /*  Set output values  */
   /***********************/
  
   /* Set Status's value */
   OMset_name_ptr_val (VisitServer_id, OMstr_to_name ("internal"), internal, 0);
   OMset_name_int_val (VisitServer_id, OMstr_to_name ("SockID"), Vscd);

   set_status(internal);


   IdDescriptionsUpdate(VisitServer_id, event_mask, seq_num);

   /*************************/
   /*  Free input variables */
   /*************************/
   if (Interface)
      free(Interface);

   if (SeapService)
      free(SeapService);

   if (SeapPasswd)
      free(SeapPasswd);

   DPRINTF(("VisitServer_inst: leaved \n" ));

   return(1);
}


/********************************************************************************************/
int VisitServer_deinst(OMobj_id VisitServer_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   char  *Status = NULL;
   char  newstatus[256];

   VisitServer_internal_t *internal;

   /***********************/
   /* Function's Body     */
   /***********************/
   OMget_name_ptr_val (VisitServer_id, OMstr_to_name ("internal"),
		       (void**) &internal, 0);
   DPRINTF(("VisitServer_deinst: start %d\n",internal->vscd));
   
   internal->lastcode = AFTERDEINST;
   internal->action   = NOLISTEN;
   set_status(internal);

   if (!internal) return 1;
   visit_srv_shutdown(internal->vscd);
   
   free (internal);

   
   /***********************/
   /*  Set output values  */
   /***********************/
   OMset_name_ptr_val (VisitServer_id, OMstr_to_name ("internal"), 0, 0);

  
   /* Set Status's value */
   OMset_name_str_val(VisitServer_id, OMstr_to_name("Status"), newstatus);
   return(1);
}

/********************************************************************************************/
/*
 * called, whenever there is data available on a data socket
 *
 */
void VisitServer_read_cb(VisitServer_internal_t *internal) {
  int id;

  if (!internal) return;
  if( ! visit_srv_get_id(internal->vscd, &id) ) return;

  /* Set Status's value */
  internal->lastcode   = READDATA;
  internal->lastid     = id;
  internal->num++;
  set_status(internal);

  /* set AVS Trigger variable */
  DPRINTF(("VisitServer_read_cb: before set Trigger (%d)\n",id));
  OMset_name_int_val (internal->module_id, OMstr_to_name ("Trigger"), id);

  DPRINTF(("VisitServer_read_cb: before sending 2nd ACK\n"));
  visit_srv_ack2(internal->vscd);

  /* Set Status's value */
  internal->lastcode   = READDATAREADY;
  /*  set_status(internal); */

  DPRINTF(("VisitServer_read_cb: finished\n"));
}

/*
 * called whenever a socket is ready to be accepted
 *
 */
void VisitServer_connection_cb (VisitServer_internal_t *internal) {

  DPRINTF(("VisitServer_connection_cb: Vscd=%d entered\n",internal->vscd));

  if( ! visit_srv_connect(internal->vscd) ) return;

  /* add our connection handler to the Express event loop */
  DPRINTF(("VisitServer_connection_cb: EVadd_select(%d, VisitServer_read_cb, %x)\n",
	   visit_srv_socket_csd(internal->vscd), (char*)internal));
  EVadd_select (EV_SELECT0, visit_srv_socket_csd(internal->vscd),
		(void (*)(char*)) VisitServer_read_cb,
  		NULL, (char*)internal, EV_SEL_IN);
  internal->action=CONNECTED;
  set_status(internal);

  DPRINTF(("VisitServer_connection_cb: finished\n"));
}


/********************************************************************************************/
int SeapUpdateInterface(OMobj_id VisitServer_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   char  *Interface = NULL;
   char  *Status = NULL;
   char  newstatus[256];

   VisitServer_internal_t *internal;

   /***********************/
   /*  Get input values   */
   /***********************/
   OMget_name_ptr_val (VisitServer_id, OMstr_to_name ("internal"),
		       (void**) &internal, 0);

   /* Get Interface's value */
   if (OMget_name_str_val(VisitServer_id, OMstr_to_name("Interface"), &Interface, 0) != 1)
      Interface = NULL;

   /***********************/
   /* Function's Body     */
   /***********************/
   visit_srv_configure(internal->vscd,VISIT_HOST,Interface);
   

   internal->lastcode=INTERFACECHANGED;
   set_status(internal);

   /*************************/
   /*  Free input variables */
   /*************************/
   if (Interface)
      free(Interface);

   return(1);
}

/********************************************************************************************/
int SeapUpdatePasswd(OMobj_id VisitServer_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   char  *SeapPasswd = NULL;

   VisitServer_internal_t *internal;

   /***********************/
   /*  Get input values   */
   /***********************/
   OMget_name_ptr_val (VisitServer_id, OMstr_to_name ("internal"),
		       (void**) &internal, 0);

   /* Get SeapPasswd's value */
   if (OMget_name_str_val(VisitServer_id, OMstr_to_name("SeapPasswd"), &SeapPasswd, 0) != 1)
      SeapPasswd = NULL;

   /***********************/
   /* Function's Body     */
   /***********************/
   visit_srv_configure(internal->vscd,VISIT_PASSWD,SeapPasswd);

   internal->lastcode=PASSWDCHANGED;
   set_status(internal);

   /*************************/
   /*  Free input variables */
   /*************************/
   if (SeapPasswd)
      free(SeapPasswd);

   return(1);
}

/********************************************************************************************/
int SeapUpdateService(OMobj_id VisitServer_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   char  *SeapService = NULL;

   VisitServer_internal_t *internal;

   /***********************/
   /*  Get input values   */
   /***********************/
   OMget_name_ptr_val (VisitServer_id, OMstr_to_name ("internal"),
		       (void**) &internal, 0);

   /* Get SeapService's value */
   if (OMget_name_str_val(VisitServer_id, OMstr_to_name("SeapService"), &SeapService, 0) != 1)
      SeapService = NULL;

   /***********************/
   /* Function's Body     */
   /***********************/
   visit_srv_configure(internal->vscd,VISIT_SERVICE,SeapService);

   internal->lastcode=SERVICECHANGED;
   set_status(internal);

   /*************************/
   /*  Free input variables */
   /*************************/
   if (SeapService)
      free(SeapService);

   return(1);
}

/********************************************************************************************/
int ListenUpdate(OMobj_id VisitServer_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   int  Listen;
   int  Port;
   char  *Interface = NULL;
   char  *SeapService = NULL;
   char  *SeapPasswd = NULL;
   char  *Status = NULL;
   char  newstatus[256];

   int    Vscd;
   VisitServer_internal_t *internal;

   /***********************/
   /*  Get input values   */
   /***********************/

   OMget_name_ptr_val (VisitServer_id, OMstr_to_name ("internal"),
		       (void**) &internal, 0);

   /* Get Port's value */ 
   if (OMget_name_int_val(VisitServer_id, OMstr_to_name("Port"), &Port) != 1) 
      Port = 0;

   /* Get Interface's value */
   if (OMget_name_str_val(VisitServer_id, OMstr_to_name("Interface"), &Interface, 0) != 1)
      Interface = NULL;

   /* Get Listen's value */ 
   if (OMget_name_int_val(VisitServer_id, OMstr_to_name("Listen"), &Listen) != 1) 
      Listen = 0;

   /* Get SeapService's value */
   if (OMget_name_str_val(VisitServer_id, OMstr_to_name("SeapService"), &SeapService, 0) != 1)
      SeapService = NULL;

   /* Get SeapPasswd's value */
   if (OMget_name_str_val(VisitServer_id, OMstr_to_name("SeapPasswd"), &SeapPasswd, 0) != 1)
      SeapPasswd = NULL;

   /* Get Listen's value */ 
   if (OMget_name_int_val(VisitServer_id, OMstr_to_name("Listen"), &Listen) != 1) 
      Listen = 0;


   /***********************/
   /* Function's Body     */
   /***********************/
   if(Listen) {
     if((internal->action==LISTENING) || (internal->action==CONNECTED)) {
       printf("ListenUpdate: already listening ...\n");
       internal->lastcode=ALREADYLISTEN;
       set_status(internal);
     } else {
       Vscd = visit_srv_init_socket(SeapService, SeapPasswd, Interface, Port,
				    VISIT_SRV_SEAP_TOGGLE,10000,
				    _Visit_disconnect_cb, internal,
				    _Visit_shutdown_cb, internal);
       
       /* add our connection handler to the Express event loop */
       DPRINTF(("ListenUpdate: EVadd_select(%d, VisitServer_connection_cb, %x) Vscd=%d\n",
		visit_srv_socket_lsd(Vscd),(char *)internal,Vscd));
       EVadd_select (EV_SELECT0,visit_srv_socket_lsd(Vscd),
		     (void (*)(char*)) VisitServer_connection_cb, NULL,
		     (char *)internal, EV_SEL_IN);
       OMset_name_int_val (VisitServer_id, OMstr_to_name ("SockID"), Vscd);
       internal->vscd = Vscd;

       internal->action  =LISTENING;
       internal->lastcode=NOTHING;
       internal->num        =  0;
       set_status(internal);
     }
   } else {
     if ((internal->action==LISTENING) || (internal->action==CONNECTED)) {

       visit_srv_shutdown(internal->vscd);
       Vscd=0;
       internal->vscd = Vscd;

       internal->lastcode=NOLISTEN;
       internal->lastcode=NOTHING;
       set_status(internal);
     }
   }
   DPRINTF(("ListenUpdate: leaved\n"));

   return(1);
}



/********************************************************************************************/
void _Visit_disconnect_cb(visit_srv_connection *vscon, void *internal) {
  char  newstatus[256];
  VisitServer_internal_t *linternal= (VisitServer_internal_t *) internal;

  DPRINTF(("_Visit_disconnect_cb: entered\n"));

  DPRINTF(("_Visit_disconnect_cb: EVdel_select(%d, VisitServer_read_cb, %x)\n",
	   visit_srv_socket_csd(linternal->vscd),(char *)internal));
  EVdel_select(EV_SELECT0, visit_srv_socket_csd(linternal->vscd),
	       (void (*)(char *))VisitServer_read_cb,
	       NULL, (char*)internal, EV_SEL_IN);

  linternal->action=LISTENING;
  set_status(linternal);

  DPRINTF(("_Visit_disconnect_cb: leaved\n"));
}


/********************************************************************************************/
/*
 * called by visit_srv_shutdown (when the server is explicitly shut down)
 *
 */
void _Visit_shutdown_cb(visit_srv_connection *vscon, void *internal) {
  char  newstatus[256];
  VisitServer_internal_t *linternal= (VisitServer_internal_t *) internal;

  DPRINTF(("_Visit_shutdown_cb: entered\n"));

  DPRINTF(("_Visit_shutdown_cb: EVdel_select(%d, VisitServer_connection_cb, %x)\n",
	   visit_srv_socket_lsd(linternal->vscd), (char *)internal));
  EVdel_select (EV_SELECT0, visit_srv_socket_lsd(linternal->vscd),
		(void (*)(char*)) VisitServer_connection_cb, NULL,
		(char *)internal, EV_SEL_IN);

  linternal->action=NOLISTEN;
  set_status(linternal);

  DPRINTF(("_Visit_shutdown_cb: leaved\n"));

}




int IdDescriptionsUpdate(OMobj_id VisitServer_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   char  **IdDescriptions = NULL;
   OMobj_id IdDescriptions_id;
   int  IdDescriptions_count;
   VisitServer_internal_t *internal;

   OMget_name_ptr_val (VisitServer_id, OMstr_to_name ("internal"),
		       (void**) &internal, 0);

   /*************************/
   /*  Free input variables */
   /*************************/
   if (internal->IdDescriptions != NULL) {
     DPRINTF(("IdDescriptionsUpdate: free ...\n"));
      for (IdDescriptions_count = 0; IdDescriptions_count < internal->IdDescriptions_size; IdDescriptions_count++) {
         if (internal->IdDescriptions[IdDescriptions_count] != NULL)
            free((char *)internal->IdDescriptions[IdDescriptions_count]);
      }
      free((char *)internal->IdDescriptions);
   }

   /***********************/
   /*  Get input values   */
   /***********************/
   /* Get string array IdDescriptions */
   IdDescriptions_id = OMfind_subobj(VisitServer_id, OMstr_to_name("IdDescriptions"), OM_OBJ_RD);


   /* Get the number of strings */
   if(!OMis_null_obj(IdDescriptions_id)) {
     DPRINTF(("IdDescriptionsUpdate: get new strings ...\n"));
     if (OMget_array_size(IdDescriptions_id, &internal->IdDescriptions_size) != 1 || internal->IdDescriptions_size == 0)
       internal->IdDescriptions = NULL;
     else 
       internal->IdDescriptions = (char **)malloc(internal->IdDescriptions_size * sizeof(char *));
     for (IdDescriptions_count = 0; IdDescriptions_count < internal->IdDescriptions_size; IdDescriptions_count++) {
       if (OMget_str_array_val(IdDescriptions_id, IdDescriptions_count, 
			       &(internal->IdDescriptions[IdDescriptions_count]), 0) != 1) {
	 internal->IdDescriptions[IdDescriptions_count] = NULL;
       }else {
	 DPRINTF(("IdDescriptionsUpdate: %d => %s\n",IdDescriptions_count,internal->IdDescriptions[IdDescriptions_count]));
       }
     } 
   }

   return(1);
}
