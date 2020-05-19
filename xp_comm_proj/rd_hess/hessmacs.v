
flibrary ReadHessMacs <compile_subs=0> {

   macro ReadHessUI {
      ilink filename;

      UImod_panel panel {
         message = "Select Read Hess control panel.";
         title => name_of(<-.<-.<-);
         parent<NEportLevels={4,0}>;
      };

      UIlabel UCD_Filename {
         parent => <-.panel;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Read Hess Filename";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

      UItext file_name {
         parent => panel;
         y => UCD_Filename.y + UCD_Filename.height + 5;
         text<NEportLevels={2,2}> => <-.filename;
         width = 170;
         showLastPosition = 1;
      };

      UIbutton visible {
         parent => panel;
         x => file_name.x + file_name.width + 5;
         y => file_name.y;
         width = 75;
         height => <-.file_name.height;
         label => "Browse...";
      };

      UIfileSB file_browser {
         GMOD.copy_on_change copy_on_change {
            trigger => <-.<-.visible.do;
            input => <-.<-.visible.do;
            output => <-.visible;
         };
         visible => <-.visible.do;
         title => "Read Hess Filename";
         searchPattern = "$XP_PATH<1>/*.n3d";
         filename<NEportLevels={2,2}> => <-.filename;
      };
   };


   macro read_hess {
      string+OPort filename;

      ReadHessUI ReadHessUI {
         filename => <-.filename;
      };

      XP_COMM_PROJ.ReadHess.ReadHessMods.Read_Hess_File Read_Hess_File {
         hess_filename => <-.filename;
      };
      GDM.DataObject DataObject {
         in => <-.Read_Hess_File.out_fld;
      };

      olink out_fld    => .Read_Hess_File.out_fld;
      olink out_obj    => .DataObject.obj;
   };


   APPS.SingleWindowApp ReadHessEg {

      XP_COMM_PROJ.ReadHess.ReadHessMacs.read_hess read_hess;
 
      MODS.mirror mirror {
         in_field => <-.read_hess.out_fld;
      };


      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.mirror.out_obj};
            };
         };
      };
   };

}; // end of flibrary

