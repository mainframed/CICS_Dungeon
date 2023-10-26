//////////////////////////////////////////////////////////////////////////
// Map Functions
// map.c
//////////////////////////////////////////////////////////////////////////

Level * create_level(int level) {
  Level * new_level;
  new_level = malloc(sizeof(Level));
  new_level->level = level;
  new_level->number_of_rooms = 6;
  new_level->leveled_up = 0;
  rooms_setup(new_level);
  connect_doors(new_level);
  add_monsters(new_level);
  add_stairs(new_level);
  add_items(new_level);
  new_level->player = player_setup();
  place_player(new_level);

  memset(new_level->message,0,80);
  return new_level;

}

// char ** save_positions() {
//   int x;
//   int y;
//   char ** positions;
//   positions = malloc(sizeof(char *) * MAXROWS);

//   for (y=0; y <= MAXROWS; y++)
//   {
//     positions[y] = malloc(sizeof(char)*MAXCOLS);
//     memset(positions[y],0,MAXCOLS);
    
//   }
  
//   return positions;
// }

// char ** save_colors() {
//   int x;
//   int y;
//   char ** tile_colors;
//   tile_colors = malloc(sizeof(char *) * MAXROWS);

//   for (y=1; y <= MAXROWS; y++)
//   {
//     tile_colors[y] = malloc(sizeof(char)*MAXCOLS);
//     for (x=1; x <= MAXCOLS; x ++)
//     {
//       // sprintf(wtomsg,"save_colors: y:%d x:%d",y,x);
//       // write_op();
//       //tile_colors[y][x] = colors[y][x];
//     }
    
//   }
  
//   return tile_colors;
// }

Room * create_room(int grid, int number_of_doors) {
  sprintf(wtomsg,"Adding room in grid %d", grid);
  write_op();
  // sprintf(wtomsg,"Adding room y:%d x:%d w:%d h:%d", y, x, width, height);
  // write_op();

  Room * new_room;
  int i, x;
  
  new_room = malloc(sizeof(Room));
  new_room->number_of_doors = number_of_doors;
  
  // new_room->pos.y = y;
  // new_room->pos.x = x;
  // new_room->width = width;
  // new_room->height = height;

  switch (grid)
  {
  case 0:
    new_room->pos.x = 1;
    new_room->pos.y = 3;
    break;
  case 1:
    new_room->pos.x = 26;
    new_room->pos.y = 3;
    break;
  case 2:
    new_room->pos.x = 52;
    new_room->pos.y = 3;
    break;
  case 3:
    new_room->pos.x = 1;
    new_room->pos.y = 14;
    break;
  case 4:
    new_room->pos.x = 26;
    new_room->pos.y = 14;
    break;
  case 5:
    new_room->pos.x = 52;
    new_room->pos.y = 14;
    break;
  
  default:
    break;
  }
  
  new_room->height = (rand() % 4) + 4; // Max room height total 8
  new_room->width =  (rand() % 13) + 7; // Max room width total 19

  // Offset
  x = 24 - new_room->width;
  // sprintf(wtomsg,"x create room %d",x);
  // write_op();
  new_room->pos.x += rand() % (x);

  x = 9 - new_room->height;
  // sprintf(wtomsg,"y create room %d",x);
  // write_op();
  new_room->pos.y += rand() % (x);

  new_room->doors = malloc(sizeof(Door *)*number_of_doors);

  // for (i = 0; i < number_of_doors; i++)
  // {
  //   new_room->doors[i] = malloc(sizeof(Door));
  //   new_room->doors[i]->connected = 0;
  // }
  

  switch (grid)
  {
  case 0:
    new_room->doors[0] = malloc(sizeof(Door));
    new_room->doors[1] = malloc(sizeof(Door));
    new_room->doors[0]->position = malloc(sizeof(Position));
    new_room->doors[1]->position = malloc(sizeof(Position));

    
    // right door
    new_room->doors[0]->position->x = new_room->pos.x + new_room->width -1;
    new_room->doors[0]->position->y = rand() % (new_room->height - 2) + \
            new_room->pos.y+1;
    
    // bottom door
    new_room->doors[1]->position->x = rand() % (new_room->width - 2) + \
            new_room->pos.x +1;
    new_room->doors[1]->position->y = new_room->pos.y + new_room->height;
    break;
  case 1:
    new_room->doors[0] = malloc(sizeof(Door));
    new_room->doors[1] = malloc(sizeof(Door));
    new_room->doors[0]->position = malloc(sizeof(Position));
    new_room->doors[1]->position = malloc(sizeof(Position));
    
    // left door
    new_room->doors[0]->position->x = new_room->pos.x;
    new_room->doors[0]->position->y = rand() % (new_room->height - 2 )+ \
            new_room->pos.y+1;
    // right door
    new_room->doors[1]->position->x = new_room->pos.x + new_room->width -1;
    new_room->doors[1]->position->y = rand() % (new_room->height - 2) + \
            new_room->pos.y+1;
    break;
  case 2:
    new_room->doors[0] = malloc(sizeof(Door));
    new_room->doors[1] = malloc(sizeof(Door));
    new_room->doors[0]->position = malloc(sizeof(Position));
    new_room->doors[1]->position = malloc(sizeof(Position));
    new_room->doors[1]->connected = 0;
    new_room->doors[0]->connected = 0;
    
    // left door
    new_room->doors[0]->position->x = new_room->pos.x;
    new_room->doors[0]->position->y = rand() % (new_room->height - 2 )+ \
            new_room->pos.y+1;
    // bottom door
    new_room->doors[1]->position->x = rand() % (new_room->width - 2) + \
            new_room->pos.x +1;
    new_room->doors[1]->position->y = new_room->pos.y + new_room->height;
    break;
  case 3:
    new_room->doors[0] = malloc(sizeof(Door));
    new_room->doors[1] = malloc(sizeof(Door));
    new_room->doors[0]->position = malloc(sizeof(Position));
    new_room->doors[1]->position = malloc(sizeof(Position));
    new_room->doors[1]->connected = 0;
    new_room->doors[0]->connected = 0;
    
    // right door
    new_room->doors[0]->position->x = new_room->pos.x + new_room->width -1;
    new_room->doors[0]->position->y = rand() % (new_room->height - 2) + \
            new_room->pos.y+1;

    // // top door-
    new_room->doors[1]->position->x = rand() % (new_room->width - 2) + \
            new_room->pos.x +1;
    new_room->doors[1]->position->y = new_room->pos.y;
    break;
  case 4:
    new_room->doors[0] = malloc(sizeof(Door));
    new_room->doors[1] = malloc(sizeof(Door));
    new_room->doors[0]->position = malloc(sizeof(Position));
    new_room->doors[1]->position = malloc(sizeof(Position));
    new_room->doors[1]->connected = 0;
    new_room->doors[0]->connected = 0;

    // left door
    new_room->doors[0]->position->x = new_room->pos.x;
    new_room->doors[0]->position->y = rand() % (new_room->height - 2 )+ \
            new_room->pos.y+1;
    // right door
    new_room->doors[1]->position->x = new_room->pos.x + new_room->width -1;
    new_room->doors[1]->position->y = rand() % (new_room->height - 2) + \
            new_room->pos.y+1;
    break;
  case 5:
    new_room->doors[0] = malloc(sizeof(Door));
    new_room->doors[1] = malloc(sizeof(Door));
    new_room->doors[0]->position = malloc(sizeof(Position));
    new_room->doors[1]->position = malloc(sizeof(Position));
    new_room->doors[1]->connected = 0;
    new_room->doors[0]->connected = 0;
    // left door
    new_room->doors[0]->position->x = new_room->pos.x;
    new_room->doors[0]->position->y = rand() % (new_room->height - 2 )+ \
            new_room->pos.y+1;
    // top door-
    new_room->doors[1]->position->x = rand() % (new_room->width - 2) + \
            new_room->pos.x +1;
    new_room->doors[1]->position->y = new_room->pos.y;
    break;
  
  default:
    break;
  }
  // // top door-
  // new_room->doors[0]->position.x = rand() % (new_room->width - 2) + \
  //          new_room->pos.x +1;
  // new_room->doors[0]->position.y = new_room->pos.y;

  // // left door
  // new_room->doors[1]->position.x = new_room->pos.x;
  // new_room->doors[1]->position.y = rand() % (new_room->height - 2 )+ \
  //          new_room->pos.y+1;
  
  // // bottom door
  // new_room->doors[2]->position.x = rand() % (new_room->width - 2) + \
  //          new_room->pos.x +1;
  // new_room->doors[2]->position.y = new_room->pos.y + new_room->height;

  // // right door
  // new_room->doors[3]->position.x = new_room->pos.x + new_room->width -1;
  // new_room->doors[3]->position.y = rand() % (new_room->height - 2) + \
  //          new_room->pos.y+1;
  
  
  return new_room;
}

void rooms_setup(Level * level) {

  int x,y;
  //Room ** rooms;
  level->rooms = malloc(sizeof(Room) * 6);

  sprintf(wtomsg,"Creating Rooms for level %d",level->level);
  write_op();
  level->tiles = malloc(sizeof(char **) * MAXROWS);
  level->colors = malloc(sizeof(char **) * MAXROWS);

  for(y=0;y<=MAXROWS;y++) {
    level->tiles[y] = malloc(sizeof(char *) * MAXCOLS);
    level->colors[y] = malloc(sizeof(char *) * MAXCOLS);
    for(x=0;x<=MAXCOLS;x++) {
      level->tiles[y][x] = 0;
      level->colors[y][x] = 0;
    }
  }

  sprintf(wtomsg,"Done Alloc %d",level->level);
  write_op();


  for (x = 0; x < 6; x++)
  {
    level->rooms[x] = create_room(x,4);
    draw_room(level->rooms[x], level);
  }
  


}

int draw_room(Room *room, Level * level) {
  int x,y;

  sprintf(wtomsg,"Drawing Room: x:%d y:%d w:%d h:%d",
  room->pos.x, room->pos.y,room->width,room->height);
  write_op();


  // Draw top and bottom
  for ( x = room->pos.x; x < room->pos.x + room->width; x++)
  {
    put_tile(room->pos.y, x, "-",WHITE, level); // Top
    put_tile(room->pos.y + room->height, x, "-",WHITE, level); // Bottom
  }
  
  // Draw floors and walls
  for (y=room->pos.y + 1;y < room->pos.y + room->height; y++)
  {
    // Draw side walls

    put_tile(y, room->pos.x, "|",WHITE, level); // Left Side
    put_tile(y, room->pos.x+room->width - 1, "|",WHITE, level); // Right Side
    // Draw floor
    for (x = room->pos.x + 1; x < room->pos.x + room->width - 1; x++)
    {
      put_tile(y, x, ".",DEEP_BLUE, level);
    }
    
  }

  // Draw doors
  sprintf(wtomsg,"Adding Doors");
  write_op();
  put_tile(room->doors[0]->position->y,
           room->doors[0]->position->x , "#",YELLOW, level);
  put_tile(room->doors[1]->position->y,
           room->doors[1]->position->x , "#",YELLOW, level);
  // put_tile(room->doors[2]->position.y,
  //          room->doors[2]->position.x , "+",YELLOW);
  // put_tile(room->doors[3]->position.y,
  //          room->doors[3]->position.x , "+",YELLOW);

  

  return 1;
}


void connect_doors(Level * level) {
  int i, j;
  int randomroom, randomdoor;
  int count;

  // The pathfinding algo eats all our memory
  // and I'm not a good enough C programmer to
  // also the random paths look like crap. 

  // room 0 right door to room 1 left door
  path_find(level->rooms[0]->doors[0]->position,
          level->rooms[1]->doors[0]->position, level);
  level->rooms[0]->doors[0]->connected = 1;
  level->rooms[1]->doors[0]->connected = 1;

  // Room 1 Right door to Room 2 Left door
  path_find(level->rooms[1]->doors[1]->position,
          level->rooms[2]->doors[0]->position, level);
  level->rooms[1]->doors[0]->connected = 1;
  level->rooms[2]->doors[0]->connected = 1;

  // Room 2 Right door to Room 5 Top  door
  path_find(level->rooms[2]->doors[1]->position,
          level->rooms[5]->doors[1]->position, level);
  level->rooms[2]->doors[1]->connected = 1;
  level->rooms[5]->doors[1]->connected = 1;
  
  // Room 5 left door to Room 4 right door
  path_find(level->rooms[5]->doors[0]->position,
          level->rooms[4]->doors[1]->position, level);
  level->rooms[4]->doors[1]->connected = 1;
  level->rooms[5]->doors[0]->connected = 1;
  
  // Room 4 left door to Room 3 right door
  path_find(level->rooms[4]->doors[0]->position,
          level->rooms[3]->doors[0]->position, level);
  level->rooms[3]->doors[0]->connected = 1;
  level->rooms[4]->doors[0]->connected = 1;
  
  // Room 3 top door to Room 0 bottom door
  path_find(level->rooms[3]->doors[1]->position,
          level->rooms[0]->doors[1]->position, level);
  level->rooms[3]->doors[1]->connected = 1;
  level->rooms[0]->doors[1]->connected = 1;
  
sprintf(wtomsg,"Done adding doors");
write_op();

  

  // for (i = 0; i < level->number_of_rooms; i++)
  // {
    
  //   for (j = 0; j < level->rooms[i]->number_of_doors; j++)
  //   {
  //     if (level->rooms[i]->doors[j]->connected == 1)
  //     {
  //       continue;
  //     }

  //     count = 0;
  //     while(count < 2) {
  //       // Pick a random room/door target
  //       randomroom = rand() % level->number_of_rooms;
  //       randomdoor = rand() % level->rooms[randomroom]->number_of_doors;

  //       if (level->rooms[randomroom]->doors[randomdoor]->connected == 1 ||
  //           randomroom == i)
  //       {
  //         count++;
  //         continue;
  //       }
  //       sprintf(wtomsg,"connect_doors  %d,%d -> %d,%d",
  //       level->rooms[randomroom]->doors[randomdoor]->position.y,
  //       level->rooms[randomroom]->doors[randomdoor]->position.x,
  //       level->rooms[i]->doors[j]->position.y,
  //       level->rooms[i]->doors[j]->position.x);
  //       write_op();
  //       path_find(level->rooms[randomroom]->doors[randomdoor],
  //                 level->rooms[i]->doors[j], level);
  //       level->rooms[randomroom]->doors[randomdoor]->connected = 1;
  //       level->rooms[i]->doors[j]->connected = 1;
  //       break;
  //     }
  //   }
    
  // }
  
}

void add_stairs(Level * level) {

  int x,y, upstairs, downstairs;
  

  // > downstairs
  downstairs = rand() %6; // there's always 6 rooms
  switch (downstairs)
  {
  case 0:
    x = (rand() % (level->rooms[0]->width - 2)) + level->rooms[0]->pos.x + 1;
    y = (rand() % (level->rooms[0]->height - 2)) + level->rooms[0]->pos.y + 1;
    break;
  case 1:
    x = (rand() % (level->rooms[1]->width - 2)) + level->rooms[1]->pos.x + 1;
    y = (rand() % (level->rooms[1]->height - 2)) + level->rooms[1]->pos.y + 1;
    break;
  case 2:
    x = (rand() % (level->rooms[2]->width - 2)) + level->rooms[2]->pos.x + 1;
    y = (rand() % (level->rooms[2]->height - 2)) + level->rooms[2]->pos.y + 1;
    break;
  case 3:
    x = (rand() % (level->rooms[3]->width - 2)) + level->rooms[3]->pos.x + 1;
    y = (rand() % (level->rooms[3]->height - 2)) + level->rooms[3]->pos.y + 1;
    break;
  case 4:
    x = (rand() % (level->rooms[4]->width - 2)) + level->rooms[4]->pos.x + 1;
    y = (rand() % (level->rooms[4]->height - 2)) + level->rooms[4]->pos.y + 1;
    break;
  case 5:
    x = (rand() % (level->rooms[5]->width - 2)) + level->rooms[5]->pos.x + 1;
    y = (rand() % (level->rooms[5]->height - 2)) + level->rooms[5]->pos.y + 1;
    break;
  
  default:
    break;
  }

  sprintf(wtomsg,"Adding Down Stairs y:%d,x:%d",y,x);
  write_op();
  level->stairs_down.y = y;
  level->stairs_down.x = x;

  //  < upstairs
  if(level->level > 1) {
    upstairs = rand() %6; // there's always 6 rooms
    switch (upstairs)
    {
    case 0:
      x = (rand() % (level->rooms[0]->width - 2)) + level->rooms[0]->pos.x + 1;
      y = (rand() % (level->rooms[0]->height - 2)) + level->rooms[0]->pos.y + 1;
      break;
    case 1:
      x = (rand() % (level->rooms[1]->width - 2)) + level->rooms[1]->pos.x + 1;
      y = (rand() % (level->rooms[1]->height - 2)) + level->rooms[1]->pos.y + 1;
      break;
    case 2:
      x = (rand() % (level->rooms[2]->width - 2)) + level->rooms[2]->pos.x + 1;
      y = (rand() % (level->rooms[2]->height - 2)) + level->rooms[2]->pos.y + 1;
      break;
    case 3:
      x = (rand() % (level->rooms[3]->width - 2)) + level->rooms[3]->pos.x + 1;
      y = (rand() % (level->rooms[3]->height - 2)) + level->rooms[3]->pos.y + 1;
      break;
    case 4:
      x = (rand() % (level->rooms[4]->width - 2)) + level->rooms[4]->pos.x + 1;
      y = (rand() % (level->rooms[4]->height - 2)) + level->rooms[4]->pos.y + 1;
      break;
    case 5:
      x = (rand() % (level->rooms[5]->width - 2)) + level->rooms[5]->pos.x + 1;
      y = (rand() % (level->rooms[5]->height - 2)) + level->rooms[5]->pos.y + 1;
      break;
    
    default:
      break;
    }

    sprintf(wtomsg,"Adding Up Stairs y:%d,x:%d",y,x);
    write_op();
    level->stairs_up.y = y;
    level->stairs_up.x = x;
  } else {
    level->stairs_up.y = 0;
    level->stairs_up.x = 0;
  }

}