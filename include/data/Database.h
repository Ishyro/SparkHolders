#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <map>
#include <list>
#include <string>
#include <cmath>
#include <algorithm>

#include "Values.h"

class Database {
  public:
    Database();
    std::map<const std::string, const int32_t> getMacros();
    const int32_t getTargetFromMacro(const std::string macro);

    const Attributes * getAttributes(const std::string attributes);
    const std::string getAttributesFile(const std::string attributes);
    const Block * getBlock(const std::string block);
    std::map<const std::string, const Block *> getAvaillableBlocks();
    const Character * getCharacter(const std::string character);
    const Effect * getEffect(const std::string effect);
    const Event * getEvent(const std::string event);
    const Item * getItem(const std::string item);
    const std::string getItemFile(const std::string item);
    const Gear * getGear(const std::string gear);
    const Furniture * getFurniture(const std::string furniture);
    const std::string getFurnitureFile(const std::string furniture);
    const Map * getMap(const std::string map);
    const Projectile * getProjectile(const std::string projectile);
    const Quest * getQuest(const std::string quest);
    const Skill * getSkill(const std::string skill);
    const std::string getSkillFile(const std::string skill);
    const PseudoSkill * getPseudoSkill(const std::string pseudoSkill);
    const Speech * getSpeech(const std::string speech);
    const Stance * getStance(const std::string stance);
    const Way * getWay(const std::string way);
    const std::string getWayFile(const std::string way);
    const int32_t getRelation(const std::string team1, const std::string team2);
    std::list<std::pair<const std::string, const std::string>> getWaysIncompatibilities();
    std::list<std::string> getKeysPaths();

    void addAttributes(const Attributes * attributes);
    void addAttributesFile(const std::string attributes, const std::string path);
    void addBlock(const Block * block);
    void addCharacter(const Character * character);
    void addEffect(const Effect * effect);
    void addEvent(const Event * event);
    void addItem(const Item * item);
    void addItemFile(const std::string item, const std::string path);
    void addFurniture(const Furniture * furniture);
    void addFurnitureFile(const std::string furniture, const std::string path);
    void addGear(const Gear * gear);
    void addMap(const Map * map);
    void addProjectile(const Projectile * projectile);
    void addQuest(const Quest * quest);
    void addSkill(const Skill * skill);
    void addSkillFile(const std::string skill, const std::string path);
    void addPseudoSkill(const PseudoSkill * pseudoSkill);
    void addSpeech(const Speech * speech);
    void addStance(const Stance * stance);
    void addWay(const Way * way);
    void addWayFile(const std::string way, const std::string path);
    void addRelation(const std::string team1, const std::string team2, int32_t relation);
    void addWayImcompatibility(const std::string way1, const std::string way2);
    void addKeyPath(const std::string path);

  private:
    std::map<const std::string, const int32_t> macros;

    std::map<const std::string, const Attributes * > attributes;
    std::map<const std::string, const std::string> attributesFiles;
    std::map<const std::string, const Block * > blocks;
    std::map<const std::string, const Character * > characters;
    std::map<const std::string, const Effect * > effects;
    std::map<const std::string, const Event * > events;
    std::map<const std::string, const Item * > items;
    std::map<const std::string, const std::string> itemsFiles;
    std::map<const std::string, const Furniture * > furnitures;
    std::map<const std::string, const std::string> furnituresFiles;
    std::map<const std::string, const Gear * > gears;
    std::map<const std::string, const Map * > maps;
    std::map<const std::string, const Projectile * > projectiles;
    std::map<const std::string, const Quest * > quests;
    std::map<const std::string, const Skill * > skills;
    std::map<const std::string, const std::string> skillsFiles;
    std::map<const std::string, const PseudoSkill * > pseudoSkills;
    std::map<const std::string, const Speech * > speechs;
    std::map<const std::string, const Stance * > stances;
    std::map<const std::string, const Way * > ways;
    std::map<const std::string, const std::string> waysFiles;
    std::map<const std::string, std::map<const std::string, int32_t>> relations;
    std::list<std::pair<const std::string, const std::string>> waysIncompatibilities;
    std::list<std::string> paths;
};

#endif // _DATABASE_H_
