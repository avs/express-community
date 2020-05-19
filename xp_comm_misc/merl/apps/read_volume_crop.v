APPS.SingleWindowApp SingleWindowApp {

   MODS.Read_Volume Read_Volume<NEx=11.,NEy=77.> {
      read_volume_ui {
         filename = "$XP_PATH<0>/data/volume/hydrogen.dat";
         panel<NEportLevels={0,3}>;
      };
   };
   DVdownsize_unif DVdownsize_unif<NEx=121.,NEy=121.> {
      in => <-.Read_Volume.field;
      factor => <-.localUI.float;
   };
   VLILookupTable VLILookupTable<NEportLevels={0,1},NEx=319.,NEy=55.> {
      red => init_array(.size,0,(.size - 1));
      size = 256;
      green => init_array(.size,0,(.size - 1));
      blue => init_array(.size,0,(.size - 1));
      alpha => concat_array(init_array(85,0,0),init_array(171,4095,4095));
   };
   VLIVolume VLIVolume<NEx=363.,NEy=176.> {
      volume => <-.DVdownsize_unif.out;
      lookup_table => <-.VLILookupTable;
      crop_bounds => <-.VLICropBounds;
   };
   GDM.DataObject VolumeDataObject<NEx=484.,NEy=253.> {
      in => <-.VLIVolume;
   };
   DVbounds DVbounds<NEx=88.,NEy=176.> {
      in => <-.DVdownsize_unif.out;
      hull = 1;
      edges = 0;
      faces = 0;
      imin = 0;
      imax = 0;
      jmin = 0;
      jmax = 0;
      kmin = 0;
      kmax = 0;
      data = 0;
      component = 0;
   };
   GDM.DataObject BoundsDataObject<NEx=330.,NEy=253.> {
      in => <-.DVbounds.out;
      Props {
         col = {0.154056,0.98,0.1372};
      };
   };
   macro localUI<NEx=44.,NEy=253.> {
      link parent<NEportLevels={2,1},NEx=66.,NEy=44.> => <-.Read_Volume.read_volume_ui.panel;
      UIlabel Camera_Mode<NEx=66.,NEy=143.> {
         parent => <-.parent;
         y = 60;
         alignment = "left";
      };
      UItext UItext<NEx=66.,NEy=187.,NEportLevels={0,1}> {
         parent => <-.parent;
         text<NEportLevels={2,0}> => camera_mode;
         y = 75;
      };
      UIbutton Update_Camera<NEx=66.,NEy=231.> {
         parent => <-.parent;
         do<NEportLevels={0,3}>;
         y = 110;
      };
      link camera_mode<NEportLevels={2,1},NEx=319.,NEy=22.> => Uviewer3D.Scene.Camera.Camera.mode;
      UItoggle UItoggle<NEx=220.,NEy=143.> {
         parent => <-.parent;
         label => "Auto/Manual";
         set<NEportLevels={2,2}> => <-.camera_mode;
         y = 82;
         x = 110;
      };
      UIslider UIslider<NEx=220.,NEy=187.> {
         parent => <-.parent;
         value<NEportLevels={2,2}> = 1.;
         title => "Downsize Factor";
         y = 150;
         min = 1.;
         max = 10.;
         mode = "integer";
      };
      float float<NEportLevels={1,2},NEx=418.,NEy=187.>[3] => {UIslider.value,
         UIslider.value,UIslider.value};
   };
   GDM.Uviewer3D Uviewer3D<NEx=418.,NEy=330.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.BoundsDataObject.obj,<-.<-.<-.VolumeDataObject.obj};
            Xform {
               ocenter = {31.5,31.5,31.5};
               dcenter = {4.,4.,4.};
               mat = {
                  -0.0268929,0.0494245,-0.0414626,0.,0.0459326,-0.0168734,-0.0499055,
0.,-0.0452999,-0.0464507,-0.0259884,0.,0.,0.,0.,1.
               };
               xlate = {-4.,-4.,-4.};
               center = {31.5,31.5,31.5};
            };
         };
         Lights {
            Lights = {
               {
                  type="BiDirectional",,,,
               },,,};
         };
         Camera {
            Camera<NEx=143.,NEy=165.> {
               update<NEportLevels={5,0}> => <-.<-.<-.<-.localUI.Update_Camera.do;
               mode<NEportLevels={0,5}>;
               auto_norm = "None";
            };
         };
         View {
            View {
               back_col = {0.5,0.5,1.};
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
   UI {
      shell {
         x = 154;
         y = 116;
      };
      Modules {
         IUI {
            optionList {
               selectedItem = 1;
            };
         };
      };
   };
   UImod_panel UImod_panel<NEx=495.,NEy=110.> {
      option {
         set = 1;
      };
   };
   VLICropBounds VLICropBounds<NEx=286.,NEy=99.,NEportLevels={0,1}> {
      max_z = 54.;
      max_y = 51.;
      max_x = 49.;
      min_z = 24.;
      min_y = 19.;
      min_x = 15.;
      crop_enabled = 1;
      predefined = 0;
   };
   Utility_macros.VLIvolume_scrollUI VLIvolume_scrollUI<NEx=517.,NEy=198.> {
      VLIvol_info => <-.VLIVolume;
      parent => <-.UImod_panel;
   };
};
