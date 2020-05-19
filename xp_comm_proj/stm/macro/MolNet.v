      macro MolNet<NExOffset=507.8478088,NEyOffset=-20.84560585,NEscalingFactor=0.9295594096,NEx=220.,NEy=253.> {
         macro atomic_positions<NEx=121.,NEy=308.,NEwidth=484.,NEheight=198.> {
            FLD_MAP.point_mesh atomic_point_mesh<NEx=0.,NEy=22.> {
               coord<NEportLevels={4,0}>;
               out<NEportLevels={0,3}>;
            };
            FLD_MAP.radius_data atomic_radii<NEx=165.,NEy=22.> {
               in_data<NEportLevels={4,0}>;
            };
            FLD_MAP.node_colors atomic_mesh_colors<NEx=330.,NEy=33.> {
               in_data<NEportLevels={4,0}>;
            };
            FLD_MAP.combine_node_datas combine_node_datas<NEx=209.,NEy=77.> {
               in => {
                  <-.atomic_mesh_colors.out,<-.atomic_radii.out};
            };
            FLD_MAP.combine_mesh_data atomic_field<NEx=88.,NEy=132.> {
               in_mesh => <-.atomic_point_mesh.out;
               in_nd => <-.combine_node_datas.out;
               obj<NEportLevels={1,5}>;
               DataObject {
                  Props {
                     line_width = 2;
                     inherit = 0;
                     subdiv = 11;
                     material = {0.5,0.26,0.55,
50.};
                  };
               };
            };
         };
         macro atomic_bonds<NEx=-55.,NEy=484.,NEwidth=330.,NEheight=154.> {
            FLD_MAP.line_mesh bond_mesh<NEx=0.,NEy=11.> {
               coord<NEportLevels={4,0},NEdisplayMode="open">;
               connect<NEportLevels={4,0}>;
               out {
                  xform {
                     mat = {
                        0.9921,0.,0.,0.,0.,0.9921,0.,0.,0.,0.,0.9921,0.,
0.,0.,0.,1.
                     };
                  };
               };
            };
            FLD_MAP.node_colors bond_colors<NEx=154.,NEy=11.> {
               in_data<NEportLevels={4,0},NEdisplayMode="open">;
            };
            FLD_MAP.combine_mesh_data bond_field<NEx=77.,NEy=77.> {
               in_mesh => <-.bond_mesh.out;
               in_nd => <-.bond_colors.out;
               obj<NEportLevels={1,5}>;
               DataObject {
                  Props {
                     line_width = 6;
                     inherit = 0;
                     line_aa = 1;
                  };
               };
            };
         };
         MODS.text_glyph residue_name_text<NEx=110.,NEy=99.> {
            in_mesh => <-.atomic_positions.atomic_point_mesh.out;
            in_text<NEportLevels={3,0}>;
            UImod_panel {
               clientHeight = 1500;
               parent<NEportLevels={2,0}>;
               option {
                  set = 1;
               };
            };
            TextUI<NEscalingFactor=0.6597014666,NExOffset=221.809967,NEyOffset=-22.> {
               Font {
                  text = "-adobe-times-*-r-*-*-18-*-*-*-*-*-*-*";
               };
               AlignHorizMenu<NEx=660.,NEy=594.> {
                  x = 0;
                  selectedItem<NEdisplayMode="open"> = 1;
               };
               UIoption {
                  set = 0;
               };
               UIoption#1 {
                  set = 1;
               };
               AlignVertMenu<NEx=352.,NEy=594.> {
                  x = 0;
                  selectedItem = 2;
                  alignment<NEdisplayMode="open">;
               };
               UIoption#3 {
                  set = 0;
               };
               UIoption#5 {
                  set = 1;
               };
               Offset {
                  set = 1;
               };
               OffsetXValue {
                  fval => 0.03;
                  field {
                     value = 0.01;
                  };
               };
               OffsetYValue {
                  fval => 0.01;
                  field {
                     value = -0.01;
                  };
               };
               UPpath<NEx=517.,NEy=440.>;
            };
            TextValues<NEx=297.,NEy=473.> {
               offset<NEdisplayMode="open">;
            };
            obj {
               Props {
                  col<NEportLevels={0,0}> = {1.,0.,1.};
                  font<NEdisplayMode="open">;
               };
               AltObject<NEx=440.,NEy=242.>;
            };
            out_obj<NEportLevels={1,4}>;
         };
         MODS.text_glyph atom_name_text<NEx=0.,NEy=176.> {
            in_mesh => <-.atomic_positions.atomic_point_mesh.out;
            in_text<NEportLevels={3,0}>;
            TextUI {
               Font {
                  text = "-adobe-times-bold-r-*-*-18-*-*-*-*-*-*-*";
               };
               AlignHorizMenu {
                  x = 0;
               };
               AlignVertMenu {
                  x = 0;
                  selectedItem = 2;
               };
               UIoption#3 {
                  set = 0;
               };
               UIoption#5 {
                  set = 1;
               };
               Offset {
                  set = 1;
               };
               OffsetXValue {
                  field {
                     value = 0.01;
                  };
               };
               OffsetYValue {
                  field {
                     value = 0.06;
                  };
               };
            };
            out_obj<NEportLevels={1,4}>;
         };
      };
