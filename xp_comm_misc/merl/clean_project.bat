@ECHO OFF
REM
REM The following directories/files are generated by Express and can therefore
REM be safely removed for transporting the project in its most compact form.
REM
REM  To re-generate the project:
REM    1) cd to the project directory.
REM    2) Edit the "avsenv" file to reflect the current directory paths of the
REM       project and the AVS/Express install directory.
REM    3) Type "base -gen_proc express -exit".
REM    4) Type "make -f express.mk".
REM

REM Express generated files
DEL /s /q *.vo
DEL /s /q *_gen.*
DEL /s /q *.obj
IF EXIST bin RMDIR /s /q bin
IF EXIST include RMDIR /s /q include
IF EXIST lib RMDIR /s /q lib
IF EXIST express.* DEL express.*
IF EXIST xexpress.cxx DEL xexpress.cxx

REM Emacs generated files
DEL /s /q #*#
DEL /s /q *.*.~*~

REM Misc stuff
IF EXIST AUTOSAVE* DEL AUTOSAVE*
DEL /s junk.*

ECHO Project clean
