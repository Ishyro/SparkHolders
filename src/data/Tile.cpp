#include "data/Tile.h"

#include "util/String.h"

std::string Tile::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_bool(ss, unwalkable);
  String::insert_bool(ss, opaque);
  String::insert_bool(ss, solid);
  String::insert_int(ss, light);
  String::insert_float(ss, ap_cost);
  std::string result = ss->str();
  delete ss;
  return result;
}

Tile * Tile::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  bool unwalkable = String::extract_bool(ss);
  bool opaque = String::extract_bool(ss);
  bool solid = String::extract_bool(ss);
  int light = String::extract_int(ss);
  float ap_cost = String::extract_float(ss);
  delete ss;
  return new Tile(name, unwalkable, opaque, solid, light, ap_cost);
}
