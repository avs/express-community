
flibrary EnhanceContrastMacs
{

  macro IPEnhanceContrastUI {
    XP_COMM_PROJ.EnhanceContrast.EnhanceContrastMods.EnhanceContrastParams &params<NEportLevels={2,1}>;

    UImod_panel UImod_panel {
      title = "Enhance Contrast";
      parent<NEportLevels={4,0}>;
    };

    UIslider UIslider {
      parent => <-.UImod_panel;
      title = "Clipping Level";
      value => <-.params.level;
      y = 5;
      min = 0.;
      max = 5.;
      decimalPoints = 1;
    };
  };



  macro IPEnhanceContrastFunc {
    ip_Image_InByte &in_image<NEportLevels={2,1}>;

    XP_COMM_PROJ.EnhanceContrast.EnhanceContrastMods.EnhanceContrastParams &params<NEportLevels={2,1}>;
      
    XP_COMM_PROJ.EnhanceContrast.EnhanceContrastMods.EnhanceContrastCore EnhanceContrastCore {
       level => <-.params.level;
       in_dims=> {<-.in_image.xsize,<-.in_image.ysize};
       img => <-.in_image.data;
    };
    
    ip_ImageByte+OPort out_ip_image {
      data+IPort2 => <-.EnhanceContrastCore.out;
      xsize => <-.in_image.xsize;
      ysize => <-.in_image.ysize;
    };

    olink out_image => out_ip_image;
  };



  macro IPEnhanceContrast {
    ip_Image_InByte &in_image<NEportLevels={2,1}>;

    XP_COMM_PROJ.EnhanceContrast.EnhanceContrastMods.EnhanceContrastParams params {
      level = 1.0;
    }; 

    IPEnhanceContrastUI IPEnhanceContrastUI {
		params => <-.params;
      UImod_panel {
        title => name_of(<-.<-.<-,1);
      };
    };
   
    IPEnhanceContrastFunc IPEnhanceContrastFunc {
      in_image => <-.in_image;
      params => <-.params;
    }; 

    IPimageToFld IPimageToFld {
      In => <-.IPEnhanceContrastFunc.out_image;
      dataObject.Obj.name => name_of(<-.<-.<-.<-);
    };

    olink out_image => IPEnhanceContrastFunc.out_image;
    olink out_fld   => IPimageToFld.TmpField;
    olink out_obj   => IPimageToFld.dataObject.obj;
  };



  group ip_enhance_contrast {
    ip_Image_InByte &in_image<NEportLevels={2,1}>;
    float level<NEportLevels={2,1}> = 1.0;

    XP_COMM_PROJ.EnhanceContrast.EnhanceContrastMods.EnhanceContrastCore EnhanceContrastCore {
       level => <-.level;
       in_dims=> {<-.in_image.xsize,<-.in_image.ysize};
       img => <-.in_image.data;
    };
    
    ip_ImageByte+OPort out_ip_image {
      data+IPort2 => <-.EnhanceContrastCore.out;
      xsize => <-.in_image.xsize;
      ysize => <-.in_image.ysize;
    };

    olink out_image => out_ip_image;
  };

};

