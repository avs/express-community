 *
 * file: wr_fld.c
 * author: Dave Goughnour, AVS Inc
 * author: Sudhir, AVS Inc
 * purpose: Writes uniform/rectilinear/structured field to an AVS .fld file
 *
 */


#include "xp_comm_proj/wr_fld/gen.h"
#include <avs/f_utils.h>

#define MAX_LABEL_SIZE 1024
#define INPUT_COMP_COUNT 100

int WriteField_update(OMobj_id Write_Field_id, OMevent_mask event_mask,
                      int seq_num)
{
    OMobj_id input_id;
    FILE *output;

    int input_ncomp, input_comp_count, input_ndata_count, input_vect_count, buffer_array_count;
    int input_data_type, input_ndata, *input_data_type_array, *input_ndata_array, input_data_type_count;
    char *filename = NULL;
    char **input_data_array, *buffer_array;
    int nnodes, nspace, ndims, *dims;
    int  stat;
    char label[MAX_LABEL_SIZE];
    int grid_type;
    int type_size, size;
    int i, j, k;

    /*
      Get filename
    */

    if (OMget_name_str_val(Write_Field_id, OMstr_to_name("filename"), 
                           &filename, 0) != 1) {
        fprintf( stderr, "Unable to get value for filename\n" );
        return( 0 );
    }

    {
        char file_buf[AVS_PATH_MAX];
        char *expanded_filename;

        expanded_filename = FILEmap_variables( filename, file_buf );

        /*
          Create and write to output file
        */
        if( (output = fopen( expanded_filename, "wb" )) == NULL ) {
            fprintf( stderr, "Unable to open file %s\n", filename );
            free( filename );
            return( 0 );
        }
    }

    /*
    ** Start with the ASCII header
    */

    fprintf( output, "# AVS field file\n" );
    fprintf( output, "# this is a header file for a field to be\n" );
    fprintf( output, "# used in conjunction with the build a field module of AVS\n" );
    fprintf( output, "#\n" );

    /*
      Get field id
    */
    input_id = OMfind_subobj(Write_Field_id, OMstr_to_name("input"),
                             OM_OBJ_RD);

    /*
      Get number of node data components.
    */
    FLDget_node_data_ncomp ( input_id, &input_ncomp );

    /*
      Get number of dimensions and dimensions array.
    */
    FLDget_dims( input_id, &dims, &ndims );
    fprintf( output, "ndim = %d\n", ndims );

    for( i = 0; i < ndims; i++ )
        fprintf( output, "dim%d = %d\n", i+1, dims[i]);

    /*
      Total number of nodes.
      This should equal the product of the dimensions.
    */
    FLDget_nnodes( input_id, &nnodes );

    FLDget_nspace( input_id, &nspace );
    fprintf( output, "nspace = %d\n", nspace );

    /*
      Write number of input components as veclen.
    */
    fprintf( output, "veclen = %d\n", input_ncomp);

    /*
      Get node data type, assume its the same type for all components.
    */
    FLDget_node_data_type(input_id, 0, &input_data_type);

    switch( input_data_type )
    {
    case DTYPE_BYTE:
        fprintf( output, "data = byte\n" );
        break;
    case DTYPE_SHORT :
        fprintf( output, "data = short\n" );
        break;
    case DTYPE_INT :
        fprintf( output, "data = int\n" );
        break;
    case DTYPE_FLOAT :
        fprintf( output, "data = float\n" );
        break;
    case DTYPE_DOUBLE :
        fprintf( output, "data = double\n" );
        break;
    default:
        fprintf( stderr, "Unsupported data type in wrt_fld...\n" );
        fclose(output);
        if (filename) free(filename);
        return( 0 );
    }
    type_size = DTYPEtype_size[input_data_type];

    /*
      Get grid_type
    */
    stat = FLDget_grid_type(input_id, &grid_type);
    if (stat == 1 && grid_type == 2)
        fprintf(  output, "field = rectilinear\n" );
    else if (stat == 1 && grid_type == 1)
        fprintf( output, "field = uniform\n" );
    else
        fprintf( output, "field = irregular\n" );


    /*
      Get label for each component
    */

    for (input_comp_count=0; input_comp_count < input_ncomp; input_comp_count++){
        /* Check to make sure it exists for all components */
        stat = FLDget_node_data_label(input_id, input_comp_count,
                                      label, MAX_LABEL_SIZE);
        if (stat != 1) break;
    }

    if( stat == 1 && input_ncomp != 0 ) {
        fprintf( output, "label = ");
        for (input_comp_count=0; input_comp_count < input_ncomp; input_comp_count++){
            FLDget_node_data_label(input_id, input_comp_count,
                                   label, MAX_LABEL_SIZE);
            fprintf(output, "\"%s\" ",label);
        }
        fprintf( output, "\n");
    }

    /*
      Get units for each component
    */

    for (input_comp_count=0; input_comp_count < input_ncomp; input_comp_count++){
        /* Check to make sure it exists for all components */
        stat = FLDget_node_data_units(input_id, input_comp_count,
                                      label, MAX_LABEL_SIZE);
        if (stat == 1 && label[0] == 0 ) stat = 0;
        if (stat != 1) break;
    }

    if( stat == 1 && input_ncomp != 0 ) {
        fprintf( output, "units = ");
        for (input_comp_count=0; input_comp_count < input_ncomp; input_comp_count++){
            FLDget_node_data_units(input_id, input_comp_count,
                                   label, MAX_LABEL_SIZE);
            fprintf(output, "\"%s\" ",label);
        }
        fprintf( output, "\n");
    }


    /*
      Magic characters that separate the ASCII header information from
      the binary node data and coordinate information.
    */
    fprintf( output, "\f\f" );

    /*
    **  Node Data
    */

    /*
      Allocate memory to hold pointers for each component input data
    */
    input_data_array = (char ** )malloc(sizeof(char *) * input_ncomp);

    /*
      For each node component get data array
    */
    for (input_comp_count=0; input_comp_count<input_ncomp; input_comp_count++)
    {
        int comp_data_type, comp_veclen, input_ndata;

        FLDget_node_data (input_id, input_comp_count,
                          &comp_data_type, input_data_array+input_comp_count,
                          &input_ndata, OM_GET_ARRAY_RD);

        FLDget_node_data_veclen (input_id, input_comp_count, &comp_veclen);

        if( input_data_type != comp_data_type || comp_veclen != 1 ) {

            /* Another check is that input_ndata == nnodes */

            if( input_data_type != comp_data_type )
                fprintf( stderr, "All node data components must have the same data type in wrt_fld...\n" );

            if( comp_veclen != 1 )
                fprintf( stderr, "All node data components must have veclen equal to 1 in wrt_fld...\n" );

            while( input_comp_count >= 0 ) {
                if ( input_data_array[input_comp_count] )
                    ARRfree( input_data_array[input_comp_count] );
                input_comp_count--;
            }

            if ( input_data_array )
                free( input_data_array );

            fclose(output);
            if (filename) free(filename);
            return( 0 );
        }
    }

    /*
      Allocate memory for the buffer_array to write n_component data
      in an interlaced way
    */

    buffer_array = (char *)malloc(sizeof(char) * input_ncomp * nnodes * type_size);

    /*
      Write component data in interlaced way.
      First element of first component, first element of second component ...
      Second element of first component, second element of second component ...
      .
      .
      .
    */

    buffer_array_count=0;

    /* Loop over nodes */
    for (input_ndata_count=0; input_ndata_count < nnodes; input_ndata_count++) {

        /* Loop over components */
        for (input_comp_count=0; input_comp_count < input_ncomp; input_comp_count++) {
            char *comp_data_array, *node_data_element;

            comp_data_array   = input_data_array[input_comp_count];
            node_data_element = comp_data_array+(input_ndata_count*type_size);
        
            switch( input_data_type )
            {
            case DTYPE_BYTE :
            case DTYPE_SHORT :
            case DTYPE_INT :
            case DTYPE_FLOAT :
            case DTYPE_DOUBLE :
                for( input_data_type_count=0;
                     input_data_type_count<type_size;
                     input_data_type_count++ ) {

                    /* doing a simple byte copy */
                    buffer_array[buffer_array_count++] = 
                        node_data_element[input_data_type_count];
                }
                break;
            }
        }
    }

    /*
      Write out the Node Data.
    */
    fwrite( buffer_array, type_size, input_ncomp * nnodes, output );

    /*
      Free allocated memory
    */
    for ( i=0; i<input_ncomp; i++ ){
        ARRfree( input_data_array[i] );
    }
    free( input_data_array );
    free( buffer_array );

    /*
    ** Coordinate information.  Each grid type needs
    ** to be handled differently.
    */

    stat = FLDget_grid_type( input_id, &grid_type );
    if ( stat == 1 && grid_type == 1 ) {
        /*
          Write the 2 points for the Uniform field.
        */
        float *points;
        int skip = 1;

        FLDget_points( input_id, &points, &size, 0 );

        if( ndims == nspace ) {
            /* Check for default coordinate information */
            for( i = 0; i < ndims; i++ ) {
                if( points[i]       != 0.0 || 
                    points[i+ndims] != (dims[i] - 1.0) ) {
                    skip = 0;
                    break;
                }
            }
        }
        else skip = 0; /* ndims != nspace is probably very uncommon */

        if( skip == 0 ) {
            float out_coords[2];
            /* (Xmin,Xmax),(Ymin,Ymax), etc. */
            for( i = 0; i < nspace; i++ ) {
                /* Write one min,max pair of coordinates */
                out_coords[0] = points[i];
                out_coords[1] = points[i+nspace];
                fwrite( out_coords, sizeof(float), 2, output );
            }
        }

        ARRfree(points);
    }
    else if ( stat == 1 && grid_type == 2 ) {
        /*
          Write the coordinates for rectilinear field.
          Squeeze out the redundant information in the points array.
          Write out all the Xs, then all the Ys, etc.
        */
        float *points, *out_coords;
        int largest_dim, skip_dim, sum_dims;

        /*
          Write the sum(dims) number of points for the rectangular field.
        */
        FLDget_points(input_id, &points, &size, 0);

        /*
          Get the largest dimension,
          to determine how much buffer space we need.
        */
        largest_dim = 0;
        for (i=0; i<ndims; i++) if (dims[i]>largest_dim) largest_dim=dims[i];
        out_coords = malloc( largest_dim * sizeof(float) );

        skip_dim = 0;
        sum_dims = 0;

        for (i=0; i<ndims; i++){
            for (j=0; j<dims[skip_dim]; j++)
                out_coords[j] = points[sum_dims+ndims*j+skip_dim];

            fwrite(out_coords, sizeof(float), dims[skip_dim], output);

            sum_dims += ndims*dims[skip_dim];
            skip_dim++;
        }

        /*
          Free allocated memory
        */
        free(out_coords);
        ARRfree(points);
    }
    else if (stat != 1) {
        /*
          Write the coordinates for structured field.
          Write out all the Xs, then all the Ys, etc.
        */
        float *coord_array, *out_coords;
        int temp;

        /* Size should be nnodes * nspace */
        FLDget_coord(input_id, &coord_array, &size, 0);
        out_coords = malloc( sizeof(float) * nnodes );

        temp = 0;
        for (i=0; i < nspace; i++) {
            temp = i;
            for (j=0; j < nnodes; j++ ) {
                out_coords[j] = coord_array[temp];
                temp += nspace;
            }

            fwrite(out_coords, sizeof(float), nnodes, output);
        }
        free(out_coords);
        ARRfree(coord_array);
    }

    if( dims ) ARRfree( dims );

    fclose( output );

    /*
        Free input variables
    */

    if( filename ) free(filename );

    return(1);
}

/*
  End of file
 */
