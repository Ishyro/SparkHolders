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
    const int getTargetFromMacro(const std::string macro);

    const Attributes * getAttributes(const std::string attributes);
    const Character * getCharacter(const std::string character);
    const Effect * getEffect(const std::string effect);
    const Event * getEvent(const std::string event);
    const Item * getItem(const std::string item);
    const Gear * getGear(const std::string gear);
    const Map * getMap(const std::string map);
    const Projectile * getProjectile(const std::string projectile);
    const Ammunition * getAmmunition(const std::string ammunition);
    const Quest * getQuest(const std::string quest);
    const Skill * getSkill(const std::string skill);
    const PseudoSkill * getPseudoSkill(const std::string pseudoSkill);
    const Speech * getSpeech(const std::string speech);
    const Tile * getTile(const std::string tile);
    const Way * getWay(const std::string way);
    const Weapon * getWeapon(const std::string weapon);
    const int getRelation(const std::string team1, const std::string team2);
    std::list<std::pair<const std::string, const std::string>> getWaysIncompatibilities();
    std::list<std::string> getTranslationPaths();

    void addAttributes(const Attributes * attributes);
    void addCharacter(const Character * character);
    void addEffect(const Effect * effect);
    void addEvent(const Event * event);
    void addItem(const Item * item);
    void addGear(const Gear * gear);
    void addMap(const Map * map);
    void addProjectile(const Projectile * projectile);
    void addAmmunition(const Ammunition * ammo);
    void addQuest(const Quest * quest);
    void addSkill(const Skill * skill);
    void addPseudoSkill(const PseudoSkill * pseudoSkill);
    void addSpeech(const Speech * speech);
    void addTile(const Tile * tile);
    void addWay(const Way * way);
    void addWeapon(const Weapon * weapon);
    void addRelation(const std::string team1, const std::string team2, int relation);
    void addWayImcompatibility(const std::string way1, const std::string way2);
    void addTranslationPath(const std::string path);

  private:
    std::map<const std::string,const int> macros;

    std::map<const std::string, const Attributes * > attributes;
    std::map<const std::string, const Character * > characters;
    std::map<const std::string, const Effect * > effects;
    std::map<const std::string, const Event * > events;
    std::map<const std::string, const Item * > items;
    std::map<const std::string, const Gear * > gears;
    std::map<const std::string, const Map * > maps;
    std::map<const std::string, const Projectile * > projectiles;
    std::map<const std::string, const Ammunition * > ammunition;
    std::map<const std::string, const Quest * > quests;
    std::map<const std::string, const Skill * > skills;
    std::map<const std::string, const PseudoSkill * > pseudoSkills;
    std::map<const std::string, const Speech * > speechs;
    std::map<const std::string, const Tile * > tiles;
    std::map<const std::string, const Way * > ways;
    std::map<const std::string, const Weapon * > weapons;
    std::map<const std::string, std::map<const std::string, int>> relations;
    std::list<std::pair<const std::string, const std::string>> waysIncompatibilities;
    std::list<std::string> paths;
};

#endif // _DATABASE_H_
