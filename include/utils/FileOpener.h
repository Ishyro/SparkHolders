#ifndef _FILE_OPENER_H_
#define _FILE_OPENER_H_

#include <string>
#include <list>
#include <map>

#include "Values.h"

namespace FileOpener {
  std::map<const std::string,std::string> getValuesFromFile(std::string fileName);

  Adventure * AdventureOpener(std::string fileName);
  void executeCommand(std::string keyword, std::string command, World * world, std::list<Quest *> * quests, std::list<Event *> * events, std::list<Spawn *> * spawns, std::list<Attributes *> * startingAttributes, std::list<Way *> * startingWays, Database * database);

  void AttributesOpener(std::string fileName, Database * database);
  void CharacterOpener(std::string fileName, Database * database);
  void EffectOpener(std::string fileName, Database * database);
  void EventOpener(std::string fileName, Database * database);
  void ItemOpener(std::string fileName, Database * database);
  void MapOpener(std::string fileName, Database * database);
  void ProjectileOpener(std::string fileName, Database * database);
  void AmmunitionOpener(std::string fileName, Database * database);
  void QuestOpener(std::string fileName, Database * database);
  void SkillOpener(std::string fileName, Database * database);
  void PseudoSkillOpener(std::string fileName, Database * database);
  void SpeechOpener(std::string fileName, Database * database);
  void TileOpener(std::string fileName, Database * database);
  void WayOpener(std::string fileName, Database * database);
  void WeaponOpener(std::string fileName, Database * database);

  void FileOpener(std::string fileName, Database * database);
  Database * DatabaseOpener(std::string fileName);
};

#endif // _FILE_OPENER_H_
