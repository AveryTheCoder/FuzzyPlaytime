#pragma once

#include "raylib.h"
#include <string>

struct GameTime {
    int gameHour;
    int gameMinute;
    int gameDay;
    float accumulator;

    GameTime();
    void Update(float realDeltaTime);
    void SetStartHour(int hour, int minute);
    float GetHourFloat() const;
    std::string GetTimeString() const;
    std::string GetPeriodString() const;
    int GetDay() const;
    bool IsBetween(int h1, int m1, int h2, int m2) const;
};
