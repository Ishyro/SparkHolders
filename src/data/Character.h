#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <list>
#include <string>

#include "src/Values.h"
#include "src/data/Attributes.h"
#include "src/data/Gear.h"
#include "src/data/Item.h"
#include "src/data/Weapon.h"
#include "src/data/Effect.h"
#include "src/data/Skill.h"

namespace character {
  static long id_cpt = 0;
}

class Character {
  public:
    const long id = ++character::id_cpt;
    const std::string name;
    const Attributes * attributes;
    const std::string race;
    const std::string sex;
    Character(std::string name, std::string race, std::string attributes, std::string sex);
    bool isAlive();
    bool isSoulBurning();
    int getX();
    int getY();
    int getOrientation();
    int getHp();
    int getMaxHp();
    int getMana();
    int getMaxMana();
    int getDefense();
    int getSoulBurnTreshold();
    int getCurrentSoulBurn();
    int getFlow();
    int getVision();

    void move(int orientation);
    void move(int x, int y, int orientation);
    void hpHeal(int hp);
    void incrMaxHp();
    void manaHeal(int mana);
    void incrMaxMana();
    void incrDefense();
    void incrSoulBurnTreshold();
    void incrFlow();
    void setVision(int vision);

    void equip(Item * to_equip);
    void addEffect(Effect * e);
    void addSkill(Skill * s);
  private:
    static long id_cpt;
    int x;
    int y;
    int orientation;
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    int defense;
    int soulBurnTreshold;
    int currentSoulBurn;
    int flow;
    int vision;

    long gold;

    std::string team;

    Gear * gear;
    std::list<Item *> stuff;
    std::list<Weapon *> weapons;
    std::list<Effect *> effects;
    std::list<Skill *> skills;
};

#endif // _CHARACTER_H_
