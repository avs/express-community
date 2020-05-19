/*
 * usage
 *
 * Synopsis           : reports CPU time, elapsed time and memory usage to the
 *                      specified file
 * Notes:               Slightly different behavior on UNIX and Windows, as 
 *                      differerent services are available
 *                      Windows system calls by Tom Parker
 */

#include "gen.hxx"

#include <avs/om.h>
#include <avs/port.h>

#ifdef MSDOS
  #include <sys/types.h>
  #include <sys/timeb.h>
  #include <sys/stat.h>
  #include <time.h>
#else
  #include <sys/time.h>
  #include <sys/resource.h>
  #include <unistd.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <avs/timer.h>
#include <avs/f_utils.h>

typedef struct mytimeval 
{
   unsigned int tval;
   unsigned int paging;
}  _Mytimeval;

/******************************************
 * START of Module, Initialization Method *
 ******************************************/

int
Instrumentation_InstrumentationMods_usage::init(OMevent_mask /*event_mask */, 
				   int /*seq_num */)
{
  // global vars accessed through C++ omx
  // timePtr (OMXptr read/write)

  int istat;  // status returns

/*
 * Get Initial Time on Entry
 */

  struct mytimeval *tptr = NULL;             // struct ptr holder
  istat = OMis_null_obj(timePtr.obj_id());   // get handle on OM ptr storage
  if (istat != 0){
    printf("error, initialize cannot get timePtr object id");
    return 0;
  }

  istat = OMget_ptr_val(timePtr.obj_id(), (void **)&tptr, 0);  // get current ptr 
  if (istat <= 0){
    printf("error, initialize cannot get timePtr pointer value");
    return 0;
  }

  if (tptr == NULL)  // setup storage area, first time only
    tptr = (struct mytimeval *)calloc(1, sizeof(struct mytimeval));

  tptr->tval = 0; // reset each time

  // TIMEstart_timer( &(tptr->tval) );  // AVS utility function, undocumented

#ifdef MSDOS
    struct _timeb tp;
    /* Get the current time */
    _ftime(&tp);
    tptr->tval = (unsigned int)(tp.time * 1000000 + tp.millitm * 1000);
#else
    struct timeval tp;
    /* Get the current time */
    gettimeofday(&tp, (struct timezone *) 0);
    tptr->tval = (unsigned int) (tp.tv_sec * 1000000 + tp.tv_usec);
#endif

  /*
   * save pointer to saved values until second method is called
   */
  istat = OMset_ptr_val(timePtr.obj_id(), (void *)tptr, 0);
  if (istat <= 0){
    printf("error, initialize cannot set timePtr pointer value on exit");
    return 0;
  }


#ifdef MSDOS
  // Windows system resource call
  //
	//  need to add some system call to get memory resource values
	//
	//  typedef struct _MEMORYSTATUS { // mst 
	//  	DWORD dwLength;        // sizeof(MEMORYSTATUS) 
	//  	DWORD dwMemoryLoad;    // percent of memory in use 
	//  	DWORD dwTotalPhys;     // bytes of physical memory 
	//  	DWORD dwAvailPhys;     // free physical memory bytes 
	//  	DWORD dwTotalPageFile; // bytes of paging file 
	//  	DWORD dwAvailPageFile; // free bytes of paging file 
	//  	DWORD dwTotalVirtual;  // user bytes of address space 
	//  	DWORD dwAvailVirtual;  // free user bytes 
	//  } MEMORYSTATUS, *LPMEMORYSTATUS; 
	
	MEMORYSTATUS MemStat; 
	ZeroMemory(&MemStat, sizeof(MEMORYSTATUS)); 
	MemStat.dwLength = sizeof(MEMORYSTATUS); 
	GlobalMemoryStatus(&MemStat); 

        /*****
	printf("%d, %d, %d, %d, %d, %d, %d\n",
		MemStat.dwMemoryLoad,
		MemStat.dwTotalPhys,
		MemStat.dwAvailPhys,
		MemStat.dwTotalPageFile,
		MemStat.dwAvailPageFile,
		MemStat.dwTotalVirtual,
		MemStat.dwAvailVirtual);
        *********/
	tptr->paging = (MemStat.dwTotalVirtual - MemStat.dwAvailVirtual);  // set to current paging value


#else
  // UNIX system resource call
  struct rusage *rptr = (struct rusage *)calloc(1, sizeof(struct rusage));
  istat = getrusage(RUSAGE_SELF, rptr);
  if(istat == -1){
    tptr->paging = 0;
  }
  else
    tptr->paging = rptr->ru_maxrss;
  free(rptr);
#endif

  // return 1 for success
  return(1);

}

/**************************************************
 * START of Module, Update Method, LOG to File    *
 **************************************************/

int
Instrumentation_InstrumentationMods_usage::output(OMevent_mask /*event_mask */, 
				     int /* seq_num */)
{
  // timePtr (OMXptr read/write)

  int istat;
  double elapsed;
  unsigned int new_time, delta_paging;

  char filename_buf[AVS_PATH_MAX];

#ifndef MSDOS
  struct rusage *srptr;
#endif

/*
 * Get Initial Time on Entry
 */

  struct mytimeval *stptr;

  /*
   * check if valid filename string before trying to open
   */
  if ( ((int)strlen(filename)) <= 1 ) {
    printf("error, no filename or invalid filename for log");
    return 0;
  }

  // Use AVS/Express function to map enviroment variables
  FILEmap_variables((char *)filename, filename_buf);

  /*
   * Open Log file in append mode
   */
  FILE *fp = fopen((char *)filename_buf, "a"); // a=append mode
  if(!fp)
    return 1;

  //  get pointer to initial time
  OMget_ptr_val(timePtr.obj_id(), (void **)&stptr, 0);
  if (!stptr) {
    printf("error, no starting values available");
    fclose(fp);
    return 0;
  }

  /*
   * Get the Current time, compare with initial time
   */
#ifdef MSDOS
    struct _timeb tp;
    /* Get the new current time */
    _ftime(&tp);
    new_time = (unsigned int)(tp.time * 1000000 + tp.millitm * 1000);
#else
    struct timeval tp;
    /* Get the new current time */
    gettimeofday(&tp, (struct timezone *) 0);
    new_time = tp.tv_sec * 1000000 + tp.tv_usec;
#endif
    /* Get time difference between old and new in seconds */
    elapsed = (float)(new_time - stptr->tval) / 1000000.0;

  // clean up storage, or not!
  // leave it allocated for next time, may look like a small leak....
  // ideally add a destroy method to cleanup
  // free(stptr);

  /*
   * Get Memory Paging - swap page count resources
   */
#ifdef MSDOS

	// Windows system resource call
	//
	//  typedef struct _MEMORYSTATUS { // mst 
	//  	DWORD dwLength;        // sizeof(MEMORYSTATUS) 
	//  	DWORD dwMemoryLoad;    // percent of memory in use 
	//  	DWORD dwTotalPhys;     // bytes of physical memory 
	//  	DWORD dwAvailPhys;     // free physical memory bytes 
	//  	DWORD dwTotalPageFile; // bytes of paging file 
	//  	DWORD dwAvailPageFile; // free bytes of paging file 
	//  	DWORD dwTotalVirtual;  // user bytes of address space 
	//  	DWORD dwAvailVirtual;  // free user bytes 
	//  } MEMORYSTATUS, *LPMEMORYSTATUS; 
	
	MEMORYSTATUS MemStat; 
	ZeroMemory(&MemStat, sizeof(MEMORYSTATUS)); 
	MemStat.dwLength = sizeof(MEMORYSTATUS); 
	GlobalMemoryStatus(&MemStat); 

	delta_paging = (MemStat.dwTotalVirtual - MemStat.dwAvailVirtual)
                          - (stptr->paging);  // difference paging value since timer started

        fprintf(fp, 
	   "%s ELAPSED: %f seconds, virtual image changed by %d\n",
	   (char *)message, elapsed, delta_paging);
	fprintf(fp,
        "        MemLoad=%d, TotalPhys=%d, AvailPhys=%d, TotalPageFile=%d\n",
		MemStat.dwMemoryLoad,
		MemStat.dwTotalPhys,
		MemStat.dwAvailPhys,
		MemStat.dwTotalPageFile);

	fprintf(fp,
        "        AvailPageFile=%d, TotalVirtual=%d, AvailVirtual %d\n",
		MemStat.dwAvailPageFile,
		MemStat.dwTotalVirtual,
		MemStat.dwAvailVirtual);


#else
  // UNIX System Resource Calls
  struct rusage *rptr = (struct rusage *)calloc(1, sizeof(struct rusage));
  istat = getrusage(RUSAGE_SELF, rptr);
  if(istat == -1){
    delta_paging = 0;
  } else if(istat == 0){
    // subtract old from new to get changes
    delta_paging = rptr->ru_maxrss - stptr->paging;
  };
  free(rptr);

  // Reporting Section after all values available:

  fprintf(fp, 
	   "%s ELAPSED: %f seconds, paging changed by %d\n",
	   (char *)message, elapsed, delta_paging);

#endif

  fclose(fp);
  return(1);
}
/* end */

