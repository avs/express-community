
flibrary ViewersLiteMacs {

  //DO_GreyScale
  //DO_VolumeRender
  //DO_HotMetal
  //DO_CyanYellowRed
  //DO_DefaultStep
  //DO_FilterDatamap
  //DO_NT_GreyScale
  //DO_NT_VolumeRender
  //DO_NT_HotMetal
  //DO_NT_CyanYellowRed
  //DO_NT_DefaultStep
  //DO_NT_FilterDatamap
  //DO_LITE_GreyScale
  //DO_LITE_VolumeRender
  //DO_LITE_HotMetal
  //DO_LITE_CyanYellowRed
  //DO_LITE_DefaultStep
  //DO_LITE_FilterDatamap

  //Datamap_Switch

  //DO_Dmap_Switch
  //DO_NT_Dmap_Switch
  //DO_LITE_Dmap_Switch

  //View3DMinimal
  //View3D_TrackObj
  //View3D_TrackAll3
  //View2DMinimal
  //View2D_TrackObj
  //View2D_TrackAll3
  //ViewMinimal
  //View_TrackObj
  //View_TrackAll3

  //EgViewLite

//DO_GreyScale

  macro DO_GreyScale{
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     Node_Data &texture_in<NEcolor2=16776960,NEportLevels={2,1},NEy=44,NEx=231> {
        node_data {
           values+byte;
           null_value+byte;
           min+byte;
           max+byte;
           min_vec+byte;
           max_vec+byte;
        };
     };
     Node_Data &texture_col_in<NEportLevels={2,1},NEy=11,NEx=286>;
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultTexture Texture<NEy=132,NEx=396,export_all=1> {
        data => texture_in;
        colormap => texture_col_in;
     };
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     AltObject AltObject<instanced=0,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.GreyScale;
        xform => in.xform;
        props => Props;
        modes => Modes;
        texture => Texture;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     GreyScale GreyScale<NEx=22.,NEy=253.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };


//DO_VolumeRender

  macro DO_VolumeRender{
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     Node_Data &texture_in<NEcolor2=16776960,NEportLevels={2,1},NEy=44,NEx=231> {
        node_data {
           values+byte;
           null_value+byte;
           min+byte;
           max+byte;
           min_vec+byte;
           max_vec+byte;
        };
     };
     Node_Data &texture_col_in<NEportLevels={2,1},NEy=11,NEx=286>;
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultTexture Texture<NEy=132,NEx=396,export_all=1> {
        data => texture_in;
        colormap => texture_col_in;
     };
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     AltObject AltObject<instanced=0,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.VolumeRender;
        xform => in.xform;
        props => Props;
        modes => Modes;
        texture => Texture;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     VolumeRender VolumeRender<NEx=22.,NEy=253.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };


//DO_HotMetal

  macro DO_HotMetal{
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     Node_Data &texture_in<NEcolor2=16776960,NEportLevels={2,1},NEy=44,NEx=231> {
        node_data {
           values+byte;
           null_value+byte;
           min+byte;
           max+byte;
           min_vec+byte;
           max_vec+byte;
        };
     };
     Node_Data &texture_col_in<NEportLevels={2,1},NEy=11,NEx=286>;
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultTexture Texture<NEy=132,NEx=396,export_all=1> {
        data => texture_in;
        colormap => texture_col_in;
     };
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     AltObject AltObject<instanced=0,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.HotMetal;
        xform => in.xform;
        props => Props;
        modes => Modes;
        texture => Texture;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     HotMetal HotMetal<NEx=22.,NEy=253.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };

//DO_CyanYellowRed

  macro DO_CyanYellowRed {
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     Node_Data &texture_in<NEcolor2=16776960,NEportLevels={2,1},NEy=44,NEx=231> {
        node_data {
           values+byte;
           null_value+byte;
           min+byte;
           max+byte;
           min_vec+byte;
           max_vec+byte;
        };
     };
     Node_Data &texture_col_in<NEportLevels={2,1},NEy=11,NEx=286>;
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultTexture Texture<NEy=132,NEx=396,export_all=1> {
        data => texture_in;
        colormap => texture_col_in;
     };
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     AltObject AltObject<instanced=0,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.CyanYellowRed;
        xform => in.xform;
        props => Props;
        modes => Modes;
        texture => Texture;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     CyanYellowRed CyanYellowRed<NEx=44.,NEy=253.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };

//DO_DefaultStep

  macro DO_DefaultStep {
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     Node_Data &texture_in<NEcolor2=16776960,NEportLevels={2,1},NEy=44,NEx=231> {
        node_data {
           values+byte;
           null_value+byte;
           min+byte;
           max+byte;
           min_vec+byte;
           max_vec+byte;
        };
     };
     Node_Data &texture_col_in<NEportLevels={2,1},NEy=11,NEx=286>;
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultTexture Texture<NEy=132,NEx=396,export_all=1> {
        data => texture_in;
        colormap => texture_col_in;
     };
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     AltObject AltObject<instanced=0,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.DefaultStep;
        xform => in.xform;
        props => Props;
        modes => Modes;
        texture => Texture;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     DefaultStep DefaultStep<NEx=33.,NEy=242.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };

//DO_FilterMap

  macro DO_FilterDatamap {
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     Node_Data &texture_in<NEcolor2=16776960,NEportLevels={2,1},NEy=44,NEx=231> {
        node_data {
           values+byte;
           null_value+byte;
           min+byte;
           max+byte;
           min_vec+byte;
           max_vec+byte;
        };
     };
     Node_Data &texture_col_in<NEportLevels={2,1},NEy=11,NEx=286>;
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultTexture Texture<NEy=132,NEx=396,export_all=1> {
        data => texture_in;
        colormap => texture_col_in;
     };
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     AltObject AltObject<instanced=0,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.FilterDatamap;
        xform => in.xform;
        props => Props;
        modes => Modes;
        texture => Texture;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     FilterDatamap FilterDatamap<NEx=22.,NEy=253.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };

//DO_NT_GreyScale

  macro DO_NT_GreyScale {
     group &in<NEx=22.,NEy=66.,NEportLevels={2,1},NEnumColors=4,NEcolor0=255> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=33.,NEx=88.>;
     DefaultMinMax MinMax<NEx=11.,NEy=110.,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEx=220.,NEy=165.,export_all=1>;
     DefaultModes Modes<NEx=264.,NEy=121.,export_all=1>;
     DefaultPickInfo PickInfo<NEx=297.,NEy=77.,export_all=1>;
     AltObject AltObject<instanced=0,NEx=451.,NEy=88.,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEx=33.,NEy=253.,NEportLevels={0,1},export_all=2> {
        input => in;
        dmap => <-.GreyScale;
        xform => in.xform;
        props => Props;
        modes => Modes;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer<NEx=429.,NEy=33.> {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=308.,NEx=77.> => Obj;
     GreyScale GreyScale<NEx=11.,NEy=176.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };


//DO_NT_VolumeRender

  macro DO_NT_VolumeRender {
     group &in<NEx=22.,NEy=66.,NEportLevels={2,1},NEnumColors=4,NEcolor0=255> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=33.,NEx=88.>;
     DefaultMinMax MinMax<NEx=11.,NEy=110.,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEx=220.,NEy=165.,export_all=1>;
     DefaultModes Modes<NEx=264.,NEy=121.,export_all=1>;
     DefaultPickInfo PickInfo<NEx=297.,NEy=77.,export_all=1>;
     AltObject AltObject<instanced=0,NEx=451.,NEy=88.,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEx=33.,NEy=253.,NEportLevels={0,1},export_all=2> {
        input => in;
        dmap => <-.VolumeRender;
        xform => in.xform;
        props => Props;
        modes => Modes;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer<NEx=429.,NEy=33.> {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=308.,NEx=77.> => Obj;
     VolumeRender VolumeRender<NEx=11.,NEy=154.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };

//DO_NT_HotMetal

  macro DO_NT_HotMetal {
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     AltObject AltObject<instanced=0,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.HotMetal;
        xform => in.xform;
        props => Props;
        modes => Modes;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     HotMetal HotMetal<NEx=33.,NEy=264.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };

//DO_NT_CyanYellowRed

  macro DO_NT_CyanYellowRed {
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     AltObject AltObject<instanced=0,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.CyanYellowRed;
        xform => in.xform;
        props => Props;
        modes => Modes;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     CyanYellowRed CyanYellowRed<NEx=33.,NEy=253.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };


//DO_NT_DefaultStep

  macro DO_NT_DefaultStep{
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     AltObject AltObject<instanced=0,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.DefaultStep;
        xform => in.xform;
        props => Props;
        modes => Modes;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     DefaultStep DefaultStep<NEx=33.,NEy=264.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };

//DO_NT_FilterDatamap

  macro DO_NT_FilterDatamap{
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     AltObject AltObject<instanced=0,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.FilterDatamap;
        xform => in.xform;
        props => Props;
        modes => Modes;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     FilterDatamap FilterDatamap<NEx=33.,NEy=275.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };



//DO_LITE_Default

  macro DO_LITE_Default{
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132,NEportLevels={2,1}>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => <-.in;
     };
     DefaultLinear Datamap<NEy=231,NEx=11,export_all=1> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => <-.in;
        dmap => <-.Datamap;
        xform => in.xform;
        props => <-.Props;
        modes => <-.Modes;
        objects => <-.child_objs;
        name => name_of(<-.<-);
        pick_info => <-.PickInfo;
     };
     GMOD.instancer instancer {
        Value => <-.Obj.use_altobj;
     };
     olink obj<NEy=385,NEx=341,NEportLevels={1,2}> => .Obj;
  };

//DO_LITE_GreyScale

  macro DO_LITE_GreyScale{
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132,NEportLevels={2,1}>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => <-.in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => <-.in;
        dmap => <-.GreyScale;
        xform => in.xform;
        props => <-.Props;
        modes => <-.Modes;
        objects => <-.child_objs;
        name => name_of(<-.<-);
        pick_info => <-.PickInfo;
     };
     GMOD.instancer instancer {
        Value => <-.Obj.use_altobj;
     };
     olink obj<NEy=385,NEx=341,NEportLevels={1,2}> => .Obj;
     GreyScale GreyScale<NEx=33.,NEy=253.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };

//DO_LITE_VolumeRender


  macro DO_LITE_VolumeRender {
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132,NEportLevels={2,1}>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
       input => <-.in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => <-.in;
        dmap => <-.VolumeRender;
        xform => in.xform;
        props => <-.Props;
        modes => <-.Modes;
        objects => <-.child_objs;
        name => name_of(<-.<-);
        pick_info => <-.PickInfo;
     };
     GMOD.instancer instancer {
        Value => <-.Obj.use_altobj;
     };
     olink obj<NEy=385,NEx=341,NEportLevels={1,2}> => .Obj;
     VolumeRender VolumeRender<NEx=44.,NEy=264.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };


//DO_LITE_HotMetal

  macro DO_LITE_HotMetal{
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132,NEportLevels={2,1}>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => <-.in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => <-.in;
        dmap => <-.HotMetal;
        xform => in.xform;
        props => <-.Props;
        modes => <-.Modes;
        objects => <-.child_objs;
        name => name_of(<-.<-);
        pick_info => <-.PickInfo;
     };
     GMOD.instancer instancer {
        Value => <-.Obj.use_altobj;
     };
     olink obj<NEy=385,NEx=341,NEportLevels={1,2}> => .Obj;
     HotMetal HotMetal<NEx=33.,NEy=264.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };

//DO_LITE_CyanYellowRed

  macro DO_LITE_CyanYellowRed{
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132,NEportLevels={2,1}>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => <-.in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => <-.in;
        dmap => <-.CyanYellowRed;
        xform => in.xform;
        props => <-.Props;
        modes => <-.Modes;
        objects => <-.child_objs;
        name => name_of(<-.<-);
        pick_info => <-.PickInfo;
     };
     GMOD.instancer instancer {
        Value => <-.Obj.use_altobj;
     };
     olink obj<NEy=385,NEx=341,NEportLevels={1,2}> => .Obj;
     CyanYellowRed CyanYellowRed<NEx=33.,NEy=253.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };

//DO_LITE_DefaultStep

  macro DO_LITE_DefaultStep{
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132,NEportLevels={2,1}>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => <-.in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => <-.in;
        dmap => <-.DefaultStep;
        xform => in.xform;
        props => <-.Props;
        modes => <-.Modes;
        objects => <-.child_objs;
        name => name_of(<-.<-);
        pick_info => <-.PickInfo;
     };
     GMOD.instancer instancer {
        Value => <-.Obj.use_altobj;
     };
     olink obj<NEy=385,NEx=341,NEportLevels={1,2}> => .Obj;
     DefaultStep DefaultStep<NEx=22.,NEy=253.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };

//DO_LITE_FilterDatamap

  macro DO_LITE_FilterDatamap{
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132,NEportLevels={2,1}>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => <-.in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => <-.in;
        dmap => <-.FilterDatamap;
        xform => in.xform;
        props => <-.Props;
        modes => <-.Modes;
        objects => <-.child_objs;
        name => name_of(<-.<-);
        pick_info => <-.PickInfo;
     };
     GMOD.instancer instancer {
        Value => <-.Obj.use_altobj;
     };
     olink obj<NEy=385,NEx=341,NEportLevels={1,2}> => .Obj;
     FilterDatamap FilterDatamap<NEx=55.,NEy=275.> {
        dataMin => <-.MinMax.min_value;
        dataMax => <-.MinMax.max_value;
     };
  };

//Datamap_Switch


  macro Datamap_Switch {
     DefaultLinear DefaultLinear<NEx=44.,NEy=88.> {
        dataMin => <-.min;
        dataMax => <-.max;
     };
     GreyScale GreyScale<NEx=44.,NEy=132.> {
        dataMin => <-.min;
        dataMax => <-.max;
     };
     VolumeRender VolumeRender<NEx=44.,NEy=176.> {
        dataMin => <-.min;
        dataMax => <-.max;
     };
     HotMetal HotMetal<NEx=44.,NEy=220.> {
        dataMin => <-.min;
        dataMax => <-.max;
     };
     CyanYellowRed CyanYellowRed<NEx=44.,NEy=264.> {
        dataMin => <-.min;
        dataMax => <-.max;
     };
     DefaultStep DefaultStep<NEx=44.,NEy=308.> {
        dataMin => <-.min;
        dataMax => <-.max;
     };
     FilterDatamap FilterDatamap<NEx=44.,NEy=352.> {
        dataMin => <-.min;
        dataMax => <-.max;
     };
     DVM.DVswitch DVswitch<NEx=297.,NEy=308.> {
        in => {
           <-.DefaultLinear,<-.GreyScale,<-.VolumeRender,<-.HotMetal,
           <-.CyanYellowRed,<-.DefaultStep,<-.FilterDatamap
        };
        index<NEportLevels={3,1}> = 0;
        out<NEportLevels={1,3}>;
     };
     link min<NEportLevels={2,1},NEx=121.,NEy=22.>;
     link max<NEportLevels={2,1},NEx=297.,NEy=44.>;
  };




//DO_Dmap_Switch

  macro DO_Dmap_Switch {
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     Node_Data &texture_in<NEcolor2=16776960,NEportLevels={2,1},NEy=44,NEx=231> {
        node_data {
           values+byte;
           null_value+byte;
           min+byte;
           max+byte;
           min_vec+byte;
           max_vec+byte;
        };
     };
     Node_Data &texture_col_in<NEportLevels={2,1},NEy=11,NEx=286>;
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultTexture Texture<NEy=132,NEx=396,export_all=1> {
        data => texture_in;
        colormap => texture_col_in;
     };
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     AltObject AltObject<instanced=0,export_all=1> {
        alt_in => in;
     };
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.Datamap_Switch.DVswitch.out;
        xform => in.xform;
        props => Props;
        modes => Modes;
        texture => Texture;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.Datamap_Switch Datamap_Switch<NEx=33.,NEy=264.> {
        DVswitch {
           index<NEportLevels={4,1}>;
        };
        min => <-.MinMax.min_value;
        max => <-.MinMax.max_value;
     };
  };


//DO_NT_Dmap_Switch

  macro DO_NT_Dmap_Switch {
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=66.,NEx=396.,export_all=1>;
     AltObject AltObject<instanced=0,export_all=1,NEx=605.,NEy=187.> {
        alt_in => in;
     };
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.Datamap_Switch.DVswitch.out;
        xform => in.xform;
        props => Props;
        modes => Modes;
        objects => child_objs;
        altobj => AltObject.obj;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     GMOD.instancer instancer<NEx=539.,NEy=132.> {
        Value => Obj.use_altobj;
        Group => AltObject;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.Datamap_Switch Datamap_Switch<NEx=22.  ,NEy=275.> {
        DVswitch {
           index<NEportLevels={4,1}>;
        };
        min => <-.MinMax.min_value;
        max => <-.MinMax.max_value;
     };
  };

//DO_LITE_Dmap_Switch

  macro DO_LITE_Dmap_Switch {
     group &in<NEportLevels={2,1},NEnumColors=4,NEcolor0=255,NEy=132,NEx=55> {
        GDxform_templ &xform;
        method render;
     };
     imlink child_objs<NEy=77,NEx=132>;
     DefaultMinMax MinMax<NEx=11,NEy=187,export_all=1> {
        input => in;
     };
     DefaultProps Props<NEy=198,NEx=297,export_all=1>;
     DefaultModes Modes<NEy=165,NEx=341,export_all=1>;
     DefaultPickInfo PickInfo<NEy=99,NEx=473,export_all=1>;
     DefaultObject Obj<NEportLevels={0,1},NEy=385,NEx=110,export_all=2> {
        input => in;
        dmap => <-.Datamap_Switch.DVswitch.out;
        xform => in.xform;
        props => Props;
        modes => Modes;
        objects => child_objs;
        name => name_of(<-.<-);
        pick_info => PickInfo;
     };
     olink obj<NEy=385,NEx=341> => Obj;
     XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.Datamap_Switch Datamap_Switch<NEx=0.,NEy=264.> {
        DVswitch {
           index<NEportLevels={4,1}>;
        };
        min => <-.MinMax.min_value;
        max => <-.MinMax.max_value;
     };
  };



//View3DMinimal

  macro View3DMinimal {
     GDM.Mscene3D Mscene3D<NEx=44.,NEy=154.> {
        Top {
           child_objs<NEportLevels={4,1}>;
        };
        View {
           View {
              handle => <-.<-.<-.UIrenderView.handle;
           };
        };
     };
     UIrenderView UIrenderView<NEx=176.,NEy=66.> {
        parent<NEportLevels={3,0}>;
     };
  };


//View3D_TrackObj

  XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.View3DMinimal View3D_TrackObj  {
     Mscene3D<NEx=55.,NEy=176.>;
     UIrenderView<NEx=209.,NEy=99.>;
     GDM.Mtrack_editor Mtrack_editor<NEx=154.,NEy=319.> {
        in_view => <-.Mscene3D.View.View;
        cur_obj => <-.Mscene3D.View.View.picked_obj;
        GDtrack_edit<NEx=99.,NEy=341.> {
           event => <-.UItwoPoint.state;
           x => <-.UItwoPoint.x;
           y => <-.UItwoPoint.y;
           mode<NEportLevels={4,0}>;
        };
        UItwoPoint UItwoPoint<NEx=66.,NEy=231.> {
           view<NEportLevels={3,0}> => <-.<-.UIrenderView;
           x<NEportLevels={0,2}>;
           y<NEportLevels={0,2}>;
           state<NEportLevels={0,2}>;
        };
        GMOD.parse_v parse_v<NEx=286.,NEy=352.> {
           v_commands = "GDtrack_edit { reset = 1; normalize = 1; center = 1; }; ";
           trigger<NEportLevels={4,0}> = 1;
           relative => <-;
        };
     };
  };

  XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.View3D_TrackObj View3D_TrackAll3 {
     Mtrack_editor {
        GDtrack_edit<NEx=77.,NEy=363.> {
           trans_mode<NEportLevels={4,0}>;
        };
     };
  };

//View2DMinimal

  macro View2DMinimal {
     GDM.Mscene2D Mscene2D<NEx=44.,NEy=110.> {
        Top {
           child_objs<NEportLevels={4,1}>;
        };
        View {
           View {
              handle => <-.<-.<-.UIrenderView.handle;
           };
        };
     };
     UIrenderView UIrenderView<NEx=143.,NEy=44.> {
        parent<NEportLevels={3,0}>;
     };
  };

//View2D_TrackObj


  XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.View2DMinimal View2D_TrackObj  {
     Mscene2D<NEx=55.,NEy=176.>;
     UIrenderView<NEx=209.,NEy=99.>;
     GDM.Mtrack_editor Mtrack_editor<NEx=154.,NEy=319.> {
        in_view => <-.Mscene2D.View.View;
        cur_obj => <-.Mscene2D.View.View.picked_obj;
        GDtrack_edit<NEx=99.,NEy=341.> {
           event => <-.UItwoPoint.state;
           x => <-.UItwoPoint.x;
           y => <-.UItwoPoint.y;
           mode<NEportLevels={4,0}>;
        };
        UItwoPoint UItwoPoint<NEx=66.,NEy=231.> {
           view<NEportLevels={3,0}> => <-.<-.UIrenderView;
           x<NEportLevels={0,2}>;
           y<NEportLevels={0,2}>;
           state<NEportLevels={0,2}>;
        };
        GMOD.parse_v parse_v<NEx=286.,NEy=352.> {
           v_commands = "GDtrack_edit { reset = 1; normalize = 1; center = 1; }; ";
           trigger<NEportLevels={4,0}> = 1;
           relative => <-;
        };
     };
  };

//View2D_TrackAll3

  XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.View2D_TrackObj View2D_TrackAll3 {
     Mtrack_editor {
        GDtrack_edit<NEx=77.,NEy=363.> {
           trans_mode<NEportLevels={4,0}>;
        };
     };
  };



//ViewMinimal

  macro ViewMinimal {
     GDM.Mscene Mscene<NEx=11.,NEy=99.> {
        Top {
           child_objs<NEportLevels={4,1}>;
        };
        Top2D {
           child_objs<NEportLevels={4,1}>;
        };
        View {
           View {
              handle => <-.<-.<-.UIrenderView.handle;
           };
        };
     };
     UIrenderView UIrenderView<NEx=88.,NEy=33.> {
        parent<NEportLevels={3,0}>;
     };
  };

//View_TrackObj


  XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.ViewMinimal View_TrackObj  {
     Mscene<NEx=55.,NEy=176.>;
     UIrenderView<NEx=209.,NEy=99.>;
     GDM.Mtrack_editor Mtrack_editor<NEx=154.,NEy=319.> {
        in_view => <-.Mscene.View.View;
        cur_obj => <-.Mscene.View.View.picked_obj;
        GDtrack_edit<NEx=99.,NEy=341.> {
           event => <-.UItwoPoint.state;
           x => <-.UItwoPoint.x;
           y => <-.UItwoPoint.y;
           mode<NEportLevels={4,0}>;
        };
        UItwoPoint UItwoPoint<NEx=66.,NEy=231.> {
           view<NEportLevels={3,0}> => <-.<-.UIrenderView;
           x<NEportLevels={0,2}>;
           y<NEportLevels={0,2}>;
           state<NEportLevels={0,2}>;
        };
        GMOD.parse_v parse_v<NEx=286.,NEy=352.> {
           v_commands = "GDtrack_edit { reset = 1; normalize = 1; center = 1; }; ";
           trigger<NEportLevels={4,0}> = 1;
           relative => <-;
        };
     };
  };

//View_TrackAll3

  XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.View_TrackObj View_TrackAll3 {
     Mtrack_editor {
        GDtrack_edit<NEx=77.,NEy=363.> {
           trans_mode<NEportLevels={4,0}>;
        };
     };
  };




//EgViewLite



  APPS.ModuleStack EgViewLite{
     ModuleStackUI {
        Modules {
           cmdList => {
              <-.<-.Read_Field.read_field_ui.panel.option,
              <-.<-.volume_render.UImod_panel.option,
              <-.<-.bounds.UIpanel.option,
              <-.<-.TextString.UImod_panel.option};
        };
        mod_shell {
           x = 32;
        };
        control_shell {
           cancel = 1;
        };
     };
     XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.DO_LITE_Dmap_Switch DO_LITE_Dmap_Switch<NEx=253.,NEy=209.> {
        in => <-.DVorthoslice_unif.out;
        MinMax<NEx=33.>;
        Datamap_Switch {
           DVswitch {
              index => <-.<-.<-.UIwidgets.UIoptionMenu_Datamap.selectedItem;
           };
        };
     };
     XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.DO_LITE_GreyScale DO_LITE_GreyScale<NEx=55.,NEy=209.> {
        in => <-.DViso.out;
     };
     MODS.Read_Field Read_Field<NEx=44.,NEy=11.> {
        read_field_ui {
           file_browser {
              x = 490;
              y = 267;
              width = 300;
              height = 390;
              ok = 1;
              dirMaskCache = "$XP_ROOT/data/field/*.fld";
           };
           filename = "$XP_ROOT/data/field/hydrogen.fld";
        };
        DVread_field<NEx=671.,NEy=55.> {
           Mesh_Unif+Node_Data Output_Field;
        };
        field<NEx=627.,NEy=242.>;
        one_time_end<NEx=308.,NEy=374.>;
        DataObject {
           AltObject<NEx=220.,NEy=121.>;
        };
        link field2<NEportLevels={1,2},NEx=605.,NEy=396.> => .DVread_field.out;
     };
     UIshell UIshell<NEx=627.,NEy=33.> {
        title => "Viewers Lite";
        x = 340;
        y = 8;
        width = 899;
        height = 937;
     };
     XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.View2DMinimal View2DMinimal<NEx=660.,NEy=396.> {
        Mscene2D {
           Top {
              child_objs => {
                 <-.<-.<-.DO_LITE_HotMetal.obj};
           };
           View {
              View {
                 trigger = 1;
              };
           };
        };
        UIrenderView<NEx=154.> {
           parent => <-.<-.UIshell;
           y => ((<-.<-.UIshell.height / 2) + 30);
           x => (<-.<-.UIshell.width / 2);
           width => (<-.<-.UIshell.width / 2);
           height => ((<-.<-.UIshell.height / 2) - 30);
           message = "This is View2DMinimal";
        };
     };
     XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.View3D_TrackObj View3D_TrackObj<NEx=253.,NEy=396.> {
        Mscene3D {
           Top {
              child_objs => {
                 <-.<-.<-.DO_LITE_Dmap_Switch.obj,
                 <-.<-.<-.DO_LITE_GreyScale.obj,
                 <-.<-.<-.bounds.out_obj};
              Xform {
                 ocenter = {31.5,31.5,31.5};
                 dcenter = {4.,4.,4.};
                 mat = {0.0494097,-0.0478616,0.0276025,0.,0.0341111,-0.00270465,-0.06575,0.,0.0434634,0.0565323,0.0202233,0.,0.,0.,0.,1.
                 };
                 xlate = {-4.,-4.,-4.};
                 center = {31.5,31.5,31.5};
              };
           };
           View {
              View {
                 trigger = 1;
              };
           };
        };
        UIrenderView<NEx=242.> {
           parent => <-.<-.UIshell;
           y => ((<-.<-.UIshell.height / 2) + 67);
           width => (<-.<-.UIshell.width / 2);
           height => ((<-.<-.UIshell.height / 2) - 54);
           message = "This is View3D_TrackObj";
        };
        Mtrack_editor {
           parse_v {
              trigger => <-.<-.<-.UIwidgets.UItoggle_RNC.do;
           };
           GDtrack_edit {
              mode => <-.<-.<-.UIwidgets.UIoptionMenuTransf.selectedItem;
           };
        };
     };
     XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.View_TrackAll3 View_TrackAll3<NEx=55.,NEy=396.> {
        Mscene {
           Top {
              child_objs => {
                 <-.<-.<-.DO_LITE_GreyScale.obj};
              Xform {
                 ocenter = {31.5,31.5,31.5};
                 dcenter = {4.44333,4.44333,4.44333};
                 mat = {0.141058,0.,0.,0.,0.,0.141058,0.,0.,0.,0.,0.141058,0.,
0.,0.,0.,1.
                 };
                 xlate = {-4.44333,-4.44333,-4.44333};
                 center = {31.5,31.5,31.5};
              };
           };
           View {
              View {
                 trigger = 1;
              };
           };
           Top2D {
              child_objs => {
                 <-.<-.<-.TextString.DefaultObject};
           };
           Lights {
              LightXform = {
                 {
                    mat=
                    {0.0382917,0.672109,0.739462,0.,0.396513,0.669033,-0.628628,0.,-0.91723,0.317278,-0.240882,0.,0.,0.,0.,1.
                    }
                 },
                 ,,};
           };
        };
        UIrenderView {
           parent => <-.<-.UIshell;
           y = 34;
           width => ((<-.<-.UIshell.width / 2) - 30);
           height => ((<-.<-.UIshell.height / 2) - 64);
           message = "This is a View_TrackALL3";
        };
        Mtrack_editor {
           parse_v {
              trigger => <-.<-.<-.UIwidgets.UItoggle_RNC.do;
           };
           GDtrack_edit {
              mode => <-.<-.<-.UIwidgets.UIoptionMenuTransf#1.selectedItem;
              trans_mode => <-.<-.<-.UIwidgets.UIoptionMenuSelect.selectedItem;
           };
        };
     };
     DViso DViso<NEx=55.,NEy=110.> {
        in => <-.Read_Field.field;
        level => <-.params.isolevel;
        cell_data = {0};
        component = 0;
     };
     DVorthoslice_unif DVorthoslice_unif<NEx=264.,NEy=110.> {
        in => <-.Read_Field.field;
        axis => <-.params.axis;
        plane => <-.params.plane;
      };
     XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.View3D_TrackObj View3D_TrackObj#1<NEx=451.,NEy=396.> {
        Mscene3D {
           Top {
              child_objs => {
                 <-.<-.<-.volume_render.DataObject.obj};
              Xform {
                 ocenter = {31.5,31.5,31.5};
                 dcenter = {4.,4.,4.};
                 mat = {0.126984,0.,0.,0.,0.,0.126984,0.,0.,0.,0.,0.126984,0.,
0.,0.,0.,1.
                 };
                 xlate = {-4.,-4.,-4.};
                 center = {31.5,31.5,31.5};
              };
           };
           View {
              View<NEdisplayMode="open",NEx=308.,NEy=297.> {
                 renderer => "Software";
                 trigger = 1;
              };
           };
        };
        UIrenderView {
           parent => <-.<-.UIshell;
           y = 34;
           x => (<-.<-.UIshell.width / 2);
           width => (<-.<-.UIshell.width / 2);
           height => ((<-.<-.UIshell.height / 2) - 64);
           message = "This is the View3D_TrackObj";
        };
        Mtrack_editor {
           parse_v<NEy=341.> {
              trigger => <-.<-.<-.UIwidgets.UIbutton_RNC.do;
           };
           GDtrack_edit {
              mode => <-.<-.<-.UIwidgets.UIoptionMenuTransf#2.selectedItem;
           };
        };
     };
     MODS.volume_render volume_render<NEx=462.,NEy=110.> {
        DataObject<NEx=33.,NEy=451.> {
           Props {
              fat_ray = 0;
              inherit = 0;
           };
           InvisibleAltObj<NEx=517.>;
           Obj<NEx=121.,NEy=407.>;
        };
        GDprops_edit {
           ray_algo = "Direct Composite";
           fat_ray = 0;
           inherit = 0;
        };
        inherit {
           set = 1;
        };
        surf_rb {
           x = 0;
        };
        vol_rb {
           x = 0;
        };
        interp_tri {
           set = 1;
        };
        interp_rb {
           x = 0;
        };
        ray_direct {
           set = 1;
        };
        ray_rb {
           x = 0;
        };
        dist_glob {
           set = 1;
        };
         dist_rb {
           x = 0;
        };
        alpha_const {
           set = 1;
        };
        alpha_rb {
           x = 0;
        };
        UImod_panel {
           option {
              set = 1;
           };
        };
        GDmodes_edit {
           volume = "Ray Tracer";
        };
        ray_tracer {
           set = 1;
        };
        in_mesh => <-.Read_Field.field2;
     };
     XP_COMM_PROJ.ViewersLite.ViewersLiteMacs.DO_LITE_HotMetal DO_LITE_HotMetal<NEx=649.,NEy=275.> {
        in => <-.reset_xform.out_fld;
     };
     MODS.bounds bounds<NEx=286.,NEy=264.> {
        in_field => <-.Read_Field.field;
     };
     GEOMS.TextString TextString<NEx=99.,NEy=286.> {
        TextUI {
           Offset {
              set = 1;
           };
           OffsetXValue {
              x = 0;
           };
           OffsetYValue {
              x = 0;
           };
           OffsetZValue {
              x = 0;
           };
           String {
              text = "\nText does not move\n\n\n\n\n\n";
           };
           AlignHorizMenu {
              selectedItem = 2;
           };
           Radial {
              set = 1;
           };
           UIoption {
              set = 0;
           };
           UIoption#2 {
              set = 1;
           };
        };
     };
     macro UIwidgets<NEx=638.,NEy=121.,NEwidth=561.,NEheight=220.> {
        UIslider UIslider_iso<NEx=22.,NEy=66.,NEwidth=231.,NEheight=231.> {
           parent => <-.link_UIshell;
           value => <-.<-.params.isolevel;
           x => ((<-.<-.UIshell.width / 4) - 100);
           y => ((<-.<-.UIshell.height / 2) - 30);
           min => <-.<-.link_fld.node_data[0].min;
           max => <-.<-.link_fld.node_data[0].max;
        };
        UIslider UIslider_axis<NEx=264.,NEy=55.> {
           parent => <-.link_UIshell;
           value => <-.<-.params.axis;
           x => (<-.<-.UIshell.width / 2);
           y => ((<-.<-.UIshell.height / 2) - 30);
           min = 0.;
           max => (<-.<-.link_fld.ndim - 1);
           mode = "integer";
        };
        UIslider UIslider_plane<NEx=286.,NEy=99.> {
           parent => <-.link_UIshell;
           value => <-.<-.params.plane;
           x => (<-.<-.UIshell.width - 200);
           y => ((<-.<-.UIshell.height / 2) - 30);
           min = 0.;
           max => (<-.<-.link_fld.dims[<-.<-.params.axis] - 1);
        };
        link link_UIshell<NEportLevels={2,1},NEx=77.,NEy=8.> => <-.UIshell;
        UIbutton UIbutton_RNC<NEx=462.,NEy=44.> {
           parent => <-.link_UIshell;
           label => "Reset/Normalize/Centre";
           do<NEportLevels={0,3}>;
           x => ((<-.<-.UIshell.width / 2) + 190);
           y = 0;
           width => ((<-.<-.UIshell.width / 2) - 190);
           height = 32;
        };
        UItoggle UItoggle_RNC<NEx=484.,NEy=88.> {
           parent => <-.link_UIshell;
           label => "RNC";
           do<NEportLevels={0,3}>;
           y => ((<-.<-.UIshell.height / 2) - 20);
        };
        UIoption UIoptionDefault<NEx=11.,NEy=275.> {
           label => "Default";
           set = 1;
        };
        UIoptionMenu UIoptionMenu_Datamap<NEx=0.,NEy=242.> {
           parent => <-.link_UIshell;
           cmdList => {
              <-.UIoptionDefault,
              <-.UIoptionGreyScale,
              <-.UIoptionVolumeRender,
              <-.UIoptionHotMetal,
              <-.UIoptionCyanYellowRed,
              <-.UIoptionDefaultStep,
              <-.UIoptionFilterMap
           };
           selectedItem<NEportLevels={0,3}> = 0;
           label => "Datamaps";
           x = 0;
           y => ((<-.<-.UIshell.height / 2) + 30);
           width = 150;
        };
        UIoption UIoptionGreyScale<NEx=22.,NEy=308.> {
           label => "GreyScale";
        };
        UIoption UIoptionVolumeRender<NEx=33.,NEy=341.> {
           label => "VolumeRender";
        };
        UIoption UIoptionHotMetal<NEx=44.,NEy=374.> {
           label => "HotMetal";
        };
        UIoption UIoptionCyanYellowRed<NEx=55.,NEy=407.> {
           label => "CyanYellowRed";
        };
        UIoption UIoptionDefaultStep<NEx=66.,NEy=440.> {
           label => "DefaultStep";
        };
        UIoption UIoptionFilterMap<NEx=99.,NEy=473.> {
           label => "FilterMap";
        };
        UIoptionMenu UIoptionMenuTransf<NEx=198.,NEy=242.> {
           parent => <-.link_UIshell;
           cmdList => {
              <-.UIoptionRotate,
              <-.UIoptionScale,
              <-.UIoptionXYTrans,
              <-.UIoptionZTrans};
           selectedItem<NEportLevels={0,3}> = 1;
           label => "Transforms";
           x = 200;
           y => ((<-.<-.UIshell.height / 2) + 30);
        };
        UIoption UIoptionRotate<NEx=220.,NEy=275.> {
           label => "Rotate";
        };
        UIoption UIoptionScale<NEx=231.,NEy=308.> {
           label => "Scale";
           set = 1;
        };
        UIoption UIoptionXYTrans<NEx=242.,NEy=341.> {
           label => "XY Translate";
        };
        UIoption UIoptionZTrans<NEx=264.,NEy=374.> {
           label => "Z Translate";
        };
        UIoptionMenu UIoptionMenuTransf#1<NEx=385.,NEy=242.> {
           cmdList => {
              <-.UIoptionRotate#1,
              <-.UIoptionScale#1,
              <-.UIoptionXYTrans#1,
              <-.UIoptionZTrans#1};
           selectedItem<NEportLevels={0,3}> = 0;
           label => "Transforms";
           x = 0;
           y = 0;
           parent => <-.link_UIshell;
        };
        UIoption UIoptionRotate#1<NEx=407.,NEy=275.> {
           label => "Rotate";
           set = 1;
        };
        UIoption UIoptionScale#1<NEx=418.,NEy=308.> {
           label => "Scale";
        };
        UIoption UIoptionXYTrans#1<NEx=429.,NEy=341.> {
           label => "XY Translate";
        };
        UIoption UIoptionZTrans#1<NEx=440.,NEy=374.> {
           label => "Z Translate";
        };
        UIoptionMenu UIoptionMenuTransf#2<NEx=605.,NEy=242.> {
           cmdList => {
              <-.UIoptionRotate#2,
              <-.UIoptionScale#2,
              <-.UIoptionXYTrans#2,
              <-.UIoptionZTrans#2};
           selectedItem<NEportLevels={0,3}> = 2;
           label => "Transforms";
           x => (<-.<-.UIshell.width / 2);
           y = 0;
           parent => <-.link_UIshell;
        };
        UIoption UIoptionRotate#2<NEx=627.,NEy=275.> {
           label => "Rotate";
        };
        UIoption UIoptionScale#2<NEx=638.,NEy=308.> {
           label => "Scale";
        };
        UIoption UIoptionXYTrans#2<NEx=649.,NEy=341.> {
           label => "XY Translate";
           set = 1;
        };
        UIoption UIoptionZTrans#2<NEx=660.,NEy=374.> {
           label => "Z Translate";
        };
        UIoptionMenu UIoptionMenuSelect<NEx=429.,NEy=407.> {
           cmdList => {
              <-.UIoptionObject,
              <-.UIoptionCamera,
              <-.UIoptionLights};
           selectedItem<NEportLevels={0,3}> = 2;
           label => "Select to Transform";
           x = 200;
           y = 0;
           parent => <-.link_UIshell;
           width = 350;
          };
        UIoption UIoptionObject<NEx=451.,NEy=440.> {
           label => "Object";
        };
        UIoption UIoptionCamera<NEx=462.,NEy=473.> {
           label => "Camera";
        };
        UIoption UIoptionLights<NEx=473.,NEy=506.> {
           label => "Lights";
           set = 1;
        };
     };
     link link_fld<NEportLevels=1,NEx=231.,NEy=22.> => .Read_Field.field;
     macro reset_xform<NEx=638.,NEy=198.> {
        ilink in_field<export_all=1> => <-.DVorthoslice_unif.out;
        group xfm {
           DefaultXform xform;
        };
        DVset_xform DVset_xform {
           in => <-.in_field;
           in_xfm => <-.xfm;
        };
        olink out_fld<export_all=2> => .DVset_xform.out;
     };
     group params<NEx=440.,NEy=22.,NEwidth=330.,NEheight=165.> {
        float isolevel<NEportLevels=1> => (<-.link_fld.node_data[0].max / 2);
        int axis<NEportLevels=1> => (<-.link_fld.ndim - 1);
        int plane<NEportLevels=1> => ((<-.link_fld.dims[.axis] / 2) - 1);
     };
  };


};










