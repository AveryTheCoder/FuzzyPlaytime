#include "include/save_load.h"
#include "raylib.h"
#include <cstdio>

bool SaveLoad::SaveGame(const Player& player, const std::string& filepath)
{
    FILE* file = fopen(filepath.c_str(), "w");
    if (!file) return false;

    fprintf(file, "health=%.1f\n", player.health);
    fprintf(file, "pos_x=%.2f\n", player.position.x);
    fprintf(file, "pos_y=%.2f\n", player.position.y);
    fprintf(file, "pos_z=%.2f\n", player.position.z);
    fprintf(file, "sprinting=%d\n", player.isSprinting ? 1 : 0);

    fclose(file);
    return true;
}

bool SaveLoad::LoadGame(Player& player, const std::string& filepath)
{
    FILE* file = fopen(filepath.c_str(), "r");
    if (!file) return false;

    float hx, px, py, pz;
    int sprinting;

    fscanf(file, "health=%f\n", &hx);
    fscanf(file, "pos_x=%f\n", &px);
    fscanf(file, "pos_y=%f\n", &py);
    fscanf(file, "pos_z=%f\n", &pz);
    fscanf(file, "sprinting=%d\n", &sprinting);

    player.health = hx;
    player.position = {px, py, pz};
    player.isSprinting = sprinting != 0;

    fclose(file);
    return true;
}
