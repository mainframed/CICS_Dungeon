//////////////////////////////////////////////////////////////////////////
// Putting/getting things on screen(tm) Functions
// screen.c
//////////////////////////////////////////////////////////////////////////

// Fills our map buffer with tiles at locations
void put_tile(int y, int x, char *ch, char color, Level * level) {
  // sprintf(wtomsg,"Placing '%s' Color: '%.02x' (y:%d, x:%d)",
  //                 ch,color,y,x);
  // write_op();  
  // colors[y][x] = color;
  // map[y][x] = ch[0];
  level->tiles[y][x] = ch[0];
  level->colors[y][x] = color;
}

// draws the contents of map and colors
void draw_tiles(Level * level) {
  int y,x;
  char * tile;
  for(y=1;y<=MAXROWS;y++) {
    for(x=1; x<= MAXCOLS;x++) {
      if (level->tiles[y][x] != '\0') {
        // sprintf(wtomsg,"Mapping '%c' Color: '%.02x' (y:%d, x:%d)",
        //                  level->tiles[y][x],level->colors[y][x],y,x);
        // write_op();
        sba(y, x);
        change_color(level->colors[y][x]);
        tile = &level->tiles[y][x];
        append_screen(tile,1);
      }
    }
  }  
}

char get_char(int y, int x, char ** tiles) {
  // sprintf(wtomsg,"get_char: y:%d, x%d = '%c'",y,x,tiles[y][x]);
  // write_op();
  return tiles[y][x];
}

void put_stairs(Level * level) {
  // down stairs >
  if (level->level < 6 )
  {
    sba(level->stairs_down.y,level->stairs_down.x);
    change_color(YELLOW);
    append_screen(">",1);
  }
  
  // up stairs <
  if (level->level > 1 )
  {
    sba(level->stairs_up.y,level->stairs_up.x);
    change_color(YELLOW);
    append_screen("<",1);
  }
}

void put_items(Level * level) {
  char temp[2];
  // weapon
  if(!level->weapon->collected) 
  {
  sba(level->weapon->position->y,level->weapon->position->x);
  change_color(level->weapon->color);
  sprintf(temp,"%c",level->weapon->symbol);
  append_screen(temp,1);
  }
  // armor
  if(!level->armor->collected) 
  {
  sba(level->armor->position->y,level->armor->position->x);
  change_color(level->armor->color);
  sprintf(temp,"%c",level->armor->symbol);
  append_screen(temp,1);
  }
  // potion
  if(!level->potion->collected) 
  {
  sba(level->potion->position->y,level->potion->position->x);
  change_color(level->potion->color);
  sprintf(temp,"%c",level->potion->symbol);
  append_screen(temp,1);
  }
}

void win_screen(char * name) {
  init_screen(); 
  sba(1,1);
  append_screen("Congrats",8);
  sba(1,10);
  change_color(RED);
  append_screen(name,19);
  sba(3,1);
  change_color(BLUE);
  append_screen("You have conquered the dungeon and defeated the final ",54);
  append_screen("boss. You grab ",15);
  sba(4,1);
  append_screen("what you came for, the source code to JES2.",43);
  sba(6,1);
  append_screen("You sheath your sword and turn to leave the ",44);
  append_screen("datacenter. You have much ",26);
  sba(7,1);
  append_screen("to think about your journey, and you know that your",52);
  append_screen("life will never be ",19);
  sba(8,1);
  append_screen("the same again.",15);
  sba(10,1);
  append_screen("But you also know that there will be other challenges ",54);
  append_screen("ahead. The world",16);
  sba(11,1);
  append_screen("is still in ruins, and there are many dangers that ",51);
  append_screen("still lurk in the",17);
  sba(12,1);
  append_screen("shadows. But you are confident that you, and the other ",55);
  append_screen("survivors, can",14);
  sba(13,1);
  append_screen("rebuild the world and create a better future.",45);
  sba(15,1);
  append_screen("You walk out of the discheveled datacenter, ready to ",53);
  append_screen("face the future",15);
  sba(16,1);
  append_screen("with courage and determination.",31);
  sba(18,1);
  change_color(GREEN);
  append_screen("Credits:",8);
  sba(19,1);
  append_screen("Game Design and Graphics: ",26);
  change_color(TURQUOISE);
  append_screen("Soldier of FORTRAN",18);

  EXEC CICS SEND TEXT
    FROM(screen) LENGTH(size_of_screen()+1)
    STRFIELD;

  EXEC KICKS RECEIVE NOHANDLE ;

  EXEC CICS 
      XCTL PROGRAM("TITLEPGM");
  return;
}

void render(Level * level) {
    killed_all_monsters(level); 
    draw_tiles(level); 
    put_stairs(level);
    put_items(level);
    put_player(level);
    move_monsters(level);
    header(level->player, level->level);
    footer(level);
}



