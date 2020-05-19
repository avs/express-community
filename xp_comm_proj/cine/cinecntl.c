

#ifdef MSDOS
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

#include "gen.h"
#include "avs/event.h"

/* Define USE_POLL_EVENT to work around a bug in the timeout mechanism which causes
 *   it to not compensate for processing done in the timeout function. We use the poll
 *   mechanism to force the processing to occur after we return from this function.
 */
#define USE_POLL_EVENT 1

typedef struct {
  int start;
  int end;
  int mode;
  int value;
  double rate;
  double period;
  int increment;
  int is_active;
  double last_time;
  double next_time;
  int shell_visible;
  OMobj_id control_id;
  int num_samples;
  int update_stats;
  double true_rate;
  double running_average;
} CineInfo;

static double GetTime()
{
  double now;

#ifdef MSDOS
  struct _timeb tp;

  _ftime(&tp);
  now = (double)tp.time + (double)(tp.millitm / 1000);
#else
  struct timeval t;

  gettimeofday( &t, NULL );
  now = (double)t.tv_sec + ((double)t.tv_usec / 1000000.0);
#endif

  return( now );
}

static void PollFunc(char *poll_arg)
{
  CineInfo *cine = (CineInfo *)poll_arg;

  /* We only want to call this once
   */
  EVdel_select(EV_POLL0, 0, PollFunc, NULL, (void *)cine, 0);

  /* Update values
   */
  OMbegin_op();
  OMset_name_int_val( cine->control_id, OMstr_to_name("value"), cine->value );

  if( cine->update_stats )
    {
      OMset_name_real_val(cine->control_id, OMstr_to_name("true_rate"), cine->true_rate );
      cine->update_stats = 0;
    }

  OMend_op();
}

static void TimerFunc(char *ts_arg)
{
  CineInfo *cine = (CineInfo *)ts_arg;
  int frames_to_skip;
  double elapsed;
  double desired;
  int frame_inc;
  double error;
  double now;

  /* Get current time
   */
  now = GetTime();

  /* Calculate error from where we expected to be, and frame to display
   *  based on that error.
   */
  frames_to_skip = 1;
  desired = 1.0 / cine->rate;
  elapsed = now - cine->last_time;
  error = now - cine->next_time;
  /* printf( "error = %f\n", error ); */

  if( error > desired )
    frames_to_skip += (int)(error / desired);

  /* printf( "CineControl: Time error = %f, skip = %d\n", error, frames_to_skip ); */

  cine->last_time = now;
  cine->next_time += (desired * frames_to_skip);
  frame_inc = (cine->increment * frames_to_skip);

  /* Increment frame number
   */
  cine->value += frame_inc;

  /* Test limits
   */
  switch( cine->mode )
    {
    case 1:
      if( cine->value < cine->start )
        cine->value = cine->end;

      break;

    case 2:
      if( cine->value >= cine->end )
        {
          cine->value = cine->end;
          cine->increment *= -1;
        }

      if( cine->value <= cine->start )
        {
          cine->value = cine->start;
          cine->increment *= -1;
        }

      break;

    default:
      if( cine->mode != 0 )
        fprintf( stderr, "CineControl: Invalid mode, using Forward\n" );

      if( cine->value > cine->end )
        cine->value = cine->start;
    }

  /* Update output values
   */
  if( cine->shell_visible )
    {
      cine->running_average += elapsed;
      if( --(cine->num_samples) <= 0 )
        {
          cine->update_stats = 1;
          cine->true_rate = 1.0 / (cine->running_average / (int)cine->rate);

          cine->running_average = 0.0;
          cine->num_samples = (int)cine->rate;
        }
    }

#ifdef USE_POLL_EVENT
  EVadd_select(EV_POLL0, 0, PollFunc, NULL, (void *)cine, 0);
#else
  OMbegin_op();
  OMset_name_int_val( cine->control_id, OMstr_to_name("value"), cine->value );

  if( cine->update_stats )
    {
      OMset_name_real_val(cine->control_id, OMstr_to_name("true_rate"), cine->true_rate );
      cine->update_stats = 0;
    }

  OMend_op();
#endif
}

int CreateControl(OMobj_id Cine_Control_id, OMevent_mask event_mask, int seq_num)
{
  CineInfo *cine;

  cine = (CineInfo *)malloc( sizeof(CineInfo) );
  if( cine )
    {
      memset( cine, 0, sizeof(CineInfo) );
      cine->control_id = Cine_Control_id;
      OMset_name_ptr_val( Cine_Control_id, OMstr_to_name("hidden"), (void *)cine, 0 );
      return( 1 );
    }
  else
    {
      fprintf( stderr, "CreateControl: Unable to allocate storage for cine info\n" );
      OMset_name_ptr_val( Cine_Control_id, OMstr_to_name("hidden"), NULL, 0 );
      return( 0 );
    }
}

int UpdateControl(OMobj_id Cine_Control_id, OMevent_mask event_mask, int seq_num)
{
  CineInfo *cine;
  double desired;
  double now;
  int run;

  cine = NULL;
  OMget_name_ptr_val( Cine_Control_id, OMstr_to_name("hidden"), (void *)&cine, 0 );
  if( cine == NULL )
    {
      fprintf( stderr, "UpdateControl: ptr value = 0\n" );
      return( 0 );
    }

   /* Get run's value */
  if (OMget_name_int_val(Cine_Control_id, OMstr_to_name("run"), &run) != 1)
    run = 0;

  /* Get mode's value */
  if (OMget_name_int_val(Cine_Control_id, OMstr_to_name("mode"), &cine->mode) != 1)
    cine->mode = 0;

  /* Get rate's value */
  if (OMget_name_real_val(Cine_Control_id, OMstr_to_name("rate"), &cine->rate) != 1)
    cine->rate = 1.0;

  /* Get start's value */
  if (OMget_name_int_val(Cine_Control_id, OMstr_to_name("start"), &cine->start) != 1)
    cine->start = 0;

  /* Get end's value */
  if (OMget_name_int_val(Cine_Control_id, OMstr_to_name("end"), &cine->end) != 1)
    cine->end = 0;

  /* Get shell_visible's value */
  if (OMget_name_int_val(Cine_Control_id, OMstr_to_name("shell_visible"), &cine->shell_visible) != 1)
    cine->shell_visible = 0;

  /***********************/
  /* Function's Body     */
  /***********************/

  /* Disable previous timer before you do anything
   */
  if( cine->is_active )
    {
      cine->is_active = 0;
      EVdel_select( EV_TIMEOUT0, 0, TimerFunc, NULL, (void *)cine, (int)cine->period );
    }

  /* Now setup new timer if required
   */
  if( run )
    {
      cine->is_active = 1;
      cine->period = 1000.0 / cine->rate;
      EVadd_select( EV_TIMEOUT0, 0, TimerFunc, NULL, (void *)cine, (int)cine->period );

      /* Initialize value's value
       */
      if( cine->mode == 1 )
        {
          cine->increment = -1;
          cine->value = cine->end;
        }
      else
        {
          cine->increment = 1;
          cine->value = cine->start;
        }

      OMset_name_int_val(Cine_Control_id, OMstr_to_name("value"), cine->value);

      /* Initialize true_rate's value  (if required)
       */
      if( cine->shell_visible )
        {
          cine->true_rate = 0.0;
          cine->running_average = 0.0;
          cine->num_samples = (int)cine->rate;
          OMset_name_real_val(Cine_Control_id, OMstr_to_name("true_rate"), cine->true_rate);
        }

      /* Calculate the next frame time
       */
      now = GetTime();
      cine->last_time = now;
      desired = 1.0 / cine->rate;
      cine->next_time = now + desired;
    }

  return(1);
}

int DeleteControl(OMobj_id Cine_Control_id, OMevent_mask event_mask, int seq_num)
{
  CineInfo *cine;

  cine = NULL;
  OMget_name_ptr_val( Cine_Control_id, OMstr_to_name("hidden"), (void *)&cine, 0 );
  if( cine )
    {
      if( cine->is_active )
        EVdel_select( EV_TIMEOUT0, 0, TimerFunc, NULL, (void *)cine, (int)cine->period );

      free( cine );
    }

  return(1);
}



