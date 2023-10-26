/////////////////////////////////////////////////////////////////
// CICS Dungeon Death Screen
//
// Transaction ID: DEAT
//
// DEATHPGM:
//   Displays the CICS EBCDIC art after deing and displays the
//   the players name, what killed them and the date. After
//   hitting enter it takes the player back to the starting
//   transaction DNGN. 
//
// AUTHOR:
//   Philip Young aka Soldier of FORTRAN
//
// 10/22/2023
// License GPL v3
/////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Required for CICS
#include "dfhaid.h"
#include "death.h"
//#include "kikaid.h"


#define WTO_MESSAGE     "DEAT - %s"
#define NFO_MESSAGE     "DEAT - TRN: %.4s UserID: %.8s TermID: %.4s"
#define SBA_MESSAGE     "DEAT - SBA %d,%d"

char wtomsg[126];               // WTO message buffer

 typedef struct __mycomm__ {
     char name[19];
     char killed_by[19];
 } mycomm;

char *trim(char *string) {
  // Check if the string is null.
  if (string == NULL) {
    return NULL;
  }

  // Initialize the start and end indices of the trimmed string.
  size_t start_index = 0, i;
  size_t end_index = strlen(string);

  // Find the first non-space character.
  while (string[start_index] == ' ') {
    start_index++;
  }

  // Find the last non-space character.
  while (string[end_index] == ' ' || string[end_index] == '\0') {
    end_index--;
  }

  // If the start and end indices are equal, then the string is empty.
  // Return an empty string in this case.
  // if (start_index == end_index) {
  //   return "";
  // }

  // Create a new string to store the trimmed string.
  char *new_string = malloc(end_index - start_index + 2);
  if (new_string == NULL) {
    return NULL;
  }

  // Copy the trimmed string to the new string.
  for ( i = start_index; i <= end_index; i++) {
    new_string[i - start_index] = string[i];
  }

  // Terminate the new string.
  new_string[end_index - start_index + 1] = '\0';

  // Return the new string.

  return new_string;
}

void center_text(char *array, size_t array_length, const char *text) {
  size_t num_spaces, i;
  int array_index;

  if (array == NULL || array_length == 0) {
    return;
  }

  // Check if the text is null.
  if (text == NULL) {
    return;
  }

  // Calculate the number of spaces to add on either side of the text.
  num_spaces = (array_length - strlen(text)) / 2;


  // Initialize the array index.
  array_index = 0;

  // Add the spaces to the beginning of the array.
  for ( i = 0; i < num_spaces; ++i) {
    array[array_index++] = ' ';
  }

  // Add the text to the array.
  for ( i = 0; i < strlen(text); ++i) {
    array[array_index++] = text[i];
  }

  // Add the spaces to the end of the array.
  for ( i = 0; i < num_spaces; ++i) {
    array[array_index++] = ' ';
  }
}

void write_op() {
    int wtolen = sizeof(wtomsg);
#ifdef DEBUG
    EXEC CICS WRITE OPERATOR
     TEXT(wtomsg) TEXTLENGTH(wtolen);
#endif
    memset(wtomsg,'\0',126); 
}


int main(KIKEIB *eib, mycomm *KIKCOMM) {
  char  ws_abstime[8], date[9];
  int dead = 1;

  char *trim_string;
  
  sprintf(wtomsg,"=======DEATH====================================");
  write_op();

  sprintf(wtomsg,"DEAT - EIBAID - 0x%.02x", eib->eibaid);
  write_op();

  if (eib->eibaid == DFHPF5 || eib->eibaid == DFHCLEAR)  {
    sprintf (wtomsg, WTO_MESSAGE, "DEAT - F5 or Clear - EXIT");
    write_op();
    EXEC CICS SEND CONTROL ERASE FREEKB;
    EXEC CICS RETURN END-EXEC;
  } 
  
  sprintf(wtomsg,"DEAT - IEBCALEN = %d",eib->eibcalen);
  write_op();

  if (eib->eibcalen == 0)
    {
    sprintf(wtomsg,"DEAT - No data returning to title");
    write_op();
    EXEC CICS 
      XCTL PROGRAM("TITLEPGM");
    }


  if (eib->eibcalen == 4)
    {
    sprintf(wtomsg,"DEAT - No data returning to title");
    write_op();
    EXEC CICS 
      XCTL PROGRAM("TITLEPGM");
    }

  memset(death1.death1o.namedo,0,sizeof(death1.death1o.namedo));
  memset(death1.death1o.killedo,0,sizeof(death1.death1o.killedo));

  sprintf(wtomsg,"Trimming '%s'",KIKCOMM->name);
  write_op();
  trim_string = trim(KIKCOMM->name);

  sprintf(wtomsg,"Trimmed '%s'",trim_string);
  write_op();

  center_text(death1.death1o.namedo, 
              sizeof(death1.death1o.namedo), 
              trim_string);


  trim_string = trim(KIKCOMM->killed_by);
  sprintf(wtomsg,"Centering '%s'",KIKCOMM->name);
  write_op();

  center_text(death1.death1o.killedo, 
              sizeof(death1.death1o.killedo), 
              trim_string);

  sprintf(wtomsg,"DEAT - Name: '%s'",
  death1.death1o.namedo);
  write_op();
  sprintf(wtomsg,"DEAT - Killed by: '%s'",
  death1.death1o.killedo);
  write_op();


  sprintf(wtomsg,"DEAT - Getting ABSTIME");
  write_op();

  EXEC CICS 
    ASKTIME ABSTIME(ws_abstime);

  sprintf(wtomsg,"DEAT - Getting date");
  write_op();

  EXEC CICS FORMATTIME ABSTIME(ws_abstime)
             MMDDYY(date) DATESEP("/") ;

  sprintf(wtomsg,"DEAT - date %s",date);
  write_op();

  memcpy(death1.death1o.dateo,date,9);
    
  EXEC CICS 
    SEND MAP("death1") 
    MAPSET("death") 
    FROM(death1.death1o)
    ERASE;
        
  sprintf (wtomsg, WTO_MESSAGE, " DEAT - END - CICS Return");
  write_op();
  
  sprintf (wtomsg, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
  write_op();
  EXEC CICS
      RETURN 
        TRANSID("DEAT")
        COMMAREA(dead) LENGTH(4);

  return 0;
}
