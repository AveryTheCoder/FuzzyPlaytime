#pragma once

#include "raylib.h"
#include <string>
#include <vector>

struct StoryEvent {
    std::string name;
    bool triggered;
    float triggerTime;
    int triggerDay;
};

class StoryManager {
public:
    bool introductionDone;
    bool teacherMelodyEventDone;
    bool missHawkEventDone;
    bool mrJJEventDone;
    bool johnJeffEventDone;
    bool pottyEventDone;
    bool fuzzyEventDone;
    bool patriotEventDone;
    bool endingTriggered;

    int jinglePressCount;
    bool jingleActive;
    float jingleTimer;

    int teacherMelodyWrongCount;
    int studentBehaviorCount;

    StoryManager();

    void Init();
    void Update(float dt, float gameHour);
    void OnJinglePressed();
    void OnTeacherMelodyWrong();
    void OnBadBehavior();
    void OnExhibitTouch();
    void OnPatriotTriggered();

    bool ShouldFuzzyBeEvil() const;
    bool IsGameEnding() const;
    int GetEndingType() const;

    std::string GetHint() const;

private:
    std::vector<StoryEvent> events;
};
