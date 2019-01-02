REM
REM Copyright (c) 1999, 2012, Oracle and/or its affiliates. 
REM All rights reserved. 
REM
@echo off
if (%1) == ()         goto usage
if (%1) == (cdemo6)   goto cplusplus
if (%1) == ("cdemo6") goto cplusplus
if (%1) == (CDEMO6)   goto cplusplus
if (%1) == ("CDEMO6") goto cplusplus
if (%1) == (ociucb)   goto callucb
if (%1) == ("ociucb") goto callucb
if (%1) == (OCIUCB)   goto callucb
if (%1) == ("OCIUCB") goto callucb

if (%1) == (cdemoucb) goto callucb2
if (%1) == (CDEMOUCB) goto callucb2

if (%1) == (occiblob)   goto occimake
if (%1) == ("occiblob") goto occimake
if (%1) == (OCCIBLOB)   goto occimake
if (%1) == ("OCCIBLOB") goto occimake
if (%1) == (occidml)    goto occimake
if (%1) == ("occidml")  goto occimake
if (%1) == (OCCIDML)    goto occimake
if (%1) == ("OCCIDML")  goto occimake
if (%1) == (occiclob)   goto occimake
if (%1) == ("occiclob") goto occimake
if (%1) == (OCCICLOB)   goto occimake
if (%1) == ("OCCICLOB") goto occimake
if (%1) == (occicoll)   goto occimake
if (%1) == ("occicoll") goto occimake
if (%1) == (OCCICOLL)   goto occimake
if (%1) == ("OCCICOLL") goto occimake
if (%1) == (occidesc)   goto occimake
if (%1) == ("occidesc") goto occimake
if (%1) == (OCCIDESC)   goto occimake
if (%1) == ("OCCIDESC") goto occimake
if (%1) == (occiobj)    goto occimakeobj
if (%1) == ("occiobj")  goto occimakeobj
if (%1) == (OCCIOBJ)    goto occimakeobj
if (%1) == ("OCCIOBJ")  goto occimakeobj
if (%1) == (occipobj)   goto occimakeobj
if (%1) == ("occipobj") goto occimakeobj
if (%1) == (OCCIPOBJ)   goto occimakeobj
if (%1) == ("OCCIPOBJ") goto occimakeobj
if (%1) == (occiinh)    goto occimakeobj
if (%1) == ("occiinh")  goto occimakeobj
if (%1) == (OCCIINH)    goto occimakeobj
if (%1) == ("OCCIINH")  goto occimakeobj
if (%1) == (occipool)   goto occimake
if (%1) == ("occipool") goto occimake
if (%1) == (OCCIPOOL)   goto occimake
if (%1) == ("OCCIPOOL") goto occimake
if (%1) == (occiproc)   goto occimake
if (%1) == ("occiproc") goto occimake
if (%1) == (OCCIPROC)   goto occimake
if (%1) == ("OCCIPROC") goto occimake
if (%1) == (occistre)   goto occimake
if (%1) == ("occistre") goto occimake
if (%1) == (OCCISTRE)   goto occimake
if (%1) == ("OCCISTRE") goto occimake

REM OCCI 10gR1 demos
if (%1) == (occiaqop)    goto occimakeobj
if (%1) == ("occiaqop")  goto occimakeobj
if (%1) == (OCCIAQOP)    goto occimakeobj
if (%1) == ("OCCIAQOP")  goto occimakeobj
if (%1) == (occiaqlis)   goto occimake
if (%1) == ("occiaqlis") goto occimake
if (%1) == (OCCIAQLIS)   goto occimake
if (%1) == ("OCCIAQLIS") goto occimake
if (%1) == (occiscp)     goto occimake
if (%1) == ("occiscp")   goto occimake
if (%1) == (OCCISCP)     goto occimake
if (%1) == ("OCCISCP")   goto occimake
if (%1) == (occixa)      goto occimake
if (%1) == ("occixa")    goto occimake
if (%1) == (OCCIXA)      goto occimake
if (%1) == ("OCCIXA")    goto occimake
if (%1) == (occiuni1)    goto occimake
if (%1) == ("occiuni1")  goto occimake
if (%1) == (OCCIUNI1)    goto occimake
if (%1) == ("OCCIUNI1")  goto occimake
if (%1) == (occiuni2)    goto occiunimakeobj
if (%1) == ("occiuni2")  goto occiunimakeobj
if (%1) == (OCCIUNI2)    goto occiunimakeobj
if (%1) == ("OCCIUNI2")  goto occiunimakeobj
if (%1) == (occimb1)     goto occimake
if (%1) == ("occimb1")   goto occimake
if (%1) == (OCCIMB1)     goto occimake
if (%1) == ("OCCIMB1")   goto occimake

REM OCCI 10gR2 demos
if (%1) == (occilbar)   goto occimake
if (%1) == ("occilbar") goto occimake
if (%1) == (OCCILBAR)   goto occimake
if (%1) == ("OCCILBAR") goto occimake

if (%1) == (EXTDEMO2)      goto dllmake
if (%1) == ("EXTDEMO2")    goto dllmake
if (%1) == (extdemo2)      goto dllmake
if (%1) == ("extdemo2")    goto dllmake
if (%1) == (EXTDEMO5)      goto dllmake2
if (%1) == ("EXTDEMO5")    goto dllmake2
if (%1) == (extdemo5)      goto dllmake2
if (%1) == ("extdemo5")    goto dllmake2
if (%1) == (EXTDEMO6)      goto dllmake2
if (%1) == ("EXTDEMO6")    goto dllmake2
if (%1) == (extdemo6)      goto dllmake2
if (%1) == ("extdemo6")    goto dllmake2
if (%1) == (cdemodp)       goto cdemodpmake
if (%1) == ("cdemodp")     goto cdemodpmake
if (%1) == (CDEMODP)       goto cdemodpmake
if (%1) == ("CDEMODP")     goto cdemodpmake
if (%1) == (cdemodp_lip)   goto cdemodpmake
if (%1) == ("cdemodp_lip") goto cdemodpmake
if (%1) == (CDEMODP_lip)   goto cdemodpmake
if (%1) == ("CDEMODP_lip") goto cdemodpmake

if (%1) == ("ODBCDEMO") goto odbcdemomake
if (%1) == (ODBCDEMO)   goto odbcdemomake
if (%1) == ("odbcdemo") goto odbcdemomake
if (%1) == (odbcdemo)   goto odbcdemomake

Rem LOB OCI demos
if (%1) == (lobdemo)   goto lobdemomake
if (%1) == ("lobdemo") goto lobdemomake
if (%1) == (LOBDEMO)   goto lobdemomake
if (%1) == ("LOBDEMO") goto lobdemomake

rem set ARCH=/D_X86_=1
rem set MACHINE=IX86
set ARCH=/D_IA64_=1
set MACHINE=AMD64
set MACHINE=X64

rem if defined WIN64BLD set CCFLAGS=/Wp64 /DWIN64 /D_WIN64 /DSS_64BIT_SERVER
rem if defined WIN64BLD set ARCH=/D_IA64_=1
rem if defined WIN64BLD set MACHINE=IA64
rem if defined AMD64BLD set MACHINE=AMD64
rem if defined AMD64BLD set ARCH=/GS- /D_AMD64_=1

cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT %1.c /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib kernel32.lib msvcrt.lib /nod:libc /MACHINE:%MACHINE%
goto end

:cplusplus
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT %1.cpp /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib msvcrt.lib /nod:libc /MACHINE:%MACHINE%
goto end

:occimakeobj
call ott userid=scott/tiger intype=%1.typ outtype=%1out.type code=cpp hfile=%1h.h cppfile=%1o.cpp mapfile=%1m.cpp attraccess=private

rem cl -GX -DWIN32COMMON -I. -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT %1.cpp %1m.cpp %1o.cpp /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib msvcrt.lib msvcprt.lib oraocci12.lib /nod:libc /MACHINE:%MACHINE%
cl  -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT %1.cpp %1m.cpp %1o.cpp /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib msvcrt.lib msvcprt.lib oraocci12.lib /nod:libc /MACHINE:%MACHINE%
goto end

:occiunimakeobj
call ott userid=scott/tiger intype=%1.typ outtype=%1out.type code=cpp hfile=%1h.h cppfile=%1o.cpp mapfile=%1m.cpp attraccess=private unicode=all

cl -GX -DWIN32COMMON /Zc:wchar_t- -I. -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT %1.cpp %1m.cpp %1o.cpp /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib msvcrt.lib msvcprt.lib oraocci12.lib /nod:libc /MACHINE:%MACHINE%
goto end

:occimake
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT %1.cpp /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib msvcrt.lib msvcprt.lib oraocci12.lib %ORACLE_HOME%\rdbms\xa\oraxa12.lib /nod:libc /MACHINE:%MACHINE%
goto end

:callucb
echo Use ociucb.bat to make user call back demos
goto end

:callucb2
REM build cdemoucb.exe
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT %1.c /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib kernel32.lib msvcrt.lib /nod:libc /MACHINE:%MACHINE%
REM call batch file to build ociucb.dll from cdemoucbl.c
call ociucb.bat %1l
goto end

:dllmake
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT %1.c /link /Dll /out:%1l.dll /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib kernel32.lib msvcrt.lib /nod:libc /export:qxiqtbi /export:qxiqtbd /export:qxiqtbu /export:qxiqtbs /export:qxiqtbf /export:qxiqtbc /MACHINE:%MACHINE%
goto end

:dllmake2
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT %1.c /link /Dll /out:%1l.dll /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib kernel32.lib msvcrt.lib /nod:libc /export:qxiqtbpi /export:qxiqtbpd /export:qxiqtbpu /export:qxiqtbps /export:qxiqtbpf /export:qxiqtbpc /MACHINE:%MACHINE%
goto end

:cdemodpmake
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT cdemdpco.c cdemodp.c /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib kernel32.lib msvcrt.lib /nod:libc /out:cdemdpco.exe /MACHINE:%MACHINE%
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT cdemdpin.c cdemodp.c /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib kernel32.lib msvcrt.lib /nod:libc /out:cdemdpin.exe /MACHINE:%MACHINE%
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT cdemdpit.c cdemodp.c /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib kernel32.lib msvcrt.lib /nod:libc /out:cdemdpit.exe /MACHINE:%MACHINE%
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT cdemdplp.c cdemodp.c /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib kernel32.lib msvcrt.lib /nod:libc /out:cdemdplp.exe /MACHINE:%MACHINE%
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT cdemdpno.c cdemodp.c /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib kernel32.lib msvcrt.lib /nod:libc /out:cdemdpno.exe /MACHINE:%MACHINE%
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT cdemdpro.c cdemodp.c /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib kernel32.lib msvcrt.lib /nod:libc /out:cdemdpro.exe /MACHINE:%MACHINE%
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT cdemdpss.c cdemodp.c /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib kernel32.lib msvcrt.lib /nod:libc /out:cdemdpss.exe /MACHINE:%MACHINE%
goto end

:lobdemomake
cl -I%ORACLE_HOME%\oci\include -I. -D_DLL %CCFLAGS% %ARCH% -D_MT lobdemo.c lwrite.c lread.c llength.c ldisplay.c lappend.c lcopy.c lcopyloc.c ldisbuf.c lequal.c lerase.c lgetchar.c lgetchfm.c linit.c lisopen.c listemp.c lloaddat.c ltrim.c lwriteap.c linsert.c fclose_c.c fclose_f.c fclosea.c fcopyloc.c fdisplay.c fexists.c ffilopen.c ffisopen.c fgetdir.c finsert.c fisopen.c flength.c fopen.c fread.c fupdate.c lreadarr.c lwritearr.c  /link /LIBPATH:%ORACLE_HOME%\oci\lib\msvc oci.lib kernel32.lib msvcrt.lib /nod:libc /out:lobdemo.exe /MACHINE:%MACHINE%
goto end

:odbcdemomake
cl %CCFLAGS% %ARCH% -I"%VCINSTALLDIR%\include" -I"%VCINSTALLDIR%\include\MFC" -DWIN32COMMON odbc32.lib %1.c
goto end

:usage
echo.
echo Usage: make filename [i.e. make oci01]
echo.
:end
