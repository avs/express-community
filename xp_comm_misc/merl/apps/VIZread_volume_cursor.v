APPS.SingleWindowApp SingleWindowApp {
   float normal_coords[2][3];
   MODS.Read_Volume Read_Volume<NEx=11.,NEy=22.> {
      read_volume_ui {
         filename = "$XP_PATH<0>/data/volume/hydrogen.dat";
         panel<NEportLevels={0,3}>;
      };
   };
   MODS.downsize downsize<NEx=121.,NEy=121.> {
      in_field => <-.Read_Volume.field;
      DownsizeParam {
	factor0 => <-.<-.localUI.UIslider.value;
	factor1 => <-.<-.localUI.UIslider.value;
	factor2 => <-.<-.localUI.UIslider.value;
      };
   };
   VLILookupTable VLILookupTable<NEportLevels={0,1},NEx=319.,NEy=66.> {
      red => init_array(.size,0,(.size - 1));
      size = 256;
      green => init_array(.size,0,(.size - 1));
      blue => init_array(.size,0,(.size - 1));
      alpha => concat_array(init_array(85,0,0),init_array(171,4095,4095));
   };
   VLIVolume VLIVolume<NEx=374.,NEy=121.> {
      volume => <-.downsize.out_fld;
      lookup_table => <-.VLILookupTable;
      cursor => <-.VLICursorInfo;
   };
   GDM.DataObject VolumeDataObject<NEx=517.,NEy=198.> {
      in => <-.VLIVolume;
   };
   MODS.bounds bounds<NEx=88.,NEy=176.> {
      BoundsParam {
	in_field => <-.downsize.out_fld;
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
      out_obj {
	props {
	  col = {0.154056,0.98,0.1372};
	};
      };
   };
   macro localUI<NEx=44.,NEy=198.> {
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
   };
   GDM.Uviewer3D Uviewer3D<NEx=418.,NEy=264.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.bounds.out_obj,<-.<-.<-.VolumeDataObject.obj};
            Xform {
               ocenter = {31.5,31.5,31.5};
               dcenter = {4.,4.,4.};
               mat = {
                  -0.0595153,-0.01031,0.0351687,0.,0.0366475,-0.0172425,0.056963,
0.,0.000273359,0.0669445,0.020088,0.,0.,0.,0.,1.
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
   };
   VLICursorInfo VLICursorInfo<NEx=297.,NEy=22.,NEportLevels={0,1}> {
      position = {20.,20.,20.};
      enabled = 1;
   };
};
