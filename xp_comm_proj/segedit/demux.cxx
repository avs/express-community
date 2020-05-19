
#include "gen_dmux.h"


int
SegmentationEditor_SegmentationEditorMods_demux::update(OMevent_mask event_mask, int seq_num)
{
   // input (OMXint read notify)
   // input_max (OMXint read notify)
   // output (group_array write)

   /***********************/
   /* Function's Body     */
   /***********************/
//   ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: SegmentationEditor_SegmentationEditorMods_demux::update\n");
   if ((int)input<0) return 1;
   if ((int)input<(int) input_max)
	   output[input].trigger=0;
   // return 1 for success
   return(1);
}
