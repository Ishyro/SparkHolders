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

#include "clients/EnglishKeyHolder.h"

#include "clients/terminal/Display.h"

#include "util/MathUtil.h"
#include "util/String.h"

namespace Display {

  void print(WINDOW* screen, int32_t offsetY, int32_t offsetX, std::string to_print) {
    uint64_t lines = 0;
    uint64_t cols = 0;
    getmaxyx(screen, lines, cols);
    std::istringstream stream(to_print);
    std::string line = "";
    std::string word;
    uint64_t currentLength = 0;
    while(getline(stream, word, ' ') && word != "") {
      if(line == "") {
        line = word;
        currentLength = word.length();
        continue;
      }
      const uint64_t begin = word.find('\n');
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

  void displayMap(StateDisplay * display, Adventure * adventure, Character * player, WINDOW * screen, Link * link) {
    int32_t lines = 0;
    int32_t cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    box(screen, ACS_VLINE, ACS_HLINE);
    Region * region = player->getRegion();
    //mvwprintw(screen, 1, cols / 2 - link->getMapName(map->name).length() / 2, link->getMapName(map->name).c_str());
    for(int32_t y = region->id.y + CHUNK_SIZE * 3 - 1; y >= region->id.y; y--) {
      for(int32_t x = region->id.x; x < region->id.x + CHUNK_SIZE * 3; x++) {
        std::string to_print = "·";
        Block * block = region->getBlock(MathUtil::Vector3i(x, y, -1));
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
      char ch = link->getEnglishFromKey(character->name).at(0);
      to_print = ch;
      int32_t color;
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
      mvwprintw(screen, lines / 2 - CHUNK_SIZE * 3 / 2 + CHUNK_SIZE * 3 - 1 - (int32_t) std::floor(character->y - region->id.y), (int32_t) std::floor(character->x - region->id.x) + cols / 2 - CHUNK_SIZE * 3 / 2, to_print.c_str());
      wattroff(screen, COLOR_PAIR(color));
    }
    for(ProjectileDisplay * projectile : display->projectiles) {
      std::string to_print = "";
      switch((int32_t) std::floor(projectile->orientation_z)) {
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
      mvwprintw(screen, lines / 2 - CHUNK_SIZE * 3 / 2 + CHUNK_SIZE * 3 - 1 - (int32_t) std::floor(projectile->y - region->id.y), (int32_t) std::floor(projectile->x - region->id.x) + cols / 2 - CHUNK_SIZE * 3 / 2, to_print.c_str());
      wattroff(screen, COLOR_PAIR(RED));
    }
    for(Loot * loot : display->loots) {
      std::string to_print = "*";
      wattron(screen, COLOR_PAIR(YELLOW));
      mvwprintw(screen, lines / 2 - CHUNK_SIZE * 3 / 2 + CHUNK_SIZE * 3 - 1 - (int32_t) std::floor(loot->y - region->id.y), (int32_t) std::floor(loot->x - region->id.x) + cols / 2 - CHUNK_SIZE * 3 / 2, to_print.c_str());
      wattroff(screen, COLOR_PAIR(YELLOW));
    }
    std::string to_print = std::to_string(player->getOrientation().z);
    // player position
    mvwprintw(screen, lines - 4, 1, to_print.c_str());
    to_print = std::string("X: ") + std::to_string(player->getCoord().x);
    mvwprintw(screen, lines - 3, 1, to_print.c_str());
    to_print = std::string("Y: ") + std::to_string(player->getCoord().y);
    mvwprintw(screen, lines - 2, 1, to_print.c_str());
    // time
    to_print = adventure->getTime().to_string_clock();
    mvwprintw(screen, lines - 2, cols - 2 - to_print.length(), to_print.c_str());

    wrefresh(screen);
  }

  void displayStats(Character * player, WINDOW * screen, Link * link) {
    int32_t lines = 0;
    int32_t cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    box(screen, ACS_VLINE, ACS_HLINE);
    std::string to_print = player->name + ", " + link->getEnglishFromKey(player->getMainClass()->name);
    mvwprintw(screen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
    mvwprintw(screen, 3, 1, (link->getEnglishFromKey("Hp") + std::string(": ") + std::to_string(player->getHp()) + std::string(" / ") + std::to_string(player->getMaxHp())).c_str());
    mvwprintw(screen, 4, 1, (link->getEnglishFromKey("Mana") + std::string(": ") + std::to_string(player->getMana()) + std::string(" / ") + std::to_string(player->getMaxMana())).c_str());
    mvwprintw(screen, 5, 1, (link->getEnglishFromKey("Stamina") + std::string(": ") + std::to_string(player->getStamina()) + std::string(" / ") + std::string("100")).c_str());
    mvwprintw(screen, 6, 1, (link->getEnglishFromKey("Hunger") + std::string(": ") + std::to_string(player->getHunger()) + std::string(" / ") + std::string("100")).c_str());
    mvwprintw(screen, 7, 1, (link->getEnglishFromKey("Soulburn") + std::string(": ") + std::to_string(player->getCurrentSoulBurn()) + std::string(" / ") + std::to_string(player->getSoulBurnThreshold())).c_str());
    mvwprintw(screen, 8, 1, (link->getEnglishFromKey("Flow") + std::string(": ") + std::to_string(player->getFlow())).c_str());
    mvwprintw(screen, 9, 1, (link->getEnglishFromKey("Shield") + std::string(": ") + std::to_string(player->getShield())).c_str());
    mvwprintw(screen, 10, 1, (link->getEnglishFromKey("Vision Range") + std::string(": ") + std::to_string(player->getVisionRange())).c_str());
    mvwprintw(screen, 11, 1, (link->getEnglishFromKey("Vision Power") + std::string(": ") + std::to_string(player->getVisionPower())).c_str());
    mvwprintw(screen, 12, 1, (link->getEnglishFromKey("Detection Range") + std::string(": ") + std::to_string(player->getDetectionRange())).c_str());
    mvwprintw(screen, 13, 1, (link->getEnglishFromKey("Level") + std::string(": ") + std::to_string(player->getLevel())).c_str());
    mvwprintw(screen, 14, 1, (link->getEnglishFromKey("Experience") + std::string(": ") + std::to_string(player->getXP()) + std::string(" / ") + std::to_string(1000 * player->getLevel() * player->getLevel())).c_str());
    wrefresh(screen);
  }

  WINDOW * displaySkill(Skill * skill, Character * player, WINDOW * screen, float overcharge, Link * link) {
    int32_t lines = 0;
    int32_t cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    wattron(screen, COLOR_PAIR(WHITE));
    box(screen, ACS_VLINE, ACS_HLINE);
    std::string to_print = link->getEnglishFromKey(skill->name);
    mvwprintw(screen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
    mvwprintw(screen, 3, 1, (link->getEnglishFromKey("Targeting") + std::string(": ") + link->getEnglishFromKey(std::string("target_type_") + std::to_string(skill->target_type))).c_str());
    mvwprintw(screen, 4, 1, (link->getEnglishFromKey("Range") + std::string(": ") + std::to_string(skill->range * overcharge)).c_str());
    mvwprintw(screen, 5, 1, (link->getEnglishFromKey("Power") + std::string(": ") + std::to_string(skill->getPower() * overcharge)).c_str());
    mvwprintw(screen, 6, 1, (link->getEnglishFromKey("Mana cost") + std::string(": ") + std::to_string(skill->getManaCost())).c_str());
    mvwprintw(screen, 7, 1, (link->getEnglishFromKey("Time") + std::string(": ") + std::to_string(skill->time)).c_str());
    int32_t i = 8;
    for(PseudoSkill * pseudoSkill : skill->skills) {
      switch(pseudoSkill->skill_type) {
        case SKILL_PROJECTILE: {
          Projectile * projectile = ((ProjectileSkill *) pseudoSkill)->getProjectile();
          mvwprintw(screen, i++, 1, (link->getEnglishFromKey("Speed") + std::string(": ") + std::to_string(projectile->getSpeed() * overcharge)).c_str());
          mvwprintw(screen, i++, 1, (link->getEnglishFromKey("Falloff timer") + std::string(": ") + std::to_string(projectile->getFalloffTimer() * overcharge)).c_str());
          mvwprintw(screen, i++, 1, (link->getEnglishFromKey("Area") + std::string(": ") + std::to_string(projectile->getArea() * overcharge)).c_str());
          mvwprintw(screen, i++, 1, (link->getEnglishFromKey("Waste per tick") + std::string(": ") + std::to_string(projectile->getWastePerTick() / overcharge)).c_str());
          mvwprintw(screen, i++, 1, (link->getEnglishFromKey("Waste per area") + std::string(": ") + std::to_string(projectile->getWastePerArea() / overcharge)).c_str());
          mvwprintw(screen, i++, 1, (link->getEnglishFromKey("Waste per hit") + std::string(": ") + std::to_string(projectile->getWastePerHit())).c_str());
        }
        default:
          ;
      }
    }
    int32_t damage_SLASH = skill->getDamageFromType(DAMAGE_SLASH, player, overcharge);
    if(damage_SLASH != 0) {
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("SLASH") + std::string(": ") + std::to_string(damage_SLASH)).c_str());
    }
    int32_t damage_PUNCTURE = skill->getDamageFromType(DAMAGE_PUNCTURE, player, overcharge);
    if(damage_PUNCTURE != 0) {
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("PUNCTURE") + std::string(": ") + std::to_string(damage_PUNCTURE)).c_str());
    }
    int32_t damage_BLUNT = skill->getDamageFromType(DAMAGE_BLUNT, player, overcharge);
    if(damage_BLUNT != 0) {
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("BLUNT") + std::string(": ") + std::to_string(damage_BLUNT)).c_str());
    }
    int32_t damage_FIRE = skill->getDamageFromType(DAMAGE_FIRE, player, overcharge);
    if(damage_FIRE != 0) {
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("FIRE") + std::string(": ") + std::to_string(damage_FIRE)).c_str());
    }
    int32_t damage_LIGHTNING = skill->getDamageFromType(DAMAGE_LIGHTNING, player, overcharge);
    if(damage_LIGHTNING != 0) {
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("DAMAGE_LIGHTNING") + std::string(": ") + std::to_string(damage_LIGHTNING)).c_str());
    }
    int32_t damage_COLD = skill->getDamageFromType(DAMAGE_FROST, player, overcharge);
    if(damage_COLD != 0) {
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("FROST") + std::string(": ") + std::to_string(damage_COLD)).c_str());
    }
    int32_t damage_POISON = skill->getDamageFromType(DAMAGE_POISON, player, overcharge);
    if(damage_POISON != 0) {
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("POISON") + std::string(": ") + std::to_string(damage_POISON)).c_str());
    }
    int32_t damage_NEUTRAL = skill->getDamageFromType(DAMAGE_ACID, player, overcharge);
    if(damage_NEUTRAL != 0) {
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("NEUTRAL") + std::string(": ") + std::to_string(damage_NEUTRAL)).c_str());
    }
    int32_t damage_TRUE = skill->getDamageFromType(DAMAGE_SOLAR, player, overcharge);
    if(damage_TRUE != 0) {
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("TRUE") + std::string(": ") + std::to_string(damage_TRUE)).c_str());
    }
    int32_t damage_SOUL = skill->getDamageFromType(DAMAGE_AETHER, player, overcharge);
    if(damage_SOUL != 0) {
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("SOUL") + std::string(": ") + std::to_string(damage_SOUL)).c_str());
    }
    wattroff(screen, COLOR_PAIR(WHITE));
    // should be constants
    int32_t separator = (float) LINES * 3 / 5;
    float ratio = 2.25;
    WINDOW * descScreen = subwin(screen, lines - i - 2, cols - 2, separator + i + 1, std::ceil((float) COLS - ratio * (float) (LINES - separator)) + 1);
    wattron(descScreen, COLOR_PAIR(WHITE));
    print(descScreen, 0, 0, (link->getEnglishFromKey(skill->name)));
    wattroff(descScreen, COLOR_PAIR(WHITE));
    wrefresh(descScreen);
    wrefresh(screen);
    return descScreen;
  }

  void displayTarget(CharacterDisplay * target, WINDOW * screen, Link * link) {
    int32_t lines = 0;
    int32_t cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    box(screen, ACS_VLINE, ACS_HLINE);
    std::string to_print = target->name + ", " + target->team;
    mvwprintw(screen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
    mvwprintw(screen, 3, 1, (link->getEnglishFromKey("Hp") + std::string(": ") + std::to_string(target->hp) + std::string(" / ") + std::to_string(target->maxHp)).c_str());
    mvwprintw(screen, 4, 1, (link->getEnglishFromKey("Mana") + std::string(": ") + std::to_string(target->mana) + std::string(" / ") + std::to_string(target->maxMana)).c_str());
    mvwprintw(screen, 5, 1, (link->getEnglishFromKey("Soulburn") + std::string(": ") + std::to_string(target->soulBurn) + std::string(" / ") + std::to_string(target->soulBurnTreshold)).c_str());
    mvwprintw(screen, 6, 1, (link->getEnglishFromKey("Flow") + std::string(": ") + std::to_string(target->flow)).c_str());
    mvwprintw(screen, 7, 1, (link->getEnglishFromKey("Shield") + std::string(": ") + std::to_string(target->shield)).c_str());

    mvwprintw(screen, 7, 1, link->getEnglishFromKey("Damages and Reductions").c_str());
    mvwprintw(screen, 8, 1, (link->getEnglishFromKey("Shield") + std::string(": ") + std::to_string(target->shield)).c_str());
    mvwprintw(screen, 9, 1, (link->getEnglishFromKey("SLASH") + std::string(": ") + std::to_string(target->damages[DAMAGE_SLASH]) + std::string(" / ") + std::to_string(target->damages[DAMAGE_SLASH])).c_str());
    mvwprintw(screen, 10, 1, (link->getEnglishFromKey("PUNCTURE") + std::string(": ") + std::to_string(target->damages[DAMAGE_PUNCTURE]) + std::string(" / ") + std::to_string(target->damages[DAMAGE_PUNCTURE])).c_str());
    mvwprintw(screen, 11, 1, (link->getEnglishFromKey("BLUNT") + std::string(": ") + std::to_string(target->damages[DAMAGE_BLUNT]) + std::string(" / ") + std::to_string(target->damages[DAMAGE_BLUNT])).c_str());
    mvwprintw(screen, 12, 1, (link->getEnglishFromKey("FIRE") + std::string(": ") + std::to_string(target->damages[DAMAGE_FIRE]) + std::string(" / ") + std::to_string(target->damages[DAMAGE_FIRE])).c_str());
    mvwprintw(screen, 13, 1, (link->getEnglishFromKey("LIGHTNING") + std::string(": ") + std::to_string(target->damages[DAMAGE_LIGHTNING]) + std::string(" / ") + std::to_string(target->damages[DAMAGE_LIGHTNING])).c_str());
    mvwprintw(screen, 14, 1, (link->getEnglishFromKey("COLD") + std::string(": ") + std::to_string(target->damages[DAMAGE_FROST]) + std::string(" / ") + std::to_string(target->damages[DAMAGE_FROST])).c_str());
    mvwprintw(screen, 15, 1, (link->getEnglishFromKey("POISON") + std::string(": ") + std::to_string(target->damages[DAMAGE_POISON]) + std::string(" / ") + std::to_string(target->damages[DAMAGE_POISON])).c_str());
    mvwprintw(screen, 16, 1, (link->getEnglishFromKey("NEUTRAL") + std::string(": ") + std::to_string(target->damages[DAMAGE_ACID])).c_str());
    mvwprintw(screen, 17, 1, (link->getEnglishFromKey("TRUE") + std::string(": ") + std::to_string(target->damages[DAMAGE_SOLAR])).c_str());
    mvwprintw(screen, 18, 1, (link->getEnglishFromKey("SOUL") + std::string(": ") + std::to_string(target->damages[DAMAGE_AETHER])).c_str());
    wrefresh(screen);
  }

  WINDOW * displayItem(Item * item, WINDOW * screen, Link * link) {
    int32_t lines = 0;
    int32_t cols = 0;
    getmaxyx(screen, lines, cols);
    wattron(screen, COLOR_PAIR(WHITE));
    std::string to_print = link->getEnglishFromKey(item->name);
    mvwprintw(screen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
    int32_t i = 3;
    mvwprintw(screen, i++, 1, (link->getEnglishFromKey("Type") + std::string(": ") + link->getEnglishFromKey(std::string("item_type_") + std::to_string(item->type))).c_str());
    mvwprintw(screen, i++, 1, (link->getEnglishFromKey("Value") + std::string(": ") + std::to_string(item->gold_value)).c_str());
    if(item->consumable) {
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("Effects")).c_str());
      for(Effect * effect : item->effects) {
        mvwprintw(screen, i++, 5, (link->getEnglishFromKey(effect->name)).c_str());
      }
    }
    if(item->type == ITEM_AMMUNITION || item->type == ITEM_SERIAL) {
      SerialItem * serial = (SerialItem *) item;
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("Number") + std::string(": ") + std::to_string(serial->getNumber())).c_str());
    }
    if(item->type == ITEM_ARMOR) {
      ArmorItem * armor = (ArmorItem *) item;
      if(armor->getDamageReductionFromType(DAMAGE_SLASH) != 0.F)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("SLASH") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_SLASH))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_PUNCTURE) != 0.F)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("PUNCTURE") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_PUNCTURE))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_BLUNT) != 0.F)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("BLUNT") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_BLUNT))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_FIRE) != 0.F)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("FIRE") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_FIRE))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_LIGHTNING) != 0.F)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("LIGHTNING") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_LIGHTNING))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_FROST) != 0.F)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("FROST") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_FROST))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_POISON) != 0.F)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("POISON") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_POISON))).c_str());
      if(armor->getDamageReductionFromType(DAMAGE_MIND) != 0.F)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("MIND") + std::string(": ") + std::to_string(armor->getDamageReductionFromType(DAMAGE_MIND))).c_str());
    }
    if(item->type == ITEM_WEAPON) {
      WeaponItem * weapon = (WeaponItem *) item;
      mvwprintw(screen, i++, 1, (link->getEnglishFromKey("Range") + std::string(": ") + std::to_string(weapon->range.y)).c_str());
      if(weapon->getDamageFromType(DAMAGE_SLASH) > 0)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("SLASH") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_SLASH))).c_str());
      if(weapon->getDamageFromType(DAMAGE_PUNCTURE) > 0)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("PUNCTURE") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_PUNCTURE))).c_str());
      if(weapon->getDamageFromType(DAMAGE_BLUNT) > 0)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("BLUNT") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_BLUNT))).c_str());
      if(weapon->getDamageFromType(DAMAGE_FIRE) > 0)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("FIRE") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_FIRE))).c_str());
      if(weapon->getDamageFromType(DAMAGE_LIGHTNING) > 0)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("LIGHTNING") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_LIGHTNING))).c_str());
      if(weapon->getDamageFromType(DAMAGE_FROST) > 0)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("FROST") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_FROST))).c_str());
      if(weapon->getDamageFromType(DAMAGE_POISON) > 0)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("POISON") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_POISON))).c_str());
      if(weapon->getDamageFromType(DAMAGE_ACID) > 0)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("ACID") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_ACID))).c_str());
      if(weapon->getDamageFromType(DAMAGE_MIND) > 0)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("MIND") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_MIND))).c_str());
      if(weapon->getDamageFromType(DAMAGE_SOLAR) > 0)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("TRUE") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_SOLAR))).c_str());
      if(weapon->getDamageFromType(DAMAGE_AETHER) > 0)
        mvwprintw(screen, i++, 1, (link->getEnglishFromKey("SOUL") + std::string(": ") + std::to_string(weapon->getDamageFromType(DAMAGE_AETHER))).c_str());
    }
    wattroff(screen, COLOR_PAIR(WHITE));
    // should be constants
    int32_t separator = (float) LINES * 3 / 5;
    float ratio = 2.25;
    WINDOW * descScreen = subwin(screen, lines - 3 - 1, cols - 20 - 1, separator + 3, std::ceil((float) COLS - ratio * (float) (LINES - separator)) + 20);
    wattron(descScreen, COLOR_PAIR(WHITE));
    print(descScreen, 0, 0, (link->getEnglishFromKey(item->name)));
    wattroff(descScreen, COLOR_PAIR(WHITE));
    wrefresh(descScreen);
    wrefresh(screen);
    return descScreen;
  }

  void displayCommands(WINDOW * screen, Link * link) {
    int32_t lines = 0;
    int32_t cols = 0;
    getmaxyx(screen, lines, cols);
    wclear(screen);
    box(screen, ACS_VLINE, ACS_HLINE);
    std::string to_print = link->getEnglishFromKey("CONTROLS");
    mvwprintw(screen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
    mvwprintw(screen, 3, 1, (link->getEnglishFromKey("NUMPAD") + std::string(": ") + link->getEnglishFromKey("MOVE") + std::string(" / ") +  link->getEnglishFromKey("ATTACK")).c_str());
    mvwprintw(screen, 4, 1, (link->getEnglishFromKey("5") + std::string(": ") + link->getEnglishFromKey("REST")).c_str());
    mvwprintw(screen, 5, 1, (link->getEnglishFromKey("<") + std::string(": ") + link->getEnglishFromKey("OPEN")).c_str());
    mvwprintw(screen, 6, 1, (link->getEnglishFromKey("I") + std::string(": ") + link->getEnglishFromKey("INVENTORY")).c_str());
    mvwprintw(screen, 7, 1, (link->getEnglishFromKey("R") + std::string(": ") + link->getEnglishFromKey("RELOAD")).c_str());
    mvwprintw(screen, 8, 1, (link->getEnglishFromKey("X") + std::string(": ") + link->getEnglishFromKey("USE SKILL")).c_str());
    mvwprintw(screen, 9, 1, (link->getEnglishFromKey("C") + std::string(": ") + link->getEnglishFromKey("SHOOT")).c_str());
    mvwprintw(screen, 10, 1, (link->getEnglishFromKey("SPACEBAR") + std::string(": ") + link->getEnglishFromKey("LOOT")).c_str());
    wrefresh(screen);
  }

  WINDOW * displayAttributes(Attributes * attributes, int32_t place, int32_t color, WINDOW * screen, int32_t offsetY, int32_t offsetX, Link * link) {
    WINDOW * attributesScreen = subwin(screen, ATTRIBUTES_HEIGHT, ATTRIBUTES_WIDTH, offsetY + 2 + ATTRIBUTES_HEIGHT * place, offsetX + 1);
    wattron(attributesScreen, COLOR_PAIR(color));
    box(attributesScreen, ACS_VLINE, ACS_HLINE);
    mvwprintw(attributesScreen, 1, ATTRIBUTES_WIDTH / 2 - link->getEnglishFromKey(attributes->name).length() / 2, link->getEnglishFromKey(attributes->name).c_str());
    mvwprintw(attributesScreen, 3, 1, (link->getEnglishFromKey("Hp") + std::string(": ") + std::to_string(attributes->baseHp)).c_str());
    mvwprintw(attributesScreen, 4, 1, (link->getEnglishFromKey("Mana") + std::string(": ") + std::to_string(attributes->baseMana)).c_str());
    mvwprintw(attributesScreen, 5, 1, (link->getEnglishFromKey("Shield") + std::string(": ") + std::to_string(attributes->baseShield)).c_str());
    mvwprintw(attributesScreen, 6, 1, (link->getEnglishFromKey("Damage") + std::string(": ") + std::to_string(attributes->baseDamageMult) + std::string("%")).c_str());
    mvwprintw(attributesScreen, 7, 1, (link->getEnglishFromKey("Soulburn") + std::string(": ") + std::to_string(attributes->baseSoulBurn)).c_str());
    mvwprintw(attributesScreen, 8, 1, (link->getEnglishFromKey("Flow") + std::string(": ") + std::to_string(attributes->baseFlow)).c_str());
    mvwprintw(attributesScreen, 9, 1, (link->getEnglishFromKey("Vision Range") + std::string(": ") + std::to_string(attributes->baseVisionRange)).c_str());
    mvwprintw(attributesScreen, 10, 1, (link->getEnglishFromKey("Vision Power") + std::string(": ") + std::to_string(attributes->baseVisionPower)).c_str());
    mvwprintw(attributesScreen, 11, 1, (link->getEnglishFromKey("Detection Range") + std::string(": ") + std::to_string(attributes->baseDetectionRange)).c_str());
    wattroff(attributesScreen, COLOR_PAIR(color));
    return attributesScreen;
  }

  WINDOW * displayWay(Way * way, int32_t place, int32_t color, WINDOW * screen, int32_t offsetY, int32_t offsetX, Link * link) {
    WINDOW * wayScreen = subwin(screen, WAY_HEIGHT, WAY_WIDTH, offsetY + 2 + WAY_HEIGHT * place, offsetX + 1);
    wattron(wayScreen, COLOR_PAIR(color));
    box(wayScreen, ACS_VLINE, ACS_HLINE);
    mvwprintw(wayScreen, 1, WAY_WIDTH / 2 - link->getEnglishFromKey(way->name).length() / 2, link->getEnglishFromKey(way->name).c_str());
    mvwprintw(wayScreen, 3, 1, (std::string("+") + link->getEnglishFromKey("Hp") + std::string(": ") + std::to_string(way->hpIncr)).c_str());
    mvwprintw(wayScreen, 4, 1, (std::string("+") + link->getEnglishFromKey("Mana") + std::string(": ") + std::to_string(way->manaIncr)).c_str());
    mvwprintw(wayScreen, 5, 1, (std::string("+") + link->getEnglishFromKey("Shield") + std::string(": ") + std::to_string(way->shieldIncr)).c_str());
    mvwprintw(wayScreen, 6, 1, (std::string("+") + link->getEnglishFromKey("Damage") + std::string(": ") + std::to_string(way->damageMultIncr) + std::string("%")).c_str());
    mvwprintw(wayScreen, 7, 1, (std::string("+") + link->getEnglishFromKey("Soulburn") + std::string(": ") + std::to_string(way->soulBurnIncr)).c_str());
    mvwprintw(wayScreen, 8, 1, (std::string("+") + link->getEnglishFromKey("Flow") + std::string(": ") + std::to_string(way->flowIncr)).c_str());
    wattroff(wayScreen, COLOR_PAIR(color));
    return wayScreen;
  }

  WINDOW * displayCharacter(Attributes * attributes, Way * race, Way * origin, Way * culture, Way * religion, Way * profession, int32_t color, WINDOW * screen, int32_t sizeX, int32_t offsetY, int32_t offsetX, Link * link) {
    int32_t lines = 0;
    int32_t cols = 0;
    getmaxyx(screen, lines, cols);
    WINDOW * characterScreen = subwin(screen, ATTRIBUTES_HEIGHT - 2, sizeX, offsetY, offsetX);
    int32_t hpIncr = 0;
    int32_t manaIncr = 0;
    int32_t shieldIncr = 0;
    int32_t damageMultIncr = 0;
    int32_t soulBurnIncr = 0;
    int32_t flowIncr = 0;
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
    int32_t space = cols / 2;
    if(attributes != nullptr) {
      mvwprintw(characterScreen, 1, 1, (link->getEnglishFromKey("Hp") + std::string(": ") + std::to_string(attributes->baseHp)).c_str());
      mvwprintw(characterScreen, 2, 1, (link->getEnglishFromKey("Mana") + std::string(": ") + std::to_string(attributes->baseMana)).c_str());
      mvwprintw(characterScreen, 3, 1, (link->getEnglishFromKey("Shield") + std::string(": ") + std::to_string(attributes->shieldIncr)).c_str());
      mvwprintw(characterScreen, 4, 1, (link->getEnglishFromKey("Damage") + std::string(": ") + std::to_string(attributes->baseDamageMult) + std::string("%")).c_str());
      mvwprintw(characterScreen, 5, 1, (link->getEnglishFromKey("Soulburn") + std::string(": ") + std::to_string(attributes->baseSoulBurn)).c_str());
      mvwprintw(characterScreen, 6, 1, (link->getEnglishFromKey("Flow") + std::string(": ") + std::to_string(attributes->baseFlow)).c_str());
      mvwprintw(characterScreen, 7, 1, (link->getEnglishFromKey("Vision Range") + std::string(": ") + std::to_string(attributes->baseVisionRange)).c_str());
      mvwprintw(characterScreen, 8, 1, (link->getEnglishFromKey("Vision Power") + std::string(": ") + std::to_string(attributes->baseVisionPower)).c_str());
      mvwprintw(characterScreen, 9, 1, (link->getEnglishFromKey("Detection Range") + std::string(": ") + std::to_string(attributes->baseDetectionRange)).c_str());
    }
    mvwprintw(characterScreen, 1, space, (std::string("+") + link->getEnglishFromKey("Hp") + std::string(": ") + std::to_string(hpIncr)).c_str());
    mvwprintw(characterScreen, 2, space, (std::string("+") + link->getEnglishFromKey("Mana") + std::string(": ") + std::to_string(manaIncr)).c_str());
    mvwprintw(characterScreen, 3, space, (std::string("+") + link->getEnglishFromKey("Shield") + std::string(": ") + std::to_string(shieldIncr)).c_str());
    mvwprintw(characterScreen, 4, space, (std::string("+") + link->getEnglishFromKey("Damage") + std::string(": ") + std::to_string(damageMultIncr)).c_str());
    mvwprintw(characterScreen, 5, space, (std::string("+") + link->getEnglishFromKey("Soulburn") + std::string(": ") + std::to_string(soulBurnIncr)).c_str());
    mvwprintw(characterScreen, 6, space, (std::string("+") + link->getEnglishFromKey("Flow") + std::string(": ") + std::to_string(flowIncr)).c_str());
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
    Link * link)
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
    int32_t cursorX = 0;
    int32_t cursorY = 0;
    clear();
    int32_t characterWidth = COLS - (ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 5) - 2;
    WINDOW * characterScreen = subwin(stdscr, LINES, characterWidth + 2, 0, 0);
    WINDOW * attributesScreen = subwin(stdscr, LINES, ATTRIBUTES_WIDTH + 2, 0, characterWidth + 2);
    WINDOW * raceScreen = subwin(stdscr, LINES, WAY_WIDTH + 2, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2);
    WINDOW * originScreen = subwin(stdscr, LINES, WAY_WIDTH + 2, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + WAY_WIDTH + 2);
    WINDOW * cultureScreen = subwin(stdscr, LINES, WAY_WIDTH + 2, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 2);
    WINDOW * religionScreen = subwin(stdscr, LINES, WAY_WIDTH + 2, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 3);
    WINDOW * professionScreen = subwin(stdscr, LINES, WAY_WIDTH + 2, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 4);
    int32_t lines = 0;
    int32_t cols = 0;
    getmaxyx(attributesScreen, lines, cols);
    int32_t numberAttributes = lines / ATTRIBUTES_HEIGHT;
    getmaxyx(raceScreen, lines, cols);
    int32_t numberWays = lines / WAY_HEIGHT;
    std::list<WINDOW *> screens = std::list<WINDOW *>();
    int32_t currentPannel = 0;
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
      int32_t attributesCount = 0;
      int32_t raceCount = 0;
      int32_t originCount = 0;
      int32_t cultureCount = 0;
      int32_t religionCount = 0;
      int32_t professionCount = 0;
      int32_t skip = 0;
      std::vector<Way *> availableRaces = std::vector<Way *>(races.size());
      std::vector<Way *> availableOrigins = std::vector<Way *>(origins.size());
      std::vector<Way *> availableCultures = std::vector<Way *>(cultures.size());
      std::vector<Way *> availableReligions = std::vector<Way *>(religions.size());
      std::vector<Way *> availableProfessions = std::vector<Way *>(professions.size());
      std::string to_print = link->getEnglishFromKey("CHARACTER");
      mvwprintw(characterScreen, 1, 1 + characterWidth / 2 - to_print.length() / 2, to_print.c_str());
      to_print = link->getEnglishFromKey("CLASS");
      mvwprintw(attributesScreen, 1, 1 + ATTRIBUTES_WIDTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = link->getEnglishFromKey("RACE");
      mvwprintw(raceScreen, 1, 1 + WAY_WIDTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = link->getEnglishFromKey("ORIGIN");
      mvwprintw(originScreen, 1, 1 + WAY_WIDTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = link->getEnglishFromKey("CULTURE");
      mvwprintw(cultureScreen, 1, 1 + WAY_WIDTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = link->getEnglishFromKey("RELIGION");
      mvwprintw(religionScreen, 1, 1 + WAY_WIDTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = link->getEnglishFromKey("PROFESSION");
      mvwprintw(professionScreen, 1, 1 + WAY_WIDTH / 2 - to_print.length() / 2, to_print.c_str());
      for(Attributes * attributes : startingAttributes) {
        if(skip++ < currentPannel * numberAttributes) {
          continue;
        }
        int32_t color = WHITE;
        if(selectedAttributes != nullptr && selectedAttributes->name == attributes->name) {
          color = GREEN;
        }
        if(cursorX == 1 && cursorY == attributesCount) {
          color = BLUE;
        }
        screens.push_back(displayAttributes(attributes, attributesCount++, color, attributesScreen, 0, characterWidth + 2, link));
      }
      wrefresh(attributesScreen);
      skip = 0;
      for(Way * way : races) {
        int32_t color = WHITE;
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
          screens.push_back(displayWay(way, raceCount, color, raceScreen, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2, link));
          availableRaces[raceCount++] = way;
        }
      }
      availableRaces.resize(raceCount);
      wrefresh(raceScreen);
      skip = 0;
      for(Way * way : origins) {
        int32_t color = WHITE;
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
          screens.push_back(displayWay(way, originCount, color, originScreen, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + WAY_WIDTH + 2, link));
          availableOrigins[originCount++] = way;
        }
      }
      availableOrigins.resize(originCount);
      wrefresh(originScreen);
      skip = 0;
      for(Way * way : cultures) {
        int32_t color = WHITE;
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
          screens.push_back(displayWay(way, cultureCount, color, cultureScreen, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 2, link));
          availableCultures[cultureCount++] = way;
        }
      }
      availableCultures.resize(cultureCount);
      wrefresh(cultureScreen);
      skip = 0;
      for(Way * way : religions) {
        int32_t color = WHITE;
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
          screens.push_back(displayWay(way, religionCount, color, religionScreen, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 3, link));
          availableReligions[religionCount++] = way;
        }
      }
      availableReligions.resize(religionCount);
      wrefresh(religionScreen);
      skip = 0;
      for(Way * way : professions) {
        int32_t color = WHITE;
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
          screens.push_back(displayWay(way, professionCount, color, professionScreen, 0, characterWidth + 2 + ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 4, link));
          availableProfessions[professionCount++] = way;
        }
      }
      availableProfessions.resize(professionCount);
      wrefresh(professionScreen);
      int32_t color = WHITE;
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
      mvwprintw(nameScreen, 1, 1, (link->getEnglishFromKey("Name") + std::string(": ") + characterName).c_str());
      wattroff(nameScreen, COLOR_PAIR(color));
      color = WHITE;
      if(cursorX == 0 && cursorY == 1) {
        color = BLUE;
      }
      screens.push_back(displayCharacter(selectedAttributes, selectedRace, selectedOrigin, selectedCulture, selectedReligion, selectedProfession, color, characterScreen, COLS - (ATTRIBUTES_WIDTH + 2 + (WAY_WIDTH + 2) * 5) - 2, 5, 1, link));
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
            case 0: to_print = link->getEnglishFromKey("Name"); break;
            case 1: to_print = link->getEnglishFromKey("CHARACTER"); break;
            case 2: to_print = link->getEnglishFromKey("CONFIRM CHARACTER CREATION"); break;
            default: to_print = "";
          }
          break;
        case 1: to_print = link->getEnglishFromKey(startingAttributes[cursorY]->name); break;
        case 2: to_print = link->getEnglishFromKey(availableRaces[cursorY]->name); break;
        case 3: to_print = link->getEnglishFromKey(availableOrigins[cursorY]->name); break;
        case 4: to_print = link->getEnglishFromKey(availableCultures[cursorY]->name); break;
        case 5: to_print = link->getEnglishFromKey(availableReligions[cursorY]->name); break;
        case 6: to_print = link->getEnglishFromKey(availableProfessions[cursorY]->name); break;
        default: to_print = "";
      }
      print(descriptionScreen, 0, 0, to_print.c_str());
      WINDOW * confirmScreen = subwin(characterScreen, 3, characterWidth, LINES - 4, 1);
      screens.push_back(confirmScreen);
      wattron(confirmScreen, COLOR_PAIR(color));
      box(confirmScreen, ACS_VLINE, ACS_HLINE);
      to_print = link->getEnglishFromKey("CONFIRM CHARACTER CREATION");
      mvwprintw(confirmScreen, 1, characterWidth / 2 - to_print.length() / 2, to_print.c_str());
      wattroff(confirmScreen, COLOR_PAIR(color));
      wrefresh(characterScreen);
      bool done2 = false;
      while(!done2) {
        flushinp();
        int32_t keyPressed = getch();
        if(!nameMode) {
          switch(keyPressed) {
            case '4':
            case KEY_LEFT:
              cursorX = (cursorX - 1 + 7) % 7;
              if(cursorX == 0) {
                cursorY = std::min(cursorY, 2);
              }
              else if(cursorX == 1) {
                cursorY = std::min(cursorY, (int32_t) startingAttributes.size() - 1 - currentPannel * numberAttributes);
              }
              else if(cursorX == 2) {
                cursorY = std::min(cursorY, (int32_t) availableRaces.size() - 1);
              }
              else if(cursorX == 3) {
                cursorY = std::min(cursorY, (int32_t) availableOrigins.size() - 1);
              }
              else if(cursorX == 4) {
                cursorY = std::min(cursorY, (int32_t) availableCultures.size() - 1);
              }
              else if(cursorX == 5) {
                cursorY = std::min(cursorY, (int32_t) availableReligions.size() - 1);
              }
              else if(cursorX == 6) {
                cursorY = std::min(cursorY, (int32_t) availableProfessions.size() - 1);
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
                cursorY = std::min(cursorY, (int32_t) startingAttributes.size() - 1);
              }
              else if(cursorX == 2) {
                cursorY = std::min(cursorY, (int32_t) availableRaces.size() - 1);
              }
              else if(cursorX == 3) {
                cursorY = std::min(cursorY, (int32_t) availableOrigins.size() - 1);
              }
              else if(cursorX == 4) {
                cursorY = std::min(cursorY, (int32_t) availableCultures.size() - 1);
              }
              else if(cursorX == 5) {
                cursorY = std::min(cursorY, (int32_t) availableReligions.size() - 1);
              }
              else if(cursorX == 6) {
                cursorY = std::min(cursorY, (int32_t) availableProfessions.size() - 1);
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
                if(cursorY == numberAttributes && (int32_t) startingAttributes.size() > numberAttributes) {
                  cursorY = 0;
                  currentPannel++;
                }
                else if(cursorY == (int32_t) startingAttributes.size()) {
                  cursorY--;
                }
              }
              else if(cursorX == 2) {
                if(cursorY == numberWays && (int32_t) availableRaces.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                }
                else if(cursorY == (int32_t) availableRaces.size()) {
                  cursorY--;
                }
              }
              else if(cursorX == 3) {
                if(cursorY == numberWays && (int32_t) availableOrigins.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                }
                else if(cursorY == (int32_t) availableOrigins.size()) {
                  cursorY--;
                }
              }
              else if(cursorX == 4) {
                if(cursorY == numberWays && (int32_t) availableCultures.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                }
                else if(cursorY == (int32_t) availableCultures.size()) {
                  cursorY--;
                }
              }
              else if(cursorX == 5) {
                if(cursorY == numberWays && (int32_t) availableReligions.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                }
                else if(cursorY == (int32_t) availableReligions.size()) {
                  cursorY--;
                }
              }
              else if(cursorX == 6) {
                if(cursorY == numberWays && (int32_t) availableProfessions.size() > numberWays) {
                  cursorY = 0;
                  currentPannel++;
                }
                else if(cursorY == (int32_t) availableProfessions.size()) {
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
                  move(3, 2 + (link->getEnglishFromKey("Name") + characterName).length() + 2);
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
                move(3, 2 + (link->getEnglishFromKey("Name") + characterName).length() + 2);
              }
              done2 = true;
              break;
            default:
              characterName += keyPressed;
              move(3, 2 + (link->getEnglishFromKey("Name") + characterName).length() + 2);
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

  void commandLoop(Link * link, WINDOW * mapScreen, WINDOW * statsScreen, WINDOW * displayScreen, WINDOW * targetScreen) {
    while(!link->isReady()) {
      usleep(1);
    }
    while(true) {
      StateDisplay * display = link->getState();
      Region * region = link->getPlayer()->getRegion();
      if(display->need_to_send_actions) {
        displayMap(display, link->getAdventure(), link->getPlayer(), mapScreen, link);
        displayStats(link->getPlayer(), statsScreen, link);
        displayCommands(targetScreen, link);
        wrefresh(targetScreen);
        bool done = false;
        int32_t type;
        int32_t object_type;
        std::string object;
        int32_t object_id;
        float orientation_z;
        Skill * skill;
        int32_t target_id;
        int32_t target_x;
        int32_t target_y;
        float overcharge;
        while(!done) {
          object_type = 0;
          object = "";
          object_id = 0;
          orientation_z = link->getPlayer()->getOrientation().z;
          skill = nullptr;
          target_id = 0;
          target_x = (int32_t) std::floor(link->getPlayer()->getCoord().x) - region->id.x;
          target_y = (int32_t) std::floor(link->getPlayer()->getCoord().y) - region->id.y;
          overcharge = 1;
          overcharge = 1;
          overcharge = 1;
          flushinp();
          int32_t keyPressed = getch();
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
              if(selectTarget(mapScreen, targetScreen, display, link->getPlayer(), link->getPlayer()->getVisionRange(), target_id, target_x, target_y, orientation_z, link)) {
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
              skill = selectSkill(displayScreen, targetScreen, link->getPlayer(), overcharge, link);
              if(skill != nullptr && (skill->target_type == TARGET_SELF || selectTarget(mapScreen, targetScreen, display, link->getPlayer(), skill->range, target_id, target_x, target_y, orientation_z, link))) {
                done = true;
                object = skill->name;
              }
              break;
            case 'i':
            case 'I':
              type = ACTION_SWAP_GEAR;
              object = selectItem(displayScreen, targetScreen, link->getPlayer(), object_type, object_id, link);
              if(object != "") {
                for(ItemSlot * slot : link->getPlayer()->getGear()->getItems()) {
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
              if(link->getPlayer()->getGear()->getWeapon_1()->use_ammo) {
                object = selectAmmo(displayScreen, targetScreen, link->getPlayer(), link);
                if(object != "" && (link->getPlayer()->getGear()->getWeapon_1()->getAmmo() == nullptr
                || link->getPlayer()->getGear()->getWeapon_1()->getAmmo()->getProjectile()->name != object
                || link->getPlayer()->getGear()->getWeapon_1()->getCurrentCapacity() < link->getPlayer()->getGear()->getWeapon_1()->capacity)) {
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
          case ACTION_REST:
          case ACTION_BREAKPOINT:
            sendAction(link, type, nullptr, nullptr, 0);
            break;
          case ACTION_MOVE:
          case ACTION_USE_SKILL: {
            MathUtil::Target * target = new MathUtil::Target();
            ((MathUtil::Target *) target)->type = (target_id == 0 ? TARGET_COORDINATES : TARGET_CHARACTER);
            ((MathUtil::Target *) target)->character = (target_id == 0 ? 0 : target_id);
            ((MathUtil::Target *) target)->coord.x = target_x + 0.5;
            ((MathUtil::Target *) target)->coord.y = target_y + 0.5;
            ((MathUtil::Target *) target)->coord.z = 0;
            ((MathUtil::Target *) target)->next = nullptr;
            sendAction(link, type, (void *) target, skill, overcharge);
            break;
          }
          case ACTION_RELOAD:
          case ACTION_GRAB:
          case ACTION_USE_ITEM: {
            ItemSlot * slot = new ItemSlot();
            for(ItemSlot * item : link->getPlayer()->getGear()->getItems()) {
              if(item->item->id == object_id) {
                slot->x = item->x;
                slot->y = item->y;
                slot->slot = item->slot;
              }
            }
            sendAction(link, type, (void *) slot, nullptr, 0);
            break;
          }
          case ACTION_SWAP_GEAR: {
            ItemSlot * slot = new ItemSlot();
            ItemSlot * dummy = new ItemSlot();
            for(ItemSlot * item : link->getPlayer()->getGear()->getItems()) {
              if(item->item->id == object_id) {
                slot->x = item->x;
                slot->y = item->y;
                slot->slot = item->slot;
              }
            }
            dummy->x = 0;
            dummy->y = 0;
            dummy->slot = 0;
            sendAction(link, type, (void *) slot, (void *) dummy, 0);
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

  Skill * selectSkill(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, float & overcharge, Link * link) {
    Skill * result = nullptr;
    bool done = false;
    int32_t cursorX = 0;
    int32_t cursorY = 0;
    int32_t lines = 0;
    int32_t cols = 0;
    getmaxyx(displayScreen, lines, cols);
    while(!done) {
      WINDOW * tempScreen = nullptr;
      wclear(displayScreen);
      wclear(targetScreen);
      box(displayScreen, ACS_VLINE, ACS_HLINE);
      box(targetScreen, ACS_VLINE, ACS_HLINE);
      std::string to_print = link->getEnglishFromKey("SKILLS");
      mvwprintw(displayScreen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
      int32_t currentX = 0;
      int32_t currentY = 0;
      int32_t offset = 0;
      int32_t sizeX = 0;
      int32_t maxY = 0;
      int32_t color = WHITE;
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
        std::string to_print = link->getEnglishFromKey(skill->name);
        if(color == BLUE) {
          result = skill;
          tempScreen = displaySkill(skill, player, targetScreen, 1, link);
        }
        sizeX = std::max(sizeX, (int32_t) to_print.length());
        wattron(displayScreen, COLOR_PAIR(color));
        mvwprintw(displayScreen, 3 + currentY++, offset + 1, to_print.c_str());
        wattroff(displayScreen, COLOR_PAIR(color));
      }
      maxY = std::max(--currentY, maxY);

      wrefresh(displayScreen);
      wrefresh(targetScreen);
      flushinp();
      int32_t keyPressed = getch();
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
          done = selectOvercharge(displayScreen, targetScreen, result, player, overcharge, link);
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

  bool selectOvercharge(WINDOW * displayScreen, WINDOW * targetScreen, Skill * skill, Character * player, float & overcharge, Link * link) {
    int32_t mana_cost = 0;
    int32_t lines = 0;
    int32_t cols = 0;
    int32_t overcharge_type = 1;
    int32_t color = WHITE;
    getmaxyx(displayScreen, lines, cols);
    while(true) {
      wclear(displayScreen);
      box(displayScreen, ACS_VLINE, ACS_HLINE);
      std::string to_print = link->getEnglishFromKey("Overcharging") + std::string(" ") + link->getEnglishFromKey(skill->name);
      mvwprintw(displayScreen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
      if(overcharge_type == 1) {
        color = BLUE;
      }
      else {
        color = WHITE;
      }
      to_print = link->getEnglishFromKey("Power Overcharging") + std::string(": ") + std::to_string(overcharge) + std::string(" - ") + link->getEnglishFromKey(std::string("overcharge_power_type") + std::to_string(skill->overcharge_power_type));
      wattron(displayScreen, COLOR_PAIR(color));
      mvwprintw(displayScreen, lines / 2 - 1, cols / 2 - to_print.length() / 2, to_print.c_str());
      wattroff(displayScreen, COLOR_PAIR(color));
      if(overcharge_type == 2) {
        color = BLUE;
      }
      else {
        color = WHITE;
      }
      to_print = link->getEnglishFromKey("Duration Overcharging") + std::string(": ") + std::to_string(overcharge) + std::string(" - ") + link->getEnglishFromKey(std::string("overcharge_duration_type") + std::to_string(skill->overcharge_duration_type));
      wattron(displayScreen, COLOR_PAIR(color));
      mvwprintw(displayScreen, lines / 2, cols / 2 - to_print.length() / 2, to_print.c_str());
      wattroff(displayScreen, COLOR_PAIR(color));
      if(overcharge_type == 3) {
        color = BLUE;
      }
      else {
        color = WHITE;
      }
      to_print = link->getEnglishFromKey("Range Overcharging") + std::string(": ") + std::to_string(overcharge) + std::string(" - ") + link->getEnglishFromKey(std::string("overcharge_range_type") + std::to_string(skill->overcharge_range_type));
      wattron(displayScreen, COLOR_PAIR(color));
      mvwprintw(displayScreen, lines / 2 + 1, cols / 2 - to_print.length() / 2, to_print.c_str());
      wattroff(displayScreen, COLOR_PAIR(color));
      wrefresh(displayScreen);
      WINDOW * tempScreen = nullptr;
      tempScreen = displaySkill(skill, player, targetScreen, overcharge, link);
      flushinp();
      int32_t keyPressed = getch();
      if(tempScreen != nullptr) {
        wclear(tempScreen);
        delwin(tempScreen);
      }
      switch(keyPressed) {
        case '4':
        case KEY_LEFT:
          switch(overcharge_type) {
            case 1:
              if(overcharge > 1) {
                overcharge--;
              }
              break;
            case 2:
              if(overcharge > 1) {
                overcharge--;
              }
              break;
            case 3:
              if(overcharge > 1) {
                overcharge--;
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
              mana_cost = skill->getManaCost();
              if(skill->overcharge_power_type != OVERCHARGE_NONE && player->getMana() >= mana_cost && player->getFlow() >= mana_cost) {
                overcharge++;
              }
              break;
            case 2:
              mana_cost = skill->getManaCost();
              if(skill->overcharge_duration_type != OVERCHARGE_NONE && player->getMana() >= mana_cost && player->getFlow() >= mana_cost) {
                overcharge++;
              }
              break;
            case 3:
              mana_cost = skill->getManaCost();
              if(skill->overcharge_range_type != OVERCHARGE_NONE && player->getMana() >= mana_cost && player->getFlow() >= mana_cost) {
                overcharge++;
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
          mana_cost = skill->getManaCost();
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

  bool selectTarget(WINDOW * mapScreen, WINDOW * targetScreen, StateDisplay * display, Character * player, int32_t range, int32_t & target_id, int32_t & target_x, int32_t & target_y, float & orientation_z, Link * link) {
    bool done = false;
    int32_t lines = 0;
    int32_t cols = 0;
    int32_t player_x = target_x;
    int32_t player_y = target_y;
    getmaxyx(mapScreen, lines, cols);
    int32_t lines2 = 0;
    int32_t cols2 = 0;
    getmaxyx(targetScreen, lines2, cols2);
    cchar_t *wch_old = new cchar_t();
    mvwin_wch(mapScreen, lines / 2 - CHUNK_SIZE * 3 / 2 + CHUNK_SIZE * 3 - 1 - target_y, target_x + cols / 2 - CHUNK_SIZE * 3 / 2, wch_old);
    Region * region = player->getRegion();
    while(!done) {
      flushinp();
      int32_t keyPressed = getch();
      target_id = 0;
      int32_t previous_x = target_x;
      int32_t previous_y = target_y;
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
          orientation_z = MathUtil::getOrientationToTarget(player_x, player_y, target_x + region->id.x, target_y + region->id.y);
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
        if((int32_t) std::floor(character->x) == target_x + region->id.x && (int32_t) std::floor(character->y) == target_y + region->id.y) {
          target_id = character->id;
          displayTarget(character, targetScreen, link);
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

  std::string selectItem(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, int32_t & object_type, int32_t & object_id, Link * link) {
      std::string result = "";
      bool done = false;
      int32_t cursorX = 0;
      int32_t cursorY = 0;
      int32_t lines = 0;
      int32_t cols = 0;
      getmaxyx(displayScreen, lines, cols);
      while(!done) {
        WINDOW * tempScreen = nullptr;
        wclear(displayScreen);
        wclear(targetScreen);
        box(displayScreen, ACS_VLINE, ACS_HLINE);
        box(targetScreen, ACS_VLINE, ACS_HLINE);
        std::string to_print = link->getEnglishFromKey("INVENTORY");
        mvwprintw(displayScreen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
        int32_t currentX = 0;
        int32_t currentY = 0;
        int32_t offset = 0;
        int32_t sizeX = 0;
        int32_t maxY = 0;
        int32_t color = WHITE;
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
          std::string to_print = link->getEnglishFromKey(slot->item->name);
          if(color == BLUE) {
            result = slot->item->name;
            object_id = slot->item->id;
            object_type = OBJECT_ITEM;
            tempScreen = displayItem(slot->item, targetScreen, link);
          }
          sizeX = std::max(sizeX, (int32_t) to_print.length());
          wattron(displayScreen, COLOR_PAIR(color));
          mvwprintw(displayScreen, 3 + currentY++, offset + 1, to_print.c_str());
          wattroff(displayScreen, COLOR_PAIR(color));
        }
        maxY = std::max(--currentY, maxY);

        wrefresh(displayScreen);
        wrefresh(targetScreen);
        flushinp();
        int32_t keyPressed = getch();
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

  std::string selectAmmo(WINDOW * displayScreen, WINDOW * targetScreen, Character * player, Link * link) {
    std::string result = "";
    bool done = false;
    int32_t cursorX = 0;
    int32_t cursorY = 0;
    int32_t lines = 0;
    int32_t cols = 0;
    getmaxyx(displayScreen, lines, cols);
    while(!done) {
      WINDOW * tempScreen = nullptr;
      wclear(displayScreen);
      wclear(targetScreen);
      box(displayScreen, ACS_VLINE, ACS_HLINE);
      box(targetScreen, ACS_VLINE, ACS_HLINE);
      std::string to_print = link->getEnglishFromKey("AMMUNITION");
      mvwprintw(displayScreen, 1, cols / 2 - to_print.length() / 2, to_print.c_str());
      int32_t currentX = 0;
      int32_t currentY = 0;
      int32_t offset = 0;
      int32_t sizeX = 0;
      int32_t maxY = 0;
      int32_t color = WHITE;
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
          std::string to_print = link->getEnglishFromKey(ammo->getProjectile()->name);
          if(color == BLUE) {
            result = ammo->getProjectile()->name;
            tempScreen = displayItem(ammo, targetScreen, link);
          }
          sizeX = std::max(sizeX, (int32_t) to_print.length());
          wattron(displayScreen, COLOR_PAIR(color));
          mvwprintw(displayScreen, 3 + currentY++, offset + 1, to_print.c_str());
          wattroff(displayScreen, COLOR_PAIR(color));
        }
      }
      maxY = std::max(--currentY, maxY);

      wrefresh(displayScreen);
      wrefresh(targetScreen);
      flushinp();
      int32_t keyPressed = getch();
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

void Display::sendAction(Link * link, int32_t type, void * arg1, void * arg2, float overcharge) {
  link->sendAction(type, arg1, arg2, overcharge);
}
