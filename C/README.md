This folder contains the source code for the KICKS game. All the files are written in C
and use the KICK C compiler proclib. 

* **DEATHPGM.c** Displays the screen when a player dies, transaction ID `DEAT`
* **TITLEPGM.c** Displays the title screen which requires a player to enter a name
* **DUNGEON** A folder containing the game logic in seperate C programs
* **build_one.sh** A shell script which generates the JCL to compile and link the C programs in this folder. This will also enable debug loggin on the MVS console. 

To build **DEATHPGM.c** and **TITLEPGM.c** you can the script `build_one.sh` as follows: `./build_one.sh DEATHPGM.c|ncat localhost 3505`