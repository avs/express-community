#include <avs/gd_def.h>
#include <avs/f_utils.h>

#include "gen.hxx"
#include "src/dxf_all.hxx"
#include "src/dxfdefine.hxx"

DXFHeader *globalVar;


int ReadDXF_ReadDXFMods_ReadDXFCore::update(OMevent_mask event_mask, int seq_num)
{
   //out.nspace = 3;
   int display = 0;
   DXFCellSet *cs;
   int cell_ncells;
   int cell_nnodes;
   int cell_ndim;
   int array_len;
   int *array;
   double *data;
   int nverts;
   double *pdata;

   char filename_buffer[AVS_PATH_MAX];


   // If this is an instance (or de-instance) notify, we want to  bail
   // out early.
   //
/*
   if ((event_mask & OM_EVENT_VAL) == 0)
   {
//       printf("In PBS_DXF_reader::read_file() for instance event\n");
      return 1;
   }
*/


   // If we have been called merely because the fill_polygons flag
   // has been changed, it would be nice to just recover the old 
   // DXFFile object
   /* if(  OMchanged(fill_polygons.obj_id() &&
       !OMchanged(file_name.obj_id() &&
	   !OMchanged(reverse_normals.obj_id()) */
       
   if (!(char *)file_name)  // ignore null file names
      return 1;

   // Use AVS/Express function to map enviroment variables into filename
   FILEmap_variables((char *)file_name, filename_buffer);


   // Instance a DXFFile object to read the specified DXF file.  This
   // object will then contain an internal representation of the contents
   // of the file.
   //
   DXFFile dxfFile((char *)filename_buffer, (int)reverse_normals);

   if (dxfFile.IsValid())
   {
	  // would like to save this DFFile object for future use
	  // but it's not that simple
	  /* dxf_file_ptr = &dxfFile; */

      //get the number of layers; is equal with number of outputfields
      printf("rd_dxf befor from GetNumLayer\n");
      int nout=dxfFile.GetNumLayers();
      nOut=nout;
      printf("rd_dxf after GetNumLayers %i \n",nout);	
      for(int i=0; i<nout; i++)
      {
         printf("rd_dxf before getnumpoints layer %i\n",i);
         OMset_str_array_val(layernames,i,dxfFile.GetLayername(i));
         int nnodes = dxfFile.GetNumPoints(i);
         printf("rd_dxf NumPoints: %i \n",nnodes);
         if (nnodes <= 0)	//not sure about this; realy empty or only empty layer
          {
	  printf("end of empty file or layer not displayed\n");
	  out[i].nnodes=0;
	  out[i].ncell_sets=0;
	  out[i].nnode_data=0;
	  // return 1;       // effectively, an empty file
	  }
	else
        {
         if(nnodes) 		 //not only text
         {  
            out[i].nspace = 3;
            out[i].nnodes = nnodes;
      	   // out.coordinates.values (float [])
           float *out_coordinates = (float *)out[i].coordinates.values.ret_array_ptr(OM_GET_ARRAY_RW);
           printf("rd_dxf before GetPoints\n");
	   dxfFile.GetPoints(out_coordinates);
	   printf("rd_dxf after GetPoints%f %f %f\n",out_coordinates[0],out_coordinates[1],out_coordinates[2]);
           ARRfree((float *)out_coordinates);
  
      	  // The cell set array needs to be "reinitialized" back to the default
      	  // type (Cell_Set) in the second and subsequent module executions,
      	  // to avoid constantly merging in new types in the FLDset_cell_set()
      	  // call.  One easy way to reset the cell_set array is to just set the
      	  //  to before going into the "foreach cell set" loop.
      	  //
	  /*printf("rd_dxf befor out.nspace %i %i %i\n",i,sizeof(out[i].nspace),sizeof(OMXint));
	  out[i].nspace = 3;
	  printf("rd_dxf after out.nspace\n");*/
      	  out[i].ncell_sets = 0; // clear out old cell set array
	  printf("rd_dxf  before GenNumCellSets\n"); 
     	  int q=dxfFile.GetNumCellSets();
          printf("rd_dxf after GetNumCellSets %i\n",q);
      	  FLDset_ncell_sets(out[i].obj_id(), q);
	  int s;
      	  FLDget_ncell_sets(out[i].obj_id(), &s);
      	  //out[i].ncell_sets = q;
      	  //printf("rd_dxf numCellSet %i %i %i",q,out[i].ncell_sets,s);
          printf("rd_dxf before MoveToFirstCellSet\n");
      	  dxfFile.MoveToFirstCellSet();
	  printf("rd_dxf after MoveToFirstCellSet\n");
      	  for (int out_set = 0;
	   (out_set < q) && dxfFile.MoreCellSets();
	   out_set++)
          {
	 cs = dxfFile.GetNextCellSet();

	 // printf("rd_dxf after got CellSet %i %i\n",out_set,out[i].ncell_sets);

	 int *index = cs->GetCells();

   	 //printf("rd_dxf first Pointer in CellSet\n");

	  for (int j = cs->GetNumVertices(); j > 0; j--, index++)
	   {
	   if((*(index))>= nnodes ) printf("\t %i \t",*(index));
	   }
	 
	 array = cs->GetCells();
 	 cell_ncells = cs->GetNumCells();
	 cell_nnodes = cs->GetNumVerticesPerCell();
	 cell_ndim = (cell_nnodes >= 3) ? 2 : (cell_nnodes == 2) ? 1 : 0;
	 array_len = cell_ncells * cell_nnodes;
         nverts = cs->GetNumVertices();
         
         printf("rd_dxf cell_ncell: %i cell_nnodes: %i cell_ndim: %i CellType: %i\n",cell_ncells
        	,cell_nnodes,cell_ndim,cs->GetCellType());

//       We can have points, lines or surfaces. For circles and polygons, the 
//       user decides
         if (cs->GetCellType() == DXF_CELL_POINT)
	    {
	    out[i].cell_set[out_set].ncells = cell_ncells;
	    out[i].cell_set[out_set].cell_nnodes = 1;
	    out[i].cell_set[out_set].cell_ndim = 0;
	    out[i].cell_set[out_set].cell_order = 1;  // no mid-edge nodes
	    out[i].cell_set[out_set].poly_flag = 0;
	    out[i].cell_set[out_set].node_connect_list.set_array(OM_TYPE_INT,
							      (char *)array, 
							      array_len,
							      OM_SET_ARRAY_COPY);
	
	  // printf("rd_dxf in DXF_CELL_Point\n");
							      
	    if ( cs->HasData())
	       {
               data = cs->GetData();
  	       out[i].cell_set[out_set].ncell_data = 1;
  	       out[i].cell_set[out_set].cell_data[0].veclen = 1;
  	       out[i].cell_set[out_set].cell_data[0].labels = "height";
  	       pdata = (double *)malloc((nverts)*sizeof(double));
  	       int i;
  	       for(i = 0; i < nverts; i++){
  	          pdata[i] = data[0];
  	          }
	       out[i].cell_set[out_set].cell_data[0].values.set_array(OM_TYPE_DOUBLE,
				                                  (char *)pdata,
							           nverts,
							       OM_SET_ARRAY_COPY);
               // printf("rd_dxf in DXF_CELL_Point HasData\n");
               free(pdata);							      
	       }
	    }
	 else if ((cs->GetCellType() == DXF_CELL_LINE) ||
	          (cs->GetCellType() == DXF_CELL_CHOOSE && fill_polygons == 0))
	    {
	    printf("rd_dxf in LINE %i\n",nverts-1);
            FLDset_cell_set(out[i].cell_set[out_set].obj_id(), "Line"); 
	    printf("rd_dxf first cell_set array\n");
 	    out[i].cell_set[out_set].cell_nnodes = 2; 
	    out[i].cell_set[out_set].ncells = (nverts-1 < 0) ? 0 : nverts-1;  
	    out[i].cell_set[out_set].cell_ndim = 1;
	    out[i].cell_set[out_set].cell_order = 1;  // no mid-edge nodes
	    out[i].cell_set[out_set].poly_flag = 0;
	    //printf("rd_dxf after out[i].cell_set\n");
            int *pconnect;
            pconnect = (int *)malloc((array_len*2 - 2)*sizeof(int));
            //printf("rd_dxf after malloc\n");
	    int j;
            for(j = 0; j < array_len -1; j++){
               pconnect[2*j    ] = array[j];
               pconnect[2*j + 1] = array[j + 1];
               }
	    out[i].cell_set[out_set].node_connect_list.set_array(OM_TYPE_INT,
				                              (char *)pconnect, 
				                              2*(array_len - 1),
				                              OM_SET_ARRAY_COPY);
										
	    if ( cs->HasData())
	      {
              data = cs->GetData();
   	      out[i].cell_set[out_set].ncell_data = 1;
  	      out[i].cell_set[out_set].cell_data[0].veclen = 1;
  	      out[i].cell_set[out_set].cell_data[0].labels = "height";
  	      double *pdata;
  	      pdata = (double *)malloc((nverts-1)*sizeof(double));
  	      int j;
  	      for(j = 0; j < nverts-1; j++){
  	         pdata[j] = data[0];
  	         }  
	      out[i].cell_set[out_set].cell_data[0].values.set_array(OM_TYPE_DOUBLE,
				                                  (char *)pdata,
							          nverts-1,
							          OM_SET_ARRAY_COPY);
              free(pdata);							      
	      }
           }
           else if (cs->GetCellType() == DXF_CELL_FACE){
 	   	out[i].cell_set[out_set].cell_nnodes = cell_nnodes; 
	   	out[i].cell_set[out_set].ncells = cell_ncells;  
	   	out[i].cell_set[out_set].cell_ndim = 2;
	   	out[i].cell_set[out_set].cell_order = 1;  // no mid-edge nodes
	   	out[i].cell_set[out_set].poly_flag = 0;
	   	out[i].cell_set[out_set].concave_flag = cs->IsConvex() ? 0 : 1;

	   	out[i].cell_set[out_set].node_connect_list.set_array(OM_TYPE_INT,
				                              (char *)array, 
				                              array_len,
				                              OM_SET_ARRAY_COPY);
	    
	   	if ( cs->HasData())
	      	{
              		data = cs->GetData();
  	      		out[i].cell_set[out_set].ncell_data = 1;
  	      		out[i].cell_set[out_set].cell_data[0].veclen = 1;
  	      		out[i].cell_set[out_set].cell_data[0].labels = "height";
	      		out[i].cell_set[out_set].cell_data[0].values.set_array(OM_TYPE_DOUBLE,
				                               (char *)data,
							       cell_ncells,
							       OM_SET_ARRAY_COPY);
	      	}
             }
         else if (fill_polygons == 1){
//         convert polylines to polyhedra 
          FLDset_cell_set(out[i].cell_set[out_set].obj_id(), "Polyhedron"); 
           out[i].cell_set[out_set].cell_nnodes = 3;
           out[i].cell_set[out_set].cell_ndim = 2;
	   out[i].cell_set[out_set].cell_order = 1;  // no mid-edge nodes
	   out[i].cell_set[out_set].poly_flag = 1;
           out[i].cell_set[out_set].poly_type = 1;
           out[i].cell_set[out_set].npolys = 1;
           int ncell_data;
           
/*        This section is intended to cut out end points which duplicate start
          points, but it doesn't work correctly yet - possibly the file does
          not contain the points defining the edges in the correct order???    

          if((out_coordinates[3*array[0]] == out_coordinates[3*array[array_len -1]]) &&
              (out_coordinates[3*array[0] + 1] == out_coordinates[3*array[array_len -1]+1] ) &&
              (out_coordinates[3*array[0] + 2] == out_coordinates[3*array[array_len -1]+2]) )  
              {
              // printf("set %d - chopping last point\n", out_set);
              int n_poly_nodes = cell_nnodes - 1;
              out[i].cell_set[out_set].poly_nnodes.set_array(OM_TYPE_INT,
                                                          (char *)&n_poly_nodes, 
                                                          1,
                                                          OM_SET_ARRAY_COPY);
              out[i].cell_set[out_set].poly_connect_list.set_array(OM_TYPE_INT,
                                                                (char *)array, 
                                                                array_len - 1,
                                                                OM_SET_ARRAY_COPY);
              ncell_data = nverts - 3;
              }
          else{ 
              printf("set %d - not chopping last point\n", out_set); */
             out[i].cell_set[out_set].poly_nnodes.set_array(OM_TYPE_INT,
                                                         (char *)&cell_nnodes, 
                                                         1,
                                                         OM_SET_ARRAY_COPY);

             out[i].cell_set[out_set].poly_connect_list.set_array(OM_TYPE_INT,
                                                               (char *)array, 
                                                               array_len,
                                                               OM_SET_ARRAY_COPY);
             ncell_data = nverts - 2;
          /*   }      */                                            
          if ( cs->HasData())
             {
             data = cs->GetData();
             out[i].cell_set[out_set].ncell_data = 1;
             out[i].cell_set[out_set].cell_data[0].veclen = 1;
             out[i].cell_set[out_set].cell_data[0].labels = "height";
             double *pdata;
             pdata = (double *)malloc((ncell_data)*sizeof(double));
             int j;
             for(j = 0; j < ncell_data; j++){
                pdata[j] = data[0];
                }
             out[i].cell_set[out_set].cell_data[0].values.set_array(OM_TYPE_DOUBLE,
                                                                 (char *)pdata,
                                                                 ncell_data,
                                                                 OM_SET_ARRAY_COPY);
             free(pdata);                                                       
             }
          }
         //ARRfree((float *)out_coordinates);
        }
     }   
     printf("rd_dxf keine CellSets mehr\n");
       
     out[i].nnode_data = 1;
	 out[i].node_data[0].veclen = 3;
     out[i].node_data[0].id = GD_COLOR_DATA_ID;

     printf("rd_dxf before GetColor\n");
     float *rgb = (float *)
	 out[i].node_data[0].values.ret_typed_array_ptr(OM_GET_ARRAY_RW,
						     OM_TYPE_FLOAT,
						     &array_len);
	 short *colors = new short[array_len];
	 
	 printf("rd_dxf vor GetColorNums\n");
	 
	 dxfFile.GetColorNums(colors);
	 

	 printf("rd_dxf array len: %i\n",array_len);

     for (int j = 0; j < array_len;)
       {
       
       
       // printf("rd_dxf colour: %i\n",colors[j/3]);
		 switch (colors[j/3])
		    {
		    case 1:
	          rgb[j++] = 1;
	          rgb[j++] = 0;
	          rgb[j++] = 0;
		      break;
			case 2:
	          rgb[j++] = 1;
	          rgb[j++] = 1;
	          rgb[j++] = 0;
		      break;
		    case 3:
	          rgb[j++] = 0;
	          rgb[j++] = 1;
	          rgb[j++] = 0;
		      break;
		    case 4:
	          rgb[j++] = 0;
	          rgb[j++] = 1;
	          rgb[j++] = 1;
		      break;
		    case 5:
	          rgb[j++] = 0;
	          rgb[j++] = 0;
	          rgb[j++] = 1;
		      break;
		    case 6:
	          rgb[j++] = 1;
	          rgb[j++] = 0;
	          rgb[j++] = 1;
		      break;
		    case 7:
	          rgb[j++] = 1;
	          rgb[j++] = 1;
	          rgb[j++] = 1;
		      break;
		    case 8:
		  rgb[j++] = 0.5;
	          rgb[j++] = 0.5;
	          rgb[j++] = 0.5;
	              break;
		    case 9:
		  rgb[j++] = 0.8;
	          rgb[j++] = 0.8;
	          rgb[j++] = 0.8;
	              break;
		    default:
		      rgb[j++] = 1;
		      rgb[j++] = 1;
		      rgb[j++] = 1;
		      break;
		 }
      }
      ARRfree((float *)rgb);
      delete colors;

      //ARRfree((char *)out_coordinates);

	//printf("rd_dxf nach GetColorNums\n");
    }	
   }
    //ARRfree((FLD_Field *)outf);
      // Finally the text strings if there are any
	  int ntext = dxfFile.GetNumText();
	  
	   //printf("rd_dxf nach GetNumText ntext: %i\n",ntext);
	  
	  nText = ntext;
	  if(ntext > 0)
	  {

		 structtext *stext;
		 //printf("Textareasize: %i haveTextSize: %i Textsize: %i Text[0]size: %i\n"
                 //,sizeof(DXF_rd_dxf_Text_array),sizeof(Text),sizeof(DXF_rd_dxf_Text),sizeof(Text[0]));
		 dxfFile.MoveToFirstText();
		 for (int i = 0; i < ntext;i++)
		 {
			stext=dxfFile.GetNextWholeText();
			Text[i].align_horiz=stext->align_horiz;
			Text[i].align_vert=stext->align_vert;
			OMobj_id Text_id = Text[i].str.obj_id();
			OMset_str_val(Text_id,stext->text);
			Text[i].stroke=1;
			Text[i].StrokeTextAttribs.style=stext->style;
			Text[i].StrokeTextAttribs.plane=stext->plane;
			Text[i].StrokeTextAttribs.orient=stext->orient;
			Text[i].StrokeTextAttribs.path=stext->path;
			Text[i].StrokeTextAttribs.angle=stext->angle;
			Text[i].StrokeTextAttribs.height=stext->height;
			Text[i].StrokeTextAttribs.expansion=stext->expansion;
			Text[i].nspace=3;
			Text[i].underline=stext->underline;
			float *pos=(float *)Text[i].position.ret_array_ptr(OM_GET_ARRAY_WR);
			if(pos){
			pos[0]=stext->point.GetX();
			pos[1]=stext->point.GetY();
			pos[2]=stext->point.GetZ();}
			//Text_id = Text[i].position.obj_id();
			//OMset_array(Text_id,OM_TYPE_FLOAT,(char *)pos,3,OM_SET_ARRAY_COPY);
			ARRfree((char *)pos);
		 }
	  }
      // return 1 for success
      DXFPoint3D extr;
      extr=globalVar->GetRExtMax();
      float *urca=(float*)urc.ret_typed_array_ptr(OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
      urca[0]=extr.GetX();
      urca[1]=extr.GetY();
      urca[2]=extr.GetZ();
      ARRfree((char *)urca);
      extr=globalVar->GetRExtMin();
      float *llca=(float*)llc.ret_typed_array_ptr(OM_GET_ARRAY_WR,OM_TYPE_FLOAT,NULL);
      llca[0]=extr.GetX();
      llca[1]=extr.GetY();
      llca[2]=extr.GetZ(); 
      ARRfree((char*)llca);
      printf("end\n");
      
      return 1;
   }

//    printf("Error reading \"%s\"\n", (char *)file_name);
   return 0;
}

