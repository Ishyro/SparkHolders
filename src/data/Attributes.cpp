#include "data/Attributes.h"

#include "utils/String.h"

std::string Attributes::to_string() {
  std::stringstream * ss = new std::stringstream();
  String::insert(ss, name);
  String::insert_int(ss, baseHp);
  String::insert_int(ss, baseMana);
  String::insert_int(ss, baseArmor);
  String::insert_int(ss, baseSoulBurn);
  String::insert_int(ss, baseFlow);
  String::insert_int(ss, baseVisionRange);
  String::insert_int(ss, baseVisionPower);
  String::insert_int(ss, baseDetectionRange);
  std::string result = ss->str();
  delete ss;
  return result;
}

Attributes * Attributes::from_string(std::string to_read) {
  std::stringstream * ss = new std::stringstream(to_read);
  std::string name = String::extract(ss);
  int baseHp = String::extract_int(ss);
  int baseMana = String::extract_int(ss);
  int baseArmor = String::extract_int(ss);
  int baseSoulBurn = String::extract_int(ss);
  int baseFlow = String::extract_int(ss);
  int baseVisionRange = String::extract_int(ss);
  int baseVisionPower = String::extract_int(ss);
  int baseDetectionRange = String::extract_int(ss);
  delete ss;
  return new Attributes(name, baseHp, baseMana, baseArmor, baseSoulBurn, baseFlow, baseVisionRange, baseVisionPower, baseDetectionRange, new Gear());
}
