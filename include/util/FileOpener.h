#ifndef _FILE_OPENER_H_
#define _FILE_OPENER_H_

#include <string>
#include <list>
#include <map>

#include "Values.h"

namespace FileOpener {

  #ifdef _WIN32_WINNT
    static std::string PATH_DELIMITER = "\\";
  #else
    static std::string PATH_DELIMITER = "/";
  #endif

  std::map<const std::string,std::string> getValuesFromFile(std::string fileName);

  Adventure * AdventureOpener(std::string fileName, bool isServer);
  void executeCommand(std::string keyword, std::string command, World * world, std::list<Quest *> * quests, std::list<Event *> * events, std::list<Spawn *> * spawns, std::list<Attributes *> * startingAttributes, std::list<Way *> * startingWays, Database * database, bool isServer);

  std::string AttributesOpener(std::string fileName, Database * database);
  void CharacterOpener(std::string fileName, Database * database);
  void EffectOpener(std::string fileName, Database * database);
  void EventOpener(std::string fileName, Database * database);
  void GearOpener(std::string fileName, Database * database);
  std::string ItemOpener(std::string fileName, Database * database);
  std::string FurnitureOpener(std::string fileName, Database * database);
  void addFurnitureToMap(int keyword, std::string command, Map * map, Database * database);
  void MapOpener(std::string fileName, Database * database);
  void ProjectileOpener(std::string fileName, Database * database);
  void QuestOpener(std::string fileName, Database * database);
  void ClientSettingsOpener(std::string fileName, Database * database);
  void SettingsOpener(std::string fileName, Database * database);
  void SkillOpener(std::string fileName, Database * database);
  void PseudoSkillOpener(std::string fileName, Database * database);
  void SpeechOpener(std::string fileName, Database * database);
  std::string TileOpener(std::string fileName, Database * database);
  std::string WayOpener(std::string fileName, Database * database);

  void FileOpener(std::string fileName, Database * database, bool isServer);
  Database * DatabaseOpener(std::string fileName, bool isServer);
};

#endif // _FILE_OPENER_H_
