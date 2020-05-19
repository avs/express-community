
flibrary CleanExtEdgesMacs <compile_subs=0> {

   macro CleanEdges {
      Mesh &in<NEportLevels={2,1}> {
         int+opt nnode_data;
         Data_Array+opt node_data[.nnode_data] {
            nvals => <-.nnodes;
         };
         xform+nonotify;
      };

      int &outer_edges<NEportLevels={2,1}> = 0;

      XP_COMM_PROJ.CleanExtEdges.CleanExtEdgesMods.CleanEdgesCore CleanEdgesCore {
         in => <-.in;
         outer_edges => <-.outer_edges;
      };

      Line+OPort Line {
         ncells+IPort2 => <-.CleanEdgesCore.nlines_out;
         node_connect_list+IPort2 => <-.CleanEdgesCore.lines_out;
      };
      group+OPort fix_cells {
         int ncell_sets = 1;
         group+IPort2 &cell_set[.ncell_sets] => {<-.Line};
      };
      group+OPort2 &out_fld => 
         switch( (CleanEdgesCore.nlines_out > 0), merge(.fix_cells,.in));

      GDM.DataObject obj {
         in => switch( (CleanEdgesCore.nlines_out > 0), out_fld);
      };
      olink out_obj => obj.obj;
   };



   APPS.SingleWindowApp CleanEdgesEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.CleanEdges.out_obj};
            };
         };
      };


      MODS.Read_UCD Read_UCD {
         read_ucd_ui {
            file_browser {
               dirMaskCache = "$XP_ROOT/data/ucd/*.inp";
            };
            filename = "$XP_ROOT/data/ucd/cells.inp";
         };
      };

      MODS.select_cells select_cells {
         in_field => <-.Read_UCD.field;
         sets = {0,1,2,4,5,6,7,8,9,10};
      };

      MODS.external_edges external_edges {
         in_field => <-.select_cells.out_fld;
         edge_angle = 0.1;
      };

      XP_COMM_PROJ.CleanExtEdges.CleanExtEdgesMacs.CleanEdges CleanEdges {
         in => <-.external_edges.out_fld;
      };
   };

};

