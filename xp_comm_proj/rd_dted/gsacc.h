//
//  Include file for the XP_GIS_DTEDACC_c class, which provides utilities
//  to read a DTED_ACC header record from a DTED file.
//

/*

    Public Constructors

      XP_GIS_DTEDACC_c();
        The constructor.

      virtual ~XP_GIS_DTEDACC_c();
        the destructor.

      XP_GIS_DTEDACC_c(const XP_GIS_DTEDACC_c &object);
        The copy constructor


    Public Operators

      XP_GIS_DTEDACC_c &operator=(const XP_GIS_DTEDACC_c &object);
        The assignment operator assigns all values as necessary.

    Public Methods

      unsigned long Print(ostream &PrintStream) const;
        Method to print a human-readable version of the ACC header record.

      unsigned long Read(ifstream *FileStream);
        Method to read the ACC record from the DTED file specified by
        FileStream.

      const char     *RecognitionSentinel() const;
         Method to get the recognition sentinel stored in the ACC record.
         A value of USHRT_MAX means that the field had a value of NA in
         the DTED file.

      unsigned short AbsoluteHorizontalAccuracy() const;
         Method to get the absolute horizontal accuracy field stored
         in the ACC record.  A value of USHRT_MAX means that the field 
         had a value of NA in the DTED file.

      unsigned short AbsoluteVerticalAccuracy() const;
         Method to get the absolute vertical accuracy field stored
         in the ACC record.  A value of USHRT_MAX means that the field 
         had a value of NA in the DTED file.

      unsigned short RandomHorizontalAccuracy() const;
         Method to get the random horizontal accuracy field stored
         in the ACC record.  A value of USHRT_MAX means that the field 
         had a value of NA in the DTED file.

      unsigned short RandomVerticalAccuracy() const;
         Method to get the random vertical accuracy  stored
         in the ACC record.  A value of USHRT_MAX means that the field 
         had a value of NA in the DTED file.

      unsigned short MultipleAccuracyOutlineFlag() const;
         Method to get the multiple accuracy outline flag stored
         in the ACC record.  A value of USHRT_MAX means that the field 
         had a value of NA in the DTED file.

*/



#ifndef XP_GIS_DTED_ACC_H
#define XP_GIS_DTED_ACC_H

#include <fstream.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>

#include "gserror.h"
#include "gsconst.h"

class XP_GIS_DTEDACC_c
{
  public:
    XP_GIS_DTEDACC_c();
    virtual ~XP_GIS_DTEDACC_c();
    XP_GIS_DTEDACC_c(const XP_GIS_DTEDACC_c &object);
    XP_GIS_DTEDACC_c &operator=(const XP_GIS_DTEDACC_c &object);

    unsigned long Print(ostream &PrintStream) const;

    unsigned long Read(ifstream *FileStream);

    //
    //  Accesors to get values
    //

    const char     *RecognitionSentinel()
                         const {return _RecognitionSentinel;}
    unsigned short AbsoluteHorizontalAccuracy()
                         const {return _AbsoluteHorizontalAccuracy;}
    unsigned short AbsoluteVerticalAccuracy()
                         const {return _AbsoluteVerticalAccuracy;}
    unsigned short RandomHorizontalAccuracy()
                         const {return _RandomHorizontalAccuracy;}
    unsigned short RandomVerticalAccuracy()
                         const {return _RandomVerticalAccuracy;}
    unsigned short MultipleAccuracyOutlineFlag()
                         const {return _MultipleAccuracyOutlineFlag;}

  private:

    unsigned long _ReadACC(ifstream *FileStream);
    unsigned long _RecordStart(ifstream      *FileStream,
                               unsigned long *RecordStart);

    char           _RecognitionSentinel[4];
    unsigned short _AbsoluteHorizontalAccuracy;   // USHRT_MAX means N/A
    unsigned short _AbsoluteVerticalAccuracy;     // USHRT_MAX means N/A
    unsigned short _RandomHorizontalAccuracy;     // USHRT_MAX means N/A
    unsigned short _RandomVerticalAccuracy;       // USHRT_MAX means N/A
    unsigned short _MultipleAccuracyOutlineFlag;  // USHRT_MAX means N/A

};

#endif   // XP_GIS_DTED_ACC_H

