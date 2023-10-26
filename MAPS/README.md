This folder contains the maps used by the programs in the `C` folder. They were auto generated from the ANSI in the `ANSI` folder. The script `build_map.sh` builds the JCL needed to assemble, link and install the maps. 

You ***must*** build the maps (compile, link) before compiling the code in the `C` folder. 

To build maps you can run the script as follows: `build_map.sh <map file>.bms|ncat localhost 3505`

* **TITLE.bms** contains the EBCDIC art for the title screen
* **DEATH.bms** contains the EBCDIC art for the death screen