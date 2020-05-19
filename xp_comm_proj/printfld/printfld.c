
#include "xp_comm_proj/printfld/gen.h"

#include <avs/err.h>
#include <avs/f_utils.h>
#include <avs/arr.h>
#include <avs/dtype.h>
#include <avs/fld.h>
 
   /**************************/
   /*  literal definitions   */
   /**************************/

#define UNSTRUCTURED    -1
#define STRUCTURED      0
#define UNIFORM         1
#define RECTILINEAR     2

#define MAX_DISPLAY_VAL 1000  /* maximum number of values to display */
#define MAX_STRING_SIZE 64

#define NODE_DATA_ARRAY 0      /* node_data Data_Array */
#define CELL_DATA_ARRAY 1      /* cell_data Data_Array */

#define UNDEFINED_STR "UNDEFINED"

 
/* macro to check for and handle errors */

#define ERR_CHECK(status, err_string) \
  { if (status == OM_STAT_ERROR)       \
     ERRerror ("print_field", 1, ERR_ORIG, err_string);  }
  

/* macro to handle errors */

#define ERR_RETURN(err_string) { \
     ERRerror ("print_field", 1, ERR_ORIG, err_string);  \
     return (OM_STAT_ERROR);  \
 }

#define STATUS_RETURN(stat_string) { \
     ERRerror ("print_field", 1, ERR_ORIG, stat_string);  \
     return (OM_STAT_ERROR);  \
 }

#define PUTSTRING(textstr) {\
     sprintf(strbuf,"%s",textstr); \
     printf("%s",strbuf); \
 }

#define PUTVALUE(string, val) {\
     sprintf(strbuf,"%s= %d",string,val); \
     printf("%s \n",strbuf); \
 }

/******************************************************************
 *
 *  Module Name:  print_field
 *
 *  Purpose:      This method prints the values of the components
 *                of a field to a file.
 *
 *                This version of print_field does not allow you
 *                to specify a starting point in any of the field's
 *                arrays or a range to print.  It just starts at the
 *                beginning and prints out MAX_DISPLAY_VAL values.
 *                MAX_DISPLAY_VAL is currently set to 1000; change it
 *                if you want to see more values.
 *
 *                If the output file already exists, the function 
 *                prints an error message and returns, rather than
 *                overwriting the file.  You can change this behavior
 *                too if you want.
 *
 *
 *  Function Parameters
 *  Inputs:
 *    print_field_id          the object id of the print_field
 *                            group object.  The field to print
 *                            is an input sub-obj of this group
 *    event_mask              mask which indicates why method got fired
 *    seq_num                 sequence num assigned by OM 
 *
 *  Outputs:
 *    None
 *
 *
 *  input sub-objects:
 *      filename       - string specifying name of file to print to
 *      in - reference to the field of interest
 *
 ******************************************************************/
 
int
print_field(OMobj_id print_field_id, OMevent_mask event_mask, int seq_num)
{

 
   /***************************/
   /*  variable declarations  */
   /***************************/

   int      status = OM_STAT_SUCCESS;     /* innocent until proven guilty */

   char     *output_filename = NULL;
   FILE     *output_fp;
 
   int      field_nspace, field_nnodes, field_size;
   int      field_nsets, field_set_count, field_ncells;
   int      field_cell_nnodes, *field_node_connect;
   OMobj_id field_id;
   OMobj_id field_cell_set;
   float    *field_coord;
 
   int      field_ncomp, field_comp_count, field_veclen;
   int      field_data_type, field_ndata;
   char     *field_data;
 
   int      grid_type; 

   char     note_string[200];
   char     temp_string[200];

   char     *strbuf;

   int      gridout, cellout, xformout, nodesout;
   int      htmlout, allout;

   int      component_num;
   int      nnodes_num;
   int      ncell_num;

   int      trigger;
   int      interrupt;

   int      print_continue = 0;

   OMobj_id evt, nevt;

    /* put in UI button to check all done */
    evt = OMfind_subobj(print_field_id, OMstr_to_name("trigger"),OM_OBJ_RD);
    if (OMchanged(evt, seq_num) == 0) return(1);   

    evt = OMfind_subobj(print_field_id, OMstr_to_name("message"),OM_OBJ_RW); 
    if (OMset_str_val(evt,"Start printing....") !=OM_STAT_SUCCESS) {
       ERR_RETURN("In print_field section");   
    }
        
    /* set event flag for printing notes; use note_string to collect notes */
    nevt = OMfind_subobj(print_field_id, OMstr_to_name("notes"),OM_OBJ_RW); 

    strcpy(note_string,"");
    strcpy(temp_string,"");

    /************************************/
    /*  Get the name of the output file */
    /************************************/

    status = OMget_name_str_val(print_field_id, OMstr_to_name("filename"), 
                                &output_filename, 0);

    if (status != OM_STAT_SUCCESS) {
       printf ("\ncan't get filename input, will use /tmp/fld.out\n");
       output_filename = (char *) malloc (120);
       strcpy (output_filename, "/tmp/fld.out");
    }

    /******************/
    /* get int params */
    /**************** */


    status = OMget_name_int_val(print_field_id, OMstr_to_name("gridout"), &gridout);
    if (status != OM_STAT_SUCCESS) ERR_RETURN("could not get gridout");

    status = OMget_name_int_val(print_field_id, OMstr_to_name("cellout"), &cellout);
    if (status != OM_STAT_SUCCESS) ERR_RETURN("could not get cellout");

    status = OMget_name_int_val(print_field_id, OMstr_to_name("xformout"), &xformout);
    if (status != OM_STAT_SUCCESS) ERR_RETURN("could not get xformout");

    status = OMget_name_int_val(print_field_id, OMstr_to_name("nodesout"), &nodesout);
    if (status != OM_STAT_SUCCESS) ERR_RETURN("could not get nodesout");

    status = OMget_name_int_val(print_field_id, OMstr_to_name("htmlout"), &htmlout);
    if (status != OM_STAT_SUCCESS) ERR_RETURN("could not get htmlout");

    status = OMget_name_int_val(print_field_id, OMstr_to_name("allout"), &allout);
    if (status != OM_STAT_SUCCESS) 
        ERR_RETURN("Could not print all points");

    /* Print all points should be complimented with category selections */

    if (gridout == 0 && xformout == 0 && cellout == 0 && nodesout == 0) 
        ERR_RETURN("Could not print; Please select a category");


    strbuf=(char *) malloc (200);

    /*************************/
    /*  Open the output file */
    /*************************/

    if (FILEexists (output_filename, FILE_WRITE, NULL)) { 
       ERR_RETURN ("\nERROR:  Output file already exists - delete it or specify a new file\n");
    } 

    output_fp = fopen (output_filename, "w");
           
    if (output_fp == NULL) {
       ERR_RETURN ("unable to open output file");
    }

    if (output_filename != (char *) NULL)
       free (output_filename);    /* we're finished with the name string */


    /*********************************************/
    /* Write Field heading to top of output file */
    /*********************************************/

    OMstatus_check(10,"Print Start",&interrupt);
    if (interrupt) {
      fclose(output_fp);
      return(1);
    }

    if (htmlout) {
      fprintf (output_fp, "<A NAME=\"TOP\"></A>\n");
      fprintf (output_fp, "<html><title>AVS/Express Grid Data</title><body>\n");
      fprintf (output_fp, "<pre>\n"); 
    }

    fprintf (output_fp, "                *********************************************\n"); 
    fprintf (output_fp, "                ******                                  *****\n");  
    fprintf (output_fp, "                ****          Field Information           ***\n"); 
    fprintf (output_fp, "                ******                                  *****\n");  
    fprintf (output_fp, "                *********************************************\n\n\n");  

    /* create hyperlinks for html */

    if (htmlout) {
        if (gridout)  fprintf (output_fp, "<A HREF=\"#Grid_Values\">Grid Values</A>\n");
        if (cellout)  fprintf (output_fp, "<A HREF=\"#CELL_Values\">Cell and Cell Data Values</A>\n");
        if (nodesout) fprintf (output_fp, "<A HREF=\"#NODE_DATA_Values\">Node Data Values</A>\n");
        if (xformout) fprintf (output_fp, "<A HREF=\"#XFORM_Values\">Transformation Values</A>\n");
    }

    /**************************************************/
    /*       Get obj id of the field to print         */
    /*  This is a sub-object, called "in" */
    /*  of the print_field_id group object            */
    /**************************************************/

    field_id = OMfind_subobj(print_field_id,
			     OMstr_to_name("in"), OM_OBJ_RD);

    if (OMis_null_obj (field_id)) {
       ERR_RETURN ("can't get object id of field to print");
    }


    /********************************/
    /*  Fetch and print grid values */
    /********************************/

    if (gridout){
        OMstatus_check(20,"Print Grids",&interrupt);
        if (interrupt) {
           fclose(output_fp);
           return(1);
        }

        status = write_grid (output_fp, &field_id, &grid_type, &allout, &htmlout, &nnodes_num);
        evt = OMfind_subobj(print_field_id, OMstr_to_name("message"),OM_OBJ_RW); 
        if (OMset_str_val(evt,"Start printing grid data") !=OM_STAT_SUCCESS) {
          ERR_RETURN("In print_field grid section");   
        }

        switch (grid_type) { 
          case UNSTRUCTURED:
   	    strcpy (note_string, "Unstructured ");
	    break;
          case STRUCTURED:
	    strcpy (note_string, "Structured ");
	    break;
          case UNIFORM:
	    strcpy (note_string, "Uniform ");
	    break;
          case RECTILINEAR:
	    strcpy (note_string, "Rectilinear ");
	    break;
          default:
            ERR_RETURN ("Undefined grid type returned ");
        }

        strcpy(temp_string,"");
        sprintf(temp_string,"Grid with nodes = %d\n",nnodes_num);
        strcat(note_string, temp_string);
        strcpy(temp_string,"");

    }

    /********************************/
    /*  Fetch and print cell info   */
    /********************************/

    if (cellout){
        OMstatus_check(80,"Print Cells",&interrupt);
        if (interrupt) {
          fclose(output_fp);
          return(1);
        }

        if (status != OM_STAT_ERROR){
           status = write_cell_info(output_fp, &field_id, grid_type, &allout, &htmlout, &ncell_num);
        }
        evt = OMfind_subobj(print_field_id, OMstr_to_name("message"),OM_OBJ_RW); 
        if (OMset_str_val(evt,"Start printing cell info") !=OM_STAT_SUCCESS) {
          ERR_RETURN("In print_field cell info section");   
        }  

        strcpy(temp_string,"");
        sprintf(temp_string,"Cell info data with ncell_number= %d\n",ncell_num);
	strcat (note_string, temp_string);
        strcpy(temp_string,"");
    }

    /********************************/
    /*  Fetch and print node data   */
    /********************************/

    if (nodesout){
        OMstatus_check(60,"Print Nodes",&interrupt);
        if (interrupt) {
           fclose(output_fp);
           return(1);
        }
        if (status != OM_STAT_ERROR) { 
           status = write_node_data(output_fp, &field_id, &allout, &htmlout, &component_num);
        }
        evt = OMfind_subobj(print_field_id, OMstr_to_name("message"),OM_OBJ_RW); 
        if (OMset_str_val(evt,"Start printing node data") !=OM_STAT_SUCCESS) {
          ERR_RETURN("In print_field node data section");   
        } 

        strcpy(temp_string,"");
        sprintf(temp_string,"Node data with component_number= %d\n",component_num);
	strcat (note_string, temp_string);
        strcpy(temp_string,"");
    }


       /*********************************/
    /*  Fetch and print xform values */
    /*********************************/

    if (xformout){
        OMstatus_check(40,"Print Xform",&interrupt);
        if (interrupt) {
          fclose(output_fp);
          return(1);
        }

        if (status != OM_STAT_ERROR) { 
          status = write_xform (output_fp, &field_id, &htmlout);
        }

        evt = OMfind_subobj(print_field_id, OMstr_to_name("message"),OM_OBJ_RW); 
        if (OMset_str_val(evt,"Start printing xform values") !=OM_STAT_SUCCESS) {
          ERR_RETURN("In print_field xform values section");   
        }

	strcat (note_string, "Xform values included\n");
    }


    /**************************************************/
    /*  Write bottom string and close the output file */
    /**************************************************/

    fprintf (output_fp, "\n************     End Field Information    ************");

    OMstatus_check(99,"Print End",&interrupt);
    evt = OMfind_subobj(print_field_id, OMstr_to_name("message"),OM_OBJ_RW); 
    if (OMset_str_val(evt,"Printing completed") !=OM_STAT_SUCCESS) {
      ERR_RETURN("In print_field end section");   
    }  

    /* Print out notes */

    if (OMset_str_val(nevt,note_string) !=OM_STAT_SUCCESS) {
      ERR_RETURN("In print_field end section");   
    }
  
    if (htmlout) fprintf (output_fp, "\n</pre></body></html>\n");

    if (interrupt) {
      fclose(output_fp);
      return(1);
    }

    if (output_fp != NULL) {
       fclose (output_fp);
    }

    free (strbuf);

    return(1);

 }  /* end print_field */

 

 /******************************************************************
  *
  *  Module Name:  write_header 
  *
  *  Purpose:      This routine takes a string as input and writes
  *                it out to the output file with a pretty header  
  *                box around it. 
  *
  *  Inputs:
  *    FILE     *output_fp      file pointer to the output file to   
  *                             write the header string to. 
  *    char    *hdr_string      header string to write                   
  *
  *  Outputs:
  *    None
  *
  ******************************************************************/

 int write_header(FILE *output_fp, char *hdr_string)
 {

    /***************************/
    /*  variable declarations  */
    /***************************/

    int      status = OM_STAT_SUCCESS;     
    int      line_length;                /* length of ea. line of box */
    char     *border_str;                /* top and bottom of box     */
    char     *cur_border_ptr;            /* pts to current loc. in str*/
    char     *text_str;                  /* middle (text line) of box */



    /* A header box looks like this:
	  +--------+        plus sign, dashes, plus sign
	  |  text  |        border, 2 spaces, text, 2 spaces, border
	  +--------+
       So, line length = string length + 2 (borders) + 4 (spaces) + 1 (NULL)
       Or, line length = string_length + 7
    */ 

    /* compute the length of each line of the header box
    */

    line_length = strlen (hdr_string)+7; 


    /* allocate space for a border string 
    */

    border_str = (char *)malloc (line_length);
    if (border_str == NULL) {
       ERR_RETURN ("ERROR - unable to allocate space");
    }


    /*****************************************/
    /*  create and write out the border line */
    /*****************************************/

    cur_border_ptr = border_str;     /* save ptr to start of string */
    *cur_border_ptr = '+';
    cur_border_ptr++;

    /* add sequence of dashes. Start after the '+' (at border_str+1)
    */
    memset (cur_border_ptr, '-', line_length-3); 
    cur_border_ptr += line_length-3;      /* increment to after dashes */


    /* add the final plus sign and print the string out to the output file
    */
    *cur_border_ptr = '+';
    cur_border_ptr++;
    *cur_border_ptr = '\0';
    fprintf (output_fp, "\n\n%s\n", border_str);


    /**************************************************/
    /*  create the middle line containing text header */
    /**************************************************/

    /* allocate space for the text string 
    */

    text_str = (char *)malloc (line_length);
    if (text_str == NULL) {
       ERR_RETURN ("ERROR - unable to allocate space");
    }

    strcpy (text_str, "|  ");
    strcat (text_str, hdr_string);
    strcat (text_str, "  |");
    fprintf (output_fp, "%s\n", text_str);

    free (text_str);


    /****************************************************/
    /*  write out border string again for bottom border */
    /****************************************************/

    fprintf (output_fp, "%s\n\n", border_str);

    free (border_str);


    return (status);

 } /* end write_header module */


 
 /******************************************************************
  *
  *  Module Name:  write_data_array
  *
  *  Purpose:      This routine prints out the contents of a        
  *                a node_data or cell_data data array component 
  *                of a field.        
  *
  *  Inputs:
  *    FILE            *output_fp    file ptr of file to write array values to 
  *    OMobj_id        *obj_id_ptr   for node_data: ptr to obj id of field
  *                                  for cell_data: ptr to obj id of cell set
  *    num_arr_vals    int           number of data array components in this array
  *    node_cell_flag  int           indicates whether this is a node_data or a 
  *                                  cell_data_array
       int             *allout       data output control
  *
  *  Outputs:
  *    None 
  *
  ******************************************************************/

 int write_data_array (FILE *output_fp, OMobj_id *obj_id_ptr, 
                      int num_arr_vals, int node_cell_flag, int *allout)
 {

    /************************/
    /*  type definition     */
    /************************/

    /* type def for FLD calls that can return data of various types */
    union data {
       char            type_char;
       unsigned char   type_byte;
       short           type_short; 
       int             type_int;
       float           type_float;
       double          type_double;
    };


    /***************************/
    /*  variable declarations  */
    /***************************/

 
    OMobj_id     obj_id = *obj_id_ptr;
    int          status = OM_STAT_SUCCESS;     

    char         tab_str[5];
    int          comp;      /* current component in the data array */

    int          veclen = 0;
    int          id = 0;

    int          data_type = 0;
    char         *values_arr = (char *) NULL;
    int          values_index;
    int          array_size = 0;

    int          nvals;
    int          max_val;
    int          cur_val;
    int          cur_vec;

    union data   min;
    union data   max;
    union data   null_value;

    union data   *min_vec_arr = (void *) NULL;
    union data   *max_vec_arr = (void *) NULL;

    int          null_flag = 0;
    char         *label_str = (char *) NULL;
    char         *units_str = (char *) NULL;

    int          cur_val_int;
    int          data_out;


    /* data output control */
    data_out = *allout;

    /* set the tab string
       for node_data, the values are printed out with just 1 indentation
       level (1 tab).  For cell_data, there are 2 tabs because the 
       array is within a cell set.  The cell set is at the first 
       indentation level and the data array is at the second.
    */

    if (node_cell_flag == NODE_DATA_ARRAY)
       strcpy (tab_str, "\t");
    else
       strcpy (tab_str, "\t\t");


    /**********************************************************************/
    /*  loop through and print out each data array structure in the array */
    /**********************************************************************/

    for (comp=0; comp<num_arr_vals; comp++) {

       /* write a title string for the this set of node data */

       if (node_cell_flag == NODE_DATA_ARRAY)
          fprintf (output_fp, "node_data (%i):\n\n", comp);
       else
          fprintf (output_fp, "\tcell_data (%i):\n\n", comp);


       /* fetch and output vector length */

       if (node_cell_flag == NODE_DATA_ARRAY)
          status = FLDget_node_data_veclen(obj_id, comp, &veclen);
       else
          status = FLDget_cell_data_veclen (obj_id, comp, &veclen);

       if (status == OM_STAT_SUCCESS)
          fprintf (output_fp, "%svector length (veclen):\t%i\n", tab_str, veclen);
       else if (status == OM_STAT_UNDEF)
          fprintf (output_fp, "%svector length (veclen):\t%s\n", 
                               tab_str, UNDEFINED_STR);
       else
          ERRerror ("print_field", 1, ERR_ORIG, "error trying to get vector length");


       /* fetch and output id of this data array component */

       if (node_cell_flag == NODE_DATA_ARRAY) 
          status = FLDget_node_data_id (obj_id, comp, &id);
       else
          status = FLDget_cell_data_id (obj_id, comp, &id);
 
       if (status == OM_STAT_SUCCESS)
          fprintf (output_fp, "%sid:\t\t\t%i\n\n", tab_str, id);
       else if (status == OM_STAT_UNDEF)
          fprintf (output_fp, "%sid:\t\t\t%s\n\n", tab_str, UNDEFINED_STR);
       else
          ERRerror ("print_field", 1, ERR_ORIG, "error trying to get data array");



       /*******************************************/
       /*  get the values array                   */
       /*******************************************/

       if (node_cell_flag == NODE_DATA_ARRAY)
          status = FLDget_node_data (obj_id, comp, &data_type, 
                                     &values_arr, &array_size, 
	    			     OM_GET_ARRAY_RD);
       else
          status = FLDget_cell_data (obj_id, comp, &data_type, 
                                     &values_arr, &array_size, 
	    			     OM_GET_ARRAY_RD);

       if (status == OM_STAT_UNDEF)
          fprintf (output_fp, "%svalues array is UNDEFINED", tab_str);
       else if (status == OM_STAT_ERROR) 
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error trying to get values data array");
 
       else  { /* got array successfully */
     
          /* the data array is nvals x veclen big.  array_size
             is the total number of values in the array, or nvals*veclen
             So, we can compute nvals:  nvals = array_size/veclen
          */

          nvals = array_size/veclen;

          /* set max values to display */

#ifdef OLD
          if (nvals > MAX_DISPLAY_VAL)
	     max_val = MAX_DISPLAY_VAL;
          else
	     max_val = nvals;
#else
          max_val = nvals;
          if (data_out) {
            cur_val_int = 1;
          }
          else {
            cur_val_int = nvals/MAX_DISPLAY_VAL;
          }
#endif

 
          /* write a header for the values array */
          if (node_cell_flag == NODE_DATA_ARRAY)
             fprintf (output_fp, 
                      "\tnode data values (to maximum of %i values):\n\n", max_val);
          else
             fprintf (output_fp, 
                      "\t\tcell data values (to maximum of %i values):\n\n", max_val);

          /* as with the previous values array, values is a 2-D array 
	     (nvals x veclen big) that we look at in 1-D 
	     loop through the array and print out its values
          */

          values_index = 0;

          for (cur_val=0; cur_val< max_val; cur_val=cur_val+cur_val_int) {

	     fprintf (output_fp, "\t%s%i:\t{ ", tab_str, cur_val);

	     for (cur_vec=0; cur_vec<veclen; cur_vec++) {

	        switch (data_type) {
		    case DTYPE_CHAR:
                       fprintf (output_fp, "%c",
                               ((char *)values_arr)[values_index]);
                       break;
                    case DTYPE_BYTE:
                       fprintf (output_fp, "%u",
                               ((unsigned char *)values_arr)[values_index]);
                       break;
                    case DTYPE_SHORT:
                       fprintf (output_fp, "%hi",
                               ((short *)values_arr)[values_index]);
                       break;
                    case DTYPE_INT:
                       fprintf (output_fp, "%i",
                               ((int *)values_arr)[values_index]);
                       break;
                    case DTYPE_FLOAT:
                       fprintf (output_fp, "%f",
                               ((float *)values_arr)[values_index]);
                       break;
                   case DTYPE_DOUBLE:
                       fprintf (output_fp, "%f",
                              ((double *)values_arr)[values_index]);
		       break;
                    case DTYPE_UNSET:
                       ERRerror ("print_field", 1, ERR_ORIG, 
                                 "unset data type encountered in the data array");
                       break;
		    default:
	               ERRerror ("print_field", 1, ERR_ORIG, 
                               "unknown data type encountered in the data array");
        	       break;
	        }  /* end switch */
                values_index++;
       
                /* if last one, add close brace and a newline, otherwise,
                   just add comma and a space
                */
                if (cur_vec == veclen-1)
                   fprintf (output_fp, " }\n "); 
                else 
                   fprintf (output_fp, ", "); 
       
             }  /* end for cur_vec */
          }  /* end for cur_val */

          fprintf (output_fp, "\n"); 
          ARRfree (values_arr);

       }  /* end got the values array successfully */


       /* fetch null_flag and NULL VAlue; output null flag */

       if (node_cell_flag == NODE_DATA_ARRAY)
          status = FLDget_node_null_data(obj_id, comp, &null_flag, 
                                        (char *) &null_value);
       else
          status = FLDget_cell_null_data(obj_id, comp, &null_flag, 
                                        (char *) &null_value);

       if (status == OM_STAT_SUCCESS)
          fprintf (output_fp, "%snull_flag:\t\t  %i\n", tab_str, null_flag);
       else if (status == OM_STAT_UNDEF)
          fprintf (output_fp, "%snull_flag:\t  %s\n", tab_str, UNDEFINED_STR);
       else
          ERRerror ("print_field", 1, ERR_ORIG, "error getting null data");


       /* fetch min and max data values in the array */

       if (node_cell_flag == NODE_DATA_ARRAY)
           status = FLDget_node_data_minmax(obj_id, comp, (char *)&min, 
                                           (char *)&max);
       else
           status = FLDget_cell_data_minmax(obj_id, comp, (char *)&min, 
                                           (char *)&max);
       if (status == OM_STAT_ERROR)
          ERRerror("print_field", 1, ERR_ORIG,"error getting minmax values");

     
       /*  get the minimum and maximum vector arrays  */ 

       data_type = 0;
       if (node_cell_flag == NODE_DATA_ARRAY)
          status = FLDget_node_data_minmax_vec (obj_id, comp, 
                            (char **)&min_vec_arr, (char **)&max_vec_arr, 
                            &data_type, OM_GET_ARRAY_RD);
       else
          status = FLDget_cell_data_minmax_vec (obj_id, comp, 
                            (char **)&min_vec_arr, (char **)&max_vec_arr, 
                            &data_type, OM_GET_ARRAY_RD);
       if (status == OM_STAT_ERROR)
          ERRerror ("print_field", 1, ERR_ORIG, "error getting min/max vector arrays");



       /**********************************************************/
       /* now output null value, min, max, min_vec and max_vec,  */
       /* based on data type                                     */
       /**********************************************************/


       switch (data_type) {

	  case DTYPE_CHAR:

             fprintf(output_fp,"%snull value:\t\t  %c\n\n", tab_str, 
                                null_value.type_char); 
             fprintf (output_fp, "%sdata type:\t\t  DTYPE_CHAR\n\n", tab_str);

             fprintf(output_fp,"%sminimum (min):\t\t  %c\n", tab_str, 
                               min.type_char); 
             fprintf(output_fp,"%smaximum (max):\t\t  %c\n", tab_str, 
                                max.type_char);

             /* output the min vector array */

             fprintf (output_fp, "%sminimum vector (min_vec): { ", tab_str);
             for (cur_vec=0; cur_vec<veclen-1; cur_vec++) { 
	        fprintf (output_fp, "%c, ", min_vec_arr[cur_vec].type_char); 
             }
             fprintf (output_fp, "%c }\n ", min_vec_arr[cur_vec].type_char); 

             /* now print max vector array */

             fprintf (output_fp, "%smaximum vector (max_vec): { ", tab_str);
             for (cur_vec=0; cur_vec<veclen-1; cur_vec++) {
	        fprintf (output_fp, "c, ", max_vec_arr[cur_vec].type_char); 
             }
             fprintf (output_fp, "%c }\n\n", max_vec_arr[cur_vec].type_char); 

	     break;

	  case DTYPE_BYTE:      

             fprintf (output_fp, "%snull value:\t\t  %u\n\n", tab_str, 
                                 null_value.type_byte); 
             fprintf (output_fp, "%sdata type:\t\t  DTYPE_BYTE\n\n", tab_str);

             fprintf (output_fp, "%sminimum (min):\t\t  %u\n", tab_str, 
                                 min.type_byte); 
             fprintf (output_fp, "%smaximum (max):\t\t  %u\n", tab_str, 
                                  max.type_byte);


             /* output the min vector array */

             fprintf (output_fp, "%sminimum vector (min_vec): { ", tab_str);
             for (cur_vec=0; cur_vec<veclen-1; cur_vec++) { 
	        fprintf (output_fp, "%u, ", min_vec_arr[cur_vec].type_byte); 
             }
             fprintf (output_fp, "%u }\n ", min_vec_arr[cur_vec].type_byte); 

             /* now print max vector array */

             fprintf (output_fp, "%smaximum vector (max_vec): { ", tab_str);
             for (cur_vec=0; cur_vec<veclen-1; cur_vec++) {
	        fprintf (output_fp, "%u, ", max_vec_arr[cur_vec].type_byte); 
             }
             fprintf (output_fp, "%u }\n\n", max_vec_arr[cur_vec].type_byte); 

	     break;

	  case DTYPE_SHORT:

             fprintf (output_fp, "%snull value:\t\t  %hi\n\n", tab_str, 
                                 null_value.type_short); 
             fprintf (output_fp, "%sdata type:\t\t  DTYPE_SHORT\n\n", tab_str);

             fprintf (output_fp, "%sminimum (min):\t\t  %hi\n", tab_str, 
                                  min.type_short); 
             fprintf (output_fp, "%smaximum (max):\t\t  %hi\n", tab_str, 
                                  max.type_short);

             /* output the min vector array */

             fprintf (output_fp, "%sminimum vector (min_vec): { ", tab_str);
             for (cur_vec=0; cur_vec<veclen-1; cur_vec++) { 
	        fprintf (output_fp, "%hi, ", min_vec_arr[cur_vec].type_short);
             }
             fprintf (output_fp, "%hi }\n ", min_vec_arr[cur_vec].type_short);

             /* now print max vector array */

             fprintf (output_fp, "%smaximum vector (max_vec): { ", tab_str);
             for (cur_vec=0; cur_vec<veclen-1; cur_vec++) {
	        fprintf (output_fp, "hi, ", max_vec_arr[cur_vec].type_short); 
             }
             fprintf (output_fp, "%hi }\n\n", max_vec_arr[cur_vec].type_short);

	     break;

	  case DTYPE_INT:

             fprintf(output_fp, "%snull value:\t\t  %i\n\n", tab_str,
                                  null_value.type_int); 
             fprintf (output_fp, "%sdata type:\t\t  DTYPE_INT\n\n", tab_str);

             fprintf(output_fp, "%sminimum (min):\t\t  %i\n", tab_str,  
                                 min.type_int); 
             fprintf(output_fp, "%smaximum (max):\t\t  %i\n", tab_str, 
                                  max.type_int);

             /* output the min vector array */

             fprintf (output_fp, "%sminimum vector (min_vec): { ", tab_str);
             for (cur_vec=0; cur_vec<veclen-1; cur_vec++) { 
	        fprintf (output_fp, "%i, ", min_vec_arr[cur_vec].type_int); 
             }
             fprintf (output_fp, "%i }\n ", min_vec_arr[cur_vec].type_int); 

             /* now print max vector array */

             fprintf (output_fp, "%smaximum vector (max_vec): { ", tab_str);
             for (cur_vec=0; cur_vec<veclen-1; cur_vec++) {
	        fprintf (output_fp, "i, ", max_vec_arr[cur_vec].type_int); 
             }
             fprintf (output_fp, "%i }\n\n", max_vec_arr[cur_vec].type_int); 

	     break;

	  case DTYPE_FLOAT:

             fprintf (output_fp, "%snull value:\t\t  %f\n\n", tab_str,
                                 null_value.type_float); 
             fprintf (output_fp, "%sdata type:\t\t  DTYPE_FLOAT\n\n", tab_str);

             fprintf (output_fp, "%sminimum (min):\t\t  %f\n", tab_str, 
                                 min.type_float); 
             fprintf (output_fp, "%smaximum (max):\t\t  %f\n", tab_str, 
                                  max.type_float);

             /* output the min vector array */

             fprintf (output_fp, "%sminimum vector (min_vec): { ", tab_str);
             for (cur_vec=0; cur_vec<veclen-1; cur_vec++) { 
	        fprintf (output_fp, "\t%f, ", min_vec_arr[cur_vec].type_float); 
             }
             fprintf (output_fp, "\t%f }\n ", min_vec_arr[cur_vec].type_float); 

             /* now print max vector array */

             fprintf (output_fp, "%smaximum vector (max_vec): { ", tab_str);
             for (cur_vec=0; cur_vec<veclen-1; cur_vec++) {
	        fprintf (output_fp, "\t%f, ", max_vec_arr[cur_vec].type_float); 
             }
             fprintf (output_fp, "\t%f }\n\n",max_vec_arr[cur_vec].type_float); 

	     break;

	  case DTYPE_DOUBLE:

             fprintf (output_fp, "%snull value:\t\t  %f\n\n", tab_str, 
                                  null_value.type_double); 
             fprintf (output_fp, "%sdata type:\t\t  DTYPE_DOUBLE\n\n", tab_str);

             fprintf (output_fp, "%sminimum (min):\t\t  %f\n", tab_str,
                                  min.type_double); 
             fprintf (output_fp, "%smaximum (max):\t\t  %f\n", tab_str, 
                                  max.type_double);

             /* output the min vector array */

             fprintf (output_fp, "%sminimum vector (min_vec): { ", tab_str);
             for (cur_vec=0; cur_vec<veclen-1; cur_vec++) { 
	        fprintf (output_fp, "%f, ", min_vec_arr[cur_vec].type_double); 
             }
             fprintf (output_fp, "%f }\n ", min_vec_arr[cur_vec].type_double); 

             /* now print max vector array */

             fprintf (output_fp, "%smaximum vector (max_vec): { ", tab_str);
             for (cur_vec=0; cur_vec<veclen-1; cur_vec++) {
	        fprintf (output_fp, "f, ", max_vec_arr[cur_vec].type_double); 
             }
             fprintf (output_fp, "%f }\n\n",max_vec_arr[cur_vec].type_double); 

	     break;

          case DTYPE_UNSET:
             ERRerror ("print_field", 1, ERR_ORIG, 
                        "unset data type encountered in the data array");
             break;

	  default:
             ERRerror ("print_field", 1, ERR_ORIG, 
                        "unknown data type encountered in the data array");
             break;

       }  /* end switch */

       ARRfree ((char *) min_vec_arr);
       ARRfree ((char *) max_vec_arr);

       /* get and output the label string
       */
 
       label_str = (char *) malloc (MAX_STRING_SIZE);
       if (label_str == NULL) {
          ERR_RETURN ("ERROR - unable to allocate space");
       }
       array_size = MAX_STRING_SIZE;    

       if (node_cell_flag == NODE_DATA_ARRAY)
          status = FLDget_node_data_label (obj_id, comp, label_str, array_size);
       else
          status = FLDget_cell_data_label (obj_id, comp, label_str, array_size);

       if ((status == OM_STAT_SUCCESS) && (label_str != (char *) NULL))
          fprintf(output_fp, "%slabel:\t\t\t  \"%s\"\n", tab_str, label_str);
       else if (status == OM_STAT_ERROR) 
          ERRerror ("print_field", 1, ERR_ORIG, "error getting data label");
       else  /* status is UNDEF or status is SUCCESS, but ptr is null */
          fprintf(output_fp, "%slabel:\t\t\t  %s\n", tab_str, UNDEFINED_STR);



       /* finally, get and output the units string
       */
 
       units_str = (char *) malloc (MAX_STRING_SIZE);
       if (units_str == NULL) {
          ERR_RETURN ("ERROR - unable to allocate space");
       }
       array_size = MAX_STRING_SIZE;    

       if (node_cell_flag == NODE_DATA_ARRAY)
          status = FLDget_node_data_units (obj_id, comp, units_str, array_size);
       else
          status = FLDget_cell_data_units (obj_id, comp, units_str, array_size);

       if ((status == OM_STAT_SUCCESS) && (units_str != (char *) NULL))
          fprintf (output_fp, "%sunits:\t\t\t  \"%s\"\n\n", 
                   tab_str, units_str);
       else if (status == OM_STAT_ERROR)
          ERRerror ("print_field", 1, ERR_ORIG, "error getting data units");
       else  /* status is UNDEF or status is SUCCESS, but ptr is null */
          fprintf(output_fp,"%sunits:\t\t\t  %s\n\n", 
                   tab_str, UNDEFINED_STR);
     
       free (label_str);
       free (units_str);

    }  /* end for each set of node_data */

    fprintf (output_fp, "\n");

    return(status);

 } /* end write_data_array module */

 

 /******************************************************************
  *
  *  Module Name:  write_grid
  *
  *  Purpose:      This routine prints out the grid component of a
  *                a field to an output file.                
  *
  *  Inputs:
  *    FILE     *output_fp      file ptr of file to write grid values to 
  *    OMobj_id *fld_id_ptr     ptr to object id of the field in question
       int      *allout          control for printout whole range data
  *
  *  Outputs:
  *    int *    grid_type_ptr   returns what kind of grid this is
  *                               0 = structured grid
  *                               1 = uniform grid
  *                               2 = rectilinear grid
  *                              -1 = unstructured grid
  *
  ******************************************************************/

 int write_grid(FILE *output_fp, OMobj_id *fld_id_ptr, int *grid_type_ptr,  
     int *allout, int *htmlout, int *nnodes_num)
 {


    /***************************/
    /*  variable declarations  */
    /***************************/

    int      status = OM_STAT_SUCCESS;     
    OMobj_id field_id = *fld_id_ptr;

    int      display_coords_flag;
    char     grid_string[50];      
    int      grid_type;

    int      nnodes = 0;
    int      max_node;
    int      nspace = 0; 
    int      ndim = 0; 
    int      *dims = (int *)NULL;
    int      cur_dim;
    int      array_size = 0;

    int      npoints = 0; 
    float    *points_arr = (float *)NULL; 
    int      cur_pt;
    int      points_index;
    int      cur_space;

    float    *coord_vals_arr = (float *) NULL;
    int      cur_val;
    int      cur_vec;
    int      veclen;
    int      values_index;

    float    *min_vec_arr = (float *) NULL;
    float    *max_vec_arr = (float *) NULL;
    char     *units_str = (char *) NULL;
    
    int      cur_val_int;
    int      all_data;
    int      html_flag;

/* remove when FLDget_coord_units is fixed */

        OMobj_id    elem_id;


    /*  write out a header for this section 
    */

    html_flag = *htmlout;
    if (html_flag) {
        fprintf (output_fp, "<A NAME=\"Grid_Values\"></A>\n");
    }

    status = write_header (output_fp, "GRID Values");  /* ignore errors */

    if (html_flag) {
        fprintf (output_fp, "<A HREF=\"#TOP\">Go to Top</A><br>\n");
    }


    /* Get response to 'display coordinates?' questions
    */
    display_coords_flag = TRUE;


    /* get grid type */

    status = FLDget_grid_type (field_id, grid_type_ptr);

    if (status != OM_STAT_SUCCESS)
       *grid_type_ptr = UNSTRUCTURED;

    grid_type = *grid_type_ptr;

    /* map type integer value to a string */

    switch (grid_type) { 
       case UNSTRUCTURED:
	   strcpy (grid_string, "unstructured");
	   break;
       case STRUCTURED:
	   strcpy (grid_string, "structured");
	   break;
       case UNIFORM:
	   strcpy (grid_string, "uniform");
	   break;
       case RECTILINEAR:
	   strcpy (grid_string, "rectilinear");
	   break;
       default:
           ERR_RETURN ("undefined grid type returned");
    }

    /* output grid type */

    fprintf (output_fp, "grid type:  %s\n\n", grid_string); 


    /* get and output nnodes */

    status = FLDget_nnodes (field_id, &nnodes);
    if (status == OM_STAT_SUCCESS)
       fprintf (output_fp, "number of nodes (nnodes):\t\t\t%i\n", nnodes);
    else if (status == OM_STAT_UNDEF)
       fprintf (output_fp, "number of nodes (nnodes):\t\t\t%s\n", 
                           UNDEFINED_STR);
    else
       ERRerror ("print_field", 1, ERR_ORIG, "error getting nnodes of grid");

    *nnodes_num = nnodes;

    /* get and output nspace */

    FLDget_nspace (field_id, &nspace);
    if (status == OM_STAT_SUCCESS)
       fprintf (output_fp, "physical dimensions (nspace):\t\t\t%i\n\n", 
                nspace);
    else if (status == OM_STAT_UNDEF)
       fprintf (output_fp, "physical dimensions (nspace):\t\t\t%s\n", 
                           UNDEFINED_STR);
    else
       ERRerror ("print_field", 1, ERR_ORIG, "error getting nspace of grid");



    /* get and output ndim and dims, but only if this is a
       structured, uniform, or rectilinear grid
    */

    if (grid_type != UNSTRUCTURED) {

       /* get and output ndim */

       status = FLDget_ndim (field_id, &ndim);
       if (status == OM_STAT_SUCCESS)
          fprintf (output_fp, 
                 "number of computational dimensions (ndim):\t%i\n\n", ndim);
       else if (status == OM_STAT_UNDEF)
          fprintf (output_fp, 
                    "number of computational dimensions (ndim):\t%s\n\n", 
                     UNDEFINED_STR);
       else
          ERRerror("print_field", 1, ERR_ORIG, "error getting ndim of grid");



       /* get the dims array */

       status = FLDget_dims (field_id, &dims, &array_size);

       if (status == OM_STAT_UNDEF)
          fprintf (output_fp, "computational dimensions (dims):\t\t%s",
                   UNDEFINED_STR);
       else if (status == OM_STAT_ERROR)
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting dims array of grid");

       else {   /* successful */

          /* print out the dims array */

          fprintf (output_fp, "computational dimensions (dims):\t\t");

          /* notice that I'm printing out to array_size-1, or the
	     second to last value in the array.  That's because I'm
	     putting a comma after each value and I don't want to do
	     that for the last value.  We'll handle that separately
	     and put a newline after it.
          */

          for (cur_dim=0; cur_dim<array_size-1; cur_dim++) {
	     fprintf (output_fp, "%i, ", dims[cur_dim]);
          }
          fprintf (output_fp, "%i\n\n", dims[cur_dim]);

          ARRfree ((char *)dims);

       }   /* end successful getting dims array */
    }   /* end if grid_type */ 


    /*************************************************/
    /*  Output the coordinates, if requested by user */
    /*************************************************/

    if (display_coords_flag) { 

       /* write a header for the coordinates section */
       fprintf (output_fp, "coordinates:\n\n");

       /* Output veclen.  This is just nspace. */
       fprintf (output_fp, "\tvector length (veclen):\t  %i\n\n", nspace);


       /*
	   get and output the values array 
       */

       array_size = 0;
       status = FLDget_coord (field_id, &coord_vals_arr, &array_size, 
                              OM_GET_ARRAY_RD);
    
       if (status == OM_STAT_UNDEF)
          fprintf (output_fp, 
                  "\tcoordinate values:\t%s\n\n", UNDEFINED_STR);
       else if (status == OM_STAT_ERROR)
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting coordinate values array of grid");

       else {   /* successful */

          /* values is a 2-D array (nvals by veclen big) that 
	     we look at in 1-D since we pass in **float to the 
	     points routine
	     loop through the array and print out its values
          */

          /* can't display more than MAX_DISPLAY_VAL sets of values
	     If number of nodes exceeds MAX_DISPLAY_VAL, set max to 
             MAX_DISPLAY_VAL, otherwise, display them all
          */

#ifdef OLD
          if (nnodes > MAX_DISPLAY_VAL)
	     max_node = MAX_DISPLAY_VAL;
          else
	     max_node = nnodes;
#else
          max_node=nnodes;

          /* printout every point or only MAX_DISPLAY_VAL points */
          all_data = *allout;
   
          if (all_data) {
            cur_val_int = 1;
          }
          else {
            cur_val_int = nnodes/MAX_DISPLAY_VAL;

          }
#endif

          /* write a header for the coordinate values array */
          fprintf (output_fp, 
                  "\tcoordinate values (to maximum of %i values):\n\n", max_node);

          values_index = 0;

          for (cur_val=0; cur_val<max_node; cur_val= cur_val+cur_val_int) {
   
	     fprintf (output_fp, "\t\t%i:\t{ ", cur_val);

	     for (cur_vec=0; cur_vec<nspace-1; cur_vec++) {
		   fprintf(output_fp, "%f, ", coord_vals_arr[values_index]); 
		   values_index++;
	     }

	     /* last value in this vector - add close brace and a newline */
	     fprintf (output_fp, "%f }\n ", coord_vals_arr[values_index]); 
	     values_index++;

          }
          fprintf (output_fp, "\n"); 

          ARRfree ((char *) coord_vals_arr);

     }  /* end else successful */

       /*************************************************************/
       /*  Get and output the min_vec and max_vec arrays
       /*************************************************************/

       /* malloc space for the arrays and fetch them */

       min_vec_arr = (float *) malloc (nspace*sizeof(float));
       max_vec_arr = (float *) malloc (nspace*sizeof(float));

       if ((min_vec_arr == (float *)NULL) || (max_vec_arr == (float *)NULL)) {
	  /* can't malloc err message */
	  return (OM_STAT_ERROR);
       }

       status = FLDget_coord_extent (field_id, min_vec_arr, max_vec_arr);
       if (status == OM_STAT_ERROR)
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting min and max vector arrays of grid");

       else {

          if (min_vec_arr == (float *) NULL) 
             fprintf (output_fp, "\tminimum vector (min_vec): %s",
                                 UNDEFINED_STR);
          else {
             
             /* output the min vector array */

             fprintf (output_fp, "\tminimum vector (min_vec): { ");

             for (cur_vec=0; cur_vec<nspace-1; cur_vec++) {
	        fprintf (output_fp, "%f, ", min_vec_arr[cur_vec]); 
             }

             /* last value in vector - add a close brace and a newline */
             fprintf (output_fp, "%f }\n ", min_vec_arr[cur_vec]); 

             free (min_vec_arr);
          }
     

          /* now print max vector array */

          if (min_vec_arr == (float *) NULL) 
             fprintf (output_fp, "\tmaximum vector (max_vec): %s",
                                 UNDEFINED_STR);
          else {
             fprintf (output_fp, "\tmaximum vector (max_vec): { ");

             for (cur_vec=0; cur_vec<nspace-1; cur_vec++) {
	        fprintf (output_fp, "%f, ", max_vec_arr[cur_vec]); 
             }
             fprintf (output_fp, "%f }\n\n", max_vec_arr[cur_vec]); 

             free (max_vec_arr);
          }
       }


       /* finally, get and output the coordinate units string
       */


       units_str = (char *) malloc (MAX_STRING_SIZE);
       if (units_str == NULL) {
          ERR_RETURN ("ERROR - unable to allocate space");
       }
       array_size = MAX_STRING_SIZE;    


/*  !!! Put this call back in and remove subsequent lines of code
        when this gets fixed!!!
 */

/*
       status = FLDget_coord_units (field_id, units_str, array_size);
*/
 

/* remove code starting here */

        elem_id = OMfind_subobj (field_id, OMstr_to_name("coordinates"),
                                   OM_OBJ_RW);
        if (OMis_null_obj(elem_id))
                return(0);
 
        elem_id = OMfind_subobj (elem_id, OMstr_to_name("units"),
                                   OM_OBJ_RW);
        if (OMis_null_obj(elem_id))
                return(0);
        status = OMget_str_val (elem_id, &units_str, array_size);

/* stop removing code here */

       if (status == OM_STAT_SUCCESS)
	  fprintf (output_fp, "\tunits:\t\t\"%s\"\n\n", units_str);
       else if (status == OM_STAT_UNDEF)
	  fprintf (output_fp, "\tunits:\t\tUNDEFINED\n\n");
       else
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting units string of grid");

    }  /* end if display_coords */


    /*************************************************************/
    /*  Output the points info for uniform and rectilinear grids */
    /*************************************************************/

    if ((grid_type == UNIFORM) || (grid_type == RECTILINEAR)) {

       /* get and output npoints */

       status = FLDget_npoints (field_id, &npoints);
       if (status == OM_STAT_SUCCESS)
          fprintf (output_fp, "number of points (npoints):\t  %i\n\n", 
                   npoints);
       else if (status == OM_STAT_UNDEF)
          fprintf (output_fp, "number of points (npoints):\t  %s\n\n", 
                   UNDEFINED_STR);
       else
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting number of points of grid");



       /* get and output the points array */

       array_size = 0;
       status = FLDget_points (field_id, &points_arr, &array_size, OM_GET_ARRAY_RD);

       if (status == OM_STAT_UNDEF)
          fprintf (output_fp, "points:\t%s\n", UNDEFINED_STR);
       else if (status == OM_STAT_ERROR)
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting points array of grid");

       else {
          /* write a header for the points array */
          fprintf (output_fp, "points:\n");


          /* the points array is handled in the same 
	     way as the coordinate values array
	     loop through the array and print out its values
          */
          points_index = 0;

          for (cur_pt=0; cur_pt<npoints; cur_pt++) {

	     fprintf (output_fp, "\t\t %i:\t{ ", cur_pt);

	     for (cur_space=0; cur_space<nspace-1; cur_space++) {
		fprintf (output_fp, "%f, ", points_arr[points_index]); 
		points_index++;
	     }

	     /* last point in this set - add a close brace and a newline */
	        fprintf (output_fp, "%f }\n ", points_arr[points_index]); 
	        points_index++;
          }

          /* last set of points - add newlines */
	     fprintf (output_fp, "\n\n"); 

          ARRfree ((char *)points_arr);

       }  /* else successful getting points */
    }  /* end if rectilinear or uniform grids */

    return (status);

 }  /* end write_grid module */

 

 /******************************************************************
  *
  *  Module Name:  write_xform_value  
  *
  *  Purpose:      This subroutine retrieves a particular piece of  
  *                transformation information and outputs it to the 
  *                print_field output file.  The component of the 
  *                the xform info to get is specified in the input
  *                parameter "comp_str".
  *
  *  Inputs:
  *    FILE      *output_fp     file ptr of file to write xform value to 
  *    OMobj_id  *fld_id_ptr    ptr to object id of the field in question
  *    char      *comp_str      string specifying the name of the component
  *                             of transformation information to retrieve
  *                             and output.
  *
  *  Outputs:
  *    none     
  *
  ******************************************************************/

 int write_xform_value (FILE *output_fp, OMobj_id *fld_id_ptr, char *comp_str)
 {


    /***************************/
    /*  variable declarations  */
    /***************************/

    OMobj_id    field_id = *fld_id_ptr;
    int         status = OM_STAT_SUCCESS;     
    char        err_string [MAX_STRING_SIZE];

    OMobj_id    elem_id;
    char        *arr_ptr;
    int         arr_size;
    int         type;
    int         i, j;   /* loop counters */



    /* get the object id of the subobject for this piece of xform info */ 

    elem_id = OMlookup_subobj (field_id, OMstr_to_name(comp_str), OM_OBJ_RD);
    if (OMis_null_obj(elem_id)) { 
       sprintf (err_string, "unable to retrieve %s sub-object of xform",
                             comp_str);
       ERRerror ("print__field", 1, ERR_ORIG, err_string);
       return (OM_STAT_ERROR);
    }

    arr_ptr = OMret_array_ptr (elem_id, OM_GET_ARRAY_RD, &arr_size, &type);

    if (arr_ptr == (char *) NULL)
       fprintf (output_fp, "\t%s:\tUNDEFINED_STR\n", comp_str);

    else {
       fprintf (output_fp, "\t%s:\t{", comp_str);
       for (i=0; i<2; i++) {
           fprintf (output_fp, "%f, ", arr_ptr[i]);
       }
       fprintf (output_fp, "%f}\n", arr_ptr[i]);
    }

} /* end write_xform_value module */


         
 /******************************************************************
  *
  *  Module Name:  write_xform
  *
  *  Purpose:      This routine prints out the xformation matrix of a
  *                a field to an output file.                
  *
  *  Inputs:
  *    FILE     *output_fp      file ptr of file to write xform values to 
  *    OMobj_id *fld_id_ptr     ptr to object id of the field in question
  *
  *  Outputs:
  *    none     
  *
  ******************************************************************/

 int write_xform (FILE *output_fp, OMobj_id *fld_id_ptr, int *htmlout) 
 {


    /***************************/
    /*  variable declarations  */
    /***************************/

    OMobj_id   field_id = *fld_id_ptr;
    int        status = OM_STAT_SUCCESS;     

    float      xform_matrix[4][4];
    int        rspace;
    OMobj_id   elem_id;
    int        i, j;   /* loop counters */
    int        html_flag;


    /* write out a header for this section */


    html_flag = *htmlout;
    if (html_flag) {
        fprintf (output_fp, "<A NAME=\"XFORM_Values\"></A>\n");
    }

    status = write_header  (output_fp, "XFORM Values");   /* ignore errors */

    if (html_flag) {
        fprintf (output_fp, "<A HREF=\"#TOP\">Go to Top</A><br>\n");
    }




    /* get the transformation matrix */

    status = FLDget_xform  (field_id, (float *) &xform_matrix); 

    if (status == OM_STAT_UNDEF)
       fprintf (output_fp, "\tThe transformation matrix is undefined.\n\n");
    else if (status == OM_STAT_ERROR)
       ERRerror ("print_field", 1, ERR_ORIG, 
                 "error getting the transformation matrix");

    else {
         
       /* print title string */
       fprintf (output_fp, "transformation matrix (mat):\n");

       /* print the 4x4 matrix */
       for (i=0; i<4; i++) {
           fprintf (output_fp, "\t{");
           for (j=0; j<3; j++) {
               fprintf (output_fp, "%f, ", xform_matrix[i][j]);
           }
           fprintf (output_fp, "%f}\n", xform_matrix[i][j]);
       }
       fprintf (output_fp, "\n"); 

    } /* end OMget_xform was successful */



    /* get xlate, center, ocenter, and dcenter */

    write_xform_value (output_fp, fld_id_ptr, "xlate");
    write_xform_value (output_fp, fld_id_ptr, "center");
    write_xform_value (output_fp, fld_id_ptr, "ocenter");
    write_xform_value (output_fp, fld_id_ptr, "dcenter");
 


    /* get rspace */

    elem_id = OMlookup_subobj (field_id, OMstr_to_name("rspace"), OM_OBJ_RD);

    if (OMis_null_obj(elem_id)) 
       ERRerror ("print_field", 1, ERR_ORIG, 
                     "unable to retrieve rspace sub-object of xform");

    else {
       status = OMget_int_val (elem_id, &rspace);

       if (status == OM_STAT_SUCCESS)
          fprintf (output_fp, "\trspace:\t\t%i", rspace);
       else if (status == OM_STAT_UNDEF)
          fprintf (output_fp, "\trspace:\t\t%s", UNDEFINED_STR);
       else 
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting rspace of xform");
    }


    fprintf (output_fp, "\n\n");

} /* end write_xform_info */


 
 /******************************************************************
  *
  *  Module Name:  write_node_data
  *
  *  Purpose:      This routine prints out the contents of the  
  *                node data component of a field.        
  *
  *  Inputs:
  *    FILE     *output_fp      file ptr of file to write values to 
  *    OMobj_id *fld_id_ptr     ptr to object id of the field to write
  *
  *  Outputs:
  *    None 
  *
  ******************************************************************/

 int write_node_data(FILE *output_fp, OMobj_id *fld_id_ptr, int *allout, int *htmlout, int *component_num)
 {


    /***************************/
    /*  variable declarations  */
    /***************************/

    OMobj_id field_id = *fld_id_ptr;
    int      status = OM_STAT_SUCCESS;     

    int      nnode_data = 0; 
    int      data_out;
    int      html_flag;


    /* data output control */
    data_out = *allout;  

    /* write out a header for this section */

    html_flag = *htmlout;
    if (html_flag) {
        fprintf (output_fp, "<A NAME=\"NODE_DATA_Values\"></A>\n");
    }

    status = write_header  (output_fp, "NODE_DATA Values");   /* ignore errors */

    if (html_flag) {
        fprintf (output_fp, "<A HREF=\"#TOP\">Go to Top</A><br>\n");
    }


    /* get and output nnode_data */

    status = FLDget_node_data_ncomp (field_id, &nnode_data);
    if (status == OM_STAT_SUCCESS)
       fprintf (output_fp, 
                "number of node data components (nnode_data):\t%i\n\n", 
                nnode_data);
    else if (status == OM_STAT_UNDEF)
       fprintf (output_fp, 
                "number of node data components (nnode_data):\t%s\n\n", 
                UNDEFINED_STR);
    else
       ERRerror ("print_field", 1, ERR_ORIG, "error getting nnode data");



    /***********************************************************/
    /* call routine to handle node_data array of Data_Arrays   */
    /***********************************************************/  

    /* errors should be handled in subroutine, so we don't do it here */     
    data_out = *allout;
    status = write_data_array (output_fp, fld_id_ptr, nnode_data, 
                               NODE_DATA_ARRAY, &data_out);
    *component_num = nnode_data;

    return (status);

}  /* end write_node_data module */



/******************************************************************
 *
 *  Module Name:  write_cell_info
 *
 *  Purpose:      This routine prints out the contents of the  
 *                CELLS and Cell_Data components of a field.        
 *
 *  Inputs:
 *    FILE     *output_fp      file ptr of file to write values to 
 *    OMobj_id field_id        the object id of the field to write
 *    int      grid_type
 *
 *  Outputs:
 *    None 
 *
 ******************************************************************/
 
int write_cell_info(FILE *output_fp, OMobj_id *field_id_ptr, 
    int grid_type, int *allout, int *htmlout, int *ncell_num)
{

 
   /***************************/
   /*  variable declarations  */
   /***************************/
 
    OMobj_id   field_id = *field_id_ptr;
    int        status = OM_STAT_SUCCESS;     

    int        ncell_sets = 0;
    int        cur_set;
    OMobj_id   set_id;
    int        ncells = 0;
 
    char       *set_name_str = (char *) NULL;
    int        *node_conn_arr = (int *) NULL;
    int        array_size = 0;
    char       *set_type_str = (char *) NULL;    
   
    int        node_index;
    int        num_nodes = 0;
    int        cur_cell;
    int        cur_node;
    int        max_cell;

    int        cell_nnodes = 0;
    int        corner_nnodes = 0;
    int        cell_order = 0;
    int        poly_flag = 0;
    OMobj_id   elem_id;

    int        nprops = 0;
    int        cur_prop;
    float      *props_arr = (float *) NULL;

    int        ndim = 0;
    int        ncell_data = 0;

    int        cur_cell_int;
    int        data_out;
    int	       html_flag;

    char       *strbuf;

    /* control for all data point print */
    data_out = *allout;


    html_flag = *htmlout;
    if (html_flag) {
        fprintf (output_fp, "<A NAME=\"CELL_Values\"></A>\n");
    }

    status = write_header (output_fp, "CELL Values (Cells and Cell_Data)");

    if (html_flag) {
        fprintf (output_fp, "<A HREF=\"#TOP\">Go to Top</A><br>\n");
    }


    /* get and output number of cell sets */

    status = FLDget_ncell_sets (field_id, &ncell_sets);

    if (status == OM_STAT_SUCCESS)
       fprintf (output_fp, "number of cell sets (ncell_sets):\t%i\n\n", 
                ncell_sets);
    else if (status == OM_STAT_UNDEF)
       fprintf (output_fp, "number of cell sets (ncell_sets):\t%s\n\n", 
                UNDEFINED_STR);
    else 
       ERRerror ("print_field", 1, ERR_ORIG, "error getting ncell_sets");

    *ncell_num = ncell_sets;



    /********************************************************/
    /*  loop through and print out info for each cell set   */
    /********************************************************/


    /* note that if ncell_sets = 0, the loop won't be entered */

    for (cur_set=0; cur_set<ncell_sets; cur_set++) {   

       /* write a header for the cell set */
       fprintf (output_fp, "cell set %i:\n\n", cur_set);


       /* get the object id of this cell set */

       status = FLDget_cell_set (field_id, cur_set, &set_id);
       if (status == OM_STAT_ERROR) {
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting the cell set id");
/*        GOTO end of loop
*/
       }

       /* get and output number of cells */

       status = FLDget_ncells (set_id, &ncells);
       if (status == OM_STAT_SUCCESS)
          fprintf (output_fp, "\tnumber of cells (ncells):\t%i\n", ncells);
       else if (status == OM_STAT_UNDEF)
          fprintf (output_fp, "\tnumber of cells (ncells):\t%s\n", UNDEFINED_STR);
       else
          ERRerror ("print_field", 1, ERR_ORIG, "error getting ncells");



       /* get and output the cell set name */

       set_name_str = (char *) malloc (MAX_STRING_SIZE);
       if (set_name_str == NULL) {
          ERR_RETURN ("ERROR - unable to allocate space");
       }
       status = FLDget_cell_set_user_name (set_id, set_name_str, 
                                          MAX_STRING_SIZE);

       if (status == OM_STAT_SUCCESS) {
          fprintf (output_fp, "\tcell set name (name):\t\t\"%s\"\n\n", 
                               set_name_str);
          free (set_name_str);
       }
       else if (status == OM_STAT_UNDEF)
          fprintf (output_fp, "\tcell set name (name):\t\tUNDEFINED\n\n");
       else 
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting the cell set id");


       /* get the cell set type */

       array_size = 0;
       status = FLDget_cell_set_name (set_id, set_type_str, array_size);

       if ((status == OM_STAT_SUCCESS) && (set_type_str != (char *) NULL)) {
          fprintf (output_fp, "\tcell type name (cell_name):\t\"%s\"\n\n", 
                               set_type_str);
          free (set_type_str);
       }
       else if (status == OM_STAT_ERROR)
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting the cell set type");
       else  /* status is UNDEF or status is SUCCESS, but ptr is null */
          fprintf (output_fp, "\tcell type name (cell_name):\t%s\n\n",
                   UNDEFINED_STR);


       /********************************************************/
       /* get and output the node connectivity list            */
       /********************************************************/

       status = FLDget_node_connect (set_id, &node_conn_arr, 
                                     &array_size, OM_GET_ARRAY_RD);
       if (status == OM_STAT_UNDEF)
          fprintf (output_fp, 
               "\tnode connectivity list (node_connect_list):\t%s\n\n",
               UNDEFINED_STR);
       else if ((status == OM_STAT_ERROR) || (node_conn_arr == (int *) NULL))
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting the node connectivity list array");

       else {
          /* compute array extent
             node_connect_list is a 2-D array of ints, array_size big
             array is usually ncells*cell_nnodes.  Since we already have
             ncells, we can compute the other extent: array_size/ncells
          */
         
          num_nodes = array_size/ncells;

          /* set max ncells to display */

#ifdef OLD
          if (ncells > MAX_DISPLAY_VAL)
	     max_cell = MAX_DISPLAY_VAL;
          else
	     max_cell = ncells;
#else
          max_cell = ncells;
          if (data_out) { 
            cur_cell_int = 1;
          }
          else {
            cur_cell_int = max_cell/MAX_DISPLAY_VAL;
          }
#endif


          /* output title for the connectivity list */

          fprintf (output_fp, "\tnode connectivity list (node_connect_list):\n");


          /* loop through the integer array connectivity list and 
             print out values 
          */

          node_index = 0;
          for (cur_cell=0; cur_cell<max_cell; cur_cell=cur_cell+cur_cell_int) {
             fprintf (output_fp, "\n\t\t{");
	     for (cur_node=0; cur_node<num_nodes-1; cur_node++) {
                fprintf (output_fp, "%i, ", node_conn_arr[node_index]);
                node_index++;
	     }
             /* last value in this vector - add close brace and a newline */
             fprintf (output_fp, "%i}", node_conn_arr[node_index]);
             node_index++;
          }

          fprintf (output_fp, "\n\n");
          ARRfree ((char *) node_conn_arr);

       } /* end else successful */


       /********************************************************/
       /* Print out other values, starting with cell_ndim      */
       /********************************************************/

       /* if this is a structured, uniform, or rectilinear grid, get dims */
     
       if (grid_type != UNSTRUCTURED) {

          status = FLDget_cell_ndim (set_id, &ndim);

          if (status == OM_STAT_SUCCESS)
             fprintf (output_fp, 
               "\tnumber of computational dims (cell_ndim):\t%i\n", ndim);
          else if (status == OM_STAT_UNDEF)
             fprintf (output_fp, 
               "\tnumber of computational dims (cell_ndim):\t%s\n", 
               UNDEFINED_STR);
          else
             ERRerror("print_field", 1, ERR_ORIG, "error getting cell_ndim");
    
       }  /* end if not unstructured */


       /* get and output the number of nodes composing this cell */

       status = FLDget_cell_set_nnodes (set_id, &cell_nnodes);

       if (status == OM_STAT_SUCCESS)
          fprintf (output_fp, 
                   "\tnumber of nodes per cell (cell_nnodes):\t\t%i\n",
                   cell_nnodes);
       else if (status == OM_STAT_UNDEF)
          fprintf (output_fp, 
                   "\tnumber of nodes per cell (cell_nnodes):\t\t%s\n",
                   UNDEFINED_STR);
       else
          ERRerror ("print_field", 1, ERR_ORIG, "error getting cell_nnodes");



       /* get and output the number of corner nodes this cell has */

       status = FLDget_cell_corner_nnodes (set_id, &corner_nnodes);

       if (status == OM_STAT_SUCCESS)
          fprintf (output_fp, 
                   "\tnumber of corner nodes (cell_corner_nnodes):\t%i\n",
                   corner_nnodes);
       else if (status == OM_STAT_UNDEF)
          fprintf (output_fp, 
                   "\tnumber of corner nodes (cell_corner_nnodes):\t%s\n",
                   UNDEFINED_STR);
       else
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting cell_corner_nnodes");


       /* get and output the cell_order */

	elem_id = OMfind_subobj (set_id, OMstr_to_name("cell_order"), OM_OBJ_RD);
	if (OMis_null_obj(elem_id)) 
	   ERRerror ("print_field", 1, ERR_ORIG, 
                     "unable to retrieve cell_order sub-object");
        else {
    	   status = OMget_int_val (elem_id, &cell_order);
           if (status == OM_STAT_ERROR)
              ERRerror ("print_field", 1, ERR_ORIG, 
                        "unable to retrieve cell_order value");
           else
              fprintf (output_fp, "\tcell_order:\t\t\t\t\t%i\n", cell_order);
	}


       /* get and output the poly_flag */

	elem_id = OMfind_subobj (set_id, OMstr_to_name("poly_flag"), OM_OBJ_RD);
	if (OMis_null_obj(elem_id)) 
	   ERRerror ("print_field", 1, ERR_ORIG, 
                     "unable to retrieve poly_flag sub-object");
        else {
    	   status = OMget_int_val (elem_id, &poly_flag);
           if (status == OM_STAT_ERROR)
              ERRerror ("print_field", 1, ERR_ORIG, 
                        "unable to retrieve poly_flag value");
           else
              fprintf (output_fp, "\tpoly_flag:\t\t\t\t\t%i\n\n", poly_flag);
	}


       /* get and output the number of properties this cell has */

       status = FLDget_cell_nprops (set_id, &nprops);
       if (status == OM_STAT_SUCCESS)
          fprintf (output_fp, "\tnumber of properties (nprops):\t\t\t%i\n",
                   nprops);
       else if (status == OM_STAT_UNDEF)
          fprintf (output_fp, 
              "\tnumber of properties (nprops):\t\t\t%s\n", UNDEFINED_STR);
       else
          ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting cell number of properties (nprops)");



       /* now get and output the properties array */

       array_size = 0;
       status = FLDget_cell_props(set_id, &props_arr, 
                                  &array_size, OM_GET_ARRAY_RD);
       if (status == OM_STAT_ERROR)  {
          ERRerror ("print_field", 1, ERR_ORIG,"error getting cell properties");
          fprintf (output_fp, "\n\n\n");
       }
       else if (status == OM_STAT_UNDEF)
          fprintf (output_fp, "\tproperties (props):\t\t\t\tUNDEFINED\n\n\n");
 
       else {
          fprintf (output_fp, "\tproperties (props):\t\t\t\t");

          for (cur_prop=0; cur_prop<array_size-1; cur_prop++) {
             fprintf (output_fp, "%f, ", props_arr[cur_prop]);
          }
          fprintf (output_fp, "%f\n\n\n", props_arr[cur_prop]);

          ARRfree ((char *) props_arr);

       }
     
       /******************************************************/
       /* get and output CELL_DATA values for this cell set  */
       /******************************************************/

       /* print title for cell_data values */

       fprintf (output_fp, "\t**  CELL_DATA Values for cell set %i  **\n\n", 
                           cur_set);
     
       
       /* get and output number of data components for this cell */
	  
       status = FLDget_cell_data_ncomp (set_id, &ncell_data);

       /* hot wired for suppressing no-cell type error message */

#ifdef DEBUG
       strbuf = (char *)malloc(200);
       PUTVALUE("FLDget_cell_data_ncomp status ",status);
       PUTVALUE("grid_type ",grid_type);
       PUTVALUE("ncell_data ",ncell_data);
       printf("OM_SUCCESS= %d \n",OM_STAT_SUCCESS);
       printf("OM_STAT_UNDEF = %d \n", OM_STAT_UNDEF);
       printf("\n");
       free (strbuf);
#endif

       if (status == OM_STAT_SUCCESS)
          fprintf (output_fp, 
                    "\tnumber of data components (ncell_data):\t%i\n\n",
                    ncell_data);
       else if (status <= OM_STAT_UNDEF) 
          fprintf (output_fp, 
                    "\tnumber of data components (ncell_data):\t%s\n\n",
                    UNDEFINED_STR);
       else {
          /* 
	  ERRerror ("print_field", 1, ERR_ORIG, 
                    "error getting cell_data_ncomp");
          */
          fprintf (output_fp, 
                    "\tException: number of data components (ncell_data):\t%s\n\n",
                    UNDEFINED_STR);
       }
          
  
       /***********************************************************/
       /* call routine to handle cell_data array of Data_Arrays   */
       /***********************************************************/  

       /* errors written in subroutine */
     
       status = write_data_array (output_fp, &set_id, ncell_data, 
                                  CELL_DATA_ARRAY, &data_out);

   } /* end for each cell set */
   
   return (status);


}  /* end write_cell_info module */

