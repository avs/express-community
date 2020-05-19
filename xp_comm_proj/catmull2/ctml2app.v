
flibrary Catmull_2Apps {

   APPS.SingleWindowApp CatmullSpline_2_3DEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.catmull_spline_2.out_obj,
                  <-.<-.<-.point_mesh.obj,
                  <-.<-.<-.polyline_mesh.obj
               };
            };
         };
      };

      float+OPort nodes[8][3] = {
         -0.2, 3., -2.,
           1.,0.5,-2.5,
           2., 1.,-3.5,
           2.,2.2, 1.5,
           3., 3., 2.5,
          2.5, 4.,  1.,
          -1.,4.5, 0.0,
          -2., 3.,-1.5
      };

      //Generates the spline curve
      XP_COMM_PROJ.Catmull_2.Catmull_2Macs.catmull_spline_2 catmull_spline_2 {
         in => <-.nodes;
      };

      //Generates dots to show the position of the nodes
      FLD_MAP.point_mesh point_mesh {
         coord => <-.nodes;
         DataObject {
            Props {
               line_width = 5;
               inherit = 0;
            };
         };
      };


      //Generates straight lines to show the difference with respect to the spline
      FLD_MAP.polyline_mesh polyline_mesh {
         coord => <-.nodes;
         float dims[] => array_dims(coord);
         connect => {0, (dims[1] - 1)};

         DataObject {
            Props {
               col = {1.,0.,0.};
               line_width = 2;
               inherit = 0;
            };
         };
      };

   };



   CatmullSpline_2_3DEg CatmullSpline_2_2DEg {
      -Uviewer3D;

      GDM.Uviewer2D Uviewer2D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.catmull_spline_2.out_obj,
                  <-.<-.<-.point_mesh.obj,
                  <-.<-.<-.polyline_mesh.obj
               };
            };
         };
      };

      float+OPort nodes[8][2] = {
         -0.2, 3.,
           1.,0.5,
           2., 1.,
           2.,2.2,
           3., 3.,
          2.5, 4.,
          -1.,4.5,
          -2., 3.
      };

   };


   APPS.SingleWindowApp PathSmoothing_2_Eg {

      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {surf_plot.out_obj, MoviePlayer.out_obj};
            };
            Camera {
               Camera {
                  depth_front = 20.0;
                  auto_norm = "None";
                  perspec = 1;
                  front = 1.;
                  fov = 70.;
               };
            };
         };
      };


      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/hydrogen.fld";
         };
      };

      MODS.orthoslice orthoslice {
         in_field => <-.Read_Field.field;
         OrthoSliceParam {
            axis = 2;
            plane => 40;
         };
      };

      MODS.surf_plot surf_plot {
         in_field => <-.orthoslice.out_fld;
         SurfPlotParam {
            nspace => 2;
            scale = 0.05;
         };
      };


      macro MoviePlayer {
         ilink Camera => <-.Uviewer3D.Scene.Camera.Camera;
         ilink TopObj => <-.Uviewer3D.Scene.Top.Top;
         ilink PickObj => <-.surf_plot.out_obj;

         group+Port Controls {
            int+Port2   Reset = 0;
            int+Port2   EnableCollect = 0;
            int+Port2   Play = 1;
            int+Port2   KeyVisible = 1;
            int+Port2   SplineVisible = 1;
            int+Port2   MorePoints = 4;
            float+Port2 Smooth = 1.;
            int+Port2   Closed = 1;
         };


         macro Collect_Points {
            GDM.GDpick_process GDpick_process {
               obj_in => <-.InObj;
               use_pick_data = 1;
            };

            ilink InObj => <-.PickObj;
            link+Port in_xform => InObj.xform;

            float+Port Point[3] => .GDpick_process.point;
            int+Port Size = 27;

            float+Port XYZ[.Size][3] = {
               54.,3.,0., 47.,12.,0., 38.,28.,0., 37.,36.,1.12, 43.,48.,0.28,
               47.,57.,0.14, 40.,61.,1.54, 32.,61.,2.94, 24.,59.,2.24, 16.,54.,0.,
               18.,45.,0.7, 23.,41.,0., 25.,35.,0., 24.,27.,0.28, 22.,24.,0.98,
               20.,16.,0., 20.,11.,0.84, 22.,6.,1.82, 31.,5.,4.76, 39.,4.,2.24, 
               45.,3.,0.42, 51.,2.,0., 57.,2.,0., 60.,6.,0.14, 60.,14.,1.68, 
               57.,10.,0.56, 54.,4.,0.
            };

            GMOD.parse_v parse_reset {
               v_commands = "Size = 0;";
               trigger => <-.<-.Controls.Reset;
               on_inst = 0;
               relative => <-;
            };
            GMOD.parse_v parse_add {
               v_commands = "Size = parse_add.Size2; \n\t\t\t\tparse_add.XOut=parse_add.XPick;\n\t\t\t\tparse_add.YOut=parse_add.YPick;\n\t\t\t\tparse_add.ZOut=parse_add.ZPick;";
               trigger => <-.GDpick_process.vert_index;
               on_inst = 0;
               relative => <-;
               int Size2<NEportLevels=1> => (<-.Size + 1);
               float XPick<NEportLevels=1> => Point[0];
               float YPick<NEportLevels=1> => Point[1];
               float ZPick<NEportLevels=1> => Point[2];
               float XOut<NEportLevels=1> => XYZ[(<-.Size - 1)][0];
               float YOut<NEportLevels=1> => XYZ[(<-.Size - 1)][1];
               float ZOut<NEportLevels=1> => XYZ[(<-.Size - 1)][2];
               active => <-.<-.Controls.EnableCollect;
            };

            group app_point_mesh {
               float+IPort2 &coord[][] => <-.XYZ;
               int coord_dims[] => array_dims(.coord);
               int error => ((array_size(.coord) > 0) && (array_size(.coord_dims) != 2));
               Mesh+OPort2 out {
                  nnodes => switch((!<-.error),<-.coord_dims[1]);
                  nspace => switch((!<-.error),<-.coord_dims[0]);
                  coordinates {
                     values => switch((!<-.<-.error),<-.<-.coord);
                  };
                  ncell_sets = 1;
                  Point cell_set {
                     ncells => <-.nnodes;
                     node_connect_list => switch((!<-.<-.error),init_array(.ncells,0,(.ncells - 1)));
                  };
                  &xform => <-.<-.in_xform;
               };
            };

            DVxform_coord DVxform_coord {
               in => <-.app_point_mesh.out;
               comp = {0,1,2};
               coord_nspace = 3;
            };

            FLD_MAP.point_mesh Key_point_mesh {
               coord => <-.DVxform_coord.coord;
               DataObject {
                  Obj {
                     visible => <-.<-.<-.<-.Controls.KeyVisible;
                     pickable = 0;
                  };
                  Props {
                     inherit = 0;
                     line_width = 6;
                     col = {1.,0.,0.};
                  };
                  Modes {
                     mode = {2,0,0,0,0};
                  };
               };
            };

            olink XYZOut => DVxform_coord.coord;
            olink KeyPointObj => Key_point_mesh.obj;
         };


         macro LoopCamera {
            ilink XYZ => <-.CatmullSpline_2Func.out_data;
            int+Port Dims[] => array_dims(XYZ);
            int+Port Size   => Dims[1];

            int+Port GoLoop = 0;
            int+Port ResetTop = 1;
            int+Port II = 134;

            GMOD.loop loop {
               reset => <-.<-.Controls.Reset;
               run => <-.GoLoop;
               done = 1;
               start_val = 0.;
               end_val => (<-.Size - 2);
               incr = 1.;
               count => <-.II;
            };

            float From<NEportLevels={2,1}>[3] => Camera.from;
            float To<NEportLevels={2,1}>[3]   => Camera.at;
            float Up<NEportLevels={2,1}>[3]   => Camera.up;

            // Offset from exact path.  Adjust so that camera does not pass through surface.
            float+Port Delta[3]       = {0., 0., 1.5};

            float+Port CurrentUp[3]   = {0., 0., 1.};
            float+Port CurrentFrom[3] => (.CurrentXYZ + .Delta);
            float+Port CurrentTo[3]   => (.NextXYZ + .Delta);

            float+Port CurrentXYZ[3] => XYZ[.II];
            float+Port NextXYZ[3] => XYZ[(.II + 1)];

            GMOD.parse_v parse_copy_vectors {
               v_commands = "From=CurrentFrom;To=CurrentTo;Up=CurrentUp;";
               trigger => <-.II;
               active => <-.GoLoop;
               on_inst = 0;
               relative => <-;
            };
            GDM.GDxform_edit GDxform_edit {
               shell_vis = 0;
               xform => TopObj.xform;
               reset => ResetTop;
            };
            GMOD.parse_v parse_play {
               v_commands = "ResetTop=1;GoLoop=1;";
               trigger => <-.<-.Controls.Play;
               active => <-.<-.Controls.Play;
               on_inst = 0;
               relative => <-;
            };
         };

         macro UI {
            UImod_panel Rec_Play {
               parent<NEportLevels={4,0}>;
               title = "Record/Play Path";
            };

            UIbutton Reset_All {
               parent => <-.Rec_Play;
               do => Controls.Reset;
               y = 0;
            };
            UItoggle Collect_Points {
               parent => <-.Rec_Play;
               set => Controls.EnableCollect;
               y => <-.Reset_All.y + <-.Reset_All.height + 2;
			   width => parent.clientWidth;
            };

            UIbutton Play_Frames {
               parent => <-.Rec_Play;
               do => Controls.Play;
               y => <-.Collect_Points.y + <-.Collect_Points.height + 20;
            };
            UItoggle Show_Spline {
               parent => <-.Rec_Play;
               set => Controls.SplineVisible;
               y => <-.Play_Frames.y + <-.Play_Frames.height + 10;
			   width => parent.clientWidth;
            };
            UItoggle Show_KeyFrames {
               parent => <-.Rec_Play;
               set => Controls.KeyVisible;
               y => <-.Show_Spline.y + <-.Show_Spline.height + 2;
 			   width => parent.clientWidth;
           };

            UItoggle Closed_Spline {
               parent => <-.Rec_Play;
               set => Controls.Closed;
               y => <-.Show_KeyFrames.y + <-.Show_KeyFrames.height + 20;
			   width => parent.clientWidth;
            };
            VUIField More_Points {
               parent => <-.Rec_Play;
               value => Controls.MorePoints;
               label = "Points to Add";
               mode = 1;
               y => <-.Closed_Spline.y + <-.Closed_Spline.height + 5;
            };
            VUIField Smoothness {
               parent => <-.Rec_Play;
               value => Controls.Smooth;
               label = "Smoothness";
               mode = 0;
               y => <-.More_Points.y + <-.More_Points.height + 5;
            };
         };

         XP_COMM_PROJ.Catmull_2.Catmull_2Mods.Catmull_2Params Catmull_2Params {
            line_type => switch((Controls.Closed + 1),"open","closed");
            spline_type = "CatmullRom";
            smoothness => Controls.Smooth;
            points_added => Controls.MorePoints;
            DistanceEps = 0.01;
         };

         XP_COMM_PROJ.Catmull_2.Catmull_2Macs.CatmullSpline_2Func CatmullSpline_2Func {
            in => switch( ((<-.Collect_Points.Size > 2) + 1), , <-.Collect_Points.XYZOut );
            Catmull_2Params => <-.Catmull_2Params;
            polyline_mesh {
               obj {
                  visible => Controls.SplineVisible;
                  pickable = 0;
               };
            };
         };

         GroupObject GroupObject {
            child_objs => {
               <-.CatmullSpline_2Func.out_obj,
               <-.Collect_Points.KeyPointObj
            };
         };

         olink out_obj => GroupObject.obj;
      };

   };


   APPS.SingleWindowApp CatmullSplineFld_2_Eg {
      UI {
        Modules {
          IUI {
            optionList {
               cmdList => {
                 <-.<-.<-.<-.catmull_spline_fld_2.CatmullFldUI.panel.option};
               selectedItem = 0;
            };
          };
        };
      };
      GDM.Uviewer3D Uviewer3D {
        Scene {
          View {
            View {
               trigger = 1;
            };
          };
          Top {
            child_objs => {
               <-.<-.<-.catmull_spline_fld_2.out_obj,<-.<-.<-.combine_mesh_data.obj};
            Xform {
               ocenter = {5.60449, 2.5, 5.5};
               dcenter = {3.7597, 1.56114, 3.69398};
               mat = {
                 -0.159999,   0.260873, -0.420022,  0.,
                 -0.486548,  -0.161625,  0.0849572, 0.,
                 -0.0879838,  0.419399,  0.293999,  0.,
                  0.,0.,0.,1.
               };
               xlate = {-3.7597, -1.56114, -3.69398};
               center = {5.60449, 2.5, 5.5};
            };
            Top {
               active = 0;
            };
          };
        };
      };
      FLD_MAP.node_scalar node_scalar {
        in_data => <-.data;
      };
      FLD_MAP.combine_mesh_data combine_mesh_data {
        in_mesh => <-.polyline_mesh.out;
        in_nd => <-.node_scalar.out;
      };
      int Connectivity[] => {0,4};
      float points[5][3] => {
        {1,0,1},
        {0,2,3},
        {1,2,3},
        {4,5,6},
        {12,2,10}
      };
      float data[] => {
        3.,1.,2.,4.,6.
      };
      XP_COMM_PROJ.Catmull_2.Catmull_2Macs.catmull_spline_fld_2 catmull_spline_fld_2 {
        in_fld => <-.combine_mesh_data.out;
        Catmull_2Params {
          smoothness = 5.;
          points_added = 15;
          DistanceEps = 0.30882;
        };
      };
      FLD_MAP.polyline_mesh polyline_mesh {
        coord => <-.points;
        connect => <-.Connectivity;
      };
   };

};

