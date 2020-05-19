//
//  Include file for the DBF_ByteUtil_c class, which provides utilities
//  for working with byte swapping.
//

/*

    Public Constructors

      DBF_ByteUtil_c(const char *FileName=NULL);
        The constructor.

      virtual ~DBF_ByteUtil_c();
        The destructor.

      DBF_ByteUtil_c(const DBF_ByteUtil_c &object);
        The copy constructor.


    Public Operators

      DBF_ByteUtil_c &operator=(const DBF_ByteUtil_c &object);
        The assignment operator.


    Public Methods

      unsigned char ByteOrder();
        Method to get the byte order for this mahcine.  Returns one of
        XP_GIS_LITTLE_ENDIAN or XP_GIS_BIG_ENDIAN.

      void *SwapBytes8(unsigned long NumberOfBytes, void* SwapBuffer);
        Method to swap byes.  Given an array of bytes, it swaps bytes
        within each 8-byte word.  The bytes are swapped in place and
        a pointer to SwapBuffer is returned.

      void *SwapBytes4(unsigned long NumberOfBytes, void* SwapBuffer);
        Method to swap byes.  Given an array of bytes, it swaps bytes
        within each 4-byte word.  The bytes are swapped in place and
        a pointer to SwapBuffer is returned.

        This method expects the number of bytes to be swapped to be
        evenly divisible by 4.  If not, NULL is returned.

*/


#ifndef XP_GIS_BYTE_UTIL_H
#define XP_GIS_BYTE_UTIL_H


#include <malloc.h>
#include <string.h>

class DBF_ByteUtil_c
{
  public:
    DBF_ByteUtil_c();
    ~DBF_ByteUtil_c();
    DBF_ByteUtil_c(const DBF_ByteUtil_c &object);
    DBF_ByteUtil_c &operator=(const DBF_ByteUtil_c &object);

    unsigned char ByteOrder();
    void *SwapBytes8(unsigned long NumberOfBytes, void* SwapBuffer);
    void *SwapBytes4(unsigned long NumberOfBytes, void* SwapBuffer);
    void *SwapBytes2(unsigned long NumberOfBytes, void* SwapBuffer);
#ifdef LITTLE_ENDIAN
	#undef LITTLE_ENDIAN
#endif
#ifdef BIG_ENDIAN
	#undef BIG_ENDIAN
#endif
    enum ByteOrder_e {LITTLE_ENDIAN=1,BIG_ENDIAN=2};

  private:
    enum ByteOrder_e _ByteOrder;

};

#endif   // XP_GIS_BYTE_UTIL_H
