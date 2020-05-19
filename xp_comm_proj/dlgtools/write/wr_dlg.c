
/*
 * Write DLG Module
 */


#include <avs/f_utils.h>

#include "xp_comm_proj/dlgtools/dlg.h"
#include "xp_comm_proj/dlgtools/write/gen.h"


int
write_dlg_om(OMobj_id top_id,
               OMevent_mask event_mask,
               int seq_num)
{

  /*******************************************************************

    parameter variables

  *******************************************************************/

  OMobj_id  dlg_id;
  OMobj_id  filename_id;
  char     *filename;

  char filename_buffer[AVS_PATH_MAX];


  /*******************************************************************

    the rest

  *******************************************************************/

  int status;
  int size, type;

  OMobj_id    debug_id;
  int         debug;
  int         debug_flag;

  FILE *output_stream;


   /*******************************************************************

    procedures

  *******************************************************************/

  int write_dlg();

  /*******************************************************************

    generics

  *******************************************************************/

#include "../gen_proc.h"

  sprintf(  LFNC_function_name, "write_dlg_om" );


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

        get the objects

      ***************************************************************/

      FIND_SUBOBJ( dlg_id, top_id, "dlg" );

      FIND_SUBOBJ( filename_id, top_id, "filename" );

      filename = NULL;
      GET_STR_VAL( filename_id, &filename, "filename" );

      /* Use AVS/Express function to map enviroment variables into filename */
      FILEmap_variables((char *)filename, filename_buffer);


      /***************************************************************

        access the file

      ***************************************************************/

      output_stream = fopen( filename_buffer, "w" );

      if ( !output_stream )
        {
          sprintf( chuge, "fopen on file |%s|", filename_buffer );

          MY_ERROR( chuge );

          return(0);
        }

      /***************************************************************

        write the data

      ***************************************************************/

      status = write_dlg( output_stream,
                         dlg_id,

                        debug_flag );
      if( status != 1 ) return(0);

      /***************************************************************

        close the file

      ***************************************************************/

      status = fclose( output_stream );

      if ( status == EOF )
        {
          sprintf( chuge, "fclose on file |%s|", filename_buffer );

          MY_ERROR( chuge );

          return(0);
        }


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

  write the dlg file

*********************************************************************/
int write_dlg(
              FILE     *output_stream,
              OMobj_id  top_id,

              int debug_flag

              )

{

  /*******************************************************************

    om

  *******************************************************************/

  OMobj_id   header_id;
  OMobj_id   pb_id;



  /*******************************************************************

    the rest

  *******************************************************************/

  int status, size, type;

  int      int_val;
  double   real_val;
  char   *string_val;

  float  *real_array;
  int    *int_array;

  int     n_nodes, n_lines, n_areas, n_attributes;
  int     line_n_nodes, area_n_lines, node_n_lines;

  float  *xy;
  int    *end_nodes;
  int    *side_areas;
  int    *lines;
  int    *attributes;
  float  *nodes;

  OMobj_id   child_id;
  OMobj_id   entry_id;
  OMobj_id   entry_child_id;

  int ii, inode, iline, iarea;
  int num;

  /*******************************************************************

    generics

  *******************************************************************/

#include "../gen_proc.h"

  sprintf(  LFNC_function_name, "write_dlg" );



  /*******************************************************************

    the header

  *******************************************************************/
  {

    /* get the object */
    FIND_SUBOBJ( header_id, top_id, "header" );


    /*****************************************************************

      make sure we have data

    *****************************************************************/

    int_val = 0;
    FIND_SUBOBJ( child_id, header_id, "have_header" );
    GET_INT_VAL( child_id, &int_val, "have_header" );
    if( status != OM_STAT_SUCCESS || int_val != 1 )
      {
        fprintf( output_stream, "No Header !!!" );
      }
    else
      {

        /*************************************************************

          banner0

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "banner0" );
        string_val = NULL;
        GET_STR_VAL( child_id, &string_val, "banner0" );

        fprintf( output_stream, "%72s\n", string_val );

        MY_FREE( string_val );

        /*************************************************************

          banner1

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "banner1" );
        string_val = NULL;
        GET_STR_VAL( child_id, &string_val, "banner1" );

        fprintf( output_stream, "%52s", string_val );

        MY_FREE( string_val );

        /*************************************************************

          scale

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "scale" );

        GET_INT_VAL( child_id, &int_val, "scale" );

        fprintf( output_stream, "%8i\n\n", int_val );

        /*************************************************************

          dlg_level

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "dlg_level" );

        GET_INT_VAL( child_id, &int_val, "dlg_level" );

        fprintf( output_stream, "%6i", int_val );

        /*************************************************************

          planimetric_zone

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "planimetric_zone" );

        GET_INT_VAL( child_id, &int_val,  "planimetric_zone" );

        fprintf( output_stream, "%6i", int_val );


        /*************************************************************

          planimetric_system

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "planimetric_system" );

        GET_INT_VAL( child_id, &int_val, "planimetric_system" );

        fprintf( output_stream, "%6i", int_val );


        /*************************************************************

          planimetric_units

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "planimetric_units" );

        GET_INT_VAL( child_id, &int_val, "planimetric_units" );

        fprintf( output_stream, "%6i", int_val );


        /*************************************************************

          resolution

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "resolution" );

        GET_REAL_VAL( child_id, &real_val, "resolution" );

        fprintf( output_stream, "%18.11e", real_val );


        /*************************************************************

          n_ftm_trans_params

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "n_ftm_trans_params" );

        GET_INT_VAL( child_id, &int_val,  "n_ftm_trans_params" );

        fprintf( output_stream, "%6i", int_val );


        /*************************************************************

          n_accuracy_records

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "n_accuracy_records" );

        GET_INT_VAL( child_id, &int_val,  "n_accuracy_records" );

        fprintf( output_stream, "%6i", int_val );


        /*************************************************************

          n_control_points

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "n_control_points" );

        GET_INT_VAL( child_id, &int_val,  "n_control_points" );

        fprintf( output_stream, "%6i", int_val );


        /*************************************************************

          n_categories

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "n_categories" );

        GET_INT_VAL( child_id, &int_val, "n_categories" );

        fprintf( output_stream, "%6i\n", int_val );


        /*************************************************************

          projection_params

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "projection_params" );

        RET_ARRAY_RD( real_array, child_id, "projection_params" );

        for( ii=0; ii<15; ii++ )
          {
            fprintf( output_stream, "%24.15e", real_array[ii] );
            if( (ii+1) % 3 == 0 ) fprintf( output_stream, "\n" );
          }

        ARRfree( (char *) real_array );


        /*************************************************************

          file2map_params

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "file2map_params" );

        RET_ARRAY_RD( real_array, child_id, "file2map_params" );

        for( ii=0; ii<4; ii++ )
          {
            fprintf( output_stream, "%18.11e", real_array[ii] );
          }
        fprintf( output_stream,"\n" );


        ARRfree( (char *) real_array );


        /*************************************************************

          corner_names & corner data

        *************************************************************/

        FIND_SUBOBJ( child_id, header_id, "corner_data" );

        RET_ARRAY_RD( real_array, child_id, "corner_data" );

        FIND_SUBOBJ( child_id, header_id, "corner_names" );

        for( ii=0; ii<4; ii++ )
          {
            string_val = NULL;
            GET_STR_ARRAY_VAL( child_id, ii, &string_val );

            fprintf( output_stream,
                    "%2s    %12.6f%12.6f      %12.2f%12.2f\n",
                    string_val,
                    real_array[4*ii+0], real_array[4*ii+1],
                    real_array[4*ii+2], real_array[4*ii+3] );

            MY_FREE( string_val );
          }



        ARRfree( (char *) real_array );



      }


  }


  /*******************************************************************

    the pb

  *******************************************************************/
  {

    /* get the object */
    FIND_SUBOBJ( pb_id, top_id, "pb" );


    /*****************************************************************

      make sure we have data

    *****************************************************************/

    int_val = 0;
    FIND_SUBOBJ( child_id, pb_id, "have_political_boundaries" );
    GET_INT_VAL( child_id, &int_val, "have_political_boundaries" );
    if( status != OM_STAT_SUCCESS || int_val != 1 )
      {
        fprintf( output_stream, "No Political Boundaries !!!" );
      }
    else
      {


        /*************************************************************

          initialize

        *************************************************************/

        fprintf( output_stream, "%20s", "POLITICAL BOUNDARIES" );


        /*************************************************************

          att_code

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "att_code" );

        GET_INT_VAL( child_id, &int_val, "att_code" );

        fprintf( output_stream, "%4i", int_val );



        /*************************************************************

          n_nodes_ref

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "n_nodes_ref" );

        GET_INT_VAL( child_id, &int_val, "n_nodes_ref" );

        fprintf( output_stream, "%6i", int_val );


        /*************************************************************

          n_nodes

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "n_nodes" );

        GET_INT_VAL( child_id, &n_nodes, "n_nodes" );

        fprintf( output_stream, "%6i ", n_nodes );


        /*************************************************************

          node_to_area_flag

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "node_to_area_flag" );

        GET_INT_VAL( child_id, &int_val, "node_to_area_flag" );

        fprintf( output_stream, "%1i", int_val );


        /*************************************************************

          node_to_line_flag

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "node_to_line_flag" );

        GET_INT_VAL( child_id, &int_val, "node_to_line_flag" );

        fprintf( output_stream, "%1i ", int_val );


        /*************************************************************

          n_areas_ref

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "n_areas_ref" );

        GET_INT_VAL( child_id, &int_val, "n_areas_ref" );

        fprintf( output_stream, "%6i", int_val );


        /*************************************************************

          n_areas

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "n_areas" );

        GET_INT_VAL( child_id, &n_areas, "n_areas" );

        fprintf( output_stream, "%6i ", n_areas );


        /*************************************************************

          area_to_node_flag

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "area_to_node_flag" );

        GET_INT_VAL( child_id, &int_val, "area_to_node_flag" );

        fprintf( output_stream, "%1i", int_val );


        /*************************************************************

          area_to_line_flag

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "area_to_line_flag" );

        GET_INT_VAL( child_id, &int_val, "area_to_line_flag" );

        fprintf( output_stream, "%1i", int_val );


        /*************************************************************

        area_coordinates_flag

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "area_coordinates_flag" );

        GET_INT_VAL( child_id, &int_val, "area_coordinates_flag" );

        fprintf( output_stream, "%1i", int_val );


        /*************************************************************

          n_lines_ref

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "n_lines_ref" );

        GET_INT_VAL( child_id, &int_val, "n_lines_ref" );

        fprintf( output_stream, "%6i", int_val );


        /*************************************************************

          n_lines

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "n_lines" );

        GET_INT_VAL( child_id, &n_lines, "n_lines" );

        fprintf( output_stream, "%6i   ", n_lines );


        /*************************************************************

        line_coordinates_flag

        *************************************************************/

        FIND_SUBOBJ( child_id, pb_id, "line_coordinates_flag" );

        GET_INT_VAL( child_id, &int_val, "line_coordinates_flag" );

        fprintf( output_stream, "%1i\n", int_val );


        /*************************************************************

          the nodes

        *************************************************************/
        {
          FIND_SUBOBJ( child_id, pb_id, "nodes" );

          for( inode=0; inode<n_nodes; inode++ )
            {

              GET_ARRAY_VAL( child_id, inode, &entry_id,  "nodes" );

              /*******************************************************

                xy's

              *******************************************************/
              FIND_SUBOBJ( entry_child_id, entry_id, "xy" );

              RET_ARRAY_RD( xy, entry_child_id, "xy" );

              /*******************************************************

                number of lines

              *******************************************************/

              FIND_SUBOBJ( entry_child_id, entry_id, "n_lines" );
              GET_INT_VAL( entry_child_id, &node_n_lines, "n_lines" );

              /*******************************************************

                lines

              *******************************************************/

              if( node_n_lines > 0 )
                {
                  FIND_SUBOBJ( entry_child_id, entry_id, "lines" );

                  RET_ARRAY_RD( lines, entry_child_id, "lines" );
                }


              /*******************************************************

                write

              *******************************************************/

              fprintf( output_stream,
                      "N%5i%12.2f%12.2f%6i%6i%6i%6i%6i%6i\n",
                      inode+1,
                      xy[0],
                      xy[1],
                      0, /* unused */
                      node_n_lines,
                      0, /* unused */
                      0,
                      0, /* unused */
                      0  /* unused */
                      );

	      for( ii=0; ii<node_n_lines; ii++ )
                {
                  fprintf( output_stream, "%6i", lines[ii] );
                  if( (ii+1) % 12 == 0 || ii+1 == node_n_lines )
                    { fprintf( output_stream, "\n" ); }
                }

              /*******************************************************

                clean-up

              *******************************************************/

              ARRfree( (char *) xy );
              if( node_n_lines > 0 )
                { ARRfree( (char *) lines ); }


            }
        }


        /*************************************************************

          the areas

        *************************************************************/
        {
          FIND_SUBOBJ( child_id, pb_id, "areas" );

          for( iarea=0; iarea<n_areas; iarea++ )
            {

              GET_ARRAY_VAL( child_id, iarea, &entry_id,  "areas" );

              /*******************************************************

                xy's

              *******************************************************/

              FIND_SUBOBJ( entry_child_id, entry_id, "xy" );

              RET_ARRAY_RD( xy, entry_child_id, "xy" );


              /*******************************************************

                number of lines

              *******************************************************/

              FIND_SUBOBJ( entry_child_id, entry_id, "n_lines" );
              GET_INT_VAL( entry_child_id, &area_n_lines, "n_lines" );

              /*******************************************************

                lines

              *******************************************************/

              FIND_SUBOBJ( entry_child_id, entry_id, "lines" );

              RET_ARRAY_RD( lines, entry_child_id, "lines" );


              /*******************************************************

                number of attributes

              *******************************************************/

              FIND_SUBOBJ( entry_child_id, entry_id, "n_attributes" );
              GET_INT_VAL( entry_child_id, &n_attributes,
                          "n_attributes" );


              /*******************************************************

                attributes

              *******************************************************/
              if( n_attributes > 0 )
                {
                  FIND_SUBOBJ( entry_child_id, entry_id,
                              "attributes" );

                  RET_ARRAY_RD( attributes, entry_child_id,
                                "attributes" );

                }

              /*******************************************************

                write

              *******************************************************/

              fprintf( output_stream,
                      "A%5i%12.2f%12.2f%6i%6i%6i%6i%6i%6i\n",
                      iarea+1,
                      xy[0],
                      xy[1],
                      0, /* unused */
                      area_n_lines,
                      0, /* unused */
                      n_attributes,
                      0, /* unused */
                      0  /* unused */
                      );

              for( ii=0; ii<area_n_lines; ii++ )
                {
                  fprintf( output_stream, "%6i", lines[ii] );
                  if( (ii+1) % 12 == 0 || ii+1 == area_n_lines )
                    { fprintf( output_stream, "\n" ); }
                }
              for( ii=0; ii<n_attributes; ii++ )
                {
                  fprintf( output_stream, "%6i%6i",
                          attributes[2*ii+0],
                          attributes[2*ii+1] );
                  if( (ii+1) % 6 == 0 || (ii+1) == n_attributes )
                    { fprintf( output_stream, "\n" ); }
                }


              /*******************************************************

                cleanup

              *******************************************************/

              ARRfree( (char *) xy );
              ARRfree( (char *) lines );
              if( n_attributes > 0 )
                { ARRfree( (char *) attributes ); }

            }

        }

        /*************************************************************

          the lines

        *************************************************************/
        {
          FIND_SUBOBJ( child_id, pb_id, "lines" );

          for( iline=0; iline<n_lines; iline++ )
            {

              GET_ARRAY_VAL( child_id, iline, &entry_id,  "lines" );

              /*******************************************************

                end nodes

              *******************************************************/

              FIND_SUBOBJ( entry_child_id, entry_id, "end_nodes" );

              RET_ARRAY_RD( end_nodes, entry_child_id, "end_nodes" );


              /*******************************************************

                side areas

              *******************************************************/

              FIND_SUBOBJ( entry_child_id, entry_id, "side_areas" );

              RET_ARRAY_RD( side_areas, entry_child_id, "side_areas");


              /*******************************************************

                number of nodes

              *******************************************************/

              FIND_SUBOBJ( entry_child_id, entry_id, "n_nodes" );
              GET_INT_VAL( entry_child_id, &line_n_nodes, "n_nodes" );

              /*******************************************************

                nodes

              *******************************************************/

              FIND_SUBOBJ( entry_child_id, entry_id, "nodes" );

              RET_ARRAY_RD( nodes, entry_child_id, "nodes" );


              /*******************************************************

                number of attributes

              *******************************************************/

              FIND_SUBOBJ( entry_child_id, entry_id, "n_attributes" );
              GET_INT_VAL( entry_child_id, &n_attributes,
                          "n_attributes" );


              /*******************************************************

                attributes

              *******************************************************/
              if( n_attributes > 0 )
                {
                  FIND_SUBOBJ( entry_child_id, entry_id,
                              "attributes" );

                  RET_ARRAY_RD( attributes, entry_child_id,
                                "attributes" );

                }

              /*******************************************************

                write

              *******************************************************/

              fprintf( output_stream,
                      "L%5i%6i%6i%6i%6i            %6i%6i%6i\n",
                      iline+1,
                      end_nodes[0],
                      end_nodes[1],
                      side_areas[0],
                      side_areas[1],
                      line_n_nodes,
                      n_attributes,
                      0  /* unused */
                      );

              for( ii=0; ii<line_n_nodes; ii++ )
                {
                  fprintf( output_stream, "%12.2f%12.2f",
                          nodes[2*ii+0],
                          nodes[2*ii+1] );
                  if( (ii+1) % 3 == 0 || ii+1 == line_n_nodes )
                    { fprintf( output_stream, "\n" ); }
                }
              for( ii=0; ii<n_attributes; ii++ )
                {
                  fprintf( output_stream, "%6i%6i",
                          attributes[2*ii+0],
                          attributes[2*ii+1] );
                  if( (ii+1) % 6 == 0 || (ii+1) == n_attributes )
                    { fprintf( output_stream, "\n" ); }
                }



              /*******************************************************

                cleanup

              *******************************************************/

              ARRfree( (char *) end_nodes );
              ARRfree( (char *) side_areas );
              ARRfree( (char *) nodes );
              if( n_attributes > 0 )
                { ARRfree( (char *) attributes ); }

            }




        }

      }



  }






  /*******************************************************************

    bye-bye

  *******************************************************************/

  return(1);

}

