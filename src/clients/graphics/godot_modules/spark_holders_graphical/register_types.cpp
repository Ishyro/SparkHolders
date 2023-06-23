#include "register_types.h"

#include "core/object/class_db.h"
#include "GodotLink.h"

void initialize_spark_holders_graphical_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
    ClassDB::register_class<GodotLink>();
}

void uninitialize_spark_holders_graphical_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
   // Nothing to do here in this example.
}
