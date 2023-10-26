# This script will build one C file on MVS3.8j KICKS
# syntax: bash build_one.sh <program.c> | ncat localhost 3505

# This script is used for compiling the C code for dungeon only
# This was made for development purposes. 

# License GPL

# This is built for MVS/CE KICKS docker container
# if trying to build on TK4-/TK5 make sure you update
# the HLQs below

KICKSHLQ='KICKS'
LOADHLQ='SYSC.LINKLIB'
GCCPREF='GCC'
PDPPREF='PDPCLIB'


cat << END
//COMPGCC JOB (JOB),
//             'COMPILE COBOL',
//             CLASS=A,
//             MSGCLASS=A,
//             MSGLEVEL=(1,1),
//             USER=IBMUSER,
//             PASSWORD=SYS1,
//             REGION=7000K,
//             NOTIFY=IBMUSER
//* KICKS PROCLIB
//JOBPROC  DD DSN=$KICKSHLQ.KICKSSYS.V1R5M0.PROCLIB,DISP=SHR 
END

cat << END
//ROGUEH  EXEC PGM=PDSLOAD
//STEPLIB  DD  DSN=$LOADHLQ,DISP=SHR
//SYSPRINT DD  SYSOUT=*
//SYSUT2   DD  DISP=SHR,DSN=$KICKSHLQ.KICKS.V1R5M0.GCCCOPY
//SYSUT1   DD DATA,DLM=@@
./ ADD NAME=ROGUE
END

cat rogue.h

echo "./ ADD NAME=ITEMS"
cat items.h

echo "@@"

# Change GCCPREF and PDPPREF here

cat << END   
//* Compiling DUNGEON                                         
//DUNGEON   EXEC PROC=KIKGCCCL,
//          LBOUTC='*',
//          GCCPREF='$GCCPREF',
//          PDPPREF='$PDPPREF',
//          COPTS='-DDEBUG'
//COPY.SYSUT1 DD DATA,DLM=@@
END

cat rogue.c
cat 3270.c
cat screen.c
cat map.c
cat init.c
cat player.c
cat monsters.c
cat combat.c
cat pathfinding.c
cat items.c

cat << END

@@
//LKED.SYSIN DD *
 INCLUDE SKIKLOAD(KIKCOBGL)
 ENTRY @@KSTRT
 NAME  DUNGEON(R)
/*
END