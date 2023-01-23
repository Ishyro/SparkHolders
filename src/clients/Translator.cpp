#include <fstream>
#include <iostream>
#include <stdexcept>

#include "clients/Translator.h"

Translator::Translator(std::list<std::string> bases, const std::string language) {
  attributesNames = std::map<const std::string, const std::string>();
  attributesDescs = std::map<const std::string, const std::string>();
  characterNames = std::map<const std::string, const std::string>();
  characterDescs = std::map<const std::string, const std::string>();
  effectsNames = std::map<const std::string, const std::string>();
  effectsDescs = std::map<const std::string, const std::string>();
  itemsNames = std::map<const std::string, const std::string>();
  itemsDescs = std::map<const std::string, const std::string>();
  mapsNames = std::map<const std::string, const std::string>();
  mapsDescs = std::map<const std::string, const std::string>();
  projectilesNames = std::map<const std::string, const std::string>();
  projectilesDescs = std::map<const std::string, const std::string>();
  questsNames = std::map<const std::string, const std::string>();
  questsDescs = std::map<const std::string, const std::string>();
  speechsNames = std::map<const std::string, const std::string>();
  speechsDescs = std::map<const std::string, const std::string>();
  skillsNames = std::map<const std::string, const std::string>();
  skillsDescs = std::map<const std::string, const std::string>();
  tilesNames = std::map<const std::string, const std::string>();
  tilesDescs = std::map<const std::string, const std::string>();
  waysNames = std::map<const std::string, const std::string>();
  waysDescs = std::map<const std::string, const std::string>();
  weaponsNames = std::map<const std::string, const std::string>();
  weaponsDescs = std::map<const std::string, const std::string>();
  standardNames = std::map<const std::string, const std::string>();
  standardDescs = std::map<const std::string, const std::string>();

  for(std::string base : bases) {
    std::string path = base + "/" + language + "/";
    loadValuesFromFile(path + "attributesNames.data", &attributesNames);
    loadValuesFromFile(path + "attributesDescs.data", &attributesDescs);
    loadValuesFromFile(path + "charactersNames.data", &characterNames);
    loadValuesFromFile(path + "charactersDescs.data", &characterDescs);
    loadValuesFromFile(path + "effectsNames.data", &effectsNames);
    loadValuesFromFile(path + "effectsDescs.data", &effectsDescs);
    loadValuesFromFile(path + "itemsNames.data", &itemsNames);
    loadValuesFromFile(path + "itemsDescs.data", &itemsDescs);
    loadValuesFromFile(path + "mapsNames.data", &mapsNames);
    loadValuesFromFile(path + "mapsDescs.data", &mapsDescs);
    loadValuesFromFile(path + "projectilesNames.data", &projectilesNames);
    loadValuesFromFile(path + "projectilesDescs.data", &projectilesDescs);
    loadValuesFromFile(path + "questsNames.data", &questsNames);
    loadValuesFromFile(path + "questsDescs.data", &questsDescs);
    loadValuesFromFile(path + "speechsNames.data", &speechsNames);
    loadValuesFromFile(path + "speechsDescs.data", &speechsDescs);
    loadValuesFromFile(path + "skillsNames.data", &skillsNames);
    loadValuesFromFile(path + "skillsDescs.data", &skillsDescs);
    loadValuesFromFile(path + "tilesNames.data", &tilesNames);
    loadValuesFromFile(path + "tilesDescs.data", &tilesDescs);
    loadValuesFromFile(path + "waysNames.data", &waysNames);
    loadValuesFromFile(path + "waysDescs.data", &waysDescs);
    loadValuesFromFile(path + "weaponsNames.data", &weaponsNames);
    loadValuesFromFile(path + "weaponsDescs.data", &weaponsDescs);
  }
  loadValuesFromFile("data/translations/" + language + "/standardNames.data", &standardNames);
  loadValuesFromFile("data/translations/" + language + "/standardDescs.data", &standardDescs);

}
const std::string Translator::getAttributesName(std::string to_translate) {
  try {
    return attributesNames.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getAttributesDesc(std::string to_translate) {
  try {
    return attributesDescs.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getCharacterName(std::string to_translate) {
  try {
    return characterNames.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getCharacterDesc(std::string to_translate) {
  try {
    return characterDescs.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getEffectName(std::string to_translate) {
  try {
    return effectsNames.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getEffectDesc(std::string to_translate) {
  try {
    return effectsDescs.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getItemName(std::string to_translate) {
  try {
    return itemsNames.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getItemDesc(std::string to_translate) {
  try {
    return itemsDescs.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getMapName(std::string to_translate) {
  try {
    if(to_translate.find('#') != std::string::npos) {
      return mapsNames.at(to_translate.substr(0, to_translate.find('#'))) + to_translate.substr(to_translate.find('#'), to_translate.length());
    } else {
      return mapsNames.at(to_translate);
    }
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getMapDesc(std::string to_translate) {
  try {
    if(to_translate.find('#') != std::string::npos) {
      return mapsDescs.at(to_translate.substr(0, to_translate.find('#')));
    } else {
      return mapsDescs.at(to_translate);
    }
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getProjectileName(std::string to_translate) {
  try {
    return projectilesNames.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getProjectileDesc(std::string to_translate) {
  try {
    return projectilesDescs.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getQuestName(std::string to_translate) {
  try {
    return questsNames.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getQuestDesc(std::string to_translate) {
  try {
    return questsDescs.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getSpeechName(std::string to_translate) {
  try {
    return speechsNames.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getSpeechDesc(std::string to_translate) {
  try {
    return speechsDescs.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getSkillName(std::string to_translate) {
  try {
    return skillsNames.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getSkillDesc(std::string to_translate) {
  try {
    return skillsDescs.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getTileName(std::string to_translate) {
  try {
    return tilesNames.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getTileDesc(std::string to_translate) {
  try {
    return tilesDescs.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getWayName(std::string to_translate) {
  try {
    return waysNames.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getWayDesc(std::string to_translate) {
  try {
    return waysDescs.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getWeaponName(std::string to_translate) {
  try {
    return weaponsNames.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getWeaponDesc(std::string to_translate) {
  try {
    return weaponsDescs.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getStandardName(std::string to_translate) {
  try {
    return standardNames.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}

const std::string Translator::getStandardDesc(std::string to_translate) {
  try {
    return standardDescs.at(to_translate);
  } catch (const std::out_of_range &e) {
    return to_translate;
  }
}


 void Translator::loadValuesFromFile(const std::string fileName, std::map<const std::string, const std::string> * map) {
  std::fstream file;
  file.open(fileName, std::ios::in);
  if(!file) {
    return;
  }
  std::string line;
  std::string delimiter = "=";

  while(getline(file, line, '%') && line != "") {
    while(line != "" && std::isspace(line.at(0))) {
      line = line.substr(1, line.length());
    }
    if(line != "" && line.at(0) != '#') {
      std::string key = line.substr(0, line.find(delimiter));
      while(std::isspace(key.at(key.length() - 1))) {
        key = key.substr(0, key.length() - 1);
      }
      if(line.length() - 1 > line.find(delimiter)) {
        std::string value = line.substr(line.find(delimiter) + 1, line.length() - 1);
        while(std::isspace(value.at(0))) {
          value = value.substr(1, value.length() - 1);
        }
        map->insert(std::make_pair(key,value));
      }
      else {
        map->insert(std::make_pair(key,""));
      }
    }
  }
  file.close();
}
