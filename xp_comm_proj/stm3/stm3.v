flibrary STM3<
#ifdef MSDOS
link_files="xp_comm_proj/stm3/lib/libstm3.lib"
#else
link_files="-L${XP_PATH1}/xp_comm_proj/stm3/lib -lstm3"
#endif
> {
    "../xp_comm_proj/stm3/stmlibs.v" STM3libs;	

	flibrary SUPPORT {
		$include xp_comm_proj/stm3/tube/tubes_mod.v
		$include xp_comm_proj/stm3/tube/MakeTubes.v
	};

	flibrary BASE {
		$include xp_comm_proj/stm3/base/MolecularDataType.v
		$include xp_comm_proj/stm3/base/DisplayParams.v
		
		$include xp_comm_proj/stm3/base/ReadMol_mod.v
		$include xp_comm_proj/stm3/base/ReadAnimMol_mod.v
		$include xp_comm_proj/stm3/base/ComputeBonds_mod.v
		$include xp_comm_proj/stm3/base/DrawAtoms_mod.v
		$include xp_comm_proj/stm3/base/SummaryResidueNames_mod.v
		$include xp_comm_proj/stm3/base/DrawBonds_mod.v
		$include xp_comm_proj/stm3/base/ReadGaussianCube_mod.v
		$include xp_comm_proj/stm3/base/ColorLegendCore_mod.v
		
		$include xp_comm_proj/stm3/base/SyncDir.v
		$include xp_comm_proj/stm3/base/ReadMolecule.v
		$include xp_comm_proj/stm3/base/ReadAnimMolecule.v
		$include xp_comm_proj/stm3/base/ReadGaussianCube.v
		$include xp_comm_proj/stm3/base/DisplayMolecule.v
		$include xp_comm_proj/stm3/base/RecomputeBonds.v
		$include xp_comm_proj/stm3/base/ColorLegend.v
		
		$include xp_comm_proj/stm3/base/MolViewer.v
		ANIM_MODS.image_capture make_movie<NEhelpContextID=1,NEhelpFile="STM3/make_movie.html"> {
			UImod_panel {
				title => name_of(<-.<-,1);
			};
			imcapUI {
				ImageCapControls {
					Memory {
						set = 1;
					};
				};
			};
			imcapCompute {
				ImageCap {
					LInputFrameBuffer<NEportLevels={2,0}>;
					LFrameBuffer<NEportLevels={0,2}>;
				};
			};
		};
	}; 

	flibrary UTILS {
		$include xp_comm_proj/stm3/utils/SelectAtoms_mod.v
		$include xp_comm_proj/stm3/utils/Select_Atoms.v
		$include xp_comm_proj/stm3/utils/SelectResidues_mod.v
		$include xp_comm_proj/stm3/utils/Select_Residues.v
		$include xp_comm_proj/stm3/utils/AddBonds.v
		$include xp_comm_proj/stm3/utils/CutSurface.v
		$include xp_comm_proj/stm3/utils/BicolorSurf.v
		$include xp_comm_proj/stm3/utils/CubeOnSurface.v
		$include xp_comm_proj/stm3/utils/CountBonds_mod.v
		$include xp_comm_proj/stm3/utils/CropAtoms_mod.v
		$include xp_comm_proj/stm3/utils/Crop_Atoms.v
		$include xp_comm_proj/stm3/utils/Picker_mod.v
		$include xp_comm_proj/stm3/utils/PickAtoms.v
		$include xp_comm_proj/stm3/utils/WritePDB_mod.v
		$include xp_comm_proj/stm3/utils/WritePDB.v
		$include xp_comm_proj/stm3/utils/Float_2_Byte_Unif.v
		$include xp_comm_proj/stm3/utils/ThresholdCharge_mod.v
		$include xp_comm_proj/stm3/utils/ThresholdCharge.v
	};
	
	flibrary MSMS {
		$include xp_comm_proj/stm3/msms/ComputeSES_mod.v
		$include xp_comm_proj/stm3/msms/ComputeSESurface.v
	};
	
	flibrary TRACES {
		$include xp_comm_proj/stm3/traces/AccumulateTraces_mod.v
		$include xp_comm_proj/stm3/traces/Accumulate_Traces.v
	};
	
	flibrary TUBES {
		$include xp_comm_proj/stm3/tube/NCACTube_mod.v
		$include xp_comm_proj/stm3/tube/NCAC_Tube.v
	};
	
	flibrary CELLS {
		$include xp_comm_proj/stm3/cells/FindTetrahedra_mod.v
		$include xp_comm_proj/stm3/cells/FindTetrahedra.v
	};
	
	flibrary SCAT2VOL {
		$include xp_comm_proj/stm3/scat2vol/scat_to_vol_mod.v
		$include xp_comm_proj/stm3/scat2vol/ScatToVolume.v
	};
	
	flibrary DENS {
		$include xp_comm_proj/stm3/dens/ParticleDensity_mod.v
		$include xp_comm_proj/stm3/dens/Particle_Density.v
		$include xp_comm_proj/stm3/dens/VelocityMean_mod.v
		$include xp_comm_proj/stm3/dens/Velocity_Mean.v
	};
	
	flibrary CRYSTAL {
		$include xp_comm_proj/stm3/crystal/LatticeReplication_mod.v
		$include xp_comm_proj/stm3/crystal/MillerPlane_mod.v
		$include xp_comm_proj/stm3/crystal/SubCell_mod.v
		$include xp_comm_proj/stm3/crystal/Symmetry_mod.v
		$include xp_comm_proj/stm3/crystal/RemoveDuplicates_mod.v
		$include xp_comm_proj/stm3/crystal/ReplicateLattice.v
		$include xp_comm_proj/stm3/crystal/DisplayUnitCell.v
		$include xp_comm_proj/stm3/crystal/MillerPlane.v
		$include xp_comm_proj/stm3/crystal/CropUnitCell.v
		$include xp_comm_proj/stm3/crystal/ApplySymmetries.v
	};
	
	flibrary APPS {
		$include xp_comm_proj/stm3/apps/MolDisplayApp.v
		$include xp_comm_proj/stm3/apps/AnimDisplayApp.v
		$include xp_comm_proj/stm3/apps/PickMoleculeApp.v
		$include xp_comm_proj/stm3/apps/TracesDisplayApp.v
		$include xp_comm_proj/stm3/apps/Scat2VolApp.v
		$include xp_comm_proj/stm3/apps/ParticleDensityApp.v
	};
	
	flibrary POV {
		$include xp_comm_proj/stm3/pov/WritePOV_mod.v
		$include xp_comm_proj/stm3/pov/WritePOV.v
	};
	
	flibrary LIB<user_library=0,compile_subs=0> {
		flibrary READERS {
			XP_COMM_PROJ.STM3.BASE.ReadMolecule Read_Molecule;
			XP_COMM_PROJ.STM3.BASE.ReadAnimMolecule Read_Anim_Molecule;
			XP_COMM_PROJ.STM3.BASE.Read_Gaussian_Cube Read_Gaussian_Cube;
		};
		flibrary MOL_MODULES {
			XP_COMM_PROJ.STM3.MSMS.ComputeSESurface Compute_SE_Surface;
			XP_COMM_PROJ.STM3.TRACES.Accumulate_Traces Accumulate_Traces;
			XP_COMM_PROJ.STM3.TUBES.NCAC_Tube NCAC_Tube;
			XP_COMM_PROJ.STM3.CELLS.FindTetrahedra Find_Tetrahedra;
			XP_COMM_PROJ.STM3.DENS.Particle_Density Particle_Density;
			XP_COMM_PROJ.STM3.DENS.Velocity_Mean Velocity_Mean;
		};
		flibrary MODULES {
			XP_COMM_PROJ.STM3.UTILS.Select_Atoms Select_Atoms;
			XP_COMM_PROJ.STM3.UTILS.Select_Residues Select_Residues;
			XP_COMM_PROJ.STM3.UTILS.AddBonds Add_Bonds;
			XP_COMM_PROJ.STM3.UTILS.CutSurface Cut_Surface;
			XP_COMM_PROJ.STM3.UTILS.Bicolor.BicolorSurface Bicolor_Surface;
			XP_COMM_PROJ.STM3.UTILS.CountBonds Count_Bonds;
			XP_COMM_PROJ.STM3.UTILS.Crop_Atoms Crop_Atoms;
			XP_COMM_PROJ.STM3.UTILS.ThresholdCharge Threshold_Charge;
		};
		flibrary CRYSTAL_MODULES {
			XP_COMM_PROJ.STM3.CRYSTAL.DisplayUnitCell Display_Unit_Cell;
			XP_COMM_PROJ.STM3.CRYSTAL.ReplicateLattice Replicate_Lattice;
			XP_COMM_PROJ.STM3.CRYSTAL.Miller_Plane Miller_Plane;
			XP_COMM_PROJ.STM3.BASE.RecomputeBonds Recompute_Bonds;
			XP_COMM_PROJ.STM3.CRYSTAL.CropUnitCell Crop_Unit_Cell;
			XP_COMM_PROJ.STM3.CRYSTAL.ApplySymmetries Apply_Symmetries;
		};
		flibrary CUBE_MODULES {
        	MODS.isosurface Isosurface<NEhelpContextID=1,NEhelpFile="STM3/Isosurface.html"> {
				obj {
					Obj {
         			cache_size => cache(switch(is_valid(getenv("CACHE_SIZE"))+1, 64, getenv("CACHE_SIZE")));
					};
				};
				UIpanel {
					parent<NEportLevels={2,0}>;
				};
			};
			XP_COMM_PROJ.STM3.UTILS.Cube_on_Surface Cube_on_Surface;
			XP_COMM_PROJ.STM3.UTILS.Float2ByteUnif Float_2_Byte;
		};
		flibrary DISPLAY {
			XP_COMM_PROJ.STM3.BASE.DisplayMolecule Display_Molecule;
			XP_COMM_PROJ.STM3.BASE.ColorLegend Color_Legend;
			XP_COMM_PROJ.STM3.UTILS.PickAtoms Pick_Atoms;
		};

		flibrary OUTPUT {
         HLM.OutputImage output_image<NEhelpContextID=1,NEhelpFile="STM3/output_image.html"> {
				output_field {
					write_image_ui {
						panel {
							title => name_of(<-.<-.<-.<-, 1);
						};
						resolution_frame {
							height => 170;
						};
						flip_toggle {
							set = 0;
						};
						format_rb {
							selectedItem = 3;
						};
						UIslider_width {
							value = 1024.;
						};
						UIslider_height {
							value = 1024.;
						};
					};
				};
			};
			XP_COMM_PROJ.STM3.BASE.make_movie make_movie;
			XP_COMM_PROJ.STM3.POV.Write_POV_Ray Write_POV_Ray;
			XP_COMM_PROJ.STM3.UTILS.WritePDB Write_PDB;
		};
		
		flibrary APPS {
			XP_COMM_PROJ.STM3.APPS.MolDisplayApp         MolDisplayApp;
			XP_COMM_PROJ.STM3.APPS.AnimDisplayApp        AnimDisplayApp;
			XP_COMM_PROJ.STM3.APPS.PickMoleculeApp       PickMoleculeApp;
			XP_COMM_PROJ.STM3.APPS.TracesDisplayApp      TracesDisplayApp;
			XP_COMM_PROJ.STM3.APPS.Scat2VolumeDisplayApp Scat2VolumeApp;
			XP_COMM_PROJ.STM3.APPS.ParticleDensityApp    ParticleDensityApp;
		};
	};
};
