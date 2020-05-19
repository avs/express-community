APPS.SingleWindowApp Image2OctreeDemoApp<NEdisplayMode="maximized"> {
   UI {
      shell {
         x = 0;
         y = 0;
      };
      Modules {
         IUI {
            optionList {
               cmdList => {
                  <-.<-.<-.<-.Read_Image.read_image_ui.panel.option,
                  <-.<-.<-.<-.Field_To_HiVis.Field_To_HiVis.option,
                  <-.<-.<-.<-.AG_Octree.AG_Octree.option,
                  <-.<-.<-.<-.HiVis_To_Field.UImod_panel.option,
                  <-.<-.<-.<-.AG_Cell2Node.HiVis_Grid_Interp.option,
                  <-.<-.<-.<-.HiVis_Render_Cells.HiVis_Render_Cells.option
               };
               selectedItem = 2;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D<NEx=518,NEy=320> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.HiVis_To_Field.DataObject.obj,
               <-.<-.<-.HiVis_Render_Cells.DataObject.obj};
            Xform {
               ocenter = {256.,192.,0.};
               dcenter = {3.99999,2.93091,
0.0295308};
               mat = {
                  0.0155967,-3.73565e-005,-0.0001642,0.,3.77421e-005,0.0155975,
3.65772e-005,0.,0.000164111,-3.69733e-005,0.0155967,0.,0.,0.,0.,1.
               };
               xlate = {-3.99999,-2.93091,
-0.0295308};
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
      };
   };
   MODS.Read_Image Read_Image<NEx=77.,NEy=77.> {
      read_image_ui {
         file_browser {
            filename = "xp_comm_proj/HiVis/data/TUM.jpg";
            ok = 1;
         };
         format_from {
            set = 1;
         };
      };
   };
   XP_COMM_PROJ.HiVis.HiVisLib.Data_IO.Field_To_HiVis Field_To_HiVis<NEx=154.,NEy=154.> {
      Field_To_HiVis {
         option {
            set = 0;
         };
      };
      Field_To_HiVis_Mod {
         in => <-.<-.Read_Image.field;
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
   XP_COMM_PROJ.HiVis.HiVisLib.Filters.AG_Octree AG_Octree<NEx=242.,NEy=231.> {
      AG_Octree {
         option {
            set = 1;
         };
      };
      in => <-.Field_To_HiVis.Field_To_HiVis_Mod.out;
      Max_tree_level {
         value = 9.;
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
            value = 27.109623;
         };
         Manual_Mode {
            set = 1;
         };
      };
   };
   XP_COMM_PROJ.HiVis.HiVisLib.Data_IO.HiVis_To_Field HiVis_To_Field<NEx=484.,NEy=165.> {
      UImod_panel {
         option {
            set = 0;
         };
      };
      HiVis_To_Field {
         in => <-.<-.AG_Cell2Node.AG_Cell2Node_Mod.out;
      };
      Adjust_Min_Max {
         y = 0;
      };
   };
   XP_COMM_PROJ.HiVis.HiVisLib.Filters.AG_Cell2Node AG_Cell2Node<NEx=319.,NEy=286.> {
      AG_Cell2Node_Mod {
         in => <-.<-.AG_Octree.AG_Octree_Mod.out;
      };
   };
   XP_COMM_PROJ.HiVis.HiVisLib.Mappers.HiVis_Render_Cells HiVis_Render_Cells<NEx=143.,NEy=330.> {
      HiVis_Render_Cells_Mod {
         in => <-.<-.AG_Cell2Node.AG_Cell2Node_Mod.out;
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
   };
};
