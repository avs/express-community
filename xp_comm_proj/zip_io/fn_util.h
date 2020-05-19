/*  WARNING: this file is program generated. */
/*  Remove these two lines to protect the file from overwrites. */
#ifndef ZipIO_FilenameUtils
#define ZipIO_FilenameUtils

#ifdef __cplusplus
extern "C" {
#endif

void spiltFilename(char* fullFilename, char* filename, char* pathname);
int removeSuffix(char* filename, char* suffix, char* newFilename);
int addSuffix(char* filename, char* suffix, char* newFilename);
void combinePathAndFilename(char* pathname, char* filename, char* fullFilename);

#ifdef __cplusplus
}
#endif

#endif /* ZipIO_FilenameUtils */

