APPS.SingleWindowApp OctreeDemoApp<NEdisplayMode="maximized"> {
   UI {
      shell {
         x = 154;
         y = 154;
      };
      Modules {
         IUI {
            optionList {
               cmdList => {
                  <-.<-.<-.<-.HiVis_Read_Octree.UImod_panel.option,
                  <-.<-.<-.<-.HiVis_Render_Cells.HiVis_Render_Cells.option};
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
               ocenter = {32768.,32768.,32768.};
               dcenter = {4.,4.,4.};
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
   XP_COMM_PROJ.HiVis.HiVisLib.Data_IO.HiVis_Read_Octree HiVis_Read_Octree<NEx=121.,NEy=66.> {
      File_Dialog {
         filename = "xp_comm_proj/HiVis/data/tank20.oc";
      };
   };
   XP_COMM_PROJ.HiVis.HiVisLib.Mappers.HiVis_Render_Cells HiVis_Render_Cells<NEx=253.,NEy=176.> {
      HiVis_Render_Cells_Mod {
         in => <-.<-.HiVis_Read_Octree.HiVis_Read_Octree.out;
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
