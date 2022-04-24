#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <list>
#include <string>
#include <cmath>
#include <algorithm>

#include "Values.h"

namespace character {
  static long id_cpt = 0;
}

typedef struct CharacterDisplay {
  std::string name;
  int id;
  int hp;
  int mana;
  bool player_character;
  int type;
  long x;
  long y;
  int orientation;
  std::string team;
  int armor;
  float damage_reductions[DAMAGE_TYPE_NUMBER];
  int damages[DAMAGE_TYPE_NUMBER];
  int teamRelation;
} CharacterDisplay;

class Character {
  public:
    const long id = ++character::id_cpt;
    const std::string name;
    const bool player_character;
    const Speech * death_speech;
    const std::list<const Speech *> talking_speechs;
    const int type;
    // not instancied character constructor
    Character(
      std::string name,
      bool player_character,
      const Speech * death_speech,
      const std::list<const Speech *> talking_speechs,
      int type,
      long gold,
      long xp,
      int level,
      std::list<Item *> items,
      std::list<Weapon *> weapons,
      std::list<Ammunition *> ammunitions,
      std::list<Effect *> effects,
      std::list<Skill *> skills
    ):
      name(name),
      player_character(player_character),
      death_speech(death_speech),
      talking_speechs(talking_speechs),
      type(type),
      gold(gold),
      xp(xp),
      level(level),
      items(items),
      weapons(weapons),
      ammunitions(ammunitions),
      effects(effects),
      skills(skills)
    {}
    Character(
      const Character * from_database,
      std::string name,
      int x,
      int y,
      int orientation,
      int current_map_id,
      std::string team,
      AI * ai,
      Way * race,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession
    ):
      name(name),
      player_character(from_database->player_character),
      death_speech(from_database->death_speech),
      talking_speechs(from_database->talking_speechs),
      type(from_database->type),
      gold(from_database->gold),
      xp(from_database->xp),
      visionRange(from_database->visionRange),
      visionPower(from_database->visionPower),
      detectionRange(from_database->detectionRange),
      items(from_database->items),
      weapons(from_database->weapons),
      ammunitions(from_database->ammunitions),
      effects(from_database->effects),
      skills(from_database->skills),
      x(x),
      y(y),
      orientation(orientation),
      current_map_id(current_map_id),
      team(team),
      ai(ai),
      race(race),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession)
    {}
    Character(
      std::string name,
      int hp,
      int mana,
      bool player_character,
      int type,
      long x,
      long y,
      int orientation,
      std::list<Effect *> effects,
      std::string team,
      Way * race,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession
    ):
      name(name),
      hp(hp),
      mana(mana),
      player_character(player_character),
      type(type),
      x(x),
      y(y),
      orientation(orientation),
      effects(effects),
      team(team),
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
    int getVisionRange();
    int getVisionPower();
    int getDetectionRange();
    long getCurrentMapId();
    long getGold();
    long getXP();
    long getLevel();

    AI * getAI();
    std::string getTeam();

    std::list<Item *> getItems();
    std::list<Weapon *> getWeapons();
    std::list<Ammunition *> getAmmunitions();
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
    void incrVisionRange();
    void incrVisionPower();
    void incrDetectionRange();
    void setCurrentMapId(long map_id);

    void applySoulBurn();
    void gainGold(long gold);
    void payMana(int cost);
    void gainXP(long xp);
    void gainLevel();

    void setAI(AI * ai);
    void setTeam(std::string team);

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

    void setWay(Way * way);

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
    bool isInWeakState();

    void useSkill(Skill * skill, Character * target, Adventure * adventure, long overcharge);
    int getDamageFromType(int damage_type);
    float getDamageReductionFromType(int damage_type);
    void attack(Character * target);
    void receiveAttack(int damage, int damage_type, int orientation);
    void receiveCriticalAttack(int damage, int damage_type);
    std::string to_string();
    static CharacterDisplay * from_string(std::string to_read);

  private:
    static long id_cpt;
    long current_map_id;
    int x;
    int y;
    int orientation;
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    int armor;
    int soulBurnTreshold;
    int currentSoulBurn;
    int flow;
    int visionRange;
    int visionPower;
    int detectionRange;

    long gold;
    long xp;
    int level;

    AI * ai;
    std::string team;

    Gear * gear;
    std::list<Item *> items;
    std::list<Weapon *> weapons;
    std::list<Ammunition *> ammunitions;
    std::list<Effect *> effects;
    std::list<Skill *> skills;

    std::string attributes;
    Way * race;
    Way * origin;
    Way * culture;
    Way * religion;
    Way * profession;
};

#endif // _CHARACTER_H_
