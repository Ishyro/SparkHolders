#include "data/Block.h"

#include "util/String.h"

std::string Block::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_int(ss, type);
  String::insert(ss, material);
  String::insert_bool(ss, unwalkable);
  String::insert_bool(ss, opaque);
  String::insert_int(ss, light);
  String::insert_int(ss, orientation);
  String::insert_float(ss, ap_cost);
  std::string result = ss->str();
  delete ss;
  return result;
}

Block * Block::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int type = String::extract_int(ss);
  std::string material = String::extract(ss);
  bool unwalkable = String::extract_bool(ss);
  bool opaque = String::extract_bool(ss);
  int light = String::extract_int(ss);
  int orientation = String::extract_int(ss);
  float ap_cost = String::extract_float(ss);
  delete ss;
  return new Block(name, type, material, unwalkable, opaque, light, orientation, ap_cost);
}
