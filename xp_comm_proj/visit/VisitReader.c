/*
 * VisitReader.c
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

/*  #define DEBUG  */

#ifdef DEBUG
#define DPRINTF(A) printf A 
#else
#define DPRINTF(A)
#endif


int VisitReader_read(OMobj_id VisitReader_id, OMevent_mask event_mask, int seq_num)
{
 
  /***********************/
  /*  Declare variables  */
  /***********************/
  int    Vscd,Trigger;
  int    size,esize,i,DataInt_size,DataByte_size,DataShort_size,DataDouble_size;
  int    *DataInt;
  short  *DataShort;
  unsigned char  *DataByte;
  double *DataDouble;
  char   *DataString; 
  double sum;


  visit_request request;

  /***********************/
  /*  Get input values   */
  /***********************/

  /* Get SockID's value */ 
  if (OMget_name_int_val(VisitReader_id, OMstr_to_name("SockID"),
			 &Vscd) != 1) 
    Vscd = -1;
  
  if( Vscd < 0 ) return 0;

  /* Get Trigger's value */ 
  if (OMget_name_int_val(VisitReader_id, OMstr_to_name("Trigger"),
			 &Trigger) != 1) 
    Trigger = 0;
  
  
  /***********************/
  /* Function's Body     */
  /***********************/


  DPRINTF(("VisitReader_read: read dims Vscd=%d\n",Vscd));
  if( (size=visit_srv_get_request(Vscd,&request)) < 0) return 0;

  OMset_name_int_val(VisitReader_id, OMstr_to_name("n1"), request.n1);
  OMset_name_int_val(VisitReader_id, OMstr_to_name("n2"), request.n2);
  OMset_name_int_val(VisitReader_id, OMstr_to_name("n3"), request.n3);
  OMset_name_int_val(VisitReader_id, OMstr_to_name("n4"), request.n4);
  OMset_name_real_val(VisitReader_id, OMstr_to_name("TimeStamp"),
		      (float)request.timestamp);

  DPRINTF(("VisitReader_read: id = %2d  size = %8d t=%10.3f %dx%dx%dx%d\n",
	 request.id,size,request.timestamp,request.n1,request.n2,request.n3,request.n4));

  switch( request.vtype ) {
  case VISIT_STRING:
    DataString=(char *) malloc(size*sizeof(char));

    /* read the data */
    if( ! visit_srv_read_data(Vscd, DataString, &request)) return 0;
    
    OMset_name_str_val(VisitReader_id, OMstr_to_name("DataString"), DataString);

    DPRINTF(("VisitReader_read: before free(DataString)\n"));
    free(DataString);
    break;
  case VISIT_INT32:
    /* get space */
    esize = size / visit_sizeof(request.vtype);
    DPRINTF(("VisitReader_read: setting DataIntSize variable to %d\n",esize));
    OMset_name_int_val(VisitReader_id, OMstr_to_name("DataIntSize"),esize);

    if( (DataInt = (int *)OMret_name_array_ptr(VisitReader_id,
			  OMstr_to_name("DataInt"), OM_GET_ARRAY_RW,
			  &DataInt_size, NULL) ) == NULL) {
      fprintf(stderr,"VisitReader_read: did not get valid DataInt pointer\n");
      return 0;
    }
    /* read the data */
    if( ! visit_srv_read_data(Vscd, DataInt, &request)) return 0;

#ifdef DEBUG2
    for(i=0;i<((esize<10)?esize:10);i++) {
      printf("VISIT_INT32: %2d: %d\n",i,DataInt[i]);
    }
#endif
    DPRINTF(("VisitReader_read: before ARRfree(DataInt)\n"));
    ARRfree(DataInt);

    if(esize==1) {
      OMset_name_int_val(VisitReader_id, OMstr_to_name("DataIntScalar"),
			 DataInt[0]);
      DPRINTF(("VisitReader_read: after set DataIntScalar\n"));
    }
    break;
  case VISIT_INT16:
    /* get space */
    esize = size / visit_sizeof(request.vtype);
    DPRINTF(("VisitReader_read: setting DataShortSize variable to %d\n",esize));
    OMset_name_int_val(VisitReader_id, OMstr_to_name("DataShortSize"),esize);

    if( (DataShort = (short *)OMret_name_array_ptr(VisitReader_id,
			  OMstr_to_name("DataShort"), OM_GET_ARRAY_RW,
			  &DataShort_size, NULL) ) == NULL) {
      fprintf(stderr,"VisitReader_read: did not get valid DataShort pointer\n");
      return 0;
    }
    /* read the data */
    if( ! visit_srv_read_data(Vscd, DataShort, &request)) return 0;

#ifdef DEBUG2
    for(i=0;i<((esize<10)?esize:10);i++) {
      printf("VISIT_INT16: %2d: %d\n",i,DataShort[i]);
    }
#endif
    DPRINTF(("VisitReader_read: before ARRfree(DataShort)\n"));
    ARRfree(DataShort);

    if(esize==1) {
      OMset_name_int_val(VisitReader_id, OMstr_to_name("DataShortScalar"),
			 DataShort[0]);
      DPRINTF(("VisitReader_read: after set DataShortScalar\n"));
    }
    break;
  case VISIT_BYTE:
    /* get space */
    esize = size / visit_sizeof(request.vtype);
    DPRINTF(("VisitReader_read: setting DataByteSize variable to %d\n",esize));
    OMset_name_int_val(VisitReader_id, OMstr_to_name("DataByteSize"),esize);

    if( (DataByte = (unsigned char *)OMret_name_array_ptr(VisitReader_id,
			  OMstr_to_name("DataByte"), OM_GET_ARRAY_RW,
			  &DataByte_size, NULL) ) == NULL) {
      fprintf(stderr,"VisitReader_read: did not get valid DataByte pointer\n");
      return 0;
    }
    /* read the data */
    if( ! visit_srv_read_data(Vscd, DataByte, &request)) return 0;

#ifdef DEBUG2
    for(i=0;i<((esize<10)?esize:10);i++) {
      printf("VISIT_BYTE: %2d: %d\n",i,DataByte[i]);
    }
#endif
    DPRINTF(("VisitReader_read: before ARRfree(DataByte)\n"));
    ARRfree(DataByte);

    if(esize==1) {
      OMset_name_int_val(VisitReader_id, OMstr_to_name("DataByteScalar"),
			 (int) (DataByte[0]));
      DPRINTF(("VisitReader_read: after set DataByteScalar\n"));
    }
    break;
  case VISIT_FLOAT64:
    /* get space */
    esize = size / visit_sizeof(request.vtype);
    DPRINTF(("VisitReader_read: setting DataDoubleSize variable to %d\n",esize));
    OMset_name_int_val(VisitReader_id, OMstr_to_name("DataDoubleSize"),esize);
  
    if( (DataDouble = (double *)OMret_name_array_ptr(VisitReader_id,
				OMstr_to_name("DataDouble"), OM_GET_ARRAY_RW,
				&DataDouble_size, NULL) ) == NULL) {
      fprintf(stderr,"VisitReader_read: did not get valid DataDouble pointer\n");
      return 0;
    }

    /* read the data */
    if( ! visit_srv_read_data(Vscd, DataDouble, &request)) return 0;

#ifdef DEBUG2
    for(i=0;i<((esize<10)?esize:10);i++) {
      printf("VISIT_FLOAT64: %2d: %10.4f\n",i,DataDouble[i]);
    }
    if(esize>10) {
      for(i=esize-10;i<esize;i++) {
	 printf("VISIT_FLOAT64: %2d: %10.4f\n",i,DataDouble[i]);
      }
    }
    sum=0.0;
    for(i=0;i<esize;i++) {
      sum+=DataDouble[i];
    }
    printf("VISIT_FLOAT64: sum = %f\n",sum);
#endif
    DPRINTF(("VisitReader_read: before ARRfree(DataDouble)\n"));
    ARRfree(DataDouble);
    
    if(esize==1) {
      OMset_name_real_val(VisitReader_id, OMstr_to_name("DataDoubleScalar"),
			  (float)DataDouble[0]);
      DPRINTF(("VisitReader_read: after set DataDoubleScalar\n"));
    }
    break;
  default:
    fprintf(stderr,"VisitReader_read: unsupported datatype %d\n",request.vtype);
    return 0;
  }

  OMset_name_int_val(VisitReader_id, OMstr_to_name("o1"), request.s1);
  OMset_name_int_val(VisitReader_id, OMstr_to_name("o2"), request.s2);
  OMset_name_int_val(VisitReader_id, OMstr_to_name("o3"), request.s3);
  OMset_name_int_val(VisitReader_id, OMstr_to_name("o4"), request.s4);
  OMset_name_int_val(VisitReader_id, OMstr_to_name("s1"), request.o1);
  OMset_name_int_val(VisitReader_id, OMstr_to_name("s2"), request.o2);
  OMset_name_int_val(VisitReader_id, OMstr_to_name("s3"), request.o3);
  OMset_name_int_val(VisitReader_id, OMstr_to_name("s4"), request.o4);

  DPRINTF(("VisitReader_read: finished\n"));
  
  return(1);
}


int
VisitReader_inst(OMobj_id VisitReader_id, OMevent_mask event_mask, int seq_num)
{
   /***********************/
   /*  Declare variables  */
   /***********************/
   double  TimeStamp;
   int  n1;
   int  n2;
   int  n3;
   int  n4;
   int  DataIntSize;
   int  DataInt_size = 0;
   int *DataInt = NULL; 
   int  DataByteSize;
   int  DataByte_size = 0;
   unsigned char *DataByte = NULL; 
   int  DataShortSize;
   int  DataShort_size = 0;
   short *DataShort = NULL; 
   int  DataDoubleSize;
   int  DataDouble_size = 0;
   double *DataDouble = NULL; 
   char  *DataString = NULL;
   int  DataIntScalar;
   short  DataShortScalar;
   unsigned char  DataByteScalar;
   double  DataDoubleScalar;

   int i;

   /***********************/
   /* Function's Body     */
   /***********************/
   /*    ERRverror("",ERR_NO_HEADER | ERR_INFO,"I'm in function: VisitReader_inst generated from method: VisitReader.VisitReader_inst\n"); */


   /***********************/
   /*  Set output values  */
   /***********************/


   /* Set TimeStamp's value */
   TimeStamp=0.0;
   OMset_name_real_val(VisitReader_id, OMstr_to_name("TimeStamp"), TimeStamp);


   /* Get n1's value */ 
   if (OMget_name_int_val(VisitReader_id, OMstr_to_name("n1"), &n1) != 1) 
      n1 = 0;

   /* Get n2's value */ 
   if (OMget_name_int_val(VisitReader_id, OMstr_to_name("n2"), &n2) != 1) 
      n2 = 0;

   /* Get n3's value */ 
   if (OMget_name_int_val(VisitReader_id, OMstr_to_name("n3"), &n3) != 1) 
      n3 = 0;

   /* Get n4's value */ 
   if (OMget_name_int_val(VisitReader_id, OMstr_to_name("n4"), &n4) != 1) 
      n4 = 0;

   if ( ( (n1==-1) && (n2==-1) && (n3==-1) && (n4==-1) ) || (n1*n2*n3*n4<=0) ) {
     DPRINTF(("VisitReader_inst: set initial dims: %d %d %d %d -> 3 3 3 1\n",n1,n2,n3,n4));
     n1=3;
     n2=3;
     n3=3;
     n4=1;
   } else {
     DPRINTF(("VisitReader_inst: use initial dims: %d %d %d %d\n",n1,n2,n3,n4));
    }

   /* Set DataIntSize's value */  
   DataIntSize=n1*n2*n3*n4;
   OMset_name_int_val(VisitReader_id, OMstr_to_name("DataIntSize"), DataIntSize);
   DataInt = (int *)OMret_name_array_ptr(VisitReader_id,
					 OMstr_to_name("DataInt"), OM_GET_ARRAY_RW,
					 &DataInt_size, NULL);
   for(i=0;i<DataIntSize;i++) DataInt[i]=4;
   ARRfree(DataInt);
 
   /* Set DataShortSize's value */  
   DataShortSize=n1*n2*n3*n4;
   OMset_name_int_val(VisitReader_id, OMstr_to_name("DataShortSize"), DataShortSize);
   DataShort = (short *)OMret_name_array_ptr(VisitReader_id,
					 OMstr_to_name("DataShort"), OM_GET_ARRAY_RW,
					 &DataShort_size, NULL);
   for(i=0;i<DataShortSize;i++) DataShort[i]=4;
   ARRfree(DataShort);

   /* Set DataByteSize's value */  
   DataByteSize=n1*n2*n3*n4;
   OMset_name_int_val(VisitReader_id, OMstr_to_name("DataByteSize"), DataByteSize);
   DataByte = (unsigned char *)OMret_name_array_ptr(VisitReader_id,
					 OMstr_to_name("DataByte"), OM_GET_ARRAY_RW,
					 &DataByte_size, NULL);
   for(i=0;i<DataByteSize;i++) DataByte[i]=4;
   ARRfree(DataByte);
 

   /* Set DataDoubleSize's value */  
   DataDoubleSize=n1*n2*n3*n4;
   OMset_name_int_val(VisitReader_id, OMstr_to_name("DataDoubleSize"), DataDoubleSize);
   DataDouble = (double *)OMret_name_array_ptr(VisitReader_id,
					       OMstr_to_name("DataDouble"), OM_GET_ARRAY_RW,
					       &DataDouble_size, NULL);
   DPRINTF(("VisitReader_inst: use initial dims: get size %d\n",DataDouble_size));
   for(i=0;i<DataDoubleSize;i++) DataDouble[i]=3;
   ARRfree(DataDouble);
   

   /* Set DataString's value */
   DataString= (char *) malloc(5*sizeof(char));
   strcpy(DataString,"init");
   OMset_name_str_val(VisitReader_id, OMstr_to_name("DataString"), DataString);

   /* Set DataIntScalar's value */  
   DataIntScalar=4;
   OMset_name_int_val(VisitReader_id, OMstr_to_name("DataIntScalar"), DataIntScalar);
   /* Set DataShortScalar's value */  
   DataShortScalar=4;
   OMset_name_int_val(VisitReader_id, OMstr_to_name("DataShortScalar"), DataShortScalar);
   /* Set DataShortScalar's value */  
   DataByteScalar=4;
   OMset_name_int_val(VisitReader_id, OMstr_to_name("DataByteScalar"), (int) DataByteScalar);
   /* Set DataDoubleScalar's value */
   DataDoubleScalar=4;
   OMset_name_real_val(VisitReader_id, OMstr_to_name("DataDoubleScalar"), DataDoubleScalar);

   /* Set n1's value */  
   OMset_name_int_val(VisitReader_id, OMstr_to_name("n1"), n1);
   /* Set n2's value */  
   OMset_name_int_val(VisitReader_id, OMstr_to_name("n2"), n2);
   /* Set n3's value */  
   OMset_name_int_val(VisitReader_id, OMstr_to_name("n3"), n3);
   /* Set n4's value */  
   OMset_name_int_val(VisitReader_id, OMstr_to_name("n4"), n4);

   return(1);
}

