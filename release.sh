#!/bin/bash

# Generates the JCL needed to build CICS Dungeon for
# KICKS on MVS/CE https://hub.docker.com/r/mainframed767/kicks

# License GPL

# This is built for MVS/CE KICKS docker container
# if trying to build on TK4-/TK5 make sure you update
# the HLQs below


if [ -z "$1" ]
  then
    KICKSHLQ='KICKS'
fi
if [ -z "$2" ]
  then
    LOADHLQ='SYSC.LINKLIB'
fi
if [ -z "$3" ]
  then
    GCCPREF='GCC'
fi
if [ -z "$4" ]
  then
    PDPPREF='PDPCLIB'
fi






cat << END
//DNGN JOB (JOB),
//             'Build CICS Dungeon',
//             CLASS=A,
//             MSGCLASS=A,
//             MSGLEVEL=(1,1),
//             USER=IBMUSER,
//             PASSWORD=SYS1,
//             REGION=7000K,
//             NOTIFY=IBMUSER
//*
//* This JCL was built for use with the MVS/CE KICKS docker
//* container. If you're using this with a default KICKS
//* install make sure you update the following items
//* 
//* The KICKS HLQ is currently: $KICKSHLQ
//* The LOADLIB with PDSLOAD: $LOADHLQ
//* The HLQ for the GCC compiler: $GCCPREF
//* The PDP C Lib location: $PDPPREF
//*
//* Container: https://hub.docker.com/r/mainframed767/kicks
//*
//JOBPROC  DD DSN=$KICKSHLQ.KICKSSYS.V1R5M0.PROCLIB,DISP=SHR 
END

for i in MAPS/*.bms; do
    m=${i%.*}
    mapname=${m##*/}

cat << END
//* Adding $mapname
//$mapname EXEC PGM=IEFBR14  
//$mapname EXEC KIKMAPS,MAPNAME=$mapname
//COPY.SYSUT1 DD DATA,DLM=@@
END
    cat "$i"
    echo ""
    echo "@@"
done

for i in C/*.c; do

    m=${i%.*}
    progname=${m##*/}

cat << END   
//* Compiling $progname 
//* To show what step we're on
//$progname EXEC PGM=IEFBR14                                        
//*
//* Compiling $progname                                         
//$progname EXEC PROC=KIKGCCCL,
//          LBOUTC='*',
//          GCCPREF='$GCCPREF',
//          PDPPREF='$PDPPREF'
//COPY.SYSUT1 DD DATA,DLM=@@
END

cat $i

cat << END

@@
//LKED.SYSIN DD *
 INCLUDE SKIKLOAD(KIKCOBGL)
 ENTRY @@KSTRT
 NAME  $progname(R)
/*
END
done

# Add BMS tables

for i in TABLES/*; do
  if [[ "$i" == 'TABLES/README.md' ]]; then
    continue
  fi  
    m=${i%.*}
    tablename=${m##*/}

cat << END
//$tablename EXEC PGM=IFOX00,
//            PARM='DECK,NOLIST'
//SYSLIB   DD DSN=SYS1.MACLIB,DISP=SHR
//         DD DSN=$KICKSHLQ.KICKSSYS.V1R5M0.MACLIB,DISP=SHR
//SYSUT1   DD UNIT=SYSDA,SPACE=(CYL,(2,1))
//SYSUT2   DD UNIT=SYSDA,SPACE=(CYL,(2,1))
//SYSUT3   DD UNIT=SYSDA,SPACE=(CYL,(2,1))
//SYSPRINT DD SYSOUT=*
//SYSLIN   DD DUMMY
//SYSPUNCH DD DSN=&&OBJSET,
//         UNIT=SYSDA,SPACE=(80,(200,200)),
//         DISP=(,PASS)
//SYSIN    DD DATA,DLM=@@
END
    cat "$i"
    echo ""
    echo "@@"

cat << END
//LKED     EXEC PGM=IEWL,PARM='XREF,MAP,LET,NCAL',
//         COND=(0,NE,$tablename)
//SYSLIN   DD DSN=&&OBJSET,DISP=(OLD,DELETE)
//SYSIN    DD DUMMY
//SYSLMOD  DD DSN=$KICKSHLQ.KICKSSYS.V1R5M0.SKIKLOAD($tablename),DISP=SHR
//SYSUT1   DD UNIT=SYSDA,SPACE=(CYL,(2,1))
//SYSPRINT DD SYSOUT=*
END
done

# Add .h files
cat << END
//ROGUEH  EXEC PGM=PDSLOAD
//STEPLIB  DD  DSN=$LOADHLQ,DISP=SHR
//SYSPRINT DD  SYSOUT=*
//SYSUT2   DD  DISP=SHR,DSN=$KICKSHLQ.KICKS.V1R5M0.GCCCOPY
//SYSUT1   DD DATA,DLM=@@
./ ADD NAME=ROGUE
END

cat C/DUNGEON/rogue.h
echo "./ ADD NAME=ITEMS"
cat C/DUNGEON/items.h
echo ""
echo "@@"

cat << END   
//* Compiling DUNGEON                                         
//DUNGEON   EXEC PROC=KIKGCCCL,
//          LBOUTC='*',
//          GCCPREF='$GCCPREF',
//          PDPPREF='$PDPPREF'
//COPY.SYSUT1 DD DATA,DLM=@@
END

cat C/DUNGEON/rogue.c
cat C/DUNGEON/3270.c
cat C/DUNGEON/screen.c
cat C/DUNGEON/map.c
cat C/DUNGEON/init.c
cat C/DUNGEON/player.c
cat C/DUNGEON/monsters.c
cat C/DUNGEON/combat.c
cat C/DUNGEON/pathfinding.c
cat C/DUNGEON/items.c

cat << END

@@
//LKED.SYSIN DD *
 INCLUDE SKIKLOAD(KIKCOBGL)
 ENTRY @@KSTRT
 NAME  DUNGEON(R)
/*
END


cat << END
//CLIST  EXEC PGM=PDSLOAD
//STEPLIB  DD  DSN=$LOADHLQ,DISP=SHR
//SYSPRINT DD  SYSOUT=*
//SYSUT2   DD  DISP=SHR,DSN=SYS2.CMDPROC
//SYSUT1   DD DATA,DLM=@@
./ ADD NAME=DUNGEON
END

cat CLIST/DUNGEON
