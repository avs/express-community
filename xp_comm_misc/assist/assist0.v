Applications
   {
Root.Templates.APPS.ModuleStack ModuleStack<NEdisplayMode="maximized">;
   macro assist0
      {
      GMOD.err_handler err_handler<NEx=330.,NEy=55.>;
      macro mainbutton<NEx=0.,NEy=11.> 
         {
         string markets<NEx=400.,NEy=11.>[] = 
	 $include $ASSIST_HOME/markets.v
         int n_markets => array_size(markets);
         string app_home<NEportLevels={0,2},NEx=11.,NEy=473.>;
         UIapp ui<NEx=5.,NEy=5.> 
            {
            clientWidth = 322;
            clientHeight = 492;
            x = 23;
            y = 52;
            width = 322;
            height = 492;
            title="Assistant v. 1.4  (1024 x 768)";
            };
         macro MARKETS<NEx=11.,NEy=363.>[n_markets]
            {
            float ht => ((<-.ui.clientHeight)/((<-.n_markets+1)/2));
            float wd => ((<-.ui.clientWidth)/2);
            int ind => index_of(MARKETS);
            string market=>"$ASSIST_HOME/"+<-.markets[ind];
            int i1 => ind/2;
            int i2 => ind%2;
            UIframe uif<NEx=50.,NEy=60.> 
               {
               parent => <-.<-.ui;
               y => ht*i1;
               x => wd*i2;
               width => wd;
               height => ht;
               };
            UIbutton uib<NEx=180.,NEy=100.>  
               {
               parent => <-.uif;
               height => <-.uif.clientHeight;
               width => <-.uif.clientWidth;
               label => <-.<-.markets[<-.ind];
               labelPixmap  
                  {
                  filename=>market+"/pixmaps/mainbut.x";
                  fileType = "avs_image";
                  };
               };
            GMOD.parse_v dir_set_b<NEx=210.,NEy=140.>
               {
               v_commands => "app_home => \""+market+"\"";
               trigger => <-.uib.do;
               on_inst = 0;
               relative => Applications.assist0.mainbutton;
               };
            GMOD.load_v_script load_b<NEx=240.,NEy=180.>
               {
               filename => ("$ASSIST_HOME/assist1.v");
               trigger => <-.dir_set_b.trigger;
               active => <-.dir_set_b.active;
               on_inst = 0;
               }; 
            };
        int dummy[n_markets]=>MARKETS.ind;
        };
    };
};
