# Raima Database Manager
#
# Copyright (c) 2010 Raima Inc.,  All rights reserved.
#
# Use of this software, whether in source code format, or in executable,
# binary object code form, is governed by the Raima LICENSE which
# is fully described in the LICENSE.TXT file, included within this
# distribution of files.

USE_DASH_L = 1
UNIX       = 1

MATH_LIB = -lm
COMPRESS_LIB = -lz

## ---------------------------------------------------------------------------
## How to define symbols in the compiler command line:
##

D = -D

## ---------------------------------------------------------------------------
## How to specify include directories in the compiler command line:
##

I = -I

## ---------------------------------------------------------------------------
## How to make an EXE
##

#define MKEXE{prog,ojbs,libs,syslibs,rc,noinstall}
#let prog $(prog)

#ifdef NO_EXECUTABLES
$O$(prog)$e:
    $(NOP)
#else
$O$(prog)$e: $(objs)
    (cd $(SYSDIR) && $(SYSLINK) $(LFLAGS) $(LOPTS) $(LEXE) -o $(prog) $(objs) -L$(DLIB) $(libs) $(syslibs) $(LEXELIBS))
#ifb $(noinstall)
    $#LNFILE{from=$O$(prog)$e, to=$(DBIN)$(prog)$e}
#endif
#endif

#edef

## ---------------------------------------------------------------------------
## How to make an C++ EXE
##

#define MKCPPEXE{prog,ojbs,libs,syslibs,rc,noinstall}
#let prog $(prog)

#ifdef NO_EXECUTABLES
$O$(prog)$e:
    $(NOP)
#else
$O$(prog)$e: $(objs)
    (cd $(SYSDIR) && $(SYSCPPLINK) $(LFLAGS) $(CPPLFLAGS) $(LOPTS) $(LEXE) -o $(prog) $(objs) -L$(DLIB) $(libs) $(syslibs) $(LEXELIBS))
#ifb $(noinstall)
    $#LNFILE{from=$O$(prog)$e, to=$(DBIN)$(prog)$e}
#endif
#endif

#edef

## ---------------------------------------------------------------------------
## How to make an Objective-C EXE
##

#define MKOBJCEXE{prog,ojbs,libs,syslibs,rc,noinstall}
#let prog $(prog)

#ifdef NO_EXECUTABLES
$O$(prog)$e:
    $(NOP)
#else
$O$(prog)$e: $(objs)
    (cd $(SYSDIR) && $(SYSOBJCLINK) $(OBJCFLAGS) $(OBJCLFLAGS) $(LOPTS) $(LEXE) -o $(prog) $(objs) -L$(DLIB) $(libs) $(syslibs) $(LEXELIBS))
#ifb $(noinstall)
    $#LNFILE{from=$O$(prog)$e, to=$(DBIN)$(prog)$e}
#endif
#endif

#edef

## ---------------------------------------------------------------------------
## How to make an DLL (shared library)
##

#ifdef STATIC_LINK

#define MKDLL{dll,objs,subdir}
#let dll $(dll)

$Olib$(dll)$d: $(objs)
    (cd $(SYSDIR) && $(SYSLIB) $(LIBFLAGS) lib$(dll)$d $(objs))
    $#MKDIR{dir=$(DLIB)$(subdir)}
    $#LNFILE{from=$Olib$(dll)$d, to=$(DLIB)$(subdir)$/lib$(dll)$d}

#edef

#else

#define MKDLL{dll,objs,libs,syslibs,def,rc,subdir}
#let dll $(dll)
#ifndef VXWORKS
._l$(dll)=-L$(DLIB) $(libs)
#endif

#ifdef LDEFOPT
._f$(dll)=$(LDEFOPT)$(def)$f
#endif

$Olib$(dll)$d: $(objs)
    (cd $(SYSDIR) && $(SYSLINK) $(LFLAGS) $(LOPTS) $(LDLL) $(._f${dll}) -o lib$(dll)$d $(objs) $(._l${dll}) $(syslibs) $(LDLLLIBS))
    $#MKDIR{dir=$(DLIB)$(subdir)}
    $#LNFILE{from=$Olib$(dll)$d, to=$(DLIB)$(subdir)$/lib$(dll)$d}

#edef
#endif 

## ---------------------------------------------------------------------------
## How to make a C++ DLL (shared library)
##

#ifdef STATIC_LINK

#define MKCPPDLL{dll,objs,subdir}
#let dll $(dll)

$Olib$(dll)$d: $(objs)
    (cd $(SYSDIR) && $(SYSLIB) $(LIBFLAGS) lib$(dll)$d $(objs))
    $#MKDIR{dir=$(DLIB)$(subdir)}
    $#LNFILE{from=$Olib$(dll)$d, to=$(DLIB)$(subdir)$/lib$(dll)$d}
#edef

#else

#define MKCPPDLL{dll,objs,libs,syslibs,def,rc,subdir}
#let dll $(dll)
#ifndef VXWORKS
._l$(dll)=-L$(DLIB) $(libs)
#endif

#ifdef LDEFOPT
#ifnb $(def)
._f$(dll)=$(LDEFOPT)$(def)$f
#endif
#endif

$Olib$(dll)$d: $(objs)
    (cd $(SYSDIR) && $(SYSCPPLINK) $(LFLAGS) $(LOPTS) $(CPPLDLL) $(._f${dll}) -o lib$(dll)$d $(objs) $(._l${dll}) $(syslibs) $(LDLLLIBS))
    $#MKDIR{dir=$(DLIB)$(subdir)}
    $#LNFILE{from=$Olib$(dll)$d, to=$(DLIB)$(subdir)$/lib$(dll)$d}
#edef
#endif 

## ---------------------------------------------------------------------------
## How to make an Objective-C DLL (shared library)
##

#ifdef STATIC_LINK

#define MKOBJCDLL{dll,objs,subdir}
#let dll $(dll)

$Olib$(dll)$d: $(objs)
    (cd $(SYSDIR) && $(SYSLIB) $(LIBFLAGS) lib$(dll)$d $(objs))
    $#MKDIR{dir=$(DLIB)$(subdir)}
    $#LNFILE{from=$Olib$(dll)$d, to=$(DLIB)$(subdir)$/lib$(dll)$d}
    $#IF_EQUAL{var1=$(RDM_TFS), var2=$(subdir), do=$#LNFILE{from=$Olib$(dll)$d, to=$(DLIB)lib$(dll)$d}}


#edef

#else

#define MKOBJCDLL{dll,objs,libs,syslibs,def,rc,subdir}
#let dll $(dll)
#ifndef VXWORKS
._l$(dll)=-L$(DLIB) $(libs)
#endif

#ifdef LDEFOPT
#ifnb $(def)
._f$(dll)=$(LDEFOPT)$(def)$f
#endif
#endif

$Olib$(dll)$d: $(objs)
    (cd $(SYSDIR) && $(SYSOBJCLINK) $(OBJCLFLAGS) $(LOPTS) $(LDLL) $(._f${dll}) -o lib$(dll)$d $(objs) $(._l${dll}) $(syslibs) $(LDLLLIBS))
    $#MKDIR{dir=$(DLIB)$(subdir)}
    $#LNFILE{from=$Olib$(dll)$d, to=$(DLIB)$(subdir)$/lib$(dll)$d}
    $#IF_EQUAL{var1=$(RDM_TFS), var2=$(subdir), do=$#LNFILE{from=$Olib$(dll)$d, to=$(DLIB)lib$(dll)$d}}

#edef
#endif 


## --------------------------------------------------------------------------
## How to make a .jar (Java Archive):


_JAVAC="$(JAVA_HOME)$/bin$/javac"
_JAR="$(JAVA_HOME)$/bin$/jar"

#define MKJAR{jar,classes,path,injars}
#let jar $(jar)
#let ._l$(jar) .
#for injar in $(injars)

#let ._l$(jar) ${._l$(jar)}:$(injar)

#efor

#for class in $(classes)

#let ._o$(jar) ${._o$(jar)} $O$(path)$/$(class).class
#let ._p$(jar) ${._p$(jar)} -C $O $(path)$/$(class).class

$O$(path)$/$(class).class: $(path)$/$(class).java
    $(_JAVAC) -Xlint -classpath ${._l$(jar)} -sourcepath . -d $O $(path)$/$(class).java

#efor

$O$(jar).jar: ${._o$(jar)} $(injars)
    $(_JAR) cf $O$(jar).jar ${._p$(jar)}
    $#CPFILES{from=$O$(jar).jar, to=$(DLIB)}

#edef

## ---------------------------------------------------------------------------
## What constitues a dynamic link library
##

#define LIBRARY{lib}
    $Olib$(lib)$d
#edef


