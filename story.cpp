#include "include/story.h"

StoryManager::StoryManager()
    : introductionDone(false), teacherMelodyEventDone(false),
      missHawkEventDone(false), mrJJEventDone(false),
      johnJeffEventDone(false), pottyEventDone(false),
      fuzzyEventDone(false), patriotEventDone(false),
      endingTriggered(false), jinglePressCount(0),
      jingleActive(false), jingleTimer(0.0f),
      teacherMelodyWrongCount(0), studentBehaviorCount(0)
{
}

void StoryManager::Init()
{
    introductionDone = false;
    teacherMelodyEventDone = false;
    missHawkEventDone = false;
    mrJJEventDone = false;
    johnJeffEventDone = false;
    pottyEventDone = false;
    fuzzyEventDone = false;
    patriotEventDone = false;
    endingTriggered = false;
    jinglePressCount = 0;
    jingleActive = false;
    jingleTimer = 0.0f;
    teacherMelodyWrongCount = 0;
    studentBehaviorCount = 0;
    events.clear();
}

void StoryManager::Update(float dt, float gameHour)
{
    if (jingleActive)
    {
        jingleTimer -= dt;
        if (jingleTimer <= 0.0f)
        {
            jingleActive = false;
            jingleTimer = 0.0f;
        }
    }

    if (!fuzzyEventDone && jinglePressCount >= 10 && !ShouldFuzzyBeEvil())
    {
        fuzzyEventDone = true;
    }

    if (!endingTriggered && jinglePressCount >= 15)
    {
        endingTriggered = true;
    }
}

void StoryManager::OnJinglePressed()
{
    jinglePressCount++;
    jingleActive = true;
    jingleTimer = 2.0f;

    if (jinglePressCount >= 15 && !johnJeffEventDone)
    {
        johnJeffEventDone = true;
    }
}

void StoryManager::OnTeacherMelodyWrong()
{
    teacherMelodyWrongCount++;
    if (teacherMelodyWrongCount >= 3 && !teacherMelodyEventDone)
    {
        teacherMelodyEventDone = true;
    }
}

void StoryManager::OnBadBehavior()
{
    studentBehaviorCount++;
    if (studentBehaviorCount >= 5 && !teacherMelodyEventDone)
    {
        teacherMelodyEventDone = true;
    }
}

void StoryManager::OnExhibitTouch()
{
    studentBehaviorCount++;
    if (studentBehaviorCount >= 5 && !teacherMelodyEventDone)
    {
        teacherMelodyEventDone = true;
    }
}

void StoryManager::OnPatriotTriggered()
{
    patriotEventDone = true;
}

bool StoryManager::ShouldFuzzyBeEvil() const
{
    return (teacherMelodyEventDone || studentBehaviorCount >= 5) && !fuzzyEventDone;
}

bool StoryManager::IsGameEnding() const
{
    return endingTriggered;
}

int StoryManager::GetEndingType() const
{
    if (endingTriggered) return 1;
    return 0;
}

std::string StoryManager::GetHint() const
{
    if (!introductionDone)
        return "Find the teacher to begin your lesson.";
    if (jinglePressCount == 0)
        return "Explore the exhibits and press [E] to interact.";
    if (ShouldFuzzyBeEvil())
        return "Something is wrong with Fuzzy... stay alert!";
    if (jinglePressCount >= 13 && jinglePressCount < 15)
        return "The jingles are getting louder... maybe stop?";
    if (jinglePressCount >= 15)
        return "It's too late... run!";
    return "Keep exploring. Watch the clock.";
}
