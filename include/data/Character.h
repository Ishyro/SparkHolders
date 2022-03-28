#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <list>
#include <string>
#include <cmath>
#include <algorithm>

#include "Values.h"

#include "data/Attributes.h"
#include "data/Gear.h"
#include "data/Item.h"
#include "data/Weapon.h"
#include "data/Effect.h"
#include "data/skills/Skill.h"
#include "data/World.h"
#include "data/Quest.h"
#include "data/Way.h"

#include "utils/FileOpener.h"

namespace character {
  static long id_cpt = 0;
}

class Character {
  public:
    const long id = ++character::id_cpt;
    const std::string name;
    const Attributes * attributes;
    const bool player_character;
    const bool quest_character;
    const Speech * death_speech;
    const Speech * encounter_speech;
    const int type;
    Character(
      std::string name,
      bool player_character,
      bool quest_character,
      Speech * death_speech,
      Speech * encounter_speech,
      int type,
      long current_map_id,
      int x,
      int y,
      int orientation,
      int ai_id,
      std::string team
    ):
      name(name),
      player_character(player_character),
      quest_character(quest_character),
      death_speech(death_speech),
      encounter_speech(encounter_speech),
      type(type),
      current_map_id(current_map_id),
      x(x),
      y(y),
      orientation(orientation),
      ai_id(ai_id),
      team(team)
    {}
    bool isAlive();
    bool isSoulBurning();
    int getX();
    int getY();
    int getOrientation();
    int getHp();
    int getMaxHp();
    int getMana();
    int getMaxMana();
    int getArmor();
    int getSoulBurnTreshold();
    int getCurrentSoulBurn();
    int getFlow();
    int getVision();
    int getDarkVision();
    long getCurrentMapId();
    long getGold();

    std::list<Item *> getStuff();
    std::list<Weapon *> getWeapons();
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();

    void move(int orientation);
    void move(int x, int y);
    void move(int x, int y, int orientation);
    void hpHeal(int hp);
    void incrMaxHp();
    void manaHeal(int mana);
    void incrMaxMana();
    void incrArmor();
    void incrSoulBurnTreshold();
    void incrFlow();
    void setVision(int vision);
    void setDarkVision(int dark_vision);
    void setCurrentMapId(long map_id);

    void equip(Item * to_equip);
    void equip(Weapon * to_equip);
    void equip(Ammunition * to_equip);
    void unequip(int type);
    void unequipWeapon();
    void unequipAmmunition();

    void addEffect(Effect * e);
    void addSkill(Skill * s);
    void removeEffect(Effect * e);
    void removeSkill(Skill * s);

    // change religion or origin
    void changeWay(std::string old_way, std::string new_way);

    void addItem(Item * i);
    void addWeapon(Weapon * w);
    void addAmmunition(Ammunition * a);
    void removeItem(Item * i);
    void removeWeapon(Weapon * w);
    void removeAmmunition(Ammunition * a);

    int isChanneling();
    int isStunned();
    int isCloaked();
    int isInvisible();
    int isSleeping();
    int cloakPower();
    int invisibilityPower();
    bool isInWeakState();

    bool isTileLighted(World * world);

    Quest * getQuest();

    void useSkill(Skill * skill, Character * target, Adventure * adventure, long overcharge);
    void receiveAttack(int damage, int damage_type, int orientation);
    void receiveCriticalAttack(int damage, int damage_type);

  private:
    static long id_cpt;
    long current_map_id;
    int x;
    int y;
    int orientation;
    int ai_id;
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    int armor;
    int soulBurnTreshold;
    int currentSoulBurn;
    int flow;
    int vision;
    int dark_vision;

    long gold;

    std::string team;

    Gear * gear;
    std::list<Item *> stuff;
    std::list<Weapon *> weapons;
    std::list<Ammunition *> ammunitions;
    std::list<Effect *> effects;
    std::list<Skill *> skills;

    Quest * quest;
    Way * religion;
    Way * origin;
    Way * race;
    Way * sex;
};

#endif // _CHARACTER_H_
