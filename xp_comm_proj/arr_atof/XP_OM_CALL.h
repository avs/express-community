#ifndef OM_CALL_H
#define OM_CALL_H

#define XP_SUCCESS 1
#define XP_FAILURE 0

/* ------------------------------------------------------------------------- 

   General Notes:

   1. Macros named OMCALL_* 

      - call an Object Manager function 
      - set the value of the status variable

   2. Macros named OMCALLR_* 

      - call an Object Manager function 
      - set the value of the status variable
      - return the status if the status is not XP_SUCCESS

   3. Macros named OMCALLFR_*

      - call an Object Manager function
      - set the value of the status variable
      - have an argument which specifies a "fail action" to be carried
        out before returning if the status is not XP_SUCCESS
      - return the status if the status is not XP_SUCCESS

   4. Macros named *_SUCCESS 

      - are used when the only acceptable return from the OM function
        is OM_STAT_SUCCESS

   5. Macros named *_DEFAULT

      - are used when the only acceptable returns from the OM function
        are OM_STAT_SUCCESS or OM_STAT_UNDEF
      - have an argument which specifies a "default action" to be carried
        out when the OM function returns OM_STAT_UNDEF

   6. Macros named *_NONNULLOBJ

      - are used when the only acceptable return from the OM function
        is a Non-Null OM object

   7. Macros named *_NONNULLPTR

      - are used when the only acceptable return from the OM function
        is a Non-Null Pointer

   8. The FUNCCALL macro 

      - calls a function 
      - is used when the only acceptable return from the function is 
        XP_SUCCESS

   9. The FUNCCALLR macro

      - calls a function
      - is used when the only acceptable return from the function is
        XP_SUCCESS
      - returns the status if the status is not XP_SUCCESS

  10. The FUNCCALLFR macro

      - calls a function
      - is used when the only acceptable return from the function is
        XP_SUCCESS
      - has an argument which specifies a "fail action" to be carried
        out before returning if the status is not XP_SUCCESS
      - returns the status if the status is not XP_SUCCESS
      
   ------------------------------------------------------------------------- */

/* Set up print statements */ 
#define report_undefined_status( line, file, desc)                           \
printf( "Undefined status returned at:\n"                                    \
        "  Line: %i\n"                                                       \
        "  File %s\n"                                                        \
        "  Description: %s\n\n", line, file, desc);

#define report_error_status( line, file, desc)                               \
printf( "Error status returned at:\n"                                        \
        "  Line: %i\n"                                                       \
        "  File: %s\n"                                                       \
        "  Description: %s\n\n", line, file, desc);

#define report_null_obj( line, file, desc)                                   \
printf( "NULL object returned at:\n"                                         \
        "  Line: %i\n"                                                       \
        "  File: %s\n"                                                       \
        "  Description: %s\n\n", line, file, desc);

#define report_null_ptr( line, file, desc)                                   \
printf( "NULL pointer returned at:\n"                                        \
        "  Line: %i\n"                                                       \
        "  File: %s\n"                                                       \
        "  Description: %s\n\n", line, file, desc);

#define report_unknown_status( status, line, file, desc)                     \
printf( "Unknown status = %i returned at:\n"                                 \
        "  Line: %i\n"                                                       \
        "  File %s\n"                                                        \
        "  Description: %s\n\n", status, line, file, desc);

/* ------------------------------------------------------------------------- 

            Macro: OMCALL_SUCCESS
      Description: This macro is used to call AVS/Express Object Manager 
                   (OM) functions for which the only acceptable OM return 
		   value is OM_STAT_SUCCESS.  

		   If the OM call returns OM_STAT_SUCCESS, then the status
		   variable is set to XP_SUCCESS.

		   Other return values are considered to be errors, are 
		   recorded in the CMS_ERROR log, and cause the status 
		   variable to be set to XP_FAILURE.

                   The user of this macro, should check the status to 
		   determine whether or not to continue, free memory and 
		   return, etc.

        Arguments: The first argument to this macro is a short description 
		   of what the call is attempting to do.  If the function call 
		   fails, this description is included in the logged 
		   error.  

		   The second argument to this macro is the actual OM 
		   function call.  

    Prerequisites: 1. In order to use this macro, the function in which the 
    		      macro is used must have the following variable declared.
		  
		        int status;

          Example: 

   OMCALL_SUCCESS( "Set current number of modules",
                  OMset_int_val( NumberOfModules_id, NumberOfModules ) );
   if( status != XP_SUCCESS ) ...

   ------------------------------------------------------------------------- */

#define OMCALL_SUCCESS( desc, func )                                         \
status = func;                                                               \
switch( status )                                                             \
  {                                                                          \
 case OM_STAT_UNDEF:                                                         \
  report_undefined_status( __LINE__, __FILE__, desc );                       \
  status = XP_FAILURE;                                                       \
  break;                                                                     \
 case OM_STAT_ERROR:                                                         \
  report_error_status( __LINE__, __FILE__, desc );                           \
  status = XP_FAILURE;                                                       \
  break;                                                                     \
 case OM_STAT_SUCCESS:                                                       \
  status = XP_SUCCESS;                                                       \
  break;                                                                     \
 default:                                                                    \
  report_unknown_status( status, __LINE__, __FILE__, desc );                 \
  status = XP_FAILURE;                                                       \
  break;                                                                     \
  }
  

/* ------------------------------------------------------------------------- 

            Macro: OMCALLR_SUCCESS
      Description: This macro behaves just like OMCALL_SUCCESS with the 
                   following addition.

		   This macro causes a return of status whenever status is 
		   not set to XP_SUCCESS. 
		   
        Arguments: Same as OMCALL_SUCCESS

    Prerequisites: Same as OMCALL_SUCCESS 

                   *PLUS*
 
		   A function in which this macro is used must be a 
		   function which returns an int.

          Example: 

   OMCALLR_SUCCESS( "Set current number of modules",
                   OMset_int_val( NumberOfModules_id, NumberOfModules ) );

   ------------------------------------------------------------------------- */

#define OMCALLR_SUCCESS( desc, func )                                        \
OMCALL_SUCCESS( desc, func );                                                \
if( status != XP_SUCCESS ) return( status )

/* ------------------------------------------------------------------------- 

            Macro: OMCALLFR_SUCCESS
      Description: This macro behaves just like OMCALLR_SUCCESS with the 
                   following addition.

		   Before causing a return, this macro executes the 
                   fail_act argument.
		   
        Arguments: Same as OMCALLR_SUCCESS plus

	           Third argument fail_act executes before return.

    Prerequisites: Same as OMCALLR_SUCCESS 

          Example: 

   OMCALLFR_SUCCESS( "Set current number of modules",
                    OMset_int_val( NumberOfModules_id, NumberOfModules ),
		    free( willie )
		    );

   ------------------------------------------------------------------------- */

#define OMCALLFR_SUCCESS( desc, func, fail_act )                             \
OMCALL_SUCCESS( desc, func );                                                \
if( status != XP_SUCCESS )                                                   \
  {                                                                          \
  fail_act;                                                                  \
  return( status );                                                          \
  }

/* ------------------------------------------------------------------------- 

            Macro: OMCALL_DEFAULT
      Description: This macro is used to call AVS/Express Object Manager 
                   (OM) functions in which the acceptable return values 
		   are OM_STAT_SUCCESS and OM_STAT_UNDEF.  

		   If the OM call returns either of those two status values,
		   then the status variable is set to XP_SUCCESS.

		   Other return values are considered to be errors, are
		   recorded in the CMS_ERROR log, and cause the status 
		   variable to be set to XP_FAILURE.

		   The user of this macro, should check the status to 
		   determine whether or not to continue, free memory and 
		   return, etc.
		   
        Arguments: The first argument to this macro is a short description 
		   of what the call is attempting to do.  If the function 
		   call fails, this description is included in the logged 
		   error.  

		   The second argument is the actual OM function call.  

		   The third argument is a single C statement which 
		   defines the UNDEF action.  That is, the action that 
		   is to occur if the function returns OM_STAT_UNDEF.

    Prerequisites: 1. In order to use this macro, the function in which the 
    		      macro is used must have the following variable declared.
		  
		        int status;


          Example: 

   int CreateModule;
     ...
   OMCALL_DEFAULT( "Get CreateModule's value",
                   OMget_name_int_val( GenericContainer_id,
                                       OMstr_to_name("CreateModule"),
		   	               &CreateModule ),
                      CreateModule = 0 );
   if( status != XP_SUCCESS ) ...

   ------------------------------------------------------------------------- */

#define OMCALL_DEFAULT( desc, func, undef_act )                              \
status = func;                                                               \
switch( status )                                                             \
  {                                                                          \
 case OM_STAT_UNDEF:                                                         \
  undef_act;                                                                 \
  status = XP_SUCCESS;                                                       \
  break;                                                                     \
 case OM_STAT_ERROR:                                                         \
  report_error_status( __LINE__, __FILE__, desc );                           \
  status = XP_FAILURE;                                                       \
  break;                                                                     \
 case OM_STAT_SUCCESS:                                                       \
  status = XP_SUCCESS;                                                       \
  break;                                                                     \
 default:                                                                    \
  report_unknown_status( status, __LINE__, __FILE__, desc );                 \
  status = XP_FAILURE;                                                       \
  break;                                                                     \
  }

/* ------------------------------------------------------------------------- 

            Macro: OMCALLR_DEFAULT
      Description: This macro behaves just like OMCALL_DEFAULT with the 
                   following addition.

		   This macro causes a return of status whenever status 
		   is not set to XP_SUCCESS.
		   
        Arguments: Same as OMCALL_DEFAULT

    Prerequisites: Same as OMCALL_DEFAULT

                   *PLUS*

		   A function in which this macro is used must be a 
		   function which returns an int.

          Example: 

   int CreateModule;
     ...
   OMCALLR_DEFAULT( "Get CreateModule's value",
                   OMget_name_int_val( GenericContainer_id,
                                      OMstr_to_name("CreateModule"),
		   	              &CreateModule ),
                   CreateModule = 0 );

   ------------------------------------------------------------------------- */

#define OMCALLR_DEFAULT( desc, func, undef_act )                             \
OMCALL_DEFAULT( desc, func, undef_act );                                     \
if( status != XP_SUCCESS ) return( status )

/* ------------------------------------------------------------------------- 

            Macro: OMCALLFR_DEFAULT
      Description: Needs Documented
		   
        Arguments: 

    Prerequisites: 

          Example: 

   ------------------------------------------------------------------------- */

#define OMCALLFR_DEFAULT( desc, func, undef_act, fail_act )                  \
OMCALL_DEFAULT( desc, func, undef_act );                                     \
if( status != XP_SUCCESS )                                                   \
  {                                                                          \
    fail_act;                                                                \
    return( status );                                                        \
  }									       

/* ------------------------------------------------------------------------- 

            Macro: OMCALL_NONNULLOBJ
      Description: This macro is used to call AVS/Express Object Manager 
                   (OM) functions in which the only acceptable return value 
		   is a non-null OM object.  

		   The return of a null OM object causes an error to be 
		   recorded in the CMS_ERROR log and the status varible 
		   to be set to XP_FAILURE.

		   The return of a non-null OM object causes the status
		   variable to be set to XP_SUCCESS.

		   The user of this macro, should check the status to
		   determine whether or not to continue, free memory and
		   return, etc.

	Arguments: The first argument to this macro is a short description 
		   of what the call is attempting to do.  If the function 
		   call fails, this description is included in the logged 
		   error.  

		   The second argument is the object that is to get a value 
		   from the OM function call.

		   The third argument to this macro is the actual OM 
		   function call.  

    Prerequisites: 1. In order to use this macro, the function in which the 
    		      macro is used must have the following variable declared.
		  
		        int status;

         Examples: 

      OMCALL_NONNULLOBJ( "Create new object", 
                        new_obj, 
                        OMcreate_obj_from_path( PathToTemplate, 
			                       NewModuleName,
			                       Parent_id ) );
      if( status != XP_SUCCESS ) ...

      OMCALL_NONNULLOBJ( "Find Container",
                        *Container_id,
                        OMfind_subobj( GenericContainer_id, 
	                              OMstr_to_name("Container"),
		                      OM_OBJ_RW ) );
      if( status != XP_SUCCESS ) ...

   ------------------------------------------------------------------------- */

#define OMCALL_NONNULLOBJ( desc, result, func )                              \
result = func;                                                               \
if OMis_null_obj( result )                                                   \
  {                                                                          \
  report_null_obj( __LINE__, __FILE__, desc );                               \
  status = XP_FAILURE;                                                       \
  }                                                                          \
else                                                                         \
  {                                                                          \
  status = XP_SUCCESS;                                                       \
  }

/* ------------------------------------------------------------------------- 

            Macro: OMCALLR_NONNULLOBJ
      Description: This macro behaves just like OMCALL_NONNULLOBJ with 
                   the following addition.

		   This macro causes a return of status whenever status is 
		   not set to XP_SUCCESS. 
		   
        Arguments: Same as OMCALL_NONNULLOBJ

    Prerequisites: Same as OMCALL_NONNULLOBJ

                   *PLUS*
 
		   A function in which this macro is used must be a 
		   function which returns an int.

          Example: 

      OMCALL_NONNULLOBJ( "Find Container",
                        *Container_id,
                        OMfind_subobj( GenericContainer_id, 
	  	                      OMstr_to_name("Container"),
		                      OM_OBJ_RW ) );

   ------------------------------------------------------------------------- */

#define OMCALLR_NONNULLOBJ( desc, result, func )                             \
OMCALL_NONNULLOBJ( desc, result, func );                                     \
if( status != XP_SUCCESS ) return( status )


/* ------------------------------------------------------------------------- 

            Macro: OMCALLFR_NONNULLOBJ
      Description: Needs documented

        Arguments:

    Prerequisites:

          Example: 

   ------------------------------------------------------------------------- */

#define OMCALLFR_NONNULLOBJ( desc, result, func, fail_act )                  \
OMCALL_NONNULLOBJ( desc, result, func );                                     \
if( status != XP_SUCCESS )                                                   \
  {                                                                          \
    fail_act;                                                                \
    return( status );                                                        \
  }									       

/* ------------------------------------------------------------------------- 

            Macro: OMCALL_NONNULLPTR
      Description: This macro is used to call AVS/Express Object Manager 
                   (OM) functions in which the only acceptable return value 
		   is non-null pointer.  

		   The return of a NULL causes an error to be recorded in 
		   the CMS_ERROR log and the status variable to be set to
		   XP_FAILURE.

		   The return of a non-NULL pointer causes the status 
		   variable to be set to XP_SUCCESS.

		   The user of this macro, should check the status to
		   determine whether to continue, free memory and return,
		   etc.

        Arguments: The first argument to this macro is a short description 
		   of what the call is attempting to do.  If the function
		   call fails, this description is included in the logged 
		   error.  

		   The second argument is the pointer that is to get a value 
		   from the OM function call.

		   The third argument to this macro is the actual OM 
		   function call.  

    Prerequisites: 1. In order to use this macro, the function in which the 
    		      macro is used must have the following variable declared.
		  
		        int status;

          Example: 

    OMCALL_NONNULLPTR( "Get the integer array specifying the range",
	              *MultiSelectRange,
		      (int *)OMret_name_array_ptr( GenericContainer_id, 
		          OMstr_to_name("MultiSelectRange"), 
	  	          OM_GET_ARRAY_RD, MultiSelectRange_size, NULL) );
    if( status != XP_SUCCESS ) ...

   ------------------------------------------------------------------------- */

#define OMCALL_NONNULLPTR( desc, result, func )                              \
result = func;                                                               \
if( result == NULL )                                                         \
  {                                                                          \
  report_null_ptr( __LINE__, __FILE__, desc );                               \
  status = XP_FAILURE;                                                       \
  }                                                                          \
else                                                                         \
  {                                                                          \
  status = XP_SUCCESS;                                                       \
  }

/* ------------------------------------------------------------------------- 

            Macro: OMCALLR_NONNULLPTR
      Description: This macro behaves just like OMCALL_NONNULLPTR with 
                   the following addition.

		   This macro causes a return of status whenever status is 
		   not set to XP_SUCCESS. 
		   
        Arguments: Same as OMCALL_NONNULLPTR

    Prerequisites: Same as OMCALL_NONNULLPTR

                   *PLUS*
 
		   A function in which this macro is used must be a 
		   function which returns an int.

          Example: 

    OMCALLR_NONNULLPTR( "Get the integer array specifying the range",
		       *MultiSelectRange,
		       (int *)OMret_name_array_ptr( GenericContainer_id, 
		         OMstr_to_name("MultiSelectRange"), 
	  		 OM_GET_ARRAY_RD, MultiSelectRange_size, NULL) );

   ------------------------------------------------------------------------- */

#define OMCALLR_NONNULLPTR( desc, result, func )                             \
OMCALL_NONNULLPTR( desc, result, func );                                     \
if( status != XP_SUCCESS ) return( status )

/* ------------------------------------------------------------------------- 

            Macro: OMCALLFR_NONNULLPTR
      Description: Needs documented

        Arguments:

    Prerequisites:

          Example: 

   ------------------------------------------------------------------------- */

#define OMCALLFR_NONNULLPTR( desc, result, func, fail_act )                  \
OMCALL_NONNULLPTR( desc, result, func );                                     \
if( status != XP_SUCCESS )                                                   \
  {                                                                          \
    fail_act;                                                                \
    return( status );                                                        \
  }

/* ------------------------------------------------------------------------- 

            Macro: FUNCCALL
      Description: This macro is used to call a function for which the 
                   only acceptable return value is XP_SUCCESS.

		   Other return values cause an error to be logged in the
		   CMS_ERROR log.

		   The user of this macro, should check the status to
		   determine whether to continue, free memory and return,
		   etc.

        Arguments: The first argument to this macro is a short description 
		   of what the call is attempting to do.  If the function
		   call fails, this description is included in the logged 
		   error.  

		   The second argument is the actual function call.  

    Prerequisites: 1. In order to use this macro, the function in which the 
    		      macro is used must have the following variable declared.
		  
		        int status;

          Example: 

    FUNCCALL( "Add name to name list",
            AddObjNameToList( NameList_id, NumberOfModules_id, new_obj ) );
    if( status != XP_SUCCESS ) return( status );

   ------------------------------------------------------------------------- */

#define FUNCCALL( desc, func )                                               \
status = func;                                                               \
if( status != XP_SUCCESS )                                                   \
  {                                                                          \
  report_error_status( __LINE__, __FILE__, desc );                           \
  }

/* ------------------------------------------------------------------------- 

            Macro: FUNCCALLR
      Description: This macro behaves just like FUNCCALL with
                   the following addition.

		   This macro causes a return of status whenever status is 
		   not set to XP_SUCCESS. 

        Arguments: Same as FUNCCALL

    Prerequisites: Same as FUNCCALL

                   *PLUS*
 
		   A function in which this macro is used must be a 
		   function which returns an int.

          Example: 

    FUNCCALLR( "Add name to name list",
              AddObjNameToList( NameList_id, NumberOfModules_id, new_obj ) );

   ------------------------------------------------------------------------- */

#define FUNCCALLR( desc, func )                                              \
FUNCCALL( desc, func );                                                      \
if( status != XP_SUCCESS ) return( status )

/* ------------------------------------------------------------------------- 

            Macro: FUNCCALLFR
      Description: needs documented

        Arguments:

    Prerequisites:

          Example: 

   ------------------------------------------------------------------------- */

#define FUNCCALLFR( desc, func, fail_act )                                   \
FUNCCALL( desc, func );                                                      \
if( status != XP_SUCCESS )                                                   \
  {                                                                          \
    fail_act;                                                                \
    return( status );                                                        \
  }

/* ------------------------------------------------------------------------- 

            Macro: FREENONNULL
      Description: This macro is used to free allocate memory and reset the
                   pointer to NULL.

   ------------------------------------------------------------------------- */

#define FREENONNULL( pointer )                                               \
if( pointer != NULL )                                                        \
  {                                                                          \
    free( pointer );                                                         \
    pointer = NULL;                                                          \
  }

/* ------------------------------------------------------------------------- */
#endif /* OM_CALL_H */
