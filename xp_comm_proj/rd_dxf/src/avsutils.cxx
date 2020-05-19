/*****************************************************************************

			Copyright (c) 1995 by
			Advanced Visual Systems Inc.
			All Rights Reserved
	
	This software comprises unpublished confidential information of
	Advanced Visual Systems Inc. and may not be used, copied or made
	available to anyone, except in accordance with the license
	under which it is furnished.

******************************************************************************

A toolbox of miscellaneous useful classes.

******************************************************************************
*****************************************************************************/

#include <string.h>

#include "avsutils.hxx"


/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Global Variables  *************************/

/* <none> */



/*****************************************************************************
***********--------   Implementations of Public Methods   --------************
*****************************************************************************/


/*****************************************************************************
Method:   AVSString::Set()
Purpose:  Sets new string text, replacing any existing text.
Params:   s   A pointer to the new text.
State:    Any existing text is destroyed.
Returns:  ---
*****************************************************************************/
void AVSString::Set(const char *s)
{
   if (text)
      delete [] text;

   text = StrDup(s);
}


/*****************************************************************************
Method:   AVSString::ReplaceSubstring()
Purpose:  Replaces the characters of the text string between the specified
          start and end positions with the specified new string.
Params:   startPos  The index of the first character to remove (the index of
                    the first character in the string is 0).
	  endPos    The index of the character immediately following the
		    last character to remove.  If startPos == endPos, no
		    characters are removed.
	          * If either of the indexes is out of range, the index of
		    the string's null terminator is assumed.  Specifically:
		       if endPos < 0, endPos = strlen(text)
		       if endPos > strlen(text), endPos = strlen(text)
		    likewise for startPos.
          s         A pointer to the new text to replace the removed
	            characters.  The length of this string need not equal
		    the number of characters removed.
		  * If this parameter is NULL, then no new text is inserted.
		  * This parameter is optional; the default value is NULL.
State:    If there is no current text, the specified string becomes the
          current text.
Returns:  ---
*****************************************************************************/
void AVSString::ReplaceSubstring(int startPos, int endPos, const char *s)
{
   int textLen = text ? strlen(text) : 0;
   int sLen = s ? strlen(s) : 0;
   char *d;

   if ((startPos < 0) || (startPos > textLen))
      startPos = textLen;

   if ((endPos < 0) || (endPos > textLen))
      endPos = textLen;

   if (endPos < startPos)
      endPos = startPos;

   int delLen = endPos - startPos;   // number of chars to delete from text

   if (sLen == delLen)  // if we don't need to reallocate
   {
      if (sLen > 0)
	 for (d = text + startPos; sLen > 0; d++, s++, sLen--)
	    *d = *s;
   }
   else if ((textLen + sLen) > delLen)  // if target string is not 0 length
   {
      d = new char[textLen + sLen - delLen + 1];

      if (d)
      {
	 char *sp;
	 char *dp = d;

	 if (text)
	    for (sp = text; startPos > 0; dp++, sp++, startPos--)
	       *dp = *sp;

	 if (s)
	    for ( ; sLen > 0; dp++, s++, sLen--)
	       *dp = *s;

	 if (text)
	 {
	    for (sp = text + endPos; *sp; dp++, sp++)
	       *dp = *sp;

	    delete [] text;
	 }

	 *dp = '\0';
	 text = d;
      }
   }
}



/*****************************************************************************
*******************--------   Private Functions   --------********************
*****************************************************************************/


/*****************************************************************************
Method:   AVSString::StrDup()
Purpose:  Allocates a new buffer and (if successful) copies the contents of
          the specified string into this new buffer.
Params:   s   A pointer to the string to duplicate.
              If this parameter is NULL, then no new buffer is allocated.
State:    ---
Returns:  A pointer to a new buffer containing a copy of "s", except:
          NULL if "s" is NULL, or if the new buffer could not be allocated.
*****************************************************************************/
char *AVSString::StrDup(const char *s)
{
   char *d = 0;

   if (s)
   {
      d = new char[strlen(s) + 1];

      if (d)
	 strcpy(d, s);
   }

   return d;
}


/*****************************************************************************
Method:   ()
Purpose:  
          
Params:   
          
	  
State:    
          
Returns:  
          
*****************************************************************************/
