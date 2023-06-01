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
class Map;
class Projectile;
class PseudoSkill;
class Quest;
class Save;
class Skill;
class Speech;
class Tile;
class Way;
class World;
class Item;
class GearItem;
class ArmorItem;
class WeaponItem;
class SerialItem;
class AmmunitionItem;
typedef struct CharacterDisplay CharacterDisplay;
typedef struct Loot Loot;
typedef struct MapLink MapLink;
typedef struct ProjectileDisplay ProjectileDisplay;
typedef struct Spawn Spawn;
typedef struct StateDisplay StateDisplay;
typedef struct Step Step;
typedef struct Target Target;

// communication

class SpeechManager;

// effect_duration_type

#define INSTANT_DURATION 1
#define INFINITE_DURATION 2
#define TEMPORARY_DURATION 3

// effect_type

#define HP 1
#define MANA 2
#define HP_MAX 3
#define MANA_MAX 4
#define STAMINA 5
#define SATIETY 6
#define ARMOR 7
#define ARMOR_MULTIPLIER 8
#define SOULBURNTRESHOLD 9
#define FLOW 10
#define VISION_RANGE 11
#define VISION_POWER 12
#define DETECTION_RANGE 13
#define EXPERIENCE 14
#define DAMAGE 15
#define DAMAGE_BUFF 16
#define DAMAGE_REDUCTION 17
#define DAMAGE_MULTIPLIER 18
#define CHANNELING 19
#define STUNNED 20
#define SLEEPING 21
#define FLY 22
#define LIGHT 23
#define CLOAKED 24
#define INVISIBLE 25
#define ETHERAL 26
#define INVULNERABLE 27
#define ACTION_TIME_MODIFIER 28
#define HAND_ACTION_TIME_MODIFIER 29
#define SKILL_TIME_MODIFIER 30
#define MOVEMENT_TIME_MODIFIER 31


// item_type

#define ITEM_WEAPON 1
#define ITEM_ARMOR 2
#define ITEM_MISCELLANEOUS 3
#define ITEM_CONSUMABLE 4
#define ITEM_AMMUNITION 5

// weapon_type

#define ITEM_LONG_SWORD 1
#define ITEM_SHORT_SWORD 2
#define ITEM_CURVED_SWORD 3
#define ITEM_RAPIER 4
#define ITEM_SPEAR 5
#define ITEM_GLAIVE 6
#define ITEM_AXE 7
#define ITEM_WARHAMMER 8
#define ITEM_DAGGER 9
#define ITEM_FIST 10
#define ITEM_BOW 11
#define ITEM_CROSSBOW 12
#define ITEM_MAGIC_STAFF 13
#define ITEM_RIFLE 14
#define ITEM_PISTOL 15
#define ITEM_SLING 16

// armor_type

#define ITEM_HEAD 1
#define ITEM_ARMS 2
#define ITEM_BODY 3
#define ITEM_LEGS 4
#define ITEM_LANTERN 5
#define ITEM_FULL_BODY 6
#define ITEM_RING 7
#define ITEM_AMULET 8
#define ITEM_LEFT_RING 9
#define ITEM_RIGHT_RING 10

// other_items_type

#define ITEM_FOOD 1
#define ITEM_MATERIAL 2
#define ITEM_POTION 3

// event_type

#define CALAMITY 1
#define BLESSING 2

// character_apparition_type

#define SOFT 1
#define HARD 2

// movement_type

#define FLYING 1
#define WALKING 2
#define TELEPORT 3

// target_type

#define NO_TARGET 0
#define SELF 1
#define CHARACTER 2
#define COORDINATES 3
#define TILE 4
#define PERMISSIVE_TARGET 5
#define MULTIPLE_TARGETS 6

// skill_type

#define CHANNELED_SKILL 1
#define MAP_LINKER_SKILL 2
#define MIND_CONTROL_SKILL 3
#define PROJECTILE_SKILL 4
#define RESURRECTION_SKILL 5
#define SIMPLE_SKILL 6
#define SUMMON_SKILL 7
#define TEAM_CHANGER_SKILL 8
#define TELEPORT_SKILL 9
#define TILE_SWAP_SKILL 10

// skill_overcharge_type

#define NO_OVERCHARGE 0
#define LINEAR 1
#define SQUARE 2
#define EXPONENTIAL 3

// map_link type

#define THROUGH 1
#define BOUNCE 2
#define BACK 3

// projectile_type

#define PROJECTILE_FIRE_FORM 1
#define PROJECTILE_LIGHTNING_FORM 2
#define PROJECTILE_AIR 3
#define PROJECTILE_ARROW 4
#define PROJECTILE_BULLET 5
#define PROJECTILE_SPECIAL 6

// character_type

#define HOMO 1
#define UNDEAD 2
#define NATURE 3
#define OMONCULUS 4
#define WALL 5

// damage_type

#define DAMAGE_TYPE_NUMBER 11

#define SLASH_DAMAGE 0
#define PUNCTURE_DAMAGE 1
#define IMPACT_DAMAGE 2
#define FIRE_DAMAGE 3
#define LIGHTNING_DAMAGE 4
#define FROST_DAMAGE 5
#define POISON_DAMAGE 6
#define ACID_DAMAGE 7 // unaffected by damage reductions, but still by armor
#define MIND_DAMAGE 8 // unaffected by armor, but still by damage reductions
#define TRUE_DAMAGE 9 // health damage
#define SOUL_DAMAGE 10 // mana damage

// loot_type

#define CORPSE 1
#define TREASURE 2

// grab_mode

#define ALL 0
#define FOOD 1

// quest_step_type

#define SLAY 1
#define OBTAIN_ITEM 2
#define DISCOVER 3
#define TALK 4

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

#define IDLE 0
#define MOVE 1
#define STRIKE 2
#define HEAVY_STRIKE 3
#define RESPITE 4
#define SHOOT 5
#define RELOAD 6
#define GRAB 7
#define SWAP_GEAR 8
#define USE_ITEM 9
#define USE_SKILL 10
#define REST 11
#define TALKING 12
#define ECONOMICS 13
#define BREAKPOINT 14

// teams relations

#define SAME 1
#define ALLY 2
#define NEUTRAL 3
#define ENEMY 4

#endif // _VALUES_H_
