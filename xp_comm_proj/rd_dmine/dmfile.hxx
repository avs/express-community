/*
   This class is a quick implementation for a Datamine .dm file reader.
   It was written for importing data into AVS/Express for a customer of
   mine. The work was done in my spare time (for no charge), so I make
   absolutely no guarantees! It works perfectly on my 4 sample data files,
   but check the To Do List for comments.

   If you do use this code please do two things:
   1) Acknowledge my contribution
   2) Send any changes, enhancements, etc for me to add

   Datamine is a product of Mineral Computing Industries Ltd - 
   www.datamine.co.uk

   Many thanks to Ben Heather of Datamine for providing the format
   specification for the .dm files.

   The data stored by the classes are:
   TDMFile:
      ByteSwapped:      whether the data is byte swapped or not
      Description:      description of the file
      DirName:          directory name (not used)
      FileName:         .dm filename
      FileType:         can be:
                           perimeter
                           plot
                           drillhole
                           blockmodel
                           wframetriangle
                           wframepoints
                           sectiondefinition
                           catalogue
                           scheduling
                           results
                           rosette
                           drivestats
                           point
                           dependency
                           plotterpen
                           plotfilter
                           validation
      LastModDate:         date of last modification
      LogicalDataRecLen:   length of the data records in the data pages
      NLastPageRecs:       number of data records in the last page
      nPhysicalPages:      number of 2048 byte pages in the file
      nVars:               number of data variables in the file
      OtherPerms:          file permissions for other users
      Owner:               the owner's name
      OwnerPerms:          file permissions for the owner
      Vars:                the data sets

   TDMVariable:
      ByteSwapped:            whether the file is byte swapped or not
      cData:                  array [nData][SIZE_OF_WORD+1] for character data
      DefaultAlphaNumerical:  the default value for an alphanumerical dataset
      DefaultNumerical:       the default value for a numerical dataset
      fData:                  array [nData] for float data
      LogicalRecPos:          position of the dataset in each logical data record
      Name:                   dataset name
      nData:                  the number of data values - one for each logical data record
      Type:                   either numerical 'N   ' or alphanumerical 'A   '
      Unit                    data units
      WordNumber:             1 for float, otherwise the position for a character

  To Do List:
      The logical record handling is working, but I have only tested it on my
         4 sample datasets
      The alphanumerical importing is completely untested, since none of my 4
         sample datasets had this type of data
      Fix up errors and error reporting

   Revisions:
      99-04-11: Written by Jeremy Maccelari, visualn@iafrica.com
      99-04-16: fixed logical data problems found whilst writing AVS/Express reader, JM
      99-05-03: added byte swapping code for UNIX/PC compatibility, JM
      99-05-06: fixed byte swapping in TDMVariable and added ByteSwapped, JM
      00-06-15: altered byte swapping routine so it reads test data correctly, AWD
*/


#include "dm.hxx"

class TDMVariable
{
   public:
      // data
      bool ByteSwapped;
      float *fData;
      char **cData;
      // methods
      TDMVariable();
      ~TDMVariable();
      float GetDefaultNumerical();
      char *GetDefaultAlphanumerical(char *def);
      int GetLogicalRecPos();
      char *GetName(char *name);
      int GetNData();
      char *GetType(char *fType);
      char *GetUnit(char *name);
      int GetWordNumber();
      void SetDefaultAlphanumericalFromBuf(char *buf,int varNo);
      void SetDefaultNumericalFromBuf(char *buf,int varNo);
      void SetLogicalRecPosFromBuf(char *buf, int varNo);
      void SetNameFromBuf(char *buf, int varNo);
      void SetNData(int ndata);
      void SetTypeFromBuf(char* buf, int varNo);
      void SetUnitFromBuf(char *buf, int varNo);
      void SetWordNumberFromBuf(char *buf, int varNo);
      bool TypeIsNumerical();
   private:
      char DefaultAlphanumerical[SIZE_OF_WORD+1];
      float DefaultNumerical;
      char Type[SIZE_OF_WORD+1];
      int LogicalRecPos;
      char Name[(2*SIZE_OF_WORD)+1];
      int nData;
      char Unit[SIZE_OF_WORD+1];
      int WordNumber;
};

class TDMFile
{
   public:
      // data
      int nVars;
      TDMVariable *Vars;
      // methods
      TDMFile();
      ~TDMFile();
      void DecrementLogicalDataRecLen();
      bool GetByteSwapped();
      char *GetDescription(char *desc);
      char *GetDirName(char *name);
      char *GetFileName(char *name);
      FileTypes GetFileType();
      int GetLastModDate();
      int GetLogicalDataRecLen();
      int GetNLastPageRecs();
      int GetNPhysicalPages();
      int GetOtherPerms();
      char *GetOwner(char *owner);
      int GetOwnerPerms();
      void LoadFile(char *fname);
      void PrintFile(char *fname);
      void SetByteSwapped(char *buf);
      void SetDescriptionFromBuf(char *buf);
      void SetFileNameFromBuf(char *buf);
      void SetFileType();
      void SetLastModDateFromBuf(char *buf);
      void SetLogicalDataRecLenFromBuf(char *buf);
      void SetNLastPageRecsFromBuf(char *buf);
      void SetNPhysicalPagesFromBuf(char *buf);
      void SetOtherPermsFromBuf(char *buf);
      void SetOwnerFromBuf(char *buf);
      void SetOwnerPermsFromBuf(char *buf);
   private:
      bool ByteSwapped;
      char Description[(16*SIZE_OF_WORD)+1];
      char DirName[(2*SIZE_OF_WORD)+1];
      char FileName[(2*SIZE_OF_WORD)+1];
      FileTypes FileType;
      int LastModDate;
      int LogicalDataRecLen;
      int NLastPageRecs;
      int NPhysicalPages;
      int OtherPerms;
      char Owner[(2*SIZE_OF_WORD)+1];
      int OwnerPerms;
};
