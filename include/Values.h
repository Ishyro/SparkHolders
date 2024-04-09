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
class Region;
class Save;
class Skill;
class Speech;
class Block;
class BlocksChunk;
class Way;
class Race;
class World;
class Item;
class BasicItem;
class GearItem;
class ArmorItem;
class WeaponItem;
class SerialItem;
class AmmunitionItem;
class ContainerItem;
class Furniture;
class ActivableFurniture;
class ContainerFurniture;
class CraftingFurniture;
class LinkedFurniture;
class SkillFurniture;
class SwitchFurniture;
typedef struct CharacterDisplay CharacterDisplay;
typedef struct ItemSlot ItemSlot;
typedef struct Loot Loot;
typedef struct MapLink MapLink;
typedef struct ProjectileDisplay ProjectileDisplay;
typedef struct Spawn Spawn;
typedef struct StateDisplay StateDisplay;
typedef struct Step Step;
typedef struct Target Target;
typedef struct Time Time;

// communication

class SpeechManager;

// block_type

#define BLOCK_SOLID 1
#define BLOCK_LIQUID 2
#define BLOCK_GAS 3
#define BLOCK_STAIRS 4
#define BLOCK_SLOPE 5

// lightening_type

#define LIGHTENING_NO 0
#define LIGHTENING_INDOORS 1
#define LIGHTENING_OUTDOORS 2
#define LIGHTENING_DARK 3

// effect_duration_type

#define DURATION_INSTANT 1
#define DURATION_INFINITE 2
#define DURATION_TEMPORARY 3

// effect_type

#define EFFECT_HP 1
#define EFFECT_MANA 2
#define EFFECT_HP_MAX 3
#define EFFECT_MANA_MAX 4
#define EFFECT_SHIELD 5
#define EFFECT_SHIELD_MAX 6
#define EFFECT_STAMINA 7
#define EFFECT_SATIETY 8
#define EFFECT_SHIELD_MULTIPLIER 9
#define EFFECT_SOULBURNTRESHOLD 10 
#define EFFECT_FLOW 11 
#define EFFECT_VISION_RANGE 12 
#define EFFECT_VISION_POWER 13 
#define EFFECT_DETECTION_RANGE 14 
#define EFFECT_EXPERIENCE 15 
#define EFFECT_DAMAGE 16 
#define EFFECT_DAMAGE_BUFF 17 
#define EFFECT_DAMAGE_REDUCTION 18 
#define EFFECT_DAMAGE_MULTIPLIER 19 
#define EFFECT_CHANNELING 20 
#define EFFECT_STUNNED 21 
#define EFFECT_SLEEPING 22 
#define EFFECT_FLY 23 
#define EFFECT_LIGHT 24 
#define EFFECT_CLOAKED 25 
#define EFFECT_INVISIBLE 26 
#define EFFECT_ETHERAL 27 
#define EFFECT_INVULNERABLE 28 
#define EFFECT_ACTION_TIME_MODIFIER 29 
#define EFFECT_HAND_ACTION_TIME_MODIFIER 30 
#define EFFECT_SKILL_TIME_MODIFIER 31 
#define EFFECT_MOVEMENT_TIME_MODIFIER 32


// item_type

#define ITEM_WEAPON 1
#define ITEM_ARMOR 2
#define ITEM_BASIC 3
#define ITEM_SERIAL 4
#define ITEM_AMMUNITION 5
#define ITEM_CONTAINER 6

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

#define ITEM_MANTLE 1
#define ITEM_HELMET 2
#define ITEM_GAUNTLETS 3
#define ITEM_CUIRASS 4
#define ITEM_BOOTS 5
#define ITEM_LANTERN 6
#define ITEM_AMULET 7
#define ITEM_RING 8

// item_slot_type

#define ITEM_SLOT_MANTLE 1
#define ITEM_SLOT_HELMET 2
#define ITEM_SLOT_GAUNTLETS 3
#define ITEM_SLOT_ARMOR 4
#define ITEM_SLOT_BOOTS 5
#define ITEM_SLOT_LANTERN 6
#define ITEM_SLOT_AMULET 7
#define ITEM_SLOT_RING_1 8
#define ITEM_SLOT_RING_2 9
#define ITEM_SLOT_WEAPON_1 10
#define ITEM_SLOT_WEAPON_2 11
#define ITEM_SLOT_WEAPON_3 12
#define ITEM_SLOT_WEAPON_4 13
#define ITEM_SLOT_BAG 14
#define ITEM_SLOT_BELT 15
#define ITEM_SLOT_INSIDE_BAG 16
#define ITEM_SLOT_INSIDE_BELT 17

// other_items_type

#define ITEM_FOOD 1
#define ITEM_MATERIAL 2
#define ITEM_POTION 3
#define ITEM_BAG 4
#define ITEM_BELT 5
#define ITEM_POUCH 6
#define ITEM_MAGIC_BOX 7

// furniture_type

#define FURNITURE_BASIC 1
#define FURNITURE_CONTAINER 2
#define FURNITURE_CRAFTING 3
#define FURNITURE_LINKED 4
#define FURNITURE_SWITCH 5
#define FURNITURE_SKILL 6

// event_type

#define EVENT_CALAMITY 1
#define EVENT_BLESSING 2

// character_apparition_type

#define APPARITION_SOFT 1
#define APPARITION_HARD 2

// movement_type

#define MOVEMENT_FLYING 1
#define MOVEMENT_WALKING 2
#define MOVEMENT_TELEPORT 3

// target_type

#define TARGET_NONE 0
#define TARGET_SELF 1
#define TARGET_CHARACTER 2
#define TARGET_COORDINATES 3
#define TARGET_BLOCK 4
#define TARGET_PERMISSIVE 5
#define TARGET_MULTIPLE 6

// skill_type

#define SKILL_CHANNELED 1
#define SKILL_MAP_LINKER 2
#define SKILL_MIND_CONTROL 3
#define SKILL_PROJECTILE 4
#define SKILL_RESURRECTION 5
#define SKILL_SIMPLE 6
#define SKILL_SUMMON 7
#define SKILL_TEAM_CHANGER 8
#define SKILL_TELEPORT 9
#define SKILL_TILE_SWAP 10

// skill_overcharge_type

#define OVERCHARGE_NONE 0
#define OVERCHARGE_LINEAR 1
#define OVERCHARGE_SQUARE 2
#define OVERCHARGE_EXPONENTIAL 3

// map_link type

#define MAPLINK_THROUGH 1
#define MAPLINK_BOUNCE 2
#define MAPLINK_BACK 3

// projectile_type

#define PROJECTILE_FIRE_FORM 1
#define PROJECTILE_LIGHTNING_FORM 2
#define PROJECTILE_AIR 3
#define PROJECTILE_ARROW 4
#define PROJECTILE_BULLET 5
#define PROJECTILE_SPECIAL 6

// race_type

#define RACE_NONE 0
#define RACE_NATURE 1
#define RACE_HOMO 2
#define RACE_OMONCULUS 3
#define RACE_MECANICUS 4
#define RACE_UNDEAD 5
#define RACE_WALL 6
#define RACE_GOD 7

// damage_type

#define DAMAGE_TYPE_NUMBER 11

#define DAMAGE_SLASH 0
#define DAMAGE_PUNCTURE 1
#define DAMAGE_BLUNT 2
#define DAMAGE_FIRE 3
#define DAMAGE_LIGHTNING 4
#define DAMAGE_FROST 5
#define DAMAGE_POISON 6
#define DAMAGE_ACID 7 // unaffected by damage reductions, but still by shield
#define DAMAGE_MIND 8 // unaffected by shield, but still by damage reductions
#define DAMAGE_TRUE 9 // health damage
#define DAMAGE_SOUL 10 // mana damage

// loot_type

#define LOOT_CORPSE 1
#define LOOT_TREASURE 2

// grab_mode

#define GRAB_ALL 0
#define GRAB_FOOD 1

// quest_step_type

#define QUEST_SLAY 1
#define QUEST_OBTAIN 2
#define QUEST_DISCOVER 3
#define QUEST_TALK 4

// way_type

#define WAY_CLASS 1
#define WAY_RACE 2
#define WAY_ORIGIN 3
#define WAY_CULTURE 4
#define WAY_RELIGION 5
#define WAY_PROFESSION 6
#define WAY_RACE_MODIFIER 7
#define WAY_TITLE 8

// speech_type

#define SPEECH_TEXT 1
#define SPEECH_VOCAL 2
#define SPEECH_LOG 3
#define SPEECH_WARNING 4
#define SPEECH_ERROR 5

// object_type

#define OBJECT_ITEM 1
#define OBJECT_SKILL 2
#define OBJECT_EFFECT 3

// action_type

#define ACTION_IDLE 0
#define ACTION_MOVE 1
#define ACTION_STRIKE 2
#define ACTION_RESPITE 3
#define ACTION_ACTIVATION 4
#define ACTION_CHANNEL 5
#define ACTION_USE_SKILL 6
#define ACTION_USE_ITEM 7
#define ACTION_RELOAD 8
#define ACTION_GRAB 9
#define ACTION_SWAP_GEAR 10
#define ACTION_REST 11
#define ACTION_TALKING 12
#define ACTION_ECONOMICS 13
#define ACTION_BREAKPOINT 14

// teams relations

#define TEAM_SAME 1
#define TEAM_ALLY 2
#define TEAM_NEUTRAL 3
#define TEAM_ENEMY 4

// window_type

#define WINDOW_FULLSCREEN 1
#define WINDOW_WINDOW 2
#define WINDOW_BORDERLESS 3

// socket_msg_type

#define SOCKET_MSG_CONNECT 1
#define SOCKET_MSG_RECONNECT 2
#define SOCKET_MSG_ADVENTURE 3
#define SOCKET_MSG_CHOICE 4
#define SOCKET_MSG_READY 5
#define SOCKET_MSG_CHARACTER 6
#define SOCKET_MSG_ACTION 7
#define SOCKET_MSG_STATE 8
#define SOCKET_MSG_SWITCH 9 

#endif // _VALUES_H_
