#ifndef MSDOS
#include <sys/time.h>
#endif
#include <errno.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include "xp_comm_proj/ximg_ply/gen.hxx"
#include <avs/event.h>

#define FALSE 0
#define TRUE  1

#define DISK_MODE   0
#define MEMORY_MODE 1

#define FF_MODE  0
#define CFF_MODE 1
#define RW_MODE  2
#define CRW_MODE 3

#define ERR_RETURN(A)  { \
 ERRverror("",ERR_NO_HEADER | ERR_INFO, A); \
 return(0); \
}

#define ERR_RETURN1(A,p1) { \
 ERRverror("",ERR_NO_HEADER | ERR_INFO, A, p1); \
 return(0); \
}

// ======================
// Static variables
// ======================

static int last_timer=0;
static int last_real_fps = -1;
static int is_loop_active=FALSE;
static AVSImagePlayer_xImgPlay *ImagePlayer;
static nb_frames=0;

// ======================
// All times are in msec
// ======================

#ifdef sol2
static void ftime(struct timeb *tb)
{
    struct timeval tv;

    gettimeofday(&tv,NULL);
    tb->time=tv.tv_sec;
    tb->millitm=tv.tv_usec/1000;
}
#endif

//=============================================================================
// This function manage enlapsed time :
// ----------------------------------
// INPUTS :
//   If cmd = -1, then init enlapsed time calculation
//   If cmd = 0, then returns enlapsed time calculation
//   If cmd > 0 in order not to take in account an extra time
//
// OUTPUTS :
//
// RETURN VALUE : enlapsed time (when cmd = 0)
//
// ERRORS :
//=============================================================================
static int GetEnlapsedTime(int cmd)
{
#ifdef MSDOS
    static struct _timeb beginning;
#else
    static struct timeb beginning;
#endif

    if (cmd == -1) { /* START */
#ifdef MSDOS
        _ftime(&beginning);
#else
        ftime(&beginning);
#endif
        return(0);
    }
    else if (cmd==0) { /* EXAMINE */
#ifdef MSDOS
        static struct _timeb current;
        _ftime(&current);
#else
        static struct timeb current;
        ftime(&current);
#endif

        int ret=1000*(current.time-beginning.time)+
            current.millitm-beginning.millitm;
        return(ret);
    }
    else { /* ADD DELAY */
        int sec=cmd/1000;
        int msec=cmd-1000*sec;
        
        beginning.time += sec;
        beginning.millitm += msec;
        return(0);
    }
}

//=============================================================================
// This function member reads one image into memory :
// ------------------------------------------------
// INPUTS :
//   image_no         : image number to read
//   data_array_index : array index where to put image
//   module_name      : name of the calling module for express status bar
//
// OUTPUTS :
//
// RETURN VALUE : OK / NOK
//
// ERRORS :
//   Read error
//   Different images size
//=============================================================================
int AVSImagePlayer_xImgPlay::ReadImage(int image_no,
                                          int data_array_index,
                                          char *module_name)
{
    FILE *fp;
    int  old_nx,old_ny,nx,ny,size;
    int  status=0,stop=0;

    char *file_format=NULL;
    char filename[1024];

    ImagePlayerParams.file_format.get_str_val(&file_format);
    sprintf(filename, file_format,
            ImagePlayerParams.first_image +
            image_no*ImagePlayerParams.step);

    if ((char *)Data_Array[data_array_index].labels &&
        !strcmp(Data_Array[data_array_index].labels,filename)) {
        return(1);
    }
    
#ifdef MSDOS
    fp=fopen(filename, "rb");
#else
    fp=fopen(filename, "r");
#endif

    if (fp==NULL) {
        ERR_RETURN1("Cannot open file %s",filename);
    }

    printf("Reading file %s\n",filename);
    fread(&nx, 4, 1, fp);
    fread(&ny, 4, 1, fp);
#ifdef MSDOS
    nx=AVS_SWAP_INT(nx);
    ny=AVS_SWAP_INT(ny);
#endif

    int *dims=(int *)out.dims.ret_array_ptr (OM_GET_ARRAY_RD);
    if (dims) {
        old_nx=dims[0];
        old_ny=dims[1];
        ARRfree(dims);
    }
    else {
        old_nx= -1;
        old_ny= -1;
    }

    if (data_array_index==0 && nx!=old_nx && ny!=old_ny) {
        int *dims=(int *)out.dims.ret_array_ptr (OM_GET_ARRAY_WR);
        
        if (!dims) {
            fclose(fp);
            ERR_RETURN("Can't get dims from out");
        }
        dims[0]=nx;
        dims[1]=ny;
        ARRfree(dims);
    }
    else if (nx!=old_nx && ny!=old_ny) {
        fclose(fp);
        ERR_RETURN("Different images size");
    }
    
    Data_Array[data_array_index].labels=filename;
    Data_Array[data_array_index].nvals=nx*ny;
    char *node_data = (char *)Data_Array[data_array_index].values.
        ret_array_ptr(OM_GET_ARRAY_WR,&size);

    if (!node_data) {
        fclose(fp);
        ERR_RETURN("Can't get Data_Array values");
    }

    for(int i=ny-1;i>=0;i--) {
        int stat=(100*(ny-i+image_no*ny))/(data_array_size*ny);
        
        int n=fread(node_data+i*nx*4, nx*4, 1, fp);
        
        if (n!=1) {
            ARRfree(node_data);
            fclose(fp);
            ERR_RETURN1("Error %d while reading file",errno);
        }

        if (status!=stat && module_name && *module_name) {
            status=stat;
            OMstatus_check (status, module_name, &stop);
            if (stop) {
                ARRfree(node_data);
                fclose(fp);
                return(0);
            }
        }
    }

    fclose(fp);
    ARRfree(node_data);

    return(1);
}

//=============================================================================
// This function member reads all the images if necessary (MEMORY_MODE) :
// --------------------------------------------------------------------
// INPUTS :
//
// OUTPUTS :
//
// RETURN VALUE : OK / NOK
//
// ERRORS :
//=============================================================================
int AVSImagePlayer_xImgPlay::ReadImages(void)
{
    int ret=1;
    char *module_name="ReadImages";

    OMstatus_check (0, module_name, NULL);

    if ((ImagePlayerParams.access_mode==MEMORY_MODE) &&
         (data_array_size!=ImagePlayerParams.nb_images))
    {
        data_array_size=ImagePlayerParams.nb_images;
    }
    else if ((ImagePlayerParams.access_mode==DISK_MODE) &&
             (data_array_size!=1))
    {
        data_array_size=1;
    }

    if (ImagePlayerParams.access_mode==DISK_MODE) {
        ret=ReadImage(ImagePlayerParams.count, 0, module_name);
    }
    else {
        for(int i=0 ; i<data_array_size; i++) {
            ret=ReadImage(i, i, module_name);
            if (ret==0) break;
        }
    }

    OMstatus_check (100, "<idle>", NULL);

    return(ret);
}

//=============================================================================
// This function member loops through the images :
// ---------------------------------------------
// INPUTS :
//
// OUTPUTS :
//
// RETURN VALUE : OK / NOK
//
// ERRORS :
//=============================================================================
void AVSImagePlayer_xImgPlay::LoopFunc(void)
{
    if ((ImagePlayerParams.count>=ImagePlayerParams.nb_images-1) &&
        ImagePlayerParams.run_mode<=1)
    {
        if (ImagePlayerParams.access_mode==DISK_MODE) {
            ReadImage(0, 0, NULL);
        }
        OMpush_ctx(OMroot_obj, OM_STATE_PROG, 0,0);
        ImagePlayerParams.count=0;
        OMpop_ctx(OMroot_obj);
    }
    else if ((ImagePlayerParams.count<=0) &&
              ImagePlayerParams.run_mode>=2)
    {
        if (ImagePlayerParams.access_mode==DISK_MODE) {
            ReadImage(ImagePlayerParams.nb_images-1, 0, NULL);
        }
        OMpush_ctx(OMroot_obj, OM_STATE_PROG, 0,0);
        ImagePlayerParams.count=ImagePlayerParams.nb_images-1;
        OMpop_ctx(OMroot_obj);
    }
    else if (ImagePlayerParams.run_mode<=1) {
        if (ImagePlayerParams.access_mode==DISK_MODE) {
            ReadImage(ImagePlayerParams.count+1, 0, NULL);
        }
        OMpush_ctx(OMroot_obj, OM_STATE_PROG, 0,0);
        ImagePlayerParams.count += 1;
        OMpop_ctx(OMroot_obj);
    }
    else if (ImagePlayerParams.run_mode>=2) {
        if (ImagePlayerParams.access_mode==DISK_MODE) {
            ReadImage(ImagePlayerParams.count+1, 0, NULL);
        }
        OMpush_ctx(OMroot_obj, OM_STATE_PROG, 0,0);
        ImagePlayerParams.count -= 1;
        OMpop_ctx(OMroot_obj);
    }

    nb_frames++;
    float real_fps=float(1000.0*nb_frames)/GetEnlapsedTime(0);
    if (last_real_fps!=int(real_fps*100)) {
        char buffer[128];
        sprintf(buffer,"Real Frames/s : %.1f",real_fps);
        ImagePlayerParams.real_fps=buffer;
        last_real_fps=int(real_fps*100);
    }

    if (ImagePlayerParams.count==ImagePlayerParams.nb_images-1 &&
        ImagePlayerParams.run_mode==CFF_MODE)
    {
        EVdel_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
        last_timer=int(1000*(ImagePlayerParams.nsec_after +
	    1.0/ImagePlayerParams.fps));
        EVadd_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
        GetEnlapsedTime(int(1000*ImagePlayerParams.nsec_after));
    }
    else if (ImagePlayerParams.count==0 &&
             ImagePlayerParams.run_mode==CFF_MODE)
    {
        EVdel_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
        last_timer=int(1000*(ImagePlayerParams.nsec_before +
	    1.0/ImagePlayerParams.fps));
        EVadd_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
        GetEnlapsedTime(int(1000*ImagePlayerParams.nsec_before));
    }
    else if (ImagePlayerParams.count==0 &&
             ImagePlayerParams.run_mode==CRW_MODE)
    {
        EVdel_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
        last_timer=int(1000*(ImagePlayerParams.nsec_after +
	    1.0/ImagePlayerParams.fps));
        EVadd_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
        GetEnlapsedTime(int(1000*ImagePlayerParams.nsec_after));
    }
    else if (ImagePlayerParams.count==ImagePlayerParams.nb_images-1 &&
             ImagePlayerParams.run_mode==CRW_MODE)
    {
        EVdel_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
        last_timer=int(1000*(ImagePlayerParams.nsec_before +
	    1.0/ImagePlayerParams.fps));
        EVadd_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
        GetEnlapsedTime(int(1000*ImagePlayerParams.nsec_before));
    }
    else if ((ImagePlayerParams.count==ImagePlayerParams.nb_images-1 &&
              ImagePlayerParams.run_mode==FF_MODE) ||
             (ImagePlayerParams.count==0 &&
              ImagePlayerParams.run_mode==RW_MODE))
    {
        EVdel_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
        is_loop_active=FALSE;
        last_timer=0;
    }
    else {
        int timer=int(1000/ImagePlayerParams.fps);

        if (last_timer!=timer) {
            EVdel_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
            last_timer=timer;
            EVadd_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
        }
    }
}

void AVSImagePlayer_LoopFuncTmp(char *arg)
{
    ImagePlayer->LoopFunc();
}

//=============================================================================
// This function member starts the loop :
// ------------------------------------
// INPUTS :
//
// OUTPUTS :
//
// RETURN VALUE : OK / NOK
//
// ERRORS :
//=============================================================================
int AVSImagePlayer_xImgPlay::image_player_run(OMevent_mask event_mask, int seq_num)
{
    ImagePlayer=this;

    int ret=ReadImages();
    if (ret==0) return(0);

    if (!is_loop_active) {
        last_timer=int(1000/ImagePlayerParams.fps);
        EVadd_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
        is_loop_active=TRUE;
        GetEnlapsedTime(-1);
        nb_frames=0;
        last_real_fps = -1;
    }

    // return 1 for success
    return(1);
}

//=============================================================================
// This function member stops the loop :
// -----------------------------------
// INPUTS :
//
// OUTPUTS :
//
// RETURN VALUE : OK / NOK
//
// ERRORS :
//=============================================================================
int AVSImagePlayer_xImgPlay::image_player_stop(OMevent_mask event_mask, int seq_num)
{
    if (is_loop_active) {
        EVdel_select(EV_TIMEOUT,0,AVSImagePlayer_LoopFuncTmp,NULL,NULL,last_timer);
        is_loop_active=FALSE;
    }

    // return 1 for success
    return(1);
}
