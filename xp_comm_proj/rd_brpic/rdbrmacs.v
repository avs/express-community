flibrary ReadBRPicMacs <compile_subs=0> {

       //ReadBRPicUI
       //readBRPic
       //ReadBRPicEg

//************************************************************************

   //ReadBRPicUI

    macro ReadBRPicUI {
      Root.UImod_panel UImod_panel {
	title => "readBRPic";
	parent<NEportLevels={3,0}>;
      };
      UIlabel Filename {
	parent => <-.UImod_panel;
	y = 0;
	width = 200;
	alignment = 0;
      };
      UItext file_name {
	parent => <-.UImod_panel;
	y => ((Filename.y + Filename.height) + 5);
	text => <-.filename_out;
	width = 170;
	showLastPosition = 1;
      };
      UIbutton visible {
	parent => <-.UImod_panel;
	x => ((file_name.x + file_name.width) + 5);
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
	title => "Filename";
	filename => <-.filename_out;
	searchPattern = "./*.pic";
      };
      string filename_out<NEportLevels={0,2}>;
    };
 


//************************************************************************

   // readBRPic

   macro readBRPic {
      XP_COMM_PROJ.ReadBioRadPic.ReadBRPicMacs.ReadBRPicUI ReadBRPicUI;

      XP_COMM_PROJ.ReadBioRadPic.ReadBRPicMods.ReadPicData ReadPicData {
         filename => <-.ReadBRPicUI.filename_out;
      };

      FLD_MAP.uniform_scalar_field uniform_scalar_field {
   	   mesh {
            in_dims => <-.<-.ReadPicData.dims;
	      };
	      data {
            in_data => <-.<-.ReadPicData.data;
	      };
      };

      olink out_fld => uniform_scalar_field.out;
      olink out_obj => uniform_scalar_field.obj;
   };


//************************************************************************

   APPS.MultiWindowApp ReadBRPicEg<NEdisplayMode="maximized"> {

      GDM.Uviewer3D Uviewer3D {
   	   Scene {
            Top {
               child_objs => {
                  <-.<-.<-.volume_render.DataObject.obj,<-.<-.<-.Axis3D.out_obj,
                  <-.<-.<-.bounds.out_obj};
            };
            View {
               View {
                  renderer => "Software";
               };
            };
         };
      };

      XP_COMM_PROJ.ReadBioRadPic.ReadBRPicMacs.readBRPic readBRPic;

      MODS.scale scale {
	 in_field => <-.readBRPic.out_fld;
      };

      MODS.volume_render volume_render {
	 in_mesh => <-.scale.out_fld;
	 DataObject {
            Props {
               ray_algo = "Maximum";
               inherit = 0;
            };
	 };
	 GDprops_edit {
            ray_algo = "Maximum";
            inherit = 0;
	 };
	 inherit {
            set = 1;
	 };
	 surf_rb {
            x = 0;
	 };
	 ray_tracer {
            set = 1;
	 };
	 vol_rb {
            x = 0;
	 };
	 interp_tri {
            set = 1;
	 };
	 interp_rb {
            x = 0;
	 };
	 ray_max {
            set = 1;
	 };
	 ray_rb {
            x = 0;
	 };
	 dist_glob {
            set = 1;
	 };
	 dist_rb {
            x = 0;
	 };
	 alpha_const {
            set = 1;
	 };
	 alpha_rb {
            x = 0;
	 };
      };

      GEOMS.Axis3D Axis3D {
	 in_field => <-.scale.out_fld;
      };

      MODS.bounds bounds {
	 in_field => <-.scale.out_fld;
      };

   };




};
