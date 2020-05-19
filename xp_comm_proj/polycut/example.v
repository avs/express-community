APPS.SingleWindowApp SingleWindowApp<NEdisplayMode="maximized"> {
   UI {
      shell {
         x = 88;
         y = 88;
      };
      Modules {
         IUI {
            optionList {
               cmdList => {
                  <-.<-.<-.<-.Read_Field.read_field_ui.panel.option,
                  <-.<-.<-.<-.isosurface.UIpanel.option,
                  <-.<-.<-.<-.cut_plane.UIpanel.option,
                  <-.<-.<-.<-.Cut_Polyline.cut.UIpanel.option,
                  <-.<-.<-.<-.bounds.UIpanel.option
               };
               selectedItem = 2;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=482,NEy=240> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.cut_plane.out_obj_cut,
               <-.<-.<-.Cut_Polyline.cut_polyline.obj};
            AltObject<instanced=0>;
            Xform {
               ocenter = {59.5,59.5,23.0028};
               dcenter = {3.99628,4.01257,
1.73037};
               mat = {
                  -0.0488468,0.00884551,0.0198862,0.,0.0160294,-0.0184313,0.0475716,
0.,0.0147229,0.0494142,0.0141842,0.,0.,0.,0.,1.
               };
               xlate = {-4.0036,-4.03714,
-1.73743};
            };
         };
         View {
            ViewUI {
               FullViewWindow<instanced=0>;
            };
         };
      };
      Scene_Editor {
         View_Editor {
            InfoPanel {
               curobj_sel {
                  width = 238;
                  height = 245;
                  x = 0;
                  y = 0;
               };
            };
         };
         Print_Editor {
            IUI<instanced=0>;
         };
      };
   };
   MODS.Read_Field Read_Field<NEx=77.,NEy=33.> {
      read_field_ui {
         file_browser {
            ok = 1;
            cancel = 1;
         };
         filename = "C:\\Express\\data\\field\\lobster.fld";
      };
      DVread_field {
         Mesh_Unif+Node_Data Output_Field;
      };
      DataObject {
         AltObject<instanced=0>;
      };
   };
   MODS.isosurface isosurface<NEx=77.,NEy=88.> {
      in_field => <-.Read_Field.field;
      DVcell_data_labels {
         labels[];
      };
      DVnode_data_labels {
         labels[];
      };
      obj {
         AltObject<instanced=0>;
      };
   };
   MODS.cut_plane cut_plane<NEx=165.,NEy=132.> {
      in_field => <-.isosurface.out_fld;
      CutParam {
         dist = -3.24;
      };
      DVcell_data_labels {
         labels[];
      };
      DVnode_data_labels {
         labels[];
      };
      UIpanel {
         option {
            set = 1;
         };
      };
      CutUI {
         plane_dist_typein {
            valEditor {
               UI<instanced=0>;
            };
         };
         UIoptionBoxLabel {
            label_cmd {
               cmd[];
            };
         };
         UIoptionBoxLabel_cell {
            label_cmd {
               cmd[];
            };
         };
         plane_xform {
            XformEditorUI<instanced=0>;
         };
      };
      cut_obj {
         AltObject<instanced=0>;
      };
      plane_obj {
         AltObject<instanced=0>;
      };
   };
   XP_COMM_PROJ.Cut_Polylines.PolyCut_Macs.Cut_Polyline Cut_Polyline<NEx=396.,NEy=143.> {
      in_level => <-.cut_plane.Cut.DVcut.level;
      in_plane => <-.cut_plane.out_plane;
      in_polyline => <-.bounds.out_fld;
      cut {
         DVcell_data_labels {
            labels[];
         };
         DVnode_data_labels {
            labels[];
         };
         CutUI {
            plane_dist_typein {
               valEditor {
                  UI<instanced=0>;
               };
            };
            UIoptionBoxLabel {
               label_cmd {
                  cmd[];
               };
            };
            UIoptionBoxLabel_cell {
               label_cmd {
                  cmd[];
               };
            };
         };
         obj {
            AltObject<instanced=0>;
         };
      };
      cut_polyline {
         AltObject<instanced=0>;
      };
   };
   MODS.bounds bounds<NEx=352.,NEy=77.> {
      in_field => <-.Read_Field.field;
      BoundsUI {
         DVnode_data_labels {
            labels[];
         };
         UIradioBoxLabel {
            label_cmd {
               cmd[];
            };
         };
      };
      obj {
         AltObject<instanced=0>;
      };
   };
};
