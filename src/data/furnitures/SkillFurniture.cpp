#include "data/furnitures/SkillFurniture.h"

bool SkillFurniture::getUnwalkable() { return unwalkable; }
bool SkillFurniture::getOpaque() { return opaque; }
bool SkillFurniture::getSolid() { return solid; }
int SkillFurniture::getLight() { return light; }

void SkillFurniture::activate(Character * user, bool remote) {
    if(remote || tryActivate(user)) {
        //
    }
}