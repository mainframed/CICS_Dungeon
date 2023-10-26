int path_find(Position * start, Position * end, Level * level) 
{
    sprintf(wtomsg,"Connecting doors (path_find) %d,%d -> %d,%d",
            start->y,start->x, 
            end->y,end->x);
    write_op();
    int x,y,j,i;
    int tempY;

    int ** frontier;
    frontier = malloc((MAXCOLS * MAXROWS) * sizeof(int *));

    int *** camefrom;
    camefrom = malloc(MAXROWS * sizeof(int **) );
    

    int frontier_index = 0;
    int frontier_count = 0;

    for (x = 0; x < (MAXCOLS * MAXROWS); x++)
    {
        frontier[x] = malloc(2* sizeof(int));
    }

    
    for (i = 0; i <= MAXROWS; i++)
    {
        
        camefrom[i] = malloc(sizeof(int *) * 80);
        
        for (j = 0; j <= MAXCOLS; j++)
        {
            camefrom[i][j] = malloc(sizeof(int *)*2);
            camefrom[i][j][0] = -1;
            camefrom[i][j][1] = -1;
        }
        
    }
    // sprintf(wtomsg,"Done Allocating Storage");
    // write_op();
    // add start to camefrom
    // sprintf(wtomsg,"add start to camefrom %d,%d",
    //             start->y,start->x);
    // write_op();
    camefrom[start->y][start->x][0] = -9;
    camefrom[start->y][start->x][1] = -9;

    // sprintf(wtomsg,"Done, Now add start to frontier %d,%d",
    //             start->y,start->x);
    // write_op();
    // add start position to frontier
    add_position_YX(frontier, frontier_count, 
                    start->y, start->x);
    frontier_count++;

    // for (x = 0; x < (MAXCOLS * MAXROWS); x++)
    // {
    //     sprintf(wtomsg,"WTF frontier[%d][0]=%d "\
    //     "frontier[%d][1]=%d",
    //     x,frontier[x][0],x,frontier[x][1]);
    //     write_op();
    // }
    

    // sprintf(wtomsg,"Finding Frotiers");
    // write_op();
    while(frontier_index < frontier_count) {
        y = frontier[frontier_index][0];
        x = frontier[frontier_index][1];
        frontier_index++;

        if (y==end->y && x==end->x)
        {
            break;
        }

        frontier_count = add_neighbors_YX(frontier, camefrom, 
                                         frontier_count, y, x, level, 0);  
        // sprintf(wtomsg,"dONE index %d count:%d",
        //         frontier_count, frontier_index);
        // write_op(); 
    }

    y = end->y;
    x = end->x;

    // sprintf(wtomsg,"Tracing Came froms");
    // write_op();
    while(y != start->y || x != start->x) {
        tempY = y;
        y = camefrom[tempY][x][0];
        x = camefrom[tempY][x][1];
        // sprintf(wtomsg,"Adding path to level y,x %d,%d",y,x);
        // write_op(); 
        level->tiles[y][x] = '#';
        level->colors[y][x] = YELLOW;
    }
    
    // sprintf(wtomsg,"Freeing up memory");
    // write_op();
    // Now we need to free up this memory

        for (x = 0; x < (MAXCOLS * MAXROWS); x++)
    {
        free(frontier[x]);
    }

    
    for (i = 0; i <= MAXROWS; i++)
    {
                
        for (j = 0; j <= MAXCOLS; j++)
        {
            free(camefrom[i][j]);

        }
        
       free(camefrom[i]);
    }
    free(frontier);
    free(camefrom);
    sprintf(wtomsg,"Pathfinding complete");
    write_op();

}

int find_monster(Player * player, Monster * monster, Level * level) 
{
    sprintf(wtomsg,"Finding path to player Monster %d,%d -> %d,%d",
            monster->position->y,monster->position->x, 
            player->position->y,player->position->x);
    write_op();
    int x,y,j,i;
    int tempY;

    int ** frontier;
    frontier = malloc((MAXCOLS * MAXROWS) * sizeof(int *));

    int *** camefrom;
    camefrom = malloc(MAXROWS * sizeof(int **) );
    

    int frontier_index = 0;
    int frontier_count = 0;

    for (x = 0; x < (MAXCOLS * MAXROWS); x++)
    {
        frontier[x] = malloc(2* sizeof(int));
    }

    
    for (i = 0; i <= MAXROWS; i++)
    {
        
        camefrom[i] = malloc(sizeof(int *) * 80);
        
        for (j = 0; j <= MAXCOLS; j++)
        {
            camefrom[i][j] = malloc(sizeof(int *)*2);
            camefrom[i][j][0] = -1;
            camefrom[i][j][1] = -1;
        }
        
    }
    // sprintf(wtomsg,"Done Allocating Storage");
    // write_op();
    // add start to camefrom
    // sprintf(wtomsg,"add start to camefrom %d,%d",
    //             start->y,start->x);
    // write_op();
    camefrom[player->position->y][player->position->x][0] = -9;
    camefrom[player->position->y][player->position->x][1] = -9;

    // sprintf(wtomsg,"Done, Now add start to frontier %d,%d",
    //             start->y,start->x);
    // write_op();
    // add start position to frontier
    add_position_YX(frontier, frontier_count, 
                    player->position->y, player->position->x);
    frontier_count++;

    // for (x = 0; x < (MAXCOLS * MAXROWS); x++)
    // {
    //     sprintf(wtomsg,"WTF frontier[%d][0]=%d "\
    //     "frontier[%d][1]=%d",
    //     x,frontier[x][0],x,frontier[x][1]);
    //     write_op();
    // }
    

    // sprintf(wtomsg,"Finding Frotiers");
    // write_op();
    while(frontier_index < frontier_count) {
        y = frontier[frontier_index][0];
        x = frontier[frontier_index][1];
        frontier_index++;

        if (y==monster->position->y && x==monster->position->x)
        {
            break;
        }

        frontier_count = add_neighbors_YX(frontier, camefrom, 
                                         frontier_count, y, x, level, 1);  
        // sprintf(wtomsg,"dONE index %d count:%d",
        //         frontier_count, frontier_index);
        // write_op(); 
    }

    y = monster->position->y;
    x = monster->position->x;

    // sprintf(wtomsg,"Tracing Came froms");
    // write_op();

      if (player->position->y == camefrom[y][x][0] && 
          player->position->x == camefrom[y][x][1])
      {
        /* combat! */
        combat(level, monster, 2);
      } else {
        monster->position->y = camefrom[y][x][0];
        monster->position->x = camefrom[y][x][1];
      }
    // sprintf(wtomsg,"Freeing up memory");
    // write_op();
    // Now we need to free up this memory

        for (x = 0; x < (MAXCOLS * MAXROWS); x++)
    {
        free(frontier[x]);
    }

    
    for (i = 0; i <= MAXROWS; i++)
    {
                
        for (j = 0; j <= MAXCOLS; j++)
        {
            free(camefrom[i][j]);

        }
        
       free(camefrom[i]);
    }
    free(frontier);
    free(camefrom);
    sprintf(wtomsg,"Pathfinding complete");
    write_op();

}

int add_position_YX(int ** frontier, int frontier_count, int y, int x) {
    frontier[frontier_count][0] = y;
    frontier[frontier_count][1] = x;
}

int position_check(int y, int x, Level * level, int monster) {
    char temp = get_char(y,x,level->tiles);
    if(monster) 
    {
        if (temp == '.' || temp == '#' )
        {
            return 1;
        } else 
        {
            return 0;
        }
    } 
    else 
    {
        if (temp == '.' || temp == '|' || temp == '-')
        {
            return 0;
        } else 
        {
            return 1;
        }
    }
    
    
}

int add_neighbors_YX(int ** frontier, int *** camefrom,
                     int frontier_count, int y, int x, 
                     Level * level, int monster) {
    // sprintf(wtomsg,"add_neighbors_YX count %d y:%d x:%d",
    //         frontier_count,y, x);
    // write_op();
    // North
    if(y > 0 && camefrom[y-1][x][0] < 0 &&
        position_check(y-1,x,level, monster)) {

        add_position_YX(frontier,frontier_count, y-1, x);
        
        camefrom[y-1][x][0] = y;
        camefrom[y-1][x][1] = x;
        frontier_count++; 
    
    // sprintf(wtomsg,"add_neighbors_YX NORTH count %d y:%d x:%d",
    //         frontier_count,y-1, x);
    // write_op();
    } 
    // South

    if(y < MAXROWS && camefrom[y+1][x][0] < 0 &&
       position_check(y+1,x,level,monster)) {

        add_position_YX(frontier,frontier_count, y+1, x);

        camefrom[y+1][x][0] = y;
        camefrom[y+1][x][1] = x;
        
        frontier_count++; 
    
    // sprintf(wtomsg,"add_neighbors_YX SOUTH count %d y:%d x:%d",
    //         frontier_count,y+1, x);
    // write_op();

    }
    // east

    if(x < MAXCOLS && camefrom[y][x+1][0] < 0&&
       position_check(y,x+1,level,monster)) {

        add_position_YX(frontier,frontier_count, y, x+1);

        camefrom[y][x+1][0] = y;
        camefrom[y][x+1][1] = x;
        frontier_count++; 
    
    // sprintf(wtomsg,"add_neighbors_YX EAST count %d y:%d x:%d",
    //         frontier_count,y, x+1);
    // write_op();

    }
    // west

    if(x > 0 && camefrom[y][x-1][0] < 0 &&
       position_check(y,x-1,level, monster)) {
        add_position_YX(frontier,frontier_count, y, x-1);

        camefrom[y][x-1][0] = y;
        camefrom[y][x-1][1] = x;
        frontier_count++; 
    
    // sprintf(wtomsg,"add_neighbors_YX WEST count %d y:%d x:%d",
    //         frontier_count,y, x-1);
    // write_op();

    }
    return frontier_count;
}