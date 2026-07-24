#include "include/game_time.h"
#include <cstdio>

GameTime::GameTime() : gameHour(8), gameMinute(0), gameDay(1), accumulator(0.0f) {}

void GameTime::Update(float realDeltaTime)
{
    accumulator += realDeltaTime;
    while (accumulator >= 1.0f)
    {
        accumulator -= 1.0f;
        gameMinute++;
        if (gameMinute >= 60)
        {
            gameMinute = 0;
            gameHour++;
            if (gameHour >= 24)
            {
                gameHour = 0;
                gameDay++;
            }
        }
    }
}

void GameTime::SetStartHour(int hour, int minute)
{
    gameHour = hour;
    gameMinute = minute;
    accumulator = 0.0f;
}

float GameTime::GetHourFloat() const
{
    return (float)gameHour + (float)gameMinute / 60.0f;
}

std::string GameTime::GetTimeString() const
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d", gameHour, gameMinute);
    return std::string(buf);
}

std::string GameTime::GetPeriodString() const
{
    if (gameHour >= 5 && gameHour < 12) return "Morning";
    if (gameHour >= 12 && gameHour < 17) return "Afternoon";
    if (gameHour >= 17 && gameHour < 20) return "Evening";
    return "Night";
}

int GameTime::GetDay() const
{
    return gameDay;
}

bool GameTime::IsBetween(int h1, int m1, int h2, int m2) const
{
    int current = gameHour * 60 + gameMinute;
    int start = h1 * 60 + m1;
    int end = h2 * 60 + m2;
    if (start <= end)
    {
        return current >= start && current < end;
    }
    else
    {
        return current >= start || current < end;
    }
}
