//////////////////////////////////////////////////////////////////////////
// Monsters
// monsters.c
//////////////////////////////////////////////////////////////////////////


 int add_monsters(Level * level) {

  sprintf(wtomsg,"Adding monsters");
  write_op();

  int x;

  level->monsters = malloc(sizeof(Monster)*6);
  level->number_of_monsters = 0;

  for (x = 0; x < level->number_of_rooms; x++)
  {
    if (rand() % 2 == 0)
    {
      
      /* Add a monster */
    level->monsters[level->number_of_monsters] = select_monster(level->level);
    set_starting_position(level->monsters[level->number_of_monsters],
                            level->rooms[x], level, x);
      level->number_of_monsters++;
      
      if (level->level == 6)
      { // just a boss in level 7
        break;
      }
    }

    
  }

    sprintf(wtomsg,"Number of monsters: %d",level->number_of_monsters);
    write_op();
  
 }

Monster * select_monster(int level) {

  int monster;

  switch (level)
  {
  case 1:
    // jelly, bat, spider, kobold 
    monster = (rand() % 4) + 1;
    break;
  case 2:
    // jelly, bat, spider, kobold, spider, kobold, owl bear
    monster = (rand() % 6) + 1;
    break;
  case 3:  
    monster = (rand() % 6) + 2;
    break;
  case 4:
    monster = (rand() % 3) + 6;
    break;
  case 5: 
    monster = (rand() % 4) + 6;
    break;
  case 6: 
    monster = 11;
  }


// Monsters:
// j - 1-2 - jelly    - atk - 1 - Blue
// B - 1-2 - bat      - atk - 1 - Red
// s - 1-3 - spider   - atk - 1 - Pink
// k - 1-3 - kobold   - atk - 1 - Yellow
// Y - 2-3 - owl bear - atk - 1 - Yellow
// C - 2-4 - centaur  - atk - 1 - Turquoise
// D - 3-5 - dark elf - atk - 1 - Green
// g - 3-5 - goblin   - atk - 1 - Green
// O - 5-6 - ogres    - atk - 1 - Green
// T - 5-6 - Troll    - atk - 1 - Turq

// char symbol, int health, int attack, 
// int speed, int defense, 
// int pathfinding, char color, char *name

  switch (monster)
  {
  case 1: // jelly
    return create_monster('j',1,1,1,1,1,BLUE,"Blue Jelly");
  break;
  case 2: // bat
    return create_monster('b',2,1,1,1,2,BLUE,"Bat");
  break;
  case 3: // spider
    return create_monster('s',3,2,1,1,1,PINK,"Spider");
  break;
  case 4: // kobold
    return create_monster('k',4,5,1,1,2,YELLOW,"Kobold");
  break;
  case 5: // owl bear
    return create_monster('o',6,6,1,1,2,YELLOW,"Owl Bear");
  break;
  case 6: // centaur
    return create_monster('C',8,8,1,1,2,BLUE,"Centaur");
  break;
  case 7: // dark elf
    return create_monster('D',9,10,1,1,2,BLUE,"Dark Elf");
  break;
  case 8: // goblin
    return create_monster('g',15,10,1,1,2,GREEN,"Goblin");
  break;
  case 9: // Blind Ogre
    return create_monster('O',25,15,1,1,1,PALE_GREEN,"Blind Ogre");
  break;
  case 10: // Troll
    return create_monster('T',40,20,1,1,2,TURQUOISE,"Troll");
  break;
  case 11: // SYSPROG (Boss)
    return create_monster('S',50,25,1,1,2,RED,"SYSPROG");
  break;
  
  }

}
/*

1 Spider
  symbol: X
  levels: 1-3
  health: 2
  attack: 1
  speed: 1
  defense: 1
  pathfinding: 1 (random)

2 Goblin
  symbol: G
  levels: 1-5
  health: 5
  attack: 3
  speed: 1
  defense: 1
  pathfinding: 2 (seeking)

3 Troll
  symbol: T
  levels: 4-6
  health: 15
  attack: 5
  speed: 1
  defense: 1
  pathfinding: 1 (random)

 4 Sysprog
  symbol: S
  levels: 6
  health: 2
  attack: 20
  speed: 1
  defense: 10
  pathfinding: 2 (seeking)

*/

Monster * create_monster(char symbol, int health, int attack, 
                          int speed, int defense, 
                          int pathfinding, char color, char *name)
{
  Monster * new_monster;
  new_monster = malloc(sizeof(Monster));
  new_monster->position = malloc(sizeof(Position));

  new_monster->symbol = symbol;
  new_monster->color = color;
  new_monster->health = health;
  new_monster->attack = attack;
  new_monster->alive =1; 
  new_monster->speed = speed;
  new_monster->name = name;
  new_monster->defense = defense;
  new_monster->pathfinding = pathfinding;
  sprintf(new_monster->string,"%c",symbol);
  
  return new_monster;


}

int set_starting_position(Monster * monster, Room * room, 
                          Level * level, int room_number) {

  monster->position->x = (rand() % (room->width - 2)) + room->pos.x + 1;
  monster->position->y = (rand() % (room->height - 2)) + room->pos.y + 1;
  monster->room_number = room_number;
  sprintf(wtomsg,"Adding Monster %s (%c) Room %d",monster->name,
                  monster->symbol, room_number);
  write_op();
  
  // put_tile(monster->position->y,monster->position->x,
  //           monster->string,monster->color, level);

}

int move_monsters(Level * level) {
  int x;
  for (x = 0; x < level->number_of_monsters; x++)
  {
    if (!level->monsters[x]->alive)
      continue;
    if (level->monsters[x]->pathfinding == RANDOM_BEHAVIOR)
    {
      m_random(level->monsters[x]->position, level,level->monsters[x]);
    } else {
      /* Seeking behavior */
      // check if the player is in the room, if they aren't move random

      if(level->level == 6) {
        find_monster(level->player, level->monsters[x], level);
      }
      else {
        if(in_room(level->rooms[level->monsters[x]->room_number],
                  level->player->position)) {

        m_seek(level->monsters[x]->position, 
                    level->player->position, level,level->monsters[x]);
        } else {
          m_random(level->monsters[x]->position, level,level->monsters[x]);
        }
      }
    }    
    put_monster(level->monsters[x]);
  }
  return 1;
}

int in_room(Room * room, Position * player_position) {
  if (player_position->y > room->pos.y && 
      player_position->y < room->pos.y + room->height &&
      player_position->x > room->pos.x &&
      player_position->x < room->pos.x + room->width)
  {
    // we're in the room!
    return 1;
  }
  return 0;
}

int m_seek(Position * start, Position * destination, 
          Level * level, Monster * monster) {
  
  // left
  if (abs((start->x -1) - destination->x) < \
      abs(start->x - destination->x) && 
      get_char(start->y,start->x - 1, level->tiles) == '.')
    {
      // sprintf(wtomsg,"Left %d,%d", start->y, start->x);
      // write_op();
      if (start->x -1 == destination->x && start->y == destination->y)
      {
        /* combat! */
        combat(level, monster, 2);
      } else {
        start->x = start->x - 1;
      }
     
    // Step right
    } else if (abs((start->x + 1) - destination->x) < \
               abs(start->x - destination->x) &&
               get_char(start->y,start->x + 1, level->tiles) == '.')
    {
      //put_tile(start->y, start->x + 1 , "#",YELLOW);
      // sprintf(wtomsg,"rIGHT %d,%d", start->y, start->x);
      // write_op();
      if (start->x +1 == destination->x && start->y == destination->y)
      {
        /* combat! */
        combat(level, monster, 2);
      } else {
        start->x = start->x + 1;
      }

    // Step Down
    } else if (abs((start->y + 1) - destination->y) < \
               abs(start->y - destination->y) &&
               get_char(start->y + 1,start->x, level->tiles) == '.')
    {

      if (start->x == destination->x && start->y + 1 == destination->y)
      {
        /* combat! */
        combat(level, monster, 2);
      } else {
        start->y = start->y + 1;
      }
      // sprintf(wtomsg,"Down %d,%d", start->y, start->x);
      // write_op();

    
    // Step up
    } else if (abs((start->y - 1) - destination->y) < \
               abs(start->y - destination->y) &&
               get_char(start->y - 1,start->x, level->tiles) == '.')    
    {
      //put_tile(start->y - 1, start->x , "#",YELLOW);
      // sprintf(wtomsg,"uP %d,%d", start->y, start->x);
      // write_op();
      if (start->x == destination->x && start->y -1 == destination->y)
      {
        /* combat! */
        combat(level, monster, 2);
      } else {
        start->y = start->y - 1;;
      }
      

    } else {
      /* do nothing */
    }
    // put_tile(start->y, start->x , "#",YELLOW);
}

void m_random(Position * position, Level * level, Monster * monster) {

  int random;
  random = rand() % 5;

  switch (random)
  {
  case 0:
    /* step up */
    if (get_char(position->y-1,position->x, level->tiles) == '.'){
      if (position->y - 1 == level->player->position->y &&
          position->x == level->player->position->x) {
            combat(level,monster,2);
      } else {
        position->y--;
      }
    }
    break;
  case 1:
    /* step down */
    if (get_char(position->y+1,position->x, level->tiles) == '.'){
      if (position->y + 1 == level->player->position->y &&
          position->x == level->player->position->x) {
            combat(level,monster,2);
      } else {
        position->y++;
      }
    }
    break;
  case 2:
    /* step left */
    if (get_char(position->y,position->x-1, level->tiles) == '.'){

      if (position->y == level->player->position->y &&
          position->x-1 == level->player->position->x) {
            combat(level,monster,2);
      } else {
        position->x--;
      }
    }
    break;
  case 3:
    /* step right */
    if (get_char(position->y,position->x+1, level->tiles) == '.'){
      
      if (position->y == level->player->position->y &&
          position->x+1 == level->player->position->x) {
            combat(level,monster,2);
      } else {
        position->x++;
      }
    }
    break;
  case 4:
    /* step none */
    break;
  
  default:
    break;
  }
}

void put_monster(Monster * monster) {
  // puts the player monster to location x/y
  // First set the buffer address
  // then the color
  // Finally the symbol
  sprintf(wtomsg,"Moving monster %s to %d,%d",
          monster->name,
          monster->position->y, 
          monster->position->x);
  write_op();
  sba(monster->position->y, 
      monster->position->x);
  change_color(monster->color);
  append_screen(monster->string,1);
}

void kill_monster(Monster * monster) {
  sprintf(wtomsg,"%s killed! %d,%d",monster->name,
                  monster->position->y,monster->position->x);
  write_op();
  monster->alive = 0;
}
