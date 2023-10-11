#include "data/furnitures/SwitchFurniture.h"

bool SwitchFurniture::getUntraversable() { 
    if(isOn) {
        return untraversable;
    }
    else {
        return untraversable_off;
    }
}
bool SwitchFurniture::getOpaque() { 
    if(isOn) {
        return opaque;
    }
    else {
        return opaque_off;
    }
}
bool SwitchFurniture::getSolid() { 
    if(isOn) {
        return solid;
    }
    else {
        return solid_off;
    }
}
int SwitchFurniture::getLight() { 
    if(isOn) {
        return light;
    }
    else {
        return light_off;
    }
}

bool SwitchFurniture::getIsOn() { return isOn; }

void SwitchFurniture::activate(Character * user, bool remote) {
    if(remote || tryActivate(user)) {
        isOn = !isOn;
    }
}
