#ifndef _SKILL_ACTION_H_
#define _SKILL_ACTION_H_

#include "data/actions/TargetedAction.h"

#include "Values.h"

class SkillAction : public TargetedAction {
    using TargetedAction::TargetedAction;
  public:
    Action * execute(Adventure * adventure);
    void computeTime(Adventure * adventure);
    Skill * getSkill();
    int getOverchargePower();
    int getOverchargeDuration();
    int getOverchargeRange();
    void setSkill(Skill * skill);
    void setOverchargePower(int overcharge);
    void setOverchargeDuration(int overcharge);
    void setOverchargeRange(int overcharge);
  private:
    Skill * skill;
    int overcharge_power;
    int overcharge_duration;
    int overcharge_range;
};

#endif // _SKILL_ACTION_H_










#ifndef _SKILL_ACTION_H_
#define _SKILL_ACTION_H_

#include <list>
#include <string>
#include <cmath>
#include <algorithm>

#include "data/Character.h"
#include "data/skills/Skill.h"

#include "Values.h"

typedef struct Target {
  int type;
  int id;
  float x;
  float y;
} Target;

class Action {
  public:
    const int type;
    const float orientation;
    const 
    const Character * target;
    const int target_x;
    const int target_y;
    const MapLink * link;
    const std::string object;
    Action(
      const int type,
      Adventure * adventure,
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
      if(previous == nullptr) {
        computeTime(adventure);
      }
      if(type == STRIKE) {
        next = new Action(
          RESPITE,
          adventure,
          this,
          user,
          0.F,
          nullptr,
          nullptr,
          0,
          0,
          nullptr,
          "",
          1,
          1,
          1
        );
      }
    }
    Action * execute(Adventure * adventure);
    Character * getUser();
    int getTick();
    void setPrevious(Action * action);
    void setNext(Action * action);
    void computeTick(float tick);
    void computeTime(Adventure * adventure);
    static std::string target_to_string(Target * target);
    static Target * target_from_string(std::string to_read);
    bool operator < (const Action& a) const { return tick < a.tick; }
  private:
    float tick;
    float time;
    Character * user;
    Action * previous;
    Action * next;
};

#endif // _SKILL_ACTION_H_
