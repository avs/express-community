
flibrary FiniteWingMods <build_dir="xp_comm_proj/f-wing",
                         out_src_file="gen.cxx",
                         out_hdr_file="gen.hxx"> {


   group+OPort FiniteWingInputParams {
      double+Port2 Angle_of_Attack;
      double+Port2 Wing_span;
      int+Port2    Grid_size;
      int+Port2    scan_probe;
   };

   group+Port FiniteWingOutputParams {
      float+Port2 probe_x;
      float+Port2 probe_y;
      float+Port2 probe_z;
      float+Port2 probe_scale;
      float+Port2 wing_x;
      float+Port2 wing_y;
      float+Port2 wing_z;
      float+Port2 wing_scale;
   };


   module FiniteWingCore<src_file="f-wing.cxx">
   {
      FiniteWingInputParams+IPort2 &in_params;

      double Angle_of_Attack => in_params.Angle_of_Attack;
      double Wing_span => in_params.Wing_span;
      int    Grid_size => in_params.Grid_size;

      cxxmethod+notify_inst+req update(
         Angle_of_Attack+read+notify+req,
         Wing_span+read+notify+req,
         Grid_size+read+notify+req+write, 
         Output_x+write, Output_y+write, Output_z+write,
         probe_x+write, probe_y+write, probe_z+write,
         wing_x+write, wing_y+write, wing_z+write,
         wing_scale+write
      );

      double+OPort2 Output_x[Grid_size * Grid_size * Grid_size];
      double+OPort2 Output_y[Grid_size * Grid_size * Grid_size];
      double+OPort2 Output_z[Grid_size * Grid_size * Grid_size];

      float probe_x;
      float probe_y;
      float probe_z;
      float probe_scale;
      float wing_x;
      float wing_y;
      float wing_z;
      float wing_scale;

      FiniteWingOutputParams+OPort2 &out_params {
         probe_x => <-.probe_x;
         probe_y => <-.probe_y;
         probe_z => <-.probe_z;
         probe_scale => <-.probe_scale;
         wing_x => <-.wing_x;
         wing_y => <-.wing_y;
         wing_z => <-.wing_z;
         wing_scale => <-.wing_scale;
      };
   };



   macro HorizontalLine3D {
   	int+IPort2   dim1;
   	float+IPort2 line_x;
   	float+IPort2 line_y;
   	float+IPort2 line_z;
   	float+IPort2 line_scale;

   	Line_Mesh+OPort line3 {
   		dims => {<-.dim1};
   		nspace = 3;
   		points = {{0,0,0}, {1,0,0}};
   		xform {
            mat => {
               0., (<-.<-.line_scale), 0., 0.,
               -1., 0., 0., 0.,
               0., 0., 1., 0.,
               0., 0., 0., 1.
            };
            xlate => {<-.<-.line_x, <-.<-.line_y, <-.<-.line_z};
   		};
   	};

   	DataObject obj {
         in => line3;
   		Obj {
   			name => name_of(<-.<-.<-);
   		};
   	};

   	olink out_fld => line3;
   	olink out_obj => obj.obj;
   };


};

