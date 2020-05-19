#include "gsbyteu.h"

//
//  Constructor.
//

GIS_ByteUtil_c::GIS_ByteUtil_c()
{
    union
    {
      unsigned short jShort;
      unsigned char  iByte[2];
    } uTest;

    uTest.jShort = 0Xff00;

    if (uTest.iByte[0] == 0Xff)
    {
      _ByteOrder = BIG_ENDIAN;
    }
    else
    {
      _ByteOrder = LITTLE_ENDIAN;
    }
};

//
//  destructor.
//

GIS_ByteUtil_c::~GIS_ByteUtil_c()
{
};

//
//  Copy constructor.
//

GIS_ByteUtil_c::GIS_ByteUtil_c(const GIS_ByteUtil_c &object)
{
    *this = object;
}

//
//  Assignment operator
//

GIS_ByteUtil_c &GIS_ByteUtil_c::operator=(const GIS_ByteUtil_c &object)
{
    _ByteOrder = object._ByteOrder;
    return *this;
}

//
//  Method to get the byte order for this machine.
//
//  Returns one of:
//     GIS_ByteUtil_c::BIG_ENDIAN  
//     GIS_ByteUtil_c::LITLE_ENDIAN
//

unsigned char GIS_ByteUtil_c::ByteOrder()
{
    return _ByteOrder;
}

//
//  Method to swap bytes.  Given an array of bytes, it swaps bytes
//  within each 2-byte word.  The bytes are swapped in place.
//
//  Returns a pointer to the array passed in as SwapBytes.
//

void *GIS_ByteUtil_c::SwapBytes2(unsigned long  NumberOfBytes,
                                 void          *SwapBuffer)
{
    unsigned long  k1;           // loop counter
    unsigned char *ByteArray;    // ptr to array of bytes to swap
    unsigned char  iTemp0;       // temporary value for byte # 0

    ByteArray = (unsigned char *) SwapBuffer;

    //
    //  kNumberOfBytes must be evenly divisible by 2
    //

    if (NumberOfBytes%2 != 0)
    {
      return NULL;
    }

    //
    //  If the swap buffer is invalid return NULL.
    //

    if (SwapBuffer == NULL)
    {
      return NULL;
    }

    //
    //  Swap the bytes
    //

    for (k1 = 0; k1 < NumberOfBytes; k1+=2)
    {
      iTemp0 = ByteArray[k1];
      ByteArray[k1]   = ByteArray[k1+1];
      ByteArray[k1+1] = iTemp0;
    }

    return ByteArray;
}

//
//  Method to swap bytes.  Given an array of bytes, it swaps bytes
//  within each 4-byte word.  The bytes are swapped in place.
//
//  Returns a pointer to the array passed in as SwapBytes.
//

void *GIS_ByteUtil_c::SwapBytes4(unsigned long  NumberOfBytes,
                                 void          *SwapBuffer)
{
    unsigned long  k1;           // loop counter
    unsigned char *ByteArray;    // ptr to array of bytes to swap
    unsigned char  iTemp0;       // temporary value for byte # 0
    unsigned char  iTemp1;       // temporary value for byte # 1

    ByteArray = (unsigned char *) SwapBuffer;

    //
    //  kNumberOfBytes must be evenly divisible by 4
    //

    if (NumberOfBytes%4 != 0)
    {
      return NULL;
    }

    //
    //  If the swap buffer is invalid return NULL.
    //

    if (SwapBuffer == NULL)
    {
      return NULL;
    }

    //
    //  Swap the bytes
    //

    for (k1 = 0; k1 < NumberOfBytes; k1+=4)
    {
      iTemp0 = ByteArray[k1];
      iTemp1 = ByteArray[k1+1];
      ByteArray[k1]   = ByteArray[k1+3];
      ByteArray[k1+1] = ByteArray[k1+2];
      ByteArray[k1+2] = iTemp1;
      ByteArray[k1+3] = iTemp0;
    }

    return ByteArray;
}

//
//  Method to swap bytes.  Given an array of bytes, it swaps bytes
//  within each 8-byte word.  The bytes are swapped in place.
//
//  Returns a pointer to the array passed in as SwapBytes.
//

void *GIS_ByteUtil_c::SwapBytes8(unsigned long  NumberOfBytes,
                                 void          *SwapBuffer)
{
    unsigned long  k1;           // loop counter
    unsigned char *ByteArray;    // ptr to array of bytes to swap
    unsigned char  iTemp0;       // temporary value for byte # 0
    unsigned char  iTemp1;       // temporary value for byte # 1
    unsigned char  iTemp2;       // temporary value for byte # 2
    unsigned char  iTemp3;       // temporary value for byte # 3

    ByteArray = (unsigned char *) SwapBuffer;

    //
    //  kNumberOfBytes must be evenly divisible by 8
    //

    if (NumberOfBytes%8 != 0)
    {
      return NULL;
    }

    //
    //  If the swap buffer is invalid return NULL.
    //

    if (SwapBuffer == NULL)
    {
      return NULL;
    }

    //
    //  Swap the bytes
    //

    for (k1 = 0; k1 < NumberOfBytes; k1+=8)
    {
      iTemp0 = ByteArray[k1];
      iTemp1 = ByteArray[k1+1];
      iTemp2 = ByteArray[k1+2];
      iTemp3 = ByteArray[k1+3];
      ByteArray[k1]   = ByteArray[k1+7];
      ByteArray[k1+1] = ByteArray[k1+6];
      ByteArray[k1+2] = ByteArray[k1+5];
      ByteArray[k1+3] = ByteArray[k1+4];
      ByteArray[k1+4] = iTemp3;
      ByteArray[k1+5] = iTemp2;
      ByteArray[k1+6] = iTemp1;
      ByteArray[k1+7] = iTemp0;
    }

    return ByteArray;
}
