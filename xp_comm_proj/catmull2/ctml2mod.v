
flibrary Catmull_2Mods <build_dir="xp_comm_proj/catmull2",
                      out_hdr_file="gen.hxx",
                      out_src_file="gen.cxx">
{

   // define the parameter block group
   group+OPort Catmull_2Params {
      enum+Port2 line_type {
         choices => {"closed","open"};
      };
      enum+Port2 spline_type {
         choices => {"CatmullRom"};
      };
      float+Port2 smoothness;
      int+Port2 points_added;
      float+Port2 DistanceEps;
   };


   // array based low-level module
   module CatmullSpline_2Core <src_file="ctml_arr.cxx",
                             cxx_src_files="catmull.cxx",
                             cxx_hdr_files="catmull.hxx">
   {
      float+IPort2 &points[];
      int dims[] => array_dims(points);

      Catmull_2Params+IPort2 &Catmull_2Params;

      int line_type => Catmull_2Params.line_type;
      int spline_type => Catmull_2Params.spline_type;
      float smoothness => Catmull_2Params.smoothness;
      int points_added => Catmull_2Params.points_added;

      cxxmethod+req+notify_inst update (
         .points+read+notify+req,
         .dims+read+notify+req,
         .line_type+read+notify+req,
         .spline_type+read+notify+req,
         .smoothness+read+notify+req,
         .points_added+read+notify+req,
         .out_size+write,
         .out+write
      );

      int+OPort2 out_size;
      float+OPort2 out[out_size][dims[0]];
   };

   // field based low-level module
   module CatmullSplineFldCore <src_file="ctml_fld.cxx",
                                libdeps="FLD",
                                cxx_src_files="catmull.cxx",
                                cxx_hdr_files="catmull.hxx fld/Xfld.h",
                                cxx_members="private:
                                             void append_array(float*& dest_arr, int& dest_size, float* src_arr, int src_size, int nspace);
                                             int cmp_coords(float* pt1, float* pt2, int nspace, float epsilon);
                                             void gen_filter_mask(int*& mask_arr, float* src_arr, int src_size, int nspace, float epsilon);
                                             void apply_filter_mask(float*& dest_arr, int& dest_size, float* src_arr, int src_size, int* mask_arr, int nspace);
                                             void interpolate_node_data(float*& dest_arr, int& dest_size, float* src_arr, int src_size, int added_points, int veclen, int is_open);">
   {
      Mesh+Node_Data+IPort2 &in_fld {
      	ncell_sets = 1;
      	Polyline cell_set;
      };

      Catmull_2Params+IPort2 &Catmull_2Params;
      int line_type => Catmull_2Params.line_type;
      int spline_type => Catmull_2Params.spline_type;
      float smoothness => Catmull_2Params.smoothness;
      int points_added => Catmull_2Params.points_added;
      float DistanceEps => Catmull_2Params.DistanceEps;
	  
      cxxmethod+req+notify_inst update (
         in_fld+req+notify+read,
         spline_type+read+notify+req,
         smoothness+read+notify+req,
         points_added+read+notify+req,
         DistanceEps+read+notify+req,
         out+write
      );

      Mesh+Node_Data+OPort2 out {
      	&xform => in_fld.xform;
      	ncell_sets = 1;
      	Polyline cell_set;
      };
  };


};

