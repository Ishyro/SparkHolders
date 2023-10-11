#include "data/furnitures/SkillFurniture.h"

void SkillFurniture::activate(Character * user, bool remote) {
    if(remote || tryActivate(user)) {
        //
    }
}