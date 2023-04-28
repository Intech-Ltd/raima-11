# Raima Database Manager
#
# Copyright (c) 2010 Raima Inc.,  All rights reserved.
#
# Use of this software, whether in source code format, or in executable,
# binary object code form, is governed by the Raima LICENSE which
# is fully described in the LICENSE.TXT file, included within this
# distribution of files.

# x86 Linux 32-bit version.
# This file contains platform-dependent definitions of things such as file
# name extensions, and macros for building such things as executables and
# shared libraries.

OS    = LINUX_X64
LINUX = 1

OS_PSP     = pspunix$o pspuxthreads$o

OS_PSP_SYSLIBS = -lpthread -lrt
OS_TERMCAP     = -ltermcap

## ---------------------------------------------------------------------------
## File extensions
##

o = .o
l = .a
d = .so
e =

## ---------------------------------------------------------------------------
## File extensions
##

#ifdef CC
GCC     = $(CC)
#else
GCC     = gcc
#endif
#ifdef CXX 
GPP     = $(CXX)
#else
GPP     = g++
#endif

SYSCC       = $(GCC)
SYSCPP      = $(GPP)
SYSOBJC     = $(GPP)
SYSLINK     = $(GCC)
SYSCPPLINK  = $(GPP)
SYSOBJCLINK = $(GPP)

## ---------------------------------------------------------------------------
## C Compiler command line arguments
##

CFLAGS    = -m64 -std=c99 -D_GNU_SOURCE -D_REENTRANT -fPIC -Wall
CCDEST    = -o$@
CCONLY    = -c
CCSYMBOLS = -g
CCNOOPT   =
CCOPT     = -O
CCDLL     = 

## ---------------------------------------------------------------------------
## C++ Compiler command line arguments
##

CPPFLAGS  = -m64 -D_GNU_SOURCE -D_REENTRANT -fPIC -Wall -frtti -fexceptions

## ---------------------------------------------------------------------------
## Linker command line arguments
##

LFLAGS    = -m64 -fPIC
LDEBUG    = -g
LDLL      = -shared
CPPLDLL   = -shared

## ---------------------------------------------------------------------------
## RDM Server defines
##

BITSUFFIX  = 64
RDS_SYSDIR = lnxamd64t
JDKOS      = linux

#include $(RDM_HOME)$/sysmake$/k_unix.mak
#include $(RDM_HOME)$/sysmake$/k_unix_host.mak

