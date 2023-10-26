# This script will build one C file on MVS3.8j KICKS
# syntax: bash build_one.sh <program.c> | ncat localhost 3505

# This script is used for compiling the C code for the title and
# death screens only and was made for development purposes. 

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
//JOBPROC  DD DSN=$KICKSHLQ.KICKSSYS.V1R5M0.PROCLIB,DISP=SHR 
END

m=${1%.*}
progname=${m##*/}

cat << END   
//* Compiling $progname                                         
//$progname EXEC PROC=KIKGCCCL,
//          LBOUTC='*',
//          GCCPREF='$GCCPREF',
//          PDPPREF='$PDPPREF',
//          COPTS='-DDEBUG'
//COPY.SYSUT1 DD DATA,DLM=@@
END

cat $1

cat << END

@@
//LKED.SYSIN DD *
 INCLUDE SKIKLOAD(KIKCOBGL)
 ENTRY @@KSTRT
 NAME  $progname(R)
/*
END