

flibrary ReadVtkMacs <compile_subs=0> {

  macro Read_VTK_UI {

      ilink filename;

      UImod_panel panel {
         message = "Select Read VTK control panel.";
         title => name_of(<-.<-.<-);
         parent<NEportLevels={4,0}>;
      };

      UIlabel UCD_Filename {
         parent => <-.panel;
         y = 0;
         width => parent.width;
         alignment = "left";
         label => "Read VTK File Name";
         color {
            backgroundColor = "blue";
            foregroundColor = "white";
         };
      };

      UItext file_name {
         parent => panel;
         y => UCD_Filename.y + UCD_Filename.height + 5;
         text<NEportLevels={2,2}> => <-.filename;
         width = 170;
         showLastPosition = 1;
      };

      UIbutton visible {
         parent => panel;
         x => file_name.x + file_name.width + 5;
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
         visible => <-.visible.do;
         title => "Read VTK Filename";
         searchPattern = "*.vtk";
         filename<NEportLevels={2,2}> => <-.filename;
      };

  };



  macro Read_VTK {

    ilink filename;

    XP_COMM_PROJ.ReadVtk.ReadVtkMods.ReadVtkCore ReadVtkCore {
      filename => <-.filename;
    };

    FLD_MAP.tri_mesh tri_mesh {
      coord => ReadVtkCore.coords;
      connect => ReadVtkCore.connect;
      DataObject {
         Obj {
            name => name_of(<-.<-.<-.<-);
         };
      };
    };

    olink out_mesh => .tri_mesh.out;
    olink out_obj => .tri_mesh.obj;



    // In a typical application the node_data_values array below should be replaced
    // with real node data.
    // If node data is present in your VTK files then it should be possible to modify
    // the ReadVtkCore so that it reads and outputs that node data.

    //byte+OPort node_data_values[.tri_mesh.out.nnodes] => init_array(.tri_mesh.out.nnodes,128,128);
    byte+OPort node_data_values[.tri_mesh.out.nnodes] => init_array(.tri_mesh.out.nnodes,0,255);

    FLD_MAP.node_scalar node_scalar {
      in_data[<-.tri_mesh.out.nnodes] => <-.node_data_values;
    };

    FLD_MAP.combine_mesh_data combine_mesh_data {
      in_mesh => <-.tri_mesh.out;
      in_nd => <-.node_scalar.out;
    };

    olink out_fld => .combine_mesh_data.out;
  };



  macro read_vtk {

    string filename<NEportLevels={2,1}>;

    Read_VTK_UI Read_VTK_UI {
      filename => <-.filename;
    };

    Read_VTK Read_VTK {
      filename => <-.filename;
      tri_mesh {
         DataObject {
            Obj {
               name => name_of(<-.<-.<-.<-.<-);
            };
         };
      };
    };

    olink out_mesh => .Read_VTK.out_mesh;
    olink out_obj =>  .Read_VTK.out_obj;
    olink out_fld =>  .Read_VTK.out_fld;
  };



  APPS.SingleWindowApp ReadVtkEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.BoundsDataObject.obj,<-.<-.<-.DataObject.obj
               };
            };
            Camera {
               Camera {
                  perspec = 1;
                  front = 1.;
               };
            };
         };
      };


      XP_COMM_PROJ.ReadVtk.ReadVtkMacs.read_vtk read_vtk {
         filename = "./xp_comm_proj/rd_vtk/ear.vtk";
      };

      GDM.DataObject BoundsDataObject {
         in => <-.read_vtk.out_mesh;
         Modes {
            mode = {1,1,1,1,2};
         };
      };

      GDM.DataObject DataObject {
         in => <-.read_vtk.out_fld;
      };
  };

};

