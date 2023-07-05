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
  float x;
  float y;
  float z;
  float size;
  float orientation;
  std::string team;
  int armor;
  int xp;
  int level;
  Speech * talking_speech;
  float damage_reductions[DAMAGE_TYPE_NUMBER];
  int damages[DAMAGE_TYPE_NUMBER];
  int teamRelation;
} CharacterDisplay;

class Character {
  public:
    const long id;
    const std::string name;
    const bool player_character;
    // not instancied character constructor
    Character(
      std::string name,
      bool player_character,
      Speech * death_speech,
      Speech * talking_speech,
      bool merchant,
      std::list<Effect *> effects,
      std::list<Skill *> skills,
      std::list<Item *> sellable_items,
      std::list<Effect *> sellable_effects,
      std::list<Skill *> sellable_skills
    ):
      name(name),
      id(0),
      player_character(player_character),
      death_speech(death_speech),
      talking_speech(talking_speech),
      merchant(merchant),
      effects(effects),
      skills(skills),
      sellable_items(sellable_items),
      sellable_effects(sellable_effects),
      sellable_skills(sellable_skills)
    {}
    Character(
      const Character * from_database,
      std::string name,
      long xp,
      int gold,
      int x,
      int y,
      int z,
      float orientation,
      Map * current_map,
      std::string team,
      AI * ai,
      Attributes * attributes,
      Attributes * second_attributes,
      Gear * gear,
      Race * race,
      std::list<Race *> race_modifiers,
      Way * origin,
      Way * culture,
      Way * religion,
      Way * profession,
      std::list<Way *> titles
    ):
      name(name),
      id(++character::id_cpt),
      player_character(from_database->player_character),
      death_speech(from_database->death_speech),
      talking_speech(from_database->talking_speech),
      gold(gold),
      xp(xp),
      level(1),
      x(x + 0.5F),
      y(y + 0.5F),
      z(z),
      orientation(orientation),
      current_map(current_map),
      merchant(from_database->merchant),
      team(team),
      ai(ai),
      items(std::list<Item *>()),
      effects(std::list<Effect *>()),
      skills(from_database->skills),
      sellable_items(std::list<Item *>()),
      sellable_effects(from_database->sellable_effects),
      sellable_skills(from_database->sellable_skills),
      attributes(attributes),
      second_attributes(second_attributes),
      race(race),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession),
      titles(std::list<Way *>())
    {
      for(Item * item : from_database->sellable_items) {
        sellable_items.push_back(Item::init(item, 1, 1));
      }
      for(Race * modifier : race_modifiers) {
        setWay( (Way *) modifier);
      }
      for(Way * title : titles) {
        setWay(title);
      }
      initializeCharacter(gear);
      initEffects(from_database->effects);
    }
    Character(
      int maxHp,
      int maxMana,
      int hp,
      int mana,
      int armor,
      int armor_multiplier,
      int damage_multiplier,
      int soulBurnTreshold,
      int flow,
      int visionRange,
      int visionPower,
      int detectionRange,
      int currentSoulBurn,
      float stamina,
      float satiety,
      float savedHpRegen,
      float savedManaRegen,
      int channeledMana,
      std::string name,
      long id,
      bool player_character,
      Speech * death_speech,
      Speech * talking_speech,
      float x,
      float y,
      float z,
      float size,
      float orientation,
      Map * current_map,
      bool merchant,
      long gold,
      long xp,
      int level,
      std::string team,
      Gear * gear,
      std::list<Item *> items,
      std::list<Effect *> effects,
      std::list<Skill *> skills,
      std::list<Item *> sellable_items,
      std::list<Effect *> sellable_effects,
      std::list<Skill *> sellable_skills,
      Attributes * attributes,
      Attributes * second_attributes,
      Race * race,
      std::list<Race *> race_modifiers,
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
      armor_multiplier(armor_multiplier),
      damage_multiplier(damage_multiplier),
      soulBurnTreshold(soulBurnTreshold),
      flow(flow),
      visionRange(visionRange),
      visionPower(visionPower),
      detectionRange(detectionRange),
      currentSoulBurn(currentSoulBurn),
      stamina(stamina),
      satiety(satiety),
      savedHpRegen(savedHpRegen),
      savedManaRegen(savedManaRegen),
      channeledMana(channeledMana),
      name(name),
      id(id),
      player_character(player_character),
      death_speech(death_speech),
      talking_speech(talking_speech),
      x(x),
      y(y),
      z(z),
      size(size),
      orientation(orientation),
      current_map(current_map),
      merchant(merchant),
      gold(gold),
      xp(xp),
      level(level),
      ai(nullptr),
      team(team),
      gear(gear),
      items(items),
      effects(effects),
      skills(skills),
      sellable_items(sellable_items),
      sellable_effects(sellable_effects),
      sellable_skills(sellable_skills),
      attributes(attributes),
      second_attributes(second_attributes),
      race(race),
      race_modifiers(race_modifiers),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession),
      titles(titles)
    {
      // should always be 0 at round start
      currentFlowOut = 0;
      currentFlowIn = 0;
    }

    ~Character();
    
    bool isMarkedDead();
    bool markDead(bool dead);
    bool isAlive();
    bool isSoulBurning();
    float getX();
    float getY();
    float getZ();
    float getOrientation();
    float getSize();
    int getHp();
    int getMaxHp();
    int getMana();
    int getAvaillableMana(bool overflow);
    int getMaxMana();
    float getStamina();
    float getSatiety();
    int getArmor();
    float getArmorMultiplier();
    int getSoulBurnTreshold();
    int getCurrentSoulBurn();
    int getFlow();
    int getVisionRange();
    int getVisionPower();
    int getDetectionRange();
    Map * getCurrentMap();
    long getGold();
    long getXP();
    int getLevel();
    float getDamageMultiplier();
    int getPowerScore();

    bool needToSend();
    void setNeedToSend(bool need_to_send);
    bool getNeedToUpdateActions();
    void setNeedToUpdateActions(bool need_to_update_actions);

    AI * getAI();
    std::string getTeam();
    Speech * getDeathSpeech();
    Speech * getTalkingSpeech();
    int getType();

    Gear * getGear();
    float getActionTimeModifier();
    float getHandActionTimeModifier();
    float getSkillTimeModifier();
    float getMovementTimeModifier();
    float getStrikeTime();
    float getReloadTime();
    float getSwapTime(long item_id);
    float getUseTime(long item_id);
    int getLight();
    std::list<Item *> getItems();
    std::list<Item *> getLoot();
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
    std::map<Skill *, std::array<int, DAMAGE_TYPE_NUMBER>> getDamageSkills();

    std::list<Item *> getSellableItems();
    std::list<Effect *> getSellableEffects();
    std::list<Skill *> getSellableSkills();
    std::list<Way *> getSellableTitles();

    Attributes * getAttributes();
    Attributes * getSecondAttributes();
    Race * getRace();
    std::list<Race *> getRaceModifiers();
    Way * getOrigin();
    Way * getCulture();
    Way * getReligion();
    Way * getProfession();
    std::list<Way *> getTitles();

    void setOrientation(float orientation);
    void setSize(float size);
    void move(float x, float y, float z, float orientation, World * world);
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
    void incrArmorMultiplier();
    void incrDamageMultiplier();
    void incrSoulBurnTreshold();
    void setCurrentSoulBurn(int soulBurn);
    void incrFlow();
    void incrVisionRange();
    void incrVisionPower();
    void incrDetectionRange();
    void setCurrentMap(Map * map);

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
    void newSkillsAndEffects();
    void selectSecondAttributes();

    void setAI(AI * ai);
    void setTeam(std::string team);
    void setDeathSpeech(std::string option, Database * database);
    void setTalkingSpeech(std::string option, Database * database);

    void equip(GearItem * to_equip);
    void unequip(int type, int type2);

    void addEffect(Effect * e);
    void addSkill(Skill * s);
    bool hasSkill(Skill * s);
    void removeEffect(Effect * e);
    void removeSkill(Skill * s);

    void setWay(Way * way);

    void addItem(Item * i);
    void removeItem(Item * i);
    void useItem(long item_id);

    bool isFlying();
    bool isChanneling();
    bool isStunned();
    bool isCloaked();
    bool isInvisible();
    bool isEtheral();
    bool isInvulnerable();
    bool isSleeping();
    int cloakPower();
    bool isInWeakState();

    void useSkill(Skill * skill, Target * target, Adventure * adventure, int overcharge_power, int overcharge_duration, int overcharge_range);
    int getDamageFromType(int damage_type);
    float getDamageReductionFromType(int damage_type);
    Projectile * shoot(Target * target, Adventure * adventure);
    void reload(AmmunitionItem * ammo);
    void attack(Character * target, int type);
    AmmunitionItem * canReload();
    WeaponItem * swapMelee();
    void receiveAttack(int damages[DAMAGE_TYPE_NUMBER], float orientation, int type);
    void receiveCriticalAttack(int damages[DAMAGE_TYPE_NUMBER], int type);
    int tryAttack(std::array<int, DAMAGE_TYPE_NUMBER> damages, int type);
    void trade(Character * buyer, int object_type, std::string object_name, float price_modifier);
    std::string to_string();
    std::string full_to_string(Adventure * adventure);
    static CharacterDisplay * from_string(std::string to_read);
    static Character * full_from_string(std::string to_read, Adventure * adventure);
    bool operator == (const Character& c) const { return id == c.id; }
    bool operator != (const Character& c) const { return !operator==(c); }

  private:
    void initializeCharacter(Gear * gear);
    void initSkillsAndEffects();
    void initEffects(std::list<Effect *> effects);
    float x;
    float y;
    float z = 0;
    float size;
    float orientation;
    Map * current_map;
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    float stamina;
    float satiety;
    float savedHpRegen;
    float savedManaRegen;
    int channeledMana;
    int armor;
    int armor_multiplier;
    int damage_multiplier;
    int soulBurnTreshold;
    int currentSoulBurn;
    int flow;
    int currentFlowOut;
    int currentFlowIn;
    int visionRange;
    int visionPower;
    int detectionRange;
    bool need_to_send = false;
    bool need_to_update_actions = true;
    bool dead = false;

    long gold;
    long xp;
    int level;

    AI * ai;
    std::string team;
    Speech * death_speech;
    Speech * talking_speech;

    Gear * gear;
    std::list<Item *> items;
    std::list<Effect *> effects;
    std::list<Skill *> skills;

    bool merchant;
    std::list<Item *> sellable_items;
    std::list<Effect *> sellable_effects;
    std::list<Skill *> sellable_skills;

    Attributes * attributes;
    Attributes * second_attributes;
    Race * race;
    std::list<Race *> race_modifiers;
    Way * origin;
    Way * culture;
    Way * religion;
    Way * profession;
    std::list<Way *> titles;
};

#endif // _CHARACTER_H_
