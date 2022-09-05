#ifndef _VALUES_H_
#define _VALUES_H_

// resolve cyclic mess

// AI

class AI;
class PassiveAI;
class PlayerAI;

// data

class Action;
class Adventure;
class Attributes;
class Character;
class Database;
class Effect;
class Event;
class Gear;
class Item;
class Map;
class Projectile;
class PseudoSkill;
class Quest;
class Save;
class Skill;
class Speech;
class Tile;
class Way;
class Weapon;
class World;
typedef struct Ammunition Ammunition;
typedef struct CharacterDisplay CharacterDisplay;
typedef struct Loot Loot;
typedef struct MapDisplay MapDisplay;
typedef struct MapLink MapLink;
typedef struct ProjectileDisplay ProjectileDisplay;
typedef struct Spawn Spawn;
typedef struct Step Step;

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
#define STAMINA 5
#define SATIETY 6
#define ARMOR 7
#define SOULBURNTRESHOLD 8
#define FLOW 9
#define VISION_RANGE 10
#define VISION_POWER 11
#define DETECTION_RANGE 12
#define EXPERIENCE 13
#define DAMAGE 14
#define DAMAGE_BUFF 15
#define DAMAGE_REDUCTION 16
#define DAMAGE_MULTIPLIER 17
#define CHANNELING 18
#define STUNNED 19
#define SLEEPING 20
#define CLOAKED 21
#define INVISIBLE 22
#define ETHERAL 23
#define INVULNERABLE 24

// weapon_type

#define LONG_SWORD 1
#define SHORT_SWORD 2
#define CURVED_SWORD 3
#define RAPIER 4
#define SPEAR 5
#define GLAIVE 6
#define AXE 7
#define WARHAMMER 8
#define DAGGER 9
#define BOW 10
#define CROSSBOW 11
#define MAGIC_STAFF 12
#define RIFLE 13
#define PISTOL 14
#define SLING 15

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
#define TARGET 2
#define TILE 3

// skill_type

#define CHANNELED_SKILL 1
#define INSTANT_SKILL 2
#define MAP_LINKER_SKILL 3
#define MIND_CONTROL_SKILL 4
#define PROJECTILE_SKILL 5
#define RESURRECTION_SKILL 6
#define SIMPLE_SKILL 7
#define SUMMON_SKILL 8
#define TEAM_CHANGER_SKILL 9
#define TELEPORT_SKILL 10
#define TILE_SWAP_SKILL 11

// skill_overcharge_type

#define NO_OVERCHARGE 0
#define LINEAR 1
#define SQUARE 2
#define EXPONENTIAL 3

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
#define LIGHTNING_FORM 2
#define AIR 3
#define ARROW 4
#define BULLET 5
#define SPECIAL 6

// character_type

#define HUMAN 1
#define UNDEAD 2
#define NATURE 3
#define OMONCULUS 4
#define WALL 5

// damage_type

#define DAMAGE_TYPE_NUMBER 10

#define SLASH_DAMAGE 0
#define PUNCTURE_DAMAGE 1
#define IMPACT_DAMAGE 2
#define FIRE_DAMAGE 3
#define LIGHTNING_DAMAGE 4
#define COLD_DAMAGE 5
#define POISON_DAMAGE 6
#define NEUTRAL_DAMAGE 7 // unaffected by damage reductions, but still by armor
#define TRUE_DAMAGE 8
#define SOUL_DAMAGE 9 // true_damage + mana_damage

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

#define RACE 1
#define ORIGIN 2
#define CULTURE 3
#define RELIGION 4
#define PROFESSION 5
#define TITLE 6

// speech_type

#define MSG_TEXT 1
#define MSG_VOCAL 2
#define MSG_LOG 3
#define MSG_WARNING 4
#define MSG_ERROR 5

// object_type

#define ITEM 1
#define WEAPON 2
#define AMMUNITION 3
#define SKILL 4
#define EFFECT 5

// action_type

#define MOVE 1
#define REST 2
#define SHOOT 3
#define FORCE_STRIKE 4
#define RELOAD 5
#define SWAP_GEAR 6
#define CHANGE_MAP 7
#define GRAB 8
#define USE_SKILL 9
#define USE_ITEM 10
#define TALKING 11
#define ECONOMICS 12

// teams relations

#define SAME 1
#define ALLY 2
#define NEUTRAL 3
#define ENEMY 4

#endif // _VALUES_H_
