#include $(RDM_HOME)$/sysmake$/$(KMAKESYS).mak

MODULE = httpdocs


#include $(RDM_HOME)$/kheader.mak

all: Directories Files

Directories: ddir
$#MKDIR{dir=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs}
$#MKDIR{dir=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}

Files:
$#CPFILES{from=$(RDM_HOME)$/httpdocs$/admin$/question.png, to=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}
$#CPFILES{from=$(RDM_HOME)$/httpdocs$/admin$/plus12.png, to=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}
$#CPFILES{from=$(RDM_HOME)$/httpdocs$/admin$/x12r.png, to=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}
$#CPFILES{from=$(RDM_HOME)$/httpdocs$/admin$/down_arrow.png, to=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}
$#CPFILES{from=$(RDM_HOME)$/httpdocs$/admin$/right_arrow.png, to=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}
$#CPFILES{from=$(RDM_HOME)$/httpdocs$/admin$/raima.png, to=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}
$#CPFILES{from=$(RDM_HOME)$/httpdocs$/admin$/admin.js, to=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}
$#CPFILES{from=$(RDM_HOME)$/httpdocs$/admin$/rdm.js, to=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}
$#CPFILES{from=$(RDM_HOME)$/httpdocs$/admin$/enc.js, to=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}
$#CPFILES{from=$(RDM_HOME)$/httpdocs$/admin$/rdm.css, to=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}
$#CPFILES{from=$(RDM_HOME)$/httpdocs$/admin$/index.html, to=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}
$#CPFILES{from=$(RDM_HOME)$/httpdocs$/admin$/raima_icon.ico, to=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin}

local_clean:
$#DELFILE{file=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin$/question.png}
$#DELFILE{file=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin$/plus12.png}
$#DELFILE{file=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin$/x12r.png}
$#DELFILE{file=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin$/down_arrow.png}
$#DELFILE{file=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin$/right_arrow.png}
$#DELFILE{file=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin$/raima.png}
$#DELFILE{file=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin$/admin.js}
$#DELFILE{file=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin$/rdm.js}
$#DELFILE{file=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin$/enc.js}
$#DELFILE{file=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin$/rdm.css}
$#DELFILE{file=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin$/index.html}
$#DELFILE{file=$(RDM_HOME)$/databases.$(SYSDIR)$/httpdocs$/admin$/raima_icon.ico}

cleanall:
    $#CLEAN_DIR{dir=$(DOCROOT)$/$(name)}

$#KEND{}

