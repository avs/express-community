/*
 * VisitWriter.c
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

#include "xp_comm_proj/visit/gen.h"

#include "visit.h"

/*  #define DEBUG */

#ifdef DEBUG
#define DPRINTF(A) printf A 
#else
#define DPRINTF(A)
#endif


int VisitWriter_read(OMobj_id VisitWriter_id, OMevent_mask event_mask,
		     int seq_num)
{
  /***********************/
  /*  Declare variables  */
  /***********************/
  int  Vscd;
  int  Trigger;
  double  TimeStamp;
  int  n1,n2,n3,n4;
  int  o1,o2,o3,o4;
  int  s1,s2,s3,s4;
  int  DataInt_size = 0;
  int *DataInt = NULL; 
  int  DataShort_size = 0;
  short *DataShort = NULL; 
  int  DataByte_size = 0;
  unsigned char *DataByte = NULL; 
  int  DataDouble_size = 0;
  double *DataDouble = NULL; 
  char  *DataString = NULL;
  int  DataIntScalar;
  int  DataShortDummy;
  short  DataShortScalar;
  int  DataByteDummy;
  unsigned char DataByteScalar;
  double  DataDoubleScalar;

  int size,esize;
  void *ptr;
  visit_request request;


  /***********************/
  /*  Get input values   */
  /***********************/
  
  /* Get SockID's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("SockID"),
			 &Vscd) != 1) 
    Vscd = 0;
  
  /* Get Trigger's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("Trigger"),
			 &Trigger) != 1) 
    Trigger = 0;
  
  /* Get TimeStamp's value */
  if (OMget_name_real_val(VisitWriter_id, OMstr_to_name("TimeStamp"),
			  &TimeStamp) != 1)
    TimeStamp = 0.0;
  
  /* Get n1's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("n1"), &n1) != 1) 
    n1 = 0;
  
  /* Get n2's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("n2"), &n2) != 1) 
    n2 = 0;
  
  /* Get n3's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("n3"), &n3) != 1) 
    n3 = 0;

  /* Get n4's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("n4"), &n4) != 1) 
    n4 = 0;

  /* Get o1's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("o1"), &o1) != 1) 
    o1 = 0;
  
  /* Get n2's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("o2"), &o2) != 1) 
    o2 = 0;
  
  /* Get n3's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("o3"), &o3) != 1) 
    o3 = 0;

  /* Get n4's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("o4"), &o4) != 1) 
    o4 = 0;

  /* Get o1's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("s1"), &s1) != 1) 
    s1 = 0;
  
  /* Get n2's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("s2"), &s2) != 1) 
    s2 = 0;
  
  /* Get n3's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("s3"), &s3) != 1) 
    s3 = 0;

  /* Get n4's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("s4"), &s4) != 1) 
    s4 = 0;
   
  DataInt = (int *)OMret_name_array_ptr(VisitWriter_id,
		   OMstr_to_name("DataInt"), OM_GET_ARRAY_RD,
					&DataInt_size, NULL);

  DataShort = (short *)OMret_name_array_ptr(VisitWriter_id,
		   OMstr_to_name("DataShort"), OM_GET_ARRAY_RD,
					&DataShort_size, NULL);

  DataByte = (unsigned char *)OMret_name_array_ptr(VisitWriter_id,
		   OMstr_to_name("DataByte"), OM_GET_ARRAY_RD,
					&DataByte_size, NULL);
   
  DataDouble = (double *)OMret_name_array_ptr(VisitWriter_id,
			 OMstr_to_name("DataDouble"), OM_GET_ARRAY_RD,
					      &DataDouble_size, NULL);

  /* Get DataString's value */
  if (OMget_name_str_val(VisitWriter_id, OMstr_to_name("DataString"),
			 &DataString, 0) != 1)
    DataString = NULL;
  
  /* Get DataIntScalar's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("DataIntScalar"),
			 &DataIntScalar) != 1) 
    DataIntScalar = 0;

  /* Get DataShortScalar's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("DataShortScalar"),
			 &DataShortDummy) != 1) 
    DataShortScalar = 0;
  else DataShortScalar = (short) DataShortDummy;

  /* Get DataByteScalar's value */ 
  if (OMget_name_int_val(VisitWriter_id, OMstr_to_name("DataByteScalar"),
			 &DataByteDummy) != 1) 
    DataByteScalar = 0;
  else DataByteScalar = (unsigned char) DataByteDummy;

  /* Get DataDoubleScalar's value */
  if (OMget_name_real_val(VisitWriter_id, OMstr_to_name("DataDoubleScalar"),
			  &DataDoubleScalar) != 1)
    DataDoubleScalar = 0.0;
  

  /***********************/
  /* Function's Body     */
  /***********************/

  if( (size=visit_srv_get_request(Vscd,&request)) < 0) return 0;

  if( request.vtype == VISIT_STRING ) {
    esize = 1;
  } else {
    esize = size / visit_sizeof(request.vtype);
  }

  request.timestamp = TimeStamp;

  DPRINTF(("VisitWriter_read: id = %2d  size = %8d t=%10.3f %dx%dx%dx%d\n",
	   request.id,size,request.timestamp,request.n1,request.n2,request.n3,request.n4));

  switch(request.vtype) {

  case VISIT_STRING: 
    request.n1 = (strlen(DataString)+1);
    request.n2=1;
    request.n3=1;
    request.n4=1;
    ptr = DataString;
    break;
  case VISIT_BYTE: 
    if(esize==1) {
      /* it's a scalar */
      ptr = &DataByteScalar;
    } else {
      /* it's a field */
      ptr = DataByte;
      if (DataByte_size <= esize) {
	if(n1!=-1 || (n2!=-1) || (n3!=-1) || (n4!=-1) ) {
	  if( (n1*n2*n3*n4)==DataByte_size) {
	    request.n1=n1;
	    request.n2=n2;
	    request.n3=n3;
	    request.n4=n4;
	  } else {
	    DPRINTF(("VisitWriter_read: request size = %d %d %d %d  DataByte_size = %d\n",n1,n2,n3,n4,DataByte_size));
	    perror("VisitWriter_read size of array <> n1*n2*n3*n4:");
	    return 0;
	  }
	} else { 
	  if (DataByte_size<esize) {
	    /* there is no information about the real dimension,
	     * reduce to a 1-D vector */
	    request.n1=DataByte_size;
	    request.n2=1;
	    request.n3=1;
	    request.n4=1;
	  }
	}
      }
      else {
	perror("VisitWriter_read send DataByte field too big:");
	return 0;
      }
    }
    break;
  case VISIT_INT16: 
    if(esize==1) {
      /* it's a scalar */
      ptr = &DataShortScalar;
    } else {
      /* it's a field */
      ptr = DataShort;
      if (DataShort_size <= esize) {
	if(n1!=-1 || (n2!=-1) || (n3!=-1) || (n4!=-1) ) {
	  if( (n1*n2*n3*n4)==DataShort_size) {
	    request.n1=n1;
	    request.n2=n2;
	    request.n3=n3;
	    request.n4=n4;
	  } else {
	    DPRINTF(("VisitWriter_read: request size = %d %d %d %d  DataShort_size = %d\n",n1,n2,n3,n4,DataShort_size));
	    perror("VisitWriter_read size of array <> n1*n2*n3*n4:");
	    return 0;
	  }
	} else { 
	  if (DataShort_size<esize) {
	    /* there is no information about the real dimension,
	     * reduce to a 1-D vector */
	    request.n1=DataShort_size;
	    request.n2=1;
	    request.n3=1;
	    request.n4=1;
	  }
	}
      }
      else {
	perror("VisitWriter_read send DataShort field too big:");
	return 0;
      }
    }
    break;
  case VISIT_INT32: 
    if(esize==1) {
      /* it's a scalar */
      ptr = &DataIntScalar;
    } else {
      /* it's a field */
      ptr = DataInt;
      if (DataInt_size <= esize) {
	if(n1!=-1 || (n2!=-1) || (n3!=-1) || (n4!=-1) ) {
	  if( (n1*n2*n3*n4)==DataInt_size) {
	    request.n1=n1;
	    request.n2=n2;
	    request.n3=n3;
	    request.n4=n4;
	  } else {
	    DPRINTF(("VisitWriter_read: request size = %d %d %d %d  DataInt_size = %d\n",n1,n2,n3,n4,DataInt_size));
	    perror("VisitWriter_read size of array <> n1*n2*n3*n4:");
	    return 0;
	  }
	} else { 
	  if (DataInt_size<esize) {
	    /* there is no information about the real dimension,
	     * reduce to a 1-D vector */
	    request.n1=DataInt_size;
	    request.n2=1;
	    request.n3=1;
	    request.n4=1;
	  }
	}
      }
      else {
	perror("VisitWriter_read send DataInt field too big:");
	return 0;
      }
    }
    break;
  case VISIT_FLOAT64: 
    if(esize==1) {
      /* it's a scalar */
      ptr = &DataDoubleScalar;
    } else {
      /* it's a field */
      ptr = DataDouble;
      if (DataDouble_size <= esize) {
	if(n1!=-1 || (n2!=-1) || (n3!=-1) || (n4!=-1) ) {
	  if(n1*n2*n3*n4==esize) {
	    request.n1=n1;
	    request.n2=n2;
	    request.n3=n3;
	    request.n4=n4;
	  } else {
	    perror("VisitWriter_read size of array <> n1*n2*n3*n4:");
	    return 0;
	  }
	} else {
	  if (DataDouble_size<esize) {
	    /* there is no information about the real dimension,
	     * reduce to a 1-D vector */
	    request.n1=DataDouble_size;
	    request.n2=1;
	    request.n3=1;
	    request.n4=1;
	  }
	}
      } else {
	perror("VisitWriter_read send DataDouble field too big:");
	return 0;
      }
    }
    break;
  default:
    fprintf(stderr,"VisitWriter_read: unknown datatype\n");
    return 0;
  }
  request.s1=s1;
  request.s2=s2;
  request.s3=s3;
  request.s4=s4;
  request.o1=o1;
  request.o2=o2;
  request.o3=o3;
  request.o4=o4;

  /* do the send operation */
  visit_srv_write_data(Vscd, ptr, &request);

  /*************************/
  /*  Free input variables */
  /*************************/
  if (DataInt != NULL) 
    ARRfree(DataInt);

  if (DataByte != NULL) 
    ARRfree(DataByte);

  if (DataShort != NULL) 
    ARRfree(DataShort);
  
  if (DataDouble != NULL) 
      ARRfree(DataDouble);
  
  if (DataString)
    free(DataString);
  
  DPRINTF(("VisitWriter_read: finished\n"));
  
   return(1);
}
