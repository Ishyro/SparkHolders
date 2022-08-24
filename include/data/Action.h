#ifndef _ACTION_H_
#define _ACTION_H_

#include <list>
#include <string>
#include <cmath>
#include <algorithm>

#include "data/Character.h"
#include "data/skills/Skill.h"

#include "Values.h"

class Action {
  public:
    const int type;
    const int orientation;
    const Skill * skill;
    const Character * target;
    const int target_x;
    const int target_y;
    const MapLink * link;
    const std::string object;
    const int overcharge_power;
    const int overcharge_duration;
    const int overcharge_range;
    Action(
      const int type,
      Character * user,
      const int orientation,
      const Skill * skill,
      const Character * target,
      const int target_x,
      const int target_y,
      const MapLink * link,
      const std::string object,
      const int overcharge_power,
      const int overcharge_duration,
      const int overcharge_range
    ):
      type(type),
      user(user),
      orientation(orientation),
      skill(skill),
      target(target),
      target_x(target_x),
      target_y(target_y),
      link(link),
      object(object),
      overcharge_power(overcharge_power),
      overcharge_duration(overcharge_duration),
      overcharge_range(overcharge_range)
    {
      switch(type) {
        case MOVE:
        case CHANGE_MAP:
        case SHOOT:
        case FORCE_STRIKE:
          priority = user->getPriorityModifier();
          break;
        case USE_SKILL:
          priority = user->getPriorityModifier() * skill->priority;
          break;
        case REST:
        case RELOAD:
        case SWAP_GEAR:
        case GRAB:
        case USE_ITEM:
        case ECONOMICS:
          priority = 0.F;
          break;
        default:
          priority = 0.F;
      }
    }
    void execute(Adventure * adventure);
    Character * getUser();
    // highest priority means first action in the list (so first action resolved)
    bool operator < (const Action& a) const { return priority > a.priority; }
  private:
    Character * user;
    float priority;
};

#endif // _ACTION_H_
