#include <string>
#include <vector>
#include <cmath>
#include <cstdio>

#include "include/player.h"
#include "include/player_controller.h"
#include "include/save_load.h"
#include "include/sound.h"
#include "include/shaders.h"
#include "include/game_time.h"
#include "include/monster.h"
#include "include/computer.h"
#include "include/dialogue.h"
#include "include/level.h"
#include "include/story.h"
#include "include/data_loader.h"
#include "raymath.h"

enum GameState {
    MENU,
    PLAYING,
    SETTINGS,
    PAUSED,
    COMPUTER
};

struct Game {
    GameState state = MENU;
    bool running = true;
    int screenWidth = 1280;
    int screenHeight = 720;
    Player player;
    Camera3D camera;
    float cameraYaw = 0.0f;
    float cameraPitch = 0.0f;
    SoundManager soundManager;
    Texture2D crosshairTexture = {0};
    Texture2D menuBackground = {0};
    Model computerModel = {0};
    float masterVolume = 1.0f;
    int selectedMenuItem = 0;
    float menuCooldown = 0.0f;

    GameTime gameTime;
    MonsterManager monsters;
    Computer computer;
    DialogueSystem dialogue;
    Level level;
    StoryManager story;
    DataLoader dataLoader;
};

void InitGameState(Game& game);
void UpdateMenu(Game& game, float dt);
void DrawMenu(Game& game);
void UpdateGameplay(Game& game, float dt);
void DrawGameplay(Game& game);
void UpdateSettings(Game& game, float dt);
void DrawSettings(Game& game);
void UpdatePaused(Game& game, float dt);
void DrawPaused(Game& game);
void DrawEnvironment(Game& game);
void DrawHUD(Game& game);

Texture2D GenerateCrosshairTexture()
{
    Image img = GenImageColor(32, 32, BLANK);
    ImageDrawLine(&img, 16, 8, 16, 24, WHITE);
    ImageDrawLine(&img, 8, 16, 24, 16, WHITE);
    ImageDrawCircle(&img, 16, 16, 2, WHITE);
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}

int main(void)
{
    Game game;

    InitWindow(game.screenWidth, game.screenHeight, "Fuzzy Playtime");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    game.crosshairTexture = GenerateCrosshairTexture();
    game.menuBackground = LoadTexture("src/assets/img/UI/main_menu_background.png");
    game.computerModel = LoadModel("src/assets/models/obj/MainComputer.glb");
    game.computerModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = Color{180, 160, 140, 255};
    game.soundManager.Init();
    game.dataLoader.Load("src/data/game_data.json");
    game.computer.Init();
    game.computer.LoadPasswords(game.dataLoader);
    game.monsters.Init(game.dataLoader);
    game.level.Init();
    game.story.Init();

    while (!WindowShouldClose() && game.running)
    {
        float dt = GetFrameTime();

        switch (game.state)
        {
            case MENU:
                UpdateMenu(game, dt);
                DrawMenu(game);
                break;
            case PLAYING:
                UpdateGameplay(game, dt);
                DrawGameplay(game);
                break;
            case SETTINGS:
                UpdateSettings(game, dt);
                DrawSettings(game);
                break;
            case PAUSED:
                UpdatePaused(game, dt);
                DrawPaused(game);
                break;
            case COMPUTER:
                UpdateGameplay(game, dt);
                DrawGameplay(game);
                game.computer.Update(dt);
                game.computer.Draw();
                break;
        }
    }

    UnloadTexture(game.crosshairTexture);
    UnloadTexture(game.menuBackground);
    UnloadModel(game.computerModel);
    CloseWindow();
    return 0;
}

void InitGameState(Game& game)
{
    game.player = Player();
    game.camera.position = {0.0f, 1.7f, 0.0f};
    game.camera.target = {0.0f, 1.7f, -1.0f};
    game.camera.up = {0.0f, 1.0f, 0.0f};
    game.camera.fovy = 70.0f;
    game.camera.projection = CAMERA_PERSPECTIVE;
    game.cameraYaw = 0.0f;
    game.cameraPitch = 0.0f;

    game.gameTime = GameTime();
    game.monsters.Init(game.dataLoader);
    game.computer.Init();
    game.computer.LoadPasswords(game.dataLoader);
    game.dialogue = DialogueSystem();
    game.level.Init();
    game.story.Init();
}

void UpdateMenu(Game& game, float dt)
{
    ShowCursor();
    game.menuCooldown -= dt;
    if (game.menuCooldown < 0.0f) game.menuCooldown = 0.0f;

    if (game.menuCooldown <= 0.0f)
    {
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            game.selectedMenuItem = (game.selectedMenuItem + 1) % 3;
            game.menuCooldown = 0.2f;
        }
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            game.selectedMenuItem = (game.selectedMenuItem + 2) % 3;
            game.menuCooldown = 0.2f;
        }
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            game.menuCooldown = 0.3f;
            switch (game.selectedMenuItem)
            {
                case 0:
                    InitGameState(game);
                    game.state = PLAYING;
                    HideCursor();
                    break;
                case 1:
                    game.state = SETTINGS;
                    break;
                case 2:
                    game.running = false;
                    break;
            }
        }
    }

    for (int i = 0; i < 3; i++)
    {
        int btnW = 300, btnH = 50;
        int btnX = game.screenWidth / 2 - btnW / 2;
        int btnY = 220 + i * 70;
        if (CheckCollisionPointRec(GetMousePosition(), {(float)btnX, (float)btnY, (float)btnW, (float)btnH}))
        {
            game.selectedMenuItem = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                switch (i)
                {
                    case 0:
                        InitGameState(game);
                        game.state = PLAYING;
                        HideCursor();
                        break;
                    case 1: game.state = SETTINGS; break;
                    case 2: game.running = false; break;
                }
            }
        }
    }
}

void DrawMenu(Game& game)
{
    BeginDrawing();
    ClearBackground(BLACK);

    if (game.menuBackground.id > 0)
    {
        DrawTexturePro(game.menuBackground,
            {0, 0, (float)game.menuBackground.width, (float)game.menuBackground.height},
            {0, 0, (float)game.screenWidth, (float)game.screenHeight},
            {0, 0}, 0.0f, WHITE);
    }

    DrawRectangle(0, 0, game.screenWidth, game.screenHeight, Color{0, 0, 0, 120});

    const char* title = "FUZZY PLAYTIME";
    int titleW = MeasureText(title, 48);
    DrawText(title, game.screenWidth / 2 - titleW / 2, 60, 48, RED);

    const char* subtitle = "The NCFEACL Experience";
    int subW = MeasureText(subtitle, 20);
    DrawText(subtitle, game.screenWidth / 2 - subW / 2, 120, 20, WHITE);

    const char* buttons[] = {"Start", "Settings", "Exit"};

    for (int i = 0; i < 3; i++)
    {
        int btnW = 300, btnH = 50;
        int btnX = game.screenWidth / 2 - btnW / 2;
        int btnY = 220 + i * 70;

        bool hovered = CheckCollisionPointRec(GetMousePosition(), {(float)btnX, (float)btnY, (float)btnW, (float)btnH});
        bool selected = (i == game.selectedMenuItem);

        Color bg = (hovered || selected) ? Color{180, 0, 0, 255} : Color{40, 40, 40, 200};
        Color fg = (hovered || selected) ? YELLOW : WHITE;

        DrawRectangle(btnX, btnY, btnW, btnH, bg);
        DrawRectangleLines(btnX, btnY, btnW, btnH, WHITE);
        int textW = MeasureText(buttons[i], 24);
        DrawText(buttons[i], game.screenWidth / 2 - textW / 2, btnY + 12, 24, fg);
    }

    const char* footer = "Fuzzy Playtime 2026 - All rights reserved";
    int footerW = MeasureText(footer, 14);
    DrawText(footer, game.screenWidth / 2 - footerW / 2, game.screenHeight - 30, 14, GRAY);

    EndDrawing();
}

void UpdateGameplay(Game& game, float dt)
{
    if (game.dialogue.active)
    {
        game.dialogue.Update(dt);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mp = GetMousePosition();
            game.dialogue.OnMouseClick((int)mp.x, (int)mp.y);
        }
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
        {
            game.dialogue.OnKeyDown(KEY_SPACE);
        }
        if (game.dialogue.IsFinished()) game.dialogue.active = false;
        return;
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (game.state == COMPUTER)
        {
            game.state = PLAYING;
            HideCursor();
        }
        else
        {
            game.state = PAUSED;
            ShowCursor();
        }
        return;
    }

    game.gameTime.Update(dt);
    game.monsters.Update(dt, game.gameTime, game.camera.position);
    game.story.Update(dt, game.gameTime.GetHourFloat());

    game.level.CheckInteractions(game.camera.position);

    if (IsKeyPressed(KEY_E))
    {
        Interactable* closest = game.level.GetClosestInteractable(game.camera.position, 3.0f);
        if (closest)
        {
            if (closest->id == 0)
            {
                game.state = COMPUTER;
                game.computer.active = true;
                ShowCursor();
                return;
            }
            else if (closest->id == 1)
            {
                game.dialogue.StartMelodyQuiz();
            }
            else if (closest->id == 2)
            {
                game.dialogue.StartHawkDialogue();
            }
            else if (closest->id == 3)
            {
                game.dialogue.StartIndianaDialogue();
            }
            else if (closest->id == 4)
            {
                game.story.OnJinglePressed();
            }
            else if (closest->id == 5)
            {
                game.story.OnBadBehavior();
            }
            else if (closest->id == 6)
            {
                game.story.OnPatriotTriggered();
            }
        }
    }

    Vector2 mouseDelta = GetMouseDelta();
    game.cameraYaw   -= mouseDelta.x * 0.003f;
    game.cameraPitch -= mouseDelta.y * 0.003f;
    if (game.cameraPitch >  89.0f * DEG2RAD) game.cameraPitch =  89.0f * DEG2RAD;
    if (game.cameraPitch < -89.0f * DEG2RAD) game.cameraPitch = -89.0f * DEG2RAD;

    Vector3 forward = {
        cosf(game.cameraPitch) * sinf(game.cameraYaw),
        sinf(game.cameraPitch),
        cosf(game.cameraPitch) * cosf(game.cameraYaw)
    };
    game.camera.target = Vector3Add(game.camera.position, forward);
    game.player.position = game.camera.position;

    float speed = IsKeyDown(KEY_LEFT_SHIFT) ? game.player.sprintSpeed : game.player.walkSpeed;
    Vector3 moveDir = {0, 0, 0};
    if (IsKeyDown(KEY_W)) moveDir.z += 1.0f;
    if (IsKeyDown(KEY_S)) moveDir.z -= 1.0f;
    if (IsKeyDown(KEY_A)) moveDir.x -= 1.0f;
    if (IsKeyDown(KEY_D)) moveDir.x += 1.0f;

    float len = sqrtf(moveDir.x * moveDir.x + moveDir.z * moveDir.z);
    if (len > 0.0f)
    {
        moveDir.x /= len;
        moveDir.z /= len;

        Vector3 forw = Vector3Subtract(game.camera.target, game.camera.position);
        forw.y = 0;
        forw = Vector3Normalize(forw);
        Vector3 right = Vector3CrossProduct(forw, game.camera.up);

        Vector3 movement = {0, 0, 0};
        movement.x = (forw.x * moveDir.z + right.x * moveDir.x) * speed * dt;
        movement.z = (forw.z * moveDir.z + right.z * moveDir.x) * speed * dt;

        game.camera.position.x += movement.x;
        game.camera.position.z += movement.z;
        game.camera.target.x += movement.x;
        game.camera.target.z += movement.z;
    }

    if (game.player.health <= 0)
    {
        game.player.Die();
        game.state = MENU;
        ShowCursor();
    }
}

void DrawGameplay(Game& game)
{
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(game.camera);
    DrawEnvironment(game);
    DrawModel(game.computerModel, {3, 0.0f, 8}, 0.01f, WHITE);
    game.monsters.DrawAll();
    EndMode3D();

    DrawHUD(game);

    if (game.dialogue.active)
    {
        game.dialogue.Draw();
    }

    if (game.state == PLAYING)
    {
        Interactable* closest = game.level.GetClosestInteractable(game.camera.position, 3.0f);
        if (closest && closest->inRange)
        {
            int hintW = MeasureText(closest->hint.c_str(), 16);
            DrawText(closest->hint.c_str(), game.screenWidth / 2 - hintW / 2, game.screenHeight - 40, 16, YELLOW);
        }
    }

    EndDrawing();
}

void DrawEnvironment(Game& game)
{
    game.level.Draw();
}

void DrawHUD(Game& game)
{
    int barW = 200;
    int barH = 15;
    int barX = 10;
    int barY = 10;

    DrawRectangle(barX, barY, barW + 4, barH + 4, BLACK);
    DrawRectangle(barX + 2, barY + 2, barW, barH, Color{40, 40, 40, 255});
    DrawRectangle(barX + 2, barY + 2, (int)(barW * (game.player.health / game.player.maxHealth)), barH, RED);

    const char* hpText = TextFormat("HP: %.0f", game.player.health);
    DrawText(hpText, barX + 6, barY + 2, 12, WHITE);

    const char* timeStr = game.gameTime.GetTimeString().c_str();
    DrawText(timeStr, game.screenWidth - 120, 12, 16, WHITE);

    const char* dayStr = TextFormat("Day %d", game.gameTime.GetDay());
    DrawText(dayStr, game.screenWidth - 120, 30, 14, GRAY);

    const char* storyHint = game.story.GetHint().c_str();
    DrawText(storyHint, 10, game.screenHeight - 60, 14, Color{200, 200, 100, 200});

    if (game.crosshairTexture.id > 0)
    {
        int cx = game.screenWidth / 2 - game.crosshairTexture.width / 2;
        int cy = game.screenHeight / 2 - game.crosshairTexture.height / 2;
        DrawTexture(game.crosshairTexture, cx, cy, WHITE);
    }
    else
    {
        int cx = game.screenWidth / 2;
        int cy = game.screenHeight / 2;
        DrawLine(cx - 8, cy, cx + 8, cy, WHITE);
        DrawLine(cx, cy - 8, cx, cy + 8, WHITE);
        DrawCircle(cx, cy, 2, WHITE);
    }
}

void UpdateSettings(Game& game, float dt)
{
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        game.masterVolume -= dt * 0.5f;
        if (game.masterVolume < 0.0f) game.masterVolume = 0.0f;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        game.masterVolume += dt * 0.5f;
        if (game.masterVolume > 1.0f) game.masterVolume = 1.0f;
    }
    if (IsKeyPressed(KEY_ESCAPE))
    {
        game.state = MENU;
        return;
    }
}

void DrawSettings(Game& game)
{
    BeginDrawing();
    ClearBackground(BLACK);

    const char* title = "SETTINGS";
    int titleW = MeasureText(title, 36);
    DrawText(title, game.screenWidth / 2 - titleW / 2, 40, 36, WHITE);

    DrawText("Master Volume", game.screenWidth / 2 - 60, 150, 20, WHITE);
    DrawRectangle(game.screenWidth / 2 - 100, 185, 200, 20, GRAY);
    DrawRectangle(game.screenWidth / 2 - 100, 185, (int)(200 * game.masterVolume), 20, RED);
    const char* volPct = TextFormat("%.0f%%", game.masterVolume * 100);
    DrawText(volPct, game.screenWidth / 2 + 110, 185, 20, WHITE);

    DrawText("[A/D] Adjust Volume", game.screenWidth / 2 - 80, 230, 16, GRAY);
    DrawText("[ESC] Back", game.screenWidth / 2 - 40, 270, 16, GRAY);

    EndDrawing();
}

void UpdatePaused(Game& game, float dt)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        game.state = PLAYING;
        HideCursor();
        return;
    }
}

void DrawPaused(Game& game)
{
    BeginMode3D(game.camera);
    DrawEnvironment(game);
    DrawModel(game.computerModel, {3, 0.0f, 8}, 0.01f, WHITE);
    game.monsters.DrawAll();
    EndMode3D();

    DrawRectangle(0, 0, game.screenWidth, game.screenHeight, Color{0, 0, 0, 160});

    const char* title = "PAUSED";
    int titleW = MeasureText(title, 48);
    DrawText(title, game.screenWidth / 2 - titleW / 2, game.screenHeight / 2 - 60, 48, WHITE);

    const char* hint = "Press ESC to resume";
    int hintW = MeasureText(hint, 20);
    DrawText(hint, game.screenWidth / 2 - hintW / 2, game.screenHeight / 2 + 10, 20, GRAY);
}

Player::Player()
    : health(100.0f), maxHealth(100.0f), walkSpeed(5.0f), sprintSpeed(8.0f),
      isSprinting(false), isAlive(true), position{0, 1.7f, 0}, velocity{0, 0, 0},
      inventoryFull(false), maxInventorySlots(10)
{
    equippedItem = {"", "", -1};
}

void Player::TakeDamage(float amount)
{
    if (!isAlive) return;
    health -= amount;
    if (health < 0.0f) health = 0.0f;
    if (health == 0.0f) Die();
}

void Player::Heal(float amount)
{
    if (!isAlive) return;
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

void Player::Die()
{
    isAlive = false;
    health = 0.0f;
}

bool Player::IsInventoryFull() const
{
    return (int)inventory.size() >= maxInventorySlots;
}

bool Player::AddToInventory(const InventoryItem& item)
{
    if (IsInventoryFull()) return false;
    inventory.push_back(item);
    inventoryFull = IsInventoryFull();
    return true;
}

void Player::Update(float deltaTime)
{
    if (!isAlive) return;
    isSprinting = IsKeyDown(KEY_LEFT_SHIFT);
}
