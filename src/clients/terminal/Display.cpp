#include <string>
#include <sstream>

#include "communication/Client.h"
#include "communication/Socket.h"

#include "data/Character.h"
#include "data/Effect.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Block.h"
#include "data/BlocksChunk.h"
#include "data/Region.h"

#include "data/items/Item.h"
#include "data/items/ArmorItem.h"
#include "data/items/WeaponItem.h"
#include "data/items/SerialItem.h"
#include "data/items/AmmunitionItem.h"
#include "data/items/Gear.h"

#include "data/skills/Skill.h"
#include "data/skills/PseudoSkill.h"

#include "data/skills/ChanneledSkill.h"
#include "data/skills/MapLinkerSkill.h"
#include "data/skills/MindControlSkill.h"
#include "data/skills/ProjectileSkill.h"
#include "data/skills/ResurrectionSkill.h"
#include "data/skills/SimpleSkill.h"
#include "data/skills/SummonSkill.h"
#include "data/skills/TeamChangerSkill.h"
#include "data/skills/TeleportSkill.h"
#include "data/skills/BlockSwapSkill.h"

#include "data/ways/Way.h"
#include "data/ways/Attributes.h"

#include "clients/Translator.h"

#include "clients/terminal/Display.h"

#include "util/MapUtil.h"
#include "util/String.h"

namespace Display {

  void print(WINDOW* screen, int offsetY, int offsetX, std::string to_print) {
    long unsigned int lines = 0;
    long unsigned int cols = 0;
    getmaxyx(screen, lines, cols);
    std::istringstream stream(to_print);
    std::string line = "";
    std::string word;
    long unsigned int currentLength = 0;
    while(getline(stream, word, ' ') && word != "") {
      if(line == "") {
        line = word;
        currentLength = word.length();
        continue;
      }
      const long unsigned int begin = word.find('\n');
      if(begin != std::string::npos) {
        if(currentLength == cols) {
          line += word;
        }
        else if(currentLength + 1 + begin <= cols) {
          line += " " + word;
        }
        else {
          line += "\n" + word;
        }
        currentLength = word.length() - begin - 1;
      }
      else if(currentLength == cols) {
        line += word;
        currentLength = word.length();
      }
      else if(currentLength + 1 + word.length() <= cols) {
        line += " " + word;
        currentLength += 1 + word.length();
      }
      else {
        line += "\n" + word;
        currentLength = word.length();
      }
    }
    mvwprintw(screen, offsetY, offsetX, line.c_str());
  }

  void displayMap(StateDisplay * display, Adventure * adventure, Character * player, WINDOW * screen, Translator * t) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    box(screen, ACS_VLINE, ACS_HLINE);
    Region * region = player->getRegion();
    //mvwprintw(screen, 1, cols / 2 - t->getMapName(map->name).length() / 2, t->getMapName(map->name).c_str());
    for(int y = region->id.y + CHUNK_SIZE * 3 - 1; y >= region->id.y; y--) {
      for(int x = region->id.x; x < region->id.x + CHUNK_SIZE * 3; x++) {
        std::string to_print = "·";
        Block * block = region->getBlock(MapUtil::makeVector3i(x, y, -1));
        if(block != nullptr) {
          if(block->unwalkable) {
            to_print = "#";
          }
          if(block->name == "TXT_MIST") { // unseen
            to_print = "~";
          }
          mvwprintw(screen, lines / 2 - CHUNK_SIZE * 3 / 2 + CHUNK_SIZE * 3 - 1 - (y - region->id.y), (x - region->id.x) + cols / 2 - CHUNK_SIZE * 3 / 2, to_print.c_str());
        }
      }
    }
    for(CharacterDisplay * character : display->characters) {
      std::string to_print;
      char ch = t->getCharacterName(character->name).at(0);
      to_print = ch;
      int color;
      switch(character->teamRelation) {
        case TEAM_SAME:
          color = BLUE;
          break;
        case TEAM_ALLY:
          color = GREEN;
          break;
        case TEAM_NEUTRAL:
          color = YELLOW;
          break;
        case TEAM_ENEMY:
          color = RED;
          break;
        default:
          color = WHITE;
      }
      wattron(screen, COLOR_PAIR(color));
      mvwprintw(screen, lines / 2 - CHUNK_SIZE * 3 / 2 + CHUNK_SIZE * 3 - 1 - (int) std::floor(character->y - region->id.y), (int) std::floor(character->x - region->id.x) + cols / 2 - CHUNK_SIZE * 3 / 2, to_print.c_str());
      wattroff(screen, COLOR_PAIR(color));
    }
    for(ProjectileDisplay * projectile : display->projectiles) {
      std::string to_print = "";
      switch((int) std::floor(projectile->orientation)) {
        case 90:
          to_print = "↑";
          break;
        case 45:
          to_print = "↗";
          break;
        case 0:
          to_print = "→";
          break;
        case 315:
          to_print = "↘";
          break;
        case 270:
          to_print = "↓";
          break;
        case 225:
          to_print = "↙";
          break;
        case 180:
          to_print = "←";
          break;
        case 135:
          to_print = "↖";
          break;
        default:
          to_print = "•";
          break;
      }
      wattron(screen, COLOR_PAIR(RED));
      mvwprintw(screen, lines / 2 - CHUNK_SIZE * 3 / 2 + CHUNK_SIZE * 3 - 1 - (int) std::floor(projectile->y - region->id.y), (int) std::floor(projectile->x - region->id.x) + cols / 2 - CHUNK_SIZE * 3 / 2, to_print.c_str());
      wattroff(screen, COLOR_PAIR(RED));
    }
    for(Loot * loot : display->loots) {
      std::string to_print = "*";
      wattron(screen, COLOR_PAIR(YELLOW));
      mvwprintw(screen, lines / 2 - CHUNK_SIZE * 3 / 2 + CHUNK_SIZE * 3 - 1 - (int) std::floor(loot->y - region->id.y), (int) std::floor(loot->x - region->id.x) + cols / 2 - CHUNK_SIZE * 3 / 2, to_print.c_str());
      wattroff(screen, COLOR_PAIR(YELLOW));
    }
    std::string to_print = std::to_string(player->getOrientation());
    // player position
    mvwprintw(screen, lines - 4, 1, to_print.c_str());
    to_print = std::string("X: ") + std::to_string(player->getCoord().x);
    mvwprintw(screen, lines - 3, 1, to_print.c_str());
    to_print = std::string("Y: ") + std::to_string(player->getCoord().y);
    mvwprintw(screen, lines - 2, 1, to_print.c_str());
    // time
    std::stringstream * ss = new std::stringstream(adventure->getTime());
    std::string skip = String::extract(ss); // skip years
    skip = String::extract(ss); // skip months
    skip = String::extract(ss); // skip weeks
    skip = String::extract(ss); // skip days
    to_print = String::extract(ss) + ":"; // hours
    to_print += String::extract(ss) + ":"; // minutes
    to_print += String::extract(ss); // seconds
    delete ss;
    mvwprintw(screen, lines - 2, cols - 2 - to_print.length(), to_print.c_str());

    wrefresh(screen);
  }

  void displayStats(Character * player, WINDOW * screen, Translator * t) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    box(screen, ACS_VLINE, ACS_HLINE);
    std::string to_print = player->name + ", " + t->getAttributesName(player->getMainClass()->name);
    mvwprintw(screen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
    mvwprintw(screen, 3, 1, (t->getStandardName("Hp") + std::string(": ") + std::to_string(player->getHp()) + std::string(" / ") + std::to_string(player->getMaxHp())).c_str());
    mvwprintw(screen, 4, 1, (t->getStandardName("Mana") + std::string(": ") + std::to_string(player->getMana()) + std::string(" / ") + std::to_string(player->getMaxMana())).c_str());
    mvwprintw(screen, 5, 1, (t->getStandardName("Stamina") + std::string(": ") + std::to_string(player->getStamina()) + std::string(" / ") + std::string("100")).c_str());
    mvwprintw(screen, 6, 1, (t->getStandardName("Satiety") + std::string(": ") + std::to_string(player->getSatiety()) + std::string(" / ") + std::string("100")).c_str());
    mvwprintw(screen, 7, 1, (t->getStandardName("Soulburn") + std::string(": ") + std::to_string(player->getCurrentSoulBurn()) + std::string(" / ") + std::to_string(player->getSoulBurnThreshold())).c_str());
    mvwprintw(screen, 8, 1, (t->getStandardName("Flow") + std::string(": ") + std::to_string(player->getFlow())).c_str());
    mvwprintw(screen, 9, 1, (t->getStandardName("Shield") + std::string(": ") + std::to_string(player->getShield())).c_str());
    mvwprintw(screen, 10, 1, (t->getStandardName("Vision Range") + std::string(": ") + std::to_string(player->getVisionRange())).c_str());
    mvwprintw(screen, 11, 1, (t->getStandardName("Vision Power") + std::string(": ") + std::to_string(player->getVisionPower())).c_str());
    mvwprintw(screen, 12, 1, (t->getStandardName("Detection Range") + std::string(": ") + std::to_string(player->getDetectionRange())).c_str());
    mvwprintw(screen, 13, 1, (t->getStandardName("Level") + std::string(": ") + std::to_string(player->getLevel())).c_str());
    mvwprintw(screen, 14, 1, (t->getStandardName("Experience") + std::string(": ") + std::to_string(player->getXP()) + std::string(" / ") + std::to_string(1000 * player->getLevel() * player->getLevel())).c_str());
    wrefresh(screen);
  }

  WINDOW * displaySkill(Skill * skill, WINDOW * screen, int overcharge_power, int overcharge_duration, int overcharge_range, Translator * t) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    wattron(screen, COLOR_PAIR(WHITE));
    box(screen, ACS_VLINE, ACS_HLINE);
    std::string to_print = t->getSkillName(skill->name);
    mvwprintw(screen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
    mvwprintw(screen, 3, 1, (t->getStandardName("Targeting") + std::string(": ") + t->getStandardName(std::string("target_type_") + std::to_string(skill->target_type))).c_str());
    mvwprintw(screen, 4, 1, (t->getStandardName("Range") + std::string(": ") + std::to_string(skill->range * overcharge_range)).c_str());
    mvwprintw(screen, 5, 1, (t->getStandardName("Power") + std::string(": ") + std::to_string(skill->getPower() * overcharge_power)).c_str());
    mvwprintw(screen, 6, 1, (t->getStandardName("Mana cost") + std::string(": ") + std::to_string(skill->getManaCost(overcharge_power, overcharge_duration, overcharge_range))).c_str());
    mvwprintw(screen, 7, 1, (t->getStandardName("Time") + std::string(": ") + std::to_string(skill->time)).c_str());
    int i = 8;
    for(PseudoSkill * pseudoSkill : skill->skills) {
      switch(pseudoSkill->skill_type) {
        case SKILL_PROJECTILE: {
          Projectile * projectile = ((ProjectileSkill *) pseudoSkill)->getProjectile();
          mvwprintw(screen, i++, 1, (t->getStandardName("Speed") + std::string(": ") + std::to_string(projectile->getSpeed() * overcharge_duration)).c_str());
          mvwprintw(screen, i++, 1, (t->getStandardName("Falloff timer") + std::string(": ") + std::to_string(projectile->getFalloffTimer() * overcharge_range)).c_str());
          mvwprintw(screen, i++, 1, (t->getStandardName("Area") + std::string(": ") + std::to_string(projectile->getArea() * overcharge_range)).c_str());
          mvwprintw(screen, i++, 1, (t->getStandardName("Waste per tick") + std::string(": ") + std::to_string(projectile->getWastePerTick() / overcharge_duration)).c_str());
          mvwprintw(screen, i++, 1, (t->getStandardName("Waste per area") + std::string(": ") + std::to_string(projectile->getWastePerArea() / overcharge_range)).c_str());
          mvwprintw(screen, i++, 1, (t->getStandardName("Waste per hit") + std::string(": ") + std::to_string(projectile->getWastePerHit())).c_str());
        }
        default:
          ;
      }
    }
    int damage_SLASH = skill->getDamageFromType(DAMAGE_SLASH, overcharge_power);
    if(damage_SLASH != 0) {
      mvwprintw(screen, i++, 1, (t->getStandardName("SLASH") + std::string(": ") + std::to_string(damage_SLASH)).c_str());
    }
    int damage_PUNCTURE = skill->getDamageFromType(DAMAGE_PUNCTURE, overcharge_power);
    if(damage_PUNCTURE != 0) {
      mvwprintw(screen, i++, 1, (t->getStandardName("PUNCTURE") + std::string(": ") + std::to_string(damage_PUNCTURE)).c_str());
    }
    int damage_BLUNT = skill->getDamageFromType(DAMAGE_BLUNT, overcharge_power);
    if(damage_BLUNT != 0) {
      mvwprintw(screen, i++, 1, (t->getStandardName("BLUNT") + std::string(": ") + std::to_string(damage_BLUNT)).c_str());
    }
    int damage_FIRE = skill->getDamageFromType(DAMAGE_FIRE, overcharge_power);
    if(damage_FIRE != 0) {
      mvwprintw(screen, i++, 1, (t->getStandardName("FIRE") + std::string(": ") + std::to_string(damage_FIRE)).c_str());
    }
    int damage_LIGHTNING = skill->getDamageFromType(DAMAGE_LIGHTNING, overcharge_power);
    if(damage_LIGHTNING != 0) {
      mvwprintw(screen, i++, 1, (t->getStandardName("DAMAGE_LIGHTNING") + std::string(": ") + std::to_string(damage_LIGHTNING)).c_str());
    }
    int damage_COLD = skill->getDamageFromType(DAMAGE_FROST, overcharge_power);
    if(damage_COLD != 0) {
      mvwprintw(screen, i++, 1, (t->getStandardName("FROST") + std::string(": ") + std::to_string(damage_COLD)).c_str());
    }
    int damage_POISON = skill->getDamageFromType(DAMAGE_POISON, overcharge_power);
    if(damage_POISON != 0) {
      mvwprintw(screen, i++, 1, (t->getStandardName("POISON") + std::string(": ") + std::to_string(damage_POISON)).c_str());
    }
    int damage_NEUTRAL = skill->getDamageFromType(DAMAGE_ACID, overcharge_power);
    if(damage_NEUTRAL != 0) {
      mvwprintw(screen, i++, 1, (t->getStandardName("NEUTRAL") + std::string(": ") + std::to_string(damage_NEUTRAL)).c_str());
    }
    int damage_TRUE = skill->getDamageFromType(DAMAGE_TRUE, overcharge_power);
    if(damage_TRUE != 0) {
      mvwprintw(screen, i++, 1, (t->getStandardName("TRUE") + std::string(": ") + std::to_string(damage_TRUE)).c_str());
    }
    int damage_SOUL = skill->getDamageFromType(DAMAGE_SOUL, overcharge_power);
    if(damage_SOUL != 0) {
      mvwprintw(screen, i++, 1, (t->getStandardName("SOUL") + std::string(": ") + std::to_string(damage_SOUL)).c_str());
    }
    wattroff(screen, COLOR_PAIR(WHITE));
    // should be constants
    int separator = (float) LINES * 3 / 5;
    float ratio = 2.25;
    WINDOW * descScreen = subwin(screen, lines - i - 2, cols - 2, separator + i + 1, std::ceil((float) COLS - ratio * (float) (LINES - separator)) + 1);
    wattron(descScreen, COLOR_PAIR(WHITE));
    print(descScreen, 0, 0, (t->getSkillDesc(skill->name)));
    wattroff(descScreen, COLOR_PAIR(WHITE));
    wrefresh(descScreen);
    wrefresh(screen);
    return descScreen;
  }

  void displayTarget(CharacterDisplay * target, WINDOW * screen, Translator * t) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    box(screen, ACS_VLINE, ACS_HLINE);
    std::string to_print = target->name + ", " + target->team;
    mvwprintw(screen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
    mvwprintw(screen, 3, 1, (t->getStandardName("Hp") + std::string(": ") + std::to_string(target->hp) + std::string(" / ") + std::to_string(target->maxHp)).c_str());
    mvwprintw(screen, 4, 1, (t->getStandardName("Mana") + std::string(": ") + std::to_string(target->mana) + std::string(" / ") + std::to_string(target->maxMana)).c_str());
    mvwprintw(screen, 5, 1, (t->getStandardName("Soulburn") + std::string(": ") + std::to_string(target->soulBurn) + std::string(" / ") + std::to_string(target->soulBurnTreshold)).c_str());
    mvwprintw(screen, 6, 1, (t->getStandardName("Flow") + std::string(": ") + std::to_string(target->flow)).c_str());
    mvwprintw(screen, 7, 1, (t->getStandardName("Shield") + std::string(": ") + std::to_string(target->shield)).c_str());

    mvwprintw(screen, 7, 1, t->getStandardName("Damages and Reductions").c_str());
    mvwprintw(screen, 8, 1, (t->getStandardName("Shield") + std::string(": ") + std::to_string(target->shield)).c_str());
    mvwprintw(screen, 9, 1, (t->getStandardName("SLASH") + std::string(": ") + std::to_string(target->damages[DAMAGE_SLASH]) + std::string(" / ") + std::to_string(target->damage_reductions[DAMAGE_SLASH])).c_str());
    mvwprintw(screen, 10, 1, (t->getStandardName("PUNCTURE") + std::string(": ") + std::to_string(target->damages[DAMAGE_PUNCTURE]) + std::string(" / ") + std::to_string(target->damage_reductions[DAMAGE_PUNCTURE])).c_str());
    mvwprintw(screen, 11, 1, (t->getStandardName("BLUNT") + std::string(": ") + std::to_string(target->damages[DAMAGE_BLUNT]) + std::string(" / ") + std::to_string(target->damage_reductions[DAMAGE_BLUNT])).c_str());
    mvwprintw(screen, 12, 1, (t->getStandardName("FIRE") + std::string(": ") + std::to_string(target->damages[DAMAGE_FIRE]) + std::string(" / ") + std::to_string(target->damage_reductions[DAMAGE_FIRE])).c_str());
    mvwprintw(screen, 13, 1, (t->getStandardName("LIGHTNING") + std::string(": ") + std::to_string(target->damages[DAMAGE_LIGHTNING]) + std::string(" / ") + std::to_string(target->damage_reductions[DAMAGE_LIGHTNING])).c_str());
    mvwprintw(screen, 14, 1, (t->getStandardName("COLD") + std::string(": ") + std::to_string(target->damages[DAMAGE_FROST]) + std::string(" / ") + std::to_string(target->damage_reductions[DAMAGE_FROST])).c_str());
    mvwprintw(screen, 15, 1, (t->getStandardName("POISON") + std::string(": ") + std::to_string(target->damages[DAMAGE_POISON]) + std::string(" / ") + std::to_string(target->damage_reductions[DAMAGE_POISON])).c_str());
    mvwprintw(screen, 16, 1, (t->getStandardName("NEUTRAL") + std::string(": ") + std::to_string(target->damages[DAMAGE_ACID])).c_str());
    mvwprintw(screen, 17, 1, (t->getStandardName("TRUE") + std::string(": ") + std::to_string(target->damages[DAMAGE_TRUE])).c_str());
    mvwprintw(screen, 18, 1, (t->getStandardName("SOUL") + std::string(": ") + std::to_string(target->damages[DAMAGE_SOUL])).c_str());
    wrefresh(screen);
  }

  WINDOW * displayItem(Item * item, WINDOW * screen, Translator * t) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    wattron(screen, COLOR_PAIR(WHITE));
    std::string to_print = t->getItemName(item->name);
    mvwprintw(screen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
    int i = 3;
    mvwprintw(screen, i++, 1, (t->getStandardName("Type") + std::string(": ") + t->getStandardName(std::string("item_type_") + std::to_string(item->type))).c_str());
    mvwprintw(screen, i++, 1, (t->getStandardName("Value") + std::string(": ") + std::to_string(item->gold_value)).c_str());
    if(item->consumable) {
      mvwprintw(screen, i++, 1, (t->getStandardName("Effects")).c_str());
      for(Effect * effect : item->effects) {
        mvwprintw(screen, i++, 5, (t->getEffectName(effect->name)).c_str());
      }
    }
    if(item->type == ITEM_AMMUNITION || item->type == ITEM_SERIAL) {
      SerialItem * serial = (SerialItem *) item;
      mvwprintw(screen, i++, 1, (t->getStandardName("Number") + std::string(": ") + std::to_string(serial->getNumber())).c_str());
    }
    if(item->type == ITEM_ARMOR) {
      ArmorItem * armor = (ArmorItem *) item;
      if(armor->getDamageReductionFromType(DAMAGE_SLASH) != 0.F)
        mvwprintw(screen, i++, 1, (t->getStandardName("SLASH") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_SLASH))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_PUNCTURE) != 0.F)
        mvwprintw(screen, i++, 1, (t->getStandardName("PUNCTURE") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_PUNCTURE))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_BLUNT) != 0.F)
        mvwprintw(screen, i++, 1, (t->getStandardName("BLUNT") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_BLUNT))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_FIRE) != 0.F)
        mvwprintw(screen, i++, 1, (t->getStandardName("FIRE") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_FIRE))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_LIGHTNING) != 0.F)
        mvwprintw(screen, i++, 1, (t->getStandardName("LIGHTNING") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_LIGHTNING))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_FROST) != 0.F)
        mvwprintw(screen, i++, 1, (t->getStandardName("FROST") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_FROST))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_POISON) != 0.F)
        mvwprintw(screen, i++, 1, (t->getStandardName("POISON") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_POISON))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_MIND) != 0.F)
        mvwprintw(screen, i++, 1, (t->getStandardName("MIND") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_MIND))).c_str());
    }
    if(item->type == ITEM_WEAPON) {
      WeaponItem * weapon = (WeaponItem *) item;
      mvwprintw(screen, i++, 1, (t->getStandardName("Range") + std::string(": ") + std::to_string(weapon->range)).c_str());
      if(weapon->getDamageFromType(DAMAGE_SLASH) > 0)
        mvwprintw(screen, i++, 1, (t->getStandardName("SLASH") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_SLASH))).c_str());
      if(weapon->getDamageFromType(DAMAGE_PUNCTURE) > 0)
        mvwprintw(screen, i++, 1, (t->getStandardName("PUNCTURE") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_PUNCTURE))).c_str());
      if(weapon->getDamageFromType(DAMAGE_BLUNT) > 0)
        mvwprintw(screen, i++, 1, (t->getStandardName("BLUNT") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_BLUNT))).c_str());
      if(weapon->getDamageFromType(DAMAGE_FIRE) > 0)
        mvwprintw(screen, i++, 1, (t->getStandardName("FIRE") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_FIRE))).c_str());
      if(weapon->getDamageFromType(DAMAGE_LIGHTNING) > 0)
        mvwprintw(screen, i++, 1, (t->getStandardName("LIGHTNING") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_LIGHTNING))).c_str());
      if(weapon->getDamageFromType(DAMAGE_FROST) > 0)
        mvwprintw(screen, i++, 1, (t->getStandardName("FROST") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_FROST))).c_str());
      if(weapon->getDamageFromType(DAMAGE_POISON) > 0)
        mvwprintw(screen, i++, 1, (t->getStandardName("POISON") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_POISON))).c_str());
      if(weapon->getDamageFromType(DAMAGE_ACID) > 0)
        mvwprintw(screen, i++, 1, (t->getStandardName("ACID") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_ACID))).c_str());
      if(weapon->getDamageFromType(DAMAGE_MIND) > 0)
        mvwprintw(screen, i++, 1, (t->getStandardName("MIND") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_MIND))).c_str());
      if(weapon->getDamageFromType(DAMAGE_TRUE) > 0)
        mvwprintw(screen, i++, 1, (t->getStandardName("TRUE") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_TRUE))).c_str());
      if(weapon->getDamageFromType(DAMAGE_SOUL) > 0)
        mvwprintw(screen, i++, 1, (t->getStandardName("SOUL") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_SOUL))).c_str());
    }
    wattroff(screen, COLOR_PAIR(WHITE));
    // should be constants
    int separator = (float) LINES * 3 / 5;
    float ratio = 2.25;
    WINDOW * descScreen = subwin(screen, lines - 3 - 1, cols - 20 - 1, separator + 3, std::ceil((float) COLS - ratio * (float) (LINES - separator)) + 20);
    wattron(descScreen, COLOR_PAIR(WHITE));
    print(descScreen, 0, 0, (t->getItemDesc(item->name)));
    wattroff(descScreen, COLOR_PAIR(WHITE));
    wrefresh(descScreen);
    wrefresh(screen);
    return descScreen;
  }

  void displayCommands(WINDOW * screen, Translator * t) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    box(screen, ACS_VLINE, ACS_HLINE);
    std::string to_print = t->getStandardName("CONTROLS");
    mvwprintw(screen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
    mvwprintw(screen, 3, 1, (t->getStandardName("NUMPAD") + std::string(": ") + t->getStandardName("MOVE") + std::string(" / ") +  t->getStandardName("ATTACK")).c_str());
    mvwprintw(screen, 4, 1, (t->getStandardName("5") + std::string(": ") + t->getStandardName("REST")).c_str());
    mvwprintw(screen, 5, 1, (t->getStandardName("<") + std::string(": ") + t->getStandardName("OPEN")).c_str());
    mvwprintw(screen, 6, 1, (t->getStandardName("I") + std::string(": ") + t->getStandardName("INVENTORY")).c_str());
    mvwprintw(screen, 7, 1, (t->getStandardName("R") + std::string(": ") + t->getStandardName("RELOAD")).c_str());
    mvwprintw(screen, 8, 1, (t->getStandardName("X") + std::string(": ") + t->getStandardName("USE SKILL")).c_str());
    mvwprintw(screen, 9, 1, (t->getStandardName("C") + std::string(": ") + t->getStandardName("SHOOT")).c_str());
    mvwprintw(screen, 10, 1, (t->getStandardName("SPACEBAR") + std::string(": ") + t->getStandardName("LOOT")).c_str());
    wrefresh(screen);
  }

  WINDOW * displayAttributes(Attributes * attributes, int place, int color, WINDOW * screen, int offsetY, int offsetX, Translator * t) {
    WINDOW * attributesScreen = subwin(screen, ATTRIBUTES_HEIGHT, ATTRIBUTES_WIDTH, offsetY + 2 + ATTRIBUTES_HEIGHT * place, offsetX + 1);
    wattron(attributesScreen, COLOR_PAIR(color));
    box(attributesScreen, ACS_VLINE, ACS_HLINE);
    mvwprintw(attributesScreen, 1, ATTRIBUTES_WIDTH / 2 - t->getAttributesName(attributes->name).length() / 2, t->getAttributesName(attributes->name).c_str());
    mvwprintw(attributesScreen, 3, 1, (t->getStandardName("Hp") + std::string(": ") + std::to_string(attributes->baseHp)).c_str());
    mvwprintw(attributesScreen, 4, 1, (t->getStandardName("Mana") + std::string(": ") + std::to_string(attributes->baseMana)).c_str());
    mvwprintw(attributesScreen, 5, 1, (t->getStandardName("Shield") + std::string(": ") + std::to_string(attributes->baseShield)).c_str());
    mvwprintw(attributesScreen, 6, 1, (t->getStandardName("Damage") + std::string(": ") + std::to_string(attributes->baseDamageMult) + std::string("%")).c_str());
    mvwprintw(attributesScreen, 7, 1, (t->getStandardName("Soulburn") + std::string(": ") + std::to_string(attributes->baseSoulBurn)).c_str());
    mvwprintw(attributesScreen, 8, 1, (t->getStandardName("Flow") + std::string(": ") + std::to_string(attributes->baseFlow)).c_str());
    mvwprintw(attributesScreen, 9, 1, (t->getStandardName("Vision Range") + std::string(": ") + std::to_string(attributes->baseVisionRange)).c_str());
    mvwprintw(attributesScreen, 10, 1, (t->getStandardName("Vision Power") + std::string(": ") + std::to_string(attributes->baseVisionPower)).c_str());
    mvwprintw(attributesScreen, 11, 1, (t->getStandardName("Detection Range") + std::string(": ") + std::to_string(attributes->baseDetectionRange)).c_str());
    wattroff(attributesScreen, COLOR_PAIR(color));
    return attributesScreen;
  }

  WINDOW * displayWay(Way * way, int place, int color, WINDOW * screen, int offsetY, int offsetX, Translator * t) {
    WINDOW * wayScreen = subwin(screen, WAY_HEIGHT, WAY_WIDTH, offsetY + 2 + WAY_HEIGHT * place, offsetX + 1);
    wattron(wayScreen, COLOR_PAIR(color));
    box(wayScreen, ACS_VLINE, ACS_HLINE);
    mvwprintw(wayScreen, 1, WAY_WIDTH / 2 - t->getWayName(way->name).length() / 2, t->getWayName(way->name).c_str());
    mvwprintw(wayScreen, 3, 1, (std::string("+") + t->getStandardName("Hp") + std::string(": ") + std::to_string(way->hpIncr)).c_str());
    mvwprintw(wayScreen, 4, 1, (std::string("+") + t->getStandardName("Mana") + std::string(": ") + std::to_string(way->manaIncr)).c_str());
    mvwprintw(wayScreen, 5, 1, (std::string("+") + t->getStandardName("Shield") + std::string(": ") + std::to_string(way->shieldIncr)).c_str());
    mvwprintw(wayScreen, 6, 1, (std::string("+") + t->getStandardName("Damage") + std::string(": ") + std::to_string(way->damageMultIncr) + std::string("%")).c_str());
    mvwprintw(wayScreen, 7, 1, (std::string("+") + t->getStandardName("Soulburn") + std::string(": ") + std::to_string(way->soulBurnIncr)).c_str());
    mvwprintw(wayScreen, 8, 1, (std::string("+") + t->getStandardName("Flow") + std::string(": ") + std::to_string(way->flowIncr)).c_str());
    wattroff(wayScreen, COLOR_PAIR(color));
    return wayScreen;
  }

  WINDOW * displayCharacter(Attributes * attributes, Way * race, Way * origin, Way * culture, Way * religion, Way * profession, int color, WINDOW * screen, int sizeX, int offsetY, int offsetX, Translator * t) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    WINDOW * characterScreen = subwin(screen, ATTRIBUTES_HEIGHT - 2, sizeX, offsetY, offsetX);
    int hpIncr = 0;
    int manaIncr = 0;
    int shieldIncr = 0;
    int damageMultIncr = 0;
    int soulBurnIncr = 0;
    int flowIncr = 0;
    if(race != nullptr) {
      hpIncr += race->hpIncr;
      manaIncr += race->manaIncr;
      shieldIncr += race->shieldIncr;
      damageMultIncr += race->damageMultIncr;
      soulBurnIncr += race->soulBurnIncr;
      flowIncr += race->flowIncr;
    }
    if(origin != nullptr) {
      hpIncr += origin->hpIncr;
      manaIncr += origin->manaIncr;
      shieldIncr += origin->shieldIncr;
      damageMultIncr += origin->damageMultIncr;
      soulBurnIncr += origin->soulBurnIncr;
      flowIncr += origin->flowIncr;
    }
    if(culture != nullptr) {
      hpIncr += culture->hpIncr;
      manaIncr += culture->manaIncr;
      shieldIncr += culture->shieldIncr;
      damageMultIncr += culture->damageMultIncr;
      soulBurnIncr += culture->soulBurnIncr;
      flowIncr += culture->flowIncr;
    }
    if(religion != nullptr) {
      hpIncr += religion->hpIncr;
      manaIncr += religion->manaIncr;
      shieldIncr += religion->shieldIncr;
      damageMultIncr += religion->damageMultIncr;
      soulBurnIncr += religion->soulBurnIncr;
      flowIncr += religion->flowIncr;
    }
    if(profession != nullptr) {
      hpIncr += profession->hpIncr;
      manaIncr += profession->manaIncr;
      shieldIncr += profession->shieldIncr;
      damageMultIncr += profession->damageMultIncr;
      soulBurnIncr += profession->soulBurnIncr;
      flowIncr += profession->flowIncr;
    }
    wattron(characterScreen, COLOR_PAIR(color));
    box(characterScreen, ACS_VLINE, ACS_HLINE);
    int space = cols / 2;
    if(attributes != nullptr) {
      mvwprintw(characterScreen, 1, 1, (t->getStandardName("Hp") + std::string(": ") + std::to_string(attributes->baseHp)).c_str());
      mvwprintw(characterScreen, 2, 1, (t->getStandardName("Mana") + std::string(": ") + std::to_string(attributes->baseMana)).c_str());
      mvwprintw(characterScreen, 3, 1, (t->getStandardName("Shield") + std::string(": ") + std::to_string(attributes->shieldIncr)).c_str());
      mvwprintw(characterScreen, 4, 1, (t->getStandardName("Damage") + std::string(": ") + std::to_string(attributes->baseDamageMult) + std::string("%")).c_str());
      mvwprintw(characterScreen, 5, 1, (t->getStandardName("Soulburn") + std::string(": ") + std::to_string(attributes->baseSoulBurn)).c_str());
      mvwprintw(characterScreen, 6, 1, (t->getStandardName("Flow") + std::string(": ") + std::to_string(attributes->baseFlow)).c_str());
      mvwprintw(characterScreen, 7, 1, (t->getStandardName("Vision Range") + std::string(": ") + std::to_string(attributes->baseVisionRange)).c_str());
      mvwprintw(characterScreen, 8, 1, (t->getStandardName("Vision Power") + std::string(": ") + std::to_string(attributes->baseVisionPower)).c_str());
      mvwprintw(characterScreen, 9, 1, (t->getStandardName("Detection Range") + std::string(": ") + std::to_string(attributes->baseDetectionRange)).c_str());
    }
    mvwprintw(characterScreen, 1, space, (std::string("+") + t->getStandardName("Hp") + std::string(": ") + std::to_string(hpIncr)).c_str());
    mvwprintw(characterScreen, 2, space, (std::string("+") + t->getStandardName("Mana") + std::string(": ") + std::to_string(manaIncr)).c_str());
    mvwprintw(characterScreen, 3, space, (std::string("+") + t->getStandardName("Shield") + std::string(": ") + std::to_string(shieldIncr)).c_str());
    mvwprintw(characterScreen, 4, space, (std::string("+") + t->getStandardName("Damage") + std::string(": ") + std::to_string(damageMultIncr)).c_str());
    mvwprintw(characterScreen, 5, space, (std::string("+") + t->getStandardName("Soulburn") + std::string(": ") + std::to_string(soulBurnIncr)).c_str());
    mvwprintw(characterScreen, 6, space, (std::string("+") + t->getStandardName("Flow") + std::string(": ") + std::to_string(flowIncr)).c_str());
    wattroff(characterScreen, COLOR_PAIR(color));
    return characterScreen;
  }

  bool compatibleWays(Way * way1, Way * way2, Way * way3, Way * way4, Way * way5, std::list<std::pair<const std::string, const std::string>> waysIncompatibilities) {
    std::list<std::pair<const std::string, const std::string>> pairs = std::list<std::pair<const std::string, const std::string>>();
    if(way1 != nullptr) {
      if(way2 != nullptr) {
        pairs.push_back(std::make_pair(way1->name, way2->name));
      }
      if(way3 != nullptr) {
        pairs.push_back(std::make_pair(way1->name, way3->name));
      }
      if(way4 != nullptr) {
        pairs.push_back(std::make_pair(way1->name, way4->name));
      }
      if(way5 != nullptr) {
        pairs.push_back(std::make_pair(way1->name, way5->name));
      }
    }
    if(way2 != nullptr) {
      if(way3 != nullptr) {
        pairs.push_back(std::make_pair(way2->name, way3->name));
      }
      if(way4 != nullptr) {
        pairs.push_back(std::make_pair(way2->name, way4->name));
      }
      if(way5 != nullptr) {
        pairs.push_back(std::make_pair(way2->name, way5->name));
      }
    }
    if(way3 != nullptr) {
      if(way4 != nullptr) {
        pairs.push_back(std::make_pair(way3->name, way4->name));
      }
      if(way5 != nullptr) {
        pairs.push_back(std::make_pair(way3->name, way5->name));
      }
    }
    if(way4 != nullptr && way5 != nullptr) {
      pairs.push_back(std::make_pair(way4->name, way5->name));
    }
    for(std::pair<const std::string, const std::string> pair1 : pairs) {
      for(std::pair<const std::string, const std::string> pair2 : waysIncompatibilities) {
        if((pair1.first == pair2.first && pair1.second == pair2.second) ||
          (pair1.first == pair2.second && pair1.second == pair2.first)) {
            return false;
        }
      }
    }
    return true;
  }

  std::vector<std::string> selectChoices(
    std::vector<Attributes *> startingAttributes,
    std::vector<Way *> startingWays,
    std::list<std::pair<const std::string, const std::string>> waysIncompatibilities,
    Translator * t)
  {
    Attributes * selectedAttributes = nullptr;
    Way * selectedRace = nullptr;
    Way * selectedOrigin = nullptr;
    Way * selectedCulture = nullptr;
    Way * selectedReligion = nullptr;
    Way * selectedProfession = nullptr;
    std::vector<Way *> races = std::vector<Way *>();
    std::vector<Way *> origins = std::vector<Way *>();
    std::vector<Way *> cultures = std::vector<Way *>();
    std::vector<Way *> religions = std::vector<Way *>();
    std::vector<Way *> professions = std::vector<Way *>();
    std::string characterName = "";
    std::vector<std::string> result = std::vector<std::string>(7);
    for(Way * way : startingWays) {
      switch(way->type) {
        case WAY_RACE:
          races.push_back(way);
          break;
        case WAY_ORIGIN:
          origins.push_back(way);
          break;
        case WAY_CULTURE:
          cultures.push_back(way);
          break;
        case WAY_RELIGION:
          religions.push_back(way);
          break;
        case WAY_PROFESSION:
          professions.push_back(way);
          break;
        default:
          ;
      }
    }
    bool done = false;
    int cursorX = 0;
    int cursorY = 0;
    clear();
    int characterWidth = COLS - (ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 5) - 2;
    WINDOW * characterScreen = subwin(stdscr, LINES, characterWidth + 2, 0, 0);
    WINDOW * attributesScreen = subwin(stdscr, LINES, ATTRIBUTES_WIDTH + 2, 0, characterWidth + 2);
    WINDOW * raceScreen = subwin(stdscr, LINES, WAY_WIDTH + 2, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2);
    WINDOW * originScreen = subwin(stdscr, LINES, WAY_WIDTH + 2, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + WAY_WIDTH + 2);
    WINDOW * cultureScreen = subwin(stdscr, LINES, WAY_WIDTH + 2, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 2);
    WINDOW * religionScreen = subwin(stdscr, LINES, WAY_WIDTH + 2, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 3);
    WINDOW * professionScreen = subwin(stdscr, LINES, WAY_WIDTH + 2, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 4);
    int lines = 0;
    int cols = 0;
    getmaxyx(attributesScreen, lines, cols);
    int numberAttributes = lines / ATTRIBUTES_HEIGHT;
    getmaxyx(raceScreen, lines, cols);
    int numberWays = lines / WAY_HEIGHT;
    std::list<WINDOW *> screens = std::list<WINDOW *>();
    int currentPannel = 0;
    bool nameMode = false;
    while(!done) {
      wclear(attributesScreen);
      wclear(raceScreen);
      wclear(originScreen);
      wclear(cultureScreen);
      wclear(religionScreen);
      wclear(professionScreen);
      wclear(characterScreen);
      box(attributesScreen, ACS_VLINE, ACS_HLINE);
      box(raceScreen, ACS_VLINE, ACS_HLINE);
      box(originScreen, ACS_VLINE, ACS_HLINE);
      box(cultureScreen, ACS_VLINE, ACS_HLINE);
      box(religionScreen, ACS_VLINE, ACS_HLINE);
      box(professionScreen, ACS_VLINE, ACS_HLINE);
      box(characterScreen, ACS_VLINE, ACS_HLINE);
      int attributesCount = 0;
      int raceCount = 0;
      int originCount = 0;
      int cultureCount = 0;
      int religionCount = 0;
      int professionCount = 0;
      int skip = 0;
      std::vector<Way *> availableRaces = std::vector<Way *>(races.size());
      std::vector<Way *> availableOrigins = std::vector<Way *>(origins.size());
      std::vector<Way *> availableCultures = std::vector<Way *>(cultures.size());
      std::vector<Way *> availableReligions = std::vector<Way *>(religions.size());
      std::vector<Way *> availableProfessions = std::vector<Way *>(professions.size());
      std::string to_print = t->getStandardName("CHARACTER");
      mvwprintw(characterScreen, 1, 1 + characterWidth / 2 - to_print.length() / 2, to_print.c_str());
      to_print = t->getStandardName("CLASS");
      mvwprintw(attributesScreen, 1, 1 + ATTRIBUTES_WIDTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = t->getStandardName("RACE");
      mvwprintw(raceScreen, 1, 1 + WAY_WIDTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = t->getStandardName("ORIGIN");
      mvwprintw(originScreen, 1, 1 + WAY_WIDTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = t->getStandardName("CULTURE");
      mvwprintw(cultureScreen, 1, 1 + WAY_WIDTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = t->getStandardName("RELIGION");
      mvwprintw(religionScreen, 1, 1 + WAY_WIDTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = t->getStandardName("PROFESSION");
      mvwprintw(professionScreen, 1, 1 + WAY_WIDTH / 2 - to_print.length() / 2, to_print.c_str());
      for(Attributes * attributes : startingAttributes) {
        if(skip++ < currentPannel * numberAttributes) {
          continue;
        }
        int color = WHITE;
        if(selectedAttributes != nullptr && selectedAttributes->name == attributes->name) {
          color = GREEN;
        }
        if(cursorX == 1 && cursorY == attributesCount) {
          color = BLUE;
        }
        screens.push_back(displayAttributes(attributes, attributesCount++, color, attributesScreen, 0, characterWidth + 2, t));
      }
      wrefresh(attributesScreen);
      skip = 0;
      for(Way * way : races) {
        int color = WHITE;
        if(selectedRace != nullptr && selectedRace->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 2 && cursorY == raceCount) {
          color = BLUE;
        }
        if(compatibleWays(way, selectedOrigin, selectedCulture, selectedReligion, selectedProfession, waysIncompatibilities)) {
          if(skip++ < currentPannel * numberWays) {
            continue;
          }
          screens.push_back(displayWay(way, raceCount, color, raceScreen, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2, t));
          availableRaces[raceCount++] = way;
        }
      }
      availableRaces.resize(raceCount);
      wrefresh(raceScreen);
      skip = 0;
      for(Way * way : origins) {
        int color = WHITE;
        if(selectedOrigin != nullptr && selectedOrigin->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 3 && cursorY == originCount) {
          color = BLUE;
        }
        if(compatibleWays(way, selectedRace, selectedCulture, selectedReligion, selectedProfession, waysIncompatibilities)) {
          if(skip++ < currentPannel * numberWays) {
            continue;
          }
          screens.push_back(displayWay(way, originCount, color, originScreen, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + WAY_WIDTH + 2, t));
          availableOrigins[originCount++] = way;
        }
      }
      availableOrigins.resize(originCount);
      wrefresh(originScreen);
      skip = 0;
      for(Way * way : cultures) {
        int color = WHITE;
        if(selectedCulture != nullptr && selectedCulture->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 4 && cursorY == cultureCount) {
          color = BLUE;
        }
        if(compatibleWays(way, selectedRace, selectedOrigin, selectedReligion, selectedProfession, waysIncompatibilities)) {
          if(skip++ < currentPannel * numberWays) {
            continue;
          }
          screens.push_back(displayWay(way, cultureCount, color, cultureScreen, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 2, t));
          availableCultures[cultureCount++] = way;
        }
      }
      availableCultures.resize(cultureCount);
      wrefresh(cultureScreen);
      skip = 0;
      for(Way * way : religions) {
        int color = WHITE;
        if(selectedReligion != nullptr && selectedReligion->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 5 && cursorY == religionCount) {
          color = BLUE;
        }
        if(compatibleWays(way, selectedRace, selectedOrigin, selectedCulture, selectedProfession, waysIncompatibilities)) {
          if(skip++ < currentPannel * numberWays) {
            continue;
          }
          screens.push_back(displayWay(way, religionCount, color, religionScreen, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 3, t));
          availableReligions[religionCount++] = way;
        }
      }
      availableReligions.resize(religionCount);
      wrefresh(religionScreen);
      skip = 0;
      for(Way * way : professions) {
        int color = WHITE;
        if(selectedProfession != nullptr && selectedProfession->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 6 && cursorY == professionCount) {
          color = BLUE;
        }
        if(compatibleWays(way, selectedRace, selectedOrigin, selectedCulture, selectedReligion, waysIncompatibilities)) {
          if(skip++ < currentPannel * numberWays) {
            continue;
          }
          screens.push_back(displayWay(way, professionCount, color, professionScreen, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 4, t));
          availableProfessions[professionCount++] = way;
        }
      }
      availableProfessions.resize(professionCount);
      wrefresh(professionScreen);
      int color = WHITE;
      if(characterName != "" && characterName.find(';') == std::string::npos && characterName.find('|') == std::string::npos && characterName.find('@') == std::string::npos
        && characterName.find('&') == std::string::npos && characterName.find('%') == std::string::npos) {
        color = GREEN;
      }
      if(cursorX == 0 && cursorY == 0) {
        color = BLUE;
      }
      WINDOW * nameScreen = subwin(characterScreen, 3, characterWidth, 2, 1);
      screens.push_back(nameScreen);
      wattron(nameScreen, COLOR_PAIR(color));
      box(nameScreen, ACS_VLINE, ACS_HLINE);
      mvwprintw(nameScreen, 1, 1, (t->getStandardName("Name") + std::string(": ") + characterName).c_str());
      wattroff(nameScreen, COLOR_PAIR(color));
      color = WHITE;
      if(cursorX == 0 && cursorY == 1) {
        color = BLUE;
      }
      screens.push_back(displayCharacter(selectedAttributes, selectedRace, selectedOrigin, selectedCulture, selectedReligion, selectedProfession, color, characterScreen, COLS - (ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 5) - 2, 5, 1, t));
      color = WHITE;
      if(characterName != "" && characterName.find(';') == std::string::npos && characterName.find('|') == std::string::npos && characterName.find('@') == std::string::npos
        && characterName.find('&') == std::string::npos && characterName.find('%') == std::string::npos && !nameMode && selectedAttributes != nullptr
        && selectedRace != nullptr && selectedOrigin != nullptr && selectedCulture != nullptr && selectedReligion != nullptr && selectedProfession != nullptr) {
        color = GREEN;
      }
      if(cursorX == 0 && cursorY == 2) {
        color = BLUE;
      }
      WINDOW * descriptionScreen = subwin(characterScreen, LINES - (3 + 5 + ATTRIBUTES_HEIGHT - 1), characterWidth, 5 + ATTRIBUTES_HEIGHT - 2, 1);
      switch(cursorX) {
        case 0:
          switch(cursorY) {
            case 0: to_print = t->getStandardDesc("Name"); break;
            case 1: to_print = t->getStandardDesc("CHARACTER"); break;
            case 2: to_print = t->getStandardDesc("CONFIRM CHARACTER CREATION"); break;
            default: to_print = "";
          }
          break;
        case 1: to_print = t->getAttributesDesc(startingAttributes[cursorY]->name); break;
        case 2: to_print = t->getWayDesc(availableRaces[cursorY]->name); break;
        case 3: to_print = t->getWayDesc(availableOrigins[cursorY]->name); break;
        case 4: to_print = t->getWayDesc(availableCultures[cursorY]->name); break;
        case 5: to_print = t->getWayDesc(availableReligions[cursorY]->name); break;
        case 6: to_print = t->getWayDesc(availableProfessions[cursorY]->name); break;
        default: to_print = "";
      }
      print(descriptionScreen, 0, 0, to_print.c_str());
      WINDOW * confirmScreen = subwin(characterScreen, 3, characterWidth, LINES - 4, 1);
      screens.push_back(confirmScreen);
      wattron(confirmScreen, COLOR_PAIR(color));
      box(confirmScreen, ACS_VLINE, ACS_HLINE);
      to_print = t->getStandardName("CONFIRM CHARACTER CREATION");
      mvwprintw(confirmScreen, 1, characterWidth / 2 - to_print.length() / 2, to_print.c_str());
      wattroff(confirmScreen, COLOR_PAIR(color));
      wrefresh(characterScreen);
      bool done2 = false;
      while(!done2) {
        flushinp();
        int keyPressed = getch();
        if(!nameMode) {
          switch(keyPressed) {
            case '4':
            case KEY_LEFT:
              cursorX = (cursorX - 1 + 7) % 7;
              if(cursorX == 0) {
                cursorY = std::min(cursorY, 2);
              }
              else if(cursorX == 1) {
                cursorY = std::min(cursorY, (int) startingAttributes.size() - 1 - currentPannel * numberAttributes);
              }
              else if(cursorX == 2) {
                cursorY = std::min(cursorY, (int) availableRaces.size() - 1);
              }
              else if(cursorX == 3) {
                cursorY = std::min(cursorY, (int) availableOrigins.size() - 1);
              }
              else if(cursorX == 4) {
                cursorY = std::min(cursorY, (int) availableCultures.size() - 1);
              }
              else if(cursorX == 5) {
                cursorY = std::min(cursorY, (int) availableReligions.size() - 1);
              }
              else if(cursorX == 6) {
                cursorY = std::min(cursorY, (int) availableProfessions.size() - 1);
              }
              if(cursorY < 0) {
                cursorY = 0;
              }
              done2 = true;
              break;
            case '8':
            case KEY_UP: {
              cursorY--;
              if(cursorX == 0) {
                cursorY = (cursorY + 3) % 3;
              }
              else if(cursorX == 1) {
                if(cursorY == -1 && currentPannel > 0) {
                  cursorY = numberAttributes -1;
                  currentPannel--;
                }
                else if(cursorY == -1) {
                  cursorY++;
                }
              }
              else if(cursorX == 2) {
                if(cursorY == -1 && currentPannel > 0) {
                  cursorY = numberWays -1;
                  currentPannel--;
                }
                else if(cursorY == -1) {
                  cursorY++;
                }
              }
              else if(cursorX == 3) {
                if(cursorY == -1 && currentPannel > 0) {
                  cursorY = numberWays -1;
                  currentPannel--;
                }
                else if(cursorY == -1) {
                  cursorY++;
                }
              }
              else if(cursorX == 4) {
                if(cursorY == -1 && currentPannel > 0) {
                  cursorY = numberWays -1;
                  currentPannel--;
                }
                else if(cursorY == -1) {
                  cursorY++;
                }
              }
              else if(cursorX == 5) {
                if(cursorY == -1 && currentPannel > 0) {
                  cursorY = numberWays -1;
                  currentPannel--;
                }
                else if(cursorY == -1) {
                  cursorY++;
                }
              }
              else if(cursorX == 6) {
                if(cursorY == -1 && currentPannel > 0) {
                  cursorY = numberWays -1;
                  currentPannel--;
                }
                else if(cursorY == -1) {
                  cursorY++;
                }
              }
              done2 = true;
              break;
            }
            case '6':
            case KEY_RIGHT:
              cursorX = (cursorX + 1) % 7;
              if(cursorX == 0) {
                cursorY = std::min(cursorY, 2);
              }
              else if(cursorX == 1) {
                cursorY = std::min(cursorY, (int) startingAttributes.size() - 1);
              }
              else if(cursorX == 2) {
                cursorY = std::min(cursorY, (int) availableRaces.size() - 1);
              }
              else if(cursorX == 3) {
                cursorY = std::min(cursorY, (int) availableOrigins.size() - 1);
              }
              else if(cursorX == 4) {
                cursorY = std::min(cursorY, (int) availableCultures.size() - 1);
              }
              else if(cursorX == 5) {
                cursorY = std::min(cursorY, (int) availableReligions.size() - 1);
              }
              else if(cursorX == 6) {
                cursorY = std::min(cursorY, (int) availableProfessions.size() - 1);
              }
              if(cursorY < 0) {
                cursorY = 0;
              }
              done2 = true;
              break;
            case '2':
            case KEY_DOWN: {
              cursorY++;
              if(cursorX == 0) {
                cursorY = cursorY % 3;
              }
              else if(cursorX == 1) {
                if(cursorY == numberAttributes && (int) startingAttributes.size() > numberAttributes) {
                  cursorY = 0;
                  currentPannel++;
                }
                else if(cursorY == (int) startingAttributes.size()) {
                  cursorY--;
                }
              }
              else if(cursorX == 2) {
                if(cursorY == numberWays && (int) availableRaces.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                }
                else if(cursorY == (int) availableRaces.size()) {
                  cursorY--;
                }
              }
              else if(cursorX == 3) {
                if(cursorY == numberWays && (int) availableOrigins.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                }
                else if(cursorY == (int) availableOrigins.size()) {
                  cursorY--;
                }
              }
              else if(cursorX == 4) {
                if(cursorY == numberWays && (int) availableCultures.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                }
                else if(cursorY == (int) availableCultures.size()) {
                  cursorY--;
                }
              }
              else if(cursorX == 5) {
                if(cursorY == numberWays && (int) availableReligions.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                }
                else if(cursorY == (int) availableReligions.size()) {
                  cursorY--;
                }
              }
              else if(cursorX == 6) {
                if(cursorY == numberWays && (int) availableProfessions.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                }
                else if(cursorY == (int) availableProfessions.size()) {
                  cursorY--;
                }
              }
              done2 = true;
              break;
            }
            case '\n': {
              if(cursorX == 0) {
                if(cursorY == 0) {
                  nameMode = true;
                  move(3, 2 + (t->getStandardName("Name") + characterName).length() + 2);
                  curs_set(1);
                }
                else if(cursorY == 2) {
                  if(characterName != "" && characterName.find(';') == std::string::npos && characterName.find('|') == std::string::npos && characterName.find('@') == std::string::npos
                    && characterName.find('&') == std::string::npos && characterName.find('%') == std::string::npos && !nameMode && selectedAttributes != nullptr
                    && selectedRace != nullptr && selectedOrigin != nullptr && selectedCulture != nullptr && selectedReligion != nullptr && selectedProfession != nullptr) {
                    done = true;
                  }
                }
              }
              else if(cursorX == 1) {
                if(selectedAttributes == startingAttributes[cursorY + currentPannel * numberAttributes]) {
                  selectedAttributes = nullptr;
                }
                else {
                  selectedAttributes = startingAttributes[cursorY + currentPannel * numberAttributes];
                }
              }
              else if(cursorX == 2 && availableRaces.size() > 0) {
                if(selectedRace == availableRaces[cursorY]) {
                  selectedRace = nullptr;
                }
                else {
                  selectedRace = availableRaces[cursorY];
                }
              }
              else if(cursorX == 3 && availableOrigins.size() > 0) {
                if(selectedOrigin == availableOrigins[cursorY]) {
                  selectedOrigin = nullptr;
                }
                else {
                  selectedOrigin = availableOrigins[cursorY];
                }
              }
              else if(cursorX == 4 && availableCultures.size() > 0) {
                if(selectedCulture == availableCultures[cursorY]) {
                  selectedCulture = nullptr;
                }
                else {
                  selectedCulture = availableCultures[cursorY];
                }
              }
              else if(cursorX == 5 && availableReligions.size() > 0) {
                if(selectedReligion == availableReligions[cursorY]) {
                  selectedReligion = nullptr;
                }
                else {
                  selectedReligion = availableReligions[cursorY];
                }
              }
              else if(cursorX == 6 && availableProfessions.size() > 0) {
                if(selectedProfession == availableProfessions[cursorY]) {
                  selectedProfession = nullptr;
                }
                else {
                  selectedProfession = availableProfessions[cursorY];
                }
              }
              done2 = true;
              break;
            }
            default:
              ;
          }
        }
        else { // nameMode is true
          switch(keyPressed) {
            case '\n':
              nameMode = false;
              done2 = true;
              curs_set(0);
              break;
            case KEY_LEFT:
            case KEY_UP:
            case KEY_RIGHT:
            case KEY_DOWN:
              break;
            case KEY_BACKSPACE:
              if(characterName != "") {
                characterName = characterName.substr(0, characterName.length() - 1);
                move(3, 2 + (t->getStandardName("Name") + characterName).length() + 2);
              }
              done2 = true;
              break;
            default:
              characterName += keyPressed;
              move(3, 2 + (t->getStandardName("Name") + characterName).length() + 2);
              done2 = true;
          }
        }
      }
      for(WINDOW * screen : screens) {
        wclear(screen);
        delwin(screen);
      }
      screens.clear();
      availableRaces.clear();
      availableOrigins.clear();
      availableCultures.clear();
      availableReligions.clear();
      availableProfessions.clear();
    }
    clear();
    result[0] = characterName;
    result[1] = selectedAttributes->name;
    result[2] = selectedRace->name;
    result[3] = selectedOrigin->name;
    result[4] = selectedCulture->name;
    result[5] = selectedReligion->name;
    result[6] = selectedProfession->name;
    return result;
  }

  void commandLoop(Link * link, WINDOW * mapScreen, WINDOW * statsScreen, WINDOW * displayScreen, WINDOW * targetScreen, Translator * t) {
    while(!link->isReady()) {
      usleep(1);
    }
    long id = link->getPlayersId().front();
    while(true) {
      StateDisplay * display = link->getState();
      Region * region = link->getPlayer(id)->getRegion();
      if(display->need_to_send_actions) {
        displayMap(display, link->getAdventure(), link->getPlayer(id), mapScreen, t);
        displayStats(link->getPlayer(id), statsScreen, t);
        displayCommands(targetScreen, t);
        wrefresh(targetScreen);
        bool done = false;
        int type;
        int object_type;
        std::string object;
        int object_id;
        float orientation;
        Skill * skill;
        int target_id;
        int target_x;
        int target_y;
        int overcharge_power;
        int overcharge_duration;
        int overcharge_range;
        while(!done) {
          object_type = 0;
          object = "";
          object_id = 0;
          orientation = link->getPlayer(id)->getOrientation();
          skill = nullptr;
          target_id = 0;
          target_x = (int) std::floor(link->getPlayer(id)->getCoord().x) - region->id.x;
          target_y = (int) std::floor(link->getPlayer(id)->getCoord().y) - region->id.y;
          overcharge_power = 1;
          overcharge_duration = 1;
          overcharge_range = 1;
          flushinp();
          int keyPressed = getch();
          switch(keyPressed) {
            case '5':
              type = ACTION_IDLE;
              done = true;
              break;
            case '1':
            case '2':
            case KEY_DOWN:
            case '3':
            case '4':
            case KEY_LEFT:
            case '6':
            case KEY_RIGHT:
            case '7':
            case '8':
            case KEY_UP:
            case '9':
              if(selectTarget(mapScreen, targetScreen, display, link->getPlayer(id), link->getPlayer(id)->getVisionRange(), target_id, target_x, target_y, orientation, t)) {
                type = ACTION_MOVE;
                done = true;
              }
              break;
            case ' ':
              type = ACTION_GRAB;
              object_id = 0;
              done = true;
              break;
            case 'x':
            case 'X':
              type = ACTION_USE_SKILL;
              skill = selectSkill(displayScreen, targetScreen, link->getPlayer(id), overcharge_power, overcharge_duration, overcharge_range, t);
              if(skill != nullptr && (skill->target_type == TARGET_SELF || selectTarget(mapScreen, targetScreen, display, link->getPlayer(id), skill->range, target_id, target_x, target_y, orientation, t))) {
                done = true;
                object = skill->name;
              }
              break;
            case 'c':
            case 'C':
              type = ACTION_STRIKE;
              if(selectTarget(mapScreen, targetScreen, display, link->getPlayer(id), link->getPlayer(id)->getGear()->getWeapon_1()->range, target_id, target_x, target_y, orientation, t)) {
                if(link->getPlayer(id)->getGear()->getWeapon_1()->use_ammo) {
                  link->getPlayer(id)->getGear()->getWeapon_1()->useAmmo();
                }
                done = true;
              }
              break;
            case 'i':
            case 'I':
              type = ACTION_SWAP_GEAR;
              object = selectItem(displayScreen, targetScreen, link->getPlayer(id), object_type, object_id, t);
              if(object != "") {
                for(ItemSlot * slot : link->getPlayer(id)->getGear()->getItems()) {
                  if(slot->item->id == object_id) {
                    if(slot->item->usable) {
                      type = ACTION_USE_ITEM;
                    }
                    if(slot->item->type == ITEM_AMMUNITION) {
                      type = ACTION_RELOAD;
                    }
                  }
                }
                done = true;
              }
              break;
            case 'r':
            case 'R':
              type = ACTION_RELOAD;
              if(link->getPlayer(id)->getGear()->getWeapon_1()->use_ammo) {
                object = selectAmmo(displayScreen, targetScreen, link->getPlayer(id), t);
                if(object != "" && (link->getPlayer(id)->getGear()->getWeapon_1()->getAmmo() == nullptr
                || link->getPlayer(id)->getGear()->getWeapon_1()->getAmmo()->getProjectile()->name != object
                || link->getPlayer(id)->getGear()->getWeapon_1()->getCurrentCapacity() < link->getPlayer(id)->getGear()->getWeapon_1()->capacity)) {
                  done = true;
                }
              }
              break;
            default:
              ;
          }
        }
        switch(type) {
          case ACTION_IDLE:
          case ACTION_RESPITE:
          case ACTION_REST:
          case ACTION_BREAKPOINT:
            sendAction(link, id, type, nullptr, nullptr, 0, 0, 0);
            break;
          case ACTION_MOVE:
          case ACTION_STRIKE:
          case ACTION_USE_SKILL: {
            Target * target = new Target();
            ((Target *) target)->type = (target_id == 0 ? TARGET_COORDINATES : TARGET_CHARACTER);
            ((Target *) target)->id = (target_id == 0 ? 0 : target_id);
            ((Target *) target)->x = target_x + 0.5;
            ((Target *) target)->y = target_y + 0.5;
            ((Target *) target)->next = nullptr;
            sendAction(link, id, type, (void *) target, skill, overcharge_power, overcharge_duration, overcharge_range);
            break;
          }
          case ACTION_RELOAD:
          case ACTION_GRAB:
          case ACTION_USE_ITEM: {
            ItemSlot * slot = new ItemSlot();
            for(ItemSlot * item : link->getPlayer(id)->getGear()->getItems()) {
              if(item->item->id == object_id) {
                slot->x = item->x;
                slot->y = item->y;
                slot->slot = item->slot;
              }
            }
            sendAction(link, id, type, (void *) slot, nullptr, 0, 0, 0);
            break;
          }
          case ACTION_SWAP_GEAR: {
            ItemSlot * slot = new ItemSlot();
            ItemSlot * dummy = new ItemSlot();
            for(ItemSlot * item : link->getPlayer(id)->getGear()->getItems()) {
              if(item->item->id == object_id) {
                slot->x = item->x;
                slot->y = item->y;
                slot->slot = item->slot;
              }
            }
            dummy->x = 0;
            dummy->y = 0;
            dummy->slot = 0;
            sendAction(link, id, type, (void *) slot, (void *) dummy, 0, 0, 0);
            break;
          }
          case ACTION_TALKING:
          case ACTION_ECONOMICS:
            break;
          default: ;
        }
      }
      for(CharacterDisplay * character : display->characters) {
        delete character;
      }
      for(ProjectileDisplay * projectile : display->projectiles) {
        delete projectile;
      }
      for(Loot * loot : display->loots) {
        delete loot;
      }
      delete display;
    }
  }

  Skill * selectSkill(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, int & overcharge_power, int & overcharge_duration, int & overcharge_range, Translator * t) {
    Skill * result = nullptr;
    bool done = false;
    int cursorX = 0;
    int cursorY = 0;
    int lines = 0;
    int cols = 0;
    getmaxyx(displayScreen, lines, cols);
    while(!done) {
      WINDOW * tempScreen = nullptr;
      wclear(displayScreen);
      wclear(targetScreen);
      box(displayScreen, ACS_VLINE, ACS_HLINE);
      box(targetScreen, ACS_VLINE, ACS_HLINE);
      std::string to_print = t->getStandardName("SKILLS");
      mvwprintw(displayScreen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
      int currentX = 0;
      int currentY = 0;
      int offset = 0;
      int sizeX = 0;
      int maxY = 0;
      int color = WHITE;
      for (Skill * skill : player->getSkills()) {
        if(currentY >= lines - 4) {
          offset += sizeX + 3;
          maxY = currentY - 1;
          currentY = 0;
          currentX++;
        }
        if(cursorX == currentX && cursorY == currentY) {
          color = BLUE;
        }
        else {
          color = WHITE;
        }
        std::string to_print = t->getSkillName(skill->name);
        if(color == BLUE) {
          result = skill;
          tempScreen = displaySkill(skill, targetScreen, 1, 1, 1, t);
        }
        sizeX = std::max(sizeX, (int) to_print.length());
        wattron(displayScreen, COLOR_PAIR(color));
        mvwprintw(displayScreen, 3 + currentY++, offset + 1, to_print.c_str());
        wattroff(displayScreen, COLOR_PAIR(color));
      }
      maxY = std::max(--currentY, maxY);

      wrefresh(displayScreen);
      wrefresh(targetScreen);
      flushinp();
      int keyPressed = getch();
      switch(keyPressed) {
        case '4':
        case KEY_LEFT:
          if(cursorX > 0) {
            cursorX--;
          }
          break;
        case '8':
        case KEY_UP: {
          if(cursorY > 0) {
            cursorY--;
          }
          break;
        }
        case '6':
        case KEY_RIGHT:
          if(cursorX < currentX) {
            cursorX++;
          }
          break;
        case '2':
        case KEY_DOWN: {
          if(cursorY < maxY) {
            cursorY++;
          }
          break;
        }
        case '\n':
          done = selectOvercharge(displayScreen, targetScreen, result, player, overcharge_power, overcharge_duration, overcharge_range, t);
          break;
        case ' ':
          done = true;
          result = nullptr;
        default:
          ;
      }
      if(tempScreen != nullptr) {
        wclear(tempScreen);
        delwin(tempScreen);
      }
    }
    wclear(displayScreen);
    wclear(targetScreen);
    box(displayScreen, ACS_VLINE, ACS_HLINE);
    box(targetScreen, ACS_VLINE, ACS_HLINE);
    wrefresh(displayScreen);
    wrefresh(targetScreen);
    return result;
  }

  bool selectOvercharge(WINDOW * displayScreen, WINDOW * targetScreen, Skill * skill, Character * player, int & overcharge_power, int & overcharge_duration, int & overcharge_range, Translator * t) {
    int mana_cost = 0;
    int lines = 0;
    int cols = 0;
    int overcharge_type = 1;
    int color = WHITE;
    getmaxyx(displayScreen, lines, cols);
    while(true) {
      wclear(displayScreen);
      box(displayScreen, ACS_VLINE, ACS_HLINE);
      std::string to_print = t->getStandardName("Overcharging") + std::string(" ") + t->getSkillName(skill->name);
      mvwprintw(displayScreen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
      if(overcharge_type == 1) {
        color = BLUE;
      }
      else {
        color = WHITE;
      }
      to_print = t->getStandardName("Power Overcharging") + std::string(": ") + std::to_string(overcharge_power) + std::string(" - ") + t->getStandardName(std::string("overcharge_type_") + std::to_string(skill->overcharge_power_type));
      wattron(displayScreen, COLOR_PAIR(color));
      mvwprintw(displayScreen, lines / 2 - 1, cols / 2 - to_print.length() / 2, to_print.c_str());
      wattroff(displayScreen, COLOR_PAIR(color));
      if(overcharge_type == 2) {
        color = BLUE;
      }
      else {
        color = WHITE;
      }
      to_print = t->getStandardName("Duration Overcharging") + std::string(": ") + std::to_string(overcharge_duration) + std::string(" - ") + t->getStandardName(std::string("overcharge_type_") + std::to_string(skill->overcharge_duration_type));
      wattron(displayScreen, COLOR_PAIR(color));
      mvwprintw(displayScreen, lines / 2, cols / 2 - to_print.length() / 2, to_print.c_str());
      wattroff(displayScreen, COLOR_PAIR(color));
      if(overcharge_type == 3) {
        color = BLUE;
      }
      else {
        color = WHITE;
      }
      to_print = t->getStandardName("Range Overcharging") + std::string(": ") + std::to_string(overcharge_range) + std::string(" - ") + t->getStandardName(std::string("overcharge_type_") + std::to_string(skill->overcharge_range_type));
      wattron(displayScreen, COLOR_PAIR(color));
      mvwprintw(displayScreen, lines / 2 + 1, cols / 2 - to_print.length() / 2, to_print.c_str());
      wattroff(displayScreen, COLOR_PAIR(color));
      wrefresh(displayScreen);
      WINDOW * tempScreen = nullptr;
      tempScreen = displaySkill(skill, targetScreen, overcharge_power, overcharge_duration, overcharge_range, t);
      flushinp();
      int keyPressed = getch();
      if(tempScreen != nullptr) {
        wclear(tempScreen);
        delwin(tempScreen);
      }
      switch(keyPressed) {
        case '4':
        case KEY_LEFT:
          switch(overcharge_type) {
            case 1:
              if(overcharge_power > 1) {
                overcharge_power--;
              }
              break;
            case 2:
              if(overcharge_duration > 1) {
                overcharge_duration--;
              }
              break;
            case 3:
              if(overcharge_range > 1) {
                overcharge_range--;
              }
              break;
          }
          break;
        case '8':
        case KEY_UP: {
          if(overcharge_type > 1) {
            overcharge_type--;
          }
          break;
        }
        case '6':
        case KEY_RIGHT:
          switch(overcharge_type) {
            case 1:
              mana_cost = skill->getManaCost(overcharge_power + 1, overcharge_duration, overcharge_range);
              if(skill->overcharge_power_type != OVERCHARGE_NONE && player->getMana() >= mana_cost && player->getFlow() >= mana_cost) {
                overcharge_power++;
              }
              break;
            case 2:
              mana_cost = skill->getManaCost(overcharge_power, overcharge_duration + 1, overcharge_range);
              if(skill->overcharge_duration_type != OVERCHARGE_NONE && player->getMana() >= mana_cost && player->getFlow() >= mana_cost) {
                overcharge_duration++;
              }
              break;
            case 3:
              mana_cost = skill->getManaCost(overcharge_power, overcharge_duration, overcharge_range + 1);
              if(skill->overcharge_range_type != OVERCHARGE_NONE && player->getMana() >= mana_cost && player->getFlow() >= mana_cost) {
                overcharge_range++;
              }
              break;
          }
          break;
        case '2':
        case KEY_DOWN: {
          if(overcharge_type < 3) {
            overcharge_type++;
          }
          break;
        }
        case '\n':
          mana_cost = skill->getManaCost(overcharge_power, overcharge_duration, overcharge_range);
          if(player->getMana() >= mana_cost && player->getFlow() >= mana_cost) {
            return true;
          }
          break;
        case ' ':
          return false;
        default:
          ;
      }
    }
    return false;
  }

  bool selectTarget(WINDOW * mapScreen, WINDOW * targetScreen, StateDisplay * display, Character * player, int range, int & target_id, int & target_x, int & target_y, float & orientation, Translator * t) {
    bool done = false;
    int lines = 0;
    int cols = 0;
    int player_x = target_x;
    int player_y = target_y;
    getmaxyx(mapScreen, lines, cols);
    int lines2 = 0;
    int cols2 = 0;
    getmaxyx(targetScreen, lines2, cols2);
    cchar_t *wch_old = new cchar_t();
    mvwin_wch(mapScreen, lines / 2 - CHUNK_SIZE * 3 / 2 + CHUNK_SIZE * 3 - 1 - target_y, target_x + cols / 2 - CHUNK_SIZE * 3 / 2, wch_old);
    Region * region = player->getRegion();
    while(!done) {
      flushinp();
      int keyPressed = getch();
      target_id = 0;
      int previous_x = target_x;
      int previous_y = target_y;
      wmove(mapScreen, lines / 2 - CHUNK_SIZE * 3 / 2 + CHUNK_SIZE * 3 - 1 - target_y, target_x + cols / 2 - CHUNK_SIZE * 3 / 2);
      wecho_wchar(mapScreen, wch_old);
      switch(keyPressed) {
        case '4':
        case KEY_LEFT:
          if(target_x > 0) {
            target_x--;
          }
          break;
        case '7':
          if(target_x > 0 && target_y < CHUNK_SIZE * 3 - 1) {
            target_x--;
            target_y++;
          }
          break;
        case '8':
        case KEY_UP: {
          if(target_y < CHUNK_SIZE * 3 - 1) {
            target_y++;
          }
          break;
        }
        case '9': {
          if(target_x < CHUNK_SIZE * 3 - 1 && target_y < CHUNK_SIZE * 3 - 1) {
            target_x++;
            target_y++;
          }
          break;
        }
        case '6':
        case KEY_RIGHT:
          if(target_x < CHUNK_SIZE * 3 - 1) {
            target_x++;
          }
          break;
        case '3':
          if(target_x < CHUNK_SIZE * 3 - 1 && target_y > 0) {
            target_x++;
            target_y--;
          }
          break;
        case '2':
        case KEY_DOWN: {
          if(target_y > 0) {
            target_y--;
          }
          break;
        }
        case '1':
          if(target_x > 0 && target_y > 0) {
            target_x--;
            target_y--;
          }
          break;
        case '\n': {
          done = true;
          orientation = MapUtil::getOrientationToTarget(player_x, player_y, target_x + region->id.x, target_y + region->id.y);
          break;
        }
        case ' ': {
          done = true;
          target_x = player_x;
          target_y = player_y;
          delete wch_old;
          return false;
        }
        default:
          ;
      }
      if(std::max(abs(target_x - player_x), abs(target_y - player_y)) > range) {
        target_x = previous_x;
        target_y = previous_y;
      }
      wclear(targetScreen);
      box(targetScreen, ACS_VLINE, ACS_HLINE);
      wrefresh(targetScreen);
      std::string to_print = "x= " + std::to_string(target_x + region->id.x) + " y= " + std::to_string(target_y + region->id.y);
      mvwprintw(targetScreen, lines2 / 2, cols2 / 2 - to_print.length() / 2, to_print.c_str());
      wrefresh(targetScreen);
      for(CharacterDisplay * character : display->characters) {
        if((int) std::floor(character->x) == target_x + region->id.x && (int) std::floor(character->y) == target_y + region->id.y) {
          target_id = character->id;
          displayTarget(character, targetScreen, t);
          break;
        }
      }
      wmove(mapScreen, lines / 2 - CHUNK_SIZE * 3 / 2 + CHUNK_SIZE * 3 - 1 - target_y, target_x + cols / 2 - CHUNK_SIZE * 3 / 2);
      win_wch(mapScreen, wch_old);
      wattron(mapScreen, COLOR_PAIR(BACK_RED));
      wprintw(mapScreen, "X");
      wattroff(mapScreen, COLOR_PAIR(BACK_RED));
      wrefresh(mapScreen);
    }
    target_x += region->id.x;
    target_y += region->id.y;
    delete wch_old;
    return true;
  }

  std::string selectItem(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, int & object_type, int & object_id, Translator * t) {
      std::string result = "";
      bool done = false;
      int cursorX = 0;
      int cursorY = 0;
      int lines = 0;
      int cols = 0;
      getmaxyx(displayScreen, lines, cols);
      while(!done) {
        WINDOW * tempScreen = nullptr;
        wclear(displayScreen);
        wclear(targetScreen);
        box(displayScreen, ACS_VLINE, ACS_HLINE);
        box(targetScreen, ACS_VLINE, ACS_HLINE);
        std::string to_print = t->getStandardName("INVENTORY");
        mvwprintw(displayScreen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
        int currentX = 0;
        int currentY = 0;
        int offset = 0;
        int sizeX = 0;
        int maxY = 0;
        int color = WHITE;
        for (ItemSlot * slot : player->getGear()->getItems()) {
          if(currentY >= lines - 4) {
            offset += sizeX + 3;
            maxY = currentY - 1;
            currentY = 0;
            currentX++;
          }
          if(cursorX == currentX && cursorY == currentY) {
            color = BLUE;
          }
          else {
            color = WHITE;
          }
          std::string to_print = t->getItemName(slot->item->name);
          if(color == BLUE) {
            result = slot->item->name;
            object_id = slot->item->id;
            object_type = OBJECT_ITEM;
            tempScreen = displayItem(slot->item, targetScreen, t);
          }
          sizeX = std::max(sizeX, (int) to_print.length());
          wattron(displayScreen, COLOR_PAIR(color));
          mvwprintw(displayScreen, 3 + currentY++, offset + 1, to_print.c_str());
          wattroff(displayScreen, COLOR_PAIR(color));
        }
        maxY = std::max(--currentY, maxY);

        wrefresh(displayScreen);
        wrefresh(targetScreen);
        flushinp();
        int keyPressed = getch();
        switch(keyPressed) {
          case '4':
          case KEY_LEFT:
            if(cursorX > 0) {
              cursorX--;
            }
            break;
          case '8':
          case KEY_UP: {
            if(cursorY > 0) {
              cursorY--;
            }
            break;
          }
          case '6':
          case KEY_RIGHT:
            if(cursorX < currentX) {
              cursorX++;
            }
            break;
          case '2':
          case KEY_DOWN: {
            if(cursorY < maxY) {
              cursorY++;
            }
            break;
          }
          case '\n':
            done = true;
            break;
          case ' ':
            done = true;
            result = "";
          default:
            ;
        }
        if(tempScreen != nullptr) {
          wclear(tempScreen);
          delwin(tempScreen);
        }
      }
      wclear(displayScreen);
      wclear(targetScreen);
      box(displayScreen, ACS_VLINE, ACS_HLINE);
      box(targetScreen, ACS_VLINE, ACS_HLINE);
      wrefresh(displayScreen);
      wrefresh(targetScreen);
      return result;
  }

  std::string selectAmmo(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, Translator * t) {
    std::string result = "";
    bool done = false;
    int cursorX = 0;
    int cursorY = 0;
    int lines = 0;
    int cols = 0;
    getmaxyx(displayScreen, lines, cols);
    while(!done) {
      WINDOW * tempScreen = nullptr;
      wclear(displayScreen);
      wclear(targetScreen);
      box(displayScreen, ACS_VLINE, ACS_HLINE);
      box(targetScreen, ACS_VLINE, ACS_HLINE);
      std::string to_print = t->getStandardName("AMMUNITION");
      mvwprintw(displayScreen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
      int currentX = 0;
      int currentY = 0;
      int offset = 0;
      int sizeX = 0;
      int maxY = 0;
      int color = WHITE;
      for (ItemSlot * slot : player->getGear()->getItems()) {
        if(slot->item->type == ITEM_AMMUNITION) {
          AmmunitionItem * ammo = (AmmunitionItem *) slot->item;
          if(currentY >= lines - 4) {
            offset += sizeX + 3;
            maxY = currentY - 1;
            currentY = 0;
            currentX++;
          }
          if(cursorX == currentX && cursorY == currentY) {
            color = BLUE;
          }
          else {
            color = WHITE;
          }
          std::string to_print = t->getProjectileName(ammo->getProjectile()->name);
          if(color == BLUE) {
            result = ammo->getProjectile()->name;
            tempScreen = displayItem(ammo, targetScreen, t);
          }
          sizeX = std::max(sizeX, (int) to_print.length());
          wattron(displayScreen, COLOR_PAIR(color));
          mvwprintw(displayScreen, 3 + currentY++, offset + 1, to_print.c_str());
          wattroff(displayScreen, COLOR_PAIR(color));
        }
      }
      maxY = std::max(--currentY, maxY);

      wrefresh(displayScreen);
      wrefresh(targetScreen);
      flushinp();
      int keyPressed = getch();
      switch(keyPressed) {
        case '4':
        case KEY_LEFT:
          if(cursorX > 0) {
            cursorX--;
          }
          break;
        case '8':
        case KEY_UP: {
          if(cursorY > 0) {
            cursorY--;
          }
          break;
        }
        case '6':
        case KEY_RIGHT:
          if(cursorX < currentX) {
            cursorX++;
          }
          break;
        case '2':
        case KEY_DOWN: {
          if(cursorY < maxY) {
            cursorY++;
          }
          break;
        }
        case '\n':
          done = true;
          break;
        case ' ':
          done = true;
          result = "";
        default:
          ;
      }
      if(tempScreen != nullptr) {
        wclear(tempScreen);
        delwin(tempScreen);
      }
    }
    wclear(displayScreen);
    wclear(targetScreen);
    box(displayScreen, ACS_VLINE, ACS_HLINE);
    box(targetScreen, ACS_VLINE, ACS_HLINE);
    wrefresh(displayScreen);
    wrefresh(targetScreen);
    return result;
  }
}

void Display::sendAction(Link * link, long id, int type, void * arg1, void * arg2, int overcharge_power, int overcharge_duration, int overcharge_range) {
  std::vector<long> ids = std::vector<long>();
  ids.push_back(id);
  std::vector<std::vector<int>> types = std::vector<std::vector<int>>(); 
  std::vector<int> types_args = std::vector<int>();
  types_args.push_back(type);
  types.push_back(types_args);
  std::vector<std::vector<void *>> args1 = std::vector<std::vector<void *>>(); 
  std::vector<void *> args1_args = std::vector<void *>();
  args1_args.push_back(arg1);
  args1.push_back(args1_args);
  std::vector<std::vector<void *>> args2 = std::vector<std::vector<void *>>(); 
  std::vector<void *> args2_args = std::vector<void *>();
  args2_args.push_back(arg2);
  args2.push_back(args2_args);
  std::vector<std::vector<int>> overcharge_powers = std::vector<std::vector<int>>(); 
  std::vector<int> overcharge_powers_args = std::vector<int>();
  overcharge_powers_args.push_back(overcharge_power);
  overcharge_powers.push_back(overcharge_powers_args);
  std::vector<std::vector<int>> overcharge_durations = std::vector<std::vector<int>>(); 
  std::vector<int> overcharge_durations_args = std::vector<int>();
  overcharge_durations_args.push_back(overcharge_duration);
  overcharge_durations.push_back(overcharge_durations_args);
  std::vector<std::vector<int>> overcharge_ranges = std::vector<std::vector<int>>(); 
  std::vector<int> overcharge_ranges_args = std::vector<int>();
  overcharge_ranges_args.push_back(overcharge_range);
  overcharge_ranges.push_back(overcharge_ranges_args);
  link->sendActions(ids, types, args1, args2, overcharge_powers, overcharge_durations, overcharge_ranges);
}
