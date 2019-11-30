#include <list>
#include <set>

#include "src/loader/CharacterLoader.h"
#include "src/data/Class.h"
#include "src/data/Gear.h"
#include "src/data/Effect.h"
#include "src/data/Skill.h"

class Character {
  public:
    const string name;
    const long id;
    const int race;
    const int sex;
    Character(String name, Class class);
    boolean isAlive();
    boolean isSoulBurning();
    int getX();
    int getY();
    int getHp();
    int getMaxHp();
    int getMana();
    int getMaxMana();
    int getSoulBurnTreshold();
    int getCurrentSoulBurn();
    int getFlow();

    void setX(int x);
    void setY(int y);
    void setHp(int hp);
    void setMaxHp(int maxHp);
    void setMana(int mana);
    void setMaxMana(int maxMana);
    void setSoulBurnTreshold(int soulBurnTreshold);

    void replaceGear(Item current, Item new);
  private:
    int x;
    int y;
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    int soulBurnTreshold;
    int currentSoulBurn;
    int flow;

    Gear gear;
    std::list<Item> stuff;
    std::list<Effect> effects;
    std::set<Skills> skills;
};
