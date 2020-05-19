
flibrary ThinPlateSplineMacs<compile_subs=0> {

   macro TPS_unif {

      mlink coord<NEportLevels={2,1}>;
      mlink values<NEportLevels={2,1}>;

      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMods.TPS_Params &TPS_Params<NEportLevels={2,1}>;

      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMods.TPS_unif_core TPS_unif_core {
         coord => <-.coord;
         values => <-.values;
         TPS_Params => <-.TPS_Params;
      };

      FLD_MAP.uniform_scalar_field uniform_scalar_field {
         mesh {
            in_dims => <-.<-.TPS_Params.dims;
            out {
               points<NEportLevels={4,0}> => <-.<-.<-.TPS_unif_core.points;
            };
         };
         data {
            in_data => <-.<-.TPS_unif_core.out;
         };
      };

      olink out_fld => .uniform_scalar_field.out;
      olink out_obj => .uniform_scalar_field.obj;
   };


   macro TPS_rect {

      mlink coord<NEportLevels={2,1}>;
      mlink values<NEportLevels={2,1}>;

      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMods.TPS_Params &TPS_Params<NEportLevels={2,1}>;

      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMods.TPS_rect_core TPS_rect_core {
         coord => <-.coord;
         values => <-.values;
         TPS_Params => <-.TPS_Params;
      };

      FLD_MAP.rect_scalar_field rect_scalar_field {
         mesh {
            in_dims => <-.<-.TPS_rect_core.dims;
            in_points => <-.<-.TPS_rect_core.points;
         };
         data {
            in_data => <-.<-.TPS_rect_core.out;
         };
      };

      olink out_fld => .rect_scalar_field.out;
      olink out_obj => .rect_scalar_field.obj;
   };


   macro TPS_baseUI {
      UImod_panel UImod_panel {
         title => "Thin Plate Spline";
         parent<NEportLevels={4,0}>;
      };

      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMods.TPS_Params &TPS_Params<NEportLevels={2,1}>;

      UIslider smooth_slider {
         parent => <-.UImod_panel;
         value => <-.TPS_Params.smooth;
         title => "Smoothing";

         min = 0.;
         max = 5.;
         mode = "real";
         decimalPoints = 2;
         horizontal = 1;
         increment = 1;
      };
   };


   TPS_baseUI TPS_unifUI {

      UImod_panel {
         title => "Uniform Thin Plate Spline";
      };

      UIslider XDimUIslider {
         parent => <-.UImod_panel;
         value => <-.TPS_Params.dims[0];
         title => "X Dimension of Surface";

         min = 2.;
         max = 200.;
         mode = "integer";
         decimalPoints = 0;
         horizontal = 1;
         increment = 1.;
      };
      UIslider YDimUIslider {
         parent => <-.UImod_panel;
         value => <-.TPS_Params.dims[1];
         title => "Y Dimension of Surface";

         min = 2.;
         max = 200.;
         mode = "integer";
         decimalPoints = 0;
         horizontal = 1;
         increment = 1.;
      };

   };


   TPS_baseUI TPS_rectUI {

      UImod_panel {
         title => "Rectilinear Thin Plate Spline";
      };

      UIslider add_slider {
         parent => <-.UImod_panel;
         value => <-.TPS_Params.add_points;
         title => "Points to add between nodes";

         min = 0.;
         max = 20.;
         mode = "integer";
         decimalPoints = 0;
         horizontal = 1;
         increment = 1.;
      };

   };


   macro tps_unif {
      mlink coord<NEportLevels={2,1}>;
      mlink values<NEportLevels={2,1}>;

      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMods.TPS_Params TPS_Params {
         smooth = 0.;
         dims = {11,11};
      };

      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMacs.TPS_unif TPS_unif {
         coord => <-.coord;
         values => <-.values;
         TPS_Params => <-.TPS_Params;
      };

      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMacs.TPS_unifUI TPS_unifUI {
         TPS_Params => <-.TPS_Params;
      };

      olink out_fld => .TPS_unif.out_fld;
      olink out_obj => .TPS_unif.out_obj;
   };

   macro tps_rect {
      mlink coord<NEportLevels={2,1}>;
      mlink values<NEportLevels={2,1}>;

      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMods.TPS_Params TPS_Params {
         smooth = 0.;
         add_points = 1;
      };

      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMacs.TPS_rect TPS_rect {
         coord => <-.coord;
         values => <-.values;
         TPS_Params => <-.TPS_Params;
      };

      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMacs.TPS_rectUI TPS_rectUI {
         TPS_Params => <-.TPS_Params;
      };

      olink out_fld => .TPS_rect.out_fld;
      olink out_obj => .TPS_rect.out_obj;
   };



   APPS.SingleWindowApp TPS_base_Eg {

      GDM.Uviewer3D Uviewer3D<NEx=275.,NEy=407.> {
         Scene {
            Top {
               child_objs => {<-.<-.<-.glyph.out_obj,<-.<-.<-.TPS_surf_plot.out_obj,<-.<-.<-.Axis3D.out_obj};
            };
            Lights {
               Lights = {{type="BiDirectional"},,,};
            };
         };
      };


      float coord<NEportLevels={0,1}>[5][2] => {
         0.,0.,3.,1.3,2.1,2.7,3.2,4.,1.4,3.2
      };
      float values<NEportLevels={0,1},export=1>[5] => {
         1.,4.,2.,5.,2.
      };



      FLD_MAP.point_mesh point_mesh {
         coord => <-.coord;
      };
      FLD_MAP.node_scalar node_scalar<NEx=264.,NEy=99.> {
         in_data => <-.values;
      };
      FLD_MAP.combine_mesh_data combine_mesh_data<NEx=33.,NEy=198.> {
         in_mesh => <-.point_mesh.out;
         in_nd => <-.node_scalar.out;
      };

      GEOMS.Cross3D Cross3D<NEx=209.,NEy=242.>;
      MODS.glyph glyph<NEx=33.,NEy=297.> {
         in_field => <-.Glyph_surf_plot.out_fld;
         in_glyph => <-.Cross3D.out_fld;
         GlyphParam {
            scale = 0.8;
            normalize = 1;
         };
      };

      MODS.surf_plot Glyph_surf_plot<NEx=33.,NEy=242.> {
         in_field => <-.combine_mesh_data.out;
      };

      GEOMS.Axis3D Axis3D<NEx=33.,NEy=352.> {
         in_field => <-.glyph.out_fld;
      };

      MODS.surf_plot TPS_surf_plot<NEx=396.,NEy=242.> {
         obj {
            Modes {
               mode = {0,2,0,0,0};
            };
         };
      };

   };


   TPS_base_Eg TPS_uniform_Eg {
      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMacs.tps_unif tps_unif<NEx=473.,NEy=132.> {
         coord => <-.coord;
         values => <-.values;
      };
      TPS_surf_plot {
         in_field => <-.tps_unif.out_fld;
      };
   };

   TPS_base_Eg TPS_rectilinear_Eg {
      XP_COMM_PROJ.ThinPlateSpline.ThinPlateSplineMacs.tps_rect tps_rect<NEx=473.,NEy=132.> {
         coord => <-.coord;
         values => <-.values;
      };
      TPS_surf_plot {
         in_field => <-.tps_rect.out_fld;
      };
   };

};

