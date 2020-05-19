 */

/* ----------------------------------------------------------------------
 * r_hdfimg.c
 * ----------------------------------------------------------------------
 * READ HDF IMAGE
 *
 * Paul G. Lever
 * International AVS Centre
 * Manchester Visualization Centre
 * Manchester Computing
 * University of Manchester
 * Oxford Road
 * Manchester
 * United Kingdom
 * M13 9PL
 *
 * email: avs@iavsc.org, p.lever@mcc.ac.uk
 * ----------------------------------------------------------------------
 * module "ReadHDFImageField"
 *   omethod update = "read_hdf_image"
 * 
 * Version 1.0 - 15th August 1997
 * 
 *   Based on the AVS 5 module Read HDF Image, written by Terry Myerson
 *   of the International AVS Center, at the North Carolina
 *   Supercomputing Center, May 18th 1992.
 *
 *   The module uses the SDSC image tool library. See the accompanying
 *   HTML documentation and INSTALL file for more information about this
 *   and other required libraries.
 * ----------------------------------------------------------------------
 */

#include "xp_comm_proj/r_hdfimg/gen.h"

/* ----------------------------------------------------------------------
 * read_hdf_image()
 *
 *   Function called by the 'update' omethod of the ReadHDFImageField
 *   low-level module.
 *
 * input:
 * 
 *   filename : filename of the HDF format image file
 *
 * output:
 * 
 *   out : output field for image data
 *
 * ----------------------------------------------------------------------
 */
 
int
read_hdf_image(OMobj_id ReadHDFImage_id, OMevent_mask event_mask, int seq_num)
{
  /* variables for accessing objects and components	 */
  
  char  *filename = NULL;
  OMobj_id out_id;
  int out_ndim, *out_dims, out_nspace;
  float *out_points;
  int out_veclen;
  int out_data_type, out_ndata;
  char  *out_data;
  
  FILE *img_file;
  ImVfb *img_buffer;
  TagTable *dataTable;
  TagEntry *imgEntry;
  ImVfbPtr ptr;
  int dims0[2],i,j;
  
  int status;
  int index;
  
  /* retrieve filename value	 */
  
  if( OMget_name_str_val( ReadHDFImage_id,
			  OMstr_to_name( "filename" ),
			  &filename, 0) != 1 )
    filename = NULL;
  
  /* open hdf image file	 */
  
  if( !filename ) return 1;
  
  dataTable = TagTableAlloc();
    
  img_file = fopen( filename, "r" );
  if( !img_file ) {
    ERRverror( "", ERR_NO_HEADER | ERR_INFO,
	       "read_hdf_image: error opening filename" );
    return 0;
  }
  
  status = ImFileFRead( img_file, "hdf", NULL, dataTable );
  if( status == -1 ) {
    ERRverror( "", ERR_NO_HEADER | ERR_INFO,
	       "read_hdf_image: error reading file" );
    return 0;
  }
  
  imgEntry = TagTableQDirect( dataTable, "image vfb", 0 );
  TagEntryQValue( imgEntry, &img_buffer );
  
  /* read information	 */
  
  /* get 'out' object id	 */
  
  out_id = OMfind_subobj( ReadHDFImage_id, OMstr_to_name("out"), OM_OBJ_RW );
  
  /* set ndim to 2 for image	 */
  
  out_ndim = 2; 
  FLDset_ndim( out_id, out_ndim );
  
  /* malloc and set the mesh dims array	 */
  
  out_dims = (int *)ARRalloc( NULL, DTYPE_INT, out_ndim, NULL );
  
  out_dims[0] = ImVfbQWidth(img_buffer);
  out_dims[1] = ImVfbQHeight(img_buffer);
  
  FLDset_dims( out_id, out_dims );
  
  /* set nspace to 2 for image data	 */
  
  out_nspace = 2;
  FLDset_nspace( out_id, out_nspace );
  
  /* set veclen to 4, for ARGB image data	 */
  
  out_veclen = 4;
  FLDset_node_data_veclen( out_id, 0, out_veclen );
  
  /* set data type, number of required nodes and allocate array	 */
  
  out_data_type = DTYPE_CHAR;
    
  out_ndata = ImVfbQWidth(img_buffer) * ImVfbQHeight(img_buffer);
  
  out_data = (char *)ARRalloc( NULL, out_data_type, 
			       out_veclen*out_ndata, NULL );
    
  /* fill in node data	 */
  
  index = 0;
  for( j=ImVfbQHeight(img_buffer)-1; j>=0; j-- ) {
    for( i=0; i<ImVfbQWidth(img_buffer); i++ ) {
      ptr = ImVfbQPtr( img_buffer, i, j );
      out_data[index++] = (char) ImVfbQAlpha( img_buffer, ptr );
      out_data[index++] = (char) ImVfbQRed( img_buffer, ptr );
      out_data[index++] = (char) ImVfbQGreen( img_buffer, ptr );
      out_data[index++] = (char) ImVfbQBlue( img_buffer, ptr );
    }
  }
  
  /* set node data	 */
  
  FLDset_node_data( out_id, 0, out_data, out_data_type,
		    out_ndata*out_veclen, OM_SET_ARRAY_FREE );
  
  /* tidy up	 */
  
  fclose( img_file );
  
  /* free output 	 */
  
  if( out_dims )
    ARRfree( (char *)out_dims );
  
  /* free input 	 */
  
  if( filename )
    free( filename );
  
  return( 1 );
}
