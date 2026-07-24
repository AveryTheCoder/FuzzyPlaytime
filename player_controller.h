#pragma once

#include "raylib.h"
#include "player.h"

class PlayerController {
public:
    PlayerController(Player& player);

    void HandleInput(float deltaTime);
    void Update(float deltaTime);
    void ProcessMovement(float deltaTime);
    void ProcessSprint();

private:
    Player& player;
    Camera3D& camera;
};
