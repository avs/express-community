
flibrary ThinPlateSplineMods <process="express",
                              cxx_src_files="solve.cxx tpspline.cxx",
                              cxx_hdr_files="solve.hxx tpspline.hxx",
                              out_src_file="gen.cxx",
                              out_hdr_file="gen.hxx",
                              build_dir="xp_comm_proj/tpspline"> {

   group+OPort TPS_Params {
      float+Port2 smooth;
      int+Port2   dims[2];
      int+Port2   add_points;
   };



   module TPS_unif_core<src_file="tps_unif.cxx"> {
      cxxmethod+notify_inst+req update(
         coord+read+notify+req,
         values+read+notify+req,
         smooth+read+notify+req,
	      dims+read+notify+req,
	      out+write,
   	   points+write
      );

      float+IPort2 coord[];
      float+IPort2 values[];

      TPS_Params+IPort2 &TPS_Params;

      float smooth => <-.TPS_Params.smooth;
      int  dims[2] => <-.TPS_Params.dims;

      float+OPort2 points[4];
      float+OPort2 out[prod(dims)];
   };


   module TPS_rect_core<src_file="tps_rect.cxx"> {
      cxxmethod+notify_inst+req update(
         coord+read+notify+req,
         values+read+notify+req,
         smooth+read+notify+req,
         add_points+read+notify+req,
         dims+write,
         points+write,
         out+write
      );

      float+IPort2 coord[];
      float+IPort2 values[];

      TPS_Params+IPort2 &TPS_Params;

      float smooth => <-.TPS_Params.smooth;
      int   add_points => <-.TPS_Params.add_points;

      int+OPort2   dims[2];
      float+OPort2 points[sum(dims)][2];
      float+OPort2 out[prod(dims)];
   };

};

