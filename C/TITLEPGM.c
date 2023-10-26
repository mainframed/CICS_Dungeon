/////////////////////////////////////////////////////////////////
// CICS Dungeon Title Screen
//
// Transaction ID: DNGN
//
// TESTMAP:
//   Displays the CICS EBCDIC art for the title screen and 
//   requires the player to input a name. Then launches the
//   RGUE transaction. 
//
// AUTHOR:
//   Philip Young aka Soldier of FORTRAN
//
// 10/08/2022
// License GPL v3
/////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Required for CICS
#include "dfhaid.h"
#include "title.h"

#define WTO_MESSAGE     "DNGN - %s"
#define NFO_MESSAGE     "DNGN - TRN: %.4s UserID: %.8s TermID: %.4s"

char wtomsg[126];               // WTO message buffer

void write_op() {
    int wtolen = sizeof(wtomsg);
#ifdef DEBUG
    EXEC CICS WRITE OPERATOR
     TEXT(wtomsg) TEXTLENGTH(wtolen);
#endif
    memset(wtomsg,'\0',126); 
}

int main(DFHEIB *eib) {
  int comm = 1;
  
  sprintf(wtomsg,"=====TITLE=====================================");
  write_op();

  if (eib->eibaid == DFHPF3 || eib->eibaid == DFHCLEAR)
  {
    sprintf (wtomsg, WTO_MESSAGE, "DNGN - F3 or Clear - EXIT");
    write_op();
    EXEC CICS SEND CONTROL ERASE FREEKB;
    EXEC CICS RETURN END-EXEC;
  }
  
  sprintf(wtomsg,"DNGN - IEBCALEN = %d",eib->eibcalen);
  write_op();

  if (eib->eibcalen == 0) {

    sprintf (wtomsg, WTO_MESSAGE, "Welcome to CICS Dungeon");
    write_op();

    sprintf (wtomsg, NFO_MESSAGE, eib->eibtrnid, \
              eib->eibusrid, eib->eibtrmid);
    write_op();

    sprintf (wtomsg, WTO_MESSAGE, "Sending Map TITLE/TITLE1");
    write_op();
    EXEC CICS 
      SEND MAP("title1") 
      MAPSET("title") 
      ERASE;

  } else if (eib->eibcalen > 0 && eib->eibaid == DFHENTER) {

    sprintf(wtomsg,"DNGN - User hit enter, getting player name");
    write_op();

    EXEC CICS
      RECEIVE MAP("title1") MAPSET("title")
              INTO(title1.title1i)
                  ASIS ;
    
      sprintf(wtomsg,"DNGN - Player Name: (%d) %s",
              strlen(title1.title1i.namei),title1.title1i.namei);
      write_op();
    
    if (strlen(title1.title1i.namei) == 0) {    
      
      memset(title1.title1o.erroro,0,30);
      memcpy(title1.title1o.erroro,
            "You must enter a name!!      ",29);
            
      sprintf(wtomsg,"DNGN - Player didn't enter a name");
      write_op();

      EXEC CICS 
        SEND MAP("title1") 
        MAPSET("title") 
        FROM(title1.title1o)
        ERASE;

    } else {
      // Player hit enter and put in a name
      sprintf(wtomsg,"DNGN - Player Name: (%d) %s",
              strlen(title1.title1i.namei),title1.title1i.namei);
      write_op();

      // for(comm = 0;comm <= strlen(title1.title1i.namei); ++comm) {

      // sprintf(wtomsg,"DNGN - Player char: %d (%d) - 0x%.02x",
      //         strlen(title1.title1i.namei),comm,title1.title1i.namei[comm]);
      // write_op();
      // }
      
      sprintf(wtomsg,"DNGN - Leaving TITLE loading DUNGEON");
      write_op();
      EXEC CICS 
          XCTL PROGRAM("DUNGEON")
          COMMAREA(title1.title1i.namei)
          LENGTH(strlen(title1.title1i.namei));
    }
  }
  
        
  sprintf (wtomsg, WTO_MESSAGE, " END - CICS Return");
  write_op();
  
  sprintf (wtomsg, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
  write_op();
  EXEC CICS
      RETURN 
        TRANSID("DNGN")
        COMMAREA(comm);

  return 0;
}

