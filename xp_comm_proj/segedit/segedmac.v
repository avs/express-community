
flibrary SegmentationEditorMacs <compile_subs=0>
{
	#ifdef XP_VERSION_PRE_63
    // Copy of the one in FLD_MAP, but without the DataObject
    group polyline_mesh {
        float+IPort2 &coord[][];
        int+IPort2 &connect[];

        int coord_dims[] => array_dims(coord);

        int error => (array_size(coord) >0) &&
                     (array_size(coord_dims) != 2);
        GMOD.print_error print_error {
            error => <-.error;
            error_source = "polyline_mesh";
            error_message = "Invalid mesh: coordinates should be 2d array [nnodes][nspace]";
            on_inst = 1;
        };

        Mesh+OPort2  out {
            nnodes => switch(!error,<-.coord_dims[1]);
            nspace => switch(!error,<-.coord_dims[0]);
            coordinates.values => switch(!error,<-.<-.coord);
            ncell_sets = 1;
            Polyline cell_set {
                npolys => switch(!error,array_size(<-.<-.connect)/2);
                poly_connect_list => switch(!error,<-.<-.connect);
            };
        };
    };

    macro bounds_dual {
        // 3D field
        ilink in_field3<export_all=1>;
        // 2D field
        ilink in_field2<export_all=1>;
        DV_Param_bounds BoundsParam<export_all=2> {
            hull=1;
            edges=0;
            faces=0;
            imin=0;
            imax=0;
            jmin=0;
            jmax=0;
            kmin=0;
            kmax=0;
            data=0;
            component=0;
        };
        DVbounds DVbounds3 {
            DV_Param_bounds+IPort2 &param => BoundsParam;
            in => in_field3;
            hull => param.hull;
            edges => param.edges;
            faces => param.faces;
            imin => param.imin;
            imax => param.imax;
            jmin => param.jmin;
            jmax => param.jmax;
            kmin => param.kmin;
            kmax => param.kmax;
            data => param.data;
            component => param.component;
        };
        DVbounds DVbounds2 {
            DV_Param_bounds+IPort2 &param => BoundsParam;
            in => in_field2;
            hull => param.hull;
            edges => param.edges;
            faces => param.faces;
            imin => param.imin;
            imax => param.imax;
            jmin => param.jmin;
            jmax => param.jmax;
            kmin => param.kmin;
            kmax => param.kmax;
            data => param.data;
            component => param.component;
        };
        macro map_2d_to_3d {
            Mesh+Space2+IPort2 &bounds2d => DVbounds2.out;
            // Map 2d coords to 3d coords.  Everything else in this macro is
            // just to generate a output field using these coordinates.
            DV.DVxform_coord DVxform_coord {
                in => <-.bounds2d;
                comp = { 0, 1, 2 };
                coord_nspace = 3;
            };
            polyline_mesh polyline_mesh {
                coord   => <-.DVxform_coord.coord;
                connect => <-.bounds2d.cell_set[0].poly_connect_list;
            };
            // Assuming no node data ....
            olink out_mesh => polyline_mesh.out;
        };
        FAST_ARR.DVcombine_sets_ARR DVcombine_sets {
            in => { DVbounds3.out, map_2d_to_3d.out_mesh };
        };
        DataObjectNoTexture obj {
            in => DVcombine_sets.out;
            Modes {
                /* set lines, gouraud mode */
                mode = {0, GD_LINES, GD_SURF_GOURAUD, 0, 0};
            };
            Obj {
                name => name_of(<-.<-.<-);
                xform_mode = GD_XFORM_MODE_PARENT;
            };
        };
        olink out_fld<export_all=2> => DVcombine_sets.out;
        olink out_obj => obj.obj;
    }; // bounds

    macro mask_display {
        // 3D field
        ilink in_field;
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorParams+IPort2 &params;
        DV.DVextract_comp DVextract_comp {
            in => <-.in_field;
            component => <-.params.value_nd;
        };
        DVM.DVorthoslice_unif DVorthoslice {
            in+nres => <-.DVextract_comp.out;
            &axis  => <-.params.axis;
            &plane => <-.params.slice;
        };
        DataObjectNoTexture orthosliceObj {
            in => DVorthoslice.out;
            Datamap {
                dataMin+nres => min_array(<-.<-.in_field.node_data.min);
                dataMax+nres => max_array(<-.<-.in_field.node_data.max);
            };
            Obj {
                name => name_of(<-.<-.<-);
                xform_mode = GD_XFORM_MODE_PARENT;
            };
        };
        bounds_dual bounds_dual {
            in_field3 => <-.in_field;
            in_field2 => <-.DVorthoslice.out;
        };
        DV.DVthresh_null DVthresh_null {
            in => <-.DVextract_comp.out;
            thresh_component => 0;
        };
        DataObjectNoTexture thresh_nullObj {
            in => DVthresh_null.out;
            Datamap {
                dataMin+nres => min_array(<-.<-.in_field.node_data.min);
                dataMax+nres => max_array(<-.<-.in_field.node_data.max);
            };
            Obj {
                name => name_of(<-.<-.<-);
                xform_mode = GD_XFORM_MODE_PARENT;
            };
        };
        GroupObject mask3Dobj {
            child_objs => {
                <-.bounds_dual.out_obj,
                <-.thresh_nullObj.obj,
                <-.orthosliceObj.obj
            };
        };
        olink out_fld => bounds_dual.out_fld;
        olink obj => mask3Dobj.obj;
    };

    macro mask2D_display {
        // 3D field
        ilink in_field;
        DVbounds DVbounds {
            in => in_field;
            hull=1;
            edges=0;
            faces=0;
            imin=0;
            imax=0;
            jmin=0;
            jmax=0;
            kmin=0;
            kmax=0;
            data=0;
            component=0;
        };
        DataObjectNoTexture BoundsObject {
            in => <-.DVbounds.out;
            Obj {
                name => name_of(<-.<-.<-);
                xform_mode = GD_XFORM_MODE_PARENT;
            };
        };
        DataObjectNoTexture DataObject {
            in => <-.in_field;
            Datamap {
                dataMin+nres => min_array(<-.<-.in_field.node_data.min);
                dataMax+nres => max_array(<-.<-.in_field.node_data.max);
            };
            Obj {
                name => name_of(<-.<-.<-);
                xform_mode = GD_XFORM_MODE_PARENT;
            };
        };
        GroupObject mask2Dobj {
            child_objs => {
                <-.BoundsObject.obj,
                <-.DataObject.obj
            };
        };
        olink obj => mask2Dobj.obj;
    };

    macro out_display {
        ilink in_field;
        ilink mask3D;
        ilink parent;
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorParams+IPort2 &params;
        GMOD.instancer inst_addmask {
            Value => (<-.params.applycommand==0);
            Group => out_add_mask;
        };
        macro out_add_mask {
            int selected[] => params.outentities;
            ilink parent => <-.parent;
            ilink in => <-.mask3D;
            ilink params => <-.params;
            UIoptionBoxLabel UIoptionBoxLabel {
                parent => <-.parent;
                labels => <-.params.entities;
                selectedItems => <-.<-.params.outentities;
                title=>"Output Segments";
                UIoptionBox {
                    cmdList {
                        set => 1;
                    };
                };
            };
            macro create_objs[array_size(selected)] {
                DV.DVextract_comp DVextract_comp {
                    in => <-.<-.in;
                    component+nres => <-.<-.selected[index_of(create_objs)];
                };
                DV.DVthresh_null DVthresh_null {
                    in => <-.DVextract_comp.out;
                    thresh_component => 0;
                };
                DataObjectNoTexture thresh_nullObj {
                    in => DVthresh_null.out;
                    Datamap {
                        dataMin+nres => min_array(<-.<-.<-.in.node_data.min);
                        dataMax+nres => max_array(<-.<-.<-.in.node_data.max);
                    };
                    Obj {
                        name => name_of(<-.<-.<-);
                        xform_mode = GD_XFORM_MODE_PARENT;
                    };
                };
                DVM.DVorthoslice_unif DVorthoslice {
                    in+nres => <-.DVextract_comp.out;
                    &axis  => <-.<-.params.axis;
                    &plane => <-.<-.params.slice;
                };
                DataObjectNoTexture orthosliceObj {
                    in => DVorthoslice.out;
                    Datamap {
                        dataMin+nres => min_array(<-.<-.<-.in.node_data.min);
                        dataMax+nres => max_array(<-.<-.<-.in.node_data.max);
                    };
                    Obj {
                        name => name_of(<-.<-.<-);
                        xform_mode = GD_XFORM_MODE_PARENT;
                    };
                };
                GroupObject out_obj {
                    child_objs+nres => { thresh_nullObj.obj, orthosliceObj.obj };
                };
                olink obj=>out_obj.obj;
            };
            GroupObject out_obj {
                child_objs+nres => create_objs.obj;
            };
            olink obj => out_obj.obj;
        };
        GMOD.instancer inst_applymask {
            Value => (<-.params.applycommand==1);
            Group => out_apply_mask;
        };
        macro out_apply_mask {
            ilink in_field => <-.in_field;
            DV.DVthresh_null DVthresh_null {
                in => <-.in_field;
                thresh_component => 0;
            };
            DataObjectNoTexture thresh_nullObj {
                in => DVthresh_null.out;
                Obj {
                    name => name_of(<-.<-.<-);
                    xform_mode = GD_XFORM_MODE_PARENT;
                };
            };
            olink obj => thresh_nullObj.obj;
        };
        DVM.DVorthoslice_unif DVorthoslice {
            in => <-.in_field;
            &axis  => <-.params.axis;
            &plane => <-.params.slice;
        };
        DataObjectNoTexture orthosliceObj {
            in => DVorthoslice.out;
            Obj {
                name => name_of(<-.<-.<-);
                xform_mode = GD_XFORM_MODE_PARENT;
            };
        };
        bounds_dual bounds_dual {
            in_field3 => <-.in_field;
            in_field2 => <-.DVorthoslice.out;
        };
        GroupObject out_obj {
            child_objs+nres => {
                switch((<-.params.applycommand+1),
                        <-.out_add_mask.obj, <-.out_apply_mask.obj),
                switch((<-.params.applycommand),
                        orthosliceObj.obj), <-.bounds_dual.out_obj
            };
        };
        olink obj => out_obj.obj;
    };

    macro AddMultitpleNodeData {
        Mesh+Node_Data+IPort2 &in;
        Node_Data+IPort2 &data {
            nnodes => <-.in.nnodes;
        };
        Node_Data+OPort tmp_data[in.nnode_data+data.nnode_data] {
            nnode_data=1;
            nnodes => <-.in.nnodes;
            &node_data => switch ((index_of(tmp_data)<(<-.in.nnode_data))+1,
                                   <-.data.node_data[index_of(tmp_data)-<-.in.nnode_data],
                                   <-.in.node_data[index_of(tmp_data)]);
        };
        FLD_MAP.combine_node_datas combine_node_datas {
            in => <-.tmp_data;
        };
        FLD_MAP.combine_mesh_data combine_mesh_data {
            in_mesh => <-.in;
            in_nd => combine_node_datas.out;
        };
        olink out => combine_mesh_data.out;
        olink obj => combine_mesh_data.obj;
    };

    macro ReadHDF5Mask {
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorParams+IPort2 &params;

        Mesh_Unif+Node_Data+IPort2 &in;

        HDF5.H5list_vars list_vars {
            filename => <-.params.filename;
            nodeVars => <-.params.readentities;
        };
        int dimsdiff[] => in.dims-list_vars.dims;
        int min => min_array(dimsdiff);
        int max => max_array(dimsdiff);
        GMOD.copy_on_change copy_on_change {
            input => ((<-.min == 0) && (<-.max == 0));
            output => <-.params.allow_read;
        };
        HDF5.H5read_field read_field {
            filename+nonotify => <-.params.filename;
            nodeVars => <-.params.selentities;
            // max => <-.list_vars.dims;

            int+read+req trigger<NEportLevels={2,0}> => <-.params.start_read;
            // Make sure it triggers on, uh, trigger
            omethod+notify_val+notify_inst update<status=1> =
                "HDF5read_field_update";

//            copyIntoExisting = 1;
//            outFld => <-.in;
        };
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.copy_node_data+IPort2 copy_node_data{
			input => <-.read_field.outFld;
			output => <-.in;
		};
		
        //olink out => read_field.outFld;
    };

    UIradioBoxLabel UIsegeditRadioBoxLabel {
        height => ((.UIradioBox.y + .UIradioBox.height) + 10);
        UIframe UIpanel {
            parent => <-.parent;
            x => <-.x;
            y => <-.y;
            width => <-.width;
            height => <-.height;
        };
        UIlabel {
            x = 2;
            y = 2;
            width => (<-.UIpanel.width - 4);
            color {
                foregroundColor = "white";
                backgroundColor = "blue";
            };
        };
    };

    UIsegeditRadioBoxLabel RegionGrowUI {
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorParams+IPort2 &params;
        Node_Data+IPort2 &in;
        string label;
        UIframe UIpanel {
            parent => <-.parent;
            visible => <-.visible;
            x => <-.x;
            y => <-.y;
            width => <-.width;
            height => <-.height;
        };
        UIlabel {
            label => <-.label;
        };
        UIslider delta {
            parent => <-.UIpanel;
            width => parent.clientWidth/2;
            y =>  <-.UIlabel.y + <-.UIlabel.height;
            value => <-.params.reg_delta;
            min => cache((in.node_data[<-.params.thr_comp].min_vec[0]));
            max => cache((in.node_data[<-.params.thr_comp].max_vec[0]));
        };
        UItoggle incremental {
            parent => <-.UIpanel;
            width => parent.clientWidth/2-40;
            x => parent.clientWidth/2 + 20;
            y => <-.delta.y + ((<-.delta.height - height)/2);
            set => <-.params.reg_incr;
        };
        UIradioBox {
            orientation = "horizontal";
            y => <-.delta.y + <-.delta.height;
            x => 5;
#ifdef MSDOS
            height => 4 + UIdata.UIfonts[0].lineHeight;
#else
            height => UIdata.UIfonts[0].lineHeight;
#endif
            itemWidth => width/2-20;
        };
    };

    UIsegeditRadioBoxLabel ThresholdUI {
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorParams+IPort2 &params;
        Node_Data+IPort2 &in;
        string label;
        UIframe UIpanel {
            parent => <-.parent;
            visible => <-.visible;
            x => <-.x;
            y => <-.y;
            width => <-.width;
            height => <-.height;
        };
        UIlabel {
            label => <-.label;
        };
        UIslider minimum {
            parent => <-.UIpanel;
            width => parent.clientWidth;
            y =>  <-.UIlabel.y + <-.UIlabel.height;
            value => <-.params.thr_min;
            min => cache((in.node_data[<-.params.thr_comp].min_vec[0])-.1);
            max => cache((in.node_data[<-.params.thr_comp].max_vec[0])+.1);
        };
        UIslider maximum {
            parent => <-.UIpanel;
            width => parent.clientWidth;
            y =>  <-.minimum.y + <-.minimum.height;
            value => <-.params.thr_max;
            min => cache((in.node_data[<-.params.thr_comp].min_vec[0])-.1);
            max => cache((in.node_data[<-.params.thr_comp].max_vec[0])+.1);
        };
        UIradioBox {
            orientation = "horizontal";
            y => <-.maximum.y + <-.maximum.height;
#ifdef MSDOS
            height => 4 + UIdata.UIfonts[0].lineHeight;
#else
            height => UIdata.UIfonts[0].lineHeight;
#endif
            itemWidth => width/2;
        };
    };

    macro Store_Mask_UI {
        ilink parent;
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorParams+IPort2 &params;
        int+IPort2 visible;
        int x;
        int y;
        int width;
        int height=> ((save.y + save.height) + 10);
        string label;
        UIframe UIframe {
            parent => <-.parent;
            visible => <-.visible;
            x => <-.x;
            y => <-.y;
            width => <-.width;
            height => <-.height;
        };
        UIlabel title {
            x = 2;
            y = 2;
            parent => <-.UIframe;
            width => parent.width - 4;
            label => <-.label;
            color {
                foregroundColor = "white";
                backgroundColor = "blue";
            };
        };
        UItext filename {
            parent => <-.UIframe;
            y => <-.title.y + <-.title.height + 10;
            width => 2*parent.clientWidth/3;
            text => <-.params.filename;
        };
        UIbutton browse {
            parent => <-.UIframe;
            y => <-.filename.y;
            height => <-.filename.height;
            x => <-.filename.x + <-.filename.width;
            width => parent.clientWidth/3;
            label => "Browse...";
        };
        UIfileSB file_browser {
            visible => <-.browse.do;
            title => "Store 3D Mask";
            filename => <-.params.filename;
            searchPattern => "$XP_PATH<0>/*.h5";
        };
        UIbutton save {
            parent => <-.UIframe;
            label => "Write";
            width => parent.clientWidth;
            height = 30;
            y => <-.filename.y + <-.filename.height + 10;
            do => <-.params.start_write; 
        };
    };

    macro Read_Mask_UI {
        ilink parent;
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorParams+IPort2 &params;
        int+IPort2 visible;
        int x;
        int y;
        int width;
        int height=> read.y + read.height + 10;
        string label;
        UIframe UIframe {
            parent => <-.parent;
            visible => <-.visible;
            x => <-.x;
            y => <-.y;
            width => <-.width;
            height => <-.height;
        };
        UIlabel title {
            x = 2;
            y = 2;
            parent => <-.UIframe;
            width => parent.width - 4;
            label => <-.label;
            color {
                foregroundColor = "white";
                backgroundColor = "blue";
            };
        };
        UItext filename {
            parent => <-.UIframe;
            y => <-.title.y + <-.title.height + 10;
            width => parent.clientWidth/2;
            text => <-.params.filename;
        };
        UIbutton browse {
            parent => <-.UIframe;
            y => <-.filename.y + <-.filename.height + 10;
            height => <-.filename.height;
            width => parent.clientWidth/2;
            label => "Browse...";
        };
        UIfileSB file_browser {
            visible => <-.browse.do;
            title => "Load 3D Mask";
            filename => <-.params.filename;
            searchPattern => "$XP_PATH<0>/*.h5";
        };
        UIbutton read {
            parent => <-.UIframe;
            label => "Read";
            active => <-.params.allow_read;
            width => parent.clientWidth/2;
            height = 30;
            y => <-.browse.y + <-.browse.height+10;
            do => <-.params.start_read; 
        };        
        UIframe entities {
            parent => <-.UIframe;
            y => <-.filename.y;
            x => (parent.clientWidth / 2);
            width => (parent.clientWidth / 2);
            height => ((<-.read.y + <-.read.height) - .y);
        };
        UIlabel entity {
            parent => <-.entities;
            label => "Read Entities";
            width => parent.clientWidth;
        };
        UIscrolledWindow UIscrolledWindow {
            parent => <-.entities;
            y = 24;
            width => parent.clientWidth;
            height => (parent.clientHeight - .y);
            virtualHeight => <-.UIoptionBox.height;
        };
        label_cmd label_cmd {
            labels => <-.params.readentities;
        };
        UIoptionBox UIoptionBox {
            parent => <-.UIscrolledWindow;
            cmdList => <-.label_cmd.cmd;
            selectedItems => <-.params.selentities;
        };
    };

    macro Edit_Entity_UI {
        ilink parent;
        ilink in;
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorParams+IPort2 &params;
        int+IPort2 visible;
        int x;
        int y;
        int width;
        int height => close.y + close.height + 10;
        string label;
        UIframe UIframe {
            parent => <-.parent;
            visible => <-.visible;
            x => <-.x;
            y => <-.y;
            width => <-.width;
            height => <-.height;
        };
        UIlabel title {
            x = 2;
            y = 2;
            parent => <-.UIframe;
            width => parent.width - 4;
            label => <-.label;
            color {
                foregroundColor = "white";
                backgroundColor = "blue";
            };
        };
        UIbutton combine {
            parent => <-.UIframe;
            x => parent.clientWidth/2;
            y => <-.UIscrolledWindow.y;
            height = 30;
            width => parent.clientWidth/2;
            label => "Combine";
            do => <-.params.combine_entity;
        };
        UIbutton delete {
            parent => <-.UIframe;
            x => <-.combine.x;
            y => <-.combine.y + <-.combine.height + 5;
            height = 30;
            width => parent.clientWidth/2;
            do => <-.params.delete_entity;
            label => "Delete";
        };
        UIbutton close {
            parent => <-.UIframe;
            label => "Close Frame";
            width => parent.clientWidth/2;
            height = 30;
            y => <-.delete.y + <-.delete.height+15;
            x => <-.combine.x;
        };        
        GMOD.parse_v close_frame {
            trigger => <-.close.do;
            v_commands => "visible=0;";        
            relative => <-;        
        };
        UIscrolledWindow UIscrolledWindow {
            parent => <-.UIframe;
            y => <-.title.y + <-.title.height + 10;
            width => parent.clientWidth/2;
            height => <-.close.y + <-.close.height - y;
            virtualHeight+nres => (<-.UIoptionBox.height + 10);
        };
        label_cmd label_cmd {
            labels => <-.params.entities;
        };
        UIoptionBox UIoptionBox {
            x = 0;
            y = 0;
            active = 1;
            parent => <-.UIscrolledWindow;
            cmdList => <-.label_cmd.cmd;
            selectedItems => <-.params.edit_selentities;
            width => parent.clientWidth;
#ifndef MSDOS
            itemWidth+nres => parent.virtualWidth;
#endif
       };
    };

    macro SegmentationEditorUI {
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorParams+IPort2 &params;
        Node_Data+IPort2 &in;
        Node_Data+IPort2 &mask3D;
        UImod_panel UImod_panel {
            title => name_of(<-.<-.<-,1);
            width => (parent.clientWidth - 10);
        };
        UIslider axis {
            parent => <-.UImod_panel;
            value => <-.params.axis;
            y = 0;
            min = 0.;
            max => switch((array_size(<-.params.dims)!=0)+1,0,array_size(<-.params.dims)-1);
            width => parent.clientWidth;
            mode = "integer";
        };
        UIslider plane {
            parent => <-.UImod_panel;
            value => <-.params.slice;
            active =>array_size(<-.params.dims)!=0;
            y => <-.axis.y + <-.axis.height + 10;
            max => <-.params.dims[<-.params.axis]-1;
            min = 0.;
            width => parent.clientWidth;
            mode = "integer";
        };

        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.demux demux {
           input => <-.params.drawcommand;
           // Needs to match size of DrawMaskCommands
           input_max = 13;
        };

        UIsegeditRadioBoxLabel Draw_Mask {
            parent => <-.UImod_panel;
            labels => params.drawcommand.choices;
            width => (parent.clientWidth / 2);
            y => <-.plane.y + <-.plane.height + 10;
            selectedItem => <-.params.drawcommand;
            int bottom => ((.UIradioBox.y + .UIradioBox.height) + 10);
            int height => <-.FixedUIBottom - y;
            // This runs too often
            GMOD.parse_v clear_volume {
                // trigger+nres => label_cmd.cmd[12].do;
                // trigger => (selectedItem == 12);
                trigger+nres => <-.<-.demux.output[12].trigger;
                v_commands => "params.clear_volume=1;";        
                relative => <-;        
            };
        };
        macro value_box {
            ilink parent => <-.UImod_panel;
            int x => parent.clientWidth/2;
            int y => <-.Apply_Mask.y + <-.Apply_Mask.height;
            int height => <-.FixedUIBottom - y;
            int bottom => clear.bottom + 10;
            int width => parent.clientWidth/2;
            UIframe UIframe {
                parent => <-.parent;
                x => <-.x;
                y => <-.y;
                width => <-.width;
                height => <-.height;
            };
            UIslider value {
                parent => <-.UIframe;
                value => <-.<-.params.value;
                y => 0;
                min = 1;
                width => parent.clientWidth;
                mode = "integer";
            };
            UItext label {
                parent => <-.UIframe;
                y => <-.value.y + <-.value.height + 5;
                width => parent.clientWidth;
                text => <-.<-.params.label;
            };
            UIbutton clear {
                parent => <-.UIframe;
                do => <-.<-.params.edit_entity;
                label => "Edit Entities";
                y => <-.label.y + <-.label.height + 5;
                height = 30;
                width => parent.clientWidth;
		int bottom => .height + .y;
            };
        };
        UIsegeditRadioBoxLabel Pen {
            parent => <-.UImod_panel;
            labels => params.pen.choices;
            width => parent.clientWidth / 2;
            height => ((.penwidth.y + .penwidth.height) + 4);
            y => <-.Draw_Mask.y;
            x => parent.clientWidth / 2;
            selectedItem => <-.params.pen;
            UIslider penwidth {
                parent => <-.UIpanel;
                value => <-.<-.params.width;
                y => <-.UIradioBox.y + <-.UIradioBox.height + 2;
                max = 20;
                min = 0;
                title = "width";
                width => parent.clientWidth;
                mode = "integer";
            };
            UIradioBox {
                orientation = "horizontal";
#ifdef MSDOS
                height => 4 + UIdata.UIfonts[0].lineHeight;
#else
                height => UIdata.UIfonts[0].lineHeight;
#endif
                itemWidth => <-.width/2-10;
            };
        };
        UIsegeditRadioBoxLabel Apply_Mask {
            parent => <-.UImod_panel;
            labels => params.applycommand.choices;
            width => parent.clientWidth / 2;
            x => parent.clientWidth / 2;
            y => <-.Pen.y + <-.Pen.height;
            selectedItem => <-.params.applycommand;
        };
        int ValueBoxBottom => .value_box.y + .value_box.bottom;
        int DrawMaskBottom => .Draw_Mask.y + .Draw_Mask.bottom;
        int FixedUIBottom => switch( (ValueBoxBottom < DrawMaskBottom)+1,
                                          ValueBoxBottom, DrawMaskBottom );	
        Edit_Entity_UI Edit_Entity_UI {
            in => <-.mask3D;
            parent => UImod_panel;
            params => <-.params;
            visible => params.edit_entity;
            y => <-.FixedUIBottom;
            int width => parent.clientWidth;
            label => "Edit Entities";
        };
        int edit_y => switch((Edit_Entity_UI.visible+1),0,Edit_Entity_UI.height);
        RegionGrowUI RegionGrow2D {
            in => <-.in;
            params => <-.params;
            labels => params.applyregcommand.choices;
            parent => <-.UImod_panel;
            visible => (<-.params.drawcommand == 7);
            label => <-.params.drawcommand;
            selectedItem => <-.params.applyregcommand;
            y => <-.FixedUIBottom + <-.edit_y;
            int width => parent.clientWidth;
            height => ((.UIbutton.y + .UIbutton.height) + 10);
            UIbutton UIbutton {
                parent => <-.UIpanel;
                height => 30;
                y => <-.UIradioBox.y + <-.UIradioBox.height + 10;
                x => 5;
                do => <-.params.reg_trig;
                width => <-.parent.clientWidth-(2*.x);
                label => "Run with Previous Seed Point";
            };
        };
        RegionGrowUI RegionGrow3D {
            in => <-.in;
            params => <-.params;
            labels => params.applyregcommand.choices;
            parent => <-.UImod_panel;
            visible => (<-.params.drawcommand == 8);
            label => <-.params.drawcommand;
            selectedItem => <-.params.applyregcommand;
            y => <-.FixedUIBottom + <-.edit_y;
            int    width => parent.clientWidth;
        };
        ThresholdUI ThresholdUI {
            in => <-.in;
            params => <-.params;
            labels => params.applyregcommand.choices;
            parent => <-.UImod_panel;
            visible => (<-.params.drawcommand == 9);
            label => <-.params.drawcommand;
            selectedItem => <-.params.applyregcommand;
            int    y => <-.FixedUIBottom + <-.edit_y;
            height => ((.UIbutton.y + .UIbutton.height) + 10);
            int    width => parent.clientWidth;
            UIbutton UIbutton {
                parent => <-.UIpanel;
                height => 30;
                y => <-.UIradioBox.y + <-.UIradioBox.height + 10;
                x => 5;
                do => <-.params.thr_trig;
                width => <-.parent.clientWidth-(2*.x);
                label => "Run Threshold";
            };
        };
        Store_Mask_UI Store_Mask_UI {
            params => <-.params;
            parent => <-.UImod_panel;
            label => "Save 3D Mask";
            visible => (<-.params.drawcommand == 10);
            int y => <-.FixedUIBottom + <-.edit_y;
            int width => parent.clientWidth;
        };
        Read_Mask_UI Read_Mask_UI {
            params => <-.params;
            parent => <-.UImod_panel;
            label => "Load 3D Mask";
            visible => (<-.params.drawcommand == 11);
            int y => <-.FixedUIBottom + <-.edit_y;
            int width => parent.clientWidth;
        };
    };

    macro SegmentationEditorFunc {
        ilink View;
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorParams+IPort2 &params;
        Mesh_Unif+Node_Data& in;
        Mesh_Unif+Node_Data+OPort out;
        Mesh_Unif+Node_Data+OPort2 mask3D;

        DVM.DVorthoslice_unif orthoslice {
            in => <-.in;
            &axis => <-.params.axis;
            &plane => <-.params.slice;
        };
        
        DV.DVnode_data_labels DVnode_data_labels {
            in => <-.mask3D;
            labels => <-.params.entities;
        };

        Mesh_Unif+Node_Data+IPort &slice => .orthoslice.out;

        MODS.reset_xform reset_xform {
            in_field => <-.orthoslice.out;
            obj {
                Obj {
                    xform_mode = "Parent";
                };
            };
        };
        Mesh_Unif+Node_Data+OPort mask {
            dims => <-.slice.dims;
            ndim => <-.slice.ndim;
            nspace => <-.slice.nspace;
            nnode_data = 1;
            node_data = {
                {
                    veclen=1,,,,,,,,
                }
            };
            points => <-.slice.points;
        };

        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.DrawMask DrawMask {
            view => <-.View;
            x+nres => view.two_point.x;
            y+nres => view.two_point.y;
            in_slice => <-.slice;
            slice => <-.params.slice;
            axis => <-.params.axis;
            state+nres => view.two_point.state;
            obj => <-.reset_xform.out_obj;
            value => <-.params.value;
            mask => <-.mask;
            width => <-.params.width;
            command => <-.params.drawcommand;
            pen => <-.params.pen;
            thr_min => <-.params.thr_min;
            thr_max => <-.params.thr_max;
            thr_comp => <-.params.thr_comp;
            thr_trig => <-.params.thr_trig;
            reg_delta => <-.params.reg_delta;
            reg_incr => <-.params.reg_incr;
            reg_trig => <-.params.reg_trig;
            regcmd => <-.params.applyregcommand;
        };
        GDM.DataObject DataObject {
            in => <-.mask;
            Datamap {
                currentColorModel = 1;
                DatamapValue = {
                    {
                    v2=1.,
                    },
                };
            };
            Modes {
                normals = "None";
            };
            Props {
                material = {0.3, 0.7, 0.4, 12.};
                inherit = 0;
                trans = 0.5;
            };
            Obj {
                xform_mode = "Parent";
            };
        };
        GDM.GroupObject GroupObject {
            child_objs => {
                <-.reset_xform.out_obj,
                <-.DataObject.obj
            };
        };
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorCore SegmentationEditorCore {
            axis => <-.params.axis;
            slice => <-.params.slice;
            value => <-.params.value;
            value_nd => <-.params.value_nd;
            draw => <-.DrawMask.draw;
            clear => <-.params.clear_volume;
            clearMask3D => <-.params.clear_mask3D;
            command => <-.params.applycommand;
            regcmd=> <-.params.applyregcommand;
            entityList => <-.params.edit_selentities;
            combineEntity => <-.params.combine_entity;
            clearEntity => <-.params.delete_entity;
            outEntities => <-.params.outentities;
            in_field => <-.in;
            mask => <-.mask;
            mask3D => <-.mask3D;
            seed => <-.DrawMask.seed_point;
            reg_delta => <-.params.reg_delta;
            reg_incr => <-.params.reg_incr;
            grow_region_3D => (<-.params.drawcommand==8);
            label => <-.params.label;
            out => <-.out;
        };
        olink mask_obj => GroupObject.obj;
        AddMultitpleNodeData AddMultitpleNodeData {
            in => <-.in;
            data => <-.mask3D;
        };
        DVM.DVswitch DVswitch {
            in => {<-.AddMultitpleNodeData.out,<-.out};
            index => <-.params.applycommand;
        };
        olink volume => DVswitch.out;

        HDF5.H5write_field write_field {
            filename+nonotify => <-.params.filename;
            compress = 1;
            inFld+nonotify => <-.mask3D;
            int+read+req trigger => <-.params.start_write;
            omethod+notify_val+notify_inst update<status=1> = "HDF5write_field_update";
        };

        ReadHDF5Mask ReadHDF5Mask {
            in     => <-.mask3D;
            params => <-.params;
        };
/*
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.GrowRegion GrowRegion3D {
            dims => in.dims;
            in => <-.in;
            seed => <-.DrawMask.seed_point;
            combine_and_or = {1};
            delta => {<-.params.reg_delta};
            use_diagonals = 0;
            set_null_value = 1;
            and_precedence = 1;
            invert_region = 0;
            out_val => <-.params.value;
            out_label => <-.params.label;
            trigger => (<-.params.drawcommand == 8);
            incremental => {<-.params.reg_incr};
        };
*/
    };

    macro SegmentationEditor {
        Mesh_Unif+Node_Data+IPort2 &in;
        ilink View;
        XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorParams params {
            dims => <-.in.dims;
            value => dims[slice]/2;
            slice = 0;
            applycommand = "Apply Mask";
            pen = "Draw";
            width = 10;
            clear_volume = 0;
            applyregcommand = "Add to Mask";
            clear_mask3D = 0;
            reg_delta = 30;
            edit_entity = 0;
            reg_incr = 0;
            value_nd = 0;
            thr_min => cache(in.node_data[thr_comp].min_vec[0]);
            thr_max => cache(in.node_data[thr_comp].max_vec[0]);
            thr_comp=0;
            thr_trig=0;
            reg_trig=1;
            allow_read=0;
            start_read=0;
         };
        SegmentationEditorUI SegmentationEditorUI {
            in => <-.in;
            mask3D => <-.SegmentationEditorFunc.mask3D;
            params => <-.params;
        };
        SegmentationEditorFunc SegmentationEditorFunc {
            params => <-.params;
            in => <-.in;
            View => <-.View;
        };
        mask_display mask_display {
            params => <-.params;
            in_field => <-.SegmentationEditorFunc.mask3D;
        };
        mask2D_display mask2D_display {
            in_field => <-.SegmentationEditorFunc.mask;
        };
        out_display out_display {
            out_add_mask {
                parent => UIframe;
                int reg2_y => switch((<-.<-.SegmentationEditorUI.RegionGrow2D.visible+1),0,<-.<-.SegmentationEditorUI.RegionGrow2D.height);
                int reg3_y => switch((<-.<-.SegmentationEditorUI.RegionGrow3D.visible+1),0,<-.<-.SegmentationEditorUI.RegionGrow3D.height);
                int thr_y => switch((<-.<-.SegmentationEditorUI.ThresholdUI.visible+1),0,<-.<-.SegmentationEditorUI.ThresholdUI.height);
                int load_y => switch((<-.<-.SegmentationEditorUI.Read_Mask_UI.visible+1),0,<-.<-.SegmentationEditorUI.Read_Mask_UI.height);
                int save_y => switch((<-.<-.SegmentationEditorUI.Store_Mask_UI.visible+1),0,<-.<-.SegmentationEditorUI.Store_Mask_UI.height);
                int edit_y => <-.<-.SegmentationEditorUI.edit_y;
                UIframe UIframe {
                    y => <-.<-.<-.SegmentationEditorUI.Draw_Mask.y + <-.<-.<-.SegmentationEditorUI.Draw_Mask.height + <-.reg2_y + <-.reg3_y + <-.thr_y + <-.load_y + <-.save_y + <-.edit_y;
                    parent => <-.<-.<-.SegmentationEditorUI.UImod_panel;
                    width=>parent.clientWidth;
                    height => <-.UIoptionBoxLabel.y + <-.UIoptionBoxLabel.height + 8;
                };

            };
            ilink in_field=> <-.SegmentationEditorFunc.volume;
            ilink mask3D => <-.SegmentationEditorFunc.mask3D;
            params => <-.params;
        };
        olink out =>  SegmentationEditorFunc.volume;
        olink obj =>  out_display.obj;
        olink mask_obj => SegmentationEditorFunc.mask_obj;
        olink mask3D_obj => mask_display.obj;
        olink mask2D_obj => mask2D_display.obj;
    };
#else
	// if we are using AVS/Express version 6.3 use internal version of the 
	// Segmentation Editor it's the same anyway
	// replace the core function as the 6.3 internal one is buggy  ... oops
	SEGM_EDTR.SegmentationEditor SegmentationEditor {
		SegmentationEditorFunc {
			XP_COMM_PROJ.SegmentationEditor.SegmentationEditorMods.SegmentationEditorCore SegmentationEditorCore {
            	axis => <-.params.axis;
            	slice => <-.params.slice;
            	value => <-.params.value;
            	value_nd => <-.params.value_nd;
            	draw => <-.DrawMask.draw;
            	clear => <-.params.clear_volume;
            	clearMask3D => <-.params.clear_mask3D;
            	command => <-.params.applycommand;
            	regcmd=> <-.params.applyregcommand;
            	entityList => <-.params.edit_selentities;
            	combineEntity => <-.params.combine_entity;
            	clearEntity => <-.params.delete_entity;
            	outEntities => <-.params.outentities;
            	in_field => <-.in;
            	mask => <-.mask;
            	mask3D => <-.mask3D;
            	seed => <-.DrawMask.seed_point;
            	reg_delta => <-.params.reg_delta;
            	reg_incr => <-.params.reg_incr;
            	grow_region_3D => (<-.params.drawcommand==8);
            	label => <-.params.label;
            	out => <-.out;
        	};
		};
	};
#endif

    macro SegmentationWizard {
        ilink in;
        int+IPort2 visible;
        macro UI<hconnect=2>{
            AU.AUtoolbar toolbar<hconnect=2> {
                x = 0;
                y = 0;
                height = 27;
                parent => <-.toolframe;
                tip_placement = 0;
            };
            UIshell SegmentationEditor {
                x = 6;
                y = 215;
                visible => <-.<-.visible;
                width = 1206;
                height = 819;
            };
            UIscrolledWindow UIscrolledWindow {
                parent => <-.SegmentationEditor;
                virtualHeight => 1500;
#ifdef MSDOS
                virtualWidth =>  (parent.clientWidth/3)-20;//switch(((300 < .clientWidth) + 1),300,.clientWidth);
#else
                virtualWidth =>  .clientWidth;
#endif
                y => outputframe.y;
                width => parent.clientWidth/3;
                height => parent.clientHeight - y;
            };
            UIframe outputframe {
                parent => <-.SegmentationEditor;
                y => (<-.toolframe.y + <-.toolframe.height);
                width => (parent.clientWidth - x);
                height => (parent.clientHeight - .y);
                x => <-.toolframe.x;
            };
            UIframe toolframe {
                parent => <-.SegmentationEditor;
                y = 0;
                x => parent.clientWidth/3;
                width => (parent.clientWidth - x);
                height = 33;
            };
            UIframe titleframe {
                parent => <-.SegmentationEditor;
                y = 0;
                width => parent.clientWidth/3;
                height => <-.toolframe.height;
            };
            UIlabel UIlabel {
                parent => <-.titleframe;
                label => <-.<-.Uviewer2D.scene_labels[<-.<-.Uviewer2D.selectded_view];
                y => (parent.clientHeight - height);
                width => parent.clientWidth;
            };
        };
        SegmentationEditor SegmentationEditor {
            in => <-.in;
            View => <-.Uviewer2D.Scene.View.View;
            params {
                axis = 2;
                slice = 20;
                drawcommand = "Draw Point";
                pen = "Draw";
                width = 10;
                value = 4;
            };
            SegmentationEditorUI {
                UIframe UImod_panel {
                    parent => <-.<-.<-.UI.UIscrolledWindow;
                    width => parent.virtualWidth;
                    height => 1500;
                };
            };
        };
        GDM.Uviewer2D Uviewer2D {
            int selectded_view => Scene.View.View.selected+2*Scene#1.View.View.selected+3*Scene#2.View.View.selected+4*Scene#3.View.View.selected;
            string scene_labels[5] =>{"Segmentation Editor","Draw Mask","2D Mask","3D Mask","Segmentation Editor Output"}; 
            Scene {
                Top {
                    child_objs <export=0> => {
                        <-.<-.<-.SegmentationEditor.mask_obj};
                };
                View {
                    View <export=0> {
                        light_info<export=0>;
                        cameras<export=0>;
                        buffers<export=0>;
                        picked_obj<export=0>;
                        renderer = "OpenGL";
                        trigger = 2;
                    };
                    FieldOutput {
                        buffers<export=0>;
                    };
                    ViewUI {
                        PickInteractor {
                            startEvent = "<Btn3Down>";
                            runEvent = "<Btn3Motion>";
                            stopEvent = "<Btn3Up>";
                        };
                        ViewPanel {
                             UI{ 
                                panel {
                                    width => ((parent.width <+> dw) / 2);
                                    parent => <-.<-.<-.<-.<-.<-.<-.UI.outputframe;
                                    height => ((parent.height <+> dh) / 2);
                                };
                            };
                        };
                    };
                };
                Lights {
                    LightInfo<export=0>;
                    nlights<export=0>;
                };
                Camera {
                    Camera<export=0>{
                        objects<export=0>;
                    };
                };
            };
            Scene_Selector {
                input_views<export=0> => {Scene.View.View,
                    <-.Scene#1.View.View,<-.Scene#2.View.View,<-.Scene#3.View.View};
                curr_view<export=0>;
            };
            Scene_Editor {
                View_Editor {
                    GDobj_sel {
                        cur_obj<export=0>;
                    };
                };
            };
            GDM.Uscene2D Scene#1 {
                Top {
                    child_objs <export=0> => {
                        <-.<-.<-.SegmentationEditor.mask2D_obj};
                };
                View {
                    View<export=0>{
                        light_info<export=0>;
                        cameras<export=0>;
                        buffers<export=0>;
                        picked_obj<export=0>;
                    };
                    FieldOutput {
                        buffers<export=0>;
                    };
                    ViewUI {
                        ViewPanel {
                            UI {
                                panel {
                                    x => ((parent.width <+> dw) / 2);
                                    parent => <-.<-.<-.<-.<-.<-.<-.UI.outputframe;
                                    width => ((parent.width <+> dw) / 2);
                                    height => ((parent.height <+> dh) / 2);
                                };
                            };
                        };
                    };
                };
                Lights {
                    LightInfo<export=0>;
                    nlights<export=0>;
                };
                Camera {
                    Camera<export=0>{
                        objects<export=0>;
                    };
                };
            };
            GDM.Uscene3D Scene#2 {
                Top {
                    child_objs <export=0> => {
                        <-.<-.<-.SegmentationEditor.mask3D_obj};
                };
                View {
                    View<export=0>{
                        light_info<export=0>;
                        cameras<export=0>;
                        buffers<export=0>;
                        picked_obj<export=0>;
                    };
                    ViewUI {
                        ViewPanel {
                            UI {
                                panel {
                                    y => ((parent.height <+> dh) / 2);
                                    parent => <-.<-.<-.<-.<-.<-.<-.UI.outputframe;
                                    width => ((parent.width <+> dw) / 2);
                                    height => ((parent.height <+> dh) / 2);
                                };
                            };
                        };
                    };
                    FieldOutput {
                        buffers<export=0>;
                    };
                };
                Lights {
                    LightInfo<export=0>;
                    nlights<export=0>;
                };
                Camera {
                    Camera<export=0>{
                        objects<export=0>;
                    };
                };
            };
            GDM.Uscene3D Scene#3 {
                Top {
                    child_objs <export=0>=> {
                        <-.<-.<-.SegmentationEditor.obj};
                };
                View <export=0>{
                    View<export=0>{
                        light_info<export=0>;
                        cameras<export=0>;
                        buffers<export=0>;
                        picked_obj<export=0>;
                    };
                    FieldOutput {
                        buffers<export=0>;
                    };
                    ViewUI {
                        ViewPanel {
                            UI {
                                panel {
                                    x => ((parent.width <+> dw) / 2);
                                    y => ((parent.height <+> dh) / 2);
                                    width => ((parent.width <+> dw) / 2);
                                    height => ((parent.height <+> dh) / 2);
                                    parent => <-.<-.<-.<-.<-.<-.<-.UI.outputframe;
                                };
                            };
                        };
                    };
                };
                Lights {
                    LightInfo<export=0>;
                    nlights<export=0>;
                };
                Camera {
                    Camera<export=0>{
                        objects<export=0>;
                    };
                };
            };
        };
        olink out<export=1> => SegmentationEditor.out;
        olink obj<export=1> => SegmentationEditor.obj;
    };
	
};
