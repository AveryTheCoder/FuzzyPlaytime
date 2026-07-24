#pragma once

#include "raylib.h"
#include "game_time.h"
#include <string>
#include <vector>

class DataLoader;

enum MonsterType {
    MONSTER_FUZZY,
    MONSTER_MELODY,
    MONSTER_HAWK,
    MONSTER_JJ,
    MONSTER_JOHN_JEFF,
    MONSTER_INDIANA,
    MONSTER_PATRIOT,
    MONSTER_POTTY
};

enum MonsterState {
    MONSTER_IDLE,
    MONSTER_PATROL,
    MONSTER_STALKING,
    MONSTER_HOSTILE,
    MONSTER_FRIENDLY,
    MONSTER_DEAD
};

struct MonsterZone {
    Vector3 position;
    float radius;
    std::string zoneName;
};

class Monster {
public:
    MonsterType type;
    MonsterState state;
    std::string name;
    std::string description;
    Vector3 position;
    Vector3 spawnPosition;
    float speed;
    bool isAlive;
    bool isHostile;
    float hostTimer;

    Monster(MonsterType type, const std::string& name, Vector3 spawn);
    virtual ~Monster() = default;

    virtual void Update(float dt, const GameTime& time, Vector3 playerPos);
    virtual void Draw();
    virtual bool CanHurtPlayer(Vector3 playerPos, const GameTime& time) const;
    virtual std::string GetHint() const;

    float DistanceTo(Vector3 target) const;
    void MoveToward(Vector3 target, float dt);
};

class MonsterManager {
public:
    std::vector<Monster*> monsters;
    void Init(const DataLoader& loader);
    void Update(float dt, const GameTime& time, Vector3 playerPos);
    void DrawAll();
    void Cleanup();
    Monster* GetClosestMonster(Vector3 pos, float maxDist) const;
    bool IsAnyMonsterHostile() const;
};
