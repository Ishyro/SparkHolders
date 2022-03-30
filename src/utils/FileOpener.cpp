#include "utils/FileOpener.h"

namespace FileOpener {

  std::map<const std::string,std::string> getValuesFromIni(std::string fileName) {
    std::map<const std::string,std::string> result = std::map<const std::string,std::string>();
    std::fstream file;
    file.open(fileName, std::ios::in);
    if(!file) {
      std::cout << "ERROR FILE" << std::endl;
    }
    std::string line;
    std::string delimiter = "=";
    while(getline(file,line)) {
      if(line.at(0) != '#') {
        while(std::isspace(line.at(0))) {
          line = line.substr(1, line.length());
        }
        std::string key = line.substr(0, line.find(delimiter));
        while(std::isspace(key.at(key.length()))) {
          key = key.substr(0, key.length() - 1);
        }
        std::string value = line.substr(line.find(delimiter), line.length());
        while(std::isspace(value.at(0))) {
          value = value.substr(1, value.length());
        }
        while(std::isspace(value.at(value.length()))) {
          value = value.substr(0, value.length() - 1);
        }
        result.insert(std::make_pair(key,value));
      }
    }
    file.close();
    return result;
  }

  void AttributesOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromIni(fileName);
    std::string name = values.at("name");
    int baseHp = stoi(values.at("baseHp"));
    int baseMana = stoi(values.at("baseMana"));
    int baseArmor = stoi(values.at("baseArmor"));
    int baseSoulBurn = stoi(values.at("baseSoulBurn"));
    int baseFlow = stoi(values.at("baseFlow"));
    int baseVisionRange = stoi(values.at("baseVisionRange"));
    int baseVisionPower = stoi(values.at("baseVisionPower"));
    int baseDetectionRange = stoi(values.at("baseDetectionRange"));
    Item * head = database->getItem(values.at("head"));
    Item * arms = database->getItem(values.at("arms"));
    Item * legs = database->getItem(values.at("legs"));
    Item * body = database->getItem(values.at("body"));
    Item * left_ring = database->getItem(values.at("left_ring"));
    Item * right_ring = database->getItem(values.at("right_ring"));
    Item * amulet = database->getItem(values.at("amulet"));
    Weapon * weapon = database->getWeapon(values.at("weapon"));
    Ammunition * ammunition = database->getAmmunition(values.at("ammunition"));
    long number = stol(values.at("number"));
    ammunition->number=number;
    Gear * gear = new Gear(head, arms, legs, body, left_ring, right_ring, amulet, weapon, ammunition);
    Attributes * attributes = new Attributes(name, baseHp, baseMana, baseArmor, baseSoulBurn, baseFlow, baseVisionRange, baseVisionPower, baseDetectionRange, gear);
    database->addAttributes(attributes);
  }

  void CharacterOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromIni(fileName);
    std::string name = values.at("name");
    /*
    player_character(player_character),
    death_speech(death_speech),
    talking_speech(talking_speech),
    type(type),
    ai(ai),
    gold(gold),
    xp(xp),
    visionRange(visionRange),
    visionPower(visionPower),
    detectionRange(detectionRange),
    stuff(stuff),
    weapons(weapons),
    ammunitions(ammunitions),
    effects(effects),
    skills(skills),
    race(race),
    origin(origin),
    culture(culture),
    religion(religion),
    profession(profession)
    */
  }

  void EffectOpener(std::string fileName, Database * database) {}

  void EventOpener(std::string fileName, Database * database) {}

  void ItemOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromIni(fileName);
    std::string name = values.at("name");
    std::istringstream is(values.at("equipable"));
    bool equipable;
    is >> std::boolalpha >> equipable;
    std::istringstream is_2(values.at("consumable"));
    bool consumable;
    is_2 >> std::boolalpha >> consumable;
    int type = database->getTargetFromMacro(values.at("type"));
    int gold_value = stoi(values.at("gold_value"));
    std::list<Effect *> effects = std::list<Effect *>();
    std::istringstream is_3(values.at("effects"));
    std::string effect;
    while(getline(is_3,effect,'%')) {
      effects.push_front(database->getEffect(effect));
    }
    float damage_reductions[DAMAGE_TYPE_NUMBER];
    damage_reductions[SLASH] = stof(values.at("SLASH_REDUCTION"));
    damage_reductions[PUNCTURE] = stof(values.at("PUNCTURE_REDUCTION"));
    damage_reductions[IMPACT] = stof(values.at("IMPACT_REDUCTION"));
    damage_reductions[FIRE] = stof(values.at("FIRE_REDUCTION"));
    damage_reductions[THUNDER] = stof(values.at("THUNDER_REDUCTION"));
    damage_reductions[COLD] = stof(values.at("COLD_REDUCTION"));
    damage_reductions[POISON] = stof(values.at("POISON_REDUCTION"));
    damage_reductions[NEUTRAL] = 0.;
    damage_reductions[TRUE] = 0.;
    damage_reductions[SOUL] = 0.;
    Item * item = new Item(name, equipable, consumable, type, gold_value, effects, damage_reductions);
    database->addItem(item);
  }

  void MapOpener(std::string fileName, Database * database) {
    std::fstream file;
    file.open(fileName, std::ios::in);
    if(!file) {
      std::cout << "ERROR FILE" << std::endl;
    }
    std::string line;
    std::string name;
    getline(file,name);
    getline(file,line);
    const long sizeX = (long) stol(line);
    getline(file,line);
    const long sizeY = (long) stol(line);
    getline(file,line);
    bool outside;
    std::istringstream is(line);
    is >> std::boolalpha >> outside;

    Map * map = new Map(name, sizeX, sizeY, outside);

    for(int x = 0; x < sizeX; x++) {
      getline(file,line);
      std::istringstream is(line);
      for(int y = 0; y < sizeY; y++) {
        std::string tile;
        getline(is,tile,' ');
        map->setTile(x,y,database->getTile(tile));
      }
    }

    for(int x = 0; x < sizeX; x++) {
      getline(file,line);
      std::istringstream is(line);
      for(int y = 0; y < sizeY; y++) {
        std::string wall;
        getline(is,wall,' ');
        if(wall != "none") {
          Character * c = new Character(database->getCharacter(wall), x, y, NORTH, map->id);
          c->applyAttributes(database->getAttributes(wall));
          map->addCharacter(c);
        }
      }
    }

    map->calculateLights();
    file.close();
    database->addMap(map);
  }

  void ProjectileOpener(std::string fileName, Database * database) {}

  void AmmunitionOpener(std::string fileName, Database * database) {}

  void QuestOpener(std::string fileName, Database * database) {}

  void SkillOpener(std::string fileName, Database * database) {}

  void SpeechOpener(std::string fileName, Database * database) {}

  void TileOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromIni(fileName);
    std::string name = values.at("name");
    std::istringstream is(values.at("untraversable"));
    bool untraversable;
    is >> std::boolalpha >> untraversable;
    int light = stoi(values.at("light"));
    Tile * tile = new Tile(name,untraversable,light);
    database->addTile(tile);
  }

  void WayOpener(std::string fileName, Database * database) {}

  void WeaponOpener(std::string fileName, Database * database) {
    std::map<const std::string,std::string> values = getValuesFromIni(fileName);
    std::string name = values.at("name");
    std::istringstream is(values.at("melee"));
    bool melee;
    is >> std::boolalpha >> melee;
    int range = database->getTargetFromMacro(values.at("range"));
    int type = database->getTargetFromMacro(values.at("type"));
    int weight = database->getTargetFromMacro(values.at("weight"));
    int gold_value = stoi(values.at("gold_value"));
    std::istringstream is_2(values.at("use_ammo"));
    bool use_ammo;
    is_2 >> std::boolalpha >> use_ammo;
    int ammo_type = database->getTargetFromMacro(values.at("ammo_type"));
    std::list<Effect *> effects = std::list<Effect *>();
    std::istringstream is_3(values.at("effects"));
    std::string effect;
    while(getline(is_3,effect,'%')) {
      effects.push_front(database->getEffect(effect));
    }
    int damages[DAMAGE_TYPE_NUMBER];
    damages[SLASH] = stoi(values.at("SLASH"));
    damages[PUNCTURE] = stoi(values.at("PUNCTURE"));
    damages[IMPACT] = stoi(values.at("IMPACT"));
    damages[FIRE] = stoi(values.at("FIRE"));
    damages[THUNDER] = stoi(values.at("THUNDER"));
    damages[COLD] = stoi(values.at("COLD"));
    damages[POISON] = stoi(values.at("POISON"));
    damages[NEUTRAL] = stoi(values.at("NEUTRAL"));
    damages[TRUE] = stoi(values.at("TRUE"));
    damages[SOUL] = stoi(values.at("SOUL"));
    Weapon * weapon = new Weapon(name, melee, range, type, weight, gold_value, use_ammo, ammo_type, effects, damages);
    database->addWeapon(weapon);
  }
}
