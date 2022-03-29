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
#include "data/Way.h"

#include "utils/FileOpener.h"

namespace character {
  static long id_cpt = 0;
}

class Character {
  public:
    const long id = ++character::id_cpt;
    const std::string name;
    const Speech * death_speech;
    const std::list<const Speech *> talking_speech;
    const int type;
    // not instancied character constructor
    Character(
      std::string name,
      const Speech * death_speech,
      const std::list<const Speech *> talking_speech,
      int type,
      int ai_id,
      long gold,
      long xp,
      int level,
      int vision,
      int dark_vision,
      std::string team,
      std::list<Item *> stuff,
      std::list<Weapon *> weapons,
      std::list<Ammunition *> ammunitions,
      std::list<Effect *> effects,
      std::list<Skill *> skills,
      Way * race,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession
    ):
      name(name),
      player_character(player_character),
      death_speech(death_speech),
      talking_speech(talking_speech),
      type(type),
      ai_id(ai_id),
      gold(gold),
      xp(xp),
      vision(vision),
      dark_vision(dark_vision),
      stuff(stuff),
      weapons(weapons),
      ammunitions(ammunitions),
      effects(effects),
      skills(skills),
      race(race),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession)
    {}
    void applyAttributes(const Attributes * attributes);
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
    long getXP();
    long getLevel();

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

    void gainXP(long xp);
    void gainLevel();

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

    void useSkill(Skill * skill, Character * target, Adventure * adventure, long overcharge);
    void receiveAttack(int damage, int damage_type, int orientation);
    void receiveCriticalAttack(int damage, int damage_type);

  private:
    static long id_cpt;
    bool player_character;
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
    long xp;
    int level;

    std::string team;

    Gear * gear;
    std::list<Item *> stuff;
    std::list<Weapon *> weapons;
    std::list<Ammunition *> ammunitions;
    std::list<Effect *> effects;
    std::list<Skill *> skills;

    Way * race;
    Way * origin;
    Way * culture;
    Way * religion;
    Way * profession;
};

#endif // _CHARACTER_H_
