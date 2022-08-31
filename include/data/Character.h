#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <list>
#include <string>
#include <cmath>
#include <algorithm>

#include "data/Attributes.h"

#include "Values.h"

namespace character {
  static long id_cpt = 0;
}

typedef struct CharacterDisplay {
  std::string name;
  long id;
  int hp;
  int maxHp;
  int mana;
  int maxMana;
  float stamina;
  float satiety;
  int soulBurn;
  int soulBurnTreshold;
  int flow;
  bool player_character;
  int type;
  int x;
  int y;
  int orientation;
  std::string team;
  int armor;
  int xp;
  int level;
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
    const std::list<Speech *> talking_speechs;
    const int type;
    const std::string attributes;
    const bool need_to_eat;
    const bool can_eat_food;
    const bool need_to_sleep;
    // not instancied character constructor
    Character(
      std::string name,
      bool player_character,
      const Speech * death_speech,
      const std::list<Speech *> talking_speechs,
      std::list<Item *> loot,
      int type,
      long gold,
      bool need_to_eat,
      bool can_eat_food,
      bool need_to_sleep,
      std::list<Item *> items,
      std::list<Weapon *> weapons,
      std::list<Ammunition *> ammunition,
      std::list<Effect *> effects,
      std::list<Skill *> skills
    ):
      name(name),
      player_character(player_character),
      death_speech(death_speech),
      talking_speechs(talking_speechs),
      loot(loot),
      type(type),
      gold(gold),
      need_to_eat(need_to_eat),
      can_eat_food(can_eat_food),
      need_to_sleep(need_to_sleep),
      items(items),
      weapons(weapons),
      ammunition(ammunition),
      effects(effects),
      skills(skills)
    {}
    Character(
      const Character * from_database,
      std::string name,
      int xp,
      int x,
      int y,
      int orientation,
      int current_map_id,
      std::string team,
      AI * ai,
      Attributes * attributes,
      Way * race,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession,
      std::list<Way *> titles
    ):
      name(name),
      player_character(from_database->player_character),
      death_speech(from_database->death_speech),
      talking_speechs(from_database->talking_speechs),
      loot(std::list<Item *>()),
      type(from_database->type),
      gold(from_database->gold),
      xp(xp),
      level(1),
      x(x),
      y(y),
      orientation(orientation),
      current_map_id(current_map_id),
      need_to_eat(from_database->need_to_eat),
      can_eat_food(from_database->can_eat_food),
      need_to_sleep(from_database->need_to_sleep),
      team(team),
      ai(ai),
      items(std::list<Item *>()),
      weapons(std::list<Weapon *>()),
      ammunition(from_database->ammunition),
      effects(from_database->effects),
      skills(from_database->skills),
      attributes(attributes->name),
      race(nullptr),
      origin(nullptr),
      culture(nullptr),
      religion(nullptr),
      profession(nullptr),
      titles(std::list<Way *>())
    {
      for(Item * item : from_database->items) {
        items.push_back(new Item(item));
      }
      for(Item * item : from_database->loot) {
        loot.push_back(new Item(item));
      }
      for(Weapon * weapon : from_database->weapons) {
        weapons.push_back(new Weapon(weapon));
      }
      applyAttributes(attributes, true);
      setWay(race);
      setWay(origin);
      setWay(culture);
      setWay(religion);
      setWay(profession);
      for(Way * title : titles) {
        setWay(title);
      }
    }
    Character(
      std::string name,
      bool player_character,
      const Speech * death_speech,
      const std::list<Speech *> talking_speechs,
      std::list<Item *> loot,
      int type,
      int x,
      int y,
      int orientation,
      int current_map_id,
      bool need_to_eat,
      bool can_eat_food,
      bool need_to_sleep,
      long gold,
      long xp,
      int level,
      std::string team,
      Gear * gear,
      std::list<Item *> items,
      std::list<Weapon *> weapons,
      std::list<Ammunition *> ammunition,
      std::list<Effect *> effects,
      std::list<Skill *> skills,
      Attributes * attributes,
      Way * race,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession,
      std::list<Way *> titles
    ):
      name(name),
      player_character(player_character),
      death_speech(death_speech),
      talking_speechs(talking_speechs),
      loot(loot),
      type(type),
      x(x),
      y(y),
      orientation(orientation),
      current_map_id(current_map_id),
      need_to_eat(need_to_eat),
      can_eat_food(can_eat_food),
      need_to_sleep(need_to_sleep),
      gold(gold),
      xp(xp),
      level(level),
      ai(nullptr),
      team(team),
      gear(gear),
      items(items),
      weapons(weapons),
      ammunition(ammunition),
      effects(effects),
      skills(skills),
      attributes(attributes->name),
      race(race),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession),
      titles(titles)
    {
      applyAttributes(attributes, false);
      // always client side
      delete attributes;
    }
    void applyAttributes(Attributes * attributes, bool init);
    bool isAlive();
    bool isSoulBurning();
    int getX();
    int getY();
    int getOrientation();
    int getHp();
    int getMaxHp();
    int getMana();
    int getMaxMana();
    float getStamina();
    float getSatiety();
    int getArmor();
    int getSoulBurnTreshold();
    int getCurrentSoulBurn();
    int getFlow();
    int getVisionRange();
    int getVisionPower();
    int getDetectionRange();
    int getCurrentMapId();
    long getGold();
    long getXP();
    int getLevel();
    float getPriorityModifier();
    float getDamageMultiplier();

    bool needToSend();
    void setNeedToSend(bool need_to_send);

    AI * getAI();
    std::string getTeam();

    Gear * getGear();
    std::list<Item *> getItems();
    std::list<Item *> getLoot();
    std::list<Weapon *> getWeapons();
    std::list<Ammunition *> getAmmunitions();
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
    std::list<Way *> getTitles();

    void setOrientation(int orientation);
    void move(int orientation);
    void move(int y, int x);
    void move(int y, int x, int orientation);
    void hpHeal(int hp);
    void incrMaxHp();
    void setHp(int hp);
    void manaHeal(int mana);
    void incrMaxMana();
    void setMana(int mana);
    void addStamina(float stamina);
    void addSatiety(float satiety);
    void removeStamina(float stamina);
    void removeSatiety(float satiety);
    void setStamina(float stamina);
    void setSatiety(float satiety);
    void incrArmor();
    void incrSoulBurnTreshold();
    void setCurrentSoulBurn(int soulBurn);
    void incrFlow();
    void incrVisionRange();
    void incrVisionPower();
    void incrDetectionRange();
    void setCurrentMapId(int map_id);

    void applySoulBurn();
    void applyTiredness();
    void applyHunger();
    void applyEffects();
    void rest();
    void gainGold(long gold);
    void payMana(int cost);
    void gainXP(long xp);
    void gainLevel();

    void setAI(AI * ai);
    void setTeam(std::string team);

    void equip(Item * to_equip);
    void equip(Weapon * to_equip);
    void unequip(int type);
    void unequipWeapon();

    void addEffect(Effect * e);
    void addSkill(Skill * s);
    bool hasSkill(Skill * s);
    void removeEffect(Effect * e);
    void removeSkill(Skill * s);

    void setWay(Way * way);

    void addItem(Item * i);
    void addWeapon(Weapon * w);
    void addAmmunition(Ammunition * a);
    void removeItem(Item * i);
    void removeWeapon(Weapon * w);
    void removeAmmunition(Ammunition * a);
    void useItem(std::string item);

    bool isChanneling();
    bool isStunned();
    bool isCloaked();
    bool isInvisible();
    bool isEtheral();
    bool isInvulnerable();
    bool isSleeping();
    int cloakPower();
    bool isInWeakState();

    void useSkill(Skill * skill, Character * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_range, int x, int y);
    int getDamageFromType(int damage_type);
    float getDamageReductionFromType(int damage_type);
    Projectile * shoot(const Character * target, int y, int x);
    void reload(Ammunition * ammo);
    void attack(Character * target);
    void receiveAttack(int damages[DAMAGE_TYPE_NUMBER], int orientation);
    void receiveCriticalAttack(int damages[DAMAGE_TYPE_NUMBER]);
    std::string to_string(int offsetY, int offsetX);
    std::string full_to_string(Adventure * adventure);
    static CharacterDisplay * from_string(std::string to_read);
    static Character * full_from_string(std::string to_read);
    bool operator == (const Character& c) const { return id == c.id; }
    bool operator != (const Character& c) const { return !operator==(c); }
    void deepDelete();

  private:
    int x;
    int y;
    int orientation;
    int current_map_id;
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    float stamina;
    float satiety;
    int armor;
    int soulBurnTreshold;
    int currentSoulBurn;
    int flow;
    int visionRange;
    int visionPower;
    int detectionRange;
    bool need_to_send = false;

    long gold;
    long xp;
    int level;

    AI * ai;
    std::string team;

    Gear * gear;
    std::list<Item *> items;
    std::list<Item *> loot;
    std::list<Weapon *> weapons;
    std::list<Ammunition *> ammunition;
    std::list<Effect *> effects;
    std::list<Skill *> skills;

    Way * race;
    Way * origin;
    Way * culture;
    Way * religion;
    Way * profession;
    std::list<Way *> titles;
};

#endif // _CHARACTER_H_
