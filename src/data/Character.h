#include <list>
#include <set>

#include "src/loader/CharacterLoader.h"
#include "src/data/Class.h"
#include "src/data/Gear.h"
#include "src/data/Effect.h"
#include "src/data/Skill.h"


// orientations

#define NORTH 1
#define NORTH_EAST 2
#define EAST 3
#define SOUTH_EAST 4
#define SOUTH 5
#define SOUTH_WEST 6
#define WEST 7
#define NORTH_WEST 8

// how

#define POSITIVE 1
#define NEGATIVE 2

class Character {
  public:
    const long id;
    const string name;
    const Class class;
    const string race;
    const string sex;
    Character(string name, string race, string class, string sex);
    boolean isAlive();
    boolean isSoulBurning();
    int getX();
    int getY();
    int getOrientation();
    int getHp();
    int getMaxHp();
    int getMana();
    int getMaxMana();
    int getDefense();
    int getSoulBurnTreshold();
    int getCurrentSoulBurn();
    int getFlow();
    int getVision();

    void move(int orientation);
    void move(int x, int y, int orientation);
    void changeHp(int hp, int how);
    void incrMaxHp();
    void changeMana(int mana, int how);
    void incrMaxMana();
    void incrSoulBurnTreshold();
    void incrFlow();

    void replaceGear(Item current, Item new);
  private:
    static long id_cpt = 0;
    int x;
    int y;
    int orientation;
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    int defense;
    int soulBurnTreshold;
    int currentSoulBurn;
    int flow;
    int vision;

    string team;

    Gear gear;
    std::list<Item> stuff;
    std::list<Effect> effects;
    std::set<Skills> skills;
};
