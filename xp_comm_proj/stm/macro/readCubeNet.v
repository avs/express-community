      macro readCubeNet<NEx=418.,NEy=77.> {
         XP_COMM_PROJ.STM.MODULE.Read_G94_cube Read_G94_cube<NEx=451.,NEy=11.> {
            File_Name<NEportLevels={3,0}>;
         };
         FLD_MAP.struct_mesh struct_mesh<NEx=517.,NEy=66.> {
            in_dims => <-.Read_G94_cube.dims;
            coord => <-.Read_G94_cube.geometry;
         };
         FLD_MAP.node_scalar node_scalar<NEx=363.,NEy=66.> {
            in_data => <-.Read_G94_cube.Orbital_Data;
         };
         FLD_MAP.combine_mesh_data combine_mesh_data<NEx=429.,NEy=110.> {
            in_mesh => <-.struct_mesh.out;
            in_nd => <-.node_scalar.out;
         };
         MODS.isosurface isosurface<NEx=352.,NEy=154.> {
            in_field => <-.combine_mesh_data.out;
            IsoParam {
               iso_level<NEdisplayMode="open"> => 0.1;
            };
            obj {
               Datamap {
                  DataRange = {
                     {
                        selectValues=1,UIMaxValue=1.26,,,,,,,,,,,,
                     }};
               };
               Props {
                  material = {0.22,0.15,0.24,
50.};
                  inherit = 0;
                  trans = 0.4900000095;
               };
            };
            out_obj<NEportLevels={1,4}>;
         };
         MODS.bounds bounds<NEx=506.,NEy=154.> {
            in_field => <-.combine_mesh_data.out;
            out_obj<NEportLevels={1,4}>;
            obj {
               Props {
                  line_width = 1;
                  inherit = 0;
               };
            };
         };
         MODS.orthoslice orthoslice<NEx=187.,NEy=154.> {
            in_field => <-.combine_mesh_data.out;
            OrthoSliceParam {
               plane => 17;
            };
            OrthoSliceUI {
               UIpanel {
                  option {
                     set = 1;
                  };
               };
            };
            obj {
               Datamap {
                  DataRange = {
                     {
                        selectValues=1,UIMaxValue=1.26,,,,,,,,,,,,
                     }};
               };
               Obj {
                  chunk = 0;
               };
               Props {
                  material = {0.36,0.36,0.33,
50.};
                  inherit = 0;
               };
            };
            out_obj<NEportLevels={1,4}>;
         };
         MODS.isoline isoline<NEx=242.,NEy=198.> {
            in_field => <-.orthoslice.out_fld;
            IsoParam {
               ncontours<NEdisplayMode="open"> = 7;
               level_min<NEdisplayMode="open"> => 0.;
               level_max<NEdisplayMode="open"> => 0.7;
            };
            obj {
               Datamap {
                  DataRange = {
                     {
                        selectValues=1,UIMaxValue=1.26,,,,,,,,,,,,
                     }};
               };
               Props {
                  jitter = 3;
                  inherit = 0;
                  line_width = 2;
                  subdiv = 14;
               };
            };
            out_obj<NEportLevels={1,4}>;
         };
      };
