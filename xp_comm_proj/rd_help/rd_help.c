*/

#include <stdio.h>
#include "gen.h"

#define DEBUG

void* my_rd_help_realloc(void *ptr, size_t size, size_t oldsize)
{
#ifdef MS_DOS
	/* fix for missing realloc in MS VC6.0 */
	void *new;
	if (new=(void*)malloc(size))
	{
		memcpy(new,ptr,oldsize);
		free(ptr);
		ptr=new;
	}
	return new;
#else
	return realloc(ptr,size);
#endif
}

int readHelp(OMobj_id VM_Read_Help_id, OMevent_mask event_mask, int seq_num)
{
	/***********************/
	/*  Declare variables  */
	/***********************/
	char *filename = NULL;
	char *text = NULL;

	char *txt;
	FILE *input;
	int i;
	int width;
	int rows;
	int cols;
	int newsize;
	int first;
	char buf[512];
	char bf[1024];
	char *b, *buff;
	int maxStrLen;
	int buffStrLen;

	/* Get width's value, default to 0 (not set) */
	if (OMget_name_int_val(VM_Read_Help_id, OMstr_to_name("width"), &width) !=1)
		width = 0;

	/* Get filename's value, no default */
	if (OMget_name_str_val(VM_Read_Help_id, OMstr_to_name("filename"), &filename, 0) != 1)
	{
		fprintf( stderr, "Unable to get value for filename from OM...\n" );
		return 0;
	}

	if( input = fopen( filename, "r" ) )
	{
		newsize = 0;
		first = 1;
		rows = 0;
		cols = 0;
		text = NULL;	
		maxStrLen = 512;	
		txt=(char*)malloc(maxStrLen*sizeof(char));
		txt[0]=0;
		if(txt==NULL) 
		{
			fprintf(stderr,"Can't allocate memory");
			return 0;
		}
		
		buffStrLen = 512;	
		buff = (char*)malloc (buffStrLen*sizeof(char));
 		buff[0]=0;
		while(fgets(buf, sizeof(buf), input ))
		{   
			rows += 1;
			if (buf[strlen(buf)-1]=='\n') buf[strlen(buf)-1]=0;
			if (buf[0]) 
			{
				while((strlen(buff)+strlen(buf)+2) >buffStrLen )
				{
					buffStrLen += 512; 
					buff = (char*)my_rd_help_realloc(buff,buffStrLen*sizeof(char),strlen(buff)); 	
				} 
				if (buff[0]) strcat(buff," ");
				strcat(buff,buf);
			} else {
				while((strlen(txt)+strlen(buff)+2) >maxStrLen )
				{
					maxStrLen += 512; 
					txt = (char*)my_rd_help_realloc(txt,maxStrLen*sizeof(char),strlen(txt)); 	
				} 
				strcat(txt,buff);
				buff[0]=0;
				strcat(txt,"\n\n");
				rows+=2;
			}									

			b=buff;
			while(strlen(b)>width)
			{  
				i=width;
				while( (i!=0) && (!isspace(b[i]))) 
					i--;
				if(i==0)
				{   
					i=width+1;
					while((strlen(b)>i) && !isspace(b[i])) 
						i++;  
				}
				strncpy(bf,b,i);
				bf[i]=0;
				b += i+1 ;
				while((strlen(txt)+strlen(bf)+2) >maxStrLen )
				{
					maxStrLen += 512; 
					txt = (char*)my_rd_help_realloc(txt,maxStrLen*sizeof(char),strlen(txt)); 	
				} 
				strcat(txt,bf);
				if (strlen(bf)+1 > (unsigned int)cols)
					cols = strlen(bf)+1;
				strcat(txt,"\n");
				rows+=1; 
			}
			
			strcpy(buff,b);	
		} 
		text=txt; 
		fclose(input);
	} else {	
		char buf[512];
		printf( buf, "Unable to open help file %s\n", filename );
		text = strdup(buf);
	}

	/*Set text's value*/ 

	OMset_name_str_val(VM_Read_Help_id, OMstr_to_name("text"), text);
	OMset_name_int_val(VM_Read_Help_id, OMstr_to_name("rows"), rows);
	OMset_name_int_val(VM_Read_Help_id, OMstr_to_name("cols"), cols);

	/*Cleanup and return*/

	if(filename)free(filename);
	if(txt)free(txt);

	return 1;
}
