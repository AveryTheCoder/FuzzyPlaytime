#pragma once

#include "raylib.h"
#include <string>
#include <vector>

struct DialogueOption {
    std::string text;
    bool isCorrect;
    std::string response;
};

struct DialogueLine {
    std::string speaker;
    std::string text;
    std::vector<DialogueOption> options;
    bool hasOptions;
};

class DialogueSystem {
public:
    bool active;
    int currentLine;
    std::vector<DialogueLine> lines;
    std::string selectedResponse;
    float responseTimer;
    bool showingResponse;
    int hoveredOption;

    DialogueSystem();

    void StartQuiz();
    void StartMelodyQuiz();
    void StartIndianaDialogue();
    void StartFuzzyDialogue();
    void StartHawkDialogue();

    void Update(float dt);
    void Draw();
    void OnMouseClick(int mx, int my);
    void OnKeyDown(int key);

    bool IsFinished() const;
    bool WasLastAnswerCorrect() const;

private:
    bool lastAnswerCorrect;
    int sw, sh;
};
