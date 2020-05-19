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

#ifndef _AVS_UTILITY_CLASSES_H
#define _AVS_UTILITY_CLASSES_H


/*********************  Macros and Manifest Constants  **********************/

/* <none> */


/************************  Public Type Definitions  *************************/

class AVSPoint
{
  public:
   AVSPoint() : x(0), y(0), z(0) {}
   AVSPoint(float a, float b, float c) : x(a), y(b), z(c) {}
   ~AVSPoint() {}

   void Set(float a, float b, float c)           { x = a; y = b; z = c; }
   void Set(const AVSPoint &p)                   { p.Get(&x, &y, &z); }
   void Get(float *a, float *b, float *c) const  { *a = x; *b = y; *c = z; }
   void Get(AVSPoint &p) const                   { p.Set(x, y, z); }

   AVSPoint &operator=(const AVSPoint &p)  { Set(p); return *this; }

  protected:
   float x, y, z;
};


class AVSString
{
  public:
   AVSString() : text(0)  {}
   AVSString(const char *s) : text(StrDup(s))  {}
   AVSString(const AVSString &a) : text(StrDup(a.text)) {}
   ~AVSString()  { if (text) delete [] text; }

   const char *Get() const      { return (const char *)(text ? text : ""); }
   void Set(const char *s);
   void Set(const AVSString &as)  { Set(as.Get()); }
   void ReplaceSubstring(int startPos, int endPos, const char *s = 0);
   void ReplaceSubstring(int startPos, int endPos, const AVSString &as)
           { ReplaceSubstring(startPos, endPos, as.Get()); }

   void Cat(const char *s)              { ReplaceSubstring(-1, -1, s); }
   void Cat(const AVSString &as)        { Cat(as.Get()); }
   void Insert(int pos, const char *s)  { ReplaceSubstring(pos, pos, s); }
   void Insert(int pos, const AVSString &as)  { Insert(pos, as.Get()); }

   const AVSString &operator=(const AVSString &as)  { Set(as); return *this; }
   const AVSString &operator=(const char *s)        { Set(s); return *this; }

   AVSString operator+(const AVSString &as)
                          { AVSString a(*this); a.Cat(as); return a; }
   AVSString operator+(const char *s)
                          { AVSString a(*this); a.Cat(s); return a; }

   const AVSString &operator+=(const AVSString &as) { Cat(as); return *this; }
   const AVSString &operator+=(const char *s)       { Cat(s); return *this; }

  private:
   char *text;

   char *StrDup(const char *s);
};


/************************  Public Global Variables  *************************/

/* <none> */


/********************  Prototypes for Public Functions  ********************/

/* <none> */



#endif
