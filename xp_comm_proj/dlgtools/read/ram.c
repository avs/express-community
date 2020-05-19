
/*
 * Ram.C - Memory management utilities
 */

#include  <stdlib.h>
#include  <stdio.h>
#include  <errno.h>
#include <sys/types.h>


char error_message[262144] = {"\0"};

#define ALF(_s0) sprintf( _s0 + strlen(_s0), "\n\nFILE: %s  LINE: %i", __FILE__, __LINE__ )

#define P_MESS(a,b) printf( "\nERROR MESSAGE\nRoutine: %s\n%s\n", a, b );
/*********************************************************************

  start of stuff to be included in all programs that use this package

*********************************************************************/


typedef struct Ram_Block_Data__ {

  char *head;
  char *tail;

  size_t size;
  size_t left;
  size_t used;

  struct Ram_Block_Data__ *next;

} Ram_Block_Data;


int              ram_init( size_t block_size );
char            *ram_more(int ram_block_number, size_t more_bytes );
int              ram_free(int ram_block_number );
int              ram_free_all( void );
Ram_Block_Data  *ram_querey( int ram_block_number );
void             ram_toggle_debug( int flag );

/*********************************************************************

  end of stuff to be included in all programs that use this package

*********************************************************************/

#define MAX_RAM_BLOCKS 128
#define DEFAULT_BLOCK_SIZE 4096

static int not_first_call_to_init = 0;
static int ram_debug_enabled = 0;

static struct Ram_Block_Data__    *ram_block_entries[ MAX_RAM_BLOCKS ];
static int                         ram_block_sizes[ MAX_RAM_BLOCKS ];

extern void ram_dump( char *routine );


/*********************************************************************

  toggle the debugger

*********************************************************************/

void ram_toggle_debug( int flag )

{
  if( flag > 1 ) flag = 1;
  if( flag < 0 ) flag = 0;
  
  ram_debug_enabled = flag;

  return;

}


/*********************************************************************

  delete a current block and zero the memory

  no input error checking

*********************************************************************/
void ram_free_block( Ram_Block_Data *this_block )

{

  /* zero the memory in the block*/
  memset( this_block->head, 0,
         sizeof( char )*this_block->size );
  
  this_block->tail = NULL;
  this_block->size = 0;
  this_block->used = 0;
  this_block->left = 0;
  
  this_block->next = NULL;


  /* free the memmory in the block */
  free( this_block->head );
  this_block->head = NULL;


  /* free the memory for the block structure */
  free( this_block ); this_block = NULL;

  return;

}





/*********************************************************************

  free an entire link

*********************************************************************/

/* the free function */
int ram_free( block_number )

     int block_number;

{

  

  Ram_Block_Data *this_block, *that_block;


  /* make sure block number in bounds */
  if(block_number < 1 ||
     block_number > MAX_RAM_BLOCKS-1)
    {

      error_message[0] = '\0';

      sprintf( error_message+strlen(error_message),
              "Block %i\nblock_number out of range\n",
               block_number);

      sprintf( error_message+strlen(error_message),
              "Allowable min = 1  Allowable max = %i ",
              MAX_RAM_BLOCKS-1);

      ALF(error_message);

      P_MESS( "ram_free", error_message );
      

      return(-1);

    }


  if( ram_block_entries[ block_number ] == NULL )
    {
      error_message[0] = '\0';

      sprintf( error_message+strlen(error_message),
              "Block %i\ncan't free block - it has a NULL head",
              block_number);

      ALF(error_message);

      P_MESS( "ram_free", error_message  );
      

      return(-1);

    }


  /* descend and free */
  this_block = ram_block_entries[ block_number ];
  while( this_block != NULL )
    {
      that_block = this_block->next;

      ram_free_block( this_block );

      this_block = that_block;

    }

  /* don't forget the head!! */
  ram_block_entries[ block_number ] = NULL;

  /* finally, the list */
  ram_block_sizes[ block_number ] = 0;

  
  /* debug printing */
  if( ram_debug_enabled == 1 ) ram_dump( "ram_free" );

  /* return the number of bytes free'd */
  return(0);


}





/*********************************************************************

  free all the links

  no input error checking

*********************************************************************/
int ram_free_all()

{

  int ii;

  int ireturn_val=0;

  for( ii=1; ii<MAX_RAM_BLOCKS; ii++ )
    {
      if( ram_block_entries[ ii ] != NULL)
        {

          /* free the data in this link */
          ireturn_val += ram_free( ii );

        }
    }

  /* debug printing */
  if( ram_debug_enabled == 1 ) ram_dump( "ram_free_all" );

  return( ireturn_val );


}






/*********************************************************************

  initialize all of the head pointers to NULL

  no input error checking

*********************************************************************/

void ram_init_first_pass( void )
{
  int ii;

  memset( ram_block_entries, 0,
         sizeof( Ram_Block_Data *)*MAX_RAM_BLOCKS );

  memset( ram_block_sizes, 0,
         sizeof(int)*MAX_RAM_BLOCKS );


  not_first_call_to_init = 1;

  return;

}






/*********************************************************************

  create a new empty block

  no input error checking

*********************************************************************/

Ram_Block_Data *ram_add_block( int block_number, size_t block_size )
{

  

  Ram_Block_Data *this_block;


  /* get the structure for the block */
  this_block = (Ram_Block_Data *) malloc( sizeof(Ram_Block_Data) );

  if( this_block == NULL )
    {
      error_message[0] = '\0';

      sprintf( error_message+strlen(error_message),
              "Block %i\nmalloc failure: can't malloc %i bytes for the block structure", block_number, sizeof(Ram_Block_Data) );

      ALF(error_message);

      P_MESS( "ram_add_block", error_message  );
      

      return( NULL );

    }


  /* get the memory for the block */
  this_block->head = (char *) malloc( block_size );

  if( this_block->head == NULL )
    {
      error_message[0] = '\0';

      sprintf( error_message+strlen(error_message),
              "Block %i\nmalloc failure: can't malloc %i bytes for the block data", block_number, block_size );

      ALF(error_message);

      P_MESS( "ram_block_add", error_message  );
      

      return( NULL );

    }


  /* initialize the remaining parts of the structure */
  this_block->tail = this_block->head;

  this_block->size = block_size;
  this_block->used = 0;
  this_block->left = block_size;

  this_block->next = NULL;

  /* return the new block number */
  return( this_block );
}







/*********************************************************************

  initialize the head structure of a link with a new empty block

*********************************************************************/

/* the init function */
int ram_init( size_t block_size )

{

  

  int block_number;

  Ram_Block_Data *this_block;


  /* Is this the first call to init? */
  if( not_first_call_to_init == 0 )
    {
      ram_init_first_pass();
    }


  /* make sure block size is reasonable */
  if( block_size < 0 )
    {
      error_message[0] = '\0';

      sprintf( error_message+strlen(error_message),
              "Invalid block size = %i",
              block_size );

      ALF(error_message);

      P_MESS( "ram_init", error_message  );
      

      return(-1);

    }

  /* allow for a default block size */
  if( block_size == 0 ) block_size = DEFAULT_BLOCK_SIZE;


  /* search for the first free block

     NB - block 0 is reserved for future use */
  for(block_number=1;
      block_number < MAX_RAM_BLOCKS &&
      ram_block_entries[ block_number ] != NULL;
      block_number++ ){}

  /* signal error if all blocks used and return 0*/
  if( block_number == MAX_RAM_BLOCKS )
    {

      error_message[0] = '\0';

      sprintf( error_message+strlen(error_message),
              "\nAll %i blocks currently in use",
              MAX_RAM_BLOCKS );

      ALF(error_message);

      P_MESS( "ram_init", error_message  );
      

      return(-1);

    }


  /* add a new block and put it at the head */
  ram_block_entries[ block_number ]
    = ram_add_block( block_number, block_size );

  if( ram_block_entries[ block_number ] == NULL )
    {
      return( (int) NULL );
    }

  /* save the block size for future use */
  ram_block_sizes[ block_number ] = block_size;

  /* debug printing */
  if( ram_debug_enabled == 1 ) ram_dump( "ram_init" );

  /* return the new block number */
  return( block_number );


}







/*********************************************************************

  get more memory from a list

*********************************************************************/

char *ram_more(int block_number, size_t more_bytes )

{

  

  div_t mydiv;

  size_t new_bytes;

  int factor;

  char *return_val;

  Ram_Block_Data *this_block;

  /* make sure block number in bounds */
  if(block_number < 1 ||
     block_number > MAX_RAM_BLOCKS-1)
    {

      error_message[0] = '\0';

      sprintf( error_message+strlen(error_message),
              "Block %i\nblock_number out of range\n",
               block_number);

      sprintf( error_message+strlen(error_message),
              "Allowable minimum = 1  Allowable maximum = %i ",
               MAX_RAM_BLOCKS-1);

      ALF(error_message);

      P_MESS( "ram_more", error_message  );
      

      return(NULL);

    }


  /* make sure the size requested makes sense */
  if( more_bytes < 1 )
    {

      error_message[0] = '\0';

      sprintf( error_message+strlen(error_message),
              "Block = %i\nrequested # bytes = %i out of range",
               block_number, more_bytes);

      ALF(error_message);

      P_MESS( "ram_more", error_message  );
      

      return(NULL);

    }


  /* make sure this block has been init'ed properly */
  if( ram_block_entries[block_number] == NULL  )
    {

      error_message[0] = '\0';

      sprintf( error_message+strlen(error_message),
              "\nrequested block = %i has NULL head (not init'ed)",
               block_number);

      ALF(error_message);

      P_MESS( "ram_more", error_message  );
      

      return(NULL);

    }


  /* no more memory needed */
  this_block = ram_block_entries[ block_number ];
  if( more_bytes <= this_block->left )
    {

      this_block->used += more_bytes;
      this_block->left -= more_bytes;

      return_val = this_block->tail;
      this_block->tail += more_bytes;

      /* is the block full */
      if( this_block->left == 0 )
        {
          this_block->tail = NULL;

        }

      /* debug printing */
      if( ram_debug_enabled == 1 ) ram_dump( "ram_more" );
      
      return( return_val );

    }


  /* first truncate the existing block */
  this_block = ram_block_entries[ block_number ];

  /* watch for zero length blocks - remove if possible */
  /* don't remove the head if there is only one link */

  if( this_block->used == 0 ) this_block->used = 1;
  
  this_block->head
    = (char *) realloc( this_block->head, this_block->used );

  if( this_block->head == NULL )
    {
      
      error_message[0] = '\0';
      
      sprintf( error_message+strlen(error_message),
	      "Block %i\nrealloc failure\nexisting bytes = %i  requested bytes = %i",
	      block_number, this_block->size, this_block->used );
      
      sprintf( error_message+strlen(error_message),
	      "\n\nERR: %s", strerror(errno) );
      
      ALF(error_message);
      
      P_MESS( "ram_more" , error_message );
      
      
      return(NULL);
      
    }

  /* adjust the information in this block */
  this_block = ram_block_entries[ block_number ];
  
  this_block->size = this_block->used;
  this_block->left = 0;

  this_block->tail = NULL;


  /* now create a new block */
  /* allow for exact block fills */
  mydiv = div( more_bytes, ram_block_sizes[ block_number ] );
  if( mydiv.rem == 0 )
    {
      new_bytes =  mydiv.quot    * ram_block_sizes[ block_number ];
    }
  else
    {
      new_bytes = (mydiv.quot+1) * ram_block_sizes[ block_number ];
    
    }


  /* create the new block */
  this_block = ram_add_block( block_number, new_bytes );

  if( this_block == NULL )
    {
      /* debug printing */
      if( ram_debug_enabled == 1 ) ram_dump( "ram_more" );
      
      return( NULL );
    }

  /* put it at the head of the list and return*/
  this_block->next = ram_block_entries[ block_number ];
  ram_block_entries[ block_number ] = this_block;

  /* adjust for memory being returned */
  this_block = ram_block_entries[ block_number ];

  this_block->used = more_bytes;
  this_block->left = this_block->size - more_bytes;

  return_val = this_block->tail;
  this_block->tail = this_block->head + more_bytes;

  if( this_block->left == 0 )
    {
      this_block->tail = NULL;
      
    }


  return_val = this_block->head;

  /* debug printing */
  if( ram_debug_enabled == 1 ) ram_dump( "ram_more" );


  return( return_val );


}








/*********************************************************************

  querey the blocks

*********************************************************************/

/* the querey function */
Ram_Block_Data *ram_querey( block_number )

     int block_number;

{

  

  int ii;

  Ram_Block_Data *this_block;

  /* make sure block number in bounds */
  if(block_number < 0 ||
     block_number > MAX_RAM_BLOCKS-1)
    {

      error_message[0] = '\0';

      sprintf( error_message+strlen(error_message),
              "Block %i\nblock_number out of range",
               block_number);

      sprintf( error_message+strlen(error_message),
              "Allowable min = 0  Allowable max = %i ",
               MAX_RAM_BLOCKS);

      ALF(error_message);

      P_MESS( "ram_querey", error_message  );
      

      return(NULL);

    }



  /* do a window if all requested */
  if( block_number == 0 )
    {

      error_message[0] = '\0';

      for(ii=0; ii< MAX_RAM_BLOCKS; ii++ )
        {
	  this_block = ram_block_entries[ ii ];
	  if( this_block != NULL )
	    {
	      
	      sprintf( error_message+strlen(error_message),
		      "\nblock   head      tail      size    used    left    next      block P ");
	      sprintf( error_message+strlen(error_message),
		      "\n------  --------  --------  ------  ------  ------  --------  --------");

	      while( this_block != NULL )
		{
		  sprintf( error_message+strlen(error_message),
			  "\n%6i  %8p  %8p  %6i  %6i  %6i  %8p  %8p",
			  ii,
			  this_block->head,
			  this_block->tail,
			  this_block->size,
			  this_block->used,
			  this_block->left,
			  this_block->next,
			  this_block );

		  this_block = this_block->next;
		  
		}

	    }

	}
      

      P_MESS( "ram_querey", error_message  );
      


      return(NULL);


    }


  /* debug printing */
/*
  if( ram_debug_enabled == 1 ) ram_dump( "ram_more" );

*/
  /* return a single */
  this_block = ram_block_entries[ block_number ];

  return(this_block);



}







/*********************************************************************

  debug printing

*********************************************************************/

void ram_dump( routine )

     char *routine;
{

  int ii;

  float total_size = 0.0e0, total_used = 0.0e0;

  Ram_Block_Data *this_block;

  printf( "\n\n\n------------------------ BEGIN ------------------------");
  printf( "\n\n%s <- calling procedure", routine );

  /* dump it all */
  for(ii=0; ii< MAX_RAM_BLOCKS; ii++ )
    {
      this_block = ram_block_entries[ ii ];
      if( this_block != NULL )
	{
	  
	  printf( "\nblock   head      tail      size        used        left        next      block P ");
	  printf( "\n------  --------  --------  ----------  ----------  ----------  --------  --------");
	  
	  while( this_block != NULL )
	    {
	      printf( "\n%6i  %8p  %8p  %10i  %10i  %10i  %8p  %8p",
		      ii,
		      this_block->head,
		      this_block->tail,
		      this_block->size,
		      this_block->used,
		      this_block->left,
		      this_block->next,
		      this_block );

	      total_size += (float) this_block->size;
	      total_used += (float) this_block->used;
	      
	      this_block = this_block->next;
	      
	    }

	}
      
    }

  printf( "\n total_size -> %g Mbyte  %g Kbyte  %g byte",
	 total_size/1024000.0e0,
	 total_size/1024.0e0,
	 total_size );
	 
  printf( "\n total_used -> %g Mbyte  %g Kbyte  %g byte",
	 total_used/1024000.0e0,
	 total_used/1024.0e0,
	 total_used );
	 

  printf( "\n\n------------------------ END --------------------------");
  fflush( stdout );
      

  return;

}







