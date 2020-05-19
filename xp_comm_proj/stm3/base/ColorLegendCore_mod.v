module ColorLegendCore<src_file="ColorLegendCore.cxx",
                  	  out_hdr_file="ColorLegendCore_gen.h",
                  	  out_src_file="ColorLegendCore_gen.cxx",
				      	  cxx_hdr_files="fld/Xfld.h",
                  	  build_dir="xp_comm_proj/stm3/base"> {

   cxxmethod+req ColorLegendCore(
      .in+read+notify+req,
      .x+read+notify,
      .y+read+notify,
      .z+read+notify,
      .scale+read+notify,
      .label_offset+read+notify,
      .labels+write,
      .labels_pt+write,
      .fld+write);
	  
   XP_COMM_PROJ.STM3.BASE.DisplayParams &in<NEportLevels={2,0}>;
   
   float  x<NEportLevels={2,0}>[2]			 = {-0.80, -0.70};
   float  y<NEportLevels={2,0}>[2]			 = {-0.90,  0.90};
   float  z<NEportLevels={2,0}>    			 =  0.90;
   float  label_offset<NEportLevels={2,0}> = -0.13;
   float  scale<NEportLevels={2,0}> 		 =  0.02;
	
	string labels<NEportLevels={0,2}>[];
	
   Mesh labels_pt<NEportLevels={0,2}> {
		nspace = 3;
		ncell_sets = 1;
		cell_set+Point = {
		{
			ncells=><-.nnodes,name=>"labels",node_connect_list=>init_array(ncells,0,(ncells - 1))
		}};
	};
   Mesh+Node_Data+Float fld<NEportLevels={0,2}> {
		nspace = 3;
		ncell_sets = 1;
		cell_set+Point = {
		{
			ncells=><-.nnodes,name=>"atoms",node_connect_list=>init_array(ncells,0,(ncells - 1))
		}};
		nnode_data = 2;
		node_data = {
			{veclen = 3,
			 id = 667,
			 labels = "rgb"},
			{veclen = 1,
			 id = 668,
			 labels = "radius"}
		};
	};
};

