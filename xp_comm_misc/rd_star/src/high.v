library star_high<compile_subs=0> {
      group readSTARparams {
         int trigger<NEportLevels={2,2}>;
         string case<NEportLevels={2,2}>;
         int load_type<NEportLevels={2,2}>;
         int cdconv<NEportLevels={2,2}>;
         int cdsel<NEportLevels={2,2}>[];
         int pconv<NEportLevels={2,2}>;
         int tconv<NEportLevels={2,2}>;
         int wdconv<NEportLevels={2,2}>;
         int wdsel<NEportLevels={2,2}>[];
         int trtype<NEportLevels={2,2}>;
         int trstep<NEportLevels={2,2}>;
         float trtime<NEportLevels={2,2}>;
      };
      macro readSTARdialog<NEx=231.,NEy=220.,NEscalingFactor=1.094688177,NExOffset=209.,NEyOffset=524.7637329> {
         UImod_panel UImod_panel<NEx=-209.,NEy=-506.> {
            x = 3;
            width = 249;
            title => "Import STAR Data";
            parent<NEportLevels={3,0}>;
            option {
               set = 1;
            };
         };
         UIlabel CaseLabel<NEx=-11.,NEy=-308.> {
            x = 3;
            y => (<-.CaseText.y + ((<-.CaseText.height - .height) / 2));
            width = 80;
            parent => <-.CasePanel;
            label => "Case";
            alignment = "left";
         };
         UItext CaseText<NEx=-11.,NEy=-264.> {
            x => ((<-.CaseLabel.x + <-.CaseLabel.width) + 3);
            y = 3;
            width = 150;
            height = 35;
            parent => <-.CasePanel;
            text => <-.param.case;
            updateMode = 3;
         };
         UIoptionMenu ResultsMenu<NEx=-209.,NEy=-363.> {
            x = 1;
            y = 48;
            parent => <-.SetupFrame;
            label => "Load Results";
            cmdList => {<-.noResultOpt,
               <-.stResultOpt,<-.trResultOpt};
            selectedItem => <-.param.load_type;
         };
         UIoption noResultOpt<NEx=-198.,NEy=-330.> {
            label => "None";
            set = 1;
         };
         UIoption stResultOpt<NEx=-198.,NEy=-297.> {
            label => "Steady State";
         };
         UIoption trResultOpt<NEx=-198.,NEy=-264.> {
            label => "Transient";
         };
         UIpanel CasePanel<NEx=-11.,NEy=-374.> {
            y = 0;
            width = 240;
            height = 47;
            parent => <-.SetupFrame;
         };
         UIframe SetupFrame<NEx=-121.,NEy=-451.> {
            x = 1;
            y = 110;
            width = 244;
            height = 295;
            parent => <-.UImod_panel;
         };
         UIframe DataFrame<NEx=187.,NEy=-451.> {
            y = 410;
            width = 244;
            height = 850;
            clientHeight = 854;
            parent => <-.UImod_panel;
         };
         UIpanel CDPanel<NEx=209.,NEy=-374.> {
            y = 1;
            width = 236;
            height = 428;
            clientHeight = 428;
            parent => <-.DataFrame;
         };
         UIpanel WDPanel<NEx=517.,NEy=-374.> {
            y = 420;
            width = 236;
            height = 432;
            parent => <-.DataFrame;
         };
         UIoptionMenu cdConvMenu<NEx=209.,NEy=-308.> {
            x = 3;
            y = 3;
            width => parent.clientWidth;
            parent => <-.CDPanel;
            label => "Get Cell Data";
            cmdList => {<-.cdNoneOpt,
               <-.cdScalOpt,<-.cdFluxOpt};
            selectedItem => <-.param.cdconv;
            alignment = "left";
         };
         UIoption cdNoneOpt<NEx=220.,NEy=-275.> {
            label => "None";
            set = 1;
         };
         UIoption cdScalOpt<NEx=220.,NEy=-242.> {
            label => "Scalar";
         };
         UIoption cdFluxOpt<NEx=220.,NEy=-209.> {
            label => "Flux";
         };
         macro cdScalMacro<NEx=209.,NEy=-110.> {
            ilink cdScalPanel<NEportLevels={2,1}> => <-.cdScalPanel;
            ilink param<NEportLevels={2,1},NEx=253.,NEy=44.> => <-.param;
            UIframe cdselScalFrame<NEx=517.,NEy=99.> {
               y = 0;
               width = 120;
               height = 370;
               parent => <-.cdScalPanel;
            };
            string cdselScalLabels<NEportLevels={1,1},NEx=517.,NEy=143.>[] = {
               "U","V","W","P","TE","ED","VIS","T","DENS","LAMV","CP","COND",
               "ENTH","CONC"
            };
            UIoptionBoxLabel cdScalMenu<NEx=517.,NEy=187.> {
               parent => <-.cdselScalFrame;
               labels => <-.cdselScalLabels;
               title => "Scalar Cell Data";
               selectedItems => <-.param.cdsel;
               width => parent.clientWidth;
            };
            UIframe pconvFrame<NEx=209.,NEy=253.> {
               x => (parent.clientWidth - .width);
               parent => <-.cdScalPanel;
               y = 0;
               height = 90;
            };
            UIframe tconvFrame<NEx=363.,NEy=253.> {
               x => (parent.clientWidth - .width);
               parent => <-.cdScalPanel;
               y => ((<-.pconvFrame.y + <-.pconvFrame.height) + 3);
               height = 90;
            };
            string convLabels<NEportLevels=1,NEx=209.,NEy=297.>[] = {"Relative",
               "Absolute"};
            UIradioBoxLabel pconvMenu<NEx=209.,NEy=341.> {
               parent => <-.pconvFrame;
               labels => <-.convLabels;
               selectedItem => <-.param.pconv;
               title => "Pressure";
               width => parent.clientWidth;
            };
            UIradioBoxLabel tconvMenu<NEx=363.,NEy=341.> {
               parent => <-.tconvFrame;
               labels => <-.convLabels;
               selectedItem => <-.param.tconv;
               title => "Temperature";
               width => parent.clientWidth;
            };
         };
         GMOD.instancer cdScal_inst {
            Value => <-.cdScalPanel.visible;
            Group => <-.cdScalMacro;
         };
         UIpanel cdScalPanel<NEx=209.,NEy=-154.> {
            x = 3;
            y => ((<-.cdConvMenu.y + <-.cdConvMenu.height) + 5);
            width => (parent.clientWidth - (2 * .x));
            parent => <-.CDPanel;
            height = 380;
            visible => ((<-.cdConvMenu.selectedItem & 1) && 1);
         };
         UIoptionMenu wdConvMenu<NEx=484.,NEy=-330.> {
            x = 3;
            y = 10;
            width => parent.clientWidth;
            parent => <-.WDPanel;
            label => "Get Wall Data";
            cmdList => {<-.wdNoneOpt,
               <-.wdConvOpt};
            selectedItem => <-.param.wdconv;
            alignment = "left";
         };
         UIoption wdNoneOpt<NEx=495.,NEy=-297.> {
            label => "None";
            set = 1;
         };
         UIoption wdConvOpt<NEx=495.,NEy=-264.> {
            label => "Convert";
         };
         UIpanel cdFluxPanel<NEx=231.,NEy=-66.> {
            x = 3;
            y => ((<-.cdConvMenu.y + <-.cdConvMenu.height) + 5);
            width => (parent.clientWidth - (2 * .x));
            parent => <-.CDPanel;
            height => parent.clientHeight;
            visible => ((<-.cdConvMenu.selectedItem & 2) && 1);
         };
         macro cdFluxMacro<NEx=231.,NEy=-22.> {
            ilink cdFluxPanel<NEportLevels={2,1}> => <-.cdFluxPanel;
            ilink param<NEportLevels={2,1}> => <-.param;
            UIframe cdselFluxFrame<NEx=517.,NEy=99.> {
               y = 0;
               height = 370;
               parent => <-.cdFluxPanel;
            };
            string cdselFluxLabels<NEportLevels={1,1},NEx=517.,NEy=143.>[] = {
               "F1","F2","F3","F4","F5","F6"
            };
            UIoptionBoxLabel cdFluxMenu<NEx=517.,NEy=187.> {
               parent => <-.cdselFluxFrame;
               labels => <-.cdselFluxLabels;
               title => "Fluxes Cell Data";
               selectedItems => <-.param.cdsel;
               width => parent.clientWidth;
            };
         };
         GMOD.instancer cdFlux_inst {
            Value => <-.cdFluxPanel.visible;
            Group => <-.cdFluxMacro;
         };
         UIpanel wdConvPanel<NEx=495.,NEy=-198.> {
            x = 3;
            y => ((<-.wdConvMenu.y + <-.wdConvMenu.height) + 5);
            width => (parent.clientWidth - (2 * .x));
            parent => <-.WDPanel;
            height => parent.clientHeight;
            visible => ((<-.wdConvMenu.selectedItem & 1) && 1);
         };
         UIframe wdselConvFrame<NEx=495.,NEy=-154.> {
            y = 0;
            height = 370;
            parent => <-.wdConvPanel;
         };
         UIoptionBoxLabel WdataMenu<NEx=506.,NEy=-66.> {
            parent => <-.wdselConvFrame;
            labels => <-.wdselConvLabels;
            title => "Wall Data";
            selectedItems => <-.param.wdsel;
            width => parent.clientWidth;
         };
         string wdselConvLabels<NEportLevels={1,1},NEx=440.,NEy=-110.>[] = {
            "SFX","SFY","SFZ","YPLU","DIST","TEMP","HTRA","MTRA","HFLU",
            "MFLU","TIRA","TRAD","SIRA","SRAD"
         };
         readSTARparams &param<NEx=352.,NEy=-506.,NEportLevels={2,0},NEwidth=209.,NEheight=440.> {
            case = "";
            load_type = 0;
            cdconv = 0;
            wdconv = 0;
            int trtype<NEportLevels={2,2}> = 0;
            int trstep<NEportLevels={2,2}> = 0;
            float trtime<NEportLevels={2,2}> = 0.;
         };
         UIslider TimeStepSel<NEx=-11.,NEy=44.> {
            x = 12;
            y = 180;
            visible => (<-.TimeStepNoOpt.set && <-.trResultOpt.set);
            parent => <-.SetupFrame;
            min = 0.;
            max = 1.;
            value<NEportLevels={2,2}> => <-.param.trtime;
            decimalPoints = 3;
            title => "Time Step Selector";
         };
         UIslider IterSel<NEx=-11.,NEy=0.> {
            x = 12;
            y = 124;
            visible => (<-.IterationNoOpt.set && <-.trResultOpt.set);
            parent => <-.SetupFrame;
            min = 0.;
            max = 150.;
            value<NEportLevels={2,2},NEdisplayMode="open"> => <-.param.trstep;
            decimalPoints = 0;
            title => "Iteration Selector";
            increment = 10.;
         };
         UIoptionMenu TransMode<NEx=-209.,NEy=-176.> {
            x = 0;
            y = 88;
            visible => <-.trResultOpt.set;
            parent => <-.SetupFrame;
            label => "Transient Mode";
            cmdList => {<-.FirstOpt,
               <-.LastOpt,<-.IterationNoOpt,<-.TimeStepNoOpt};
            selectedItem => <-.param.trtype;
         };
         UIoption FirstOpt<NEx=-198.,NEy=-143.> {
            label => "First";
            set = 1;
         };
         UIoption LastOpt<NEx=-198.,NEy=-110.> {
            label => "Last";
         };
         UIoption IterationNoOpt<NEx=-198.,NEy=-77.> {
            label => "Iteration No";
         };
         UIoption TimeStepNoOpt<NEx=-198.,NEy=-44.> {
            label => "Time Step";
         };
         UIbutton OK_to_load<NEx=-198.,NEy=44.> {
            x = 5;
            y = 240;
            width = 230;
            height = 40;
            parent => <-.SetupFrame;
            do => <-.param.trigger;
         };
         UIlabel UIlabel<NEx=385.,NEy=-176.> {
            parent => <-.UImod_panel;
            height<NEdisplayMode="open"> => 100;
            y<NEdisplayMode="open"> = 10;
            width<NEdisplayMode="open"> => .parent.width;
            labelPixmap<NEdisplayMode="open"> {
               filename<NEdisplayMode="open"> = "../logo.x";
               fileType<NEdisplayMode="open"> = "avs_image";
            };
         };
      };
      macro ReadSTARMacro<NEx=209.,NEy=22.> {
         WORKSPACE_1.star_low.readSTAR readSTAR<NEx=33.,NEy=121.> {
            trigger => <-.readSTARparams.trigger;
            case => <-.readSTARparams.case;
            load_type => <-.readSTARparams.load_type;
            cdconv => <-.readSTARparams.cdconv;
            cdsel => <-.readSTARparams.cdsel;
            pconv => <-.readSTARparams.pconv;
            tconv => <-.readSTARparams.tconv;
            wdconv => <-.readSTARparams.wdconv;
            wdsel => <-.readSTARparams.wdsel;
            trtype => <-.readSTARparams.trtype;
            trstep => <-.readSTARparams.trstep;
            trtime => <-.readSTARparams.trtime;
            out_fld<NEportLevels={0,3}>;
         };
         readSTARparams readSTARparams<NEx=33.,NEy=66.,NEportLevels={0,1}>;
         readSTARdialog readSTARdialog<NEx=341.,NEy=121.> {
            param => <-.readSTARparams;
         };
      };
};

