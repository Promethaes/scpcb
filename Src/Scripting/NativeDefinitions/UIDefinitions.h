#ifndef UIDEFINITIONS_H_INCLUDED
#define UIDEFINITIONS_H_INCLUDED

#include "../NativeDefinition.h"

class ScriptManager;
class UIMesh;
class Config;
class World;

class UIDefinitions : NativeDefinition {
    public:
        UIDefinitions(ScriptManager* mgr, UIMesh* uiMesh, Config* config, World* world);
};

#endif
