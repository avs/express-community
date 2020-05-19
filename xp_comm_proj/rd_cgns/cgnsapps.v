
//
// This library contains example application networks using the modules
//

flibrary ReadCgnsApps<compile_subs=0> {

   APPS.SingleWindowApp ReadCgnsUnstructuredEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.extract_cell_component_ARR.out_obj
               };
            };
         };
      };

      XP_COMM_PROJ.ReadCgns.ReadCgnsMacs.read_cgns read_cgns {
         CgnsParams {
            filename = "$XP_PATH<1>/xp_comm_proj/rd_cgns/data/tut21.cgns";
         };
      };

      MODS_ARR.extract_cell_component_ARR extract_cell_component_ARR {
         in_field => <-.read_cgns.unstr_fields;
      };
   };



   APPS.SingleWindowApp ReadCgnsMultiBlockEg {
      GDM.Uviewer3D Uviewer3D {
         Scene {
            Top {
               child_objs => {
                  <-.<-.<-.external_faces_ARR.out_obj
               };
            };
         };
      };


      XP_COMM_PROJ.ReadCgns.ReadCgnsMacs.read_cgns read_cgns {
         CgnsParams {
            filename = "$XP_PATH<1>/xp_comm_proj/rd_cgns/data/5blocks.cgns";
         };
      };

      MODS_ARR.mblock_to_fields mblock_to_fields {
         multi_block => <-.read_cgns.Multi_Block;
      };

      MODS_ARR.external_faces_ARR external_faces_ARR {
         in_field => <-.mblock_to_fields.fields;
      };
   };



	library TestCases {
		APPS.SingleWindowApp TstMixed {
			GDM.Uviewer3D Uviewer3D {
				Scene {
					Top {
						child_objs => {
							<-.<-.<-.DataObject_str.obj,
							<-.<-.<-.DataObject_unstr.obj
					   };
					};
				};
			};

			XP_COMM_PROJ.ReadCgns.ReadCgnsMacs.read_cgns read_cgns {
				CgnsParams {
					filename = "$XP_PATH<1>/xp_comm_proj/rd_cgns/data/tst-mixed.cgns";
				};
			};

			MODS_ARR.mblock_to_fields mblock_to_fields {
				multi_block => <-.read_cgns.Multi_Block;
			};

			link+Port structured_first_field => mblock_to_fields.fields[0];
			GDM.DataObject DataObject_str {
				in => <-.structured_first_field;
			};

			link+Port unstructured_first_field => read_cgns.unstr_fields[0];
			GDM.DataObject DataObject_unstr {
				in => <-.unstructured_first_field;
			};
		};


		APPS.SingleWindowApp TstNGON {
			GDM.Uviewer3D Uviewer3D {
				Scene {
					Top {
						child_objs => {
							<-.<-.<-.DataObject.obj
					   };
					};
				};
			};

			XP_COMM_PROJ.ReadCgns.ReadCgnsMacs.read_cgns read_cgns {
				CgnsParams {
					filename = "$XP_PATH<1>/xp_comm_proj/rd_cgns/data/tst-ngon.cgns";
				};
			};

			link+Port unstructured_first_field => read_cgns.unstr_fields[0];
			GDM.DataObject DataObject {
				in => <-.unstructured_first_field;
				Modes {
					mode = {0,2,0,0,0};
				};
			};
		};


		APPS.SingleWindowApp TstRind {
			GDM.Uviewer3D Uviewer3D {
				Scene {
					Top {
						child_objs => <-.<-.<-.DataObjects.dos.obj;
					};
				};
			};

			XP_COMM_PROJ.ReadCgns.ReadCgnsMacs.read_cgns read_cgns {
				CgnsParams {
					filename = "$XP_PATH<1>/xp_comm_proj/rd_cgns/data/tst-rind.cgns";
				};
			};

			MODS_ARR.mblock_to_fields mblock_to_fields {
				multi_block => <-.read_cgns.Multi_Block;
			};

         macro DataObjects {
            imlink in_fields => <-.mblock_to_fields.fields;
            GDM.DataObject dos[array_size(in_fields)] {
               in+nres => in_fields[index_of(dos)];
               obj+OPort3;
            };
         };
		};


		APPS.SingleWindowApp TstUnsupported {
			GDM.Uviewer3D Uviewer3D {
				Scene {
					Top {
						child_objs => <-.<-.<-.DataObjects.dos.obj;
					};
				};
			};

			XP_COMM_PROJ.ReadCgns.ReadCgnsMacs.read_cgns read_cgns {
				CgnsParams {
					filename = "$XP_PATH<1>/xp_comm_proj/rd_cgns/data/tst-unsupp.cgns";
				};
			};

			MODS_ARR.mblock_to_fields mblock_to_fields {
				multi_block => <-.read_cgns.Multi_Block;
			};

         macro DataObjects {
            imlink in_fields => <-.mblock_to_fields.fields;
            GDM.DataObject dos[array_size(in_fields)] {
               in+nres => in_fields[index_of(dos)];
               obj+OPort3;
            };
         };
		};


		APPS.SingleWindowApp TstCellTypes {
			GDM.Uviewer3D Uviewer3D {
				Scene {
					Top {
						child_objs => {
						   <-.<-.<-.DataObject.obj
						};
					};
				};
			};

			XP_COMM_PROJ.ReadCgns.ReadCgnsMacs.read_cgns read_cgns {
				CgnsParams {
					filename = "$XP_PATH<1>/xp_comm_proj/rd_cgns/data/tst-cells.cgns";
				};
			};

			link+Port unstructured_first_field => read_cgns.unstr_fields[0];
			GDM.DataObject DataObject {
				in => <-.unstructured_first_field;
				Modes {
					mode = {0,2,0,0,0};
				};
			};
		};


/*
		APPS.SingleWindowApp Tst1DScndOrderCells {
			GDM.Uviewer3D Uviewer3D {
				Scene {
					Top {
						child_objs => {
						   <-.<-.<-.isoline.out_obj,
						   <-.<-.<-.extract_mesh.out_obj
						};
					};
				};
			};

			XP_COMM_PROJ.ReadCgns.ReadCgnsMacs.read_cgns read_cgns {
				CgnsParams {
					filename = "$XP_PATH<1>/xp_comm_proj/rd_cgns/data/tst-1D-scnd_order.cgns";
				};
			};

			link+Port unstructured_first_field => read_cgns.unstr_fields[0];

			MODS.isoline isoline {
				in_field => <-.unstructured_first_field;
				IsoParam {
					ncontours = 30;
					level_min => 3.87;
					level_max => 22.65;
				};
				obj {
					Props {
						line_width = 2;
						inherit = 0;
					};
				};
			};

			MODS.extract_mesh extract_mesh {
				in_field => <-.unstructured_first_field;
				obj {
					Props {
						col = {0.608329,0.608329,0.608329};
						inherit = 0;
					};
					Modes {
						mode = {0,2,1,0,0};
					};
				};
			};
		};
*/


		APPS.SingleWindowApp Tst2DScndOrderCells {
			GDM.Uviewer3D Uviewer3D {
				Scene {
					Top {
						child_objs => {
						   <-.<-.<-.isoline.out_obj,
						   <-.<-.<-.extract_mesh.out_obj
						};
					};
				};
			};

			XP_COMM_PROJ.ReadCgns.ReadCgnsMacs.read_cgns read_cgns {
				CgnsParams {
					filename = "$XP_PATH<1>/xp_comm_proj/rd_cgns/data/tst-2D-scnd_order.cgns";
				};
			};

			link+Port unstructured_first_field => read_cgns.unstr_fields[0];

			MODS.isoline isoline {
				in_field => <-.unstructured_first_field;
				IsoParam {
					ncontours = 30;
					level_min => 3.87;
					level_max => 22.65;
				};
				obj {
					Props {
						line_width = 2;
						inherit = 0;
					};
				};
			};

			MODS.extract_mesh extract_mesh {
				in_field => <-.unstructured_first_field;
				obj {
					Props {
						col = {0.608329,0.608329,0.608329};
						inherit = 0;
					};
					Modes {
						mode = {0,2,1,0,0};
					};
				};
			};
		};


		APPS.SingleWindowApp Tst3DScndOrderCells {
			GDM.Uviewer3D Uviewer3D {
				Scene {
					Top {
						child_objs => {
						   <-.<-.<-.isosurface.out_obj,
						   <-.<-.<-.extract_mesh.out_obj
						};
					};
				};
			};

			XP_COMM_PROJ.ReadCgns.ReadCgnsMacs.read_cgns read_cgns {
				CgnsParams {
					filename = "$XP_PATH<1>/xp_comm_proj/rd_cgns/data/tst-3D-scnd_order.cgns";
				};
			};

			link+Port unstructured_first_field => read_cgns.unstr_fields[0];

			MODS.isosurface isosurface {
				in_field => <-.unstructured_first_field;
				IsoParam {
					iso_level => 73.01;
				};
				obj {
					Props {
						line_width = 2;
						inherit = 0;
					};
				};
			};

			MODS.extract_mesh extract_mesh {
				in_field => <-.unstructured_first_field;
				obj {
					Modes {
						mode = {0,2,1,0,0};
					};
				};
			};
		};

	};

};

