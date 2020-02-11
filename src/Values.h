#ifndef _VALUES_H_
#define _VALUES_H_

// resolve cyclic mess

// data

class Adventure;
class Attributes;
class Character;
class Effect;
class Event;
class Gear;
class Item;
class Map;
class Projectile;
class Quest;
class Save;
class Skill;
class Speech;
class Tile;
class Weapon;
class Way;
class World;
typedef struct MapLink MapLink;
typedef struct Step Step;
typedef struct Loot Loot;

// communication

class SpeechManager;

// effect_duration_type

#define INSTANT 1
#define INFINITE 2
#define TEMPORARY 3

// effect_type

#define HP 1
#define MANA 2
#define HP_MAX 3
#define MANA_MAX 4
#define ARMOR 5
#define SOULBURNTRESHOLD 6
#define FLOW 7
#define VISION 8
#define DARK_VISION 9
#define DAMAGE 10
#define CHANNELING 11
#define STUNNED 12
#define CLOAKED 13
#define INVISIBLE 14
#define SLEEPING 15

// melee_type

#define LONG_SWORD 1
#define SHORT_SWORD 2
#define CURVED_SWORD 3
#define RAPIER 4
#define SPEAR 5
#define BLADED_SPEAR 6
#define AXE 7
#define WARHAMMER 8
#define DAGGER 9

// melee_range

#define SHORT 1
#define STANDARD 2
#define LONG 3

// range_type

#define BOW 1
#define CROSSBOW 2
#define MAGIC_STAFF 3
#define RIFLE 4
#define PISTOL 5
#define SLING 6

// weight

#define LIGHT 1
#define MEDIUM 2
#define HEAVY 3

// item_type

#define UNEQUIPABLE 0
#define HEAD 1
#define ARMS 2
#define BODY 3
#define LEGS 4
#define FULL_BODY 5
#define RING 6
#define AMULET 7

#define LEFT_RING 8
#define RIGHT_RING 9

// event_type

#define CALAMITY 1
#define BLESSING 2

// character_apparition_type

#define SOFT 1
#define HARD 2

// skill_target

#define SELF 1
#define TOUCH 2
#define TARGET 3
#define TILE 4

// skill_cast_type

#define ACTIVE 1
#define TOGGLE 2

// skill_special

#define TIMESTOP 1
#define TILESWAP 2
#define TELEPORT 3
#define MAPLINKER 4
#define TEAMCHANGER 5
#define MINDCONTROL 6
#define PROJECTILE 7
#define SUMMON 8
#define RESURRECTION 9

// skill_overcharge_type

#define NO_OVERCHARGE 0
#define LINEAR 1
#define EXPONENTIAL 2

// orientations

#define NORTH 0
#define NORTH_EAST 1
#define EAST 2
#define SOUTH_EAST 3
#define SOUTH 4
#define SOUTH_WEST 5
#define WEST 6
#define NORTH_WEST 7
#define NO_ORIENTATION 8

// projectile_type

#define FIRE_FORM 1
#define THUNDER_FORM 2
#define AIR 3
#define ARROW 4
#define BULLET 5
#define SPECIAL 6

// target_type

#define CHARACTER 1
#define TILE 2
#define MULTIPLE_CHARACTERS 3
#define MULTIPLE_TILES 4

// character_type

#define HUMAN 1
#define UNDEAD 2
#define NATURE 3
#define OMONCULUS 4
#define WALL 5

// damage_type

#define DAMAGE_TYPE_NUMBER 10

#define SLASH 0
#define PUNCTURE 1
#define IMPACT 2
#define FIRE 3
#define THUNDER 4
#define COLD 5
#define POISON 6
#define NEUTRAL 7 // unaffected by damage reductions, but still by armor
#define TRUE 8
#define SOUL 9 // true_damage + mana_damage

// loot_type

#define CORPSE 1
#define TREASURE 2

// quest_step_type

#define SLAY 1
#define OBTAIN_ITEM 2
#define OBTAIN_WEAPON 3
#define DISCOVER 4
#define TALK 5

// way_type

#define RELIGION 1
#define ORIGIN 2
#define RACE 3

// speech_type

#define WARNING 1
#define ERROR 2

#endif // _VALUES_H_
