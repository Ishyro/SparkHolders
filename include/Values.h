#ifndef _VALUES_H_
#define _VALUES_H_

#include <cstdint>

// resolve cyclic mess

// AI

class AI;
class PassiveAI;
class PlayerAI;

// data
class Action;
class Adventure;
class Attributes;
class Biome;
class Block;
class BlocksChunk;
class Character;
class Database;
class Effect;
class Event;
class Gear;
class Projectile;
class PseudoSkill;
class Quest;
class Region;
class Save;
class Skill;
class Speech;
class Stance;
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
struct CharacterDisplay;
struct ItemSlot;
struct Loot;
struct MapLink;
struct ProjectileDisplay;
struct Shield;
struct Spawn;
struct StateDisplay;
struct Step;
struct Time;

namespace MathUtil {
  struct Vector3;
  struct Vector3i;
  struct Coords;
  struct Hitbox;
  struct Target;
}

// communication

class SpeechManager;

// special_constants

#define CHUNK_SIZE 16

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

// side_position

#define EAST 0
#define NORTH 1
#define WEST 2
#define SOUTH 3
#define UP 4
#define DOWN 5

// side_type

#define SIDE_OPEN 0
#define SIDE_OPEN_Z1 1
#define SIDE_OPEN_Z2 2
#define SIDE_OPEN_Z3 3
#define SIDE_OPEN_Z4 4
#define SIDE_OPEN_Z5 5
#define SIDE_OPEN_Z6 6
#define SIDE_OPEN_Z7 7
#define SIDE_OPEN_Z8 8
#define SIDE_OPEN_Z9 9
#define SIDE_OPEN_Z10 10
#define SIDE_OPEN_Z11 11
#define SIDE_OPEN_Z12 12
#define SIDE_OPEN_Z13 13
#define SIDE_OPEN_Z14 14
#define SIDE_OPEN_Z15 15
#define SIDE_CLOSED 100

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
#define EFFECT_HUNGER 7
#define EFFECT_THIRST 8
#define EFFECT_STAMINA 9
#define EFFECT_SANITY 10
#define EFFECT_SHIELD_MULTIPLIER 11
#define EFFECT_SOULBURNTRESHOLD 12
#define EFFECT_FLOW 13
#define EFFECT_VISION_RANGE 14
#define EFFECT_VISION_POWER 15
#define EFFECT_DETECTION_RANGE 16
#define EFFECT_EXPERIENCE 17
#define EFFECT_DAMAGE 18
#define EFFECT_DAMAGE_BUFF 19
#define EFFECT_DAMAGE_REDUCTION 20
#define EFFECT_DAMAGE_MULTIPLIER 21
#define EFFECT_STATUS_POWER 22
#define EFFECT_STATUS_REDUCTION 23
#define EFFECT_CHANNELING 24
#define EFFECT_SLEEPING 25
#define EFFECT_FLY 26
#define EFFECT_STUNNED 27
#define EFFECT_LIGHT 28
#define EFFECT_CLOAKED 29
#define EFFECT_INVISIBLE 30
#define EFFECT_ETHERAL 31
#define EFFECT_INVULNERABLE 32
#define EFFECT_ACTION_TIME_MODIFIER 33
#define EFFECT_HAND_ACTION_TIME_MODIFIER 34
#define EFFECT_SKILL_TIME_MODIFIER 35
#define EFFECT_MOVEMENT_TIME_MODIFIER 36

//status_type

#define EFFECT_STATUS_BLEEDING 101
#define EFFECT_STATUS_WEAKENED 102
#define EFFECT_STATUS_CONFUSED 103
#define EFFECT_STATUS_BURNING 104
#define EFFECT_STATUS_FROZEN 105
#define EFFECT_STATUS_SHOCKED 106
#define EFFECT_STATUS_POISONED 107
#define EFFECT_STATUS_CORRODED 108
#define EFFECT_STATUS_BROKEN 109
#define EFFECT_STATUS_DISINTEGRATED 110

#define EFFECT_STATUS_IMMUNITY_BLEEDING 121
#define EFFECT_STATUS_IMMUNITY_WEAKENED 122
#define EFFECT_STATUS_IMMUNITY_CONFUSED 123
#define EFFECT_STATUS_IMMUNITY_BURNING 124
#define EFFECT_STATUS_IMMUNITY_FROZEN 125
#define EFFECT_STATUS_IMMUNITY_SHOCKED 126
#define EFFECT_STATUS_IMMUNITY_POISONED 127
#define EFFECT_STATUS_IMMUNITY_CORRODED 128
#define EFFECT_STATUS_IMMUNITY_BROKEN 129
#define EFFECT_STATUS_IMMUNITY_DISINTEGRATED 130

// item_type

#define ITEM_WEAPON 1
#define ITEM_ARMOR 2
#define ITEM_BASIC 3
#define ITEM_SERIAL 4
#define ITEM_AMMUNITION 5
#define ITEM_CONTAINER 6

// weapon_type
// use prime numbers only, because combinations are computed
// by multipling weapon1->subtype with weapon2->subtype
#define ITEM_HAND 2
#define ITEM_LONG_SWORD 3
#define ITEM_SHORT_SWORD 5
#define ITEM_CURVED_SWORD 7
#define ITEM_RAPIER 11
#define ITEM_DAGGER 13
#define ITEM_AXE 17
#define ITEM_CLUB 19
#define ITEM_SPEAR 23
#define ITEM_PISTOL 29
#define ITEM_SHIELD 31
#define ITEM_ZWEIHANDER 37
#define ITEM_ZWEICURVED 41
#define ITEM_WARAXE 43
#define ITEM_HEAVY_SPEAR 47
#define ITEM_GLAIVE 53
#define ITEM_WARHAMMER 59
#define ITEM_BOW 61
#define ITEM_CROSSBOW 67
#define ITEM_MAGIC_STAFF 71
#define ITEM_RIFLE 73

// combination_weapon_type
#define ITEM_COMBO_DUAL_WELDING 9
#define ITEM_COMBO_LONG_AND_SHORT 15
#define ITEM_COMBO_LONG_AND_CURVED 21
#define ITEM_COMBO_LONG_AND_RAPIER 33
#define ITEM_COMBO_LONG_AND_DAGGER 39
#define ITEM_COMBO_LONG_AND_AXE 51
#define ITEM_COMBO_LONG_AND_CLUB 57
#define ITEM_COMBO_LONG_AND_SPEAR 69
#define ITEM_COMBO_LONG_AND_PISTOL 87
#define ITEM_COMBO_LONG_AND_SHIELD 93

#define ITEM_COMBO_DUAL_SHORT 25
#define ITEM_COMBO_CURVED_AND_SHORT 35
#define ITEM_COMBO_RAPIER_AND_SHORT 55
#define ITEM_COMBO_SHORT_AND_DAGGER 65
#define ITEM_COMBO_SHORT_AND_AXE 85
#define ITEM_COMBO_SHORT_AND_CLUB 95
#define ITEM_COMBO_SPEAR_AND_SHORT 115
#define ITEM_COMBO_SHORT_AND_PISTOL 145
#define ITEM_COMBO_SHORT_AND_SHIELD 155

#define ITEM_COMBO_DUAL_CURVED 49
#define ITEM_COMBO_CURVED_AND_RAPIER 77
#define ITEM_COMBO_CURVED_AND_DAGGER 91
#define ITEM_COMBO_CURVED_AND_AXE 119
#define ITEM_COMBO_CURVED_AND_CLUB 133
#define ITEM_COMBO_CURVED_AND_SPEAR 161
#define ITEM_COMBO_CURVED_AND_PISTOL 203
#define ITEM_COMBO_CURVED_AND_SHIELD 217

#define ITEM_COMBO_DUAL_RAPIER 121
#define ITEM_COMBO_RAPIER_AND_DAGGER 143
#define ITEM_COMBO_RAPIER_AND_AXE 187
#define ITEM_COMBO_RAPIER_AND_CLUB 209
#define ITEM_COMBO_RAPIER_AND_SPEAR 253
#define ITEM_COMBO_RAPIER_AND_PISTOL 319
#define ITEM_COMBO_RAPIER_AND_SHIELD 341

#define ITEM_COMBO_DUAL_DAGGER 169
#define ITEM_COMBO_AXE_AND_DAGGER 221
#define ITEM_COMBO_CLUB_AND_DAGGER 247
#define ITEM_COMBO_SPEAR_AND_DAGGER 299
#define ITEM_COMBO_DAGGER_AND_PISTOL 37
#define ITEM_COMBO_DAGGER_AND_SHIELD 403

#define ITEM_COMBO_DUAL_AXE 289
#define ITEM_COMBO_AXE_AND_CLUB 323
#define ITEM_COMBO_AXE_AND_SPEAR 391
#define ITEM_COMBO_AXE_AND_PISTOL 493
#define ITEM_COMBO_AXE_AND_SHIELD 527

#define ITEM_COMBO_DUAL_CLUB 361
#define ITEM_COMBO_CLUB_AND_SPEAR 437
#define ITEM_COMBO_CLUB_AND_PISTOL 551
#define ITEM_COMBO_CLUB_AND_SHIELD 589

#define ITEM_COMBO_DUAL_SPEAR 529
#define ITEM_COMBO_SPEAR_AND_PISTOL 667
#define ITEM_COMBO_SPEAR_AND_SHIELD 713

#define ITEM_COMBO_GUNSLINGER 841
#define ITEM_COMBO_PISTOL_AND_SHIELD 899

#define ITEM_COMBO_WALL 961

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
#define ITEM_SLOT_BAG 13
#define ITEM_SLOT_INSIDE_BASE_INVENTORY 14
#define ITEM_SLOT_INSIDE_BAG 15

// other_items_type

#define ITEM_FOOD 1
#define ITEM_MATERIAL 2
#define ITEM_POTION 3
#define ITEM_BAG 4
#define ITEM_BASE_INVENTORY 5
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
#define TARGET_FRONT 1
#define TARGET_SELF 2
#define TARGET_CHARACTER 3
#define TARGET_FURNITURE 4
#define TARGET_COORDINATES 5
#define TARGET_MULTIPLE 6

// hitbox_type

#define HITBOX_BALL 1
#define HITBOX_AABB 2
#define HITBOX_OBB 3

// hitbox_owner_type

#define HITBOX_OWNER_CHARACTER 1
#define HITBOX_OWNER_SHIELD 2
#define HITBOX_OWNER_FURNITURE 3

// shield skill type

#define SHIELD_PHYSICAL 1
#define SHIELD_MAGICAL 2
#define SHIELD FORCEFIELD 3

// skill_school

#define SKILL_FIRE 1
#define SKILL_AIR 2
#define SKILL_WATER 3
#define SKILL_EARTH 4

// skill_type

#define SKILL_MAP_LINKER 1
#define SKILL_MIND_CONTROL 2
#define SKILL_PROJECTILE 3
#define SKILL_RESURRECTION 4
#define SKILL_SHIELD 6
#define SKILL_SIMPLE 7
#define SKILL_SUMMON 8
#define SKILL_TEAM_CHANGER 9
#define SKILL_TELEPORT 10
#define SKILL_TILE_SWAP 11

// skill_scaling_type

#define SKILL_SCALE_NONE 0
#define SKILL_SCALE_MAIN_WEAPON 1
#define SKILL_SCALE_SUB_WEAPON 2
#define SKILL_SCALE_BOTH_WEAPONS 3

// skill_overcharge_type

#define OVERCHARGE_NONE 0
#define OVERCHARGE_LINEAR 1
#define OVERCHARGE_SQUARE 2
#define OVERCHARGE_EXPONENTIAL 3

// attack_order

#define ATTACK_LEFT_TO_RIGHT 1
#define ATTACK_RIGHT_TO_LEFT 2
#define ATTACK_FRONT_TO_BACK 3
#define ATTACK_BACK_TO_FRONT 4

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
#define RACE_HUMAN 2
#define RACE_OMONCULUS 3
#define RACE_AUTOMATA 4
#define RACE_ROBOT 5
#define RACE_UNDEAD 6
#define RACE_ELDRITCH 7

// damage_type

#define DAMAGE_TYPE_NUMBER 14
#define DAMAGE_TYPE_STATUS_NUMBER 9

#define DAMAGE_SLASH 0
#define DAMAGE_PUNCTURE 1
#define DAMAGE_BLUNT 2
#define DAMAGE_FIRE 3
#define DAMAGE_LIGHTNING 4
#define DAMAGE_FROST 5
#define DAMAGE_POISON 6
#define DAMAGE_ACID 7 // unaffected by damage reductions, but still by shield
#define DAMAGE_MIND 8 // unaffected by shield, but still by damage reductions
#define DAMAGE_SOLAR 9 // health damage
#define DAMAGE_AETHER 10 // mana damage
#define DAMAGE_NEUTRAL 11
#define DAMAGE_TRUE 12
// DAMAGE_PHYSICAL needs to be the last damage
#define DAMAGE_PHYSICAL 13 // raw weapon damage (slash + puncture + blunt)

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
#define ACTION_RUN 2
#define ACTION_JUMP 3
#define ACTION_ACTIVATION 4
#define ACTION_USE_SKILL 5
#define ACTION_USE_ITEM 6
#define ACTION_RELOAD 7
#define ACTION_GRAB 8
#define ACTION_SWAP_GEAR 9
#define ACTION_REST 10
#define ACTION_TALKING 11
#define ACTION_ECONOMICS 12
#define ACTION_BREAKPOINT 13
// shortcut for client only
#define ACTION_ATTACK 101
#define ACTION_BLOCK 102

// settings

#define SETTINGS_PAUSE_NO_ACTION 1
#define SETTINGS_PAUSE_ALL 2
#define SETTINGS_PAUSE_MASTER 3
#define SETTINGS_PAUSE_NONE 4

// teams relations

#define TEAM_SAME 1
#define TEAM_ALLY 2
#define TEAM_NEUTRAL 3
#define TEAM_ENEMY 4

// socket_msg_type

#define SOCKET_MSG_CONNECT 1
#define SOCKET_MSG_RECONNECT 2
#define SOCKET_MSG_ADVENTURE 3
#define SOCKET_MSG_CHOICE 4
#define SOCKET_MSG_READY 5
#define SOCKET_MSG_CHARACTER 6
#define SOCKET_MSG_ACTION 7
#define SOCKET_MSG_STATE 8
#define SOCKET_MSG_PAUSE 9
#define SOCKET_MSG_UNPAUSE 10
#define SOCKET_MSG_QUIT 11
#define SOCKET_MSG_SHUTDOWN 12

#endif // _VALUES_H_
