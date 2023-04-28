# Raima Database Manager
#
# Copyright (c) 2010 Raima Inc.,  All rights reserved.
#
# Use of this software, whether in source code format, or in executable,
# binary object code form, is governed by the Raima LICENSE which
# is fully described in the LICENSE.TXT file, included within this
# distribution of files.

# From a bourne compatible shell (such as sh, bash or ksh)
# please source this code by using ". ./rdm.sh"
# Needed environment variables for compilation of this product
# will then be set.  Environment variables needed for providing
# access to the compiler tool chains is not provided.
#
# This script replaces rdme.sh and rdme.csh.  C shell is no longer
# supported.

VCS_BRANCH=CastleRock
RDM_MAJOR_VER=11
PSP_MAJOR_VER=11
export VCS_BRANCH RDM_MAJOR_VER PSP_MAJOR_VER

if [ "${RDME_DEBUG_BUILD}" = "1" ]; then
    DEBUG=1
    export DEBUG
fi

RDM_HOME=`pwd`

hastput=1
called_usage=0
bitsize=0
bitsize_specified=0
use_LD_LIBRARY_PATH=1
bold=bold
normal=sgr0
unset HOSTDIR
unset ENV_PREFIX


rdm_usage () {
    echo ""
    echo "Usage:  . ./rdm.sh [Target] [Options]"
    echo ""
    echo "Set up environment for properly building & running the software."
    echo ""
    echo "      [Target]:      Specify Target Platform"
#    echo "                      On Current Host
    echo "                        -32      Specify 32bit arch"
    echo "                        -64      Specify 64bit arch"
    echo "                      Cross Compile"
    echo "                        -vx6sh4     VxWorks 6.x for SuperH SH4"
    echo "                        -vx6x86     VxWorks 6.x for Intel x86"
    echo "                        -vx6ppc     VxWorks 6.x for PowerPC"
    echo ""
    echo "                        -neux86     QNX Neutrino 6.5 for x86"
    echo "                        -neux86     QNX Neutrino 6.5 for ARM"
    echo "                        -neuppc     QNX Neutrino 6.5 for PowerPC"
    echo ""
    echo "                        -intx86     Green Hills Integrity 5.0.x for x86"
    echo "                        -intppc     Green Hills Integrity 5.0.x for PowerPC"
#    echo ""
#    echo "                        -ios        Apple iOS (All devices)"
#    echo "                        -ios_sim    Apple iOS (Simulator)"
    echo ""
    echo "                        -lnxarm     Linux on ARM (armv7 instruction set)"
#    echo ""
#    echo "                        -android    Android OS"
#    echo ""
#    echo "                        -lynx5      LynxOS 5"
    echo ""                              
    echo ""              
    echo "      [Options]:     Specify Build Options"
    echo "                        -gnutools   Use GNU toolchain"
    echo ""
    echo "      [-h | --help]: Help"
    echo ""
    called_usage=1
}

setup_32() {
    if [ $bitsize -eq 32 -o $bitsize -eq 64 ]; then
        echo "Cannot specify the bit size multiple times: '-32'"
        rdm_usage
    fi

    using64=0
    bitsuf=
    bitsize=32
}

setup_64() {
    if [ $bitsize -eq 32 -o $bitsize -eq 64 ]; then
        echo "Cannot specify the bit size multiple times: '-64'"
        rdm_usage
    fi

    using64=1
    bitsuf=64
    bitsize=64
}

unset USE_GNUTOOLS
unset gnutools
vx6sh4=0
vx6x86=0
vx6ppc=0
intx86=0
intppc=0
lynx5=0
ios=0
ios_sim=0
lnxarm=0
android=0
neux86=0
neuarm=0
neuppc=0
if [ ! -f ./rdm.sh ]; then
    echo ". ./rdm.sh: Please source this file from the directory where rdm.sh is located"
    rdm_usage
else
    for p in "$@";
    do
        if [ x$p = x'-32' ]; then
            bitsize_specified=1
            setup_32
        elif [ x$p = x'-64' ]; then
            bitsize_specified=1
            setup_64
        elif [ x$p = x'-gnutools' ]; then
            export USE_GNUTOOLS=1
            gnutools=" using gnutools"
        elif [ x$p = x'-vx6sh4' ]; then
            vx6sh4=1
        elif [ x$p = x'-vx6x86' ]; then
            vx6x86=1
        elif [ x$p = x'-vx6ppc' ]; then
            vx6ppc=1
        elif [ x$p = x'-intx86' ]; then
            intx86=1
        elif [ x$p = x'-intppc' ]; then
            intppc=1
        elif [ x$p = x'-lynx5' ]; then
            lynx5=1
        elif [ x$p = x'-ios' ]; then
            ios=1
        elif [ x$p = x'-ios_sim' ]; then
            ios_sim=1
        elif [ x$p = x'-lnxarm' ]; then
            lnxarm=1
        elif [ x$p = x'-android' ]; then
            android=1
        elif [ x$p = x'-neux86' ]; then
            neux86=1
        elif [ x$p = x'-neuarm' ]; then
            neuarm=1
        elif [ x$p = x'-neuppc' ]; then
            neuppc=1
        elif [ x$p = x'-h' -o x$p = x'--help' ]; then
            rdm_usage
        else
            echo "Unknown option '$p'"
            rdm_usage
        fi
    done

    unset p

    if [ $DO64 ]; then
        setup_64
    fi

    if [ $bitsize -eq 0 ]; then
        setup_32
    fi

    if [ $called_usage -eq 0 ]; then
        # Platform Dependencies -------------------------------------
        case `uname` in
            SunOS)
                osname="Solaris"
                if [ `uname -p` = 'i386' ]; then
                    if [ $using64 -eq 1 ]; then
                        arch='Intel x64'
                        KMAKESYS=k_sol_x64_64
                    else
                        arch='Intel x86'
                        KMAKESYS=k_sol_x86_32
                    fi
                    SYSDIR=isol$bitsuf
                    localpath=/eng/local/isolaris/bin
                else
                    if [ $vx6sh4 -eq 1 ]; then
                        arch='VxWorks 6.4'
                        KMAKESYS=k_vx6_sh4_32
                        SYSDIR=vx6sh4$bitsuf
                        HOSTDIR=sol$bitsuf
                        UNIX_HOST=1
                        export UNIX_HOST
                        export HOSTDIR
                    elif [ $vx6x86 -eq 1 ]; then
                        arch='VxWorks 6.4'
                        KMAKESYS=k_vx6_x86_32
                        SYSDIR=vx6x86$bitsuf
                        HOSTDIR=sol$bitsuf
                        UNIX_HOST=1
                        export UNIX_HOST
                        export HOSTDIR
                    else
                        arch='Sparc'
                        KMAKESYS=k_sol_sparc_$bitsize
                        SYSDIR=sol$bitsuf
                    fi
                    localpath=/eng/local/solaris/bin
                fi
                ;;
            FreeBSD)
                osname="FreeBSD"
                machine=`uname -m`
                if [ $machine = 'amd64' -a $bitsize_specified -eq 0 ]; then
                    # The default assumed on a 64-bit FreeBSD platform is
                    # the 64-bit version.
                    bitsize=0
                    setup_64
                fi

                rdm_freebsd_major=`uname -r | sed -n 's:^\([0-9]*\)\..*:\1:p'`
                if [ $rdm_freebsd_major = '7' ]; then
                    localpathsuffix=7
                else
                    localpathsuffix=
                fi

                if [ $using64 -eq 1 ]; then
                    arch='Intel x64'
                    bitsuf=64
                    bitsize=64
                    KMAKESYS=k_fbsd_x64_64
                    localpath=/eng/local/fbsd$localpathsuffix\64/bin
                else
                    arch='Intel x86'
                    bitsuf=
                    bitsize=32
                    KMAKESYS=k_fbsd_x86_32
                    localpath=/eng/local/fbsd$localpathsuffix/bin
                fi
                SYSDIR=fbsd$bitsuf
                unset rdm_freebsd_major
                unset localpathsuffix
                ;;
            NetBSD)
                osname="NetBSD"
                machine=`uname -m`
                if [ $machine = 'amd64' -a $bitsize_specified -eq 0 ]; then
                    # The default assumed on a 64-bit FreeBSD platform is
                    # the 64-bit version.
                    bitsize=0
                    setup_64
                fi

                if [ $using64 -eq 1 ]; then
                    arch='Intel x64'
                    bitsuf=64
                    bitsize=64
                    KMAKESYS=k_nbsd_x64_64
                    localpath=/raima/local/netbsd-64/bin
                else
                    arch='Intel x86'
                    bitsuf=
                    bitsize=32
                    KMAKESYS=k_nbsd_x86_32
                    localpath=/raima/local/netbsd/bin
                fi
                SYSDIR=nbsd$bitsuf
                ;;
            OpenBSD)
                osname="OpenBSD"
                machine=`uname -m`
                if [ $machine = 'amd64' -a $bitsize_specified -eq 0 ]; then
                    # The default assumed on a 64-bit FreeBSD platform is
                    # the 64-bit version.
                    bitsize=0
                    setup_64
                fi

                if [ $using64 -eq 1 ]; then
                    arch='Intel x64'
                    bitsuf=64
                    bitsize=64
                    KMAKESYS=k_obsd_x64_64
                    localpath=/raima/local/openbsd-64/bin
                else
                    arch='Intel x86'
                    bitsuf=
                    bitsize=32
                    KMAKESYS=k_obsd_x86_32
                    localpath=/raima/local/openbsd/bin
                fi
                SYSDIR=obsd$bitsuf
                ;;
            Linux)
                osname="Linux"
                machine=`uname -m`
                unset MAKEFLAGS
                if [ $machine = 'ia64' ]; then
                    arch='Itanium'
                    bitsuf=ia64
                    bitsize=64

                    KMAKESYS=k_lnx_ia_64
                    SYSDIR=lnx$bitsuf
                else
                    if [ $vx6sh4 -eq 1 ]; then
                        arch='VxWorks 6.4'
                        KMAKESYS=k_vx6_sh4_32
                        SYSDIR=vx6sh4$bitsuf
                        HOSTDIR=lnx$bitsuf
                        UNIX_HOST=1
                        export UNIX_HOST
                        export HOSTDIR
                    elif [ $lnxarm -eq 1 ]; then
                        arch='ARM'
                        KMAKESYS=k_lnx_arm_32
                        SYSDIR=lnxarm
                        HOSTDIR=lnx$bitsuf
                        UNIX_HOST=1
                        export UNIX_HOST
                        export HOSTDIR
                    elif [ $vx6x86 -eq 1 ]; then
                        arch='VxWorks 6.4'
                        KMAKESYS=k_vx6_x86_32
                        SYSDIR=vx6x86$bitsuf
                        HOSTDIR=lnx$bitsuf
                        UNIX_HOST=1
                        export UNIX_HOST
                        export HOSTDIR
                    elif [ $vx6ppc -eq 1 ]; then
                        arch='VxWorks 6.4'
                        KMAKESYS=k_vx6_ppc_32
                        SYSDIR=vx6ppc$bitsuf
                        HOSTDIR=lnx$bitsuf
                        UNIX_HOST=1
                        export UNIX_HOST
                        export HOSTDIR
                    elif [ $intx86 -eq 1 ]; then
                        arch='Integrity 5.0.x'
                        KMAKESYS=k_int_x86_32
                        SYSDIR=intx86$bitsuf
                        HOSTDIR=lnx$bitsuf
                        UNIX_HOST=1
                        export UNIX_HOST
                        export HOSTDIR
                    elif [ $intppc -eq 1 ]; then
                        arch='Integrity 5.0.x'
                        KMAKESYS=k_int_ppc_32
                        SYSDIR=intppc$bitsuf
                        HOSTDIR=lnx$bitsuf
                        UNIX_HOST=1
                        export UNIX_HOST
                        export HOSTDIR
                    elif [ $neux86 -eq 1 ]; then
                        arch='Neutrino 6.5 x86'
                        KMAKESYS=k_neu_x86_32
                        SYSDIR=neux86$bitsuf
                        HOSTDIR=lnx$bitsuf
                        UNIX_HOST=1
                        unset MAKEFLAGS
                        export UNIX_HOST
                        export HOSTDIR
                    elif [ $neuarm -eq 1 ]; then
                        arch='Neutrino 6.5 ARM'
                        KMAKESYS=k_neu_arm_32
                        SYSDIR=neuarm$bitsuf
                        HOSTDIR=lnx$bitsuf
                        UNIX_HOST=1
                        unset MAKEFLAGS
                        export UNIX_HOST
                        export HOSTDIR
                    elif [ $neuppc -eq 1 ]; then
                        arch='Neutrino 6.5 PPC'
                        KMAKESYS=k_neu_ppc_32
                        SYSDIR=neuppc$bitsuf
                        HOSTDIR=lnx$bitsuf
                        UNIX_HOST=1
                        unset MAKEFLAGS
                        export UNIX_HOST
                        export HOSTDIR
                    elif [ $lynx5 -eq 1 ]; then
                        osname="LynxOS-SE 5.0"
                        arch='Intel x86'
                        KMAKESYS=k_lynx_x86_32
                        SYSDIR=lynx$bitsuf
                        HOSTDIR=lnx$bitsuf
                        ENV_PREFIX=/usr/lynxos-se/5.0.0/x86
                        export HOSTDIR ENV_PREFIX
                    elif [ $android -eq 1 ]; then
                        osname="Android 1.6 NDK"
                        arch="ARM"
                        KMAKESYS=k_and_arm_32
                        SYSDIR=and$bitsuf
                        HOSTDIR=lnx$bitsuf
                        ENV_PREFIX=/users/jedgar/android/android-ndk-r5
                        export HOSTDIR ENV_PREFIX
                    else
                        if [ $machine = 'x86_64' -a $bitsize_specified -eq 0 ]; then
                            # The default assumed on a 64-bit Linux platform is
                            # the 64-bit version.
                            bitsize=0
                            setup_64
                        fi

                        if [ $using64 -eq 1 ]; then
                            arch='Intel x64'
                            bitsuf=64
                            bitsize=64
                            KMAKESYS=k_lnx_x64_64
                        else
                            arch='Intel x86'
                            bitsuf=
                            bitsize=32
                            KMAKESYS=k_lnx_x86_32
                        fi
                        SYSDIR=lnx$bitsuf
                    fi
                    localpath=/eng/local/linux$bitsuf/bin
                fi
                bold=smso
                ;;
            HP-UX)
                osname="HP-UX"
                if [ `uname -m` = 'ia64' ]; then
                    arch='Itanium'
                    ARCHDIR=hpia
                    SYSDIR=$ARCHDIR$bitsuf
                    KMAKESYS=k_hp_ia_$bitsize
                else
                    arch='PA-RISC'
                    ARCHDIR=hpux
                    SYSDIR=$ARCHDIR$bitsuf
                    KMAKESYS=k_hp_risc_$bitsize
                fi
                use_LD_LIBRARY_PATH=0
                if [ $SHLIB_PATH ]; then
                    rdm_pathfind=`echo $SHLIB_PATH | grep $RDM_HOME/$SYSDIR/lib`
                    if [ $rdm_pathfind ]; then : ; else
                        SHLIB_PATH="$RDM_HOME/$SYSDIR/lib":$SHLIB_PATH
                    fi
                else
                    SHLIB_PATH="$RDM_HOME/$SYSDIR/lib";
                fi
                export SHLIB_PATH
                localpath=/eng/local/${ARCHDIR}/bin
                ;;
            AIX)
                osname="AIX"
                arch='PowerPC'
                SYSDIR=aix$bitsuf
                KMAKESYS=k_aix_ppc_$bitsize

                use_LD_LIBRARY_PATH=0
                if [ $LIBPATH ]; then
                    rdm_pathfind=`echo $LIBPATH | grep $RDM_HOME/$SYSDIR/lib`
                    if [ $rdm_pathfind ]; then : ; else
                        LIBPATH="$RDM_HOME/$SYSDIR/lib":$LIBPATH
                    fi
                else
                    LIBPATH="$RDM_HOME/$SYSDIR/lib"
                fi
                export LIBPATH
                localpath=/eng/local/aix$bitsuf/bin
                ;;
            QNX)
                osname="QNX Neutrino"
                arch='Intel x86'
                if [ using64 = 1 ]; then
                    echo "Unable to build 64-bit on QNX Neutrino"
                fi
                SYSDIR=neut
                KMAKESYS=k_neut_x86_$bitsize
                localpath=/raima/local/neu/bin
                hastput=0
            ;;
            Darwin)
                if [ $ios = 1 ]; then
                    osname="iOS"
                    arch='ARM'
                    KMAKESYS=k_ios_arm_32
                    SYSDIR=ios$bitsuf
                    HOSTDIR=osx64$bitsuf
                    export HOSTDIR
                elif [ $ios_sim = 1 ]; then
                    osname="iOS"
                    arch='Sim'
                    KMAKESYS=k_ios_sim_32
                    SYSDIR=ios_sim$bitsuf
                    HOSTDIR=osx64$bitsuf
                    export HOSTDIR
                else
                    osname="Mac OSX"
                    if [ "`sysctl hw | grep 64bit`" = 'hw.cpu64bit_capable: 1' ]; then
                       if [ $bitsize_specified -eq 0 ]; then
                          # Default to 64bit
                          bitsize=0
                          setup_64
                       fi
                    fi

                    if [ $using64 -eq 1 ]; then
                       arch='Intel x64'
                       bitsuf=64
                       bitsize=64
                       KMAKESYS=k_mac_x86_64
                    else
                       if [ $STATIC_BUILD -eq 1 ]; then
                          static=_static
                       fi
                       arch='Intel x86'
                       bitsuf=
                       bitsize=32
                       KMAKESYS=k_mac_x86_32$static
                    fi

                    SYSDIR=osx$bitsuf$static
                fi

                use_LD_LIBRARY_PATH=0
                if [ $DYLD_LIBRARY_PATH ]; then
                    DYLD_LIBRARY_PATH="$RDM_HOME/$SYSDIR/lib":$DYLD_LIBRARY_PATH
                else
                    DYLD_LIBRARY_PATH="$RDM_HOME/$SYSDIR/lib"
                fi
                export DYLD_LIBRARY_PATH
                localpath=/eng/local/macx86/bin

                bold=smso
            ;;
            *)
                KMAKESYS=unknown
                ;;
        esac

        if [ $KMAKESYS = unknown ]; then
            echo ". ./rdm.sh: Unknown platform"
            unset KMAKESYS
        else
            if [ $hastput = 1 ]; then
                BOLD=`tput $bold`
                NORMAL=`tput $normal`
            fi

            echo "Setting up environment for $BOLD$osname$NORMAL ($BOLD$arch$NORMAL) ($BOLD$bitsize-bit$NORMAL) into $BOLD$SYSDIR$NORMAL$gnutools"
            unset BOLD
            unset NORMAL
            unset DBDPATH
            unset DBFPATH
            unset RDM_MOBILE
            CLASSPATH=$RDM_HOME:$RDM_HOME/rdm_embedded.jar:$RDM_HOME/test:.:$CLASSPATH

            # Library path for non-AIX/non-HP --------------------------

            if [ $use_LD_LIBRARY_PATH = 1 ]; then
                if [ $LD_LIBRARY_PATH ]; then
                    rdm_pathfind=`echo $LD_LIBRARY_PATH | grep $RDM_HOME/lib/$SYSDIR`
                    if [ $rdm_pathfind ]; then : ; else
                        LD_LIBRARY_PATH="$RDM_HOME/$SYSDIR/lib":$LD_LIBRARY_PATH
                    fi
                else
                    LD_LIBRARY_PATH="$RDM_HOME/$SYSDIR/lib"
                fi
                export LD_LIBRARY_PATH
            fi

            rdm_pathfind=`echo $PATH | grep $RDM_HOME/$SYSDIR/bin`
            if [ $rdm_pathfind ]; then : ; else
                PATH=$RDM_HOME/bin:$RDM_HOME/$SYSDIR/bin:$PATH
            fi

            local_pathfind=`echo $PATH | grep $localpath`
            if [ $local_pathfind ]; then : ; else
                ## DFT: do not place $localpath before $PATH; doing so may
                ##      cause compatibility issues with programs like perl
                PATH=$PATH:$localpath
            fi

            if [ $HOSTDIR ]; then
                LD_LIBRARY_PATH="$RDM_HOME/$HOSTDIR/lib":$LD_LIBRARY_PATH
                if [ $DYLD_LIBRARY_PATH ]; then
                    DYLD_LIBRARY_PATH="$RDM_HOME/$HOSTDIR/lib":$DYLD_LIBRARY_PATH
                fi
                PATH=$RDM_HOME/bin:$RDM_HOME/$HOSTDIR/bin:$PATH
            fi

            # Export environment variables -----------------------------

            export RDM_HOME LD_LIBRARY_PATH PATH KMAKESYS CLASSPATH SYSDIR 

            # Clean up -------------------------------------------------
        fi
    fi
    unset rdm_pathfind
fi

# Aliases --------------------------------------------------
case $SHELL in
*bash) alias ch='source ${RDM_HOME}/bin/rdm_ch.sh' ;;
*) alias ch='cd ${RDM_HOME}' ;;
esac

alias k="kmake -r -f kmake.mak"

alias makedep="perl $RDM_HOME/perl/makedep.pl"
alias prebuild="perl $RDM_HOME/prebuild.pl"


# Clean up -------------------------------------------------
unset hastput
unset called_usage
unset arch
unset using64
unset bitsuf
unset bitsize
unset bitsize_specified
unset osname
unset setup_32
unset setup_64
unset rdm_usage
unset vx6sh4
unset vx6x86
unset vx6ppc
unset intx86
unset intppc
unset neux86
unset neuarm
unset neuppc
unset lynx5
unset ios
unset ios_sim
unset android
unset bold
unset normal
unset localpath
unset local_pathfind
unset use_LD_LIBRARY_PATH

