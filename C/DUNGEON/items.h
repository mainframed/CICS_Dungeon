const char* weapon_names[] = {
  "Nunchucks of Power",
  "Fakescalibur",
  "Grimm Hammer",
  "Razor Song",
  "Soulless Scimitar",
  "Pointed Stick",
  "Excalibur",
  "Durendal",
  "Grieving Blade",
  "Unholy Cleaver",
  "System 370 Operators Reference Guide",
  "Liar's Boomstick",
  "Assassination Scythe",
  "Sinister Obsidian Reaper",
  "Staff of Sacrifice",
  "Kumorigachi",
  "Thundersoul",
  "Jawbreaker",
  "Axe of Joy",
  "Dawnbreaker",
  "Big Sais",
  "Doomblade"
};

const char* armor_names[] = {
"Undead Blockade",
"Eternal Copper Carapace",
"Savage Iron Defender",
"Soulless Mithril Guardian",
"Judgement Barrier",
"Vindicator Shield",
"Ivory Guard",
"Renewed Iron Gloves",
"Gloves of Demonic Protection",
"Fusion Linen Handwraps",
"Thunder Silk Cloak",
"Reincarnated Shroud of Kings",
"Roaring Cloak of Assassination",
"Proud Mantle of the Talon",
"Wrap of Ancient Glory",
"Warrior Cloak of the Storm",
"Conqueror's Chain Chestpiece",
"Breastplate of the Boar",
"Prime Bronze Tunic",
"Chain Chestguard of Fortune"
};

const char* potion_names[] = {
"Flask of Cure Disease",
"Elixir of Health",
"Vial of Cure Wounds",
"Flask of Holy Water",
"Brew of the Sun",
"Potion of Salvation",
"Elixir of Excitement",
"Tonic of the High Mage",
"Potion of Comfort",
"Draught of Healing",
"Elixir of Peace of Mind"
};

void add_items(Level * level);
Item * create_sword(int y, int x, int attack);
Item * create_potion(int y, int x, int health);
Item * create_armor(int y, int x, int armor);
