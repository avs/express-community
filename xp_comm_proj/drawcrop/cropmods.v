flibrary DrawCropMods { 

   


  // define crop_controls


      module crop_controls<build_dir="xp_comm_proj/drawcrop/cropcont", out_hdr_file="gen.h">{
         int dims_fld<NEportLevels={2,1}>[];
         int restart<NEportLevels={2,1}>;
         int OK_axis2<NEportLevels={2,1}>;
         int OK_axis1<NEportLevels={2,1}>;
         int OK_axis0<NEportLevels={2,1}>;
         int all_OK<NEportLevels={2,1}>;
         int set_coord<NEportLevels={0,2}>;
         omethod+notify_inst+req update<src_file="cropcont.c",weight=0>(
            dims_fld+read+notify+req,
            restart+read+notify+req,
            OK_axis2+read+notify+req,
            OK_axis1+read+notify+req,
            OK_axis0+read+notify+req,
            all_OK+read+notify+req,
            set_coord+write
         ) = "crop_control";
      };





  // define crop_mouse  

      module crop_mouse<build_dir="xp_comm_proj/drawcrop/cropmous", out_hdr_file="gen.h"> {
         int dims_in<NEportLevels={2,1}>[];
         int set_coord<NEportLevels={2,1}>;
         int reset_coord<NEportLevels={0,2}>;
         float min_vec_axis2_cs<NEportLevels={2,1}>[];
         float min_vec_axis1_cs<NEportLevels={2,1}>[];
         float min_vec_axis0_cs<NEportLevels={2,1}>[];
         float max_vec_axis2_cs<NEportLevels={2,1}>[];
         float max_vec_axis1_cs<NEportLevels={2,1}>[];
         float max_vec_axis0_cs<NEportLevels={2,1}>[];
         float min_vec_axis2<NEportLevels={0,2}>[];
         float min_vec_axis1<NEportLevels={0,2}>[];
         float min_vec_axis0<NEportLevels={0,2}>[];
         float max_vec_axis2<NEportLevels={0,2}>[];
         float max_vec_axis1<NEportLevels={0,2}>[];
         float max_vec_axis0<NEportLevels={0,2}>[];
         omethod+notify_inst+req update1<src_file="cropmou1.c",weight=1>(
            dims_in+read+notify+req,
            set_coord+read+notify+req,
            min_vec_axis2+write,
            min_vec_axis1+write,
            min_vec_axis0+write,
            max_vec_axis2+write,
            max_vec_axis1+write,
            max_vec_axis0+write
         ) = "crop_mouse1";
         omethod+notify_inst+req update2<src_file="cropmou2.c",weight=2>(
            set_coord+read+notify+req,
            min_vec_axis2_cs+read+notify+req,
            max_vec_axis2_cs+read+notify+req,
            min_vec_axis2+write,
            max_vec_axis2+write
         ) = "crop_mouse2";
         omethod+notify_inst+req update3<src_file="cropmou3.c",weight=2>(
            set_coord+read+notify+req,
            min_vec_axis1_cs+read+notify+req,
            max_vec_axis1_cs+read+notify+req,
            min_vec_axis1+write,
            max_vec_axis1+write
         ) = "crop_mouse3";
         omethod+notify_inst+req update4<src_file="cropmou4.c",weight=2>(
            set_coord+read+notify+req,
            min_vec_axis0_cs+read+notify+req,
            max_vec_axis0_cs+read+notify+req,
            min_vec_axis0+write,
            max_vec_axis0+write
         ) = "crop_mouse4";
         omethod+notify_inst+req update5<src_file="cropmou5.c",weight=3>(
            set_coord+read+notify+req,
            reset_coord+write
         ) = "crop_mouse5";
      };



  // define cropcoordinate
 
      module crop_coordinate<build_dir="xp_comm_proj/drawcrop/cordcrop",out_hdr_file="gen.h"> {
         int set_coord<NEportLevels={2,1}>;
         int dims_in<NEportLevels={2,1}>[];
         float z_scale<NEportLevels={2,1}>;
         float min_vec_axis2<NEportLevels={2,1}>[];
         float min_vec_axis1<NEportLevels={2,1}>[];
         float min_vec_axis0<NEportLevels={2,1}>[];
         float max_vec_axis2<NEportLevels={2,1}>[];
         float max_vec_axis1<NEportLevels={2,1}>[];
         float max_vec_axis0<NEportLevels={2,1}>[];
         int stored[];
         int mins_store[];
         int maxs_store[];
         int mins<NEportLevels={1,2}>[];
         int maxs<NEportLevels={1,2}>[];
         float xlate1_axis2<NEportLevels={1,2}>[][];
         float xlate2_axis2<NEportLevels={1,2}>[][];
         float xlate1_axis1<NEportLevels={1,2}>[][];
         float xlate2_axis1<NEportLevels={1,2}>[][];
         float xlate1_axis0<NEportLevels={1,2}>[][];
         float xlate2_axis0<NEportLevels={1,2}>[][];
         omethod+notify_inst+req update1<src_file="corcrop1.c",weight=1>(
            set_coord+read+notify+req,
            dims_in+read+notify+req,
            z_scale+read+notify+req,
            min_vec_axis2+read+notify+req,
            max_vec_axis2+read+notify+req,
            stored+write,
            mins_store+write,
            maxs_store+write,
            xlate1_axis2+write,
            xlate2_axis2+write,
            xlate1_axis1+write,
            xlate2_axis1+write,
            xlate1_axis0+write,
            xlate2_axis0+write
         ) = "coordcrop1";
         omethod+notify_inst+req update2<src_file="corcrop2.c",weight=2>(
            set_coord+read+notify+req,
            dims_in+read+notify+req,
            z_scale+read+notify+req,
            min_vec_axis1+read+notify+req,
            max_vec_axis1+read+notify+req,
            stored+read+notify+req,
            mins_store+write,
            maxs_store+write,
            xlate1_axis1+write,
            xlate2_axis1+write,
            xlate1_axis0+write,
            xlate2_axis0+write
         ) = "coordcrop2";
         omethod+notify_inst+req update3<src_file="corcrop3.c",weight=3>(
            set_coord+read+notify+req,
            dims_in+read+notify+req,
            z_scale+read+notify+req,
            min_vec_axis0+read+notify+req,
            max_vec_axis0+read+notify+req,
            stored+read+notify+req,
            mins_store+write,
            maxs_store+write,
            xlate1_axis1+write,
            xlate2_axis1+write,
            xlate1_axis0+write,
            xlate2_axis0+write
         ) = "coordcrop3";
         omethod+notify_inst+req update4<src_file="corcrop4.c",weight=4>(
            set_coord+read+notify+req,
            mins_store+read,
            maxs_store+read,
            mins+write,
            maxs+write
         ) = "coordcrop4";
         omethod+notify_inst+req update5<src_file="corcrop5.c",weight=5>(
            set_coord+read+notify+req,
            dims_in+read+notify+req,
            z_scale+read+notify+req,
            stored+write,
            mins_store+write,
            maxs_store+write,
            xlate1_axis2+write,
            xlate2_axis2+write,
            xlate1_axis1+write,
            xlate2_axis1+write,
            xlate1_axis0+write,
            xlate2_axis0+write
         ) = "coordcrop5";
      };





//************************************************************************

  // define cropdata  

      module cropdata<build_dir="xp_comm_proj/drawcrop/cropdata", out_hdr_file="gen.h"> {
         int dims_in<NEportLevels={2,1}>[];
         int set_coord<NEportLevels={2,1}>;         
         int reset_coord<NEportLevels={0,2}>;
         int which_view<NEportLevels={2,1}>;
         float min_in<NEportLevels={2,1}>[];
         float max_in<NEportLevels={2,1}>[];
         float min_axis2<NEportLevels={0,2}>[];
         float max_axis2<NEportLevels={0,2}>[];
         float min_axis1<NEportLevels={0,2}>[];
         float max_axis1<NEportLevels={0,2}>[];
         float min_axis0<NEportLevels={0,2}>[];
         float max_axis0<NEportLevels={0,2}>[];

         omethod+notify_inst+req update1<src_file="cropdat1.c",weight=1>(
		dims_in+read+notify+req,
		set_coord+read+notify+req,
		min_axis2+write,
		max_axis2+write,
		min_axis1+write,
		max_axis1+write,
		min_axis0+write,
		max_axis0+write
         ) = "cropdat1";
         omethod+notify_inst+req update2<src_file="cropdat2.c",weight=2>(
		set_coord+read+notify+req,
                which_view+read+notify+req,
		min_in+read+notify+req,
		max_in+read+notify+req,
		min_axis2+write,		
		max_axis2+write	
         ) = "cropdat2";
         omethod+notify_inst+req update3<src_file="cropdat3.c",weight=2>(
		set_coord+read+notify+req,
                which_view+read+notify+req,
		min_in+read+notify+req,
		max_in+read+notify+req,
		min_axis1+write,
		max_axis1+write
         ) = "cropdat3";
         omethod+notify_inst+req update4<src_file="cropdat4.c",weight=2>(
		set_coord+read+notify+req,
                which_view+read+notify+req,
		min_in+read+notify+req,
		max_in+read+notify+req,
		min_axis0+write,
		max_axis0+write
         ) = "cropdat4";
         omethod+notify_inst+req update5<src_file="cropdat5.c",weight=3>(
		set_coord+read+notify+req,
		reset_coord+write
	) = "cropdat5";
      };

};
