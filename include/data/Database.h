#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <map>
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
    const Map * getMap(const std::string map);
    const Projectile * getProjectile(const std::string projectile);
    const Ammunition * getAmmunition(const std::string ammunition);
    const Quest * getQuest(const std::string quest);
    const Skill * getSkill(const std::string skill);
    const Speech * getSpeech(const std::string speech);
    const Tile * getTile(const std::string tile);
    const Way * getWay(const std::string way);
    const Weapon * getWeapon(const std::string weapon);

    void addAttributes(const Attributes * attributes);
    void addCharacter(const Character * character);
    void addEffect(const Effect * effect);
    void addEvent(const Event * event);
    void addItem(const Item * item);
    void addMap(const Map * map);
    void addProjectile(const Projectile * projectile);
    void addAmmunition(const Ammunition * ammunition);
    void addQuest(const Quest * quest);
    void addSkill(const Skill * skill);
    void addSpeech(const Speech * speech);
    void addTile(const Tile * tile);
    void addWay(const Way * way);
    void addWeapon(const Weapon * weapon);

  private:
    std::map<const std::string,const int> macros;

    std::map<const std::string,const Attributes * > attributes;
    std::map<const std::string,const Character * > characters;
    std::map<const std::string,const Effect * > effects;
    std::map<const std::string,const Event * > events;
    std::map<const std::string,const Item * > items;
    std::map<const std::string,const Map * > maps;
    std::map<const std::string,const Projectile * > projectiles;
    std::map<const std::string,const Ammunition * > ammunitions;
    std::map<const std::string,const Quest * > quests;
    std::map<const std::string,const Skill * > skills;
    std::map<const std::string,const Speech * > speechs;
    std::map<const std::string,const Tile * > tiles;
    std::map<const std::string,const Way * > ways;
    std::map<const std::string,const Weapon * > weapons;
};

#endif // _DATABASE_H_
