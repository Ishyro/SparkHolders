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
    const float orientation;
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
      const Action * previous,
      Character * user,
      const float orientation,
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
      previous((Action *) previous),
      next(nullptr),
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
          time = 1;
          break;
        case SHOOT:
          time = 1;
          break;
        case FORCE_STRIKE:
          time = 15;
          break;
        case USE_SKILL:
          time = 1;
          break;
        case REST:
          time = 1;
          break;
        case RELOAD:
          time = 2;
          break;
        case SWAP_GEAR:
          time = 5;
          break;
        case GRAB:
          time = 10;
          break;
        case USE_ITEM:
          time = 5;
          break;
        case ECONOMICS:
        case TALKING:
          time = 0;
          break;
        default:
          time = 0;
      }
    }
    Action * execute(Adventure * adventure);
    Character * getUser();
    int getTick();
    void setPrevious(Action * action);
    void setNext(Action * action);
    void computeTick(int tick);
    // highest priority means first action in the list (so first action resolved)
    // only matters when both actions resolve at the same tick.
    bool operator < (const Action& a) const { return user->getPriorityModifier() > a.user->getPriorityModifier(); }
  private:
    int tick;
    int time;
    Character * user;
    Action * previous;
    Action * next;
};

#endif // _ACTION_H_
