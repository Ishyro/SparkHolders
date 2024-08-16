#ifndef _SKILL_ACTION_H_
#define _SKILL_ACTION_H_

#include "data/actions/TargetedAction.h"

#include "Values.h"

class SkillAction : public TargetedAction {
  public:
    const int32_t mana_cost;
    SkillAction(
      const int32_t type,
      Adventure * adventure,
      const Action * previous,
      Character * user,
      Target * target,
      Skill * skill,
      const int32_t mana_cost
    ):
      TargetedAction(
        type,
        adventure,
        previous,
        user,
        target
      ),
      skill(skill),
      mana_cost(mana_cost)
    {}
    Action * execute(Adventure * adventure);
    void computeTime(Adventure * adventure);
    Skill * getSkill();
  private:
    Skill * skill;
};

#endif // _SKILL_ACTION_H_
