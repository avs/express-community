
flibrary WritePovMacs <compile_subs=0> {

    macro WritePovUI {
        ilink err_vis;
        ilink err_msg;

        UImod_panel panel {
            title => name_of(<-.<-.<-,1);
            message = "Select Write POVray control panel.";
            parent<NEportLevels={4,0}>;
        };
        UIlabel UCD_Filename {
            parent => <-.panel;
            y = 0;
            width => parent.width;
            alignment = 0;
            label = "Write POVray File Name";
            color {
               backgroundColor = "blue";
               foregroundColor = "white";
            };
        };
        UItext file_name {
            parent => panel;
            y => UCD_Filename.y + UCD_Filename.height + 5;
            text => <-.filename;
            width = 170;
            showLastPosition = 1;
        };
        UIbutton visible {
            parent => panel;
            x => file_name.x + file_name.width + 5;
            y => file_name.y;
            width = 75;
            height => <-.file_name.height;
            label = "Browse...";
        };
        
        UIfileSB file_browser {
            GMOD.copy_on_change copy_on_change {
               trigger => <-.<-.visible.do; 
               input => <-.<-.visible.do;
               output => <-.visible;
            };
            title = "Write POVray Filename";
            searchPattern = "/tmp/*";
            filename => <-.filename;
        };
        UIlabel err_label {
            parent => <-.panel;
            visible => <-.err_vis;
            y => file_name.y + file_name.height + 5;
            width => parent.width;
            alignment = 0;
            label = "Read Error:";
            color {
               foregroundColor = "red";
            };
        };
        UIlabel err_label_msg {
            parent => <-.panel;
            visible => <-.err_vis;
            y => err_label.y + err_label.height + 5;
            width => parent.width;
            alignment = 0;
            label => <-.err_msg;
        };

        string+OPort2 filename = "/tmp/avs.pov";
    };


//----------------------------------------------------------------------------------------------


   // Writes a POV file based upon a single field input
   macro write_pov {

      ilink in_fld;

      string name = "default_avs_field";

      XP_COMM_PROJ.WritePov.WritePovMods.WritePovCore WritePovCore {
         filename => <-.WritePovUI.filename;
         in => {<-.in_fld};
         nobjs = 1;
         top_name => <-.name;
      };

      WritePovUI WritePovUI {
         err_vis => <-.WritePovCore.err;
         err_msg => <-.WritePovCore.err_str;
      };

   };



   // Writes a POV file based upon a single Graphic Display Object
   write_pov write_pov_GD {

      GDM.GDobject_templ+nres &obj<NEportLevels={2,1}>;
      link in_fld<NEportLevels={1,1}> => obj.input;
      name => obj.name;

   };


   // Writes a POV file based upon a GroupObject (a group of Graphic Display Objects)
   write_pov write_pov_ARR {

      link in_fld<NEportLevels={1,1}> => ;

      GDM.GDobject_templ+nres &top_obj<NEportLevels={2,1}>;

      int+nres nobjs => top_obj.nobjs;

      string+nres top_name => top_obj.name;

      GDobject_templ+nres &obj_list<NEportLevels={1,1}>[] => top_obj.objects;

      mlink+nres fld_list<NEportLevels={1,1}> => obj_list.input;

      link+nres in_fld<NEportLevels={1,1}> => fld_list[0];

      WritePovCore {
         nobjs => <-.nobjs;
         in => <-.fld_list;
         top_name => <-.top_name;
      };

   };


//----------------------------------------------------------------------------------------------


   // Demonstrates how a POV file can be created from a field input
   APPS.SingleWindowApp WritePovEg {
      GDM.Uviewer3D Uviewer3D<NEx=432,NEy=240> {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface.out_obj};
            };
         };
      };

      MODS.Read_Volume Read_Volume<NEx=121.,NEy=33.> {
         read_volume_ui {
            filename = "$XP_PATH<0>/data/volume/hydrogen.dat";
         };
      };
      MODS.bounds bounds<NEx=418.,NEy=99.> {
         in_field => <-.Read_Volume.field;
      };
      MODS.downsize downsize<NEx=121.,NEy=110.> {
         in_field => <-.Read_Volume.field;
      };
      MODS.isosurface isosurface<NEx=121.,NEy=187.> {
         in_field => <-.downsize.out_fld;
      };

      write_pov write_pov<NEx=121.,NEy=253.> {
         in_fld => <-.isosurface.out_fld;
      };

   };



   // Demonstrates how a POV file can be created from a GroupObject input
   APPS.MultiWindowApp WritePovArrEg {

      GDM.Uviewer3D Uviewer3D<NEx=209.,NEy=231.> {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.stream_multi_block.out_obj,<-.<-.<-.bounds_ARR.out_obj,<-.<-.<-.isosurface_ARR.out_obj};
            };
         };
      };

      MODS.Plot3d_Multi_Block Plot3d_Multi_Block<NEx=220,NEy=22> {
         filename_xyz = "$XP_PATH<0>/data/plot3d/multb_x.dat";
         filename_q = "$XP_PATH<0>/data/plot3d/multb_q.dat";
      };


      MODS_ARR.combine_vect_ARR combine_vect_ARR<NEx=55.,NEy=88.> {
         in_field => <-.Plot3d_Multi_Block.fields;
         CombineVectParam {
            components = {1,2,3};
         };
      };
      MODS.fields_to_mblock fields_to_mblock<NEx=55.,NEy=132.> {
         fields => <-.combine_vect_ARR.out_fld;
      };


      GEOMS.FPlane FPlane<NEx=209.,NEy=88.> {
         in => <-.Plot3d_Multi_Block.mblock;
         probe_ui {
            dim2_slider {
               y => 56;
            };
         };
         plane {
            xform {
               xlate = {0.,0.,0.04};
            };
         };
      };
      MODS.stream_multi_block stream_multi_block<NEx=55.,NEy=176.> {
         in_field => <-.fields_to_mblock.multi_block;
         in_probe => <-.FPlane.out_fld;
      };

      MODS_ARR.bounds_ARR bounds_ARR<NEx=209.,NEy=143.> {
         in_field => <-.Plot3d_Multi_Block.fields;
      };

      MODS_ARR.isosurface_ARR isosurface_ARR<NEx=418.,NEy=88.> {
         in_field => <-.Plot3d_Multi_Block.fields;
         IsoParam {
            iso_component = 4;
            map_component = {1};
            iso_level => -1.7;
         };
      };

      write_pov_ARR write_pov_ARR<NEx=484.,NEy=231.> {
         top_obj => <-.isosurface_ARR.out_obj;
      };

   };


};

