/*
 */

/* ----------------------------------------------------------------------
 * StringArrayConcatenateCore Module
 * ----------------------------------------------------------------------
 * Description:
 *   
 *   Concatenates strings with along with leader, separator,
 *   and trailer strings.  Useful to generate SQL queries and such.
 *   Example:
 *     input_strings = {"VA","PA","CA"}
 *     leader_string = " where STATE =' "
 *     separator_string = " ' and STATE=' "
 *     trailer_string = " ' "
 *     => output_str = "where STATE='VA' and STATE='PA' and STATE='CA'"
 *
 * Authors: 
 *   Brian Selle, Leon Thrane, Advanced Visual Systems Inc.
 *   Documentation written by Ian Curington
 *
 * Revision: 9th February 2000 - Paul G. Lever, IAC
 *   Converted to IAC format.
 *
 * ----------------------------------------------------------------------
 * Note:
 *   The gen.h include file is generated by Express when the module is 
 *   compiled. It avoids including "user.h" or "express.h" directly, so 
 *   that the module may be moved across the processes by changing the V
 *   properties in the library.
 * ----------------------------------------------------------------------
 */

#include "xp_comm_proj/strarrct/gen.h"
#include "XP_OM_CALL.h"
#include <ctype.h>

//----------------------------------------------------------------------------
//  prototypes for static functions                                           
//----------------------------------------------------------------------------

static void
FreeOnExit( char  *separator,
            char  *output,
	    char  *temp_string );

static void
SkipWhiteSpace( char  *input_string, 
                char **new_start );

//----------------------------------------------------------------------------
//  external functions                                                        
//----------------------------------------------------------------------------

int StringArrayConcatenate_StringArrayConcatenateMods_StringArrayConcatenateCore::update( OMevent_mask ,int ) {

  int    status;              // Return status for XP_OM_CALL macros
  char  *output = NULL;   
  char  *separator = NULL;   
  char  *temp_string = NULL;   
  
  // Get the leader string 
  OMCALLFR_SUCCESS( "Get leader string",
		    OMget_str_val( (OMobj_id)leader_string.obj_id(), 
				   &output, 0 ),
		    FreeOnExit( separator, output, temp_string ) );
  
  char *temp_start;   
  if( num_strings < 1 )
  {
    temp_string = (char *)malloc( 1 );
    *temp_string = '\0';      
    temp_start = temp_string;   
  }
  else
  {       
    // Get the first string in input_strings 
    temp_string = input_strings.ret_str_array_val( 0, NULL, 0 );
    
    if( temp_string != NULL )      
    {	    
      // Skip leading and trailing whitespace
      SkipWhiteSpace( temp_string, &temp_start );
    }
    else
    {
      temp_string = (char *)malloc( 1 );
      *temp_string = '\0';      
      temp_start = temp_string; 
    }  
  }
  
  // Concatenate leader string and first string
  OMCALLFR_NONNULLPTR( "Resize output string for leader",   
		       output, 
		       (char *)realloc( output, 
					strlen( output ) + 
					strlen( temp_start ) + 1 ),
		       FreeOnExit( separator, output, temp_string ) );
  
  output = strcat( output, temp_start );
  
  free( temp_string );     
  temp_string = (char *)NULL;      
  
  // Get the separator string 
  OMCALLFR_SUCCESS( "Get separator string",
		    OMget_str_val( (OMobj_id)separator_string.obj_id(), 
				   &separator, 0 ),
		    FreeOnExit( separator, output, temp_string ) ); 
  
  // Loop through input array and concatenate each string onto the output
  for( int string_index = 1; string_index < num_strings; string_index++ )
  {
    OMCALLFR_NONNULLPTR( "Get next string in input string array",   
			 temp_string, input_strings.
			 ret_str_array_val( string_index, NULL, 0 ),
			 FreeOnExit( separator, output, temp_string ) );
    
    // Strip off leading and trailing whitespace
    SkipWhiteSpace( temp_string, &temp_start );
    
    // Set output string size
    OMCALLFR_NONNULLPTR( "Resize output string",   
			 output, 
			 (char *)realloc( output, 
					  strlen( output ) + 
					  strlen( separator ) + 
					  strlen( temp_start ) + 1 ),
			 FreeOnExit( separator, output, temp_string ) );
    
    if( strlen( temp_start ) != 0 )
    {
      output = strcat( output, separator );
    }
    output = strcat( output, temp_start );
    
    free( temp_string );     
    temp_string = (char *)NULL;      
  } 
  
  // Get the trailer string 
  OMCALLFR_SUCCESS( "Get trailer string",
		    OMget_str_val( (OMobj_id)trailer_string.obj_id(), 
				   &temp_string, 0 ),
		    FreeOnExit( separator, output, temp_string ) );
  
  // Concatenate output string and the trailer string
  OMCALLFR_NONNULLPTR( "Resize output string",   
		       output, 
		       (char *)realloc( output, 
					strlen( output ) + 
					strlen( temp_string ) + 1 ),
		       FreeOnExit( separator, output, temp_string ) );
  
  output = strcat( output, temp_string );
  
  // Set the output string in XP
  OMCALLFR_SUCCESS( "Set output string",
		    OMset_str_val( (OMobj_id)output_string.obj_id(), 
				   output ), 
		    FreeOnExit( separator, output, temp_string ) );
  
  // Final cleanup
  FreeOnExit( separator, output, temp_string );
  
  // Set done processing flag
  done = 1;   
  
  return( XP_SUCCESS );
}

//-----------------------------------------------------------------------------
//  static functions                                                           
//-----------------------------------------------------------------------------

static void
FreeOnExit( char  *separator,
            char  *output,
	    char  *temp_string )
{

   if ( separator != (char *)NULL ) 
   {
      free( separator );
   }

   if ( output != (char *)NULL ) 
   {
      free( output );
   }

   if ( temp_string != (char *)NULL ) 
   {
      free( temp_string );
   }

}

static void
SkipWhiteSpace( char  *input_string, 
                char **new_start )

{
   int   new_len   = strlen(input_string);   
   char *new_end   = input_string + new_len - 1;
        *new_start = input_string;   
   
   while (new_len && isspace(*new_end))  
   {       
      *new_end = '\0';
      new_end--;      
      new_len--;      
   }
   
   while (isspace(**new_start)) 
   {	    
      (*new_start)++;
   }
}

/* end of file */
