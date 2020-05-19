library MERL<build_dir="src",out_hdr_file="vlixpgen.hxx",
             cxx_hdr_files="c:\\progra~1\\VolumePro\\inc\\vli.h",
             link_files="c:\\progra~1\\VolumePro\\lib\\vli.lib"> {
 
   enum VLIboolean {
     choices = {"VLIfalse", "VLItrue" };
   };

   group VLIBoardInfo {
      int available_memory;
   };

   group VLIConfigInfo {
      int major_version;
      int minor_version;
      int vli_major_version;
      int vli_minor_version;
      int max_cut_planes;
      int gradient_table_length;
      int board_count;
      VLIBoardInfo board_info[board_count];
   };

   group VLILookupTable {
      int   size;          // 256, 512, 1024, 2048, or 4096
      byte  red[size];
      byte  green[size];
      byte  blue[size];
      short alpha[size];   // each alpha can range from 0 to 4095 only
   };

   group VLICropTerm<export_cxx=1> {
      short en_x;
      short en_y;
      short en_z;
      short inv;
   };

   group VLICropBounds<export_cxx=1> {
      float min_x;
      float max_x;
      float min_y;
      float max_y;
      float min_z;
      float max_z;
      VLICropTerm term0;
      VLICropTerm term1;
      VLICropTerm term2;
      enum combine {
         choices = { "AND", "OR" };
      };
      int invert_result;
      enum predefined {
	 choices = { "SubVolume", "3DCross", "3DCrossInverted", "3DFence",
                     "3DFenceInverted", "User specified" };
      };
      int crop_enabled;
   };

   group VLICursorAxis<export_cxx=1> {
     boolean  enabled;
     double   red;    // 0 to 1
     double   green;  // 0 to 1
     double   blue;   // 0 to 1
     double   alpha;  // 0 to 1
   };

   group VLICursorInfo<export_cxx=1> {
     double  position[3];   // in VLI "object" coordinates
     boolean enabled;
     boolean disable_crop;
     enum type {
       choices = { "CrossHair", "Plane" };
     };
     double width;
     double length;
     VLICursorAxis x_axis;
     VLICursorAxis y_axis;
     VLICursorAxis z_axis;

   };
   
   group VLIVolumeInfo {
      VLIConfigInfo config_info;
      int data_component;
      Mesh_Unif+Dim3+Space3+Node_Data+Scalar+IPort2 &volume;
      DefaultXform &xform => volume.xform;
      Mesh_Unif+Dim2+Space2+IPort2 &slice_plane;
      double slice_offset;
      double slice_falloff;
      enum slice_flags {
         choices = { "disable", "inside", "outside" };
      };
      VLICropBounds+IPort2 &crop_bounds;
      VLILookupTable+IPort2 &lookup_table;
      VLICursorInfo+IPort2 &cursor;
      VLIboolean correct_opacity;
      VLIboolean grad_opacity_mod;
      VLIboolean grad_specular_illum_mod;
      VLIboolean grad_diffuse_illum_mod;
      double gradient_table[config_info.gradient_table_length];
      enum blend_mode {
         choices = { "Front to back", "MIP", "MinIP" };
      };
      float viewport_scale = 1;  // only needed for 'vox' files
      double x_shear;            // to correct for gantry tilt
      double y_shear;
      double z_shear;
      enum base_plane_accum {
        choices = { "Grow Base Plane", "Blend Base Plane" };
      };
      enum supersample_space {
	choices = { "Camera Space" , "Object Space" };
      };
      double x_supersample;
      double y_supersample;
      double z_supersample;
      enum texture_interp {
         choices = { "point", "bilinear" };
      };
      enum texture_type {
         choices = { "unfiltered", "mipmap" };
      };
      string vox_file_name;
      float tex_coords[6][2];
      ptr state<NEvisible=0>;

   };

   // module:  VLIVolume
   // purpose: This module implements a new render method to exercise the
   //          vg500 simulator.
   //
   VLIVolumeInfo+OPort VLIVolume<cxx_hdr_files="vliren.hxx",
                                 src_file="vliren.cxx"> {
      data_method+virtual+nosave render = "VLIVolumeRender";
      data_method+virtual+nosave pick = "GDpick_polytri";
      omethod+notify_deinst destroy<weight=0>() = "VLIVolumeDestroy";
   };

   module Volume_Histogram<src_file="hist.cxx",
                           out_hdr_file="hist.hxx"> {
      Mesh_Unif+Dim3+Space3+Node_Data+Scalar+IPort2 &volume;
      int data_component;
      int threshold;

      int+OPort2 histogram[256];

      cxxmethod ComputeHistogram(volume+read+notify,
                                 data_component+read+notify,
                                 threshold+read+notify,
                                 histogram+write);
   };
    
   flibrary+sort Utility_macros<libfile="v/utility_macros/Merl_util_lib.v">;
};
