#ifndef _GODOT_LINK_H_
#define _GODOT_LINK_H_

#include "core/object/ref_counted.h"


#include "communication/Socket.h"

#include "clients/Link.h"
#include "clients/Translator.h"

#include "Values.h"

class GodotLink : public RefCounted {
  GDCLASS(GodotLink, RefCounted);
public:
  GodotLink():
    link(nullptr),
    translator(nullptr)
  {}
  void initialize(String ip);
  Array getAvaillableTiles();
  String getPathFromTile(String tile);

protected:
  static void _bind_methods();
private:
  Socket s;
  Link * link;
  Translator * translator;
};

#endif // _GODOT_LINK_H_
