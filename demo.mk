#/bin/make
###############################################################################
#                       Make file for OCI and OCCI demos
###############################################################################
#  Usage :
# For compiling & linking the cdemo81.c file
#    make -f demo.mk buildoci EXE=cdemo81 OBJS=cdemo81.o 
#
# For compiling & linking the occidml.cpp
#    make -f demo.mk buildocci EXE=occidml OBJS=occidml.o
#
# For compiling & linking the occiobj.cpp
#    make -f demo.mk occiobj 
#
# In general, for any occi program
#    make -f demo.mk buildocci EXE=<exename> OBJS="<list of dependent objs>"
#
# For compiling all demos
#    make -f demo.mk
#
# NOTE: Please change cc and CC to point to the appropiate location on your
#       machine.
#
###############################################################################

.SUFFIXES: .o .c .cpp

CC=/opt/SunProd/SUNWspro6.1/bin/CC
cc=/opt/SunProd/SUNWspro6.1/bin/cc

ICINCHOME=../
ICLIBHOME=../../
ICLIBPATH=-L$(ICLIBHOME)
THREADLIBS=-lthread
CCLIB=$(ICLIBPATH) -locci -lclntsh $(THREADLIBS)

CCINCLUDES = -I$(ICINCHOME)include

CCFLAGS=$(CCINCLUDES) -D_REENTRANT -g -xs 
LDFLAGS=
SO_EXT=.so

REMOVE=rm -rf
MKLINK=ln
MAKE=make
MAKEFILE=demo.mk
CLNCACHE=cleancache
CACHEDIR=SunWS_cache

CDEMOEXE=cdemo81
CDEMOOBJS=cdemo81.o
OCCIDEMO=occidml
OCCIOBJDEMO=occiobj
OTT=../ott
OCCIOTTUSR=hr
OCCIOTTPWD=hr

.cpp.o:
	$(CC) -c -I$(ICINCHOME)include $(CCFLAGS) $<

.c.o:
	$(cc) -c -I$(ICINCHOME)include $(CCFLAGS) $<

all: clean buildoci $(OCCIDEMO) $(OCCIOBJDEMO)

buildoci: $(CLNCACHE) $(LIBCLNT) $(CDEMOOBJS)
	$(CC) -o $(CDEMOEXE) $(LDFLAGS) $(CDEMOOBJS) $(CCLIB)

buildocci: $(CLNCACHE) $(LIBCLNT) $(OBJS)
	$(CC) -o $(EXE) $(LDFLAGS) $(OBJS) $(CCLIB)

$(OCCIDEMO):
	$(MAKE) -f $(MAKEFILE) buildocci OBJS=$@.o EXE=$@

$(OCCIOBJDEMO):
	$(OTT) userid=$(OCCIOTTUSR)/$(OCCIOTTPWD) \
                intype=$@.typ \
                outtype=$@out.type \
                code=cpp \
                hfile=$@.h \
                cppfile=$@o.cpp \
                attraccess=private \
                unicode=none
	$(MAKE) -f $(MAKEFILE) buildocci OBJS="$@.o $@m.o $@o.o" EXE=$@

cleancache:
	$(REMOVE) $(CACHEDIR)

clean: $(CLNCACHE)
	$(REMOVE) cdemo81 cdemo81.o occidml occidml.o occiobj occiobj.o occiobjo* occiobjm* occiobj.h occiobjout.type



#
# This port-specific file is currently empty on Solaris. Product
# lines may use this file to override compiler definitions and
# flags used in occi.mk.
#

cc=oraxlc
CC=oraxlc -cpp

# Pick up object mode from environment if set. Otherwise, check 
# whether QA home is 32bit or 64bit.
ifneq ("$(OBJECT_MODE)", "32")
ifneq ("$(OBJECT_MODE)", "64")
    OBJECT_MODE := $(shell if [ `file $S/lib/nautab.o | awk ' {print $2}' ` = "64-bit" ]; then echo "64"; \
                           else echo "32"; fi)
endif
endif

# Long double flag required for both 64-bit & 32-bit
LDBL=-qlongdouble

CCFLAGS=-g -q$(OBJECT_MODE) $(LDBL) $(CCINCLUDES) -brtl
CCLIB+=$(OCCILIBPATH) -L../../ -locci -lclntsh $(THREADLIBS) /usr/lib/libC.a -brtl
##CCLIB+=$(OCCILIBPATH) -L../../ -locci -lclntsh $(THREADLIBS) -F$S/lib/xlc.cfg /usr/lib/libC.a -brtl

# Linking flags
LDFLAGS=-q$(OBJECT_MODE)
THREADLIBS=-lpthread

# No cache directory on AIX
CLNCACHE=

