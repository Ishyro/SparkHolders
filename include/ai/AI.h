#ifndef _AI_H_
#define _AI_H_

#include <map>

#include "util/MathUtil.h"

#include "Values.h"

class AI {
  public:
    AI(){}
    AI(int32_t x, int32_t y):origin_x(x),origin_y(y){}
    virtual Action * getActions(Adventure * adventure, Character * c) = 0;
    static float getFollowOrientation(Adventure * adventure, Character * self, int32_t x, int32_t y);
    static float getFleeOrientation(Adventure * adventure, Character * self, int32_t x, int32_t y);
    std::vector<MathUtil::Pair> getFollowPath(Adventure * adventure, Character * self, int32_t x, int32_t y);
  protected:
    int32_t origin_x;
    int32_t origin_y;
    bool hungry;
    bool sleepy;
    void selectHungriness(Character * self);
    void selectTiredness(Character * self);
    Action * moveTowards(Adventure * adventure, Character * self, int32_t target_x, int32_t target_y);
    Action * eat(Adventure * adventure, Character * self);
    Action * trackPrey(Adventure * adventure, Character * self);
    std::list<Character *> getThreats(Adventure * adventure, Character * self, int32_t range);
    std::map<Character *, Skill *> getBestDamageSkills(std::list<Character *> threats, std::map<Skill *, std::array<float, DAMAGE_TYPE_NUMBER>> skills, Character * self);
    Action * attack(Adventure * adventure, std::list<Character *> threats, Character * self);
};

#endif // _AI_H_
