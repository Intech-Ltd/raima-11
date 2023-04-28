# Auto generated please do not edit this file

#include $(RDM_HOME)$/sysmake$/$(KMAKESYS).mak

CC_DEFS = $DNEED_SETUP_DATABASE 
CC_INCL = $I../../include 

#include $(RDM_HOME)$/kheader.mak

all tutorials:
    krun CircularTable          kmake -rf kmake.mak
    krun CompoundKey          kmake -rf kmake.mak
    krun DisplayTFS_Users          kmake -rf kmake.mak
    krun HelloWorld          kmake -rf kmake.mak
    krun HelloWorldRSQL          kmake -rf kmake.mak
    krun HelloWorldODBC          kmake -rf kmake.mak
    krun HybridInMem          kmake -rf kmake.mak
    krun InMem          kmake -rf kmake.mak
    krun InitializeTFS_RPC          kmake -rf kmake.mak
    krun InitializeTFS_STANDALONE          kmake -rf kmake.mak
    krun InitializeTFS_TFS          kmake -rf kmake.mak
    krun MultiUser          kmake -rf kmake.mak
    krun OneToMany          kmake -rf kmake.mak
    krun OptionalKey          kmake -rf kmake.mak
    krun Transaction          kmake -rf kmake.mak
    krun UniqueKey          kmake -rf kmake.mak
    krun DuplicateKey          kmake -rf kmake.mak

embedfiles test clean cleanall clean_all:
    krun CircularTable          kmake -rf kmake.mak $@
    krun CompoundKey          kmake -rf kmake.mak $@
    krun DisplayTFS_Users          kmake -rf kmake.mak $@
    krun HelloWorld          kmake -rf kmake.mak $@
    krun HelloWorldRSQL          kmake -rf kmake.mak $@
    krun HelloWorldODBC          kmake -rf kmake.mak $@
    krun HybridInMem          kmake -rf kmake.mak $@
    krun InMem          kmake -rf kmake.mak $@
    krun InitializeTFS_RPC          kmake -rf kmake.mak $@
    krun InitializeTFS_STANDALONE          kmake -rf kmake.mak $@
    krun InitializeTFS_TFS          kmake -rf kmake.mak $@
    krun MultiUser          kmake -rf kmake.mak $@
    krun OneToMany          kmake -rf kmake.mak $@
    krun OptionalKey          kmake -rf kmake.mak $@
    krun Transaction          kmake -rf kmake.mak $@
    krun UniqueKey          kmake -rf kmake.mak $@
    krun DuplicateKey          kmake -rf kmake.mak $@

