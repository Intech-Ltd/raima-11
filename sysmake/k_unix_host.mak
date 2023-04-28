
## ---------------------------------------------------------------------------
## How to give executable permission
##

#define SETEXECPERM{file}
    -@chmod 750 $(file)
#edef

## ---------------------------------------------------------------------------
## How to make a subdirectory
##

#define MKDIR{dir}
    -@if [ ! -d $(dir) ]; then mkdir $(dir); else exit 0; fi
#edef


## ---------------------------------------------------------------------------
## How to delete a file
##

#define DELFILE{file}
    @rm -f $(file); exit 0
#edef

## ---------------------------------------------------------------------------
## How to delete a list of files
##

#define DELFILES{files}
    @rm -f $(files); exit 0
#edef

## ---------------------------------------------------------------------------
## How to clean the output subdirectory
##

#define CLEAN_DIR{dir}
    @rm -rf $(dir); exit 0
#edef

## ---------------------------------------------------------------------------
## How to copy files
##

#define CPFILES{from,to}
    cp $(from) $(to); exit 0
#edef

## ---------------------------------------------------------------------------
## How to link files
##

#define LNFILE{from,to}
    ln -f $(from) $(to); exit 0
#edef

## ---------------------------------------------------------------------------
## How to set up a conditional action
##

#define IF_EQUAL{var1, var2, do}
    @if [ "$(var1)" = "$(var2)" ]; then $(do); else exit 0; fi
#edef

## ---------------------------------------------------------------------------
## How to set up a conditional action
##

#define IF_EXIST{file, do}
    @if [ -f $(file) ]; then $(do); else exit 0; fi
#edef

