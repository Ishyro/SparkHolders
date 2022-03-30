#ifndef _FILE_OPENER_H_
#define _FILE_OPENER_H_

#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <map>
#include <iostream>
#include <fstream>

#include "data/Attributes.h"
#include "data/Character.h"
#include "data/Effect.h"
#include "data/Event.h"
#include "data/Item.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Quest.h"
#include "data/skills/Skill.h"
#include "data/Speech.h"
#include "data/Tile.h"
#include "data/Way.h"
#include "data/Weapon.h"

#include "data/Database.h"

#include "Values.h"

namespace FileOpener {
  std::map<const std::string,std::string> getValuesFromIni(std::string fileName);

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
  void SpeechOpener(std::string fileName, Database * database);
  void TileOpener(std::string fileName, Database * database);
  void WayOpener(std::string fileName, Database * database);
  void WeaponOpener(std::string fileName, Database * database);
};

#endif // _FILE_OPENER_H_
