$push
Root.Applications
   {
   -ModuleStack;
   };
$pop
Root.Applications
   {
// $echo Assistant exit.v load
$push
   GMOD.load_v_script load_begin 
      {
      filename => ("$ASSIST_HOME/assist0.v");
      };
   Root.Templates.APPS.ModuleStack ModuleStack<NEdisplayMode="maximized">;
$pop
//cleanup added
   -load_begin; 
   };
