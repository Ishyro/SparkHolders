#ifndef _FILE_OPENER_H_
#define _FILE_OPENER_H_

#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <map>
#include <iostream>
#include <fstream>

#include "data/Map.h"
#include "data/World.h"

#include "Values.h"

namespace fileOpener {
  std::map<const std::string,std::string> getValuesFromIni(std::string fileName);
  Map getValuesFromMapFile(std::string fileName, World world);
};

#endif // _FILE_OPENER_H_
