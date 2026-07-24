#pragma once

#include "raylib.h"
#include <string>
#include <vector>

struct VentZone {
    Vector3 position;
    float radius;
    bool isDangerous;
};

struct Interactable {
    Vector3 position;
    float radius;
    std::string name;
    std::string hint;
    bool inRange;
    int id;
};

struct Room {
    Vector3 minBound;
    Vector3 maxBound;
    std::string name;
};

class Level {
public:
    std::vector<VentZone> vents;
    std::vector<Interactable> interactables;
    std::vector<Room> rooms;

    void Init();
    void Draw();
    void CheckInteractions(Vector3 playerPos);
    Interactable* GetClosestInteractable(Vector3 playerPos, float maxDist);
    bool IsInVentZone(Vector3 playerPos) const;
    bool IsInVentDanger(Vector3 playerPos, float gameHour) const;
};
