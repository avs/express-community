/*********************************************************************

  this is the generic include file for file scope

*********************************************************************/


/*********************************************************************

  include files

*********************************************************************/

/* standard includes */
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <math.h>
#include  <time.h>
#include  <limits.h>
#include  <float.h>
#include  <errno.h>
#include  <sys/types.h>
#include  <signal.h>
/*
#include  <unistd.h>
 */

/* avs includes */
#include <avs/util.h>
#include <avs/err.h>
#include <avs/math.h>
#include <avs/db.h>
#include <avs/fld.h>
#include <avs/arr.h>
#include <avs/om.h>
#include <avs/om_type.h>
#include <avs/event.h>




/*********************************************************************

  miscellaneous defines and macros

*********************************************************************/


/* min/max macros */
#define  MAX(a,b) ( (a) > (b) ? (a) : (b) )
#define  MIN(a,b) ( (a) < (b) ? (a) : (b) )

/* macro for number of spaces in output */
#define  l_exp(a) ( (long) ceil( log10( (double) (a)+1 ) ) )
#define  i_exp(a) ( (int) ceil( log10( (double) (a)+1 ) ) )

/* macros for error and return */
#define E_dump(a)  ERRdump( a ); return(0)
#define E_error(a) ERRerror( __FILE__, 1, ERR_ORIG, a ); return(0)

#define MY_ERROR(a) printf( "\nFunction: %s\nLine: %i\nFile:%s\nERROR: %s\n", LFNC_function_name, __LINE__, __FILE__, a )
#define MY_MESSAGE(a) printf( "\nFunction: %s\nLine: %i\nFile:%s\nMESSAGE: %s\n", LFNC_function_name, __LINE__, __FILE__, a )

#define MY_LOCATION(a) sprintf( a, "Function: %s - Line: %i - File:%s", LFNC_function_name, __LINE__, __FILE__ )

#define WINDOW_ERROR(a) printf( "\nFunction: %s\nLine: %i\nFile:%s\nERROR: %s\n", LFNC_function_name, __LINE__, __FILE__, a )
#define MY_MESSAGE(a) printf( "\nFunction: %s\nLine: %i\nFile:%s\nMESSAGE: %s\n", LFNC_function_name, __LINE__, __FILE__, a )

/* macros for event mask errors */
#define E_EV_INST(a) printf( "Function: %s - Line: %i - File: %s\nInvalid instance method invocation\n ==> %s", LFNC_function_name, __LINE__, __FILE__, a )
#define E_EV_VAL(a) printf( "Function: %s - Line: %i - File: %s\nInvalid update method invokation\n ==> %s\n", LFNC_function_name, __LINE__, __FILE__, a )
#define E_EV_DEINST(a) printf( "Function: %s - Line: %i - File: %s\nInvalid de-instance method invokation\n ==> %s\n", LFNC_function_name, __LINE__, __FILE__, a )


#define PRINT_ERRNO printf( "\n errno = %i\n==> %s\n",errno, strerror(errno ))

/* malloc macros */
#define MY_MALLOC(a) malloc( a )
#define MY_CALLOC(a,b) calloc( a, b )
#define MY_REALLOC(a,b) realloc( a, b )
#define MY_FREE(a) free(a); a = NULL


/*********************************************************************

   OM calls w/ error

*********************************************************************/

#define MY_ARRfree(a) ARRfree( (char *) a ); a = NULL

#define FIND_SUBOBJ(a,b,c) a = OMfind_subobj(b,OMstr_to_name(c),OM_OBJ_RW); if( OMis_null_obj(a) ){ sprintf( chuge, "finding id for for %s", c ); MY_ERROR( chuge ); return(0); }

#define FIND_STR_SUBOBJ(a,b,c) a = OMfind_str_subobj(b,c,OM_OBJ_RW); if( OMis_null_obj(a) ){ sprintf( chuge, "finding id for for %s", c ); MY_ERROR( chuge ); return(0); }



#define GET_INT_VAL(a,b,c) status = OMget_int_val(a,b); if( status == OM_STAT_ERROR ) { sprintf( chuge, "getting int val for %s", c ); MY_ERROR( chuge ); return(0); }

#define GET_REAL_VAL(a,b,c) status = OMget_real_val(a,b); if( status == OM_STAT_ERROR ) { sprintf( chuge, "getting real val for %s", c ); MY_ERROR( chuge ); return(0); }

#define GET_STR_VAL(a,b,c) status = OMget_str_val(a,b,0); if( status == OM_STAT_ERROR ) { sprintf( chuge, "getting str val for %s", c ); MY_ERROR( chuge ); return(0); }

#define GET_STR_ARRAY_VAL(a,b,c) status = OMget_str_array_val(a,b,c,0); if( status == OM_STAT_ERROR ) { sprintf( chuge, "setting %i str array val for %s", b, c ); MY_ERROR( chuge ); return(0); }

#define GET_PTR_VAL(a,b,c) status = OMget_ptr_val(a,b,0); if( status == OM_STAT_ERROR ) { sprintf( chuge, "getting ptr val for %s", c ); MY_ERROR( chuge ); return(0); }



#define SET_INT_VAL(a,b,c) status = OMset_int_val(a,b); if( status == OM_STAT_ERROR ) { sprintf( chuge, "setting int val for %s", c ); MY_ERROR( chuge ); return(0); }

#define SET_REAL_VAL(a,b,c) status = OMset_real_val(a,b); if( status == OM_STAT_ERROR ) { sprintf( chuge, "setting real val for %s", c ); MY_ERROR( chuge ); return(0); }

#define SET_STR_VAL(a,b,c) status = OMset_str_val(a,b); if( status == OM_STAT_ERROR ) { sprintf( chuge, "setting str val for %s", c ); MY_ERROR( chuge ); return(0); }

#define SET_STR_ARRAY_VAL(a,b,c) status = OMset_str_array_val(a,b,c); if( status == OM_STAT_ERROR ) { sprintf( chuge, "setting %i str array val for %s", b, c ); MY_ERROR( chuge ); return(0); }

#define SET_PTR_VAL(a,b,c) status = OMset_ptr_val(a,b,0); if( status == OM_STAT_ERROR ) { sprintf( chuge, "setting ptr val for %s", c ); MY_ERROR( chuge ); return(0); }





#define GET_ARRAY_SIZE(a,b,c) status = OMget_array_size(a,b); if( status == OM_STAT_ERROR ) { sprintf( chuge, "getting array size %i for %s ", b,c ); MY_ERROR( chuge ); return(0); }

#define SET_ARRAY_SIZE(a,b,c) status = OMset_array_size(a,b); if( status == OM_STAT_ERROR ) { sprintf( chuge, "setting array size %i for %s ", b,c ); MY_ERROR( chuge ); return(0); }



#define RET_ARRAY_RW(a,b,c) a = OMret_array_ptr( b, OM_GET_ARRAY_RW, &size, &type ); if( a == NULL ) { sprintf( chuge, "getting array pointer for %s", c ); MY_ERROR( chuge ); return(0); }

#define RET_ARRAY_RD(a,b,c) a = OMret_array_ptr( b, OM_GET_ARRAY_RD, &size, &type ); if( a == NULL ) { sprintf( chuge, "getting array pointer for %s", c ); MY_ERROR( chuge ); return(0); }


#define GET_ARRAY_VAL(a,b,c,d) status = OMget_array_val(a,b,c,OM_OBJ_RW); if( status == OM_STAT_ERROR ) { sprintf( chuge, "getting array val for %s[%i]", d,b ); MY_ERROR( chuge ); return(0); }



/*********************************************************************

  MSDOS vs UNIX

*********************************************************************/
#ifdef MSDOS
#define SLASH "\\"
#else
#define SLASH "/"
#endif

/* sampler types */
#define SAMPLE_POINT            1
#define SAMPLE_LINE             2
#define SAMPLE_SQUARE           3
#define SAMPLE_PLANE            4
#define SAMPLE_CUBE             5
#define SAMPLE_SPACE            6
#define SAMPLE_CIRCLE           7
#define SAMPLE_ANNULUS          8
#define SAMPLE_CONTOUR_PLANE    9




/*********************************************************************

  structures and assocaited defines

*********************************************************************/


typedef struct timing_values__ {

  long int       seconds;
  long int  milliseconds;
  long int  microseconds;

} Timing_Values;




/*********************************************************************

  1. Ram_Block memory allocations

  FILE: LFNC_ram_control.c

*********************************************************************/

typedef struct Ram_Block_Data__ {

  char *head;
  char *tail;

  size_t size;
  size_t left;
  size_t used;

  struct Ram_Block_Data__ *next;

} Ram_Block_Data;

#ifdef __cplusplus
extern "C" {
#endif

extern int ram_init(
		    size_t block_size
		    );
extern char *ram_more(
		      int ram_block_number,
		      size_t more_bytes
		      );
extern int ram_free(
		    int block_number
		    );
extern int ram_free_all(
			void
			);
extern Ram_Block_Data *ram_querey(
				   int ram_block_number
				  );
extern void ram_toggle_debug(
			     int
			     );


#ifdef __cplusplus
} /* extern "C" */
#endif


/*********************************************************************

  1. display a message in an X window

  FILE: LFNC_display_text.c

*********************************************************************/
#define PM_WAIT 1
#define PM_EXIT 2

#ifdef __cplusplus
extern "C" {
#endif

extern void print_message(
			  unsigned int flags,
			  char *routine,
			  char *message
			  );

#ifdef __cplusplus
} /* extern "C" */
#endif







/*********************************************************************

  1. return a unique integer given a string - subsequent calls give
     different integers
  2. sleep using select(3c)

  LFNC_misc.c

*********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

extern int LFNC_unique_func_id(
			       char *input_string
			       );


extern int LFNC_wait(
		     struct timing_values__ timing_values
		     );


extern char *LFNC_current_time(
			       void
			       );

#ifdef __cplusplus
} /* extern "C" */
#endif

/*********************************************************************

  1. string enetry

  FILE: LFNC_strings.c

*********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

extern char *LFNC_str_entry(
			    char *string,
			    int entry
			    );

#ifdef __cplusplus
} /* extern "C" */
#endif




/*********************************************************************

  1. decompose a 4x4 into rotation, scale and translation
  2. build a tranformation matrix from indivdual matrices
  3. 4x4 double precision matrix multiplicat
  4. decompose 4x4 into rotation 4x4, scale and translation vector

*********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

extern int LFNC_decompose_affine(
				 float transformation_matrix[4][4],
				 float radians[3],
				 float scales[3],
				 float positions[3]
				 );

extern void LFNC_build_trans_matrix(
				    float trans_mat[4][4],
				    float angles[3],
				    float scales[3],
				    float translations[3] );

extern void LFNC_affine_mult_DOUBLE(double mat1[4][4],
				    double mat2[4][4],
				    double mat3[4][4] );


extern void LFNC_affine_mult(float mat1[4][4],
			     float mat2[4][4],
			     float mat3[4][4] );

extern void LFNC_remove_ST(
			   float trans_mat[4][4],
			   float new_mat[4][4],
			   float scales[3]
			   );

#ifdef __cplusplus
} /* extern "C" */
#endif


/*********************************************************************

  quaternion manipulations

  1. rotation matrix to quaternion
  2. quaternion to rotation matrix
  3. quaternion dot product
  4. interpolation between two quaternions
  5. rotation matrix to euler angles
  6. euler angles to rotation matrix
  7. euler angles + scalings to rotation matrix

*********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


extern void LFNC_q2m( float q[4],
		     float mat[4][4] );

extern void LFNC_m2q( float mat[4][4],
		     float q[4]       );

extern void LFNC_quat_dot( float p[4],
			   float q[4],
			   float *dot );

extern void LFNC_slerp( float p[4],
		       float q[4],
		       float t,
		       float qt[4] );

extern void LFNC_m2a(
		     float rot_matrix[4][4],
		     float radians[3]
		     );

extern void LFNC_a2m(
		     float radians[3],
		     float rot_matrix[4][4]
		     );

extern void LFNC_as2m(
		      float radians[3],
		      float scales[3],
		      float rot_matrix[4][4]
		      );
#ifdef __cplusplus
} /* extern "C" */
#endif


/*********************************************************************

  interpolation

  1. unity interpolation for animation

*********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


extern void LFNC_unity_interpolation( float *weight, int order );


#ifdef __cplusplus
} /* extern "C" */
#endif




/*********************************************************************

  local pointer creation/destruction

  UTptr.c

*********************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

extern int
  UTptr_set( OMobj_id    top_id,
	     void       *pointer,
	     char       *name       );

extern int
  UTptr_get(OMobj_id     top_id,
	    void       **pointer,
	    char        *name       );

extern int
  UTptr_free( OMobj_id  top_id,
	      char     *name     );


extern int
  UTptr_create( OMobj_id    top_id,
	        char       *name       );
  
#ifdef __cplusplus
} /* extern "C" */
#endif


/*********************************************************************

  file utilities

  UTfile.c

*********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

extern int
  UTfile_open( char   *filename,
	       char   *mode,
	       FILE  **fd         );

extern int
  UTfile_close( char   *filename,
	        FILE   *fd         );


extern int
  UTfile_size( char   *filename,
	      FILE    *fd,
	      int     *filesize );




  
#ifdef __cplusplus
} /* extern "C" */
#endif

/*********************************************************************

  miscellaneous

  UTmisc.c

  1. make sure an integer value is valid and unity

*********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

extern int
  UTcheck_valid( OMobj_id    top_id,
		char       *name       );
  
#ifdef __cplusplus
} /* extern "C" */
#endif

