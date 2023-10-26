
int combat(Level * level, Monster * monster, int order) {

    int damage;

    sprintf(wtomsg,"Fight! '%s' vs '%s'",level->player->name,monster->name);
    write_op();

    sprintf(wtomsg,"Player Health: %d Monster Health: %d",
            level->player->hp,monster->health);
    write_op();
    if (order = 1)
    {
        /* player attacking */
        sprintf(wtomsg,"Player attacking");
        write_op();

        monster->health -= level->player->atk;
        if (monster->health > 0)
        {
         level->player->hp -= monster->attack;

         if(level->player->hp <=0) 
         {
         dead_player(level,monster->name);
         }
         sprintf(level->message,"%s took %d damage and attacked for %d damage",
                 monster->name,level->player->atk,monster->attack);
        } 
        else 
        {
            kill_monster(monster);
            level->player->xp++;
            sprintf(level->message,"You killed the %s", monster->name);
        }

        
    } else {
        
        /* Monster attacking */
        sprintf(wtomsg,"Monster attacking");
        write_op();

        if (monster->attack >= level->player->def) {
            damage = monster->attack * 2 - level->player->def;
        } else {
            damage = monster->attack * monster->attack / level->player->def;
        }

        level->player->hp -= damage;

        if (level->player->hp > 0)
        {
           monster->health -= level->player->atk; 
        }
        else 
        {
            dead_player(level,monster->name);
        }

         sprintf(level->message,
            "%s attacked for %d damage and you retaliated for %d damage",
            monster->name,monster->attack, level->player->atk);
    }
    
    sprintf(wtomsg,"Player Health: %d Monster Health: %d",
            level->player->hp,monster->health);
    write_op();
    return 1;

}
