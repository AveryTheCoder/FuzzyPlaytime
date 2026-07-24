#pragma once

#include "raylib.h"

int video_length(const char* path);
int animation_length(const char* path);

class Effects {
public:
    static void FadeIn(float duration);
    static void FadeOut(float duration);
};
