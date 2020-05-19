flibrary BoundingboxesApps {

   APPS.SingleWindowApp BoundingboxesEg<NEdisplayMode="maximized"> {
      MODS.Read_Field Read_Field {
	 read_field_ui {
	    file_browser {
	       ok = 1;
	       dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
	    };
	    filename = "$XP_PATH<0>/data/field/hydrogen.fld";
	 };
      };
      XP_COMM_PROJ.Boundingboxes.BoundingboxesMacs.Bounding_boxes Bounding_boxes {
	 in_field => <-.Read_Field.field;
	 Bounding_boxes_func {
	    plane_free {
	       xform {
		  ocenter = {31.5,31.5,0.};
		  dcenter = {38.8138,38.8138,0.};
		  mat = {
		     1.11237,0.0982872,0.520816,0.,0.293383,0.894203,-0.795364,0.,
		     -0.441402,0.842032,0.783851,0.,0.,0.,0.,1.
		  };
		  xlate = {-7.31378,-7.31378,31.5};
		  center = {31.5,31.5,0.};
	       };
	    };
	 };
      };
      MODS.isovolume isovolume {
	 in_field => <-.cut.out_fld;
      };
      MODS.cut cut {
	 in_field => <-.Read_Field.field;
	 in_plane => <-.Bounding_boxes.out_plane;
      };
      GDM.Uviewer3D Uviewer3D {
	 Scene {
	    Top {
	       child_objs => {
	       <-.<-.<-.Bounding_boxes.out_boxes,<-.<-.<-.isovolume.out_obj};
	       Xform {
		  ocenter = {31.5,31.5,31.5};
		  dcenter = {3.87132,3.39941,	4.28114};
		  mat = {
		     0.00858664,-0.0549804,-0.060615,0.,-0.0443845,0.0480758,-0.0498944,
		     0.,0.0687533,0.0379026,-0.0246398,0.,0.,0.,0.,1.
		  };
		  xlate = {-3.87132,-3.39941, -4.28114};
		  center = {31.5,31.5,31.5};
	       };
	    };
	    View {
	       View {
		  back_col = {1.,1.,1.};
	       };
	    };
	 };
      };
   };

   APPS.SingleWindowApp BoundingboxesEg2<NEdisplayMode="maximized"> {
      MODS.Read_Field Read_Field {
	 read_field_ui {
	    file_browser {
	       ok = 1;
	       dirMaskCache = "$XP_PATH<0>/data/field/*.fld";
	    };
	    filename = "$XP_PATH<0>/data/field/hydrogen.fld";
	 };
      };
      MODS.coordinate_math coordinate_math {
	 in_field1 => <-.Read_Field.field;
	 UIpanel {
	    option {
	       set = 1;
	    };
	 };
	 operation_x = "#1x*.8";
	 operation_y = "#1y*.8";
	 operation_z = "#1z*.8";
      };
      XP_COMM_PROJ.Boundingboxes.BoundingboxesMacs.Bounding_boxes Bounding_boxes {
	 in_field => <-.Read_Field.field;
	 in_deformed => <-.coordinate_math.out_fld;
	 Bounding_boxes_func {
	    plane_free {
	       xform {
		  ocenter = {31.5,31.5,0.};
		  dcenter = {38.8138,38.8138,0.};
		  mat = {
		     1.11237,0.0982872,0.520816,0.,0.293383,0.894203,-0.795364,0.,
		     -0.441402,0.842032,0.783851,0.,0.,0.,0.,1.
		  };
		  xlate = {-7.31378,-7.31378,31.5};
		  center = {31.5,31.5,0.};
	       };
	    };
	 };
      };
      MODS.isovolume isovolume {
	 in_field => <-.cut.out_fld;
      };
      MODS.cut cut {
	 in_field => <-.Read_Field.field;
	 in_plane => <-.Bounding_boxes.out_plane;
      };
      GDM.Uviewer3D Uviewer3D {
	 Scene {
	    Top {
	       child_objs => {
	       <-.<-.<-.Bounding_boxes.out_boxes,<-.<-.<-.isovolume.out_obj};
	       Xform {
		  ocenter = {31.5,31.5,31.5};
		  dcenter = {3.87132,3.39941,	4.28114};
		  mat = {
		     0.00858664,-0.0549804,-0.060615,0.,-0.0443845,0.0480758,-0.0498944,
		     0.,0.0687533,0.0379026,-0.0246398,0.,0.,0.,0.,1.
		  };
		  xlate = {-3.87132,-3.39941, -4.28114};
		  center = {31.5,31.5,31.5};
	       };
	    };
	    View {
	       View {
		  back_col = {1.,1.,1.};
	       };
	    };
	 };
      };
   };

};
