
#include "gen.h"

// #define DEBUG
int
OrthogonalViewer_OrthogonalViewerMods_copy_on_modify::update(OMevent_mask event_mask, int seq_num)
{
   // in (OMXint read req notify)
   // old (OMXint read write)
   // out (OMXint write)

   /***********************/
   /* Function's Body     */
   /***********************/

#ifdef DEBUG   
    ERRverror("",ERR_NO_HEADER | ERR_PRINT,"I'm in method: OrthogonalViewer_OrthogonalViewerMods_copy_on_modify::update\n");
#endif
	if ( (int) reset) count=0;
	if ((int)in != (int)old)
	{
		out = (int)in;
		diff = (int)in - (int)old;
		change = ((int)diff<0)?-1:1;
		old = (int)in;
		
		val += (int) change* (int)count;
		val = ((int)val<(int)min)?(int)min:(int)val;
		val = ((int)val>(int)max)?(int)max:(int)val;
		count += 1;
		
	};	

   // return 1 for success
   return(1);
}
