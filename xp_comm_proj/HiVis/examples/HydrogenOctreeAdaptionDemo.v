APPS.SingleWindowApp HydrogenOctreeAdaptionDemoApp {
   UI {
      shell {
         x = 22;
         y = 22;
      };
      Modules {
         IUI {
            optionList {
               cmdList => {
                  <-.<-.<-.<-.Read_Field.read_field_ui.panel.option,
                  <-.<-.<-.<-.Field_To_HiVis.Field_To_HiVis.option,
                  <-.<-.<-.<-.downsize.panel.option,
                  <-.<-.<-.<-.AG_Octree.AG_Octree.option,
                  <-.<-.<-.<-.HiVis_Render_Cells.HiVis_Render_Cells.option
               };
               selectedItem = 4;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=518,NEy=320> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.HiVis_Render_Cells.DataObject.obj};
            Xform {
               ocenter = {11.,10.5,10.5};
               dcenter = {3.47165,3.52837,
3.81854};
               mat = {
                  0.274786,0.0696616,-0.176518,0.,0.00289029,0.309058,0.126467,
0.,0.189746,-0.105592,0.253705,0.,0.,0.,0.,1.
               };
               xlate = {-3.47165,-3.52837,
-3.81854};
            };
         };
         Camera {
            Camera {
               perspec = 1;
               front = 1.;
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
         Camera_Editor {
            GDcamera_edit {
               front = 1.;
            };
         };
      };
   };
   MODS.Read_Field Read_Field<NEx=77.,NEy=22.> {
      read_field_ui {
         file_browser {
            ok = 1;
         };
         filename = "hydrogen.fld";
      };
      DVread_field {
         Mesh_Unif+Node_Data Output_Field;
      };
   };
   XP_COMM_PROJ.HiVis.HiVisLib.Data_IO.Field_To_HiVis Field_To_HiVis<NEx=77.,NEy=121.> {
      Field_To_HiVis {
         option {
            set = 0;
         };
      };
      Field_To_HiVis_Mod<NEdisplayMode="open"> {
         in => <-.<-.downsize.out_fld;
         out<NEdisplayMode="open"> {
            size<NEdisplayMode="open">;
         };
      };
      Inherit {
         set = 0;
      };
      Cell {
         set = 1;
      };
      Attribute_Association {
         selectedItem = 2;
      };
   };
   MODS.downsize downsize<NEx=77.,NEy=77.> {
      in_field => <-.Read_Field.field;
      DownsizeParam {
         factor0 = 3.;
         factor1 = 3.;
         factor2 = 3.;
      };
   };
   XP_COMM_PROJ.HiVis.HiVisLib.Filters.AG_Octree AG_Octree<NEx=77.,NEy=176.> {
      in => <-.Field_To_HiVis.Field_To_HiVis_Mod.out;
      Max_tree_level {
         value = 5.;
      };
      HiVis_Attr_Comb_Epsilon {
         HiVis_Attr_Comb_Epsilon {
            y = 32;
         };
         UIradioBoxLabel {
            label_cmd {
               cmd[1];
            };
         };
         UIdial {
            value = 3.576389;
         };
      };
   };
   XP_COMM_PROJ.HiVis.HiVisLib.Mappers.HiVis_Render_Cells HiVis_Render_Cells<NEx=77.,NEy=231.> {
      HiVis_Render_Cells_Mod {
         in => <-.<-.AG_Octree.AG_Octree_Mod.out;
      };
      Color_Attribute {
         label_cmd {
            cmd[];
         };
      };
      Range_Attribute {
         label_cmd {
            cmd[];
         };
      };
      Cell_Render_Type {
         selectedItem = 1;
      };
      Edge {
         set = 0;
      };
      Face {
         set = 1;
      };
      Ranges {
         value = 1.;
      };
   };
};
