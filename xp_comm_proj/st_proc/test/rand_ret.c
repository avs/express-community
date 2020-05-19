#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
   float norm_val;
   int   result, i;


   /* Seed the random-number generator with current time so that
    * the numbers will be different every time we run.
    */
   srand( (unsigned)time( NULL ) );

   /* Run the random number series for a while so we're sure that
    * they actually are random.  This might just be a Microsoft problem though...
    */
   for (i=0; i<20; i++)
      rand();

   norm_val = (float)rand()/(float)RAND_MAX;
   result = (int)(norm_val * 200.0) + 1;

   printf("return value = %d\n", result);

   return result;
}

