#include "include/monster.h"
#include "include/data_loader.h"
#include "raymath.h"
#include <cmath>

Monster::Monster(MonsterType t, const std::string& n, Vector3 spawn)
    : type(t), state(MONSTER_IDLE), name(n), position(spawn), spawnPosition(spawn),
      speed(2.0f), isAlive(true), isHostile(false), hostTimer(0.0f)
{
    description = "";
}

void Monster::Update(float dt, const GameTime& time, Vector3 playerPos)
{
    if (!isAlive) { state = MONSTER_DEAD; return; }

    if (hostTimer > 0.0f)
    {
        hostTimer -= dt;
        if (hostTimer <= 0.0f)
        {
            hostTimer = 0.0f;
            isHostile = false;
            state = MONSTER_IDLE;
        }
    }
}

void Monster::Draw()
{
    Color bodyColor = isHostile ? RED : DARKGREEN;
    float bobY = sinf(GetTime() * 2.0f) * 0.05f;

    DrawCube({position.x, position.y + bobY, position.z}, 0.6f, 1.6f, 0.4f, bodyColor);
    DrawCube({position.x, position.y + 1.1f + bobY, position.z}, 0.5f, 0.5f, 0.4f, bodyColor);

    DrawSphere({position.x - 0.15f, position.y + 1.3f + bobY, position.z}, 0.08f, WHITE);
    DrawSphere({position.x + 0.15f, position.y + 1.3f + bobY, position.z}, 0.08f, WHITE);
    DrawSphere({position.x - 0.15f, position.y + 1.3f + bobY, position.z - 0.03f}, 0.04f, BLACK);
    DrawSphere({position.x + 0.15f, position.y + 1.3f + bobY, position.z - 0.03f}, 0.04f, BLACK);
}

bool Monster::CanHurtPlayer(Vector3 playerPos, const GameTime& time) const
{
    if (!isAlive || !isHostile) return false;
    return DistanceTo(playerPos) < 2.0f;
}

std::string Monster::GetHint() const
{
    return "You see something ahead...";
}

float Monster::DistanceTo(Vector3 target) const
{
    float dx = position.x - target.x;
    float dy = position.y - target.y;
    float dz = position.z - target.z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

void Monster::MoveToward(Vector3 target, float dt)
{
    Vector3 dir = {target.x - position.x, 0, target.z - position.z};
    float len = sqrtf(dir.x * dir.x + dir.z * dir.z);
    if (len > 0.1f)
    {
        dir.x /= len;
        dir.z /= len;
        position.x += dir.x * speed * dt;
        position.z += dir.z * speed * dt;
    }
}

// === FUZZY ===
class FuzzyMonster : public Monster {
public:
    FuzzyMonster(Vector3 spawn) : Monster(MONSTER_FUZZY, "Fuzzy", spawn)
    {
        description = "The mascot. Nice when not turned evil.";
        speed = 4.0f;
    }

    void Update(float dt, const GameTime& time, Vector3 playerPos) override
    {
        Monster::Update(dt, time, playerPos);
        if (!isHostile)
        {
            state = MONSTER_FRIENDLY;
            float dist = DistanceTo(playerPos);
            if (dist > 4.0f) MoveToward(playerPos, dt * 0.3f);
        }
        else
        {
            state = MONSTER_HOSTILE;
            MoveToward(playerPos, dt);
        }
    }

    void Draw() override
    {
        Color bodyColor = isHostile ? Color{0, 180, 0, 255} : Color{0, 200, 0, 255};
        float bobY = sinf(GetTime() * 2.0f) * 0.05f;

        DrawCube({position.x, position.y + bobY, position.z}, 0.8f, 2.0f, 0.5f, bodyColor);
        DrawCube({position.x, position.y + 1.3f + bobY, position.z}, 0.7f, 0.6f, 0.5f, bodyColor);

        DrawSphere({position.x - 0.18f, position.y + 1.5f + bobY, position.z}, 0.1f, WHITE);
        DrawSphere({position.x + 0.18f, position.y + 1.5f + bobY, position.z}, 0.1f, WHITE);
        DrawSphere({position.x - 0.18f, position.y + 1.5f + bobY, position.z - 0.04f}, 0.05f, BLACK);
        DrawSphere({position.x + 0.18f, position.y + 1.5f + bobY, position.z - 0.04f}, 0.05f, BLACK);

        DrawCube({position.x, position.y + 2.0f + bobY, position.z}, 0.9f, 0.15f, 0.55f, Color{100, 70, 30, 255});
        DrawCube({position.x, position.y + 2.1f + bobY, position.z}, 0.7f, 0.1f, 0.4f, Color{100, 70, 30, 255});
    }

    bool CanHurtPlayer(Vector3 playerPos, const GameTime& time) const override
    {
        return isHostile && DistanceTo(playerPos) < 2.5f;
    }

    std::string GetHint() const override
    {
        return isHostile ? "Fuzzy seems different..." : "Fuzzy waves at you warmly.";
    }
};

// === TEACHER MELODY ===
class MelodyMonster : public Monster {
public:
    bool quizActive;
    float angerLevel;
    float patienceTimer;

    MelodyMonster(Vector3 spawn) : Monster(MONSTER_MELODY, "Teacher Melody", spawn),
        quizActive(false), angerLevel(0.0f), patienceTimer(10.0f)
    {
        description = "Quizzes you on nature and history. Don't make her wait.";
        speed = 3.0f;
    }

    void Update(float dt, const GameTime& time, Vector3 playerPos) override
    {
        Monster::Update(dt, time, playerPos);
        float dist = DistanceTo(playerPos);

        if (dist < 5.0f && !isHostile)
        {
            state = MONSTER_FRIENDLY;
            patienceTimer -= dt;
            if (patienceTimer <= 0.0f)
            {
                angerLevel += dt * 10.0f;
                if (angerLevel >= 100.0f)
                {
                    isHostile = true;
                    hostTimer = 5.0f;
                }
            }
        }
        else
        {
            patienceTimer = 10.0f;
        }

        if (isHostile)
        {
            state = MONSTER_HOSTILE;
            MoveToward(playerPos, dt);
        }
    }

    void Draw() override
    {
        Color bodyColor = isHostile ? Color{139, 0, 50, 255} : Color{180, 50, 80, 255};
        float bobY = sinf(GetTime() * 1.5f) * 0.03f;

        DrawCube({position.x, position.y + bobY, position.z}, 0.5f, 2.2f, 0.3f, bodyColor);
        DrawCube({position.x, position.y + 1.4f + bobY, position.z}, 0.4f, 0.4f, 0.35f, bodyColor);

        DrawSphere({position.x - 0.12f, position.y + 1.55f + bobY, position.z}, 0.07f, WHITE);
        DrawSphere({position.x + 0.12f, position.y + 1.55f + bobY, position.z}, 0.07f, WHITE);
        DrawSphere({position.x - 0.12f, position.y + 1.55f + bobY, position.z - 0.03f}, 0.035f, BLACK);
        DrawSphere({position.x + 0.12f, position.y + 1.55f + bobY, position.z - 0.03f}, 0.035f, BLACK);

        DrawCube({position.x, position.y + 1.75f + bobY, position.z}, 0.3f, 0.15f, 0.3f, Color{100, 40, 60, 255});

        Vector3 armL = {position.x - 0.4f, position.y + 0.8f + bobY, position.z};
        Vector3 armR = {position.x + 0.4f, position.y + 0.8f + bobY, position.z};
        DrawCylinder(armL, 0.05f, 0.05f, 1.2f, 8, bodyColor);
        DrawCylinder(armR, 0.05f, 0.05f, 1.2f, 8, bodyColor);
    }

    void OnWrongAnswer() { angerLevel += 30.0f; if (angerLevel >= 100.0f) { isHostile = true; hostTimer = 5.0f; } }
    void OnGoodAnswer() { angerLevel = 0.0f; patienceTimer = 10.0f; }

    bool CanHurtPlayer(Vector3 playerPos, const GameTime& time) const override
    {
        return isHostile && DistanceTo(playerPos) < 2.0f;
    }

    std::string GetHint() const override
    {
        if (isHostile) return "Teacher Melody looks FURIOUS!";
        return "Teacher Melody smiles at you. [E] Talk";
    }
};

// === MISS HAWK ===
class HawkMonster : public Monster {
public:
    bool inVent;
    Vector3 ventPosition;

    HawkMonster(Vector3 spawn) : Monster(MONSTER_HAWK, "Miss Hawk", spawn),
        inVent(false), ventPosition{0, 0, 0}
    {
        description = "Security hawk. Friendly during day, deadly at night.";
        speed = 5.0f;
    }

    void Update(float dt, const GameTime& time, Vector3 playerPos) override
    {
        Monster::Update(dt, time, playerPos);

        if (time.IsBetween(11, 0, 16, 0))
        {
            isHostile = false;
            state = MONSTER_PATROL;
            float dist = DistanceTo(playerPos);
            if (dist > 6.0f) MoveToward(playerPos, dt * 0.5f);
        }
        else if (time.IsBetween(16, 30, 17, 0))
        {
            state = MONSTER_STALKING;
            isHostile = false;
        }
        else if (time.IsBetween(18, 0, 19, 15))
        {
            state = MONSTER_HOSTILE;
            isHostile = true;
            inVent = true;
        }
        else
        {
            isHostile = false;
            state = MONSTER_IDLE;
            inVent = false;
        }
    }

    void Draw() override
    {
        if (inVent && isHostile) return;

        float bobY = sinf(GetTime() * 3.0f) * 0.08f;
        Color bodyColor = isHostile ? RED : Color{139, 90, 43, 255};

        DrawCube({position.x, position.y + 0.4f + bobY, position.z}, 0.6f, 0.8f, 0.4f, bodyColor);
        DrawSphere({position.x, position.y + 1.0f + bobY, position.z}, 0.3f, bodyColor);

        DrawCylinder({position.x, position.y + 1.0f + bobY, position.z + 0.3f}, 0.04f, 0.0f, 0.3f, 8, Color{220, 160, 40, 255});

        DrawSphere({position.x - 0.1f, position.y + 1.1f + bobY, position.z}, 0.08f, WHITE);
        DrawSphere({position.x + 0.1f, position.y + 1.1f + bobY, position.z}, 0.08f, WHITE);
        DrawSphere({position.x - 0.1f, position.y + 1.1f + bobY, position.z - 0.03f}, 0.04f, BLACK);
        DrawSphere({position.x + 0.1f, position.y + 1.1f + bobY, position.z - 0.03f}, 0.04f, BLACK);

        DrawCube({position.x, position.y + 1.4f + bobY, position.z}, 0.35f, 0.1f, 0.4f, Color{80, 60, 30, 255});
    }

    bool CanHurtPlayer(Vector3 playerPos, const GameTime& time) const override
    {
        if (!isAlive || !isHostile) return false;
        if (time.IsBetween(18, 0, 19, 15)) return true;
        return false;
    }

    std::string GetHint() const override
    {
        if (isHostile) return "You hear feathers rustling in the vents...";
        if (state == MONSTER_STALKING) return "Something is watching you from the vents.";
        if (state == MONSTER_PATROL) return "Miss Hawk patrols the area. [E] Ask questions";
        return "Miss Hawk is perched nearby.";
    }
};

// === MR JJ ===
class JJMonster : public Monster {
public:
    bool active;

    JJMonster(Vector3 spawn) : Monster(MONSTER_JJ, "Mr. JJ", spawn),
        active(false)
    {
        description = "Extremely hostile cowboy. Lurks in Revolution exhibit at midnight.";
        speed = 6.0f;
    }

    void Update(float dt, const GameTime& time, Vector3 playerPos) override
    {
        Monster::Update(dt, time, playerPos);
        active = time.IsBetween(23, 30, 0, 30);

        if (active)
        {
            isHostile = true;
            state = MONSTER_HOSTILE;
            float dist = DistanceTo(playerPos);
            if (dist < 20.0f) MoveToward(playerPos, dt);
        }
        else
        {
            isHostile = false;
            state = MONSTER_IDLE;
            position = spawnPosition;
        }
    }

    void Draw() override
    {
        if (!active) return;

        float bobY = sinf(GetTime() * 4.0f) * 0.04f;
        DrawCube({position.x, position.y + bobY, position.z}, 0.6f, 1.7f, 0.4f, Color{60, 30, 10, 255});
        DrawCube({position.x, position.y + 1.2f + bobY, position.z}, 0.5f, 0.5f, 0.4f, Color{60, 30, 10, 255});

        DrawSphere({position.x - 0.13f, position.y + 1.35f + bobY, position.z}, 0.07f, YELLOW);
        DrawSphere({position.x + 0.13f, position.y + 1.35f + bobY, position.z}, 0.07f, YELLOW);
        DrawSphere({position.x - 0.13f, position.y + 1.35f + bobY, position.z - 0.03f}, 0.035f, BLACK);
        DrawSphere({position.x + 0.13f, position.y + 1.35f + bobY, position.z - 0.03f}, 0.035f, BLACK);

        DrawCube({position.x, position.y + 1.6f + bobY, position.z}, 0.7f, 0.15f, 0.5f, Color{80, 50, 20, 255});
    }

    bool CanHurtPlayer(Vector3 playerPos, const GameTime& time) const override
    {
        return active && isHostile && DistanceTo(playerPos) < 2.0f;
    }

    std::string GetHint() const override
    {
        return active ? "A dark figure watches you from the shadows..." : "";
    }
};

// === JOHN AND JEFF ===
class JohnJeffMonster : public Monster {
public:
    int jinglePressCount;
    bool triggered;

    JohnJeffMonster(Vector3 spawn) : Monster(MONSTER_JOHN_JEFF, "John & Jeff", spawn),
        jinglePressCount(0), triggered(false)
    {
        description = "Two static figures in a pickup truck. Don't press the button too many times.";
        speed = 3.0f;
    }

    void PressJingle()
    {
        jinglePressCount++;
        if (jinglePressCount >= 15 && !triggered)
        {
            triggered = true;
            isHostile = true;
            hostTimer = 8.0f;
            state = MONSTER_HOSTILE;
        }
    }

    void Update(float dt, const GameTime& time, Vector3 playerPos) override
    {
        Monster::Update(dt, time, playerPos);
        if (triggered && isHostile) MoveToward(playerPos, dt);
        if (hostTimer <= 0.0f) { triggered = false; jinglePressCount = 0; }
    }

    void Draw() override
    {
        float bobY = triggered ? sinf(GetTime() * 6.0f) * 0.1f : 0.0f;
        Color bodyColor = triggered ? RED : Color{100, 80, 60, 255};

        DrawCube({position.x - 0.4f, position.y + bobY, position.z}, 0.4f, 1.5f, 0.3f, bodyColor);
        DrawCube({position.x + 0.4f, position.y + bobY, position.z}, 0.4f, 1.5f, 0.3f, bodyColor);

        DrawCube({position.x - 0.4f, position.y + 1.1f + bobY, position.z}, 0.35f, 0.4f, 0.3f, bodyColor);
        DrawCube({position.x + 0.4f, position.y + 1.1f + bobY, position.z}, 0.35f, 0.4f, 0.3f, bodyColor);

        DrawCube({position.x - 0.4f, position.y + 1.3f + bobY, position.z - 0.3f}, 0.1f, 0.4f, 0.3f, bodyColor);
        DrawCube({position.x + 0.4f, position.y + 1.3f + bobY, position.z - 0.3f}, 0.1f, 0.4f, 0.3f, bodyColor);
    }

    bool CanHurtPlayer(Vector3 playerPos, const GameTime& time) const override
    {
        return triggered && isHostile && DistanceTo(playerPos) < 2.0f;
    }

    std::string GetHint() const override
    {
        if (triggered) return "John & Jeff's necks are EXTENDING!";
        return "Two static figures sit in a pickup truck. [E] Press jingle button";
    }
};

// === MISS INDIANA ===
class IndianaMonster : public Monster {
public:
    IndianaMonster(Vector3 spawn) : Monster(MONSTER_INDIANA, "Miss Indiana", spawn)
    {
        description = "Always nice. Answers questions about exhibits. Dies saving you.";
        speed = 0.0f;
        isHostile = false;
    }

    void Update(float dt, const GameTime& time, Vector3 playerPos) override
    {
        Monster::Update(dt, time, playerPos);
        state = MONSTER_FRIENDLY;
    }

    void Draw() override
    {
        float bobY = sinf(GetTime() * 1.0f) * 0.02f;
        DrawCube({position.x, position.y + bobY, position.z}, 0.5f, 1.6f, 0.35f, Color{160, 82, 45, 255});
        DrawCube({position.x, position.y + 1.1f + bobY, position.z}, 0.4f, 0.45f, 0.35f, Color{210, 180, 140, 255});

        DrawSphere({position.x - 0.12f, position.y + 1.25f + bobY, position.z}, 0.07f, BLACK);
        DrawSphere({position.x + 0.12f, position.y + 1.25f + bobY, position.z}, 0.07f, BLACK);
    }

    bool CanHurtPlayer(Vector3 playerPos, const GameTime& time) const override { return false; }

    std::string GetHint() const override
    {
        return "Miss Indiana smiles kindly. [E] Ask about exhibits";
    }
};

// === OLD GUY PATRIOT ===
class PatriotMonster : public Monster {
public:
    PatriotMonster(Vector3 spawn) : Monster(MONSTER_PATRIOT, "Old Guy Patriot", spawn)
    {
        description = "Enforces American Revolution exhibit rules. Armed with musket.";
        speed = 2.5f;
    }

    void Update(float dt, const GameTime& time, Vector3 playerPos) override
    {
        Monster::Update(dt, time, playerPos);
        state = MONSTER_PATROL;
        float dist = DistanceTo(playerPos);
        if (dist > 8.0f) MoveToward(playerPos, dt * 0.4f);
    }

    void Draw() override
    {
        float bobY = sinf(GetTime() * 1.5f) * 0.02f;
        DrawCube({position.x, position.y + bobY, position.z}, 0.5f, 1.7f, 0.35f, Color{30, 50, 80, 255});
        DrawCube({position.x, position.y + 1.2f + bobY, position.z}, 0.45f, 0.45f, 0.38f, Color{30, 50, 80, 255});

        DrawSphere({position.x - 0.12f, position.y + 1.35f + bobY, position.z}, 0.07f, WHITE);
        DrawSphere({position.x + 0.12f, position.y + 1.35f + bobY, position.z}, 0.07f, WHITE);

        DrawCube({position.x + 0.4f, position.y + 0.8f + bobY, position.z}, 0.05f, 1.5f, 0.05f, Color{80, 60, 30, 255});
    }

    bool CanHurtPlayer(Vector3 playerPos, const GameTime& time) const override
    {
        return isHostile && DistanceTo(playerPos) < 2.0f;
    }

    std::string GetHint() const override
    {
        return "Old Guy Patriot guards the exhibit. Follow the rules!";
    }
};

// === POTTY THE PARROT ===
class PottyMonster : public Monster {
public:
    float violenceTimer;
    bool yapping;

    PottyMonster(Vector3 spawn) : Monster(MONSTER_POTTY, "Potty the Parrot", spawn),
        violenceTimer(0.0f), yapping(true)
    {
        description = "Loves to yap and fight with Miss Hawk. Violent at random times.";
        speed = 3.5f;
    }

    void Update(float dt, const GameTime& time, Vector3 playerPos) override
    {
        Monster::Update(dt, time, playerPos);

        violenceTimer -= dt;
        if (violenceTimer <= 0.0f)
        {
            violenceTimer = 30.0f + (float)(GetRandomValue(0, 60));
            isHostile = !isHostile;
            if (isHostile) hostTimer = 5.0f + (float)(GetRandomValue(0, 5));
        }

        if (isHostile)
        {
            state = MONSTER_HOSTILE;
            MoveToward(playerPos, dt);
        }
        else
        {
            state = MONSTER_IDLE;
        }
    }

    void Draw() override
    {
        float bobY = sinf(GetTime() * 5.0f) * 0.1f;
        Color bodyColor = isHostile ? RED : Color{0, 150, 0, 255};

        DrawCube({position.x, position.y + 0.3f + bobY, position.z}, 0.3f, 0.5f, 0.25f, bodyColor);
        DrawSphere({position.x, position.y + 0.7f + bobY, position.z}, 0.2f, bodyColor);

        DrawCylinder({position.x, position.y + 0.7f + bobY, position.z + 0.2f}, 0.03f, 0.0f, 0.2f, 8, Color{255, 200, 0, 255});

        DrawSphere({position.x - 0.08f, position.y + 0.78f + bobY, position.z}, 0.05f, WHITE);
        DrawSphere({position.x + 0.08f, position.y + 0.78f + bobY, position.z}, 0.05f, WHITE);
        DrawSphere({position.x - 0.08f, position.y + 0.78f + bobY, position.z - 0.02f}, 0.025f, BLACK);
        DrawSphere({position.x + 0.08f, position.y + 0.78f + bobY, position.z - 0.02f}, 0.025f, BLACK);

        DrawCube({position.x - 0.2f, position.y + 0.4f + bobY, position.z - 0.1f}, 0.03f, 0.4f, 0.2f, bodyColor);
        DrawCube({position.x + 0.2f, position.y + 0.4f + bobY, position.z - 0.1f}, 0.03f, 0.4f, 0.2f, bodyColor);
    }

    bool CanHurtPlayer(Vector3 playerPos, const GameTime& time) const override
    {
        return isHostile && DistanceTo(playerPos) < 1.5f;
    }

    std::string GetHint() const override
    {
        return yapping ? "Potty squawks and flaps his wings." : "Potty stares at you...";
    }
};

// === MONSTER MANAGER ===
void MonsterManager::Init(const DataLoader& loader)
{
    MonsterData fz = loader.GetMonsterData("fuzzy");
    MonsterData ml = loader.GetMonsterData("melody");
    MonsterData hk = loader.GetMonsterData("miss_hawk");
    MonsterData jj = loader.GetMonsterData("jj");
    MonsterData jj2 = loader.GetMonsterData("john_jeff");
    MonsterData in = loader.GetMonsterData("indiana");
    MonsterData pt = loader.GetMonsterData("patriot");
    MonsterData po = loader.GetMonsterData("polly");

    monsters.push_back(new FuzzyMonster(fz.spawn));
    monsters.push_back(new MelodyMonster(ml.spawn));
    monsters.push_back(new HawkMonster(hk.spawn));
    monsters.push_back(new JJMonster(jj.spawn));
    monsters.push_back(new JohnJeffMonster(jj2.spawn));
    monsters.push_back(new IndianaMonster(in.spawn));
    monsters.push_back(new PatriotMonster(pt.spawn));
    monsters.push_back(new PottyMonster(po.spawn));
}

void MonsterManager::Update(float dt, const GameTime& time, Vector3 playerPos)
{
    for (auto* m : monsters) m->Update(dt, time, playerPos);
}

void MonsterManager::DrawAll()
{
    for (auto* m : monsters) m->Draw();
}

void MonsterManager::Cleanup()
{
    for (auto* m : monsters) delete m;
    monsters.clear();
}

Monster* MonsterManager::GetClosestMonster(Vector3 pos, float maxDist) const
{
    Monster* closest = nullptr;
    float minDist = maxDist;
    for (auto* m : monsters)
    {
        float d = m->DistanceTo(pos);
        if (d < minDist) { minDist = d; closest = m; }
    }
    return closest;
}

bool MonsterManager::IsAnyMonsterHostile() const
{
    for (auto* m : monsters) if (m->isHostile) return true;
    return false;
}
