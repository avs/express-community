/*
 * Obj_Stats
 *
 * Synopsis           : reports object subelement count statistics to the
 *                      specified log file
 */

#include "gen.hxx"

#include <avs/err.h>
#include <avs/util.h>
#include <avs/om.h>
#include <avs/om_type.h>
#include <avs/om_att.h>
#include <avs/port.h>
#include <avs/f_utils.h>

#include <stdio.h>
#include <errno.h>

static int count_objects(OMobj_id obj_id, int mode);
static int obj_stats_num_ui;
static int obj_stats_num_ag;
static int obj_stats_num_meth;


/**********************************
 * START of Module, Update Method *
 **********************************/

int
Instrumentation_InstrumentationMods_obj_stats::update(OMevent_mask /*event_mask */, int /*seq_num */)
{

  int i, depth_local, subcount, gpcount, totcount, gtotcount, istat;
  int num_subs;

  char path_buf[200], *name_buf;
  char filename_buf[AVS_PATH_MAX];
  OMobj_id subObjId, topObjId, tmpObjId;

  int t_obj_stats_num_ui, t_obj_stats_num_ag, t_obj_stats_num_meth;

  //
  // get a pointer to the specified depth backwards
  //
  depth_local = (int)depth + 1;
  if (depth_local < 1) {
    depth_local = 1;
  }
  tmpObjId = filename.obj_id();

  for (i=0; i<depth_local; i++) {
     topObjId = tmpObjId;
     OMget_obj_parent(topObjId,&tmpObjId);
     istat = OMis_null_obj(tmpObjId);   // get handle on OM ptr storage
     if (istat != 0){
       printf("error, cannot get parent object id");
       return 0;
     }
  }

  // Store handle to top object
  topObjId = tmpObjId;

  //
  // get full path to object
  //
  OMret_obj_path( topObjId, path_buf, 199);
 
  // See if we have any subobjects 
  if (OMget_num_subobjs(topObjId, &num_subs) != OM_STAT_SUCCESS) 
        return (1);

  /*
   * check for valid filename
   */
  if ( ((int)strlen(filename)) <= 1 ) {
    printf("error, no filename or invalid filename for log\n");
    return 0;
  }

  // Use AVS/Express function to map enviroment variables
  FILEmap_variables((char *)filename, filename_buf);

  /*
   * Open Log file in append mode
   */
  FILE *fp = fopen(filename_buf, "a"); // a=append mode
  if(!fp)
    return 1;


  // Reporting Section after all values available:
  // initial header to log file,
  // write file in comma separated format for Spreadsheet

  fprintf(fp, "%s: Object Count Report for: %s\n", (char *)message, path_buf);
  fprintf(fp, "Item ,    Name     ,  UI  ,  AG   , Methods   , New , AllObjects\n");

  // loop over all children at this level
  totcount = 0;
  gtotcount = 0;
  t_obj_stats_num_ui = t_obj_stats_num_ag = t_obj_stats_num_meth =0;

  for (i=0; i<num_subs; i++)
  {
        istat = OMget_array_subobj( topObjId, i, &subObjId, OM_OBJ_RD );
        if (istat == 1 && OMis_null_obj(subObjId) == 0 )
        {  // make sure valid

                obj_stats_num_ui = 0;
                obj_stats_num_ag = 0;
                obj_stats_num_meth = 0;

                // get the object instance name
                name_buf = (char *) OMret_obj_name( subObjId );
                // count number of all objects inside
                gpcount  = count_objects( subObjId, 0);
                // count number of new objects defined inside this
                subcount = count_objects( subObjId, 1);

                // write one line to log file
                fprintf(fp, "%d   , %s   , %d ,  %d ,  %d ,  %d ,  %d\n",
                         i, name_buf, obj_stats_num_ui, obj_stats_num_ag,
                         obj_stats_num_meth, subcount, gpcount);

                // collect for grand total
                t_obj_stats_num_ui += obj_stats_num_ui;
                t_obj_stats_num_ag += obj_stats_num_ag;
                t_obj_stats_num_meth += obj_stats_num_meth;
                totcount += subcount;
                gtotcount += gpcount;
        }
  }
  // print closing total
  fprintf(fp, "    , Total-Count ,  %d ,   %d ,   %d ,  %d ,  %d\n",
          t_obj_stats_num_ui, t_obj_stats_num_ag, t_obj_stats_num_meth,
          totcount, gtotcount);

  // close file and exit
  fclose(fp);
  return(1);
}


/*
 * Local Utility to count objects
 *   similar to OMcount_objs(OMobj_id obj_id, int mode)
 */

static int
count_objects(OMobj_id obj_id, int mode)
{
   OMobj_id sub_id, membs;
   int total = 1;
   int i, num_subs;
   char	buf[256], *class_str;

   if (mode) {
      OMobj_id class_id;
      int nclasses;

      /*
       * In this mode, we only count the actual objects created that are
       * stored in this hierarchical tree.  We need to go to any 
       * equal path classes and count these objects as well.
       */
      if (OMget_num_classes(obj_id,&nclasses,OM_CLASS_EQUAL_PATHS_ONLY) != 1) {
	 nclasses = 0;
      }
      /*
       * recursive loop
       */
      for (i = 0; i < nclasses; i++) {
	 if (OMget_array_class(obj_id, i, &class_id, OM_CLASS_EQUAL_PATHS_ONLY)
	     != 1) continue;
	 total += count_objects(class_id, mode);
      }

      if (OMobj_is_type_of(obj_id, OM_type_group)) {
	 /*
	  * Get the members array for this object.  If we don't have one, 
	  * we don't care about this object.  make sure that it is not a link
	  * just redirecting our find_subobj call.
	  */
	 membs = OMfind_subobj(obj_id, OM_name_members, OM_OBJ_RW);
	 if (OMis_null_obj(membs)) return(total);
      }
   }
   else membs = obj_id;


   /*
    * Find item base type
    *  Look for methods, UI and AG type objects for special counts
    */
   if ((OMobj_is_type_of(obj_id,OM_type_method)  == 1) ||
       (OMobj_is_type_of(obj_id,OM_type_omethod) == 1) ||
       (OMobj_is_type_of(obj_id,OM_type_cmethod) == 1)) obj_stats_num_meth++;


    class_str = OMget_user_class_name(obj_id, buf, sizeof(buf));
    if (class_str != NULL) 
    {
        //printf("obj_stats: %s\n",class_str);
        if ( strncmp(class_str,"UI",2)==0) obj_stats_num_ui++;
        if ( strncmp(class_str,"AG",2)==0) obj_stats_num_ag++;
    }


   /*
    * Only count group subobjects since otherwise we'll get links...
    */
   if (OMobj_is_type_of(obj_id,OM_type_group) != 1)
      return(total);

   if (OMget_num_subobjs(membs, &num_subs) != 1)
      num_subs = 0;

   for (i = 0; i <= num_subs; i++) {
      if (OMget_array_subobj(membs, i, &sub_id, OM_OBJ_RD) != 1)
	 continue;

      total += count_objects(sub_id, mode);
   }
   return(total);
}

/* end */

