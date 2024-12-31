#include <set>

#include "data/Effect.h"

#include "data/items/ContainerItem.h"

#include "util/String.h"

bool ContainerItem::overlap(ItemSlot * s1, ItemSlot * s2) {
  if(s1->slot != s2->slot) {
    return false;
  }
  return s1->x < s2->x + s2->item->sizeX && s1->x + s1->item->sizeX > s2->x &&
    s1->y < s2->y + s2->item->sizeY && s1->y + s1->item->sizeY > s2->y;
}

bool ContainerItem::add(Item * item, int32_t x, int32_t y) {
  ItemSlot * toadd = new ItemSlot();
  toadd->x = x;
  toadd->y = y;
  if(subtype == ITEM_BAG) {
    toadd->slot = ITEM_SLOT_INSIDE_BAG;
  }
  else if(subtype == ITEM_BELT) {
    toadd->slot = ITEM_SLOT_INSIDE_BELT;
  }
  toadd->item = item;
  if(y > contentY - item->sizeY || x > contentX - item->sizeX) {
    return false;
  }
  for(ItemSlot * slot : content) {
    if(overlap(toadd, slot)) {
      toadd->item = nullptr;
      delete toadd;
      return false;
    }
  }
  content.push_back(toadd);
  return true;
}

void ContainerItem::add_all(std::list<Item *> items) {
  int32_t slot_type;
  if(subtype == ITEM_BAG) {
    slot_type = ITEM_SLOT_INSIDE_BAG;
  }
  else if(subtype == ITEM_BELT) {
    slot_type = ITEM_SLOT_INSIDE_BELT;
  }
  int32_t current_x = 0;
  int32_t current_y = 0;
  for(Item * item : items) {
    if(current_x + item->sizeX >= contentX) {
      current_x = 0;
      // TODO
      current_y += item->sizeY;
    }
    ItemSlot * slot = new ItemSlot();
    slot->x = current_x;
    slot->y = current_y;
    slot->slot = slot_type;
    slot->item = item;
    content.push_back(slot);
    current_x += item->sizeX;
  }
}

Item * ContainerItem::remove(int32_t x, int32_t y) {
  Item * result = nullptr;
  ItemSlot * toremove = nullptr;
  for(ItemSlot * slot : content) {
    if(slot->x == x && slot->y == y) {
      result = slot->item;
      toremove = slot;
      break;
    }
  }
  if(toremove != nullptr) {
    content.remove(toremove);
    toremove->item = nullptr;
    delete toremove;
  }
  return result;
}

bool ContainerItem::move(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
  Item * item = remove(x1, y1);
  if(!add(item, x2, y2)) {
    add(item, x1, y1);
    return false;
  }
  return true;
}

float ContainerItem::getWeight() {
  float result = weight;
  for(ItemSlot * slot : content) {
    result += slot->item->getWeight();
  }
  return result;
}

std::list<ItemSlot *> ContainerItem::getItems() { return content; }

std::string ContainerItem::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_long(ss, id);
  String::insert_int(ss, type);
  String::insert_int(ss, subtype);
  String::insert_int(ss, tier);
  String::insert_int(ss, max_tier);
  String::insert_float(ss, weight);
  String::insert_int(ss, sizeX);
  String::insert_int(ss, sizeY);
  String::insert_int(ss, gold_value);
  String::insert_bool(ss, droppable);
  String::insert_bool(ss, usable);
  String::insert_bool(ss, consumable);
  String::insert_int(ss, use_time);
  std::stringstream * ss_effects = new std::stringstream();
  for(Effect * effect : effects) {
    String::insert(ss_effects, effect->to_string());
  }
  String::insert(ss, ss_effects->str());
  delete ss_effects;
  String::insert_int(ss, swap_time);
  String::insert_int(ss, max_durability);
  String::insert_int(ss, durability);
  String::insert_bool(ss, canTakeFrom);
  String::insert_bool(ss, repercuteWeight);
  String::insert_bool(ss, isLimited);
  String::insert_int(ss, limit_type);
  String::insert_int(ss, contentX);
  String::insert_int(ss, contentY);
  std::stringstream * ss_items = new std::stringstream();
  for(ItemSlot * slot : content) {
    String::insert_int(ss_items, slot->x);
    String::insert_int(ss_items, slot->y);
    String::insert(ss_items, slot->item->to_string());
  }
  String::insert(ss, ss_items->str());
  delete ss_items;
  std::string result = ss->str();
  delete ss;
  return result;
}
