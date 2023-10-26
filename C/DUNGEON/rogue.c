/////////////////////////////////////////////////////////////////
// CICS Dungeon main program
//
// Transaction ID: RGUE
//
// DUNGEON:
//   This contains the main game logic and screen drawing
//   algorithms. It's broken up in to multiple C files but
//   the scripts build.sh and release.sh simply combine
//   all these files in to one DD inline statement. 
//
// AUTHOR:
//   Philip Young aka Soldier of FORTRAN
//
// 10/08/2022
// License GPL v3
/////////////////////////////////////////////////////////////////

#include "rogue.h"
#include "items.h"

//////////////////////////////////////////////////////////////////////////
// Debug
//////////////////////////////////////////////////////////////////////////

void write_op() {
    int wtolen = sizeof(wtomsg);
#ifdef DEBUG
    EXEC CICS WRITE OPERATOR
     TEXT(wtomsg) TEXTLENGTH(wtolen);
#endif
    memset(wtomsg,'\0',126); 
}

//////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////

int main(KIKEIB *eib, mycomm *KIKCOMM) {


  sprintf(wtomsg,"=======ROGUE================================");
  write_op();
  srand(time(NULL));

  int y,x,counter=0;
  Game * game;
  Level * level;

  game->current_level = 0;
  game->number_of_levels = 6;

  if (eib->eibcalen == 0)
    {
    sprintf(wtomsg,"RGUE - No data returning to title");
    write_op();
    EXEC CICS 
      XCTL PROGRAM("TITLEPGM");
    }

  game->levels[0] = create_level(1);
  game->levels[1] = create_level(2);
  game->levels[2] = create_level(3);
  game->levels[3] = create_level(4);
  game->levels[4] = create_level(5);
  game->levels[5] = create_level(6);
  // game->levels[6] = create_level(7);
  memcpy(game->levels[0]->player->name,KIKCOMM->name,19);
  memcpy(game->levels[1]->player->name,KIKCOMM->name,19);
  memcpy(game->levels[2]->player->name,KIKCOMM->name,19);
  memcpy(game->levels[3]->player->name,KIKCOMM->name,19);
  memcpy(game->levels[4]->player->name,KIKCOMM->name,19);
  memcpy(game->levels[5]->player->name,KIKCOMM->name,19);
  // memcpy(game->levels[6]->player->name,KIKCOMM->name,19);
  intro(KIKCOMM->name);
  eib->eibaid = DFHNULL;
  
  // 


  while(1) {   
    int i;
    sprintf(wtomsg,"=======WHILE (%d)================================", 
              game->current_level);
    write_op(); 

    init_screen();  
    sync_stats_between_levels(game);
    handle_input(eib->eibaid, level, game) ;
    level = game->levels[game->current_level];
    render(level);

    sprintf(wtomsg,"RGUE - CICS SEND screen");
    write_op();


    EXEC CICS SEND TEXT
      FROM(screen) LENGTH(size_of_screen()+1)
      STRFIELD;

    sprintf(wtomsg,"RGUE - Get Data");
    write_op();

    EXEC KICKS RECEIVE NOHANDLE ;

  } // end of while loop

  // EXEC CICS
  //     RETURN 
  //       TRANSID("RGUE")
  //       COMMAREA(player) LENGTH(sizeof(player));

  return 0;
}

