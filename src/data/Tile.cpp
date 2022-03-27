#include "data/Tile.h"

Tile::Tile(std::string fileName) {
  std::map<const std::string,std::string> values = fileOpener::getValuesFromIni(fileName);
  this->name = values.at("name");
  std::istringstream is(values.at("untraversable"));
  is >> std::boolalpha >> this->untraversable;
}
