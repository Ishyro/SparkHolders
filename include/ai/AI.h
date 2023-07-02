#ifndef _AI_H_
#define _AI_H_

#include <map>

#include "util/MapUtil.h"

#include "Values.h"

class AI {
  public:
    AI(){}
    AI(int x, int y):origin_x(x),origin_y(y){}
    virtual Action * getActions(Adventure * adventure, Character * c) = 0;
    static float getFollowOrientation(Adventure * adventure, Character * self, int x, int y);
    static float getFleeOrientation(Adventure * adventure, Character * self, int x, int y);
    static Map * updateMap(Adventure * adventure, Character * c);
    std::vector<MapUtil::Pair> getFollowPath(Adventure * adventure, Character * self, int x, int y);
  protected:
    int origin_x;
    int origin_y;
    bool hungry;
    bool sleepy;
    void selectHungriness(Character * self);
    void selectTiredness(Character * self);
    Action * moveTowards(Adventure * adventure, Character * self, int target_x, int target_y);
    Action * eat(Adventure * adventure, Character * self);
    Action * trackPrey(Adventure * adventure, Character * self);
    std::list<Character *> getThreats(Adventure * adventure, Map * map, Character * self, int range);
    std::map<Character *, Skill *> getBestDamageSkills(std::list<Character *> threats, std::map<Skill *, std::array<int, DAMAGE_TYPE_NUMBER>> skills, Character * self);
    Action * attack(Adventure * adventure, std::list<Character *> threats, Character * self);
};

#endif // _AI_H_
