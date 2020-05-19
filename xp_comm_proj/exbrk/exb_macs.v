
flibrary ExcavateBrickMacros{

  //ExcavateBrickUI
  macro ExcavateBrickUI{

    ilink Input_Field;
    XP_COMM_PROJ.ExcavateBrick.ExcavateBrickMods.ExcavateBrickParams &Params <NEportLevels={2,1}>;

    UI.UImod_panel UImod_panel{
	  title = "ExcavateBrick";
    };
    UItoggle Below_X{
      parent => <-.UImod_panel;
	  set =><-.Params.belowX;
      y = 10;
      x = 10;
    };
    UIslider X{
      parent => <-.UImod_panel;
	  value =><-.Params.X;
      x = 10;
      y => Below_X.y + Below_X.height + 10;
      min = 0.;
      max+nres => Input_Field.dims[0];
    };
    UItoggle Below_Y{
      parent => <-.UImod_panel;
	  set =><-.Params.belowY;
      y => X.y + X.height + 20;
      x = 10;
    };
    UIslider Y{
      parent => <-.UImod_panel;
	  value =><-.Params.Y;
      y => Below_Y.y + Below_Y.height + 10;
      min = 0.;
      max+nres => Input_Field.dims[1];
      x = 10;
    };
    UItoggle Below_Z{
      parent => <-.UImod_panel;
	  set =><-.Params.belowZ;
      y => Y.y + Y.height + 20;
      x = 10;
    };
    UIslider Z{
      parent => <-.UImod_panel;
	  value =><-.Params.Z;
      y => Below_Z.y + Below_Z.height + 10;
      min = 0.;
      max+nres => Input_Field.dims[2];
      x = 10;
    };
  };


 //ExcavateBrickFunc
 // Funcitonal macro
 macro ExcavateBrickFunc{
    ilink Input_Field;
    XP_COMM_PROJ.ExcavateBrick.ExcavateBrickMods.ExcavateBrickParams &Params <NEportLevels={2,1}>;
	XP_COMM_PROJ.ExcavateBrick.ExcavateBrickMods.ExcavateBrick ExcavateBrick{
	  Input_Field =><-. Input_Field;
      ExcavateBrickParams =><-.Params;
    };

	olink out => ExcavateBrick.Output_Field;
 };

 //Excavate_Brick
 // User Macro 
 macro Excavate_Brick{

   ilink Input_Field;
   XP_COMM_PROJ.ExcavateBrick.ExcavateBrickMods.ExcavateBrickParams Params;

   XP_COMM_PROJ.ExcavateBrick.ExcavateBrickMacs.ExcavateBrickFunc ExcavateBrickFunc{
     &Params => <-.Params;
	 Input_Field => <-. Input_Field;
   };

   XP_COMM_PROJ.ExcavateBrick.ExcavateBrickMacs.ExcavateBrickUI ExcavateBrickUI{
      &Params => <-.Params;
      Input_Field =><-. Input_Field;
      UImod_panel{
	    title => name_of(<-.<-.<-);
		};
   };

   olink out => ExcavateBrickFunc.out;
 };

};
