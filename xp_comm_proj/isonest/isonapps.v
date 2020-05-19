
flibrary IsonestApps <compile_subs=0> {


APPS.SingleWindowApp IsonestTestNoUI{
   UI {
      shell {
         x = 65;
         y = 36;
      };
      Editors {
         IUI {
            optionList {
               selectedItem = 0;
            };
         };
      };
      Modules {
         IUI {
            optionList {
               selectedItem = 1;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.IsonestFunc.obj,<-.<-.<-.bounds.out_obj};
            Props {
               material = {0.3,0.7,0.8,
12.};
            };
            Xform {
               ocenter = {31.5,32.0235,31.5};
               dcenter = {3.96792,4.0823,
4.08214};
               mat = {
                  0.114043,0.069019,0.0589554,0.,0.00503164,-0.09933,0.106552,
0.,0.0906316,-0.0813333,-0.0801001,0.,0.,0.,0.,1.
               };
               xlate = {-3.96792,-4.0823,
-4.08214};
            };
         };
         Lights {
            LightXform = {
               {mat=
                  {
                     1.36416,0.392224,-1.1742,0.,0.890771,0.902386,1.33632,0.,
0.859709,-1.55736,0.478582,0.,0.,0.,0.,1.
                  },},
               ,,};
         };
         Camera {
            Camera {
               perspec = 1;
               front = 1.;
               auto_norm = "None";
            };
         };
      };
   };
   MODS.Read_Volume Read_Volume {
      read_volume_ui {
         filename = "$XP_PATH<0>/data/volume/hydrogen.dat";
      };
   };
   MODS.downsize downsize {
      in_field => <-.Read_Volume.field;
      DownsizeParam {
         factor0 = 2.;
         factor1 = 2.;
         factor2 = 2.;
      };
   };
   XP_COMM_PROJ.Isonest.IsonestMods.IsonestParams isonest_params {
         num_levels = 5;
         min = 40.;
         max = 200.;
      };
   XP_COMM_PROJ.Isonest.IsonestMacs.IsonestFunc IsonestFunc {
      fld_in => <-.downsize.out_fld;
      params => <-.isonest_params;
   };
   MODS.bounds bounds {
      in_field => <-.downsize.out_fld;
   };
};

APPS.SingleWindowApp IsonestEx1 {
   GDM.Uviewer3D Uviewer3D {
      Scene {
         Top {
            child_objs => { <-.<-.<-.isosurface_nesting.obj,<-.<-.<-.bounds.out_obj};
         };
         Lights {
            LightXform = {
               {mat=
                  {
                     1.36416,0.392224,-1.1742,0.,0.890771,0.902386,1.33632,0.,
0.859709,-1.55736,0.478582,0.,0.,0.,0.,1.
                  },},
               ,,};
         };
         Camera {
            Camera {
               perspec = 1;
               front = 1.;
            };
         };
      };
   };
   MODS.Read_Volume Read_Volume {
      read_volume_ui {
         filename = "$XP_PATH<0>/data/volume/hydrogen.dat";
      };
   };
   MODS.downsize downsize {
      in_field => <-.Read_Volume.field;
      DownsizeParam {
         factor0 = 2.;
         factor1 = 2.;
         factor2 = 2.;
      };
   };
   XP_COMM_PROJ.Isonest.IsonestMacs.isosurface_nesting isosurface_nesting {
      in_field => <-.downsize.out_fld;
      isonest_params {
         num_levels = 5;
         min = 40.;
         max = 200.;
      };
   };
   MODS.bounds bounds {
      in_field => <-.downsize.out_fld;
   };
};

APPS.MultiWindowApp IsonestEx2 {
   UI {
      Modules {
         IUI {
            optionList {
               selectedItem = 2;
            };
         };
      };
   };
   GDM.Uviewer3D Uviewer3D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.bounds.out_obj,<-.<-.<-.isosurface_nesting.obj};
            Xform {
               ocenter = {12.,12.,15.5};
               dcenter = {4.,4.,5.16667};
               mat = {
                  0.406227,-0.048866,0.19949,0.,0.205385,0.0990491,-0.393968,0.,
-0.00111524,0.441595,0.110442,0.,0.,0.,0.,1.
               };
               xlate = {-5.30206,-2.69794,
-5.16667};
               center = {12.,12.,15.5};
            };
         };
         Lights {
            LightXform = {
               {mat=
                  {
                     0.178717,-0.635145,0.751433,0.,0.632333,0.659264,0.40685,
0.,-0.753801,0.402445,0.519445,0.,0.,0.,0.,1.
                  },},
               ,,};
         };
         Camera {
            Camera {
               auto_norm = "None";
               perspec = 1;
               front = 1.;
            };
         };
         View {
            ViewUI {
               ViewPanel {
                  UI {
                     panel {
                        defaultX = 365;
                        defaultY = 86;
                     };
                  };
               };
            };
         };
      };
   };
   MODS.Read_Field Read_Field {
      read_field_ui {
         filename = "$XP_PATH<0>\\data\\field\\water.fld";
      };
   };
   MODS.bounds bounds {
      in_field => <-.Read_Field.field;
   };
   XP_COMM_PROJ.Isonest.IsonestMacs.isosurface_nesting isosurface_nesting {
      in_field => <-.Read_Field.field;
      isonest_params {
            num_levels = 6;
            min => 0.06;
            max => 0.99;
            transparency = 0.4;
            ramp_delta = 0.5;
      };
   };
};

APPS.MultiWindowApp IsonestEx3 {
   GDM.Uviewer3D Uviewer3D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.external_edges.out_obj,<-.<-.<-.isosurface_nesting.obj};
            Props {
               material = {0.3,0.7,0.79,
14.1};
            };
            Xform {
               ocenter = {0.,3.875,-0.5};
               dcenter = {0.,4.,-0.516129};
               mat = {
                  1.13501,-0.288322,0.456669,0.,0.0150191,1.07928,0.644085,0.,
-0.539862,-0.576143,0.978022,0.,0.,0.,0.,1.
               };
               xlate = {0.,-4.,0.516129};
            };
         };
         Lights {
            LightXform = {
               {mat=
                  {
                     0.751136,0.0860365,0.654517,0.,0.026499,0.986742,-0.160118,
0.,-0.659616,0.137615,0.738898,0.,0.,0.,0.,1.
                  },},
               ,,};
         };
         Camera {
            Camera {
               auto_norm = "None";
            };
         };
         View {
            ViewUI {
               ViewPanel {
                  UI {
                     panel {
                        defaultX = 399;
                        defaultY = 8;
                     };
                  };
               };
            };
         };
      };
   };
   MODS.Read_UCD Read_UCD {
      read_ucd_ui {
         filename = "$XP_PATH<0>\\MicroAVS\\data\\UCD\\fork.inp";
      };
   };
   MODS.external_edges external_edges {
      in_field => <-.Read_UCD.field;
      edge_angle = 32.0;
   };
   XP_COMM_PROJ.Isonest.IsonestMacs.isosurface_nesting isosurface_nesting {
      in_field => <-.Read_UCD.field;
      isonest_params {
            num_levels = 6;
            min => 128606.02;
      };
	  IsonestUI{
         UIpanel {
           option {
              set = 1;
           };
         };
	  };
   };
};
}; // end of lib

