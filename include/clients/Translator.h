#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_

#include <string>
#include <list>
#include <map>

class Translator {
  public:
    Translator(std::list<std::string> bases, const std::string language);
    const std::string getAttributesName(std::string to_translate);
    const std::string getAttributesDesc(std::string to_translate);
    const std::string getCharacterName(std::string to_translate);
    const std::string getCharacterDesc(std::string to_translate);
    const std::string getEffectName(std::string to_translate);
    const std::string getEffectDesc(std::string to_translate);
    const std::string getItemName(std::string to_translate);
    const std::string getItemDesc(std::string to_translate);
    const std::string getMapName(std::string to_translate);
    const std::string getMapDesc(std::string to_translate);
    const std::string getProjectileName(std::string to_translate);
    const std::string getProjectileDesc(std::string to_translate);
    const std::string getQuestName(std::string to_translate);
    const std::string getQuestDesc(std::string to_translate);
    const std::string getSpeechName(std::string to_translate);
    const std::string getSpeechDesc(std::string to_translate);
    const std::string getSkillName(std::string to_translate);
    const std::string getSkillDesc(std::string to_translate);
    const std::string getBlockName(std::string to_translate);
    const std::string getBlockDesc(std::string to_translate);
    const std::string getWayName(std::string to_translate);
    const std::string getWayDesc(std::string to_translate);
    const std::string getWeaponName(std::string to_translate);
    const std::string getWeaponDesc(std::string to_translate);
    const std::string getStandardName(std::string to_translate);
    const std::string getStandardDesc(std::string to_translate);
  private:
    void loadValuesFromFile(const std::string fileName, std::map<const std::string, const std::string> * map);
    std::map<const std::string, const std::string> attributesNames;
    std::map<const std::string, const std::string> attributesDescs;
    std::map<const std::string, const std::string> characterNames;
    std::map<const std::string, const std::string> characterDescs;
    std::map<const std::string, const std::string> effectsNames;
    std::map<const std::string, const std::string> effectsDescs;
    std::map<const std::string, const std::string> itemsNames;
    std::map<const std::string, const std::string> itemsDescs;
    std::map<const std::string, const std::string> mapsNames;
    std::map<const std::string, const std::string> mapsDescs;
    std::map<const std::string, const std::string> projectilesNames;
    std::map<const std::string, const std::string> projectilesDescs;
    std::map<const std::string, const std::string> questsNames;
    std::map<const std::string, const std::string> questsDescs;
    std::map<const std::string, const std::string> speechsNames;
    std::map<const std::string, const std::string> speechsDescs;
    std::map<const std::string, const std::string> skillsNames;
    std::map<const std::string, const std::string> skillsDescs;
    std::map<const std::string, const std::string> tilesNames;
    std::map<const std::string, const std::string> tilesDescs;
    std::map<const std::string, const std::string> waysNames;
    std::map<const std::string, const std::string> waysDescs;
    std::map<const std::string, const std::string> weaponsNames;
    std::map<const std::string, const std::string> weaponsDescs;
    std::map<const std::string, const std::string> standardNames;
    std::map<const std::string, const std::string> standardDescs;
    #ifdef _WIN32_WINNT
      const std::string PATH_DELIMITER = "\\";
    #else
      const std::string PATH_DELIMITER = "/";
    #endif
};

#endif // _TRANSLATOR_H_
