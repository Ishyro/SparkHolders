#ifndef _STANCE_H_
#define _STANCE_H_

#include <list>
#include <string>
#include <vector>

#include "Values.h"

class Stance {
  public:
    const std::string name;
    const bool magical;
    const int32_t attack_skills_number;
    Stance(
      std::string name,
      bool magical,
      int32_t attack_skills_number,
      std::vector<Skill *> attack_skills,
      Skill * block_skill,
      std::list<int32_t> weapon_types
    ):
      name(name),
      magical(magical),
      attack_skills_number(attack_skills_number),
      attack_skills(std::vector<Skill *>()),
      block_skill(block_skill),
      weapon_types(std::list<int32_t>())
    {
      for(Skill * skill : attack_skills) {
        this->attack_skills.push_back(skill);
      }
      for(int32_t weapon_type : weapon_types) {
        this->weapon_types.push_back(weapon_type);
      }
    }
    Skill * getAttack(int32_t indice);
    Skill * getBlock();
    bool isValid(int32_t weapon_type);
    bool operator == (const Stance& s) const { return name == s.name; }
    bool operator != (const Stance& s) const { return !operator==(s); }
  private:
    std::vector<Skill *> attack_skills;
    Skill * block_skill;
    std::list<int32_t> weapon_types;
};

#endif // _STANCE_H_
