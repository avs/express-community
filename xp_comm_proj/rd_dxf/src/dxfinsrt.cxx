
#include "rd_dxf/avs_gen.hxx"


int
DXF_dxf_insert_detail::update(OMevent_mask event_mask, int seq_num)
{

   int i,j,k;
   // threshold (OMXfloat read req notify)
   float Threshold = threshold;

   /*
    * first pass: 
    *
    * go through the data set to work out how many nodes are 
    * required in out-field.  set the appropriate number of
    * nodes.  also for each cell-set work out how many new
    * cells will be required.  set the appropriate number in
    * out-field.
    */
   int totalNodes = in.nnodes;
   int newNodes;
   int newCells;

   // in (Field read req notify)

   // SET OUTPUT NSPACE, NCELL_SETS, NNODE_DATA
   if((int)in.nnode_data > 1) 
   {
      puts("More than one item of node data");
      return 0;
   }
   out.nnode_data = 1;

   if((int)in.node_data[0].veclen > 1)
   {
      puts("Node data must be scalar, not vector");
      return 0;
   }
   out.node_data[0].veclen = 1;

   out.nspace = (int) in.nspace;
   out.ncell_sets = (int) in.ncell_sets;

   // in.nspace (int) 
   // in.nnodes (int)
   // in.coordinates.values (float [])
   float *in_coordinates = (float *)in.coordinates.values.ret_array_ptr(OM_GET_ARRAY_RD);
   // in.ncell_sets (int)
   int in_set;
   for (in_set=0; in_set < (int)in.ncell_sets; in_set++) {
   	// in.cell_set[in_set].ncells (int)
   	// in.cell_set[in_set]. cell_nnodes (int)
   	// in.cell_set[in_set]. cell_ndim (int)
   	// in.cell_set[in_set]. cell_order (int)
   	// in.cell_set[in_set]. cell_name (char *)
   	// in.cell_set[in_set].node_connect_list (int *)

   	int *in_node_connect_list = (int *)in.cell_set[in_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);

        if(in.cell_set[in_set].cell_order == 1 && in.cell_set[in_set].cell_nnodes == 2)
        {
           // simple lines
          dxfinsert.line_cell_set_counter(  &newCells, &newNodes, 
                                          in_coordinates, (int)in.nnodes, 
                                          in_node_connect_list, (int)in.cell_set[in_set].ncells, 
                                          Threshold);
        }
        else
        {
           newCells = 0;
           newNodes = 0;
        }

        totalNodes = totalNodes + newNodes;
        // SET OUTPUT CELL_SET::NCELLS
        out.cell_set[in_set].ncells = (int)in.cell_set[in_set].ncells + newCells;
        out.cell_set[in_set].cell_nnodes = (int) in.cell_set[in_set].cell_nnodes;
        out.cell_set[in_set].cell_ndim = (int) in.cell_set[in_set].cell_ndim;
        out.cell_set[in_set].cell_order = (int) in.cell_set[in_set].cell_order;

        ARRfree((char *)in_node_connect_list);
   }


   // SET OUTPUT NNODES
   out.nnodes = totalNodes;

   int nodeCount = 0;
   int cellCount;

   // copy the existing coordinate sets and nodal data
   int  in_data_size, in_data_type;
   int  out_data_size, out_data_type;
   float *out_coordinates = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
   short *in_node_data = (short *)in.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_RD, &in_data_size, &in_data_type);
   if(in_data_type != OM_TYPE_SHORT)
   {
       puts("Data type must be SHORT");
       ARRfree((char *)in_coordinates);
       ARRfree((char *)out_coordinates);
       ARRfree((char *)in_node_data);
       return 0;
   }
   out_data_type = OM_TYPE_SHORT;
   short *out_node_data = (short *)out.node_data[0].values.ret_array_ptr(OM_GET_ARRAY_WR, &out_data_size, &out_data_type);

   float *pFin, *pFout;
   short *pSin, *pSout;
   pFin = in_coordinates;
   pFout = out_coordinates;
   pSin = in_node_data;
   pSout = out_node_data;
   for(nodeCount=0;nodeCount<(int)in.nnodes;nodeCount++)
   {
       *pFout = *pFin; pFin++; pFout++;
       *pFout = *pFin; pFin++; pFout++;
       *pFout = *pFin; pFin++; pFout++;
       
       *pSout = *pSin; pSin++; pSout++;
   }

   int *pIin, *pIout;
   for (in_set=0; in_set < (int)in.ncell_sets; in_set++) {


         
   	int *in_node_connect_list = (int *)in.cell_set[in_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_RD);
   	int *out_node_connect_list = (int *)out.cell_set[in_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);
        pIin = in_node_connect_list;
        pIout = out_node_connect_list;
        if(in.cell_set[in_set].cell_order == 1 && in.cell_set[in_set].cell_nnodes == 2)
        {
           // simple lines
           dxfinsert.line_cell_set_adder( in_node_connect_list, (int)in.cell_set[in_set].ncells,
                                          out_node_connect_list, (int)out.cell_set[in_set].ncells,
                                          in_coordinates, &pFout,
                                          in_node_data, &pSout,
                                          Threshold); 
               *pIout = *pIin; pIin++; pIout++;
        }
        else
        {

           *pIout = *pIin; pIin++; pIout++;

        }


        ARRfree((char *)in_node_connect_list);
        ARRfree((char *)out_node_connect_list);
   }


   ARRfree((char *)in_coordinates);
   ARRfree((char *)out_coordinates);
   ARRfree((char *)in_node_data);
   ARRfree((char *)out_node_data);

int mystop = 1;if(mystop)return 1;

   // in.nnodes (int)
   // in.nnode_data (int)
   int  in_data_comp;
//   int  in_data_size, in_data_type;
   for (in_data_comp = 0; in_data_comp < (int)in.nnode_data; in_data_comp++) { 
	// in.node_data[in_data_comp].veclen (int) 
   	// in.node_data[in_data_comp].values (char [])
   	char *in_node_data = (char *)in.node_data[in_data_comp].values.ret_array_ptr(OM_GET_ARRAY_RD, &in_data_size, &in_data_type);
        ARRfree((char *)in_node_data);
   }

   // out (Field write)

   // out.nspace (int) 
   // out.nnodes (int)
   // out.coordinates.values (float [])
  // float *out_coordinates = (float *)out.coordinates.values.ret_array_ptr(OM_GET_ARRAY_WR);
   // out.ncell_sets (int)
   int out_set;
   int out_ncell_sets = out.ncell_sets;
   for (out_set=0; out_set < (int)out.ncell_sets; out_set++) {
   	// out.cell_set[out_set].ncells (int)
   	// out.cell_set[out_set]. cell_nnodes (int)
   	// out.cell_set[out_set]. cell_ndim (int)
   	// out.cell_set[out_set]. cell_order (int)
   	// out.cell_set[out_set]. cell_name (char *)
   	// out.cell_set[out_set].node_connect_list (int *)

//   	int *out_node_connect_list = (int *)out.cell_set[out_set].node_connect_list.ret_array_ptr(OM_GET_ARRAY_WR);

    //    ARRfree((char *)out_node_connect_list);
   }


   // out.nnodes (int)
   // out.nnode_data (int)
   int  out_data_comp;
  // int  out_data_size, out_data_type;
   for (out_data_comp = 0; out_data_comp < (int)out.nnode_data; out_data_comp++) { 
	// out.node_data[out_data_comp].veclen (int) 
   	// out.node_data[out_data_comp].values (char [])
   	char *out_node_data = (char *)out.node_data[out_data_comp].values.ret_array_ptr(OM_GET_ARRAY_WR, &out_data_size, &out_data_type);
        ARRfree((char *)out_node_data);
   }


   /***********************/
   /* Function's Body     */
   /***********************/
//   printf("I'm in method: dxf_insert_detail::update\n");



   ARRfree((char *)in_coordinates);



   ARRfree((char *)out_coordinates);

   // return 1 for success
   return(1);
}


