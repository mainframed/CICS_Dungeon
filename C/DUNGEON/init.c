//////////////////////////////////////////////////////////////////////////
// Initialization
//////////////////////////////////////////////////////////////////////////

void init_screen() {
  // \x27 - TSO Escape Char
  // \x7E - Erase/Write Alternate (EWA)
  // \xC3 - WCC
  location = 0;
  memset(screen,0,MAXSCREEN);
  append_screen("\x27\x7e\xc3",3); 
  // KICKS Documentation says WCC is enough
  //append_screen("\xc3",1); // it wasn't
  sba(1,1);
  append_screen("\x1d\xf0",2);
}

void header(Player * player, int floor) {
  sprintf(wtomsg,"hp: %d (%d), attack: %d, defense: %d",
          player->hp, player->max_health, player->atk, player->def);
  write_op();
  char stat[10];
  sba(1,1);
  change_color(RED);
  append_screen("CICS Dungeon",12);
  sba(1,16); // SBA is ROW, COL
  change_color(WHITE);
  sprintf(stat,"Health: %d/%d",player->hp, player->max_health);
  append_screen(stat,strlen(stat));
  sba(1,33);
  change_color(GREEN);
  sprintf(stat,HEADER_STATS,"Attack",player->atk);
  append_screen(stat,strlen(stat));
  sba(1,46);
  change_color(TURQUOISE);
  sprintf(stat,HEADER_STATS,"Defense",player->def);
  append_screen(stat,strlen(stat));
  sba(1,60);
  change_color(BLUE);
  sprintf(stat,HEADER_STATS,"Level",player->level);
  append_screen(stat,strlen(stat));
  sba(1,72);
  change_color(YELLOW);
  sprintf(stat,HEADER_STATS,"Floor",floor);
  append_screen(stat,strlen(stat));
}

void footer(Level * level) {
  sba(24,1); // Row 24, Col 1
  char clear[80];
  sprintf(clear,"%s%s",
  "                                        ",
  "                                       ");
  append_screen(clear,79);
  sba(24,1); // Row 24, Col 1
  change_color(RED);
  append_screen(level->message,strlen(level->message));
  // I kind of like the message staying on screen for that level
  // sprintf(level->message,"%s%s",
  // "                                        ",
  // "                                       ");
  // append_screen(clear,79);
}

void intro(char * name) {
  // print the intro screen
  init_screen(); 
  sba(1,1);
  change_color(BLUE);
  append_screen("Welcome to CI",13); // KICKS changes CICS to KICKS
  append_screen("CS Dungeon ",12);   // So we have to do this here
  sba(1,25);
  change_color(RED);
  append_screen(name,19);
  sba(3,1);
  change_color(BLUE);
  append_screen("You stand before a dark and ominous dungeon. ",45);
  append_screen("You know that great",19);
  sba(4,1);
  append_screen("danger lurks within, but you are also drawn by"\
                " the promise of",61);
  sba(5,1);
  append_screen("tapes and glory.",16);
  sba(7,1);
  append_screen("You take a deep breath and step inside. ",40);
  append_screen("The door slams shut behind",26); 
  sba(8,1);
  append_screen("you, leaving you in darkness. You can hear the sound of ",56);
  append_screen("giant spiders",13);
  sba(9,1);
  append_screen("scuttling in the shadows.",25);
  sba(11,1);
  append_screen("You draw your sword and make your way forward. ",47);
  append_screen("You don't know what",19);
  sba(12,1);
  append_screen("lies ahead, but you're determined to find out.",46);                    
  sba(14,1);  
  change_color(WHITE);
  append_screen("How to play:",12);
  sba(15,1); 
  change_color(WHITE); 
  append_screen("- ",2);
  change_color(BLUE); 
  append_screen("Use the ",8);
  change_color(YELLOW); 
  append_screen("F1 ",3);
  change_color(BLUE); 
  append_screen("through ",8);
  change_color(YELLOW); 
  append_screen("F4 ",3);
  change_color(BLUE); 
  append_screen("keys for movement",17);
  sba(16,1);  
  change_color(WHITE); 
  append_screen("- ",2);
  change_color(YELLOW); 
  append_screen("F1 ",3);
  change_color(BLUE); 
  append_screen("Left ",5);
  change_color(YELLOW); 
  append_screen("F2 ",3);
  change_color(BLUE); 
  append_screen("Down ",5);
  change_color(YELLOW); 
  append_screen("F3 ",3);
  change_color(BLUE); 
  append_screen("Up ",3);
  change_color(YELLOW); 
  append_screen("F4 ",3);
  change_color(BLUE); 
  append_screen("Right ",6);
  sba(17,1);  
  change_color(WHITE); 
  append_screen("- ",2);
  change_color(YELLOW); 
  append_screen("F9 ",3);
  change_color(BLUE); 
  append_screen("exits the dungeon",18);
  sba(19,1);  
  append_screen("Clearing a floor levels up your character. ",43);
  append_screen("There are weapons '",19);
  change_color(WHITE); 
  append_screen(")",1);
  change_color(BLUE); 
  append_screen("',",2);
  sba(20,1);  
  append_screen("armor '",7);
  change_color(TURQUOISE); 
  append_screen("[",1);
  change_color(BLUE); 
  append_screen("' and potions '",15);
  change_color(PINK); 
  append_screen("!",1);
  change_color(BLUE); 
  append_screen("' to collect. There are also enemies so ",39);
  append_screen("watchout!",9);
  sba(22,1);  
  change_color(GREEN); 
  append_screen("Press ENTER to continue",23);

  EXEC CICS SEND TEXT
    FROM(screen) LENGTH(size_of_screen()+1)
    STRFIELD;

  sprintf(wtomsg,"RGUE - Get Data");
  write_op();

  EXEC KICKS RECEIVE NOHANDLE ;


}
