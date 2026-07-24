#pragma once

#include "player.h"
#include <string>

class SaveLoad {
public:
    static bool SaveGame(const Player& player, const std::string& filepath);
    static bool LoadGame(Player& player, const std::string& filepath);
};
