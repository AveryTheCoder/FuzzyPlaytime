#pragma once

#include "raylib.h"
#include "nlohmann/json.hpp"
#include <string>
#include <fstream>

using json = nlohmann::json;

struct MonsterData {
    std::string name;
    std::string description;
    Vector3 spawn;
    float speed;
    float attackRange;
    float sightRange;
    Color friendlyColor;
    Color hostileColor;
    json behavior;
};

struct PlayerData {
    float health;
    float maxHealth;
    float walkSpeed;
    float sprintSpeed;
    Vector3 pos;
};

struct SettingsData {
    float masterVolume;
    float mouseSensitivity;
    float gameMinutesPerSecond;
    int startHour;
    int startMinute;
};

struct PasswordsData {
    std::string cctv;
    std::string pictures;
};

class DataLoader {
public:
    json data;
    bool loaded;

    DataLoader();

    bool Load(const std::string& path);
    bool Save(const std::string& path);

    MonsterData GetMonsterData(const std::string& key) const;
    PlayerData GetPlayerData() const;
    SettingsData GetSettingsData() const;
    PasswordsData GetPasswords() const;

    int GetJingleTriggerMin() const;
    int GetJingleTriggerMax() const;
};
