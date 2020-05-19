
/*
 * DLG Outline Module
 */

#include "xp_comm_proj/dlgtools/dlg.h"
#include "xp_comm_proj/dlgtools/outline/gen.h"


int
dlg_outline_om(OMobj_id top_id,
	       OMevent_mask event_mask,
	       int seq_num)
{

  /*******************************************************************

    parameter variables

  *******************************************************************/

  OMobj_id  dlg_id;
  OMobj_id  outline_id;


  /*******************************************************************

    the rest

  *******************************************************************/

  int status;
  int size, type;

  OMobj_id    debug_id;
  int         debug;
  int         debug_flag;

   /*******************************************************************

    procedures

  *******************************************************************/

  int dlg_outline_all();

  /*******************************************************************

    generics

  *******************************************************************/
#include "../gen_proc.h"

  sprintf(  LFNC_function_name, "dlg_outline_om" );


  /*******************************************************************

    debugger

  *******************************************************************/

  debug_flag = 0;
  if( debug_env ) debug_flag = 1;

  if( debug_flag == 0 )
    {

      debug_id = OMfind_subobj( top_id,
			       OMstr_to_name("debug"),
			       OM_OBJ_RW );
      if( ! OMis_null_obj( debug_id ) )
	{

	  status = OMget_int_val( debug_id,
				 &debug    );
	  if( status == OM_STAT_ERROR )
	    {
	      E_dump( "error getting int_val debug " );
	    }
	  if( status != OM_STAT_UNDEF )
	    {
	      if( debug == 1 ) debug_flag = 1;
	    }
	}


    }


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

      if( debug_flag == 1 ) MY_MESSAGE( "valuate ..." );

      /***************************************************************

	get object id'd

      ***************************************************************/

      FIND_SUBOBJ( dlg_id, top_id, "dlg_in" );

      FIND_SUBOBJ( outline_id, top_id, "outline" );


      /***************************************************************

	load the field with polylines

      ***************************************************************/

      status = dlg_outline_all( top_id,
                dlg_id,
			       outline_id,
			       debug_flag );
      if( status != 1 ) return(0);


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









/*********************************************************************

  create polylines of all the data

*********************************************************************/

int dlg_outline_all( OMobj_id top_id,
          OMobj_id dlg_id,
		    OMobj_id outline_id,
		    int debug_flag  )

{


  /*******************************************************************

    parameter variables

  *******************************************************************/

  OMobj_id pb_id;
  OMobj_id polys_id;


  /*******************************************************************

    the rest

  *******************************************************************/

  int status;
  int size, type;


  int   n_polys;
  int   *poly_limits;
  int   total_nodes;
  float *poly_xy;
  int   *selected;
  int   selected_size;
  /*int    total_on; */

  OMobj_id child_id;

  int have_political_boundaries;

  float  *coordinates;
  int    *poly_connect;

  OMobj_id this_cell_set;

  int num_so_far, ii;


  /*******************************************************************

    procedures

  *******************************************************************/


  /*******************************************************************

    generics

  *******************************************************************/

#include "../gen_proc.h"

  sprintf(  LFNC_function_name, "dlg_outline_all" );


  
  
  /*******************************************************************

    wax any existing data

  *******************************************************************/

  status = FLDset_ncell_sets (outline_id, 0);
  if( status != OM_STAT_SUCCESS )
    {
      MY_ERROR( "FLDset_ncell_sets (outline_id, 0)" );
      return(0);
    }


  status = FLDset_nnodes (outline_id, 0);
  if( status != OM_STAT_SUCCESS )
    {
      MY_ERROR( "FLDset_nnodes (outline_id, 0)" );
      return(0);
    }


  

  /*******************************************************************

    make sure we have data

  *******************************************************************/
  {
    
    have_political_boundaries = 0;

    FIND_SUBOBJ( pb_id, dlg_id, "pb" );
    FIND_SUBOBJ( child_id, pb_id, "have_political_boundaries" );
    
    GET_INT_VAL( child_id, &have_political_boundaries,
		"have_political_boundaries" );
    if( status != OM_STAT_SUCCESS || have_political_boundaries != 1 )
      {
	return(0);
      };

  }


  

  /*******************************************************************

    make sure we have polygons

  *******************************************************************/
  {
    n_polys = 0;

    FIND_SUBOBJ( polys_id, dlg_id, "polys" );
    FIND_SUBOBJ( child_id, polys_id, "n_polys" );

    GET_INT_VAL( child_id, &n_polys, "n_polys" );

    if( n_polys < 1 ) return(0);

  }




  /*******************************************************************

    make sure at least one polyline is to be rendered

  *******************************************************************/
  FIND_SUBOBJ( child_id, top_id, "selected" );
  RET_ARRAY_RD( selected, child_id, "selected" );
  selected_size = size;

  if (selected_size < 1) return(1);
  
  for( ii=0; ii<selected_size; ii++ ) {
     if ((selected[ii]<0) || (selected[ii]>=n_polys)) {
        ARRfree( (char *)selected );
        MY_ERROR( "Invalid polygon number in selected array" );
        return(0);
     }
  }



  /*******************************************************************

    set the space

  *******************************************************************/

  status = FLDset_nspace (outline_id, 2 );
  if( status != OM_STAT_SUCCESS )
    {
      ARRfree( (char *)selected );
      sprintf( chuge, "FLDset_nspace (outline_id, %i)", 2 );
      MY_ERROR( chuge );
      return(0);
    }
  

  

  /*******************************************************************

    load the nodes

  *******************************************************************/

  FIND_SUBOBJ( child_id, polys_id, "total_nodes" );

  GET_INT_VAL( child_id, &total_nodes, "total_nodes" );

  status = FLDset_nnodes (outline_id, total_nodes);
  if( status != OM_STAT_SUCCESS )
    {
      ARRfree( (char *)selected );
      sprintf( chuge, "FLDset_nnodes (outline_id, %i)", total_nodes );
      MY_ERROR( chuge );
      return(0);
    }


  status = FLDget_coord( outline_id, &coordinates,
			&size, OM_GET_ARRAY_RW );
  if( status != OM_STAT_SUCCESS )
    {
      ARRfree( (char *)selected );
      MY_ERROR( "FLDget_coord(");
      return(0);
    }

  FIND_SUBOBJ( child_id, polys_id, "poly_xy" );

  RET_ARRAY_RD( poly_xy, child_id, "poly_xy" );
  
  memcpy( coordinates, poly_xy, sizeof(float) * total_nodes * 2 );

  
  ARRfree( (char *) coordinates );
  ARRfree( (char *) poly_xy );





  /*******************************************************************

    load the cell set with the polylines

  *******************************************************************/

  status = FLDadd_cell_set( outline_id, "Polyline" );
  if( status != OM_STAT_SUCCESS )
    {
      ARRfree( (char *)selected );
      MY_ERROR( "FLDadd_cell_set (outline_id, \"Polyline\" )");
      return(0);
    }

  status = FLDget_cell_set( outline_id, 0, &this_cell_set);
  if( status != OM_STAT_SUCCESS )
    {
      ARRfree( (char *)selected );
      MY_ERROR( "FLDget_cell_set( outline_id, 0, ..." );
      return(0);
    }

  status = FLDset_npolys( this_cell_set, selected_size );
  if( status != OM_STAT_SUCCESS )
    {
      ARRfree( (char *)selected );
      sprintf( chuge, "FLDset_npolys( this_cell_set, %i )", selected_size);
      MY_ERROR( chuge );
      return(0);
    }

  status= FLDget_poly_connect( this_cell_set, &poly_connect,
			      &size, OM_GET_ARRAY_RW );
  if( status != OM_STAT_SUCCESS )
    {
      ARRfree( (char *)selected );
      MY_ERROR( "FLDget_node_connect( this_cell_set,  ..." );
      return(0);
    }


  FIND_SUBOBJ( child_id,      polys_id, "poly_limits" );
  RET_ARRAY_RD( poly_limits, child_id, "poly_limits" );

  for( ii=0; ii<selected_size; ii++ ) {
     int oi = selected[ii];
     poly_connect[ 2*ii + 0 ] = poly_limits[ 2*oi + 0 ];
     poly_connect[ 2*ii + 1 ] = poly_limits[ 2*oi + 0 ] + poly_limits[ 2*oi + 1 ] - 1;
  }

  ARRfree( (char *) selected );
  ARRfree( (char *) poly_connect );

  /*******************************************************************

    bye-bye

  *******************************************************************/

  return(1);

}

