
/*
 * Read DLG Module
 */


#include <avs/f_utils.h>

#include "xp_comm_proj/dlgtools/dlg.h"
#include "xp_comm_proj/dlgtools/read/gen.h"


/* nodes */
typedef struct {

  float   xy[2];
  int     n_lines;
  int    *lines;

} Node;

/* lines */
typedef struct {

  int     end_nodes[2];
  int     side_areas[2];
  int     n_nodes;
  float  *nodes;
  int     n_attributes;
  int    *attributes;

} Line;

/* areas */
typedef struct {

  float   xy[2];
  int     n_lines;
  int    *lines;
  int     n_attributes;
  int    *attributes;

} Area;

/* poly */
typedef struct {

  int     n_polys;
  int    *poly_area;
  int    *poly_limits;
  int     total_nodes;
  float  *poly_xy;

} Poly;


/* header */
typedef struct {

  int    have_header;

  char   banner0[73];
  char   banner1[73];
  int    scale;

  int dlg_level;
  int planimetric_zone;
  int planimetric_system;
  int planimetric_units;
  float resolution;
  int n_ftm_trans_params;
  int n_accuracy_records;
  int n_control_points;
  int n_categories;

  float projection_params[15];

  float file2map_params[4];

  char  corner_names[4][3];
  float corner_data[4][4];

} Header;

 /* political boundaries */
typedef struct {

  int have_political_boundaries;

  int  att_code;
  int  n_nodes_ref;
  int  n_nodes;
  int  node_to_area_flag;
  int  node_to_line_flag;
  int  n_areas_ref;
  int  n_areas;
  int  area_to_node_flag;
  int  area_to_line_flag;
  int  area_coordinates_flag;
  int  n_lines_ref;
  int  n_lines;
  int  line_coordinates_flag;

  Node *nodes;

  Line *lines;

  Area *areas;

} Pb;


typedef struct {

  /* header */
  Header header;

  /* political boundaries */
  Pb pb;

  /* the polygons from the pb's */
  Poly polys;

} Dlg;


#define TEST(a) if( status != a ) { sprintf( chuge, "read #%i", read_num ); MY_ERROR( chuge ); return(0); } read_num++

#define PARSE(a,b) memcpy( chuge, line+a, sizeof(char)*b ); chuge[b] = '\0'

#define READLINE status = fscanf( input_stream, "%80c", line ); line[80] = '\0'


int
read_dlg_om(OMobj_id top_id,
	    OMevent_mask event_mask,
	    int seq_num)
{

  /*******************************************************************

    parameter variables

  *******************************************************************/

  OMobj_id    filename_id;
  char       *filename;

  char filename_buffer[AVS_PATH_MAX];


  /*******************************************************************

    the rest

  *******************************************************************/

  int status;
  int size, type;

  OMobj_id    debug_id;
  int         debug;
  int         debug_flag;

  OMobj_id    dlg_id;
  OMobj_id    header_id;
  OMobj_id    pb_id;
  OMobj_id    file_info_id;
  OMobj_id    child_id;

  /*******************************************************************

    data

  *******************************************************************/

  Dlg *dlg;

  int ram_block;
  
  /*******************************************************************

    procedures

  *******************************************************************/

  int read_dlg();
  int process_dlg();
  int load_dlg();

  /*******************************************************************

    generics

  *******************************************************************/

#include "../gen_proc.h"

  sprintf(  LFNC_function_name, "read_dlg_om" );



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


      /***************************************************************

	immediately set output flags to be zero

      ***************************************************************/

      FIND_SUBOBJ( dlg_id, top_id, "dlg_out" );
      
      FIND_SUBOBJ( header_id, dlg_id, "header" );
      FIND_SUBOBJ( pb_id,     dlg_id, "pb"     );

      FIND_SUBOBJ( child_id, header_id, "have_header" );
      SET_INT_VAL( child_id, 0,         "have_header" );

      FIND_SUBOBJ( child_id, pb_id, "have_political_boundaries" );
      SET_INT_VAL( child_id, 0,     "have_political_boundaries" );
      
	
      /***************************************************************

	get the file name

      ***************************************************************/

      FIND_SUBOBJ( filename_id, top_id, "filename" );

      if( !  OMchanged( filename_id, seq_num  ) )
	{
	  MY_ERROR( "filename not changed but method fired anyway" );
	  return(0);
	}

      filename = NULL;
      GET_STR_VAL( filename_id, &filename, "filename" );

      /* Use AVS/Express function to map enviroment variables into filename */
      FILEmap_variables((char *)filename, filename_buffer);


      /***************************************************************

	initilaize the memory pool

      ***************************************************************/

      ram_block = ram_init( 8092 );
      if( ram_block == - 1 ) return( 0 );

      /***************************************************************

	initialize the structure

      ***************************************************************/

      dlg = NULL;
      dlg = (Dlg *) ram_more( ram_block,
			     sizeof( Dlg ) );
      if( dlg == NULL )
	{
	  MY_ERROR( "malloc on dlg" );
	  ram_free( ram_block );
	  return(0);
	}

      /***************************************************************

	read the data

      ***************************************************************/

      status = read_dlg( filename_buffer,
			dlg,
			ram_block,

			debug_flag );
      if( status != 1 )
	{
	  ram_free( ram_block );
	  return(0);
	}


      /***************************************************************

	process the data

      ***************************************************************/

      status = process_dlg( dlg,
			   ram_block,

			   debug_flag );
      if( status != 1 )
	{
	  ram_free( ram_block );
	  return(0);
	}
      
      /***************************************************************

	load the output structure

      ***************************************************************/

      dlg_id = OMfind_subobj( top_id,
			     OMstr_to_name("dlg_out"),
			     OM_OBJ_RW );
      if( OMis_null_obj( dlg_id ) )
	{
	  MY_ERROR( "can't find dlg_ids" );
	  ram_free( ram_block );
	  return(0);
	}

      status = load_dlg( dlg_id,
			dlg,

			debug_flag );
      if( status != 1 )
	{
	  ram_free( ram_block );
	  return(0);
	}


      /***************************************************************

	load the information string

      ***************************************************************/

      FIND_SUBOBJ( file_info_id, top_id, "file_info" );

      status = load_file_info( file_info_id,
			      filename_buffer,
			      dlg,

			      debug_flag );
      if( status != 1 )
	{
	  ram_free( ram_block );
	  return(0);
	}


      /***************************************************************

	free the memory pool

      ***************************************************************/

      if( debug_flag == 1 ) ram_dump();
      status = ram_free( ram_block );
      if( status == - 1 ) return( 0 );


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

  read the dlg file and load it into out structure

*********************************************************************/

int read_dlg(
             char  *filename,
             Dlg  *dlg,
	     int   ram_block,

             int debug_flag

             )

{


  /*******************************************************************

    parameters

  *******************************************************************/

  /* declare locals */
  FILE *input_stream;



  /*******************************************************************

    the rest

  *******************************************************************/

  int status, ii;

  int read_num = 0;

  char line[81];
  int  idata[14];

  Header *header;
  Pb     *pb;

  /*******************************************************************

    precedures

  *******************************************************************/

  int read_dlg_pb();

  /*******************************************************************

    generics

  *******************************************************************/

#include "../gen_proc.h"

  sprintf(  LFNC_function_name, "read_dlg" );



  /*******************************************************************

    set some pointers

  *******************************************************************/
  header = &(dlg->header);
  pb     = &(dlg->pb);


  /*******************************************************************

     set some flags

  *******************************************************************/

  header->have_header = 0;
  pb->have_political_boundaries = 0;



  /*******************************************************************

    access the file

  *******************************************************************/

  input_stream = fopen( filename, "r" );

  if ( !input_stream )
    {
      sprintf( chuge, "fopen on file |%s|", filename );

      MY_ERROR( chuge );

      return(0);
    }



  /*******************************************************************

    read the header

  *******************************************************************/
  {
    READLINE;
    TEST(1);

    PARSE(0,72);
    sprintf( header->banner0, "%s", chuge );

    READLINE;
    TEST(1);

    PARSE(0,52);
    sprintf( header->banner1, "%s", chuge );

    PARSE(52,8);
    header->scale = atoi( chuge );

    READLINE;

    READLINE;
    PARSE(0,6);
    header->dlg_level = atoi( chuge );

    PARSE(6,6);
    header->planimetric_zone = atoi( chuge );

    PARSE(12,6);
    header->planimetric_system = atoi( chuge );

    PARSE(18,6);
    header->planimetric_units = atoi( chuge );

    PARSE(24,18);
    header->resolution = atof( chuge );

    PARSE(42,6);
    header->n_ftm_trans_params = atoi( chuge );

    PARSE(48,6);
    header->n_accuracy_records = atoi( chuge );

    PARSE(54,6);
    header->n_control_points = atoi( chuge );

    PARSE(60,6);
    header->n_categories = atoi( chuge );


    READLINE;
    PARSE(0,24);
    header->projection_params[ 0] = atof( chuge );

    PARSE(24,24);
    header->projection_params[ 1] = atof( chuge );

    PARSE(48,24);
    header->projection_params[ 2] = atof( chuge );

    READLINE;
    PARSE(0,24);
    header->projection_params[ 3] = atof( chuge );

    PARSE(24,24);
    header->projection_params[ 4] = atof( chuge );

    PARSE(48,24);
    header->projection_params[ 5] = atof( chuge );

    READLINE;
    PARSE(0,24);
    header->projection_params[ 6] = atof( chuge );

    PARSE(24,24);
    header->projection_params[ 7] = atof( chuge );

    PARSE(48,24);
    header->projection_params[ 8] = atof( chuge );

    READLINE;
    PARSE(0,24);
    header->projection_params[ 9] = atof( chuge );

    PARSE(24,24);
    header->projection_params[10] = atof( chuge );

    PARSE(48,24);
    header->projection_params[11] = atof( chuge );

    READLINE;
    PARSE(0,24);
    header->projection_params[12] = atof( chuge );

    PARSE(24,24);
    header->projection_params[13] = atof( chuge );

    PARSE(48,24);
    header->projection_params[14] = atof( chuge );


    READLINE;
    PARSE(0,18);
    header->file2map_params[ 0] = atof( chuge );

    PARSE(18,18);
    header->file2map_params[ 1] = atof( chuge );

    PARSE(36,18);
    header->file2map_params[ 2] = atof( chuge );

    PARSE(54,18);
    header->file2map_params[ 3] = atof( chuge );


    for( ii=0; ii<4; ii++ )
      {
	READLINE;
	PARSE(0,2);
	sprintf( header->corner_names[ii], "%s", chuge );

	PARSE(6,12);
	header->corner_data[ii][0] = atof( chuge );

	PARSE(18,12);
	header->corner_data[ii][1] = atof( chuge );

	PARSE(36,12);
	header->corner_data[ii][2] = atof( chuge );

	PARSE(48,12);
	header->corner_data[ii][3] = atof( chuge );

      }

    header->have_header = 1;

    if( debug_flag == 1 )
      {
        printf( "banner0 %s\n",
               header->banner0 );
        printf( "banner1 %s\n",
               header->banner1 );
        printf( "scale %i\n",
               header->scale );

        printf( "\n" );

        printf( "dlg_level          %i\n",
	       header->dlg_level );
        printf( "planimetric_zone   %i\n",
	       header->planimetric_zone );
        printf( "planimetric_system %i\n",
	       header->planimetric_system );
        printf( "planimetric_units  %i\n",
	       header->planimetric_units );
        printf( "resolution>        %g\n",
	       header->resolution );
        printf( "n_ftm_trans_params %i\n",
	       header->n_ftm_trans_params );
        printf( "n_accuracy_records %i\n",
	       header->n_accuracy_records );
        printf( "n_control_points   %i\n",
	       header->n_control_points );
        printf( "n_categories       %i\n",
	       header->n_categories );

        printf( "\n" );

        printf( "projection_parameters ...\n" );
        printf( "%24.15e %24.15e %24.15e\n",
               header->projection_params[ 0],
               header->projection_params[ 1],
               header->projection_params[ 2] );
        printf( "%24.15e %24.15e %24.15e\n",
               header->projection_params[ 3],
               header->projection_params[ 4],
               header->projection_params[ 5] );
        printf( "%24.15e %24.15e %24.15e\n",
               header->projection_params[ 6],
               header->projection_params[ 7],
               header->projection_params[ 8] );
        printf( "%24.15e %24.15e %24.15e\n",
               header->projection_params[ 9],
               header->projection_params[10],
               header->projection_params[11] );
        printf( "%24.15e %24.15e %24.15e\n",
               header->projection_params[12],
               header->projection_params[13],
               header->projection_params[14] );

        printf( "\n" );

        printf( "file2map_parameters ...\n" );
        printf( "%24.15e %24.15e %24.15e %24.15e\n",
               header->file2map_params[ 0],
               header->file2map_params[ 1],
               header->file2map_params[ 2],
               header->file2map_params[ 3] );

        printf( "\n" );

        printf( "corners ...\n" );
        printf( "%s %12.6f %12.6f %12.2f %12.2f\n",
               header->corner_names[0],
               header->corner_data[0][0],
               header->corner_data[0][1],
               header->corner_data[0][2],
               header->corner_data[0][3] );
        printf( "%s %12.6f %12.6f %12.2f %12.2f\n",
               header->corner_names[1],
               header->corner_data[1][0],
               header->corner_data[1][1],
               header->corner_data[1][2],
               header->corner_data[1][3] );
        printf( "%s %12.6f %12.6f %12.2f %12.2f\n",
               header->corner_names[2],
               header->corner_data[2][0],
               header->corner_data[2][1],
               header->corner_data[2][2],
               header->corner_data[2][3] );
        printf( "%s %12.6f %12.6f %12.2f %12.2f\n",
               header->corner_names[3],
               header->corner_data[3][0],
               header->corner_data[3][1],
               header->corner_data[3][2],
               header->corner_data[3][3] );


        printf ("\n\n" );
      }
  }

  /*******************************************************************

    read the catergories

  *******************************************************************/
  {
    READLINE;
    TEST(1);

    if( debug_flag == 1 )
      {
        printf( "\n" );

        printf( "line      = %s\n", line );

        printf( "\n" );
      }

    if( strncmp( line, "POLITICAL BOUNDARIES",
                strlen("POLITICAL BOUNDARIES") ) == 0 )
      {

        /*************************************************************

          parse the line

        *************************************************************/

        PARSE( 20,4 );
        pb->att_code = atoi( chuge );

        PARSE( 24,6 );
        pb->n_nodes_ref = atoi( chuge );

        PARSE( 30,6 );
        pb->n_nodes = atoi( chuge );

        PARSE( 37,1 );
        pb->node_to_area_flag = atoi( chuge );

        PARSE( 38,1 );
        pb->node_to_line_flag = atoi( chuge );

        PARSE( 40,6 );
        pb->n_areas_ref = atoi( chuge );

        PARSE( 46,6 );
        pb->n_areas = atoi( chuge );

        PARSE( 53,1 );
        pb->area_to_node_flag = atoi( chuge );

        PARSE( 54,1 );
        pb->area_to_line_flag = atoi( chuge );

        PARSE( 55,1 );
        pb->area_coordinates_flag = atoi( chuge );

        PARSE( 56,6 );
        pb->n_lines_ref = atoi( chuge );

        PARSE( 62,6 );
        pb->n_lines = atoi( chuge );

        PARSE( 71,1 );
        pb->line_coordinates_flag = atoi( chuge );


        if( debug_flag == 1 )
          {
            printf( "\n" );

            printf( "pb_att_code               = %i\n",
                  pb->att_code );
            printf( "pb_n_nodes_ref            = %i\n",
                  pb->n_nodes_ref );
            printf( "pb_n_nodes                = %i\n",
                  pb->n_nodes );
            printf( "pb_node_to_area_flag      = %i\n",
                  pb->node_to_area_flag );
            printf( "pb_node_to_line_flag      = %i\n",
                  pb->node_to_line_flag );
            printf( "pb_n_areas_ref            = %i\n",
                  pb->n_areas_ref );
            printf( "pb_n_areas                = %i\n",
                  pb->n_areas );
            printf( "pb_area_to_node_flag      = %i\n",
                  pb->area_to_node_flag );
            printf( "pb_area_to_line_flag      = %i\n",
                  pb->area_to_line_flag );
            printf( "pb_area_coordinates_flag  = %i\n",
                  pb->area_coordinates_flag );
            printf( "pb_n_lines_ref            = %i\n",
                  pb->n_lines_ref );
            printf( "pb_n_lines                = %i\n",
                  pb->n_lines );
            printf( "pb_line_coordinates_flag  = %i\n",
                  pb->line_coordinates_flag );

            printf( "\n" );
          }


	/*************************************************************

	  allocate memory for the internal structures

        *************************************************************/
	{

	  /***********************************************************

	    nodes

          ***********************************************************/

	  pb->nodes = (Node *) ram_more( ram_block,
					sizeof(Node)
					* pb->n_nodes );
	  if( pb->nodes == NULL )
	    {
	      MY_ERROR( "malloc for pb->nodes" );
	      return(0);
	    }


	  /***********************************************************

	    areas

          ***********************************************************/

	  pb->areas = (Area *) ram_more( ram_block,
					sizeof(Area)
					* pb->n_areas );
	  if( pb->areas == NULL )
	    {
	      MY_ERROR( "malloc for pb->areas" );
	      return(0);
	    }

	  /***********************************************************

	    lines

          ***********************************************************/

	  pb->lines = (Line *) ram_more( ram_block,
					sizeof(Line)
					* pb->n_lines );
	  if( pb->lines == NULL )
	    {
	      MY_ERROR( "malloc for pb->lines" );
	      return(0);
	    }


	}



        status = read_dlg_pb( input_stream,
                             pb,
			     ram_block,

                             debug_flag );

	if( status != 1 ) return( status );

	pb->have_political_boundaries = 1;

      }
    else
      {
        sprintf( chuge,
                "cannot handle => %s",
                line );
        MY_ERROR( chuge );
        return(0);
      }

  }

  /*******************************************************************

    close the file

  *******************************************************************/
  status = fclose( input_stream );

  if ( status == EOF )
    {
      sprintf( chuge, "fclose on file |%s|", filename );

      MY_ERROR( chuge );

      return(0);
    }


  /*******************************************************************

    bye-bye

  *******************************************************************/

  return(1);


}




int read_dlg_pb(
                FILE *input_stream,
                Pb  *pb,
		int ram_block,

                int debug_flag

                )

{


  /*******************************************************************

    the rest

  *******************************************************************/

  int status;

  char line[81];
  int  idata[14];

  int inode, iarea, iline;

  int jnode, jline, jarea;

  int read_num;

  int ii,jj;


  Area *this_area;
  Node *this_node;
  Line *this_line;

  /*******************************************************************

    generics

  *******************************************************************/

#include "../gen_proc.h"

  sprintf(  LFNC_function_name, "read_dlg_pb" );








  /*******************************************************************

    read the data

  *******************************************************************/
  inode = iarea = iline = 0;

  while( inode < pb->n_nodes ||
	 iarea < pb->n_areas ||
	 iline < pb->n_lines    )
    {
      READLINE;
      TEST(1);

      if( strncmp( line, "N", 1 ) == 0 ) /* a node */
	{

	  /***********************************************************

	    set a pointer to reduce confusion

          ***********************************************************/

	  this_node = &(pb->nodes[inode]);


	  /***********************************************************

	    read the data

          ***********************************************************/

	  PARSE(6,12);
	  this_node->xy[0] = atof( chuge );

	  PARSE(18,12);
	  this_node->xy[1] = atof( chuge );

	  /* # lines for this node */
	  PARSE(36,6);
	  this_node->n_lines = atoi( chuge );

	  /* memory for the lines */
	  this_node->lines = NULL;
	  if( this_node->n_lines > 0 )
	    {
	      this_node->lines
		= (int *) ram_more( ram_block,
				   sizeof(int)
				   *this_node->n_lines );
	      if( this_node->lines  == NULL )
		{
		  sprintf( chuge, "malloc pb->nodes[%i].lines", inode);
		  MY_ERROR( chuge );
		  return(0);
		}
	    }

	  /* read the lines (if any ) */
	  if( this_node->n_lines > 0 )
	    {
	      ii = 0;
	      while( ii<this_node->n_lines )
		{
		  jj = ii % 12;

		  if( jj == 0 )
		    {
		      READLINE;
		      TEST(1);
		    }

		  PARSE(jj*6,6);
		  this_node->lines[ii]= atoi(chuge);

		  ii += 1;
		}

	    }



	  if( debug_flag == 1 )
	    {
	      printf( "N %i = %12.2f %12.2f\n",
		     inode+1,
		     this_node->xy[0],
		     this_node->xy[1] );

	    }

	  inode += 1;
	}
      else if( strncmp( line, "A", 1 ) == 0 ) /* an area */
	{

	  /***********************************************************

	    set a pointer to reduce confusion

          ***********************************************************/

	  this_area = &(pb->areas[iarea]);


	  /***********************************************************

	    read the data

          ***********************************************************/

	  /* xy's of the area */
	  PARSE(6,12);
	  this_area->xy[0] = atof( chuge );

	  PARSE(18,12);
	  this_area->xy[1] = atof( chuge );

	  /* # lines for this area */
	  PARSE(36,6);
	  this_area->n_lines = atoi( chuge );

	  /* # attribute pairs for this area */
	  PARSE(48,6);
	  this_area->n_attributes = atoi( chuge );

	  /* memory for the lines */
	  this_area->lines = NULL;
	  this_area->lines
	    = (int *) ram_more( ram_block,
			       sizeof(int)
			       *this_area->n_lines );
	  if( this_area->lines  == NULL )
	    {
	      sprintf( chuge, "malloc pb->areas[%i].lines", iarea );
	      MY_ERROR( chuge );
	      return(0);
	    }

	  /* memory for the attributes */
	  this_area->attributes = NULL;
	  if( this_area->n_attributes > 0 )
	    {
	      this_area->attributes
		= (int *) ram_more( ram_block,
				   sizeof(int)
				   * 2*this_area->n_attributes );
	      if( this_area->attributes == NULL )
		{
		  sprintf( chuge,
			  "malloc  pb->areas[%i].attributes", iarea );
		}
	    }


	  /* read the lines for this area */
	  ii = 0;
	  while( ii<this_area->n_lines )
	    {
	      jj = ii % 12;

	      if( jj == 0 )
		{
		  READLINE;
		  TEST(1);
		}

	      PARSE(jj*6,6);
	      this_area->lines[ii]= atoi(chuge);

	      ii += 1;
	    }

	  /* read the attributes (if any) */
	  ii = 0;
	  while( ii<this_area->n_attributes )
	    {
	      jj = ii % 6;

	      if( jj == 0 )
		{
		  READLINE;
		  TEST(1);
		}

	      PARSE(jj*12+0,6);
	      this_area->attributes[2*ii+0]= atoi(chuge);

	      PARSE(jj*12+6,6);
	      this_area->attributes[2*ii+1]= atoi(chuge);

	      ii += 1;
	    }


	  if( debug_flag == 1 )
	    {
	      printf( "A %i = %12.2f %12.2f\n",
		     iarea+1,
		     this_area->xy[0],
		     this_area->xy[1] );
	      for( ii=0; ii< this_area->n_lines; ii++ )
		{
		  printf( "%6i",
			 this_area->lines[ii] );
		  if( ( (ii+1) % 12) == 0  ||
		      ii+1 == this_area->n_lines )
		    { printf("\n"); }
		}
	      for( ii=0; ii< this_area->n_attributes; ii++ )
		{
		  printf( "%6i%6i",
			 this_area->attributes[2*ii+0],
			 this_area->attributes[2*ii+1] );
		  if( ( (ii+1) % 6) == 0  ||
		     ii+1 == this_area->n_attributes )
		    { printf("\n"); }
		}

	    }


	  iarea += 1;
	}
      else if( strncmp( line, "L", 1 ) == 0 ) /* a line */
	{

	  /***********************************************************

	    set a pointer to reduce confusion

          ***********************************************************/

	  this_line = &(pb->lines[iline]);


	  /***********************************************************

	    read the data

          ***********************************************************/

	  /* starting and ending nodes for this line */
	  PARSE(6,6);
	  this_line->end_nodes[0] = atoi( chuge );

	  PARSE(12,6);
	  this_line->end_nodes[1] = atoi( chuge );

	  PARSE(18,6);
	  this_line->side_areas[0] = atoi( chuge );

	  PARSE(24,6);
	  this_line->side_areas[1] = atoi( chuge );

	  /* number of nodes for this line */
	  PARSE(42,6);
	  this_line->n_nodes = atoi( chuge );

	  /* number of attribute codes */
	  PARSE(48,6);
	  this_line->n_attributes = atoi( chuge );

	  /* memory for the nodes */
	  this_line->nodes = NULL;
	  this_line->nodes
	    = (float *) ram_more( ram_block,
				 sizeof(float)
				 * (2*this_line->n_nodes) );
	  if( this_line->nodes  == NULL )
	    {
	      sprintf( chuge, "malloc pb->lines[%i].nodes", iline );
	      MY_ERROR( chuge );
	      return(0);
	    }

	  /* memory for the attributes */
	  this_line->attributes = NULL;
	  if( this_line->n_attributes > 0 )
	    {
	      this_line->attributes
		= (int *) ram_more( ram_block,
				   sizeof(int)
				   * 2*this_line->n_attributes );
	      if( this_line->attributes == NULL )
		{
		  sprintf( chuge,
			  "malloc  pb->lines[%i].attributes", iline );
		}
	    }

	  /* read the x,y for each node in this line */
	  ii = 0;
	  while( ii<this_line->n_nodes )
	    {
	      jj = ii % 3;

	      if( jj == 0 )
		{
		  READLINE;
		  TEST(1);
		}

	      PARSE(jj*24+ 0,12);
	      this_line->nodes[2*ii+0]= atof(chuge);

	      PARSE(jj*24+12,12);
	      this_line->nodes[2*ii+1]= atof(chuge);

	      ii += 1;
	    }

	  /* read the attributes (if any) */
	  ii = 0;
	  while( ii<this_line->n_attributes )
	    {
	      jj = ii % 6;

	      if( jj == 0 )
		{
		  READLINE;
		  TEST(1);
		}

	      PARSE(jj*12+0,6);
	      this_line->attributes[2*ii+0]= atoi(chuge);

	      PARSE(jj*12+6,6);
	      this_line->attributes[2*ii+1]= atoi(chuge);

	      ii += 1;
	    }
	  if( debug_flag == 1 )
	    {

	      printf( "L %i = \n", iline+1 );
	      for( ii=0; ii< this_line->n_nodes; ii++ )
		{
		  printf( "%12.2f%12.2f",
			 this_line->nodes[2*ii+0],
			 this_line->nodes[2*ii+1] );
		  if( ( (ii+1) % 3) == 0  ||
		     ii+1 == this_line->n_nodes )
		    { printf("\n"); }
		}
	      for( ii=0; ii< this_line->n_attributes; ii++ )
		{
		  printf( "%6i%6i",
			 this_line->attributes[2*ii+0],
			 this_line->attributes[2*ii+1] );
		  if( ( (ii+1) % 6) == 0  ||
		     ii+1 == this_line->n_attributes )
		    { printf("\n"); }
		}

	     }
	  iline += 1;
	}
      else
	{
	  /* garbage we don't want */
	}

    }





  /*******************************************************************

    bye-bye

  *******************************************************************/

  return(1);
}







/*********************************************************************

  process the dlg file to remove garbage

*********************************************************************/

int process_dlg(
		Dlg  *dlg,
		int ram_block,
		
		int debug_flag

		)

{

  

  /*******************************************************************

    parameters

  *******************************************************************/
  


  /*******************************************************************

    the rest

  *******************************************************************/

  int status, ii, jj, kk, mm;

  Header  *header;
  Pb      *pb;
  Node    *node;
  Area    *area;
  Line    *line;
  Poly    *poly;
  
  int n_nodes, n_lines, n_areas;
  int inode, iline, iarea, ivalid;

  int total_nodes;
  int added_nodes;

  int *valid;
  int n_valid;
  
  /*******************************************************************

    generics

  *******************************************************************/

#include "../gen_proc.h"

  sprintf(  LFNC_function_name, "read_dlg" );



  /*******************************************************************

    set some pointers

  *******************************************************************/
  header = &(dlg->header);
  pb     = &(dlg->pb);

  poly  = &(dlg->polys);

  
  /*******************************************************************

    remove unneeded areas

  *******************************************************************/
  {


    /*****************************************************************

      allocate

    *****************************************************************/

    valid = ( int * ) ram_more( ram_block,
			       sizeof(int)
			       * pb->n_areas );
    if( valid == NULL )
      {
	MY_ERROR( "malloc on data->valid" );
	return(0);
      }

    /*****************************************************************

      search

    *****************************************************************/
    n_valid = 0;
    
    for( iarea=0; iarea < pb->n_areas; iarea++ )
      {

	/*************************************************************

	  point

        *************************************************************/

	area = &(pb->areas[iarea]);


	/*************************************************************

	  initialize to valid

        *************************************************************/

	valid[n_valid] = 1;
	

	/*************************************************************

	  check attributes and decide

        *************************************************************/

	if( area->n_attributes  <  2  ||
	    area->attributes[0] != 91 ||
	    area->attributes[2] != 92    )
	  {
	    valid[ n_valid ] = 0;
	  }
	
	/*************************************************************

	  make sure the area is actually a single area and not
	  multplle loops

        *************************************************************/

	for( ii=0; ii<area->n_lines; ii++ )
	  {
	    if( abs(area->lines[ii]) < 1 )
	      {
		valid[ n_valid ] = 0;
	      }
	  }
	     
	/*************************************************************

	  add if still valid (i.e passed all tests)

        *************************************************************/

	if( valid[n_valid] == 1 )
	  {
	    valid[n_valid] = iarea;
	    n_valid += 1;
	  }

      }

    if( n_valid < 1 )
      {
	MY_MESSAGE( "no valid areas");
	return(0);
      }


  }


  /*******************************************************************

    compute closed polylines for each area

  *******************************************************************/
  {


    /*****************************************************************

      set the number of polys' and allocate for polyline limits
      and referernce to original area

    *****************************************************************/

    poly->n_polys = n_valid;

    poly->poly_limits = (int *) ram_more( ram_block,
					 sizeof(int)
					 * poly->n_polys * 2 );
    if( poly->poly_limits == NULL )
      {
	MY_ERROR( "malloc poly->poly_limits" );
	return(0);
      }
    
    poly->poly_area = (int *) ram_more( ram_block,
				       sizeof(int)
				       * poly->n_polys );
    if( poly->poly_area == NULL )
      {
	MY_ERROR( "malloc poly->poly_area" );
	return(0);
      }
    
    
    /*****************************************************************

      set the referencing

    *****************************************************************/

    for( ivalid=0; ivalid<n_valid; ivalid++ )
      {
	poly->poly_area[ ivalid ] = valid[ ivalid ];
      }

    /*****************************************************************

      first determine the totals for allocation

      foreach area, add up the nodes on all the lines. subtract one
      for each line since the ends overlap. and unity to the sum
      since the ends must overlap to close

    *****************************************************************/

    total_nodes = 0;
    for( ivalid=0; ivalid<n_valid; ivalid++ )
      {

	iarea = valid[ ivalid ];
	area = &(pb->areas[iarea]);

	for( ii=0; ii<area->n_lines; ii++ )
	  {
	    iline = abs(area->lines[ii]) - 1;
	    
	    line = &(pb->lines[ iline ] );
	    
	    total_nodes += line->n_nodes - 1;

	  }

	total_nodes += 1;

       }

    poly->total_nodes = total_nodes;
    
    /*****************************************************************

      now allocate for all the nodes for all the polylines

    *****************************************************************/

    poly->poly_xy = (float *) ram_more( ram_block,
				       sizeof(float)
				       * total_nodes * 2 );

    if( poly->poly_xy == NULL )
      {
	MY_ERROR( "malloc on poly_xy" );
	return(0);
      }


    /*****************************************************************

      now compute the true polygons and load into arrays. also load
      the pointers to the limits for each polyline

    *****************************************************************/

    total_nodes = 0;

    for( ivalid=0; ivalid<n_valid; ivalid++ )
      {

	iarea = valid[ ivalid ];
	
	area = &(pb->areas[iarea]);

	added_nodes = 0;

	for( ii=0; ii<area->n_lines; ii++ )
	  {
	    iline = abs( area->lines[ii] ) - 1;

	    line = &(pb->lines[iline]);

	    /*****************************************************
	      
	      modify entry into our array to allow for line
	      reversal so we get correct ordering
	
	      *****************************************************/

	    if(  area->lines[ii] > 0 )
	      {
		    
		for( jj=0; jj<line->n_nodes - 1; jj++ )
		  {
		    kk = total_nodes + added_nodes + jj;
		    mm = jj;
		    
		    poly->poly_xy[2*kk+0] = line->nodes[2*mm+0];
		    poly->poly_xy[2*kk+1] = line->nodes[2*mm+1];
		  }
	      }
	    else
	      {
		
		for( jj=0; jj<line->n_nodes - 1; jj++ )
		  {
		    kk = total_nodes + added_nodes + jj;
		    mm = line->n_nodes - 1 - jj;
		    
		    poly->poly_xy[2*kk+0] = line->nodes[2*mm+0];
		    poly->poly_xy[2*kk+1] = line->nodes[2*mm+1];
		  }
	      }
	    
	    added_nodes += line->n_nodes - 1;
	    
	  }

	/* now close the loop */
	kk = total_nodes + added_nodes;
	jj = total_nodes;
	
	poly->poly_xy[2*kk+0] = poly->poly_xy[2*jj+0];
	poly->poly_xy[2*kk+1] = poly->poly_xy[2*jj+1];

	added_nodes += 1;

	poly->poly_limits[ 2*ivalid+0 ] = total_nodes;
	poly->poly_limits[ 2*ivalid+1 ] = added_nodes;

	total_nodes += added_nodes;

      }


  }
  
  /*******************************************************************

    bye-bye

  *******************************************************************/

  return(1);

}



/*********************************************************************

  load our dlg structures into the dlg object

*********************************************************************/

int load_dlg( OMobj_id top_id,
	     Dlg  *dlg,

	     int debug_flag )
{


  /*******************************************************************

    om

  *******************************************************************/

  OMobj_id   header_id;
  OMobj_id   pb_id;
  OMobj_id   areas_id;
  OMobj_id   lines_id;
  OMobj_id   nodes_id;
  OMobj_id   polys_id;
  


  /*******************************************************************

    the rest

  *******************************************************************/

  int status, size, type;

  float  *real_array;
  int    *int_array;

  OMobj_id   child_id;
  OMobj_id   entry_id;
  OMobj_id   entry_child_id;

  Header  *header;
  Pb      *pb;
  Node    *node;
  Area    *area;
  Line    *line;
  Poly    *poly;

  int ii, jj, kk, mm;
  int total_nodes;

  int inode, iarea, iline;


  /*******************************************************************

    generics

  *******************************************************************/

#include "../gen_proc.h"

  sprintf(  LFNC_function_name, "load_dlg" );



  /*******************************************************************

    set some pointers

  *******************************************************************/
  header = &(dlg->header);
  pb     = &(dlg->pb);
  poly   = &(dlg->polys);

  /*******************************************************************

    the header

  *******************************************************************/
  {

    /* get the object */
    FIND_SUBOBJ( header_id, top_id, "header" );


    /* load it */
    {

      /***************************************************************

	have_header

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "have_header" );

      SET_INT_VAL( child_id, header->have_header, "have_header" );


      /***************************************************************

	banner0

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "banner0" );

      SET_STR_VAL( child_id, header->banner0, "banner0" );



      /***************************************************************

	banner1

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "banner1" );

      SET_STR_VAL( child_id, header->banner1, "banner1" );



      /***************************************************************

	scale

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "scale" );

      SET_INT_VAL( child_id, header->scale, "scale" );


      /***************************************************************

	dlg_level

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "dlg_level" );

      SET_INT_VAL( child_id, header->dlg_level, "dlg_level" );


      /***************************************************************

	planimetric_zone

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "planimetric_zone" );

      SET_INT_VAL( child_id, header->planimetric_zone,
		  "planimetric_zone" );


      /***************************************************************

	planimetric_system

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "planimetric_system" );

      SET_INT_VAL( child_id, header->planimetric_system,
		  "planimetric_system" );


      /***************************************************************

	planimetric_units

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "planimetric_units" );

      SET_INT_VAL( child_id, header->planimetric_units,
		  "planimetric_units" );


      /***************************************************************

	resolution

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "resolution" );

      SET_REAL_VAL( child_id, header->resolution,
		  "resolution" );


      /***************************************************************

	n_ftm_trans_params

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "n_ftm_trans_params" );

      SET_INT_VAL( child_id, header->n_ftm_trans_params,
		  "n_ftm_trans_params" );


      /***************************************************************

	n_accuracy_records

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "n_accuracy_records" );

      SET_INT_VAL( child_id, header->n_accuracy_records,
		  "n_accuracy_records" );


      /***************************************************************

	n_control_points

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "n_control_points" );

      SET_INT_VAL( child_id, header->n_control_points,
		  "n_control_points" );


      /***************************************************************

	n_categories

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "n_categories" );

      SET_INT_VAL( child_id, header->n_categories, "n_categories" );


      /***************************************************************

	projection_params

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "projection_params" );

      RET_ARRAY_RW( real_array, child_id, "projection_params" );

      memcpy( real_array, header->projection_params,
	     sizeof(header->projection_params) );

      ARRfree( (char *) real_array );


      /***************************************************************

	file2map_params

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "file2map_params" );

      RET_ARRAY_RW( real_array, child_id, "file2map_params" );

      memcpy( real_array, header->file2map_params,
	     sizeof(header->file2map_params) );

      ARRfree( (char *) real_array );


      /***************************************************************

	corner_names

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "corner_names" );

      SET_STR_ARRAY_VAL( child_id, 0, header->corner_names[0] );
      SET_STR_ARRAY_VAL( child_id, 1, header->corner_names[1] );
      SET_STR_ARRAY_VAL( child_id, 2, header->corner_names[2] );
      SET_STR_ARRAY_VAL( child_id, 3, header->corner_names[3] );


      /***************************************************************

	corner_data

      ***************************************************************/

      FIND_SUBOBJ( child_id, header_id, "corner_data" );

      RET_ARRAY_RW( real_array, child_id, "corner_data" );

      memcpy( real_array, header->corner_data,
	     sizeof(header->corner_data) );

      ARRfree( (char *) real_array );



    }


  }




  

  /*******************************************************************

    the political boundaries

  *******************************************************************/

  {

    /* get the object */
    FIND_SUBOBJ( pb_id, top_id, "pb" );


    /*****************************************************************

      have_political_boundaries

    *****************************************************************/

    FIND_SUBOBJ( child_id, pb_id, "have_political_boundaries" );

    SET_INT_VAL( child_id, pb->have_political_boundaries,
		"have_political_boundaries" );


    if( pb->have_political_boundaries == 1 )
      {

      /***************************************************************

	att_code

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "att_code" );

      SET_INT_VAL( child_id, pb->att_code, "att_code" );


      /***************************************************************

	n_nodes_ref

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "n_nodes_ref" );

      SET_INT_VAL( child_id, pb->n_nodes_ref, "n_nodes_ref" );


      /***************************************************************

	n_nodes

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "n_nodes" );

      SET_INT_VAL( child_id, pb->n_nodes, "n_nodes" );


      /***************************************************************

	node_to_area_flag

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "node_to_area_flag" );

      SET_INT_VAL( child_id, pb->node_to_area_flag,
		  "node_to_area_flag" );


      /***************************************************************

	node_to_line_flag

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "node_to_line_flag" );

      SET_INT_VAL( child_id, pb->node_to_line_flag,
		  "node_to_line_flag" );


      /***************************************************************

	n_areas_ref

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "n_areas_ref" );

      SET_INT_VAL( child_id, pb->n_areas_ref, "n_areas_ref" );


      /***************************************************************

	n_areas

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "n_areas" );

      SET_INT_VAL( child_id, pb->n_areas, "n_areas" );


      /***************************************************************

	area_to_node_flag

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "area_to_node_flag" );

      SET_INT_VAL( child_id, pb->area_to_node_flag,
		  "area_to_node_flag" );


      /***************************************************************

	area_to_line_flag

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "area_to_line_flag" );

      SET_INT_VAL( child_id, pb->area_to_line_flag,
		  "area_to_line_flag" );


      /***************************************************************

	area_coordinates_flag

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "area_coordinates_flag" );

      SET_INT_VAL( child_id, pb->area_coordinates_flag,
		  "area_coordinates_flag" );


      /***************************************************************

	n_lines_ref

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "n_lines_ref" );

      SET_INT_VAL( child_id, pb->n_lines_ref, "n_lines_ref" );


      /***************************************************************

	n_lines

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "n_lines" );

      SET_INT_VAL( child_id, pb->n_lines, "n_lines" );


      /***************************************************************

	line_coordinates_flag

      ***************************************************************/

      FIND_SUBOBJ( child_id, pb_id, "line_coordinates_flag" );

      SET_INT_VAL( child_id, pb->line_coordinates_flag,
		  "line_coordinates_flag" );


      /***************************************************************

	nodes

      ***************************************************************/
      {
	FIND_SUBOBJ(nodes_id, pb_id, "nodes" );

	for( inode=0; inode<pb->n_nodes; inode++ )
	  {

	    GET_ARRAY_VAL(nodes_id, inode, &entry_id,  "nodes" );

	    /*********************************************************

	      point

            *********************************************************/

	    node = &(pb->nodes[inode]);


	    /*********************************************************

	      xy's

            *********************************************************/
	    FIND_SUBOBJ( entry_child_id, entry_id, "xy" );

	    RET_ARRAY_RW( real_array, entry_child_id, "xy" );

	    real_array[0] = node->xy[0];
	    real_array[1] = node->xy[1];

	    ARRfree( (char *) real_array );


	    /*********************************************************

	      number of lines

            *********************************************************/

	    FIND_SUBOBJ( entry_child_id, entry_id, "n_lines" );
	    SET_INT_VAL( entry_child_id, node->n_lines, "n_lines" );

	    /*********************************************************

	      lines

            *********************************************************/
	    if( node->n_lines > 0 )
	      {
		FIND_SUBOBJ( entry_child_id, entry_id, "lines" );

		RET_ARRAY_RW( int_array, entry_child_id, "lines" );

		memcpy( int_array, node->lines,
		       sizeof(int)*node->n_lines );

		ARRfree( (char *) int_array );
	      }



	  }
      }

      /***************************************************************

	areas

      ***************************************************************/
      {
	FIND_SUBOBJ( areas_id, pb_id, "areas" );

	for( iarea=0; iarea<pb->n_areas; iarea++ )
	  {

	    GET_ARRAY_VAL( areas_id, iarea, &entry_id,  "areas" );

	    /*********************************************************

	      point

            *********************************************************/

	    area = &(pb->areas[iarea]);


	    /*********************************************************

	      xy's

            *********************************************************/

	    FIND_SUBOBJ( entry_child_id, entry_id, "xy" );

	    RET_ARRAY_RW( real_array, entry_child_id, "xy" );

	    real_array[0] = area->xy[0];
	    real_array[1] = area->xy[1];

	    ARRfree( (char *) real_array );


	    /*********************************************************

	      number of lines

            *********************************************************/

	    FIND_SUBOBJ( entry_child_id, entry_id, "n_lines" );
	    SET_INT_VAL( entry_child_id, area->n_lines, "n_lines" );

	    /*********************************************************

	      lines

            *********************************************************/

	    FIND_SUBOBJ( entry_child_id, entry_id, "lines" );

	    RET_ARRAY_RW( int_array, entry_child_id, "lines" );

	    memcpy( int_array, area->lines,
		   sizeof(int)*area->n_lines );

	    ARRfree( (char *) int_array );


	    /*********************************************************

	      number of attributes

            *********************************************************/

	    FIND_SUBOBJ( entry_child_id, entry_id, "n_attributes" );
	    SET_INT_VAL( entry_child_id, area->n_attributes,
			"n_attributes" );


	    /*********************************************************

	      attributes

            *********************************************************/
	    if( area->n_attributes > 0 )
	      {
		FIND_SUBOBJ( entry_child_id, entry_id, "attributes" );

		RET_ARRAY_RW( int_array, entry_child_id,
			      "attributes" );

		memcpy( int_array, area->attributes,
		       sizeof(int)*area->n_attributes*2 );

		ARRfree( (char *) int_array );
	      }


	  }

      }

      /***************************************************************

	lines

      ***************************************************************/
      {
	FIND_SUBOBJ( lines_id, pb_id, "lines" );

	for( iline=0; iline<pb->n_lines; iline++ )
	  {

	    GET_ARRAY_VAL( lines_id, iline, &entry_id,  "lines" );

	    /*********************************************************

	      point

            *********************************************************/

	    line = &(pb->lines[iline]);

	    /*********************************************************

	      end_nodes

            *********************************************************/

	    FIND_SUBOBJ( entry_child_id, entry_id, "end_nodes" );

	    RET_ARRAY_RW( int_array, entry_child_id, "end_nodes" );

	    int_array[0] = line->end_nodes[0];
	    int_array[1] = line->end_nodes[1];

	    ARRfree( (char *) int_array );


	    /*********************************************************

	      side_areas

            *********************************************************/

	    FIND_SUBOBJ( entry_child_id, entry_id, "side_areas" );

	    RET_ARRAY_RW( int_array, entry_child_id, "side_areas" );

	    int_array[0] = line->side_areas[0];
	    int_array[1] = line->side_areas[1];

	    ARRfree( (char *) int_array );


	    /*********************************************************

	      number of nodes

            *********************************************************/

	    FIND_SUBOBJ( entry_child_id, entry_id, "n_nodes" );
	    SET_INT_VAL( entry_child_id, line->n_nodes, "n_nodes" );

	    /*********************************************************

	      nodes

            *********************************************************/

	    FIND_SUBOBJ( entry_child_id, entry_id, "nodes" );

	    RET_ARRAY_RW( int_array, entry_child_id, "nodes" );

	    memcpy( int_array, line->nodes,
		   sizeof(int)*line->n_nodes*2 );

	    ARRfree( (char *) int_array );


	    /*********************************************************

	      number of attributes

            *********************************************************/

	    FIND_SUBOBJ( entry_child_id, entry_id, "n_attributes" );
	    SET_INT_VAL( entry_child_id, line->n_attributes,
			"n_attributes" );


	    /*********************************************************

	      attributes

            *********************************************************/
	    if( line->n_attributes > 0 )
	      {
		FIND_SUBOBJ( entry_child_id, entry_id, "attributes" );

		RET_ARRAY_RW( int_array, entry_child_id,
			      "attributes" );

		memcpy( int_array, line->attributes,
		       sizeof(int)*line->n_attributes*2 );

		ARRfree( (char *) int_array );
	      }



	  }
      }




    }

  }

  
  /*****************************************************************

    polys

   *****************************************************************/
  {
      
    FIND_SUBOBJ( polys_id, top_id, "polys" );
    
    /***************************************************************
      
      number of polylines

      NB - this allocate for poly_area, poly_limits
      
    ***************************************************************/
    
    FIND_SUBOBJ( child_id, polys_id, "n_polys" );
    
    SET_INT_VAL( child_id, poly->n_polys, "n_polys" );
    
    /***************************************************************
      
      areas refernced by the polylines
      
    ***************************************************************/
    
    FIND_SUBOBJ( child_id, polys_id, "poly_area" );
    
    RET_ARRAY_RW( int_array, child_id, "poly_area" );
    
    memcpy( int_array, poly->poly_area, sizeof(int)*poly->n_polys );
    
    ARRfree( (char *) int_array );
    
    /***************************************************************
      
      limits of the line nodes in the block array
      
    ***************************************************************/
    
    FIND_SUBOBJ( child_id, polys_id, "poly_limits" );
    
    RET_ARRAY_RW( int_array, child_id, "poly_limits" );
    
    memcpy( int_array, poly->poly_limits,
	   sizeof(int)*poly->n_polys*2 );
    
    ARRfree( (char *) int_array );

    /*****************************************************************

      total number of nodes

      NB - this allocates for poly_xy

    *****************************************************************/

    FIND_SUBOBJ( child_id, polys_id, "total_nodes" );
    
    SET_INT_VAL(  child_id, poly->total_nodes, "total_nodes" );
    
    /***************************************************************
      
      the actual node coordinates
      
    ***************************************************************/
    
    FIND_SUBOBJ( child_id, polys_id, "poly_xy" );
    
    RET_ARRAY_RW( real_array, child_id, "poly_xy" );
    
    memcpy( real_array, poly->poly_xy,
	   sizeof(float)*poly->total_nodes*2 );
    
    ARRfree( (char *) real_array );
    
    /***************************************************************
      
      turn all the groups on
      
    ***************************************************************/
    
/*
    FIND_SUBOBJ( child_id, polys_id, "on_off" );
    
    RET_ARRAY_RW( int_array, child_id, "on_off" );
    
    for( ii=0; ii<poly->n_polys; ii++ )
      {
	int_array[ii] = 1;
      }
    
    ARRfree( (char *) int_array );
*/

    /***************************************************************
      
      give the groups names
      
    ***************************************************************/
    FIND_SUBOBJ( child_id, polys_id, "names" );
    
    for( ii=0; ii<poly->n_polys; ii++ )
      {
	
	/***********************************************************
	  
	  point
	  
        ***********************************************************/
	iarea = poly->poly_area[ii];
	
	area = &(pb->areas[iarea]);
	
	/***********************************************************
	  
	  load attributes into a string
	  
        ***********************************************************/
	
	sprintf( chuge, "%.*i:", i_exp( pb->n_areas ), iarea );

	for( jj=0; jj<area->n_attributes; jj++ )
	  {
	    sprintf( chuge+strlen(chuge),
		    " %i-%i",
		    area->attributes[2*jj+0],
		    area->attributes[2*jj+1] );
	    
	  }
	
	/***********************************************************
	  
	  set the name
	  
        ***********************************************************/
	
	SET_STR_ARRAY_VAL( child_id, ii, chuge );
	
      }
    
    
  }
  




  /*******************************************************************

    bye-bye

  *******************************************************************/

  return(1);
}







/*********************************************************************

  load the file information string

*********************************************************************/

int load_file_info( OMobj_id top_id,
		   char *filename,
		   Dlg  *dlg,

		   int debug_flag )
{


  /*******************************************************************

    om

  *******************************************************************/



  /*******************************************************************

    the rest

  *******************************************************************/

  int status, size, type;

  char file_info[1024];

  Header *header;
  Pb     *pb;

  /*******************************************************************

    generics

  *******************************************************************/

#include "../gen_proc.h"

  sprintf(  LFNC_function_name, "load_dlg" );


  /*******************************************************************

    point

  *******************************************************************/
  header = &(dlg->header);
  pb     = &(dlg->pb);

  /*******************************************************************

    load the string

  *******************************************************************/
  {

    file_info[0] = '\0';


    /*****************************************************************

      filename

    *****************************************************************/

    cstart = filename;
    while( (cfinal = strstr( cstart, SLASH )) != NULL )
      { cstart = cfinal+1; }
	
    sprintf( file_info+strlen(file_info), "FILE: %s\n\n", cstart );


    /*****************************************************************

      banners

    *****************************************************************/

    sprintf( file_info+strlen(file_info), "%s\n", header->banner0 );

    sprintf( file_info+strlen(file_info), "%s\n", header->banner1 );


    /*****************************************************************

      scale

    *****************************************************************/

    sprintf( file_info+strlen(file_info), "Scale  1:%i\n", header->scale );


    /*****************************************************************

      dlg_level

    *****************************************************************/

    sprintf( file_info+strlen(file_info), "Level  %i\n", header->dlg_level );


    /*****************************************************************

      node, area, lines for POLITICAL BOUNDARIES

    *****************************************************************/

    sprintf( file_info+strlen(file_info), "\nPOLITCAL BOUNDARIES\n" );
    
    sprintf( file_info+strlen(file_info), "     Nodes:  %i\n", pb->n_nodes );
    sprintf( file_info+strlen(file_info), "     Areas:  %i\n", pb->n_areas );
    sprintf( file_info+strlen(file_info), "     Lines:  %i\n", pb->n_lines );


    /*****************************************************************

      trailer

    *****************************************************************/

    sprintf( file_info+strlen(file_info), "\n\n" );


  }


  /*******************************************************************

    send it out

  *******************************************************************/

  SET_STR_VAL( top_id, file_info, "file_info" );


  /*******************************************************************

    bye-bye

  *******************************************************************/

  return(1);
    
}

