#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <list>
#include <string>
#include <map>
#include <set>
#include <array>
#include <cmath>
#include <algorithm>

#include "data/Projectile.h"

#include "data/items/Item.h"

#include "data/ways/Attributes.h"

#include "util/MathUtil.h"

#include "Values.h"

namespace character {
  static int64_t id_cpt = 0;
}

typedef struct CharacterDisplay {
  std::string name;
  int64_t id;
  float hp;
  int32_t maxHp;
  float mana;
  int32_t maxMana;
  float shield;
  int32_t maxShield;
  float hunger;
  float thirst;
  float stamina;
  float sanity;
  int32_t soulBurn;
  int32_t soulBurnTreshold;
  int32_t flow;
  bool player_character;
  int32_t type;
  float x;
  float y;
  float z;
  float size;
  float height;
  float orientation;
  std::string team;
  int32_t xp;
  int32_t level;
  Speech * talking_speech;
  float damage_reductions[DAMAGE_TYPE_NUMBER];
  int32_t damages[DAMAGE_TYPE_NUMBER];
  int32_t teamRelation;
} CharacterDisplay;

typedef struct Environment {
  int64_t light;
  int32_t lightening;
  float temperature;
  int32_t biome_type;
  int32_t weather_type;
  int32_t depth;
  bool making_effort;
} Environment;

class Character {
  public:
    const int64_t id;
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
      int64_t xp,
      int32_t gold,
      int32_t x,
      int32_t y,
      int32_t z,
      float orientation,
      Region * region,
      std::string team,
      AI * ai,
      Attributes * main_class,
      Attributes * second_class,
      Attributes * spec_class,
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
      coord(MathUtil::makeVector3(x + 0.5F, y + 0.5F, z)),
      orientation(orientation),
      region(region),
      merchant(from_database->merchant),
      team(team),
      ai(ai),
      effects(std::list<Effect *>()),
      skills(from_database->skills),
      sellable_items(std::list<Item *>()),
      sellable_effects(from_database->sellable_effects),
      sellable_skills(from_database->sellable_skills),
      main_class(main_class),
      second_class(second_class),
      spec_class(spec_class),
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
      int32_t maxHp,
      int32_t maxMana,
      int32_t maxShield,
      float hp,
      float mana,
      float shield,
      int32_t damage_multiplier,
      int32_t soulBurnTreshold,
      int32_t flow,
      int32_t visionRange,
      int32_t visionPower,
      int32_t detectionRange,
      int32_t currentSoulBurn,
      float hunger,
      float thirst,
      float stamina,
      float sanity,
      int32_t channeledMana,
      std::string name,
      int64_t id,
      bool player_character,
      Speech * death_speech,
      Speech * talking_speech,
      MathUtil::Vector3 coord,
      float size,
      float height,
      float orientation,
      Region * region,
      bool merchant,
      int64_t gold,
      int64_t xp,
      int32_t level,
      std::string team,
      Gear * gear,
      std::list<Effect *> effects,
      std::list<Skill *> skills,
      std::list<Item *> sellable_items,
      std::list<Effect *> sellable_effects,
      std::list<Skill *> sellable_skills,
      Attributes * main_class,
      Attributes * second_class,
      Attributes * spec_class,
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
      maxShield(maxShield),
      hp(hp),
      mana(mana),
      shield(shield),
      damage_multiplier(damage_multiplier),
      soulBurnTreshold(soulBurnTreshold),
      flow(flow),
      visionRange(visionRange),
      visionPower(visionPower),
      detectionRange(detectionRange),
      currentSoulBurn(currentSoulBurn),
      hunger(hunger),
      thirst(thirst),
      stamina(stamina),
      sanity(sanity),
      channeledMana(channeledMana),
      name(name),
      id(id),
      player_character(player_character),
      death_speech(death_speech),
      talking_speech(talking_speech),
      coord(coord),
      size(size),
      height(height),
      orientation(orientation),
      region(region),
      merchant(merchant),
      gold(gold),
      xp(xp),
      level(level),
      ai(nullptr),
      team(team),
      gear(gear),
      effects(effects),
      skills(skills),
      sellable_items(sellable_items),
      sellable_effects(sellable_effects),
      sellable_skills(sellable_skills),
      main_class(main_class),
      second_class(second_class),
      spec_class(spec_class),
      race(race),
      race_modifiers(race_modifiers),
      origin(origin),
      culture(culture),
      religion(religion),
      profession(profession),
      titles(titles)
    {}

    ~Character();
    
    bool isMarkedDead();
    void markDead(bool dead);
    bool isAlive();
    bool isSoulBurning();
    MathUtil::Vector3 getCoord();
    MathUtil::Coords getWorldCoords();
    float getOrientation();
    float getSize();
    float getHeight();
    float getHp();
    int32_t getMaxHp();
    float getMana();
    float getChanneledMana();
    int32_t getMaxMana();
    float getShield();
    int32_t getMaxShield();
    float getHunger();
    float getThirst();
    float getStamina();
    float getSanity();
    int32_t getSoulBurnThreshold();
    int32_t getCurrentSoulBurn();
    int32_t getFlow();
    int64_t getRawPower();
    int32_t getVisionRange();
    int32_t getVisionPower();
    int32_t getDetectionRange();
    Region * getRegion();
    Action * getCurrentAction();
    int64_t getGold();
    int64_t getXP();
    int32_t getLevel();
    float getDamageMultiplier();
    int32_t getPowerScore();

    bool needToSend();
    void setNeedToSend(bool need_to_send);

    AI * getAI();
    std::string getTeam();
    Speech * getDeathSpeech();
    Speech * getTalkingSpeech();
    int32_t getType();

    Gear * getGear();
    float getActionTimeModifier();
    float getHandActionTimeModifier();
    float getSkillTimeModifier();
    float getMovementTimeModifier();
    float getStrikeTime(int32_t slot);
    float getReloadTime(int32_t slot);
    float getSwapTime(int32_t slot1, int32_t slot2);
    float getUseTime(Item * item);
    int32_t getLight();
    std::list<Item *> getLoot();
    std::list<Effect *> getEffects();
    std::list<Skill *> getSkills();
    std::map<Skill *, std::array<int32_t, DAMAGE_TYPE_NUMBER>> getDamageSkills();

    std::list<Item *> getSellableItems();
    std::list<Effect *> getSellableEffects();
    std::list<Skill *> getSellableSkills();
    std::list<Way *> getSellableTitles();

    Attributes * getMainClass();
    Attributes * getSecondClass();
    Attributes * getSpecClass();
    Race * getRace();
    std::list<Race *> getRaceModifiers();
    Way * getOrigin();
    Way * getCulture();
    Way * getReligion();
    Way * getProfession();
    std::list<Way *> getTitles();

    void setOrientation(float orientation);
    void setSize(float size);
    void move(MathUtil::Vector3 coord, float orientation, World * world);
    void hpHeal(float hp);
    void incrMaxHp();
    void setHp(float hp);
    void manaHeal(float mana);
    void incrMaxMana();
    void setMana(float mana);
    float shieldRestore(float shield);
    void setShield(float shield);
    void incrMaxShield();
    void addHunger(float hunger);
    void addThirst(float thirst);
    void addStamina(float stamina);
    void addSanity(float sanity);
    void setHunger(float hunger);
    void setThirst(float thirst);
    void setStamina(float stamina);
    void setSanity(float sanity);
    void incrDamageMultiplier();
    void incrSoulBurnTreshold();
    void setCurrentSoulBurn(int32_t soulBurn);
    void incrFlow();
    void incrVisionRange();
    void incrVisionPower();
    void incrDetectionRange();
    void setRegion(Region * region);
    void setCurrentAction(Action * action);

    void applySoulBurn();
    void applyManaWaste();
    void channel(int32_t cost);
    void hungerStep(Environment e);
    void thirstStep(Environment e);
    void staminaStep(Environment e);
    void sanityStep(Environment e);
    void applyBodyNeeds();
    void applySoulNeeds();
    void applySpiritNeeds();
    void applyEffects();
    void rest();
    void gainGold(int64_t gold);
    void loseGold(int64_t gold);
    void payMana(float cost);
    void gainXP(int64_t xp);
    void gainLevel();
    void newSkillsAndEffects();
    void selectSecondAttributes();

    void setAI(AI * ai);
    void setTeam(std::string team);
    void setDeathSpeech(std::string option, Database * database);
    void setTalkingSpeech(std::string option, Database * database);

    void add(Item * item, int32_t slot, int32_t x, int32_t y);
    Item * remove(int32_t slot, int32_t x, int32_t y);
    float swap(int32_t slot1, int32_t slot2);

    void addEffect(Effect * e);
    void addSkill(Skill * s);
    bool hasSkill(Skill * s);
    void removeEffect(Effect * e);
    void removeSkill(Skill * s);

    void setWay(Way * way);

    void addItem(Item * i, int32_t x, int32_t y, int32_t slot);
    void removeItem(int32_t x, int32_t y, int32_t slot);
    void useItem(int32_t x, int32_t y, int32_t slot);

    bool isFlying();
    bool isChanneling();
    bool isStunned();
    bool isCloaked();
    bool isInvisible();
    bool isEtheral();
    bool isInvulnerable();
    bool isSleeping();
    bool isIdling();
    int32_t cloakPower();
    bool isInWeakState();

    void useSkill(Skill * skill, Target * target, Adventure * adventure, int32_t overcharge_power, int32_t overcharge_duration, int32_t overcharge_range);
    int32_t getDamageFromType(int32_t damage_type, int32_t slot);
    float getDamageReductionFromType(int32_t damage_type);
    Projectile * shoot(Target * target, Adventure * adventure, int32_t slot);
    void reload(ItemSlot * ammo, int32_t slot_weapon);
    void attack(Character * target, Adventure * adventure, int32_t type);
    void mainAttack(Character * target, Adventure * adventure, int32_t type);
    void subAttack(Character * target, Adventure * adventure, int32_t type);
    ItemSlot * canReload(int32_t slot);
    void receiveAttack(int32_t damages[DAMAGE_TYPE_NUMBER], float orientation, int32_t type);
    void receiveCriticalAttack(int32_t damages[DAMAGE_TYPE_NUMBER], int32_t type);
    int32_t tryAttack(std::array<int32_t, DAMAGE_TYPE_NUMBER> damages, int32_t type);
    void trade(Character * buyer, int32_t object_type, std::string object_name, float price_modifier);
    std::set<std::string> getTags();
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
    MathUtil::Vector3 coord;
    float size;
    float height;
    float orientation;
    Region * region;
    Action * current_action = nullptr;
    float hp;
    int32_t maxHp;
    float mana;
    int32_t maxMana;
    float shield;
    int32_t maxShield;
    float hunger;
    float thirst;
    float stamina;
    float sanity;
    float channeledMana;
    int32_t damage_multiplier;
    int32_t soulBurnTreshold;
    float currentSoulBurn;
    int32_t flow;
    int32_t visionRange;
    int32_t visionPower;
    int32_t detectionRange;
    bool need_to_send = false;
    bool dead = false;

    int64_t gold;
    int64_t xp;
    int32_t level;

    AI * ai;
    std::string team;
    Speech * death_speech;
    Speech * talking_speech;

    Gear * gear;
    std::list<Effect *> effects;
    std::list<Skill *> skills;

    bool merchant;
    std::list<Item *> sellable_items;
    std::list<Effect *> sellable_effects;
    std::list<Skill *> sellable_skills;

    Attributes * main_class;
    Attributes * second_class;
    Attributes * spec_class;
    Race * race;
    std::list<Race *> race_modifiers;
    Way * origin;
    Way * culture;
    Way * religion;
    Way * profession;
    std::list<Way *> titles;
};

#endif // _CHARACTER_H_
