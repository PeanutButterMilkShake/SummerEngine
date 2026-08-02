#pragma once

#include "EnginePanel.h"

class WorkspaceManager
{
public:
    static EnginePanel* RegisterPanel(std::string title, std::string category, Vector2 defaultLocation)
    {
        return new EnginePanel(title, category, defaultLocation);
    }
};


// Helper macros
/*#define REGISTER_ENGINE_COMMAND(name, menu, func) \ 
    static struct UniqueEngineFunc_##__LINE__ { \
        UniqueReg_##__LINE__() { \
            RegisterCommand(name, menu, func); \
        } \
    } unique_reg_instance_##__LINE__;

#define REGISTER_ENGINE_MENU(name, keybind, editorPanel) \ 
    static struct UniqueEngineMenu_##__LINE__ { \
        UniqueReg_##__LINE__() { \
            RegisterMenu(name, keybind, editorPanel); \
        } \
    } unique_reg_instance_##__LINE__;
*/