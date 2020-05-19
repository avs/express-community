
/* *****************************************************/
/* definitions required for NURBS evaluation functions */
/* *****************************************************/

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define MAXLINE   132
#define MAX_ORDER 11
#define Qfloat float

typedef struct {                     /* 3D point definition */
	Qfloat x;
	Qfloat y;
	Qfloat z;
	} Qpoint3;

typedef struct {                     /* 4D point definition */
	Qfloat x;
	Qfloat y;
	Qfloat z;
	Qfloat w;
	} Qpoint4;  

typedef struct {                     /* NURBS PCH geometry entity */
        int     eval;                /* index of evaluator routines */
        int     ttyp;                /* type of t intervals */
        int     ku;                  /* order in u direction */
        int     kv;                  /* order in v direction */
        int     nu;                  /* number of control points in u */
        int     nv;                  /* number of control points in v */
        Qfloat  d[1];                /* data area for knots in u & v */ 
                                     /* directions, 4D control points */
        } Qnurbs_pch ;
 

    /****************************/
    /***  Defined Parameters  ***/
    /****************************/

/***
#define DBG_MESSAGE 1
 ***/

#define ERR_RETURN(A) { \
   ERRerror("nurbs demo", 1, ERR_ORIG, A); return(0); \
}


#define RESOLVE_INT_PARM(elem_id,string,nurbs,ii)             \
  {local_id = OMfind_subobj((elem_id),                       \
              OMstr_to_name((string)),nurbs);                 \
  if ( OMis_null_obj(local_id))                              \
  {                                                          \
      ERRerror("nurbs_demo",1,ERR_ORIG,                       \
               "search failed for subobj int param name");   \
      return(0);                                             \
  }                                                          \
  if ( OMget_int_val(local_id,&(ii)) != 1 )                  \
  {                                                          \
      ERRerror("nurbs_demo",1,ERR_ORIG,                       \
               "search failed for subobj int param value");  \
      return(0);                                             \
  }                                                          \
  }

#define RESOLVE_FLT_PARM(elem_id,string,nurbs,xx)             \
  {local_id = OMfind_subobj((elem_id),                       \
              OMstr_to_name((string)),nurbs);                 \
  if ( OMis_null_obj(local_id))                              \
  {                                                          \
      ERRerror("nurbs_demo",1,ERR_ORIG,                       \
               "search failed for subobj flt param name");   \
      return(0);                                             \
  }                                                          \
  if ( OMget_real_val(local_id,&(xd)) != 1 )                 \
  {                                                          \
      ERRerror("nurbs_demo",1,ERR_ORIG,                       \
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
      ERRerror("nurbs_demo",1,ERR_ORIG,                      \
               "search failed for subobj str param name");   \
      return(0);                                             \
  }                                                          \
  if ( OMget_str_val(local_id,(char **) &(cc),0) != 1 )      \
  {                                                          \
      ERRerror("nurbs_demo",1,ERR_ORIG,                      \
               "search failed for subobj str param value");  \
      return(0);                                             \
  }                                                          \
  }

    /**************************/
    /***  Global Variables  ***/
    /**************************/

/*
 * User - Interface Control Structure
 *        Parameters from OM subobjects
 */

typedef struct
{
   char  *nurbs_filename;
   int    nurbs_nv;
   int    nurbs_nu;
   int    nurbs_show_ctrl;
}
Ui;

void  nrb_pch_eval(Qnurbs_pch *,Qfloat,Qfloat,int,Qpoint3 *);
void nrb_core2(Qfloat *,Qfloat *,int,int,Qpoint4 *); 
void nrb_core1(Qfloat *,Qfloat *,int,int,Qpoint4 *); 
void nrb_interv_knots (int,int,Qfloat,Qfloat *,int *);
Qnurbs_pch *nrb_read_pch(char *);
void nrb_dump_pch (char *,int,Qnurbs_pch *);
void nrb_pch_data (Qnurbs_pch *,int *,int *,int *,int *,Qfloat **,Qfloat **,Qpoint4 **);
int nrb_pch_size (int,int,int,int);
int nurbs_util(OMobj_id , OMevent_mask, int);
