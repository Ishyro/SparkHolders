#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <list>
#include <string>
#include <map>
#include <array>
#include <cmath>
#include <algorithm>

#include "data/Attributes.h"
#include "data/Projectile.h"

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
  Speech * talking_speech;
  float damage_reductions[DAMAGE_TYPE_NUMBER];
  int damages[DAMAGE_TYPE_NUMBER];
  int teamRelation;
  std::list<Item *> sellable_items;
  std::list<Weapon *> sellable_weapons;
  std::list<Ammunition *> sellable_ammunition;
  std::list<Effect *> sellable_effects;
  std::list<Skill *> sellable_skills;
} CharacterDisplay;

class Character {
  public:
    const long id = ++character::id_cpt;
    const std::string name;
    const bool player_character;
    const int type;
    const std::string attributes;
    const bool has_soulspark;
    const bool need_to_eat;
    const bool can_eat_food;
    const bool need_to_sleep;
    // not instancied character constructor
    Character(
      std::string name,
      bool player_character,
      Speech * death_speech,
      Speech * talking_speech,
      std::list<Item *> loot,
      int type,
      long gold,
      bool has_soulspark,
      bool need_to_eat,
      bool can_eat_food,
      bool need_to_sleep,
      bool merchant,
      std::list<Item *> items,
      std::list<Weapon *> weapons,
      std::list<Ammunition *> ammunition,
      std::list<Effect *> effects,
      std::list<Skill *> skills,
      std::list<Item *> sellable_items,
      std::list<Weapon *> sellable_weapons,
      std::list<Ammunition *> sellable_ammunition,
      std::list<Effect *> sellable_effects,
      std::list<Skill *> sellable_skills
    ):
      name(name),
      player_character(player_character),
      death_speech(death_speech),
      talking_speech(talking_speech),
      loot(loot),
      type(type),
      gold(gold),
      has_soulspark(has_soulspark),
      need_to_eat(need_to_eat),
      can_eat_food(can_eat_food),
      need_to_sleep(need_to_sleep),
      merchant(merchant),
      items(items),
      weapons(weapons),
      ammunition(ammunition),
      effects(effects),
      skills(skills),
      sellable_items(sellable_items),
      sellable_weapons(sellable_weapons),
      sellable_ammunition(sellable_ammunition),
      sellable_effects(sellable_effects),
      sellable_skills(sellable_skills)
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
      talking_speech(from_database->talking_speech),
      loot(std::list<Item *>()),
      type(from_database->type),
      gold(from_database->gold),
      xp(xp),
      level(1),
      x(x),
      y(y),
      orientation(orientation),
      current_map_id(current_map_id),
      has_soulspark(from_database->has_soulspark),
      need_to_eat(from_database->need_to_eat),
      can_eat_food(from_database->can_eat_food),
      need_to_sleep(from_database->need_to_sleep),
      merchant(from_database->merchant),
      team(team),
      ai(ai),
      items(std::list<Item *>()),
      weapons(std::list<Weapon *>()),
      ammunition(std::list<Ammunition *>()),
      effects(std::list<Effect *>()),
      skills(from_database->skills),
      sellable_items(std::list<Item *>()),
      sellable_weapons(std::list<Weapon *>()),
      sellable_ammunition(from_database->sellable_ammunition),
      sellable_effects(from_database->sellable_effects),
      sellable_skills(from_database->sellable_skills),
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
      for(Ammunition * ammo : from_database->ammunition) {
        Ammunition * toadd = new Ammunition();
        toadd->projectile = ammo->projectile;
        toadd->number = ammo->number;
        toadd->gold_value = ammo->gold_value;
        toadd->ammo_type = ammo->ammo_type;
        ammunition.push_back(toadd);
      }
      for(Item * item : from_database->sellable_items) {
        sellable_items.push_back(new Item(item));
      }
      for(Weapon * weapon : from_database->sellable_weapons) {
        sellable_weapons.push_back(new Weapon(weapon));
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
      initEffects(from_database->effects);
    }
    Character(
      int maxHp,
      int maxMana,
      int hp,
      int mana,
      int armor,
      int damage_multiplier,
      int soulBurnTreshold,
      int flow,
      int visionRange,
      int visionPower,
      int detectionRange,
      int currentSoulBurn,
      float stamina,
      float satiety,
      std::string name,
      bool player_character,
      Speech * death_speech,
      Speech * talking_speech,
      std::list<Item *> loot,
      int type,
      int x,
      int y,
      int orientation,
      int current_map_id,
      bool has_soulspark,
      bool need_to_eat,
      bool can_eat_food,
      bool need_to_sleep,
      bool merchant,
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
      std::list<Item *> sellable_items,
      std::list<Weapon *> sellable_weapons,
      std::list<Ammunition *> sellable_ammunition,
      std::list<Effect *> sellable_effects,
      std::list<Skill *> sellable_skills,
      std::string attributes,
      Way * race,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession,
      std::list<Way *> titles
    ):
      maxHp(maxHp),
      maxMana(maxMana),
      hp(hp),
      mana(mana),
      armor(armor),
      damage_multiplier(damage_multiplier),
      soulBurnTreshold(soulBurnTreshold),
      flow(flow),
      visionRange(visionRange),
      visionPower(visionPower),
      detectionRange(detectionRange),
      currentSoulBurn(currentSoulBurn),
      stamina(stamina),
      satiety(satiety),
      name(name),
      player_character(player_character),
      death_speech(death_speech),
      talking_speech(talking_speech),
      loot(loot),
      type(type),
      x(x),
      y(y),
      orientation(orientation),
      current_map_id(current_map_id),
      has_soulspark(has_soulspark),
      need_to_eat(need_to_eat),
      can_eat_food(can_eat_food),
      need_to_sleep(need_to_sleep),
      merchant(merchant),
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
      sellable_items(sellable_items),
      sellable_weapons(sellable_weapons),
      sellable_ammunition(sellable_ammunition),
      sellable_effects(sellable_effects),
      sellable_skills(sellable_skills),
      attributes(attributes),
      race(race),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession),
      titles(titles)
    {}
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
    int getPowerScore();

    bool needToSend();
    void setNeedToSend(bool need_to_send);

    AI * getAI();
    std::string getTeam();
    Speech * getDeathSpeech();
    Speech * getTalkingSpeech();

    Gear * getGear();
    std::list<Item *> getItems();
    std::list<Item *> getLoot();
    std::list<Weapon *> getWeapons();
    std::list<Ammunition *> getAmmunitions();
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
    std::map<Skill *, std::array<int, DAMAGE_TYPE_NUMBER>> getDamageSkills();
    std::list<Way *> getTitles();

    std::list<Item *> getSellableItems();
    std::list<Weapon *> getSellableWeapons();
    std::list<Ammunition *> getSellableAmmunitions();
    std::list<Effect *> getSellableEffects();
    std::list<Skill *> getSellableSkills();
    std::list<Way *> getSellableTitles();

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
    void incrDamageMultiplier();
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
    void loseGold(long gold);
    void payMana(int cost);
    void gainXP(long xp);
    void gainLevel();

    void setAI(AI * ai);
    void setTeam(std::string team);
    void setDeathSpeech(std::string option, Database * database);
    void setTalkingSpeech(std::string option, Database * database);

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
    Ammunition * canReload();
    Weapon * swapMelee();
    void receiveAttack(int damages[DAMAGE_TYPE_NUMBER], int orientation);
    void receiveCriticalAttack(int damages[DAMAGE_TYPE_NUMBER]);
    int tryAttack(std::array<int, DAMAGE_TYPE_NUMBER> damages);
    void trade(Character * buyer, int object_type, std::string object_name, float price_modifier);
    std::string to_string(int offsetY, int offsetX);
    std::string full_to_string(Adventure * adventure);
    static CharacterDisplay * from_string(std::string to_read);
    static Character * full_from_string(std::string to_read);
    bool operator == (const Character& c) const { return id == c.id; }
    bool operator != (const Character& c) const { return !operator==(c); }
    void deepDelete();

  private:
    void initEffects(std::list<Effect *> effects);
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
    int damage_multiplier;
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
    Speech * death_speech;
    Speech * talking_speech;

    Gear * gear;
    std::list<Item *> items;
    std::list<Item *> loot;
    std::list<Weapon *> weapons;
    std::list<Ammunition *> ammunition;
    std::list<Effect *> effects;
    std::list<Skill *> skills;

    bool merchant;
    std::list<Item *> sellable_items;
    std::list<Weapon *> sellable_weapons;
    std::list<Ammunition *> sellable_ammunition;
    std::list<Effect *> sellable_effects;
    std::list<Skill *> sellable_skills;

    Way * race;
    Way * origin;
    Way * culture;
    Way * religion;
    Way * profession;
    std::list<Way *> titles;
};

#endif // _CHARACTER_H_
