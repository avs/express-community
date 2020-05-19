
flibrary WriteSTLMacs <compile_subs=0> {

   macro writeSTL {

      ilink in_fld;

      XP_COMM_PROJ.WriteSTL.WriteSTLMods.write_stl write_stl {
         filename => <-.WriteSTL_UI.filename;
         trigger => <-.WriteSTL_UI.trigger;
         in => <-.in_fld;
      };

      macro WriteSTL_UI {
         ilink err_vis => <-.write_stl.err;
         ilink err_msg => <-.write_stl.err_str;

	      UImod_panel panel {
            title => name_of(<-.<-.<-,1);
	         message = "Select Write STL control panel.";
	         parent<NEportLevels={4,0}>;
	      };
	      UIlabel STL_Filename {
	         parent => <-.panel;
	         y = 0;
	         width => parent.width;
	         alignment = 0;
            label = "Write STL File Name";
            color {
               backgroundColor = "blue";
               foregroundColor = "white";
            };
	      };
         UItext file_name {
	         parent => panel;
	         y => STL_Filename.y + STL_Filename.height + 5;
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
	
	      UIbutton write_file {
	         parent => panel;
	         x => file_name.x + 15;
	         y => file_name.y + file_name.height + 5;
	         width = 75;
	         height => <-.file_name.height;
	         label = "Write File";
	      };

	      UIfileSB file_browser {
	         GMOD.copy_on_change copy_on_change {
               trigger => <-.<-.visible.do; 
               input => <-.<-.visible.do;
               output => <-.visible;
	         };
	         title = "Write STL Filename";
	         searchPattern = "*";
	         filename => <-.filename;
         };
         UIlabel err_label {
	         parent => <-.panel;
            visible => <-.err_vis;
	         y => write_file.y + write_file.height + 5;
	         width => parent.width;
	         alignment = 0;
            label = "Write STL Error:";
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

	      string+OPort2 filename = "/tmp/avs.stl";
         int+OPort2 trigger => .write_file.do;
      };

   };

   APPS.SingleWindowApp WriteSTLEg<NEdisplayMode="maximized"> {

      GDM.Uviewer3D Uviewer3D<NEx=432,NEy=240> {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface.out_obj
               };
            };
         };
      };

      MODS.Read_Volume Read_Volume<NEx=121.,NEy=33.> {
         read_volume_ui {
            filename = "$XP_ROOT/data/volume/hydrogen.dat";
         };
      };
      MODS.bounds bounds<NEx=418.,NEy=99.> {
         in_field => <-.Read_Volume.field;
      };
      MODS.downsize downsize<NEx=121.,NEy=110.> {
         in_field => <-.Read_Volume.field;
         DownsizeParam {
            factor0 = 2.;
            factor1 = 2.;
            factor2 = 2.;
         };
      };
      MODS.isosurface isosurface<NEx=121.,NEy=187.> {
         in_field => <-.downsize.out_fld;
      };

      XP_COMM_PROJ.WriteSTL.WriteSTLMacs.writeSTL writeSTL<NEx=121.,NEy=253.> {
         in_fld => <-.isosurface.out_fld;
      };
   };

};

