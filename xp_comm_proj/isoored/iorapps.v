// by James S Perrin  Time-stamp: <Monday 14/05/01 15:10:54 zzcgujp>

flibrary IsoObjsReduceApps {
   APPS.SingleWindowApp IsoObjsReduceEg<NEdisplayMode="maximized"> {
      GDM.Uviewer3D Uviewer3D {
	 Scene {
	    Top {
	       child_objs => {
	       <-.<-.<-.IsoObjsReduce.obj};
	       Xform {
		  ocenter = {59.5,58.9655,16.4145};
		  dcenter = {12.4162,17.7089,
		  -20.8603};
		  mat = {
		     0.0249306,-0.0119579,0.0587354,0.,0.0589222,0.0165634,-0.0216377,
		     0.,-0.0110004,0.0616202,0.0172142,0.,0.,0.,0.,1.
		  };
		  xlate = {-12.4162,-17.7089,
		  20.8603};
	       };
	    };
	    View {
	       View {
		  trigger = 1;
	       };
	    };
	 };
      };
      MODS.Read_Field Read_Field {
	 read_field_ui {
	    file_browser {
	       x = 496;
	       y = 287;
	       width = 300;
	       height = 388;
	       ok = 1;
	    };
	 };
	 DVread_field {
	    Mesh_Unif+Node_Data Output_Field;
	 };
      };
      MODS.downsize downsize {
	 in_field => <-.Read_Field.field;
	 DownsizeParam {
	    factor0 = 2.;
	    factor1 = 2.;
	    factor2 = 1.;
	 };
      };
      MODS.isosurface isosurface {
	 in_field => <-.downsize.out_fld;
	 IsoParam {
	    iso_level => 88.5;
	 };
      };
      XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMacs.IsoObjsReduce IsoObjsReduce {
	 in => <-.isosurface.out_fld;
      };
   };

   APPS.SingleWindowApp IsoObjsReduce2Eg<NEdisplayMode="maximized"> {
      GDM.Uviewer3D Uviewer3D {
	 Scene {
	    Top {
	       child_objs => {
	       <-.<-.<-.IsoObjsReduce2.obj};
	       Xform {
		  ocenter = {59.5,58.9655,16.4145};
		  dcenter = {12.4162,17.7089,
		  -20.8603};
		  mat = {
		     0.000874584,-0.00502081,0.0572207,0.,0.0568391,0.00833538,-0.000137399,
		     0.,-0.00829056,0.0566172,0.00509447,0.,0.,0.,0.,1.
		  };
		  xlate = {-12.4162,-17.7089,
		  20.8603};
	       };
	    };
	    View {
	       View {
		  trigger = 1;
	       };
	    };
	 };
      };
      MODS.Read_Field Read_Field {
	 read_field_ui {
	    file_browser {
	       x = 496;
	       y = 287;
	       width = 300;
	       height = 388;
	       ok = 1;
	    };
	 };
	 DVread_field {
	    Mesh_Unif+Node_Data Output_Field;
	 };
      };
      MODS.downsize downsize {
	 in_field => <-.Read_Field.field;
	 DownsizeParam {
	    factor0 = 2.;
	    factor1 = 2.;
	    factor2 = 1.;
	 };
      };
      MODS.isosurface isosurface {
	 in_field => <-.downsize.out_fld;
	 IsoParam {
	    iso_level => 88.5;
	 };
      };
      XP_COMM_PROJ.IsoObjsReduce.IsoObjsReduceMacs.IsoObjsReduce2 IsoObjsReduce2 {
	 in => <-.isosurface.out_fld;
      };
   };

};
