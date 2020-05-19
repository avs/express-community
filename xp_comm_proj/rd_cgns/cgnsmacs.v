

// This library contains a number of higher level wrapper macros to the cgns module.

flibrary ReadCgnsMacs<compile_subs=0> {

   macro ReadCgnsUI {
      XP_COMM_PROJ.ReadCgns.ReadCgnsMods.CgnsParams &CgnsParams<NEportLevels={2,1}>;

		XP_COMM_PROJ.ReadCgns.ReadCgnsMods.CgnsOutputParams CgnsOutputParams<NEportLevels={0,2}>{
			currentBase => <-.currentBase;
			currentActiveZones => <-.zoneUI.activeArr;
			currentSolutions => <-.zoneUI.solutionArr;
		};

      XP_COMM_PROJ.ReadCgns.ReadCgnsMods.BaseInfo &BaseInfo<NEportLevels={2,1}>;
 	    

		UImod_panel Read_CGNS<NEx=44.,NEy=77.> {
			parent<NEportLevels={4,0}>;
		};
		UIfileDialog Read_CGNS_Browse<NEx=77.,NEy=374.> {
			visible => <-.file_browse_button.do;
			isModal = 1;
			searchPattern = "*.cgns";
    		//transfer parameter
			filename => <-.CgnsParams.filename;
		};
		UIlabel CGNS_file_name<NEx=77.,NEy=198.> {
			parent => <-.Read_CGNS;
			y = 0;
			width = 120;
		};
		UItext file_name_text<NEx=77.,NEy=253.> {
			parent => <-.Read_CGNS;
			text<NEportLevels={0,0}> => <-.CgnsParams.filename;
			y => (<-.CGNS_file_name.y + <-.CGNS_file_name.height) + 3;
			width => ((parent.clientWidth - <-.file_browse_button.width) - 5);
		};
		UIbutton file_browse_button<NEx=77.,NEy=308.> {
			parent => <-.Read_CGNS;
			height => <-.file_name_text.height;
			label => "Browse...";
			y => <-.file_name_text.y;
			x => (parent.clientWidth - .width);
			width = 55;
		};

		UItext status_text<NEx=528.,NEy=176.> {
			parent => <-.Read_CGNS;
			y => (<-.file_name_text.y + <-.file_name_text.height) + 3;
			width => parent.clientWidth;
			rows = 3;
			multiLine =1;
			outputOnly = 1;
			resizeToText = 0;
			updateMode = 7;
			color {
				foregroundColor = "Blue";
			};

			text => <-.CgnsParams.status;
		};

		int nBase<NEx=682.,NEy=33.> => BaseInfo.nBase;

		UIoption base_options<NEx=517.,NEy=242.>[nBase] {
    		label => <-.BaseInfo.base_info[index_of(base_options)].base_name;
		};

		UIoptionMenu base_option_menu<NEx=297.,NEy=242.> {
#ifdef MSDOS
			y => (<-.status_text.y + <-.status_text.height) + 25;
#else
			y => (<-.status_text.y + <-.status_text.height) + 6;
#endif
			parent => <-.Read_CGNS;
			width => parent.clientWidth;
			label = "Base :";
			cmdList => <-.base_options;
		};
		int currentBase<NEx=539.,NEy=33.> =>
			switch(is_valid(base_option_menu.selectedItem)+1, (!(!nBase)),base_option_menu.selectedItem+1);

#ifdef MSDOS
		GMOD.copy_on_change copy_on_change<NEx=682.,NEy=77.> {
			trigger => <-.nBase;
			input = 0;
			output => <-.base_option_menu.selectedItem;
			on_inst = 0;
		};
#endif

  		macro zoneUI<NEx=297.,NEy=319.> {
			ilink parent<NEx=44.,NEy=22.> => <-.Read_CGNS;

			XP_COMM_PROJ.ReadCgns.ReadCgnsMods.ZoneInfo &ZoneInfo<NEportLevels={3,1},NEx=517.,NEy=22.>;

			int nZone<NEx=682.,NEy=22.> => ZoneInfo.nZone;
			 
			UIlabel nZonesLabel<NEx=11.,NEy=77.> {
				parent => <-.parent;
				label => ("Zones: " + <-.nZone);
				y => (<-.<-.base_option_menu.y + <-.<-.base_option_menu.height) + 3;
				width => (parent.clientWidth / 2);
			};
			UIlabel nActiveLabel<NEx=176.,NEy=77.> {
				parent => <-.parent;
				label => ("active: " + <-.nActive);
				y => <-.nZonesLabel.y;
				x => <-.nZonesLabel.width;
				width => (parent.clientWidth / 2);
			};
			int nActive<NEx=363.,NEy=22.> => sum(.activeArr);
			int activeArr<NEx=363.,NEy=66.>[.nZone];
			int allowedArr<NEx=363.,NEy=110.>[.nZone] => ZoneInfo.zone_info.cartesian;
			int disallowedArr<NEx=363.,NEy=110.>[.nZone] => init_array(nZone, 0, 0);
			 
			int solutionArr<NEx=363.,NEy=154.>[.nZone] => zone.solutionArrEl;

			UIbutton activateAllButton<NEx=11.,NEy=165.> {
				parent => <-.parent;
				y => (<-.nZonesLabel.y + <-.nZonesLabel.height) + 3;
				width => (parent.clientWidth / 2);
				active => nZone;
				label = "activate all";
			};
			UIbutton deactivateAllButton<NEx=176.,NEy=165.> {
				parent => <-.parent;
				x => <-.activateAllButton.width;
				y => <-.activateAllButton.y;
				width => (parent.clientWidth / 2);
				active => nZone;
				label = "deactivate all";
			};
			UIbutton loadButton<NEx=11.,NEy=209.> {
				parent => <-.parent;
				y => (<-.activateAllButton.y + <-.activateAllButton.height) + 3;
				width => parent.clientWidth;
				height = 30;
				active => nActive;
				label = "Load CGNS";
				do => <-.<-.CgnsParams.load;

			};
			GMOD.parse_v V_do_deactivate<NEx=682.,NEy=154.> {
				v_commands = "!activeArr=disallowedArr;";
				trigger => (<-.deactivateAllButton.do + <-.nZone);
				no_errors = 1;
				on_inst = 0;
				relative => <-.activeArr + <-.disallowedArr;
			};
			GMOD.parse_v V_do_activate<NEx=528.,NEy=154.> {
				v_commands = "!activeArr=allowedArr;";
				trigger => (<-.activateAllButton.do + <-.nZone);
				no_errors = 1;
				on_inst = 0;
				relative => <-.activeArr + <-.allowedArr;
			};
			group zone<NEx=198.,NEy=264.>[nZone] {
				UIframe frame {
					parent => <-.<-.zoneWindow;
					x = 1;
					y => 1+height*index_of(zone);
					width => (parent.clientWidth - 5);
#ifdef MSDOS
					height = 90;
#else
					height = 112;
#endif
				};
				UIlabel zone_label {
					parent => frame;
					y = 0;
					width => (parent.clientWidth - 2);
					label => <-.<-.ZoneInfo.zone_info[index_of(zone)].zone_label;
				};
				UItoggle active_toggle {
					parent => frame;
					x => (parent.clientWidth / 2);
					y => zone_label.y + zone_label.height;
					width => (parent.clientWidth / 2);
					set => <-.<-.activeArr[index_of(zone)];
					active => <-.<-.allowedArr[index_of(zone)];
					label = "Active";
				};
				int nSolution => <-.ZoneInfo.zone_info[index_of(zone)].nSolution;

				UIoption sol_options[nSolution] {
				    label => <-.<-.ZoneInfo.zone_info[index_of(zone)].sol_info[index_of(sol_options)].sol_name;
				};
				UIoptionMenu sol_option_menu {
					x = 10;
					y => active_toggle.y + active_toggle.height;
					parent => frame;
					width => parent.clientWidth - x;
					label = "Solution :";
					visible => <-.nSolution;
					cmdList => <-.sol_options;
				};
				int solutionArrEl => switch(is_valid(sol_option_menu.selectedItem)+1,
					(!(!nSolution)),
					sol_option_menu.selectedItem+1);
				UIlabel zone_info {
					parent => frame;
					y => sol_option_menu.y + sol_option_menu.height;
					width => parent.clientWidth;
					label => <-.<-.ZoneInfo.zone_info[index_of(zone)].zone_info;
				};
			};

			UIscrolledWindow zoneWindow<NEx=11.,NEy=264.> {
				parent => <-.parent;
				x = 0;
				y => (<-.loadButton.y + <-.loadButton.height) + 3;
				width => parent.clientWidth;
#ifdef MSDOS
				height => switch((virtualHeight < 401)+1, 401, virtualHeight-1);
				virtualHeight => max_array(<-.zone.frame.y + <-.zone.frame.height + 20);
#else
				height = 400;
				virtualHeight => max_array(<-.zone.frame.y + <-.zone.frame.height + 2);
#endif
			};
		};
	};



   // ReadCgnsFunc
   // Functional macro

   macro ReadCgnsFunc {
      XP_COMM_PROJ.ReadCgns.ReadCgnsMods.CgnsParams &CgnsParams<NEportLevels={2,1}>;
		XP_COMM_PROJ.ReadCgns.ReadCgnsMods.CgnsOutputParams &CgnsOutputParams<NEportLevels={2,1}>;
	
	   XP_COMM_PROJ.ReadCgns.ReadCgnsMods.DVread_cgns DVread_cgns {
	      CgnsParams => <-.CgnsParams;
			CgnsOutputParams => <-.CgnsOutputParams;
      };

		olink BaseInfo    => .DVread_cgns.BaseInfo;
		olink ZoneInfo    => .DVread_cgns.ZoneInfo;
		olink Multi_Block  => .DVread_cgns.Multi_Block;
	   olink unstr_fields => .DVread_cgns.Multi_Block_Unstr.fields;
   };   



   // read_cgns 
   // user Macro 

   macro read_cgns {

      XP_COMM_PROJ.ReadCgns.ReadCgnsMods.CgnsParams CgnsParams;

	   XP_COMM_PROJ.ReadCgns.ReadCgnsMacs.ReadCgnsUI+nosave ReadCgnsUI {
  		   CgnsParams => <-.CgnsParams;
    	   BaseInfo => <-.ReadCgnsFunc.BaseInfo;

         UImod_panel {
            parent<NEportLevels={4,0}>;
         };
         
         zoneUI{
		       ZoneInfo => <-.<-.ReadCgnsFunc.ZoneInfo;
    	   };
      };


      XP_COMM_PROJ.ReadCgns.ReadCgnsMacs.ReadCgnsFunc+nosave ReadCgnsFunc {
			CgnsParams => <-.CgnsParams;
			CgnsOutputParams => <-.ReadCgnsUI.CgnsOutputParams;
	   };

	   olink Multi_Block  => ReadCgnsFunc.Multi_Block;
	   olink unstr_fields => ReadCgnsFunc.unstr_fields;
   };

};

