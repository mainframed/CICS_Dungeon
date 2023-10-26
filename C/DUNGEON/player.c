//////////////////////////////////////////////////////////////////////////
// Player
// player.c
//////////////////////////////////////////////////////////////////////////

Player * player_setup() {
    Player * new_player;
    new_player = malloc(sizeof(Player));
    new_player->position = malloc(sizeof(Position));
    // new_player->position->x = 14;
    // new_player->position->y = 14;
    new_player->hp = 20;
    new_player->atk = 1;
    new_player->def = 1;
    new_player->xp = 0;
    new_player->gold = 0;
    new_player->level = 1; // their XP level, not floor
    new_player->max_health = 20;
    return new_player;
}

int place_player(Level * level) {
  if (level->level == 1)
  {
    level->player->position->x = level->rooms[3]->pos.x + 1;
    level->player->position->y = level->rooms[3]->pos.y + 1;
  } else {
    level->player->position->y = level->stairs_up.y;
    level->player->position->x = level->stairs_up.x;
  }
  
}

int sync_stats_between_levels(Game * game) {
  Player * player;
  int i, hp, atk, def, lvl, max;

  player = game->levels[game->current_level]->player;
  hp = player->hp;
  atk = player->atk;
  def = player->def;
  lvl = player->level;
  max = player->max_health;
  

  sprintf(wtomsg,"Current Stats: hp %d atk %d def %d lvl %d",
          hp, atk, def, lvl);
  write_op();

  for (i = 0; i < game->number_of_levels; i++)
  {
    // yes this is hacky but i did this to myself

    game->levels[i]->player->hp = hp;
    game->levels[i]->player->atk = atk;
    game->levels[i]->player->def = def;
    game->levels[i]->player->level = lvl;
    game->levels[i]->player->max_health = max;
  }
  
}

int handle_input(int eibaid, Level * level, Game * game) {

    int i;
    char quit_message[5][19] = {
    "Unfortunate Event",
    "Early Retirement",
    "Honorable Defeat",
    "Epic Fail",
    "Unexpected Twist"
    };


    // We have to use F1-F4 for movement
    // We mirrored the vi format but with F keys
    sprintf(wtomsg,"handle_input: Player location y:%d, x:%d",
    level->player->position->y,level->player->position->x);
    write_op();
    switch (eibaid)  
    {
    case DFHPF1:
      /* LEFT */
      // There is no zeroeth column
      if (check_position(level->player->position->y,
                   level->player->position->x-1,
                   level)) {
        level->player->position->x--;
        sprintf(wtomsg,"LEFT");
        write_op();
      }
      break;
    case DFHPF2:
      /* DOWN */
      if (check_position((level->player->position->y + 1),
                    level->player->position->x,
                    level)) {
        level->player->position->y++;
        sprintf(wtomsg,"DOWN");
        write_op();
      }
      break;
    case DFHPF3:
      /* UP */
      // There is no zeroeth row
      if (check_position((level->player->position->y - 1),
                    level->player->position->x,
                    level)) {
       level->player->position->y--;
      sprintf(wtomsg,"RGUE - GOING UP");
      write_op();
      }
      break;
    case DFHPF4:
      /* RIGHT */
      if (check_position((level->player->position->y),
                    level->player->position->x+1,
                   level)) {
        level->player->position->x++;
        sprintf(wtomsg,"RIGHT");
        write_op();
      }
      break;
    case DFHPF9:
      /* QUIT */

        dead_player(level, quit_message[rand()%6]);

      break;
    
    default:
      break;
    }

    // What if we touched a weapon?
    //weapon

    if (level->player->position->y == level->weapon->position->y &&
        level->player->position->x == level->weapon->position->x &&
        level->weapon->collected == 0)
    {
      // sprintf(wtomsg,"Weapon Player %d,%d Weapon %d,%d collected %d",
      // level->player->position->y,level->player->position->x, 
      //    level->weapon->position->y, level->weapon->position->x,
      //   level->weapon->collected == 0);
      // write_op();
      level->player->atk += level->weapon->item.weapon->attack;
      level->weapon->collected = 1;

      sprintf(level->message,"Obtained '%s' (+%d attack)",
              level->weapon->name,level->weapon->item.weapon->attack);
    }
    //armor
    if (level->player->position->y == level->armor->position->y &&
        level->player->position->x == level->armor->position->x &&
        level->armor->collected == 0)
    {
      level->player->def += level->armor->item.armor->defense;
      level->armor->collected = 1;
      sprintf(level->message,"Obtained '%s' (+%d defense)",
              level->armor->name,level->armor->item.armor->defense);
    }
    //potion
    if (level->player->position->y == level->potion->position->y &&
        level->player->position->x == level->potion->position->x &&
        level->potion->collected == 0)
    {

      if(level->potion->item.potion->health + level->player->hp > \
          level->player->max_health ) {
            level->player->hp = level->player->max_health;
          } else {
            level->player->hp += level->potion->item.potion->health;
          }
      level->potion->collected = 1;
      sprintf(level->message,"Obtained '%s' (+%d healing)",
              level->potion->name,level->potion->item.potion->health);
    }

    // What if we're on stairs?

    // sprintf(wtomsg,"Player y,x %d,%d Stairs Down y,x %d,%d",
    //         level->player->position->y,level->player->position->x,
    //         level->stairs_down.y, level->stairs_down.x);
    // write_op();

    if (level->player->position->y == level->stairs_down.y &&
        level->player->position->x == level->stairs_down.x)
    {
      game->current_level++;
    }

    // sprintf(wtomsg,"Player y,x %d,%d Stairs Down y,x %d,%d",
    //         level->player->position->y,level->player->position->x,
    //         level->stairs_up.y, level->stairs_up.x);
    // write_op();
    
    if (level->player->position->y == level->stairs_up.y &&
        level->player->position->x == level->stairs_up.x)
    {
      game->current_level--;
    }
    

}

int check_position(int y,int x, Level * level) {
  int i;
  sprintf(wtomsg,"Player wants to move to y:%d,x:%d = '%c'",y,x,
                get_char(y,x, level->tiles));
  write_op();
  for (i = 0; i < level->number_of_monsters; i++)
  { 
   if (level->monsters[i]->alive) 
   {  
   sprintf(wtomsg,"Player Position y:%d,x:%d - Monster Position %s y:%d,x:%d",
           level->player->position->y,
           level->player->position->x,
           level->monsters[i]->name,
           level->monsters[i]->position->y,
           level->monsters[i]->position->x);
  write_op();
   }
  }
  
  switch (get_char(y,x,level->tiles) )
  {
  case '.':
  case '+':
  case '#':
    for (i = 0; i < level->number_of_monsters; i++)
    {
        if(level->monsters[i]->alive) {
            // they alive
            // sprintf(wtomsg,"level->monsters[%d]->position->y (%d) == y (%d)"\
            //            "level->monsters[%d]->position->x (%d) == x (%d)",
            //            i, level->monsters[i]->position->y, y,
            //            i, level->monsters[i]->position->x, x);
            // write_op();
            if(level->monsters[i]->position->y == y &&
                level->monsters[i]->position->x == x) {
                    combat(level, level->monsters[i],1);
                
                    if(level->monsters[i]->alive) {
                    return 0;
                }
            }
        }
    } 
    return 1;     
  break;

  default:
    break;
  }
  return 0;
}

void put_player(Level * level) {
  // puts the player '@' to location x/y
  // First set the buffer address
  // then the color
  // Finally the @
  sprintf(wtomsg,"Moving player to %d,%d",
          level->player->position->y, 
          level->player->position->x);
  write_op();
  sba(level->player->position->y, 
      level->player->position->x);
  change_color(NEUTRAL_WHITE);
  append_screen("@",1);
}

void dead_player(Level * level, char * killed_by) {

    int i,j;
    struct dead death;
    memset(&death,0,sizeof(dead));

    sprintf (wtomsg, WTO_MESSAGE, "RGUE - Player Died");
    write_op();
    sprintf (wtomsg, "Name %s", level->player->name);
    write_op();
    memcpy(death.name,level->player->name,17);
    memcpy(death.killed_by,killed_by,strlen(killed_by));


    // sprintf (wtomsg, "Freeing Monsters");
    // write_op();
    

    // for(i=0;i < level->number_of_monsters;i++) {
    //       sprintf (wtomsg, "Freeing Monster %d",i);
    // write_op();
    //   free(level->monsters[i]->position);
    //   free(level->monsters[i]);
    // }

    // sprintf (wtomsg, "Freeing Rooms");
    // write_op();

    // for(i=0;i < level->number_of_rooms;i++) {
    //   for(j=0;j <= level->rooms[i]->number_of_doors;j++ ) {
    //     free(level->rooms[i]->doors[j]);
    //   }
    //   free(level->rooms[i]);
    // }
    // sprintf (wtomsg, "Freeing tiles/colors");
    // write_op();
    for (i=0; i < MAXROWS; i++)
    {
      free(level->tiles[i]);
      free(level->colors[i]);
    }
      free(level->tiles);
      free(level->colors);
    // sprintf (wtomsg, "Freeing Monsters");
    // write_op();
    // free(level->monsters);
    // sprintf (wtomsg, "Freeing Player Position");
    // write_op();
  
    // free(level->player->position);
    // sprintf (wtomsg, "Freeing Player");
    // write_op();
    // free(level->player);
    // sprintf (wtomsg, "Freeing Level");
    // write_op();
    // free(level);
    // sprintf (wtomsg, "Opening DeathPGM");
    // write_op();

      EXEC CICS 
          XCTL PROGRAM("DEATHPGM")
          COMMAREA(death)
          LENGTH(sizeof(death));
}

void killed_all_monsters(Level * level) {
  int x;
    int i,j;
    struct dead death;
    memset(&death,0,sizeof(dead));

  if (level->leveled_up)
  {
    return;
  }
  

  for(x=0;x<level->number_of_monsters;x++) {
    if(level->monsters[x]->alive) {
      break;
    }
  }
  
  if(x == level->number_of_monsters) {
    sprintf(level->message,"Floor Cleared - Level Up!");
    level->player->level++;
    level->player->atk++;
    level->player->def++;
    level->player->max_health += 10;
    level->leveled_up = 1;
    if(level->level == 6) {
      win_screen(level->player->name);
    }
  }

}

