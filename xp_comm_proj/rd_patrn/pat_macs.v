
flibrary ReadPatranMacs <compile_subs=0>  {



   macro ReadPatranUI {

      XP_COMM_PROJ.ReadPatran.ReadPatranMods.ReadPatranParams &ReadPatranParams<NEportLevels={2,1}>;
      XP_COMM_PROJ.ReadPatran.ReadPatranMods.neutral_output &neutral_output<NEportLevels={2,1}>;

      UImod_panel parent {
         width = 500;
         title => "Read Patran Neutral File";
         clientWidth = 500;
      };


      UIfileDialog PatranFileDialog {
         parent => <-.parent;
         title = "Read Patran Filename";
         visible => <-.BrowseButton.do;
         searchPattern = "*.*";
         filename => <-.ReadPatranParams.filename;
      };
      UIbutton BrowseButton {
         parent => <-.parent;
         label => "Browse...";
         width = 80;
      };


      UItext Title1Text {
         parent => <-.parent;
         text<NEportLevels={2,2}> =>  <-.neutral_output.title1;
         width = 240;
         outputOnly = 1;
      };
      UItext Title2Text {
         parent => <-.parent;
         text<NEportLevels={2,2}> =>  <-.neutral_output.title2;
         width = 240;
         outputOnly = 1;
      };
      UItoggle ShowPointsToggle {
         parent => <-.parent;
         label => "Display Points";
         width = <-.parent.width;
         set => <-.ReadPatranParams.points_visible;
      };
      UItoggle ShowLineToggle {
         parent => <-.parent;
         label => "Display Lines";
         width = <-.parent.width;
         active => <-.neutral_output.n_bar;
         set => <-.ReadPatranParams.line_visible;
      };
      UItoggle ShowTriToggle {
         parent => <-.parent;
         label => "Display Triangles";
         width = <-.parent.width;
         active => <-.neutral_output.n_tri;
         set => <-.ReadPatranParams.tri_visible;
      };
      UItoggle ShowQuadsToggle {
         parent => <-.parent;
         label => "Display Quads";
         width = <-.parent.width;
         active => <-.neutral_output.n_quad;
         set => <-.ReadPatranParams.quad_visible;
      };
      UItoggle ShowTetToggle {
         parent => <-.parent;
         label => "Display Tetrahedron";
         width = <-.parent.width;
         active => <-.neutral_output.n_tet;
         set => <-.ReadPatranParams.tet_visible;
      };
      UItoggle ShowPatchToggle {
         parent => <-.parent;
         label => "Approximate patches";
         width = <-.parent.width;
         active => <-.neutral_output.n_patch;
         set => <-.ReadPatranParams.patch_visible;
      };
   };



   macro ReadPatran {
      XP_COMM_PROJ.ReadPatran.ReadPatranMods.ReadPatranParams &ReadPatranParams<NEportLevels={2,1}>;

      XP_COMM_PROJ.ReadPatran.ReadPatranMods.ReadPatranCore ReadPatranCore {
         ReadPatranParams => <-.ReadPatranParams;
      };

      FLD_MAP.point_mesh point_mesh {
         coord => <-.ReadPatranCore.neutral_output.xyz;
         DataObject {
            Obj {
               visible => ReadPatranParams.points_visible;
            };
         };
      };
      FLD_MAP.line_mesh line_mesh {
         coord => <-.ReadPatranCore.neutral_output.xyz;
         connect => <-.ReadPatranCore.neutral_output.bar_conn;
         DataObject {
            Obj {
               visible => (ReadPatranCore.neutral_output.n_bar && ReadPatranParams.line_visible);
            };
         };
      };
      FLD_MAP.tri_mesh tri_mesh {
         coord => <-.ReadPatranCore.neutral_output.xyz;
         connect => <-.ReadPatranCore.neutral_output.tri_conn;
         DataObject {
            Obj {
               visible => (ReadPatranCore.neutral_output.n_tri && ReadPatranParams.tri_visible);
            };
         };
      };
      FLD_MAP.quad_mesh quad_mesh {
         coord => <-.ReadPatranCore.neutral_output.xyz;
         connect => <-.ReadPatranCore.neutral_output.quad_conn;
         DataObject {
            Obj {
               visible => (ReadPatranCore.neutral_output.n_quad && ReadPatranParams.quad_visible);
            };
         };
      };
      FLD_MAP.tet_mesh tet_mesh {
         coord => <-.ReadPatranCore.neutral_output.xyz;
         connect => <-.ReadPatranCore.neutral_output.tet_conn;
         GDM.DataObject DataObject {
            in => <-.out;
            Obj {
               name => name_of(<-.<-.<-);
               visible => (ReadPatranCore.neutral_output.n_tet && ReadPatranParams.tet_visible);
            };
    	   };
    	   olink obj => DataObject.obj;
      };
      FLD_MAP.quad_mesh patch_quad_mesh {
         coord => <-.ReadPatranCore.neutral_output.xyz;
         connect => <-.ReadPatranCore.neutral_output.patch;
         DataObject {
            Obj {
               visible => (ReadPatranCore.neutral_output.n_patch && ReadPatranParams.patch_visible);
            };
         };
      };


      Mesh+Port &mesh[] => {
         .point_mesh.out,.line_mesh.out,.tri_mesh.out,
         .quad_mesh.out,.tet_mesh.out,.patch_quad_mesh.out
      };


      GDM.GroupObject GroupObject {
         child_objs => {
            <-.point_mesh.obj,<-.line_mesh.obj,<-.tri_mesh.obj,
            <-.quad_mesh.obj,<-.tet_mesh.obj,<-.patch_quad_mesh.obj
         };
      };

      XP_COMM_PROJ.ReadPatran.ReadPatranMods.neutral_output &neutral_output<NEportLevels={1,2}> => .ReadPatranCore.neutral_output;

      olink out_mesh => mesh;
      olink out_obj => GroupObject.obj;
   };




   macro read_patran {

      XP_COMM_PROJ.ReadPatran.ReadPatranMods.ReadPatranParams ReadPatranParams;

      ReadPatran ReadPatran {
         ReadPatranParams => <-.ReadPatranParams;
      };

      ReadPatranUI ReadPatranUI {
         ReadPatranParams => <-.ReadPatranParams;
         neutral_output => <-.ReadPatran.neutral_output;
      };


      olink out_mesh => ReadPatran.out_mesh;
      olink out_obj => ReadPatran.out_obj;
   };




/*
   APPS.SingleWindowApp ReadPatranEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {<-.<-.<-.read_patran.out_obj};
            };
         };
      };

      read_patran read_patran {
         ReadPatran {
            tet_mesh {
               DataObject {
                  Props {
                     //inherit = 0;
                     cull = "Flip Normals";
                  };
               };
            };
         };
      };

   };
*/


APPS.MultiWindowApp ReadPatranEg {

   GDM.Uviewer3D BasicUviewer3D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.read_patran.out_obj
            };
            Xform {
               ocenter = {10.,7.5,2.5};
               dcenter = {4.,3.,1.};
               mat = {
                  0.310646,0.0426245,-0.0442156,0.,
                  -0.0348177,0.310028,0.0542527,0.,
                  0.0505926,-0.0483609,0.308828,0.,
                  0.,0.,0.,1.
               };
               xlate = {-4.,-3.,-1.};
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
   GDM.Uviewer3D VizUviewer3D {
      Scene {
         Top {
            child_objs => {
               <-.<-.<-.external_edges.out_obj,<-.<-.<-.shrink_cells.out_obj
            };
            Xform {
               ocenter = {10.,7.5,2.5};
               dcenter = {4.,3.,1.};
               mat = {
                  0.310646,0.0426245,-0.0442156,0.,
                  -0.0348177,0.310028,0.0542527,0.,
                  0.0505926,-0.0483609,0.308828,0.,
                  0.,0.,0.,1.
               };
               xlate = {-4.,-3.,-1.};
            };
         };
         Lights {
            Lights = {
               {
                  type="BiDirectional",,
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

   XP_COMM_PROJ.ReadPatran.ReadPatranMacs.read_patran read_patran {
      ReadPatranParams {
         tet_visible = 1;
         tri_visible = 0;
	 filename  = "$XP_PATH<1>/xp_comm_proj/rd_patrn/data/ex01.out";
      };
      ReadPatranUI {
         PatranFileDialog {
            searchPattern = "$XP_PATH<1>/xp_comm_proj/rd_patrn/data/*";
         };
      };
   };
   MODS.external_edges external_edges {
      in_field => <-.read_patran.out_mesh[2];
      edge_angle = 0.01;
   };
   MODS.shrink_cells shrink_cells {
      in_field => <-.read_patran.out_mesh[4];
      ShrinkParam {
         scale = 0.5;
      };
   };

};

}; // end of library

