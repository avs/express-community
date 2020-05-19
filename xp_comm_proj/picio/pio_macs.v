
//
// Copyright (C) Chris Pudney, The University of Western Australia, 1997.
// All rights reserved.
// 
// Permission to use, copy, modify and distribute this software and its
// documentation only for the purposes of teaching and research is hereby
// granted without fee, provided that the above copyright notice and this
// permission notice appear in all copies of this software/documentation and
// that you do not sell the software.  No commercial use or distribution
// of the software is permitted without the consent of the copyright
// owners.  Commercial licensing is available by contacting the author(s).
//
// THIS SOFTWARE/DOCUMENTATION IS PROVIDED WITH NO WARRANTY, EXPRESS OR
// IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTY OF MERCHANTABILITY OR
// FITNESS FOR A PARTICULAR PURPOSE.
//

flibrary PICioMacs<compile_subs=0> {

  macro Read_PIC_UI {
    Root.UImod_panel UImod_panel {
      title => name_of(<-.<-.<-);
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
      text => <-.filename;
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
      filename => <-.filename;
      searchPattern = "./*.pic";
    };

    string+OPort2 filename;
  };


  macro Read_BioRad_PIC {
    ilink FILE;

    XP_COMM_PROJ.PIC_IO.PICioMods.Read_PIC_core Read_PIC_core {
      FILE => <-.FILE;
    };

    FLD_MAP.uniform_scalar_field uniform_scalar_field {
      mesh {
        in_dims => <-.<-.Read_PIC_core.dims;
      };
      data {
        in_data => <-.<-.Read_PIC_core.data;
      };
    };

    olink out_fld => .uniform_scalar_field.out;
    olink out_obj => .uniform_scalar_field.obj;
  };


  macro read_BioRad_PIC {

    XP_COMM_PROJ.PIC_IO.PICioMacs.Read_PIC_UI Read_PIC_UI;

    file FILE {
      name => <-.Read_PIC_UI.filename;
    };

    XP_COMM_PROJ.PIC_IO.PICioMacs.Read_BioRad_PIC Read_Biorad_PIC {
      FILE => <-.FILE;
    };

    olink out_fld => .Read_Biorad_PIC.out_fld;
    olink out_obj => .Read_Biorad_PIC.out_obj;
  };



  APPS.MultiWindowApp Read_BioRad_PIC_Eg {

    GDM.Uviewer3D Uviewer3D {
      Scene {
        Top {
          child_objs => {
            <-.<-.<-.volume_render.out_obj,<-.<-.<-.Axis3D.out_obj,
            <-.<-.<-.bounds.out_obj};
        };
      };
      Scene_Editor {
        View_Editor {
          GDview_edit {
            renderer => "Software";
          };
        };
      };
    };

    XP_COMM_PROJ.PIC_IO.PICioMacs.read_BioRad_PIC read_BioRad_PIC{
		Read_PIC_UI {
			filename="xp_comm_proj/picio/flea3.pic";
		};
	};

    MODS.scale scale {
      in_field => <-.read_BioRad_PIC.out_fld;
    };


    MODS.volume_render volume_render {
      in_mesh => <-.scale.out_fld;
      DataObject {
        VolRenderDatamap {
          DataRange = {
            {
              UIMaxValue=0.,,,,,,,,,,,,
            },};
          DatamapValue = {
            {
              v4=0.,v3=0.,,
            },,,
            {
              v3=0.,,
            }};
        };
        Props {
          ray_algo = 2;
          inherit = 0;
        };
      };
      GDprops_edit {
        ray_algo = 2;
        inherit = 0;
      };
      modes_ui {
		  surf_rb {
	        x = 0;
	      };
	      vol_rb {
	        x = 0;
	      };
	  };
	  props_ui{
    	  interp_rb {
        	x = 0;
    	  };
    	  ray_max {
        	set = 1;
    	  };
    	  ray_rb {
        	x = 0;
    	  };
    	  dist_rb {
        	x = 0;
    	  };
	  };
	  datamap_ui {
    	  alpha_rb {
        	x = 0;
    	  };
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
