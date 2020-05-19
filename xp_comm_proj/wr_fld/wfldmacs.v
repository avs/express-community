//
// file: wfldmacs.v
// author: Dave Goughnour, AVS
// purpose: High level macro library for write field module
//


flibrary WriteFldMacs<compile_subs=0> {

   macro WriteFldUI {

      UImod_panel UImod_panel {
         parent<NEportLevels={4,0}>;
         title => name_of(<-.<-.<-,1);
      };

      UIfileDialog Select_File {
         parent => <-.UImod_panel;
         visible<NEportLevels={2,1}> => <-.BrowseButton.do;
         searchPattern = "*.fld";
      };


      UIlabel FilenameLabel {
         parent => <-.UImod_panel;
         label => "Write Field Filename";
         alignment = 0;
         x = 0;
         y = 0;
         width = parent.width;
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

      UIbutton BrowseButton {
         parent => <-.UImod_panel;
         label = "Browse...";
         x = 0;
         y => <-.FilenameLabel.y + <-.FilenameLabel.height + 10;
      };
      UIbutton TriggerButton {
         parent => <-.UImod_panel;
         label = "Save File";
         x => <-.BrowseButton.x + <-.BrowseButton.width + 15;
         y => <-.BrowseButton.y;
      };


      UItext FilenameText {
         parent => <-.UImod_panel;
         text => <-.Select_File.filename;
         x = 0;
         y => <-.BrowseButton.y + <-.BrowseButton.height + 10;
         width => parent.width;
      };

      olink filename => .FilenameText.text;
      olink trigger => .TriggerButton.do;
   };


   macro write_field {
      ilink in_field;

      WriteFldUI WriteFldUI;

      XP_COMM_PROJ.WriteFld.WriteFldMods.WriteFldCore WriteFldCore {
         filename => <-.WriteFldUI.filename;
         input => <-.in_field;
         trigger => <-.WriteFldUI.trigger;
      };
   };


   APPS.SingleWindowApp WriteFldEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.orthoslice.out_obj,<-.<-.<-.bounds.out_obj
               };
               Xform {
                  ocenter = {31.5,31.5,31.5};
                  dcenter = {4.,4.,4.};
                  mat = {
                     -0.0504008,-0.0494651,0.0473139,0.,
                     0.00725434,0.0545653,0.0647738,0.,
                     -0.0680645,0.0424437,-0.0281317,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {-4.,-4.,-4.};
                  center = {31.5,31.5,31.5};
               };
            };
            Lights {
               Lights = {
                  {
                     type="BiDirectional"
                  },,,};
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
      };

      MODS.Read_Field Read_Field {
         read_field_ui {
            filename = "$XP_PATH<0>/data/field/hydrogen.fld";
         };
      };

      MODS.bounds bounds {
         in_field => <-.Read_Field.field;
      };
      MODS.orthoslice orthoslice {
         in_field => <-.Read_Field.field;
         OrthoSliceParam {
            axis = 1;
         };
      };

      XP_COMM_PROJ.WriteFld.WriteFldMacs.write_field write_field {
         in_field => <-.orthoslice.out_fld;
      };
   };


   APPS.SingleWindowApp ReadFldEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.Read_Field.out_obj
               };
               Xform {
                  ocenter = {31.5,31.5,0.};
                  dcenter = {4.,4.,0.};
                  mat = {
                     0.126984,0.,0.,0.,
                     0.,0.126984,0.,0.,
                     0.,0.,0.126984,0.,
                     0.,0.,0.,1.
                  };
                  xlate = {-4.,-4.,0.};
                  center = {31.5,31.5,0.};
               };
            };
         };
      };

      MODS.Read_Field Read_Field;
   };


};

