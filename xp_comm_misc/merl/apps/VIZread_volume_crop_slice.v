APPS.SingleWindowApp SingleWindowApp {
   MODS.Read_Volume Read_Volume<NEx=11.,NEy=77.> {
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
   VLILookupTable VLILookupTable<NEportLevels={0,1},NEx=319.,NEy=55.> {
      red => init_array(.size,0,(.size - 1));
      size = 256;
      green => init_array(.size,0,(.size - 1));
      blue => init_array(.size,0,(.size - 1));
      alpha => concat_array(init_array(85,0,0),init_array(171,4095,4095));
   };
   VLIVolume VLIVolume<NEx=363.,NEy=176.> {
      volume => <-.downsize.out_fld;
      lookup_table => <-.VLILookupTable;
      crop_bounds => <-.VLICropBounds;
      slice_plane => <-.Plane.out_fld;
      slice_flags = "inside";
      slice_offset = 10.;
      slice_falloff = 10.;
   };
   GDM.DataObject VolumeDataObject<NEx=484.,NEy=253.> {
      in => <-.VLIVolume;
      child_objs => {<-.Plane.out_obj};
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
   };
   GDM.Uviewer3D Uviewer3D<NEx=418.,NEy=330.> {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.bounds.out_obj,<-.<-.<-.VolumeDataObject.obj};
            Xform {
               ocenter = {50.,50.,31.5};
               dcenter = {4.,4.,2.52};
               mat = {
                  -0.000354397,-0.000327516,0.0799985,0.,0.000106669,0.0799992,
0.000327992,0.,-0.079999,0.000108122,-0.000353987,0.,0.,0.,0.,1.
               };
               xlate = {-4.17893,-3.9006,
-2.52};
               center = {50.,50.,31.5};
            };
         };
         Lights {
            Lights = {
               {
                  type="BiDirectional"
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
                  selectedItem = 3;
                  ok = 1;
               };
               RaiseCurObj {
                  do = 1;
               };
            };
            GDobj_sel {
               curobj_name = "      Plane";
            };
         };
         Object_Editor {
            GDmodes_edit {
               lines = "Regular";
               surf = "None";
            };
            IUI {
               optionList {
                  selectedItem = 2;
               };
               Modes {
                  IUI {
                     panel {
                        visible = 1;
                     };
                     Line {
                        OPcmdList = {
                           ,,
                           {
                              set=1
                           },
                           ,,
                        };
                     };
                     Surface {
                        OPcmdList = {
                           ,
                           {
                              set=1
                           },
                           ,,,
                        };
                     };
                  };
               };
            };
         };
      };
   };
   UI {
      shell {
         x = 33;
         y = 33;
      };
      Modules {
         IUI {
            optionList {
               selectedItem = 1;
            };
         };
      };
      Windows {
         IUI {
            panel {
               height = 482;
            };
         };
      };
      Editors {
         IUI {
            optionList {
               selectedItem = 0;
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
      max_z = 63.;
      max_y = 63.;
      max_x = 63.;
      min_z = 0.;
      min_y = 0.;
      min_x = 0.;
      crop_enabled = 1;
      predefined = 0;
   };
   Utility_macros.VLIvolume_scrollUI VLIvolume_scrollUI<NEx=517.,NEy=198.> {
      VLIvol_info => <-.VLIVolume;
      parent => <-.UImod_panel;
      Slice {
         Slice_Options = {,
            {
               set=1
            },};
      };
   };
   GEOMS.Plane Plane<NEx=242.,NEy=11.> {
      plane {
         points = {0.,0.,100.,100.};
         xform {
            xlate = {0.162376,1.77532,38.6243};
         };
      };
      obj {
         Modes {
            mode = {0,2,1,0,0};
         };
      };
      dim1 = 3;
      dim2 = 3;
   };
};
