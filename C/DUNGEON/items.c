
void add_items(Level * level) {

    sprintf(wtomsg,"Creating Items");
    write_op();
    int rand_room, y, x, i;
    for (i = 0; i < 3; i++)
    {
        rand_room = rand() % 6;
        y = (rand() % (level->rooms[rand_room]->height - 2)) + \
            level->rooms[rand_room]->pos.y + 1;
        x = (rand() % (level->rooms[rand_room]->width - 2)) + \
            level->rooms[rand_room]->pos.x + 1;

        sprintf(wtomsg,"Adding Item y:%d,x:%d room: %d", y,x,rand_room);
        write_op();
        switch (i)
        {
        case 0:
            sprintf(wtomsg,"Creating Weapon");
            write_op();
            level->weapon = create_sword(y,x,rand()%6+1);
            break;
        case 1:
            sprintf(wtomsg,"Creating Armor");
            write_op();
            level->armor = create_armor(y,x,rand()%6+1);
            break;
        case 2:
            sprintf(wtomsg,"Creating Potion");
            write_op();
            level->potion = create_potion(y,x,rand()%10+10);
            break;
        
        default:
            break;
        }
        /* code */
    }
}


Item * create_sword(int y, int x, int attack) {
    int random_name;
    Item * item;
    item = malloc(sizeof(Item));
    random_name = rand() % 20;
    item->type = WEAPON_TYPE;
    item->item.weapon = malloc(sizeof(Weapon));
    item->item.weapon->attack = attack;
    item->position = malloc(sizeof(Position));
    item->position->y = y;
    item->position->x = x;
    item->color = WHITE;
    item->collected = 0;
    item->symbol = ')';
    strcpy(item->name,weapon_names[random_name]);
    sprintf(wtomsg,"Adding Weapon y:%d,x:%d attack: %d '%s'",
            y,x,attack,item->name);
    write_op();
    return item;
}

Item * create_potion(int y, int x, int health) {
    int random_name;
    Item * item;
    item = malloc(sizeof(Item));
    random_name = rand() % 10;
    item->type = POTION_TYPE;
    strcpy(item->name,potion_names[random_name]);
    item->item.potion = malloc(sizeof(Potion));
    item->item.potion->health = health;
    item->position = malloc(sizeof(Position));
    item->position->y = y;
    item->position->x = x;
    item->collected = 0;
    item->color = PINK;
    item->symbol = '!';
    sprintf(wtomsg,"Adding Potion y:%d,x:%d heals: %d '%s'",
            y,x,health,item->name);
    write_op();
    return item;
}

Item * create_armor(int y, int x, int armor) {
    int random_name;
    Item * item;
    item = malloc(sizeof(Item));
    random_name = rand() % 20;
    item->type = ARMOR_TYPE;
    strcpy(item->name,armor_names[random_name]);
    item->item.armor = malloc(sizeof(Armor));
    item->item.armor->defense = armor;
    item->position = malloc(sizeof(Position));
    item->position->y = y;
    item->position->x = x;
    item->collected = 0;
    item->color = BLUE;
    item->symbol = '[';
    sprintf(wtomsg,"Adding Potion y:%d,x:%d armor: %d '%s'",
            y,x,armor,item->name);
    write_op();
    return item;
}

