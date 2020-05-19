
/*
 * SelectAllCore polygon list management module
 */

#include "xp_comm_proj/dlgtools/dlg.h"
#include "xp_comm_proj/dlgtools/outline/gen.h"


int
select_all_om(OMobj_id top_id,
	  OMevent_mask event_mask,
	  int seq_num)
{

  /*******************************************************************

    parameter variables

  *******************************************************************/

  OMobj_id    choice_id;
  int         *choice;
  int         choice_size;
  
  OMobj_id    all_flag_id;
  int         all_flag = 0;

  OMobj_id    n_polys_id;
  int         n_polys = 0;

  OMobj_id    list_size_id;
  int         list_size;
  
  OMobj_id    selected_id;
  int         *selected;


  /*******************************************************************

    the rest

  *******************************************************************/

  int status;

  int size, type;

  int ii;


  /*******************************************************************

    generics

  *******************************************************************/

#include "../gen_proc.h"

  sprintf(  LFNC_function_name, "select_all_om" );

  
  /*******************************************************************

    instance

  *******************************************************************/
  
  if( event_mask & OM_EVENT_INST )
    {

      E_EV_INST(""); return(0);

    }


  /*******************************************************************

    evaluate

  *******************************************************************/
  if( event_mask & OM_EVENT_VAL )
    {


      /***************************************************************

         get some id's

      ***************************************************************/

      FIND_SUBOBJ( choice_id, top_id, "choice" );
      FIND_SUBOBJ( all_flag_id, top_id, "all_flag" );
      FIND_SUBOBJ( n_polys_id, top_id, "n_polys" );
      FIND_SUBOBJ( list_size_id, top_id, "list_size" );
      FIND_SUBOBJ( selected_id, top_id, "selected" );


      /***************************************************************

         get the values of the n_polys and all_flag parameters

      ***************************************************************/

      GET_INT_VAL( n_polys_id, &n_polys, "n_polys" );

      GET_INT_VAL( all_flag_id, &all_flag, "all_flag" );
      all_flag = MIN( 1, MAX( all_flag, 0 ) );



      /***************************************************************

         if all_flag is true we can ignore everything else and just fill the array

      ***************************************************************/

      if (all_flag) {
         SET_INT_VAL( list_size_id, n_polys, "list_size" );
         
         RET_ARRAY_RW( selected, selected_id, "selected" );

         if (size != n_polys) {
            ARRfree( (char *)selected );
            MY_ERROR( "selected array size is not equal to list_size" );
            return(0);
         }

         for (ii=0; ii<n_polys; ii++) {
            selected[ii] = ii;
         }

         ARRfree( (char *)selected );

         return (1);
      }




      /***************************************************************

         all_flag is false therefore we have to look at the choice array

      ***************************************************************/

      OMget_array_size(choice_id, &choice_size);

      if( choice_size < 1 )
      {
         if( debug_env )
         {
            sprintf( chuge, "choice_size = %i < 1", choice_size );
            MY_MESSAGE( chuge );
         }
  
         SET_INT_VAL( list_size_id, 0, "list_size" );
         return(1);
      } 
	      
      RET_ARRAY_RD( choice, choice_id, "choice" );

      if (size != choice_size) {
         ARRfree( (char *)choice );
         MY_ERROR( "choice array size is incorrect" );
         return(0);
      }
      

      /***************************************************************

         we have a valid choice array, now we just have to copy values across

      ***************************************************************/

      SET_INT_VAL( list_size_id, choice_size, "list_size" );

      RET_ARRAY_RW( selected, selected_id, "selected" );

      if (size != choice_size) {
         ARRfree( (char *)choice );
         ARRfree( (char *)selected );
         MY_ERROR( "selected array size is not equal to list_size" );
         return(0);
      }

      for (ii=0; ii<choice_size; ii++) {
         if ((choice[ii]>=0) && (choice[ii]<n_polys))
            selected[ii] = choice[ii];
         else
            selected[ii] = 0;
      }

      ARRfree( (char *)choice );
      ARRfree( (char *)selected );

      return (1);
   }


            
  /*******************************************************************

    de-instance

  *******************************************************************/
  
  if( event_mask & OM_EVENT_DEINST )
    {

       E_EV_DEINST(""); return(0);

    }



  /*******************************************************************

    bye-bye

  *******************************************************************/

  return(1);
  
}

