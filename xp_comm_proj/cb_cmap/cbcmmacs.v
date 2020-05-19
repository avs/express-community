flibrary CB_ColorMapMacs {

	macro UIColorMap {
		ilink visible = 1;
		ilink parent;
		XP_COMM_PROJ.CB_ColorMap.CB_ColorMapMods.color_map+IPort2 &color_map;
		int ncolors => color_map.ncolors;
		int x = 0;
		int y = 0;
		int height => UIframe.height;
		int colwidth = 15;
		int colheight => (colwidth * 2);
		int cmap_index = 1;
		int cmout;
		GMOD.copy_on_change copy_on_change {
			trigger = 0;
			input => <-.cmap_index;
			output => <-.cmout;
		};
		UIframe UIframe {
			x => <-.x;
			y => <-.y;
			visible => <-.visible;
			parent => <-.parent;
			width => (<-.colwidth  * <-.ncolors) +6;
			height => <-.colheight + 6;
		};
		UIpanel cb_color [.ncolors] {
			parent => <-.UIframe;
			int index =>index_of(cb_color);
			x => (.index * <-.colwidth);
			y => 0;
			width => <-.colwidth;
			height => <-.parent.clientheight;
			int r => <-.color_map.colors[.index][0];
			int g => <-.color_map.colors[.index][1];
			int b => <-.color_map.colors[.index][2];
			color {
				backgroundColor => str_format("#%02x%02x%02x",<-.r,<-.g,<-.b);
			};
		};
		UImouseEvents UImouseEvents[.ncolors] {
			view => <-.cb_color[index_of(UImouseEvents)];
			buttonMask => <-.copy_on_change.trigger;
		};
	};
	
	macro UIColorMapPanel {
		ilink visible = 1;
		ilink parent;
		int index;
		XP_COMM_PROJ.CB_ColorMap.CB_ColorMapMods.color_map+IPort2 &color_map;
		int+IPort2 use_name = 0; 
		string label => switch(use_name+1,str_format("%i colors",color_map.ncolors),color_map.name);
		int x = 0;
		int y = 0;
		UIColorMap UIColorMap {
			cmap_index => <-.index;
			x => UIlabel.x + UIlabel.width + 5;
			visible => <-.visible;
			parent => <-.UIpanel;
			color_map => <-.color_map;
			cmout => <-.cmout;
		};
		int width => UIpanel.width;
		int height => UIpanel.height;
		UIpanel UIpanel {
			x => <-.x;
			y => <-.y;
			parent => <-.parent;
			width => parent.clientWidth;
			height => <-.UIColorMap.height;
			color {
				backgroundColor => switch((<-.<-.cmout==<-.<-.index),"#ffffff");
			};		
		};
		UIlabel UIlabel {
			x=0;
			y=0;
			parent => <-.UIpanel;
			label => <-.label;
			alignment => "left";
			width => 60;
			color {
				backgroundColor => switch((<-.<-.cmout==<-.<-.index),"#ffffff");
			};		
		};
		UImouseEvents UIpanelEvents {
			view => <-.UIpanel;
			buttonMask => <-.UIColorMap.copy_on_change.trigger;
		};
		UImouseEvents UIlabelEvents {
			view => <-.UIlabel;
			buttonMask => <-.UIColorMap.copy_on_change.trigger;
		};
		int cmout;
	};
	macro UICategoryMapPanel {
		ilink visible = 1;
		ilink parent;
		int x = 0;
		int y = 0;
		int+IPort2 use_name = 0; 
		XP_COMM_PROJ.CB_ColorMap.CB_ColorMapMods.CB_ColorMapParams+IPort2 &CB_ColorMapParams;
		int ncmaps => CB_ColorMapParams.ncmap;
		UIpanel UIpanel {
			x => <-.x;
			y => <-.y;
			parent => <-.parent;
			width => parent.clientWidth;
			height =1000;
			// <-.ColorMapPanel[0].height * <-.ncmaps;
		};
		UIColorMapPanel ColorMapPanel[ncmaps]{
			index => index_of(ColorMapPanel);
			parent => <-.UIpanel;
			color_map => <-.CB_ColorMapParams.color_map[index];
			use_name => <-.use_name; 
			y => index * height;
			cmout => <-.CB_ColorMapParams.colidx;
		};
	};
	
	macro+Datamap CB_Datamap {
		int stepped = 0;
		float+nres ratio => (DataRange[0].UIMaxValue - DataRange[0].UIMinValue) / (dataMax - dataMin);
		XP_COMM_PROJ.CB_ColorMap.CB_ColorMapMods.color_map+IPort2 &color_map;
		DMAP.DatamapValue+nres DatamapValue[<-.color_map.ncolors]{
			int idx => index_of(DatamapValue);
			v1 => 1;
			v2 => <-.color_map.colors[idx][0]/255.0;
			v3 => <-.color_map.colors[idx][1]/255.0;
			v4 => <-.color_map.colors[idx][2]/255.0;
		};

		/* FOUR ranges */
		DefaultDataRange DataRange[.color_map.ncolors-1]{
			int idx => index_of(DataRange);
			float split => 1/(<-.color_map.ncolors-1.0);
			size => 256 * split;
			controlPoints => {<-.DatamapValue[idx], <-.DatamapValue[idx+1]};
			DataMinValue => <-.dataMin + ((<-.dataMax - <-.dataMin) * (idx * split));
			DataMaxValue => <-.dataMin + ((<-.dataMax - <-.dataMin) * ((idx + 1.0) * split));
			UIMinValue => idx * (255.0 * split);
			UIMaxValue => (idx+1.0) * (255.0 * split);
			minActive = 1;
			maxActive = 1;
			sizeActive = 1;
			selectValues = 0;
			selectAlphaRange = 0;
			selectColorRange = 0;
		};
		
		DefaultDataRange DataRange_stepped[1]{
			size => <-.color_map.ncolors;
			controlPoints => <-.DatamapValue;
			DataMinValue => <-.dataMin;
			DataMaxValue => <-.dataMax;
			UIMinValue = 0;
			UIMaxValue = 255.0;
			minActive = 1;
			maxActive = 1;
			sizeActive = 1;
			selectValues = 0;
			selectAlphaRange = 0;
			selectColorRange = 0;
		};

		editable = 1;
		/* allows merge operation to work properly. */
		dataMin =;
		dataMax =;
		currentColorModel = 1;
		colorModel => ColorModels.models[currentColorModel];
		ranges+IPort => switch(stepped+1,DataRange,DataRange_stepped);

		/* For compatibility, we need to have minvalue and
		maxvalue around so if people had actually modified
		them, they will be able to access them.
		*/
		DMAP.DatamapValue &minvalue => DatamapValue[0];
		DMAP.DatamapValue &maxvalue => DatamapValue[<-.color_map.ncolors - 1];
	};

	GDM.DataObject DataObjectDmap{
      Datamap+IPort2 &Datamap;
	  Obj {
		dmap =><-.Datamap;
      };
	};

	macro CB_ColorMap {
		XP_COMM_PROJ.CB_ColorMap.CB_ColorMapMods.CB_ColorMapParams CB_ColorMapParams; 
		XP_COMM_PROJ.CB_ColorMap.CB_ColorMapMods.CB_ColorMapCore CB_ColorMapCore {
			ncats => <-.CB_ColorMapParams.ncats;
			categories => <-.CB_ColorMapParams.categories;
			selector => <-.CB_ColorMapParams.map_type;
			catidx => <-.CB_ColorMapParams.catidx;
			ncatmaps => <-.CB_ColorMapParams.ncatmaps;
			catmaps => <-.CB_ColorMapParams.catmaps;
			mapidx => <-.CB_ColorMapParams.mapidx;
			ncmap => <-.CB_ColorMapParams.ncmap;
			colormaps => <-.CB_ColorMapParams.color_map;
		};
		UImod_panel UImod_panel {
			title => name_of(<-.<-,1);
		};
		UIoptionMenuLabel Category {
			parent => <-.UImod_panel;
         	labels => <-.CB_ColorMapParams.categories;
			selectedItem => <-.CB_ColorMapParams.catidx;
		};
		UIradioBoxLabel mapSel {
			parent => <-.UImod_panel;
			title => "Select Colourmap by";
			labels => <-.CB_ColorMapParams.map_type.choices;
         	selectedItem => <-.CB_ColorMapParams.map_type;
			UIradioBox {
           		orientation = "horizontal";
				itemWidth => <-.width /2;
         	};
			width => parent.clientWidth;
			y => <-.Category.y + <-.Category.height+20;
		};
		UIoptionMenuLabel catmaps {
			y => <-.mapSel.y + <-.mapSel.height+10;
			width => parent.clientWidth * 2/3;
			parent => <-.UImod_panel;
			title => <-.mapSel.selectedString;
         	labels => <-.CB_ColorMapParams.catmaps;
         	selectedItem => <-.CB_ColorMapParams.mapidx;
		};
		UItoggle Stepped{
			y => <-.catmaps.y;
			x => <-.catmaps.x + catmaps.width+10;
			width => parent.clientWidth /3 - 10;
			parent => <-.UImod_panel;
			set => <-.CB_ColorMapParams.stepped;
		};
		UICategoryMapPanel UICategoryMapPanel{
			parent => <-.UImod_panel;
			y => <-.catmaps.y + <-.catmaps.height+20;
			CB_ColorMapParams =><-.CB_ColorMapParams;
			use_name =><-.mapSel.selectedItem;
			
		};
		XP_COMM_PROJ.CB_ColorMap.CB_ColorMapMods.color_map+OPort2 &color_map => CB_ColorMapParams.color_map[CB_ColorMapParams.colidx];
		
		CB_Datamap+OPort2 CB_Datamap{
			stepped => <-.CB_ColorMapParams.stepped;
			color_map => <-.color_map;  
			dataMin = 0;
  			dataMax = 255;

		};
	};
	
};
