
flibrary TubeMacs <compile_subs=0> {

// Higher level tube macro, containing params block an I/O
macro Tube {
   ilink in;

   XP_COMM_PROJ.Tube.TubeMods.TubeParams &TubeParams;
   XP_COMM_PROJ.Tube.TubeMods.tube tube {
      in => <-.in;
      axis => <-.TubeParams.axis;
      xform => <-.TubeParams.do_transform;
      zscale => <-.TubeParams.z_scale;
      resolution => <-.TubeParams.resolution;
      normalise => <-.TubeParams.normalise;
      scale => <-.TubeParams.radius_scale;
      node_cell => <-.TubeParams.node_cell;
      node_data_comp => <-.TubeParams.node_data_comp;
      cell_data_comp => <-.TubeParams.cell_data_comp;
      col_data_comp => <-.TubeParams.col_data_comp;
   };

   olink out_fld => .tube.out;
};


// UI macro
macro TubeUI {
   UImod_panel UImod_panel<NEx=11.,NEy=11.> {
      title => name_of(<-.<-.<-);
   };
   XP_COMM_PROJ.Tube.TubeMods.TubeParams TubeParams<NEx=550.,NEy=55.,NEportLevels={0,2}> {
      axis = 2;
      do_transform = 1;
      z_scale = 1.;
      resolution = 3;
      normalise = 0;
      radius_scale = 1.;
      node_cell = 0;
      node_data_comp = 0;
      cell_data_comp = 0;
      col_data_comp = 0;
   };
   UIslider axisSlider<NEx=198.,NEy=11.> {
      parent => <-.UImod_panel;
      value = 2.;
      title => "2D projection axis";
      y = 8;
      width => w;
      min = 0.;
      max = 2.;
      mode = "integer";
      decimalPoints = 0;
      immediateMode = 0;
      value => TubeParams.axis;
   };
   UItoggle xformToggle<NEx=198.,NEy=44.> {
      parent => <-.UImod_panel;
      label => "Do xform on 2D datasets";
      y => ((<-.axisSlider.y + <-.axisSlider.height) + axisSlider.y);
      width => <-.w;
      set => TubeParams.do_transform;
   };
   int w<NEportLevels=1,NEx=11.,NEy=44.> => UImod_panel.clientWidth;
   UIlabel zsLabel<NEx=198.,NEy=77.> {
      parent => <-.UImod_panel;
      label => "Z Scale for 2D transform";
      y => ((xformToggle.y + xformToggle.height) + axisSlider.y);
      width = 120;
      alignment = "left";
   };
   UIfield zsField<NEx=341.,NEy=77.> {
      parent => <-.UImod_panel;
      value = 1.;
      y => (zsLabel.y - (height / 5));
      x => (zsLabel.x + zsLabel.width);
      width => (w - x);
      value => TubeParams.z_scale;
   };
   UIslider resSlider<NEx=198.,NEy=110.> {
      parent => <-.UImod_panel;
      value = 3.;
      title => "Number of facets on tubes";
      y => ((zsField.y + zsField.height) + axisSlider.y);
      min = 3.;
      max = 36.;
      mode = "integer";
      decimalPoints = 0;
      immediateMode = 0;
      value => TubeParams.resolution;
   };
   UItoggle normToggle<NEx=198.,NEy=143.> {
      parent => <-.UImod_panel;
      label => "Normalise tube radii to Radius Scale";
      y => ((resSlider.y + resSlider.height) + axisSlider.y);
      width => w;
      label => "Normalise tubes to Radius Scale";
      set => TubeParams.normalise;
   };
   UIlabel scaleLabel<NEx=198.,NEy=176.> {
      parent => <-.UImod_panel;
      label => "Radius Scale";
      y => ((normToggle.y + normToggle.height) + axisSlider.y);
      width = 120;
      alignment = "left";
   };
   UIfield scaleField<NEx=341.,NEy=176.> {
      parent => <-.UImod_panel;
      value = 1.;
      y => (scaleLabel.y - (height / 5));
      x => (scaleLabel.x + scaleLabel.width);
      width => (w - x);
      value => <-.TubeParams.radius_scale;
      decimalPoints = 6;
   };
   UItoggle ncToggle<NEx=198.,NEy=209.> {
      parent => <-.UImod_panel;
      label => "Select node data (off) or cell data (on)";
      y => ((scaleField.y + scaleField.height) + axisSlider.y);
      width => w;
      set => TubeParams.node_cell;
   };
   UIlabel ndLabel<NEx=198.,NEy=242.> {
      parent => <-.UImod_panel;
      label => "Node data component";
      y => ((ncToggle.y + ncToggle.height) + axisSlider.y);
      width = 120;
      active => (!ncToggle.set);
      alignment = "left";
   };
   UIlabel cdLabel<NEx=198.,NEy=275.> {
      parent => <-.UImod_panel;
      label => "Cell data component";
      y => ((ndField.y + ndField.height) + axisSlider.y);
      width = 120;
      active => (!ndLabel.active);
      alignment = "left";
   };
   UIfield ndField<NEx=341.,NEy=242.> {
      parent => <-.UImod_panel;
      value = 0.;
      y => (ndLabel.y - (height / 5));
      x => (ndLabel.x + ndLabel.width);
      width => (w - x);
      active => ndLabel.active;
      min = 0.;
      mode = "integer";
      decimalPoints = 0;
      value => <-.TubeParams.node_data_comp;
   };
   UIfield cdField<NEx=341.,NEy=275.> {
      parent => <-.UImod_panel;
      value = -1.;
      y => (cdLabel.y - (height / 5));
      x => (colLabel.x + colLabel.width);
      width => (w - x);
      active => cdLabel.active;
      min = 0.;
      mode = "integer";
      decimalPoints = 0;
      value => <-.TubeParams.cell_data_comp;
   };
   UIlabel colLabel<NEx=198.,NEy=308.> {
      parent => <-.UImod_panel;
      label => "Colour component";
      y => ((cdField.y + cdField.height) + axisSlider.y);
      width = 120;
      alignment = "left";
   };
   UIfield colField<NEx=341.,NEy=308.> {
      parent => <-.UImod_panel;
      value = 0.;
      y => (colLabel.y - (height / 5));
      x => (colLabel.x + colLabel.width);
      width => (w - x);
      min = 0.;
      mode = "integer";
      decimalPoints = 0;
      value => <-.TubeParams.col_data_comp;
   };
};


// user macro
macro tube {
   ilink in_fld;

   XP_COMM_PROJ.Tube.TubeMacs.TubeUI TubeUI;
   XP_COMM_PROJ.Tube.TubeMacs.Tube Tube {
      TubeParams => <-.TubeUI.TubeParams;
      in => <-.in_fld;
   };

   GDM.DataObject out_GD_obj {
      in => Tube.out_fld;
   };

   olink out_fld => Tube.out_fld;
   olink out_obj => out_GD_obj.obj;
};


// example app - normal usage
APPS.SingleWindowApp TubeEg1 {

   GDM.Uviewer3D Uviewer3D {
      Scene {
         Top {
            child_objs => {<-.<-.<-.bounds.out_obj,<-.<-.<-.tube.out_obj};
         };
      };
   };

   MODS.Read_Field Read_Field {
      read_field_ui {
         file_browser {
            ok = 1;
         };
         filename = "$XP_PATH<0>/data/field/hydrogen.fld";
      };
      DVread_field {
         Mesh_Unif+Node_Data Output_Field;
      };
   };

   MODS.bounds bounds {
      in_field => <-.Read_Field.field;
   };

   MODS.orthoslice orthoslice {
      in_field => <-.Read_Field.field;
      OrthoSliceParam {
         axis = 2;
      };
      OrthoSliceUI {
         UIpanel {
            option {
               set = 1;
            };
         };
      };
   };

   MODS.isoline isoline {
      in_field => <-.orthoslice.out_fld;
   };

   XP_COMM_PROJ.Tube.TubeMacs.tube tube {
      TubeUI {
         TubeParams {
            resolution = 4;
            normalise = 1;
            radius_scale = 0.2;
         };
      };
      in_fld => <-.isoline.out_fld;
   };

};


// example to show Z scaling for slice plane
APPS.SingleWindowApp TubeEg2 {
   GDM.Uviewer3D Uviewer3D {
      Scene {
         Top {
            child_objs => {<-.<-.<-.tube.out_obj,<-.<-.<-.bounds.out_obj};
         };
      };
   };

   MODS.Read_Field Read_Field {
      read_field_ui {
         file_browser {
            ok = 1;
         };
         filename = "$XP_PATH<0>/data/field/hydrogen.fld";
      };
      DVread_field {
         Mesh_Unif+Node_Data Output_Field;
      };
   };

   MODS.bounds bounds {
      in_field => <-.Read_Field.field;
   };

   MODS.slice_plane slice_plane {
      in_field => <-.Read_Field.field;
      plane {
         xform {
            xlate = {0.,0.,60.};
            mat = {0.5,0.,-0.866025,0.,0.224144,0.965926,0.12941,0.,0.836516,-0.258819,0.482963,0.,0.,0.,0.,1.};
         };
      };
      SliceUI {
         plane_xform {
            GDxform_editor {
               x_rot = 15.;
               y_rot = 60.;
               z_trans = 60.;
               abs_x_rot = 15.;
               abs_y_rot = 60.;
               abs_z_trans = 60.;
            };
            XformEditorUI {
               trans_shell {
                  ok = 1;
               };
            };
         };
      };
   };


   MODS.isoline isoline {
      in_field => <-.slice_plane.out_fld;
      IsoParam {
         level_min => 0.;
         level_max => 255.;
      };
   };

   XP_COMM_PROJ.Tube.TubeMacs.tube tube {
      TubeUI {
         TubeParams {
            z_scale = 64.;
            radius_scale = 0.0001;
         };
         UImod_panel {
            option {
               set = 1;
            };
         };
      };
      in_fld => <-.isoline.out_fld;
   };

};


};

