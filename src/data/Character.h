#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <list>
#include <string>
#include <cmath>

#include "src/Values.h"
#include "src/data/Attributes.h"
#include "src/data/Gear.h"
#include "src/data/Item.h"
#include "src/data/Weapon.h"
#include "src/data/Effect.h"
#include "src/data/Skill.h"
#include "src/data/World.h"
#include "src/data/Quest.h"
#include "src/data/Way.h"

namespace character {
  static long id_cpt = 0;
}

class Character {
  public:
    const long id = ++character::id_cpt;
    const std::string name;
    const Attributes * attributes;
    const bool sex;
    const bool player_character;
    const bool quest_character;
    const Way * religion;
    const Way * origin;
    const Way * race;
    const Speech * death_speech;
    const Speech * encounter_speech;
    const int type;
    Character(std::string name, std::string race, std::string attributes, int sex, std::string religion, std::string origin);
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
    void unequip(int type);
    void unequipWeapon();

    void addEffect(Effect * e);
    void addSkill(Skill * s);
    void removeEffect(Effect * e);
    void removeSkill(Skill * s);

    // change religion or origin
    void changeWay(std::string old_way, std::string new_way);

    void addItem(Item * i);
    void addWeapon(Weapon * w);
    void removeItem(Item * i);
    void removeWeapon(Weapon * w);

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

    void useSkill(Skill * skill, World * world, int overcharge);
    void receiveAttack(int damage, int damage_type, int orientation);
    void receiveCriticalAttack(int damage, int damage_type);

  private:
    static long id_cpt;
    long current_map_id;
    int x;
    int y;
    int orientation;
    int ai;
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
    std::list<Effect *> effects;
    std::list<Skill *> skills;

    Quest * quest;
};

#endif // _CHARACTER_H_
