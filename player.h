#pragma once

#include "raylib.h"
#include <string>
#include <vector>

struct InventoryItem {
    std::string name;
    std::string description;
    int id;
};

class Player {
public:
    float health;
    float maxHealth;
    float walkSpeed;
    float sprintSpeed;
    bool isSprinting;
    bool isAlive;
    Vector3 position;
    Vector3 velocity;
    std::vector<InventoryItem> inventory;
    bool inventoryFull;
    int maxInventorySlots;
    InventoryItem equippedItem;

    Player();
    void TakeDamage(float amount);
    void Heal(float amount);
    void Die();
    bool IsInventoryFull() const;
    bool AddToInventory(const InventoryItem& item);
    void Update(float deltaTime);
};
