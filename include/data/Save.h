#ifndef _SAVE_H_
#define _SAVE_H_

#include "data/Adventure.h"

class Save {
  public:
    Adventure * load();
    void save(Adventure * adventure);
    Save(Adventure * adventure) {
      save(adventure);
    }
};

#endif // _SAVE_H_
