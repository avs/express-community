macro DisplayUnitCell<NEhelpContextID=1,NEhelpFile="STM3/DisplayUnitCell.html"> {
   XP_COMM_PROJ.STM3.BASE.MolecularDataType &in<NEx=154.,NEy=165.,NEportLevels={2,0}>;
   float coordinates<NEportLevels={0,1},NEx=154.,NEy=209.>[8][3] => {
      
      ((in.unit_cell.origin[0] + in.unit_cell.side_vectors[1][0]) + in.unit_cell.side_vectors[2][0]),
      ((in.unit_cell.origin[1] + in.unit_cell.side_vectors[1][1]) + in.unit_cell.side_vectors[2][1]),
      ((in.unit_cell.origin[2] + in.unit_cell.side_vectors[1][2]) + in.unit_cell.side_vectors[2][2]),
      (in.unit_cell.origin[0] + in.unit_cell.side_vectors[2][0]),
      (in.unit_cell.origin[1] + in.unit_cell.side_vectors[2][1]),
      (in.unit_cell.origin[2] + in.unit_cell.side_vectors[2][2]),
      ((in.unit_cell.origin[0] + in.unit_cell.side_vectors[0][0]) + in.unit_cell.side_vectors[2][0]),
      ((in.unit_cell.origin[1] + in.unit_cell.side_vectors[0][1]) + in.unit_cell.side_vectors[2][1]),
      ((in.unit_cell.origin[2] + in.unit_cell.side_vectors[0][2]) + in.unit_cell.side_vectors[2][2]),
      (((in.unit_cell.origin[0] + in.unit_cell.side_vectors[0][0]) + in.unit_cell.side_vectors[1][0]) + in.unit_cell.side_vectors[2][0]),
      (((in.unit_cell.origin[1] + in.unit_cell.side_vectors[0][1]) + in.unit_cell.side_vectors[1][1]) + in.unit_cell.side_vectors[2][1]),
      (((in.unit_cell.origin[2] + in.unit_cell.side_vectors[0][2]) + in.unit_cell.side_vectors[1][2]) + in.unit_cell.side_vectors[2][2]),
      (in.unit_cell.origin[0] + in.unit_cell.side_vectors[1][0]),
      (in.unit_cell.origin[1] + in.unit_cell.side_vectors[1][1]),
      (in.unit_cell.origin[2] + in.unit_cell.side_vectors[1][2]),
      in.unit_cell.origin[0],in.unit_cell.origin[1],in.unit_cell.origin[2],
      (in.unit_cell.origin[0] + in.unit_cell.side_vectors[0][0]),
      (in.unit_cell.origin[1] + in.unit_cell.side_vectors[0][1]),
      (in.unit_cell.origin[2] + in.unit_cell.side_vectors[0][2]),
      ((in.unit_cell.origin[0] + in.unit_cell.side_vectors[0][0]) + in.unit_cell.side_vectors[1][0]),
      ((in.unit_cell.origin[1] + in.unit_cell.side_vectors[0][1]) + in.unit_cell.side_vectors[1][1]),
      ((in.unit_cell.origin[2] + in.unit_cell.side_vectors[0][2]) + in.unit_cell.side_vectors[1][2])
   };
   int connectivity<NEportLevels={0,1},NEx=154.,NEy=99.>[] => {
      0,1,2,3,4,5,6,7
   };
   FLD_MAP.hex_mesh hex_mesh<NEx=374.,NEy=275.> {
      coord => <-.coordinates;
      connect => <-.connectivity;
   };
   float white<NEportLevels={0,1},NEx=528.,NEy=132.>[8][3] => init_array(24,1,1);
   FLD_MAP.rgb_data rgb_data<NEx=660.,NEy=220.> {
      in_data => <-.white;
   };
   FLD_MAP.combine_mesh_data combine_mesh_data<NEx=550.,NEy=407.> {
      in_mesh => <-.hex_mesh.out;
      in_nd => <-.rgb_data.out;
   	DataObject {
      	Modes {
         	mode = {0,2,1,0,0};
      	};
			Obj {
				name = "unit cell";
			};
			Props {
				line_aa = 1;
			};
   	};
   };
   link fld<NEportLevels={1,2},NEx=473.,NEy=539.> => .combine_mesh_data.out;
   link obj<NEportLevels={1,2},NEx=693.,NEy=539.> => .combine_mesh_data.obj;
};
