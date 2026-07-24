#include "include/data_loader.h"
#include <fstream>
#include <iostream>

DataLoader::DataLoader() : loaded(false) {}

bool DataLoader::Load(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) return false;

    try
    {
        data = json::parse(file);
        loaded = true;
        return true;
    }
    catch (...)
    {
        loaded = false;
        return false;
    }
}

bool DataLoader::Save(const std::string& path)
{
    std::ofstream file(path);
    if (!file.is_open()) return false;

    file << data.dump(4);
    return file.good();
}

MonsterData DataLoader::GetMonsterData(const std::string& key) const
{
    MonsterData md = {};
    if (!loaded || !data.contains("entities") || !data["entities"].contains(key))
        return md;

    const json& e = data["entities"][key];

    md.name = e.value("name", key);
    md.description = e.value("description", "");
    md.speed = e.value("speed", 2.0f);
    md.attackRange = e.value("attack_range", 2.0f);
    md.sightRange = e.value("sight_range", 10.0f);

    if (e.contains("spawn"))
    {
        md.spawn.x = e["spawn"].value("x", 0.0f);
        md.spawn.y = e["spawn"].value("y", 0.0f);
        md.spawn.z = e["spawn"].value("z", 0.0f);
    }

    if (e.contains("color"))
    {
        if (e["color"].contains("friendly"))
        {
            md.friendlyColor.r = e["color"]["friendly"].value("r", 200);
            md.friendlyColor.g = e["color"]["friendly"].value("g", 200);
            md.friendlyColor.b = e["color"]["friendly"].value("b", 200);
            md.friendlyColor.a = e["color"]["friendly"].value("a", 255);
        }
        if (e["color"].contains("hostile"))
        {
            md.hostileColor.r = e["color"]["hostile"].value("r", 200);
            md.hostileColor.g = e["color"]["hostile"].value("g", 0);
            md.hostileColor.b = e["color"]["hostile"].value("b", 0);
            md.hostileColor.a = e["color"]["hostile"].value("a", 255);
        }
    }

    if (e.contains("behavior")) md.behavior = e["behavior"];

    return md;
}

PlayerData DataLoader::GetPlayerData() const
{
    PlayerData pd = {};
    if (!loaded || !data.contains("player")) return pd;

    const json& p = data["player"];
    pd.health = p.value("health", 100.0f);
    pd.maxHealth = p.value("max_health", 100.0f);
    pd.walkSpeed = p.value("walk_speed", 5.0f);
    pd.sprintSpeed = p.value("sprint_speed", 8.0f);

    if (p.contains("pos"))
    {
        pd.pos.x = p["pos"].value("x", 0.0f);
        pd.pos.y = p["pos"].value("y", 1.7f);
        pd.pos.z = p["pos"].value("z", 0.0f);
    }

    return pd;
}

SettingsData DataLoader::GetSettingsData() const
{
    SettingsData sd = {};
    if (!loaded || !data.contains("settings")) return sd;

    const json& s = data["settings"];
    sd.masterVolume = s.value("master_volume", 1.0f);
    sd.mouseSensitivity = s.value("mouse_sensitivity", 0.003f);
    sd.gameMinutesPerSecond = s.value("game_minutes_per_second", 1.0f);
    sd.startHour = s.value("start_hour", 8);
    sd.startMinute = s.value("start_minute", 0);

    return sd;
}

PasswordsData DataLoader::GetPasswords() const
{
    PasswordsData pd = {};
    if (!loaded || !data.contains("objects")) return pd;

    const json& objs = data["objects"];
    if (objs.contains("main_computer"))
    {
        const json& mc = objs["main_computer"];
        if (mc.contains("passwords"))
        {
            pd.cctv = mc["passwords"].value("cctv", "NCFEACL2011");
            pd.pictures = mc["passwords"].value("pictures", "SUSPHOTOS");
        }
    }

    return pd;
}

int DataLoader::GetJingleTriggerMin() const
{
    if (!loaded || !data.contains("entities") || !data["entities"].contains("john_jeff"))
        return 15;

    const json& b = data["entities"]["john_jeff"].value("behavior", json::object());
    return b.value("jingle_trigger_min", 15);
}

int DataLoader::GetJingleTriggerMax() const
{
    if (!loaded || !data.contains("entities") || !data["entities"].contains("john_jeff"))
        return 17;

    const json& b = data["entities"]["john_jeff"].value("behavior", json::object());
    return b.value("jingle_trigger_max", 17);
}
