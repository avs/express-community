
flibrary GenerateSurfaceMacs{

  //GenerateSurfaceUI
  macro GenerateSurfaceUI{

    ilink in;
    XP_COMM_PROJ.GenerateSurface.GenerateSurfaceMods.GenerateSurfaceParams &Params <NEportLevels={2,1}>;

    UI.UImod_panel UImod_panel{
	  title = "GenerateSurfaceUI";
    };

    UIslider threshold{
      parent => <-.UImod_panel;
	  value =><-.Params.threshold;
      x = 10;
      y => 0;
      min = 0.0;
      max+nres => 10.0;
    };

  };
 
 //GenerateSurfaceFunc
 // Funcitonal macro
 macro GenerateSurfaceFunc{

    ilink in;
    XP_COMM_PROJ.GenerateSurface.GenerateSurfaceMods.GenerateSurfaceParams &Params <NEportLevels={2,1}>;

	XP_COMM_PROJ.GenerateSurface.GenerateSurfaceMods.GenerateSurfaceCore GenerateSurfaceCore{
	  input =><-. in;
      Params =><-.Params;
    };

	olink out => GenerateSurfaceCore.output;

 };


 //GenerateSurf
 // User Macro 
 macro GenerateSurface{

   ilink in;
   XP_COMM_PROJ.GenerateSurface.GenerateSurfaceMods.GenerateSurfaceParams &Params;

   XP_COMM_PROJ.GenerateSurface.GenerateSurfaceMacs.GenerateSurfaceFunc GenerateSurfaceFunc{
     &Params => <-.Params;
	 in => <-. in;
   };

   XP_COMM_PROJ.GenerateSurface.GenerateSurfaceMacs.GenerateSurfaceUI GenerateSurfaceUI{
      &Params => <-.Params;
      in =><-. in;
      UImod_panel{
	    title => name_of(<-.<-.<-);
		};
   };

   olink out => GenerateSurfaceFunc.out;

 };

};
