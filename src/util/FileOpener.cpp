#include "util/FileOpener.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <regex>

// time
#include<ctime>

#include "ai/AI.h"
#include "ai/PlayerAI.h"
#include "ai/DiurnalPassiveAI.h"
#include "ai/NocturnalPassiveAI.h"
#include "ai/DiurnalAgressiveAI.h"
#include "ai/NocturnalAgressiveAI.h"
#include "ai/EtheralCasterAI.h"
#include "ai/GuardAI.h"
#include "ai/RoamerAI.h"

#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Effect.h"
#include "data/Event.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Quest.h"
#include "data/Settings.h"
#include "data/skills/Skill.h"
#include "data/Speech.h"
#include "data/Block.h"
#include "data/World.h"
#include "data/Database.h"

#include "data/items/Item.h"
#include "data/items/ArmorItem.h"
#include "data/items/WeaponItem.h"
#include "data/items/BasicItem.h"
#include "data/items/SerialItem.h"
#include "data/items/AmmunitionItem.h"
#include "data/items/ContainerItem.h"
#include "data/items/Gear.h"

#include "data/furnitures/Furniture.h"
#include "data/furnitures/BasicFurniture.h"
#include "data/furnitures/ContainerFurniture.h"
#include "data/furnitures/CraftingFurniture.h"
#include "data/furnitures/LinkedFurniture.h"
#include "data/furnitures/SkillFurniture.h"
#include "data/furnitures/SwitchFurniture.h"

#include "data/skills/PseudoSkill.h"
#include "data/skills/Skill.h"

#include "data/skills/ChanneledSkill.h"
#include "data/skills/MapLinkerSkill.h"
#include "data/skills/MindControlSkill.h"
#include "data/skills/ProjectileSkill.h"
#include "data/skills/ResurrectionSkill.h"
#include "data/skills/SimpleSkill.h"
#include "data/skills/SummonSkill.h"
#include "data/skills/TeamChangerSkill.h"
#include "data/skills/TeleportSkill.h"
#include "data/skills/BlockSwapSkill.h"

#include "data/ways/Way.h"
#include "data/ways/Race.h"
#include "data/ways/Attributes.h"

#include "data/Settings.h"
#include "data/ClientSettings.h"

#include "util/String.h"

namespace FileOpener {

  float _stof(std::string msg) {
    float result = stof(msg);
    int32_t int_part = (int32_t) result;
    if(result - int_part == 0.) {
      // float formating is not the same everywhere
      std::replace(msg.begin(), msg.end(), '.', ',');
      result = stof(msg);
    }
    return result;
  }

  std::map<const std::string,std::string> getValuesFromFile(std::string fileName) {
    std::map<const std::string, std::string> result = std::map<const std::string,std::string>();
    std::fstream file;
    std::string os_fileName = std::regex_replace(fileName, std::regex("/"), PATH_DELIMITER);
    file.open(os_fileName, std::ios::in);
    if(!file) {
      std::cout << "File not found: " + fileName << std::endl;
    }
    std::string line;
    std::string delimiter = "=";
    while(getline(file,line)) {
      while(line != "" && std::isspace(line.at(0))) {
        line = line.substr(1, line.length());
      }
      if(line == "!end") {
        break;
      }
      if(line != "" && line.at(0) != '#') {
        std::string key = line.substr(0, line.find(delimiter));
        while(key.length() > 0 && std::isspace(key.at(key.length() - 1))) {
          key = key.substr(0, key.length() - 1);
        }
        if(line.length() - 1 > line.find(delimiter)) {
          std::string value = line.substr(line.find(delimiter) + 1, line.length() - 1);
          while(value.length() > 0 && std::isspace(value.at(0))) {
            value = value.substr(1, value.length() - 1);
          }
          while(value.length() > 0 && std::isspace(value.at(value.length() - 1))) {
            value = value.substr(0, value.length() - 1);
          }
          result.insert(std::make_pair(key,value));
        }
        else {
          result.insert(std::make_pair(key,""));
        }
      }
    }
    file.close();
    return result;
  }

  void getCoordinates(std::string to_read, int32_t & x, int32_t & y, int32_t & z) {
    std::stringstream * ss = new std::stringstream(to_read);
    x = String::extract_int(ss);
    y = String::extract_int(ss);
    z = String::extract_int(ss);
    delete ss;
  }

  Adventure * AdventureOpener(std::string fileName, bool isServer) {
    std::string delimiter = ".";
    Database * database = DatabaseOpener(fileName, isServer);
    if(isServer) {
      SettingsOpener("data" + PATH_DELIMITER + "settings_server.data", database);
    }
    std::fstream file;
    std::string os_fileName = std::regex_replace(fileName, std::regex("/"), PATH_DELIMITER);
    file.open(os_fileName, std::ios::in);
    if(!file) {
      std::cout << "File not found: " + fileName << std::endl;
    }
    std::string line;
    std::string name;
    // skip the database
    while(getline(file, line)) {
      if(line == "!end") {
        break;
      }
    }
    // first line is the adventure name
    do {
      getline(file, name);
    } while(name == "" || name.at(0) == '#');
    World * world = new World(name);
    std::list<Quest *> * quests = new std::list<Quest *>();
    std::list<Event *> * events = new std::list<Event *>();
    std::list<Spawn *> * spawns = new std::list<Spawn *>();
    std::list<Attributes *> * startingAttributes = new std::list<Attributes *>();
    std::list<Way *> * startingWays = new std::list<Way *>();

    while(getline(file,line)) {
      while(line != "" && std::isspace(line.at(0))) {
        line = line.substr(1, line.length());
      }
      if(line != "" && line.at(0) != '#') {
        std::string keyword = line.substr(0, line.find(delimiter));
        while(std::isspace(keyword.at(keyword.length() - 1))) {
          keyword = keyword.substr(0, keyword.length() - 1);
        }
        std::string command = line.substr(line.find(delimiter) + 1, line.length() - 1);
        while(std::isspace(command.at(0))) {
          command = command.substr(1, command.length());
        }
        while(std::isspace(command.at(command.length() - 1))) {
          command = command.substr(0, command.length() - 1);
        }
        executeCommand(keyword, command, world, quests, events, spawns, startingAttributes, startingWays, database, isServer);
      }
    }
    file.close();
    // we need to escape '\' 2 times
    Adventure * adventure = new Adventure(name, std::regex_replace(fileName, std::regex("\\\\"), "/"), spawns->size(), database, world, *quests, *events, *spawns, *startingAttributes, *startingWays);
    delete quests;
    delete events;
    delete spawns;
    delete startingAttributes;
    delete startingWays;
    return adventure;
  }

  void executeCommand(std::string keyword, std::string command, World * world, std::list<Quest *> * quests, std::list<Event *> * events, std::list<Spawn *> * spawns, std::list<Attributes *> * startingAttributes, std::list<Way *> * startingWays, Database * database, bool isServer) {
    std::stringstream * ss = new std::stringstream(command);
    if(keyword == "Character" && isServer) {
      std::string name = String::extract(ss);
      std::string coord = String::extract(ss);
      int32_t x, y, z;
      getCoordinates(coord, x, y, z);
      std::string team = String::extract(ss);
      std::string ai_str = String::extract(ss);
      Attributes * main_class = (Attributes *) database->getAttributes(String::extract(ss));
      Attributes * second_class = (Attributes *) database->getAttributes(String::extract(ss));
      Attributes * spec_class = (Attributes *) database->getAttributes(String::extract(ss));
      Gear * gear = (Gear *) database->getGear(String::extract(ss));

      Race * race = (Race *) database->getWay(String::extract(ss));
      std::stringstream * ss_race_modifiers = new std::stringstream(String::extract(ss));
      std::list<Race *> * race_modifiers = new std::list<Race *>();
      while(ss_race_modifiers->rdbuf()->in_avail() != 0) {
        race_modifiers->push_back((Race *) database->getWay(String::extract(ss_race_modifiers)));
      }
      delete ss_race_modifiers;
      Way * origin = (Way *) database->getWay(String::extract(ss));
      Way * culture = (Way *) database->getWay(String::extract(ss));
      Way * religion = (Way *) database->getWay(String::extract(ss));
      Way * profession = (Way *) database->getWay(String::extract(ss));
      std::stringstream * ss_titles = new std::stringstream(String::extract(ss));
      std::list<Way *> * titles = new std::list<Way *>();
      while(ss_titles->rdbuf()->in_avail() != 0) {
        titles->push_back((Way *) database->getWay(String::extract(ss_titles)));
      }
      delete ss_titles;
      int64_t xp = String::extract_long(ss);
      int64_t gold = String::extract_long(ss);
      AI * ai;
      if (ai_str == "DiurnalPassiveAI") {
        ai = new DiurnalPassiveAI(x, y);
      }
      else if (ai_str == "NocturnalPassiveAI") {
        ai = new NocturnalPassiveAI(x, y);
      }
      else if (ai_str == "DiurnalAgressiveAI") {
        ai = new DiurnalAgressiveAI(x, y);
      }
      else if (ai_str == "NocturnalAgressiveAI") {
        ai = new NocturnalAgressiveAI(x, y);
      }
      else if (ai_str == "GuardAI") {
        ai = new GuardAI(x, y);
      }
      else if (ai_str == "RoamerAI") {
        ai = new RoamerAI(x, y);
      }
      Character * c = new Character(
        database->getCharacter(name),
        name,
        xp,
        gold,
        x,
        y,
        z,
        90.F,
        nullptr,
        team,
        ai,
        main_class,
        second_class,
        spec_class,
        gear,
        race,
        *race_modifiers,
        origin,
        culture,
        religion,
        profession,
        *titles
      );
      world->addCharacter(c);
      delete race_modifiers;
      delete titles;
    }
    else if(keyword == "Event" && isServer) {
      Event * event = new Event(database->getEvent(String::extract(ss)));
      events->push_back(event);
    }
    else if(keyword == "Map") {
      std::string map_name = String::extract(ss);
      int32_t offsetX = String::extract_int(ss);
      int32_t offsetY = String::extract_int(ss);
      int32_t offsetZ = String::extract_int(ss);
      int32_t rotation = String::extract_int(ss);
      int32_t lightening = database->getTargetFromMacro(String::extract(ss));
      Map * map = new Map( (Map *) database->getMap(map_name.substr(0, map_name.find('#'))), map_name, offsetX, offsetY, offsetZ, rotation, lightening, database);
      world->addMap(map);
    }
    else if(keyword == "MapLink") {
      MapLink * link = new MapLink();
      std::string coord = String::extract(ss);
      getCoordinates(coord, link->x1, link->y1, link->z1);
      coord = String::extract(ss);
      getCoordinates(coord, link->x2, link->y2, link->z2);
      link->type = database->getTargetFromMacro(String::extract(ss));
      //link->map1 = world->getMap(link->x1, link->y1, link->z1);
      //link->map2 = world->getMap(link->x2, link->y2, link->z2);
      world->addMapLink(link);
      world->addMapLink(link);
    }
    else if(keyword == "Loot" && isServer) {

    }
    else if(keyword == "Quest") {
      Quest * quest = new Quest(database->getQuest(String::extract(ss)));
      quests->push_back(quest);
    }
    else if(keyword == "Settings") {
      std::string setting = String::extract(ss);
      std::string value_str = String::extract(ss);
      if(setting == "YEAR_DURATION") {
        Settings::setYearDuration(stoi(value_str));
      }
      else if(setting == "MONTH_DURATION") {
        Settings::setMonthDuration(stoi(value_str));
      }
      else if(setting == "WEEK_DURATION") {
        Settings::setWeekDuration(stoi(value_str));
      }
      else if(setting == "DAY_DURATION") {
        Settings::setDayDuration(stoi(value_str));
      }
      else if(setting == "HOUR_DURATION") {
        Settings::setHourDuration(stoi(value_str));
      }
      else if(setting == "MINUTE_DURATION") {
        Settings::setMinuteDuration(stoi(value_str));
      }
      else if(setting == "STARTING_YEAR") {
        Settings::setStartingYear(stoi(value_str));
      }
      else if(setting == "STARTING_MONTH") {
        Settings::setStartingMonth(stoi(value_str));
      }
      else if(setting == "STARTING_DAY") {
        Settings::setStartingDay(stoi(value_str));
      }
      else if(setting == "STARING_HOUR") {
        Settings::setStartingHour(stoi(value_str));
      }
      else if(setting == "ORIGIN") {
        std::stringstream * ss_origin = new std::stringstream(value_str);
        int64_t longitudeOrigin = String::extract_long(ss_origin);
        int64_t latitudeOrigin = String::extract_long(ss_origin);
        Settings::setOrigin(longitudeOrigin, latitudeOrigin);
      }
      else if(setting == "SECOND_TO_METER") {
        Settings::setSecondToMeter(_stof(value_str));
      }
      else if(setting == "ZENITH_LIGHT_POWERS") {
        std::stringstream * ss_zenith = new std::stringstream(value_str);
        for(int32_t i = 0; i < Settings::getWeekDuration(); i++) {
          Settings::setZenithLightPower(i, String::extract_long(ss_zenith));
        }
      }
      else if(setting == "NIGHT_LIGHT_POWERS") {
        std::stringstream * ss_night = new std::stringstream(value_str);
        for(int32_t i = 0; i < Settings::getWeekDuration(); i++) {
          Settings::setNightLightPower(i, String::extract_long(ss_night));
        }
      }
      else if(setting == "TIDAL_LOCKED") {
        bool tidalLocked;
        std::istringstream is_tidalLocked(value_str);
        is_tidalLocked >> std::boolalpha >> tidalLocked;
        Settings::setTidalLocked(tidalLocked);
      }
      else if(setting == "NIGHT_DURATION") {
        Settings::setNightDuration(stoi(value_str));
      }
      else if(setting == "DAWN_DURATION") {
        Settings::setDawnDuration(stoi(value_str));
      }
      else if(setting == "DAYTIME_DURATION") {
        Settings::setDaytimeDuration(stoi(value_str));
      }
      else if(setting == "DUSK_DURATION") {
        Settings::setDuskDuration(stoi(value_str));
      }
      else if(setting == "MONTH_NAMES") {
        std::stringstream * ss_months = new std::stringstream(value_str);
        for(int32_t i = 0; i < Settings::getYearDuration() / Settings::getMonthDuration(); i++) {
          Settings::setMonthName(i, String::extract(ss_months));
        }
      }
      else if(setting == "DAY_NAMES") {
        std::stringstream * ss_days = new std::stringstream(value_str);
        for(int32_t i = 0; i < Settings::getWeekDuration(); i++) {
          Settings::setDayName(i, String::extract(ss_days));
        }
      }
    }
    else if(keyword == "Spawn" && isServer) {
      Spawn * spawn = new Spawn();
      std::string coord = String::extract(ss);
      getCoordinates(coord, spawn->x, spawn->y, spawn->z);
      spawns->push_back(spawn);
    }
    else if(keyword == "StartingAttributes") {
      startingAttributes->push_back( (Attributes *) database->getAttributes(String::extract(ss)));
    }
    else if(keyword == "StartingWay") {
      startingWays->push_back( (Way *) database->getWay(String::extract(ss)));
    }
    else if(keyword == "TeamRelation") {
      int32_t relation = database->getTargetFromMacro(String::extract(ss));
      std::string team1 = String::extract(ss);
      std::string team2 = String::extract(ss);
      database->addRelation(team1, team2, relation);
    }
    else if(keyword == "Translation" && !isServer) {
      database->addTranslationPath(String::extract(ss));
    }
    else if(keyword == "WayImcompatibility") {
      std::string way1 = String::extract(ss);
      std::string way2 = String::extract(ss);
      database->addWayImcompatibility(way1, way2);
    }
    delete ss;
  }

  std::string AttributesOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int32_t type = database->getTargetFromMacro(values.at("type"));
    int32_t tier = stoi(values.at("tier"));
    int32_t baseHp = stoi(values.at("baseHp"));
    int32_t baseMana = stoi(values.at("baseMana"));
    int32_t baseShield = stoi(values.at("baseShield"));
    int32_t baseDamageMult = stoi(values.at("baseDamageMult"));
    int32_t baseSoulBurn = stoi(values.at("baseSoulBurn"));
    int32_t baseFlow = stoi(values.at("baseFlow"));
    int32_t baseVisionRange = stoi(values.at("baseVisionRange"));
    int32_t baseVisionPower = stoi(values.at("baseVisionPower"));
    int32_t baseDetectionRange = stoi(values.at("baseDetectionRange"));
    int32_t hpIncr = stoi(values.at("hpIncr"));
    int32_t manaIncr = stoi(values.at("manaIncr"));
    int32_t shieldIncr = stoi(values.at("shieldIncr"));
    int32_t damageMultIncr = stoi(values.at("damageMultIncr"));
    int32_t soulBurnIncr = stoi(values.at("soulBurnIncr"));
    int32_t flowIncr = stoi(values.at("flowIncr"));
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_effects(values.at("effects"));
    std::string effect;
    while(getline(is_effects, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    std::list<Skill *> * skills = new std::list<Skill *>();
    std::istringstream is_skills(values.at("skills"));
    std::string skill;
    while(getline(is_skills, skill, '%')) {
      skills->push_back((Skill *) database->getSkill(skill));
    }
    std::list<std::string> * tags = new std::list<std::string>();
    std::istringstream is_tags(values.at("tags"));
    std::string tag;
    while(getline(is_tags, tag, '%')) {
      tags->push_back(tag);
    }
    std::list<std::string> * and_requirements = new std::list<std::string>();
    std::istringstream is_and_requirements(values.at("and_requirements"));
    std::string requirement;
    while(getline(is_and_requirements, requirement, '%')) {
      and_requirements->push_back(requirement);
    }
    std::list<std::string> * or_requirements = new std::list<std::string>();
    std::istringstream is_or_requirements(values.at("or_requirements"));
    while(getline(is_or_requirements, requirement, '%')) {
      or_requirements->push_back(requirement);
    }
    std::list<std::string> * not_requirements = new std::list<std::string>();
    std::istringstream is_not_requirements(values.at("not_requirements"));
    while(getline(is_not_requirements, requirement, '%')) {
      not_requirements->push_back(requirement);
    }
    Attributes * attributes = new Attributes(
      name,
      type,
      tier,
      baseHp,
      baseMana,
      baseShield,
      baseDamageMult,
      baseSoulBurn,
      baseFlow,
      baseVisionRange,
      baseVisionPower,
      baseDetectionRange,
      hpIncr,
      manaIncr,
      shieldIncr,
      damageMultIncr,
      soulBurnIncr,
      flowIncr,
      *effects,
      *skills,
      *tags,
      *and_requirements,
      *or_requirements,
      *not_requirements
    );
    database->addAttributes(attributes);
    delete effects;
    delete skills;
    delete tags;
    delete and_requirements;
    delete or_requirements;
    delete not_requirements;
    return name;
  }

  void BlockOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int32_t type = database->getTargetFromMacro(values.at("type"));
    std::string material = values.at("material");
    std::istringstream is_unwalkable(values.at("unwalkable"));
    bool unwalkable;
    is_unwalkable >> std::boolalpha >> unwalkable;
    std::istringstream is_opaque(values.at("opaque"));
    bool opaque;
    is_opaque >> std::boolalpha >> opaque;
    int32_t light = stoi(values.at("light"));
    int32_t orientation = stoi(values.at("orientation"));
    float ap_cost = _stof(values.at("ap_cost"));
    Block * block = new Block(name, type, material, unwalkable, opaque, light, orientation, ap_cost);
    database->addBlock(block);
    // add all type of blocks for solids
    if(type == BLOCK_SOLID) {
      database->addBlock(new Block(name + "_SLOPE#EAST", BLOCK_SLOPE, material, unwalkable, opaque, light, 0, ap_cost));
      database->addBlock(new Block(name + "_SLOPE#NORTH", BLOCK_SLOPE, material, unwalkable, opaque, light, 90, ap_cost));
      database->addBlock(new Block(name + "_SLOPE#WEST", BLOCK_SLOPE, material, unwalkable, opaque, light, 180, ap_cost));
      database->addBlock(new Block(name + "_SLOPE#SOUTH", BLOCK_SLOPE, material, unwalkable, opaque, light, 270, ap_cost));
      database->addBlock(new Block(name + "_STAIRS#EAST", BLOCK_STAIRS, material, unwalkable, opaque, light, 0, ap_cost));
      database->addBlock(new Block(name + "_STAIRS#NORTH", BLOCK_STAIRS, material, unwalkable, opaque, light, 90, ap_cost));
      database->addBlock(new Block(name + "_STAIRS#WEST", BLOCK_STAIRS, material, unwalkable, opaque, light, 180, ap_cost));
      database->addBlock(new Block(name + "_STAIRS#SOUTH", BLOCK_STAIRS, material, unwalkable, opaque, light, 270, ap_cost));
    }
  }

  void CharacterOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    std::istringstream is_player_character(values.at("player_character"));
    bool player_character;
    is_player_character >> std::boolalpha >> player_character;
    Speech * death_speech = nullptr;
    if(values.at("death_speech") != "none") {
      death_speech = (Speech *) database->getSpeech(values.at("death_speech"));
    }
    Speech * talking_speech = nullptr;
    if(values.at("talking_speech") != "none") {
      talking_speech = (Speech *) database->getSpeech(values.at("talking_speech"));
    }
    std::istringstream is_merchant(values.at("merchant"));
    bool merchant;
    is_merchant >> std::boolalpha >> merchant;
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_effects(values.at("effects"));
    std::string effect;
    while(getline(is_effects, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    std::list<Skill *> * skills = new std::list<Skill *>();
    std::istringstream is_skills(values.at("skills"));
    std::string skill;
    while(getline(is_skills, skill, '%')) {
      skills->push_back((Skill *) database->getSkill(skill));
    }
    std::list<Item *> * sellable_items = new std::list<Item *>();
    std::string item;
    std::istringstream is_sellable_items(values.at("sellable_items"));
    while(getline(is_sellable_items, item, '%')) {
      sellable_items->push_back((Item *) database->getItem(item));
    }
    std::list<Effect *> * sellable_effects = new std::list<Effect *>();
    std::istringstream is_sellable_effects(values.at("sellable_effects"));
    while(getline(is_sellable_effects, effect, '%')) {
      sellable_effects->push_back((Effect *) database->getEffect(effect));
    }
    std::list<Skill *> * sellable_skills = new std::list<Skill *>();
    std::istringstream is_sellable_skills(values.at("sellable_skills"));
    while(getline(is_sellable_skills, skill, '%')) {
      sellable_skills->push_back((Skill *) database->getSkill(skill));
    }
    Character * character = new Character(
      name,
      player_character,
      death_speech,
      talking_speech,
      merchant,
      *effects,
      *skills,
      *sellable_items,
      *sellable_effects,
      *sellable_skills
    );
    database->addCharacter(character);
    delete effects;
    delete skills;
    delete sellable_items;
    delete sellable_effects;
    delete sellable_skills;
  }

  void EffectOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int32_t level = stoi(values.at("level"));
    std::string attributes = values.at("attributes");
    int32_t type = database->getTargetFromMacro(values.at("type"));
    int32_t duration_type = database->getTargetFromMacro(values.at("duration_type"));
    int32_t duration = 0;
    if(duration_type == DURATION_TEMPORARY) {
      duration = stoi(values.at("duration"));
    }
    int32_t power = stoi(values.at("power"));
    int32_t damages[DAMAGE_TYPE_NUMBER] = {0};
    float damage_reductions[DAMAGE_TYPE_NUMBER] = {0.};
    if(type == EFFECT_DAMAGE || type == EFFECT_DAMAGE_BUFF) {
      damages[DAMAGE_SLASH] = stoi(values.at("DAMAGE_SLASH"));
      damages[DAMAGE_PUNCTURE] = stoi(values.at("DAMAGE_PUNCTURE"));
      damages[DAMAGE_BLUNT] = stoi(values.at("DAMAGE_BLUNT"));
      damages[DAMAGE_FIRE] = stoi(values.at("DAMAGE_FIRE"));
      damages[DAMAGE_LIGHTNING] = stoi(values.at("DAMAGE_LIGHTNING"));
      damages[DAMAGE_FROST] = stoi(values.at("DAMAGE_FROST"));
      damages[DAMAGE_POISON] = stoi(values.at("DAMAGE_POISON"));
      damages[DAMAGE_ACID] = stoi(values.at("DAMAGE_ACID"));
      damages[DAMAGE_MIND] = stoi(values.at("DAMAGE_MIND"));
      damages[DAMAGE_TRUE] = stoi(values.at("DAMAGE_TRUE"));
      damages[DAMAGE_SOUL] = stoi(values.at("DAMAGE_SOUL"));
    }
    if(type == EFFECT_DAMAGE_REDUCTION) {
      damage_reductions[DAMAGE_SLASH] = _stof(values.at("SLASH_REDUCTION"));
      damage_reductions[DAMAGE_PUNCTURE] = _stof(values.at("PUNCTURE_REDUCTION"));
      damage_reductions[DAMAGE_BLUNT] = _stof(values.at("BLUNT_REDUCTION"));
      damage_reductions[DAMAGE_FIRE] = _stof(values.at("FIRE_REDUCTION"));
      damage_reductions[DAMAGE_LIGHTNING] = _stof(values.at("LIGHTNING_REDUCTION"));
      damage_reductions[DAMAGE_FROST] = _stof(values.at("COLD_REDUCTION"));
      damage_reductions[DAMAGE_POISON] = _stof(values.at("POISON_REDUCTION"));
      damage_reductions[DAMAGE_ACID] = 0.;
      damage_reductions[DAMAGE_MIND] = _stof(values.at("MIND_REDUCTION"));
      damage_reductions[DAMAGE_TRUE] = 0.;
      damage_reductions[DAMAGE_SOUL] = 0.;
    }
    Effect * effect = new Effect(name, 0, level, attributes, type, duration_type, power, duration, damages, damage_reductions);
    database->addEffect(effect);
  }

  void EventOpener(std::string fileName, Database * database) {}

  void GearOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    std::string mantle_str = values.at("mantle");
    ArmorItem * mantle = mantle_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(mantle_str.substr(0, mantle_str.find('|'))), stoi(mantle_str.substr(mantle_str.find('|') + 1, mantle_str.length())), 1
    ) : nullptr;
    std::string helmet_str = values.at("helmet");
    ArmorItem * helmet = helmet_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(helmet_str.substr(0, helmet_str.find('|'))), stoi(helmet_str.substr(helmet_str.find('|') + 1, helmet_str.length())), 1
    ) : nullptr;
    std::string armor_str = values.at("armor");
    ArmorItem * armor = armor_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(armor_str.substr(0, armor_str.find('|'))), stoi(armor_str.substr(armor_str.find('|') + 1, armor_str.length())), 1
    ) : nullptr;
    std::string gauntlets_str = values.at("gauntlets");
    ArmorItem * gauntlets = gauntlets_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(gauntlets_str.substr(0, gauntlets_str.find('|'))), stoi(gauntlets_str.substr(gauntlets_str.find('|') + 1, gauntlets_str.length())), 1
    ) : nullptr;
    std::string boots_str = values.at("boots");
    ArmorItem * boots = boots_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(boots_str.substr(0, boots_str.find('|'))), stoi(boots_str.substr(boots_str.find('|') + 1, boots_str.length())), 1
    ) : nullptr;
    std::string lantern_str = values.at("lantern");
    ArmorItem * lantern = lantern_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(lantern_str.substr(0, lantern_str.find('|'))), stoi(lantern_str.substr(lantern_str.find('|') + 1, lantern_str.length())), 1
    ) : nullptr;
    std::string amulet_str = values.at("amulet");
    ArmorItem * amulet = amulet_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(amulet_str.substr(0, amulet_str.find('|'))), stoi(amulet_str.substr(amulet_str.find('|') + 1, amulet_str.length())), 1
    ) : nullptr;
    std::string ring_1_str = values.at("ring_1");
    ArmorItem * ring_1 = ring_1_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(ring_1_str.substr(0, ring_1_str.find('|'))), stoi(ring_1_str.substr(ring_1_str.find('|') + 1, ring_1_str.length())), 1
    ) : nullptr;
    std::string ring_2_str = values.at("ring_2");
    ArmorItem * ring_2 = ring_2_str != "none" ? (ArmorItem *) Item::init(
      database->getItem(ring_2_str.substr(0, ring_2_str.find('|'))), stoi(ring_2_str.substr(ring_2_str.find('|') + 1, ring_2_str.length())), 1
    ) : nullptr;
    std::string weapon_1_str = values.at("weapon_1");
    WeaponItem * weapon_1 = weapon_1_str != "none" ? (WeaponItem *) Item::init(
      database->getItem(weapon_1_str.substr(0, weapon_1_str.find('|'))), stoi(weapon_1_str.substr(weapon_1_str.find('|') + 1, weapon_1_str.length())), 1
    ) : nullptr;
    std::string weapon_2_str = values.at("weapon_2");
    WeaponItem * weapon_2 = weapon_2_str != "none" ? (WeaponItem *) Item::init(
      database->getItem(weapon_2_str.substr(0, weapon_2_str.find('|'))), stoi(weapon_2_str.substr(weapon_2_str.find('|') + 1, weapon_2_str.length())), 1
    ) : nullptr;
    std::string weapon_3_str = values.at("backup_weapon_1");
    WeaponItem * backup_weapon_1 = weapon_3_str != "none" ? (WeaponItem *) Item::init(
      database->getItem(weapon_3_str.substr(0, weapon_3_str.find('|'))), stoi(weapon_3_str.substr(weapon_3_str.find('|') + 1, weapon_3_str.length())), 1
    ) : nullptr;
    std::string weapon_4_str = values.at("backup_weapon_2");
    WeaponItem * backup_weapon_2 = weapon_4_str != "none" ? (WeaponItem *) Item::init(
      database->getItem(weapon_4_str.substr(0, weapon_4_str.find('|'))), stoi(weapon_4_str.substr(weapon_4_str.find('|') + 1, weapon_4_str.length())), 1
    ) : nullptr;
    std::string bag_str = values.at("bag");
    ContainerItem * bag = bag_str != "none" ? (ContainerItem *) Item::init(
      database->getItem(bag_str.substr(0, bag_str.find('|'))), stoi(bag_str.substr(bag_str.find('|') + 1, bag_str.length())), 1
    ) : nullptr;
    std::string belt_str = values.at("belt");
    ContainerItem * belt = belt_str != "none" ? (ContainerItem *) Item::init(
      database->getItem(belt_str.substr(0, belt_str.find('|'))), stoi(belt_str.substr(belt_str.find('|') + 1, belt_str.length())), 1
    ) : nullptr;
    std::list<Item *> * items = new std::list<Item *>();
    std::istringstream is_items(values.at("items"));
    std::string item;
    while(getline(is_items, item, '%')) {
      std::string item_name = item.substr(0, item.find('|'));
      item = item.substr(item.find('|') + 1, item.length());
      int32_t tier = stoi(item.substr(0, item.find('|')));
      int32_t number = stoi(item.substr(item.find('|') + 1, item.length()));
      items->push_back(Item::init(database->getItem(item_name), tier, number));
    }
    Gear * gear = new Gear(
      name,
      mantle,
      helmet,
      armor,
      gauntlets,
      boots,
      lantern,
      amulet,
      ring_1,
      ring_2,
      weapon_1,
      weapon_2,
      backup_weapon_1,
      backup_weapon_2,
      bag,
      belt,
      *items
    );
    database->addGear(gear);
    delete items;
  }

  std::string ItemOpener(std::string fileName, Database * database) {
    std::map<const std::string, std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int32_t type = database->getTargetFromMacro(values.at("type"));
    int32_t type2 = database->getTargetFromMacro(values.at("type2"));
    int32_t max_tier = stoi(values.at("tier"));
    float weight = _stof(values.at("weight"));
    int32_t sizeX = stoi(values.at("sizeX"));
    int32_t sizeY = stoi(values.at("sizeY"));
    int32_t gold_value = stoi(values.at("gold_value"));
    std::istringstream is_droppable(values.at("droppable"));
    bool droppable;
    is_droppable >> std::boolalpha >> droppable;
    std::istringstream is_usable(values.at("usable"));
    bool usable;
    is_usable >> std::boolalpha >> usable;
    std::istringstream is_consumable(values.at("consumable"));
    bool consumable;
    is_consumable >> std::boolalpha >> consumable;
    int32_t use_time = 0;
    if(usable) {
      use_time = stoi(values.at("use_time"));
    }
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_3(values.at("effects"));
    std::string effect;
    while(getline(is_3, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    Item * item;
    if(type == ITEM_ARMOR) {
      int32_t swap_time = stoi(values.at("swap_time"));
      float damage_reductions[DAMAGE_TYPE_NUMBER] = {0.};
      damage_reductions[DAMAGE_SLASH] = _stof(values.at("SLASH_REDUCTION"));
      damage_reductions[DAMAGE_PUNCTURE] = _stof(values.at("PUNCTURE_REDUCTION"));
      damage_reductions[DAMAGE_BLUNT] = _stof(values.at("BLUNT_REDUCTION"));
      damage_reductions[DAMAGE_FIRE] = _stof(values.at("FIRE_REDUCTION"));
      damage_reductions[DAMAGE_LIGHTNING] = _stof(values.at("LIGHTNING_REDUCTION"));
      damage_reductions[DAMAGE_FROST] = _stof(values.at("FROST_REDUCTION"));
      damage_reductions[DAMAGE_POISON] = _stof(values.at("POISON_REDUCTION"));
      damage_reductions[DAMAGE_ACID] = 0.;
      damage_reductions[DAMAGE_MIND] = _stof(values.at("MIND_REDUCTION"));
      damage_reductions[DAMAGE_TRUE] = 0.;
      damage_reductions[DAMAGE_SOUL] = 0.;
      item = new ArmorItem(
        name,
        0,
        type,
        type2,
        0,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        swap_time,
        damage_reductions
      );
    }
    if(type == ITEM_WEAPON) {
      int32_t swap_time = stoi(values.at("swap_time"));
      float range = _stof(values.at("range"));
      int32_t strike_time = stoi(values.at("strike_time"));
      std::istringstream is_use_projectile(values.at("use_projectile"));
      bool use_projectile;
      is_use_projectile >> std::boolalpha >> use_projectile;
      bool use_ammo = false;
      int32_t ammo_type = 0;
      int32_t capacity = 0;
      int32_t reload_time = 0;
      if(use_projectile) {
        ammo_type = database->getTargetFromMacro(values.at("ammo_type"));
        std::istringstream is_use_ammo(values.at("use_ammo"));
        is_use_ammo >> std::boolalpha >> use_ammo;
        if(use_ammo) {
          capacity = stoi(values.at("capacity"));
          reload_time = stoi(values.at("reload_time"));
        }
      }
      int32_t damages[DAMAGE_TYPE_NUMBER] = {0};
      damages[DAMAGE_SLASH] = stoi(values.at("DAMAGE_SLASH"));
      damages[DAMAGE_PUNCTURE] = stoi(values.at("DAMAGE_PUNCTURE"));
      damages[DAMAGE_BLUNT] = stoi(values.at("DAMAGE_BLUNT"));
      damages[DAMAGE_FIRE] = stoi(values.at("DAMAGE_FIRE"));
      damages[DAMAGE_LIGHTNING] = stoi(values.at("DAMAGE_LIGHTNING"));
      damages[DAMAGE_FROST] = stoi(values.at("DAMAGE_FROST"));
      damages[DAMAGE_POISON] = stoi(values.at("DAMAGE_POISON"));
      damages[DAMAGE_ACID] = stoi(values.at("DAMAGE_ACID"));
      damages[DAMAGE_MIND] = stoi(values.at("DAMAGE_MIND"));
      damages[DAMAGE_TRUE] = stoi(values.at("DAMAGE_TRUE"));
      damages[DAMAGE_SOUL] = stoi(values.at("DAMAGE_SOUL"));
      item = new WeaponItem(
        name,
        0,
        type,
        type2,
        0,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        swap_time,
        range,
        strike_time,
        use_projectile,
        use_ammo,
        ammo_type,
        capacity,
        reload_time,
        nullptr,
        damages
      );
    }
    if(type == ITEM_BASIC) {
      item = new BasicItem(
        name,
        0,
        type,
        type2,
        0,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects
      );
    }
    if(type == ITEM_SERIAL) {
      int32_t max_stack = stoi(values.at("max_stack"));
      item = new SerialItem(
        name,
        0,
        type,
        type2,
        0,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        0,
        max_stack
      );
    }
    if(type == ITEM_AMMUNITION) {
      int32_t max_stack = stoi(values.at("max_stack"));
      Projectile * projectile = (Projectile *) database->getProjectile(values.at("projectile"));
      item = new AmmunitionItem(
        name,
        0,
        type,
        type2,
        0,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        0,
        max_stack,
        projectile
      );
    }
    if(type == ITEM_CONTAINER) {
      int32_t swap_time = stoi(values.at("swap_time"));
      std::istringstream is_can_take_from(values.at("can_take_from"));
      bool can_take_from;
      is_can_take_from >> std::boolalpha >> can_take_from;
      std::istringstream is_repercute_weight(values.at("repercute_weight"));
      bool repercute_weight;
      is_repercute_weight >> std::boolalpha >> repercute_weight;
      std::istringstream is_limited(values.at("limited"));
      bool limited;
      is_limited >> std::boolalpha >> limited;
      int32_t limit_type = stoi(values.at("limit_type"));
      int32_t contentX = stoi(values.at("contentX"));
      int32_t contentY = stoi(values.at("contentY"));
      item = new ContainerItem(
        name,
        0,
        type,
        type2,
        0,
        max_tier,
        weight,
        sizeX,
        sizeY,
        gold_value,
        droppable,
        usable,
        consumable,
        use_time,
        *effects,
        swap_time,
        can_take_from,
        repercute_weight,
        limited,
        limit_type,
        contentX,
        contentY
      );
    }
    database->addItem(item);
    delete effects;
    return name;
  }

  
  std::string FurnitureOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int32_t type = database->getTargetFromMacro(values.at("type"));
    int32_t sizeX = stoi(values.at("sizeX"));
    int32_t sizeY = stoi(values.at("sizeY"));
    int32_t sizeZ = stoi(values.at("sizeZ"));
    std::istringstream is_unwalkable(values.at("unwalkable"));
    bool unwalkable;
    is_unwalkable >> std::boolalpha >> unwalkable;
    std::istringstream is_opaque(values.at("opaque"));
    bool opaque;
    is_opaque >> std::boolalpha >> opaque;
    std::istringstream is_solid(values.at("solid"));
    bool solid;
    is_solid >> std::boolalpha >> solid;
    int32_t light = stoi(values.at("light"));
    float fire_size = 0.F;
    float fire_posX = 0.F;
    float fire_posY = 0.F;
    float fire_posZ = 0.F;
    if(light != 0) {
      fire_size = stof(values.at("fire_size"));
      fire_posX = stof(values.at("fire_posX"));
      fire_posY = stof(values.at("fire_posY"));
      fire_posZ = stof(values.at("fire_posZ"));
    }
    float activation_time = 0;
    if(type != FURNITURE_BASIC) {
      activation_time = stof(values.at("activation_time"));
    }
    Furniture * furniture;
    switch(type) {
      case FURNITURE_BASIC:
        furniture = new BasicFurniture(name, type, sizeX, sizeY, sizeZ, unwalkable, opaque, solid, light, fire_size, fire_posX, fire_posY, fire_posZ);
        break;
      case FURNITURE_CONTAINER:
        furniture = new ContainerFurniture(name, type, sizeX, sizeY, sizeZ, unwalkable, opaque, solid, light, activation_time, fire_size, fire_posX, fire_posY, fire_posZ);
        break;
      case FURNITURE_CRAFTING:
        furniture = new CraftingFurniture(name, type, sizeX, sizeY, sizeZ, unwalkable, opaque, solid, light, activation_time, fire_size, fire_posX, fire_posY, fire_posZ);
        break;
      case FURNITURE_LINKED:
        furniture = new LinkedFurniture(name, type, sizeX, sizeY, sizeZ, unwalkable, opaque, solid, light, activation_time, fire_size, fire_posX, fire_posY, fire_posZ);
        break;
      case FURNITURE_SKILL: {
        Skill * skill = (Skill *) database->getSkill(values.at("skill"));
        furniture = new SkillFurniture(name, type, sizeX, sizeY, sizeZ, unwalkable, opaque, solid, light, activation_time, fire_size, fire_posX, fire_posY, fire_posZ, skill);
        break;
      }
      case FURNITURE_SWITCH: {
        std::istringstream is_unwalkable_off(values.at("unwalkable_off"));
        bool unwalkable_off;
        is_unwalkable_off >> std::boolalpha >> unwalkable_off;
        std::istringstream is_opaque_off(values.at("opaque_off"));
        bool opaque_off;
        is_opaque_off >> std::boolalpha >> opaque_off;
        std::istringstream is_solid_off(values.at("solid_off"));
        bool solid_off;
        is_solid_off >> std::boolalpha >> solid_off;
        int32_t light_off = stoi(values.at("light_off"));
        // case where the light source is off when the furniture is on
        if(light == 0 && light_off > 0) {
          fire_size = stof(values.at("fire_size"));
          fire_posX = stof(values.at("fire_posX"));
          fire_posY = stof(values.at("fire_posY"));
          fire_posZ = stof(values.at("fire_posZ"));
        }
        furniture = new SwitchFurniture(name, type, sizeX, sizeY, sizeZ, unwalkable, opaque, solid, light, activation_time, fire_size, fire_posX, fire_posY, fire_posZ, unwalkable_off, opaque_off, solid_off, light_off);
        break;
      }
    }
    database->addFurniture(furniture);
    return name;
  }

  void MapOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int32_t sizeX = stoi(values.at("sizeX"));
    int32_t sizeY = stoi(values.at("sizeY"));
    int32_t sizeZ = stoi(values.at("sizeZ"));

    Map * map = new Map(name, sizeX, sizeY, sizeZ, LIGHTENING_INDOORS);

    std::fstream file;
    std::string line;
    std::string os_fileName = std::regex_replace(fileName, std::regex("/"), PATH_DELIMITER);
    file.open(os_fileName, std::ios::in);
    if(!file) {
      std::cout << "File not found: " + fileName << std::endl;
    }
    // skip lines until we reach the map itself
    while(getline(file, line) && line != "!end");
    // tiles
    for(int32_t z = 0; z < sizeZ; z++) {
      // skip line
      getline(file, line);
      for(int32_t y = sizeY - 1; y >= 0; y--) {
        getline(file, line);
        std::istringstream is(line);
        for(int32_t x = 0; x < sizeX; x++) {
          std::string block;
          getline(is, block, ' ');
          if(values.at(block) != "TXT_VOID") {
            map->setBlock(MathUtil::makeVector3i(x, y, z), (Block *) database->getBlock(values.at(block)));
          }
        }
      }
    }
    // furnitures
    std::string delimiter = ".";
    while(getline(file, line)) {
      while(line != "" && std::isspace(line.at(0))) {
        line = line.substr(1, line.length());
      }
      if(line != "" && line.at(0) != '#') {
        std::string keyword = line.substr(0, line.find(delimiter));
        while(std::isspace(keyword.at(keyword.length() - 1))) {
          keyword = keyword.substr(0, keyword.length() - 1);
        }
        std::string command = line.substr(line.find(delimiter) + 1, line.length() - 1);
        while(std::isspace(command.at(0))) {
          command = command.substr(1, command.length());
        }
        while(std::isspace(command.at(command.length() - 1))) {
          command = command.substr(0, command.length() - 1);
        }
        addFurnitureToMap(database->getTargetFromMacro(keyword), command, map, database);
      }
    }
    //map->calculateLights();
    file.close();
    database->addMap(map);
  }

  void addFurnitureToMap(int32_t keyword, std::string command, Map * map, Database * database) {
    std::stringstream * ss = new std::stringstream(command);
    std::string name = String::extract(ss);
    int32_t x = String::extract_int(ss);
    int32_t y = String::extract_int(ss);
    int32_t z = String::extract_int(ss);
    float orientation = String::extract_float(ss);
    if(keyword == FURNITURE_BASIC) {
      map->addFurniture(new BasicFurniture( (BasicFurniture *) database->getFurniture(name), x, y, z, orientation));
    }
    else {
      bool isLocked = String::extract_bool(ss);
      std::string key_name = "";
      if(isLocked) {
        key_name = String::extract(ss);
      }
      if(keyword == FURNITURE_SWITCH) {
        map->addFurniture(new SwitchFurniture( (SwitchFurniture *) database->getFurniture(name), x, y, z, orientation, isLocked, key_name));
      }
      if(keyword == FURNITURE_CRAFTING) {
        map->addFurniture(new CraftingFurniture( (CraftingFurniture *) database->getFurniture(name), x, y, z, orientation, isLocked, key_name));
      }
      if(keyword == FURNITURE_SKILL) {
        map->addFurniture(new SkillFurniture( (SkillFurniture *) database->getFurniture(name), x, y, z, orientation, isLocked, key_name));
      }
      if(keyword == FURNITURE_CONTAINER) {
        int64_t gold = String::extract_long(ss);
        std::stringstream * ss_items = new std::stringstream(String::extract(ss));
        std::list<Item *> * items = new std::list<Item *>();
        while(ss_items->rdbuf()->in_avail() != 0) {
          items->push_back((Item *) database->getItem(String::extract(ss_items)));
        }
        map->addFurniture(new ContainerFurniture( (ContainerFurniture *) database->getFurniture(name), x, y, z, orientation, isLocked, key_name, gold, *items));
        delete ss_items;
      }
      if(keyword == FURNITURE_LINKED) {
        int64_t linked_x = String::extract_long(ss);
        int64_t linked_y = String::extract_long(ss);
        map->addFurniture(new LinkedFurniture( (LinkedFurniture *) database->getFurniture(name), x, y, z, orientation, isLocked, key_name, (ActivableFurniture *) map->getFurniture(linked_x, linked_y)));
      }
    }
  }

  void ProjectileOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int32_t projectile_type = database->getTargetFromMacro(values.at("projectile_type"));
    float size = _stof(values.at("size"));
    std::istringstream is(values.at("homing"));
    bool homing;
    is >> std::boolalpha >> homing;
    std::string skill_str = values.at("skill");
    Skill * skill = nullptr;
    if(skill_str != "none") {
      skill = (Skill *) database->getSkill(skill_str);
    }
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_effects(values.at("effects"));
    std::string effect;
    while(getline(is_effects, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    float speed = _stof(values.at("speed"));
    float area = _stof(values.at("area"));
    int32_t falloff_timer = stoi(values.at("falloff_timer"));
    float waste_per_tick = _stof(values.at("waste_per_tick"));
    float waste_per_area = _stof(values.at("waste_per_area"));
    float waste_per_hit = _stof(values.at("waste_per_hit"));
    int32_t damages[DAMAGE_TYPE_NUMBER];
    damages[DAMAGE_SLASH] = stoi(values.at("DAMAGE_SLASH"));
    damages[DAMAGE_PUNCTURE] = stoi(values.at("DAMAGE_PUNCTURE"));
    damages[DAMAGE_BLUNT] = stoi(values.at("DAMAGE_BLUNT"));
    damages[DAMAGE_FIRE] = stoi(values.at("DAMAGE_FIRE"));
    damages[DAMAGE_LIGHTNING] = stoi(values.at("DAMAGE_LIGHTNING"));
    damages[DAMAGE_FROST] = stoi(values.at("DAMAGE_FROST"));
    damages[DAMAGE_POISON] = stoi(values.at("DAMAGE_POISON"));
    damages[DAMAGE_ACID] = stoi(values.at("DAMAGE_ACID"));
    damages[DAMAGE_MIND] = stoi(values.at("DAMAGE_ACID"));
    damages[DAMAGE_TRUE] = stoi(values.at("DAMAGE_TRUE"));
    damages[DAMAGE_SOUL] = stoi(values.at("DAMAGE_SOUL"));
    Projectile * projectile = new Projectile(name, projectile_type, size, homing, skill, *effects, speed, area, falloff_timer, waste_per_tick, waste_per_area, waste_per_hit, damages);
    database->addProjectile(projectile);
    delete effects;
  }

  void QuestOpener(std::string fileName, Database * database) {}

  void SettingsOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    Settings::setTickDurationInSeconds(_stof(values.at("TICK_DURATION")));
    Settings::setMaxNumberOfDaysAwake(stoi(values.at("MAX_NUMBER_DAYS_AWAKE")));
    Settings::setMaxNumberOfDaysFasting(stoi(values.at("MAX_NUMBER_DAYS_FASTING")));
    Settings::setStaminaRecoveryRatio(stoi(values.at("STAMINA_RECOVERY_RATIO")));
    Settings::setSatietyRecoveryRatio(stoi(values.at("SATIETY_RECOVERY_RATIO")));
    Settings::setStaminaOverextendRatio(stoi(values.at("STAMINA_OVEREXTEND_RATIO")));
    Settings::setSatietyOverextendRatio(stoi(values.at("SATIETY_OVEREXTEND_RATIO")));
    Settings::setBuyingPriceModifier(_stof(values.at("BUYING_PRICE_MODIFIER")));
    Settings::setPort(stoi(values.at("PORT")));
    std::string seed = values.at("SEED");
    seed == "rand" ? Settings::setSeed(time(0)) : Settings::setSeed(stol(seed));
    Settings::setMasterPassword(values.at("MASTER_PASSWORD"));
  }

  void ClientSettingsOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    ClientSettings::setPort(stoi(values.at("PORT")));
    ClientSettings::setLang(values.at("LANG"));
  }

  void SkillOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int32_t level = stoi(values.at("level"));
    std::string attributes = values.at("attributes");
    int32_t target_type = database->getTargetFromMacro(values.at("target_type"));
    std::istringstream is(values.at("is_instant"));
    bool is_INSTANT_DURATION;
    is >> std::boolalpha >> is_INSTANT_DURATION;
    int32_t overcharge_power_type = database->getTargetFromMacro(values.at("overcharge_power_type"));
    int32_t overcharge_duration_type = database->getTargetFromMacro(values.at("overcharge_duration_type"));
    int32_t overcharge_range_type = database->getTargetFromMacro(values.at("overcharge_range_type"));
    int32_t range = stoi(values.at("range"));
    int32_t time = stoi(values.at("time"));
    std::list<PseudoSkill *> * skills = new std::list<PseudoSkill *>();
    std::istringstream is_skills(values.at("skills"));
    std::string pseudoSkill;
    while(getline(is_skills, pseudoSkill, '%')) {
      skills->push_back((PseudoSkill *) database->getPseudoSkill(pseudoSkill));
    }
    Skill * skill = new Skill(name, level, attributes, target_type, is_INSTANT_DURATION, overcharge_power_type, overcharge_duration_type, overcharge_range_type, range, time, *skills);
    database->addSkill(skill);
    delete skills;
  }

  void PseudoSkillOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int32_t skill_type = database->getTargetFromMacro(values.at("skill_type"));
    int32_t target_type = database->getTargetFromMacro(values.at("target_type"));
    int32_t mana_cost = stoi(values.at("mana_cost"));
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_effects(values.at("effects"));
    std::string effect;
    PseudoSkill * pseudoSkill;
    while(getline(is_effects, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    switch(skill_type) {
      case SKILL_SIMPLE:
        pseudoSkill = new SimpleSkill(name, skill_type, target_type, mana_cost, *effects);
        break;
      case SKILL_PROJECTILE: {
        Projectile * projectile = (Projectile *) database->getProjectile(values.at("projectile"));
        pseudoSkill = new ProjectileSkill(name, skill_type, target_type, mana_cost, *effects, projectile);
        break;
      }
      case SKILL_TELEPORT: {
        int32_t apparition_type = database->getTargetFromMacro(values.at("apparition_type"));
        int32_t movement_type = database->getTargetFromMacro(values.at("movement_type"));
        pseudoSkill = new TeleportSkill(name, skill_type, target_type, mana_cost, *effects, apparition_type, movement_type);
        break;
      }
      case SKILL_TILE_SWAP: {
        Block * current_block = (Block *) database->getBlock(values.at("current_block"));
        Block * new_block = (Block *) database->getBlock(values.at("new_block"));
        pseudoSkill = new BlockSwapSkill(name, skill_type, target_type, mana_cost, *effects, current_block, new_block);
        break;
      }
      case SKILL_SUMMON: {
        Character * character = (Character *) database->getCharacter(values.at("character"));
        std::string ai_str = values.at("ai");
        std::string team = values.at("team");
        if(team == "same") {
          team = "";
        }
        Race * race = (Race *) database->getWay(values.at("race"));
        std::list<Race *> * race_modifiers = new std::list<Race *>();
        std::istringstream is_race_modifiers(values.at("race_modifiers"));
        std::string modifier;
        while(getline(is_race_modifiers, modifier, '%') && modifier != "none") {
          race_modifiers->push_back((Race *) database->getWay(modifier));
        }
        Way * origin = (Way *) database->getWay(values.at("origin"));
        Way * culture = (Way *) database->getWay(values.at("culture"));
        Way * religion = (Way *) database->getWay(values.at("religion"));
        Way * profession = (Way *) database->getWay(values.at("profession"));
        Attributes * attributes = (Attributes *) database->getAttributes(values.at("attributes"));
        std::list<Way *> * titles = new std::list<Way *>();
        std::istringstream is_titles(values.at("titles"));
        std::string title;
        while(getline(is_titles, title, '%') && title != "none") {
          titles->push_back((Way *) database->getWay(title));
        }
        int32_t apparition_type = database->getTargetFromMacro(values.at("apparition_type"));
        int32_t xp = stoi(values.at("xp"));
        pseudoSkill = new SummonSkill(
          name,
          skill_type,
          target_type,
          mana_cost,
          *effects,
          character,
          ai_str,
          team,
          race,
          *race_modifiers,
          origin,
          culture,
          religion,
          profession,
          attributes,
          *titles,
          apparition_type,
          xp
        );
        delete race_modifiers;
        delete titles;
        break;
      }
      default:
        pseudoSkill = new SimpleSkill(name, skill_type, target_type, mana_cost, *effects);
    }
    database->addPseudoSkill(pseudoSkill);
    delete effects;
  }

  void SpeechOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int32_t type = database->getTargetFromMacro(values.at("type"));
    std::istringstream is_empty(values.at("empty"));
    bool empty;
    is_empty >> std::boolalpha >> empty;
    std::istringstream is_constant(values.at("constant"));
    bool constant;
    is_constant >> std::boolalpha >> constant;
    std::map<const std::string, const std::string> * options = new std::map<const std::string, const std::string>();
    std::istringstream is_options(values.at("options"));
    std::string option;
    while(getline(is_options, option, '%')) {
      std::string pair1 = option.substr(0, option.find('|'));
      std::string pair2 = option.substr(option.find('|') + 1, option.length());
      options->insert(std::make_pair(pair1, pair2));
    }
    Speech * speech = new Speech(name, type, empty, constant, *options);
    database->addSpeech(speech);
    delete options;
  }

  std::string WayOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromFile(fileName);
    std::string name = values.at("name");
    int32_t type = database->getTargetFromMacro(values.at("type"));
    int32_t tier = stoi(values.at("tier"));
    int32_t baseHp = stoi(values.at("baseHp"));
    int32_t baseMana = stoi(values.at("baseMana"));
    int32_t baseShield = stoi(values.at("baseShield"));
    int32_t baseDamageMult = stoi(values.at("baseDamageMult"));
    int32_t baseSoulBurn = stoi(values.at("baseSoulBurn"));
    int32_t baseFlow = stoi(values.at("baseFlow"));
    int32_t baseVisionRange = stoi(values.at("baseVisionRange"));
    int32_t baseVisionPower = stoi(values.at("baseVisionPower"));
    int32_t baseDetectionRange = stoi(values.at("baseDetectionRange"));
    int32_t hpIncr = stoi(values.at("hpIncr"));
    int32_t manaIncr = stoi(values.at("manaIncr"));
    int32_t shieldIncr = stoi(values.at("shieldIncr"));
    int32_t damageMultIncr = stoi(values.at("damageMultIncr"));
    int32_t soulBurnIncr = stoi(values.at("soulBurnIncr"));
    int32_t flowIncr = stoi(values.at("flowIncr"));
    std::list<Effect *> * effects = new std::list<Effect *>();
    std::istringstream is_effects(values.at("effects"));
    std::string effect;
    while(getline(is_effects, effect, '%')) {
      effects->push_back((Effect *) database->getEffect(effect));
    }
    std::list<Skill *> * skills = new std::list<Skill *>();
    std::istringstream is_skills(values.at("skills"));
    std::string skill;
    while(getline(is_skills, skill, '%')) {
      skills->push_back((Skill *) database->getSkill(skill));
    }
    std::list<std::string> * tags = new std::list<std::string>();
    std::istringstream is_tags(values.at("tags"));
    std::string tag;
    while(getline(is_tags, tag, '%')) {
      tags->push_back(tag);
    }
    if(type == WAY_RACE) {
      int32_t race_type = database->getTargetFromMacro(values.at("race_type"));
      float size = _stof(values.at("size"));
      float height = _stof(values.at("height"));
      std::istringstream is_need_to_eat(values.at("need_to_eat"));
      bool need_to_eat;
      is_need_to_eat >> std::boolalpha >> need_to_eat;
      std::istringstream is_can_eat_food(values.at("can_eat_food"));
      bool can_eat_food;
      is_can_eat_food >> std::boolalpha >> can_eat_food;
      std::istringstream is_need_to_sleep(values.at("need_to_sleep"));
      bool need_to_sleep;
      is_need_to_sleep >> std::boolalpha >> need_to_sleep;
      std::istringstream is_has_soulspark(values.at("has_soulspark"));
      bool has_soulspark;
      is_has_soulspark >> std::boolalpha >> has_soulspark;
      float action_time_modifier = _stof(values.at("action_time_modifier"));
      float strike_time_modifier = _stof(values.at("strike_time_modifier"));
      float skill_time_modifier = _stof(values.at("skill_time_modifier"));
      float movement_time_modifier = _stof(values.at("movement_time_modifier"));
      std::list<Item *> * loots = new std::list<Item *>();
      std::istringstream is_loot(values.at("loot"));
      std::string loot;
      while(getline(is_loot, loot, '%')) {
        loots->push_back((Item *) database->getItem(loot));
      }
      Way * way = new Race(
        name,
        type,
        tier,
        baseHp,
        baseMana,
        baseShield,
        baseDamageMult,
        baseSoulBurn,
        baseFlow,
        baseVisionRange,
        baseVisionPower,
        baseDetectionRange,
        hpIncr,
        manaIncr,
        shieldIncr,
        damageMultIncr,
        soulBurnIncr,
        flowIncr,
        *effects,
        *skills,
        *tags,
        race_type,
        size,
        height,
        need_to_eat,
        can_eat_food,
        need_to_sleep,
        has_soulspark,
        action_time_modifier,
        strike_time_modifier,
        skill_time_modifier,
        movement_time_modifier,
        *loots
      );
      database->addWay(way);
      delete loots;
      delete effects;
      delete skills;
      delete tags;
    }
    else {
      Way * way = new Way(
        name,
        type,
        tier,
        baseHp,
        baseMana,
        baseShield,
        baseDamageMult,
        baseSoulBurn,
        baseFlow,
        baseVisionRange,
        baseVisionPower,
        baseDetectionRange,
        hpIncr,
        manaIncr,
        shieldIncr,
        damageMultIncr,
        soulBurnIncr,
        flowIncr,
        *effects,
        *skills,
        *tags
      );
      database->addWay(way);
      delete effects;
      delete skills;
      delete tags;
    }
    return name;
  }

  void FileOpener(std::string fileName, Database * database, bool isServer) {
    std::fstream file;
    std::string resFileName = std::regex_replace(fileName, std::regex("data/"), "resources/");
    if(fileName.find("/attributes/") != std::string::npos) {
      std::string attributes_name = AttributesOpener(fileName, database);
      if(!isServer) {
        database->addAttributesFile(attributes_name, std::regex_replace(resFileName, std::regex(".data"), ".png"));
      }
    }
    else if(fileName.find("/blocks/") != std::string::npos) {
      BlockOpener(fileName, database);
    }
    else if(fileName.find("/characters/") != std::string::npos) {
      CharacterOpener(fileName, database);
    }
    else if(fileName.find("/effects/") != std::string::npos) {
      EffectOpener(fileName, database);
    }
    else if(fileName.find("/events/") != std::string::npos) {
      EventOpener(fileName, database);
    }
    else if(fileName.find("/gears/") != std::string::npos) {
      GearOpener(fileName, database);
    }
    else if(fileName.find("/items/") != std::string::npos) {
      std::string item_name = ItemOpener(fileName, database);
      if(!isServer) {
        database->addItemFile(item_name, std::regex_replace(resFileName, std::regex(".data"), ".png"));
      }
    }
    else if(fileName.find("/furnitures/") != std::string::npos) {
      std::string furniture_name = FurnitureOpener(fileName, database);
      if(!isServer) {
        database->addFurnitureFile(furniture_name, std::regex_replace(resFileName, std::regex(".data"), ".glb"));
      }
    }
    else if(fileName.find("/maps/") != std::string::npos) {
      MapOpener(fileName, database);
    }
    else if(fileName.find("/projectiles/") != std::string::npos) {
      ProjectileOpener(fileName, database);
    }
    else if(fileName.find("/quests/") != std::string::npos) {
      QuestOpener(fileName, database);
    }
    else if(fileName.find("/skills/") != std::string::npos) {
      SkillOpener(fileName, database);
    }
    else if(fileName.find("/pseudoskills/") != std::string::npos) {
      PseudoSkillOpener(fileName, database);
    }
    else if(fileName.find("/speechs/") != std::string::npos) {
      SpeechOpener(fileName, database);
    }
    else if(fileName.find("/ways/") != std::string::npos) {
      std::string way_name = WayOpener(fileName, database);
      if(!isServer) {
        database->addWayFile(way_name, std::regex_replace(resFileName, std::regex(".data"), ".png"));
      }
    }
  }

  Database * DatabaseOpener(std::string fileName, bool isServer) {
    Database * database = new Database();
    std::fstream file;
    std::string os_fileName = std::regex_replace(fileName, std::regex("/"), PATH_DELIMITER);
    file.open(os_fileName, std::ios::in);
    if(!file) {
      std::cout << "File not found: " + fileName << std::endl;
    }
    std::string line;
    while(getline(file,line)) {
      while(line != "" && std::isspace(line.at(0))) {
        line = line.substr(1, line.length());
      }
      if(line != "" && line.at(0) != '#') {
        while(std::isspace(line.at(line.length() - 1))) {
          line = line.substr(0, line.length() - 1);
        }
        FileOpener(line, database, isServer);
      }
    }
    file.close();
    return database;
  }
}
