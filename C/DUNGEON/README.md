This folder contains the C files which makeup the screens, level, game logic for CICS Dungeon. 

Though the files are seperate C files, when building from source they are all combined in to one C file
and submmited using JCL. 

The script `build.sh` facilitates compiling and linking all the C files in this folder. It also enables debug logging which logs to the MVS console. 

To build the C code in this folder you can use the following command: `./build.sh | ncat localhost 3505`.