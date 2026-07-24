#include "include/level.h"
#include "raymath.h"
#include <cmath>

void Level::Init()
{
    rooms.push_back({{-12, 0, -12}, {12, 4, 12}, "Main Hall"});

    vents.push_back({{-6, 0.5f, -2}, 1.5f, false});
    vents.push_back({{6, 0.5f, -2}, 1.5f, false});
    vents.push_back({{-2, 0.5f, 6}, 1.5f, false});
    vents.push_back({{4, 0.5f, -6}, 1.5f, false});
    vents.push_back({{-8, 0.5f, 4}, 1.5f, false});

    interactables.push_back({{3, 1.0f, 8}, 2.5f, "Computer", "[E] Use Computer", false, 0});
    interactables.push_back({{-5, 1.0f, -6}, 3.0f, "Teacher Melody", "[E] Talk to Melody", false, 1});
    interactables.push_back({{8, 1.0f, 3}, 3.0f, "Miss Hawk", "[E] Talk to Hawk", false, 2});
    interactables.push_back({{-3, 1.0f, 3}, 3.0f, "Miss Indiana", "[E] Talk to Indiana", false, 3});
    interactables.push_back({{6, 1.0f, -8}, 2.5f, "John & Jeff", "[E] Press Jingle", false, 4});
    interactables.push_back({{3, 1.0f, -6}, 2.5f, "Potty", "[E] Interact", false, 5});
    interactables.push_back({{-8, 1.0f, -3}, 3.0f, "Patriot", "[E] Interact", false, 6});
}

void Level::Draw()
{
    DrawGrid(40, 1.0f);

    DrawCube({0, 2.0f, -12}, 24, 4, 0.3f, Color{60, 60, 60, 255});
    DrawCube({0, 2.0f, 12}, 24, 4, 0.3f, Color{60, 60, 60, 255});
    DrawCube({-12, 2.0f, 0}, 0.3f, 4, 24, Color{60, 60, 60, 255});
    DrawCube({12, 2.0f, 0}, 0.3f, 4, 24, Color{60, 60, 60, 255});

    for (auto& v : vents)
    {
        Color ventColor = v.isDangerous ? Color{200, 0, 0, 180} : Color{100, 100, 100, 200};
        DrawCube({v.position.x, 0.02f, v.position.z}, 2.0f, 0.04f, 2.0f, ventColor);

        for (int i = 0; i < 5; i++)
        {
            float x = v.position.x - 0.8f + i * 0.4f;
            DrawCube({x, 0.03f, v.position.z}, 0.05f, 0.02f, 1.5f, Color{70, 70, 70, 255});
        }
    }

    DrawCube({-5, 0.5f, 5}, 3, 1, 3, Color{80, 80, 80, 255});

    DrawCube({0, 1.5f, -5}, 0.3f, 3, 0.3f, Color{60, 40, 20, 255});
    DrawCube({0, 0.02f, -5}, 2.0f, 0.04f, 2.0f, Color{100, 70, 30, 255});

    DrawCube({8, 0.02f, -3}, 1.5f, 0.04f, 1.5f, Color{139, 69, 19, 255});
    DrawCube({8, 0.8f, -3}, 0.1f, 1.6f, 0.1f, Color{101, 67, 33, 255});

    DrawCube({-8, 0.02f, 8}, 4, 0.04f, 2, Color{50, 50, 50, 255});
    DrawCube({-8, 0.5f, 8.8f}, 4, 1, 0.1f, Color{40, 40, 40, 255});
    DrawCube({-8, 0.5f, 7.2f}, 4, 1, 0.1f, Color{40, 40, 40, 255});
    DrawCube({-10, 0.5f, 8}, 0.1f, 1, 2, Color{40, 40, 40, 255});
    DrawCube({-6, 0.5f, 8}, 0.1f, 1, 2, Color{40, 40, 40, 255});

    DrawCube({6, 0.02f, -8}, 3, 0.04f, 2, Color{139, 90, 43, 255});
    DrawCube({5, 0.4f, -8.5f}, 1, 0.8f, 0.5f, Color{100, 70, 30, 255});
    DrawCube({7, 0.4f, -8.5f}, 1, 0.8f, 0.5f, Color{100, 70, 30, 255});

    DrawCube({0, 3.0f, -12}, 10, 1.5f, 0.1f, Color{180, 0, 0, 255});

    DrawCube({-5, 0, -9}, 0.2f, 1.8f, 0.2f, Color{60, 40, 20, 255});
    DrawCube({-5, 1.8f, -9}, 1.5f, 0.1f, 0.8f, Color{80, 50, 20, 255});

    DrawCube({9, 0, 6}, 0.2f, 1.8f, 0.2f, Color{60, 40, 20, 255});
    DrawCube({9, 1.8f, 6}, 1.5f, 0.1f, 0.8f, Color{80, 50, 20, 255});
}

void Level::CheckInteractions(Vector3 playerPos)
{
    for (auto& inter : interactables)
    {
        float dx = playerPos.x - inter.position.x;
        float dz = playerPos.z - inter.position.z;
        float dist = sqrtf(dx * dx + dz * dz);
        inter.inRange = (dist < inter.radius);
    }
}

Interactable* Level::GetClosestInteractable(Vector3 playerPos, float maxDist)
{
    Interactable* closest = nullptr;
    float minDist = maxDist;
    for (auto& inter : interactables)
    {
        float dx = playerPos.x - inter.position.x;
        float dz = playerPos.z - inter.position.z;
        float dist = sqrtf(dx * dx + dz * dz);
        if (dist < minDist) { minDist = dist; closest = &inter; }
    }
    return closest;
}

bool Level::IsInVentZone(Vector3 playerPos) const
{
    for (const auto& v : vents)
    {
        float dx = playerPos.x - v.position.x;
        float dz = playerPos.z - v.position.z;
        float dist = sqrtf(dx * dx + dz * dz);
        if (dist < v.radius) return true;
    }
    return false;
}

bool Level::IsInVentDanger(Vector3 playerPos, float gameHour) const
{
    if (gameHour >= 18.0f && gameHour < 19.25f)
    {
        return IsInVentZone(playerPos);
    }
    return false;
}
