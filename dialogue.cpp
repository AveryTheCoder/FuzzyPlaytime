#include "include/dialogue.h"

DialogueSystem::DialogueSystem()
    : active(false), currentLine(0), responseTimer(0.0f),
      showingResponse(false), hoveredOption(-1), lastAnswerCorrect(true),
      sw(0), sh(0)
{
}

void DialogueSystem::StartQuiz()
{
    StartMelodyQuiz();
}

void DialogueSystem::StartMelodyQuiz()
{
    lines.clear();
    currentLine = 0;
    active = true;
    showingResponse = false;

    lines.push_back({"Teacher Melody",
        "Welcome, student! I am Teacher Melody. Let's test your knowledge!",
        {}, false});

    lines.push_back({"Teacher Melody",
        "Question 1: What is the largest mammal on Earth?",
        {
            {"Blue Whale", true, "Correct! The Blue Whale can grow up to 100 feet!"},
            {"Elephant", false, "Incorrect! The elephant is the largest LAND mammal."},
            {"Giraffe", false, "No, giraffes are tall but not the largest mammal."}
        }, true});

    lines.push_back({"Teacher Melody",
        "Question 2: What gas do trees absorb from the atmosphere?",
        {
            {"Carbon Dioxide", true, "Excellent! Trees use CO2 for photosynthesis!"},
            {"Oxygen", false, "No, trees RELEASE oxygen, they absorb CO2."},
            {"Nitrogen", false, "Not quite. Nitrogen is abundant but not what trees absorb."}
        }, true});

    lines.push_back({"Teacher Melody",
        "Question 3: In what year was the NCFEACL founded?",
        {
            {"1952", true, "That's right! We've been educating children for decades!"},
            {"1999", false, "Incorrect! We were around long before that."},
            {"2010", false, "No, we were established much earlier."}
        }, true});

    lines.push_back({"Teacher Melody",
        "Well done! You've passed the quiz. I hope you learned something today!",
        {}, false});
}

void DialogueSystem::StartIndianaDialogue()
{
    lines.clear();
    currentLine = 0;
    active = true;
    showingResponse = false;

    lines.push_back({"Miss Indiana",
        "Hello there, dear! Welcome to the NCFEACL.",
        {}, false});

    lines.push_back({"Miss Indiana",
        "I've been here for a very long time. I know all the exhibits.",
        {}, false});

    lines.push_back({"Miss Indiana",
        "If you have any questions about the exhibits, feel free to ask!",
        {}, false});

    lines.push_back({"Miss Indiana",
        "Be careful out there... not everything is as it seems.",
        {}, false});
}

void DialogueSystem::StartFuzzyDialogue()
{
    lines.clear();
    currentLine = 0;
    active = true;
    showingResponse = false;

    lines.push_back({"Fuzzy",
        "*happy waving* Hiii there, friend! I'm Fuzzy!",
        {}, false});

    lines.push_back({"Fuzzy",
        "Welcome to the Nature Center! I hope you have a great time!",
        {}, false});

    lines.push_back({"Fuzzy",
        "*tilts head* Something feels... different today. Be careful, okay?",
        {}, false});
}

void DialogueSystem::StartHawkDialogue()
{
    lines.clear();
    currentLine = 0;
    active = true;
    showingResponse = false;

    lines.push_back({"Miss Hawk",
        "*perches on display* Squawk! Welcome, visitor!",
        {}, false});

    lines.push_back({"Miss Hawk",
        "I patrol this area during the day. Everything is safe... for now.",
        {}, false});

    lines.push_back({"Miss Hawk",
        "*ruffles feathers* Just be careful when the sun starts to set.",
        {}, false});
}

void DialogueSystem::Update(float dt)
{
    sw = GetScreenWidth();
    sh = GetScreenHeight();
    if (showingResponse) responseTimer -= dt;
    if (showingResponse && responseTimer <= 0.0f)
    {
        showingResponse = false;
        currentLine++;
        if (currentLine >= (int)lines.size()) active = false;
    }

    Vector2 mp = GetMousePosition();
    hoveredOption = -1;
    if (active && currentLine < (int)lines.size() && lines[currentLine].hasOptions && !showingResponse)
    {
        for (int i = 0; i < (int)lines[currentLine].options.size(); i++)
        {
            int oy = sh / 2 + 60 + i * 35;
            if (mp.x >= 60 && mp.x <= sw - 60 && mp.y >= oy && mp.y <= oy + 30)
            {
                hoveredOption = i;
            }
        }
    }
}

void DialogueSystem::Draw()
{
    if (!active) return;

    DrawRectangle(0, 0, sw, sh, Color{0, 0, 0, 150});

    DrawRectangle(30, sh / 2 - 80, sw - 60, 160, Color{20, 20, 40, 240});
    DrawRectangleLines(30, sh / 2 - 80, sw - 60, 160, Color{100, 100, 200, 255});

    if (currentLine < (int)lines.size())
    {
        const DialogueLine& line = lines[currentLine];

        DrawText(line.speaker.c_str(), 50, sh / 2 - 65, 18, YELLOW);
        DrawText(line.text.c_str(), 50, sh / 2 - 35, 16, WHITE);

        if (line.hasOptions && !showingResponse)
        {
            for (int i = 0; i < (int)line.options.size(); i++)
            {
                int oy = sh / 2 + 60 + i * 35;
                Color bg = (i == hoveredOption) ? Color{60, 60, 120, 255} : Color{30, 30, 60, 255};

                DrawRectangle(60, oy, sw - 120, 30, bg);
                DrawRectangleLines(60, oy, sw - 120, 30, Color{80, 80, 160, 255});
                DrawText(line.options[i].text.c_str(), 75, oy + 7, 14, WHITE);
            }
        }
        else if (showingResponse)
        {
            DrawText(selectedResponse.c_str(), 50, sh / 2 + 10, 16,
                     lastAnswerCorrect ? Color{0, 255, 0, 255} : Color{255, 80, 80, 255});
        }
        else
        {
            DrawText("Press any key to continue...", 50, sh / 2 + 50, 14, GRAY);
        }
    }
}

void DialogueSystem::OnMouseClick(int mx, int my)
{
    if (!active || currentLine >= (int)lines.size()) return;

    if (showingResponse) return;

    const DialogueLine& line = lines[currentLine];

    if (line.hasOptions)
    {
        for (int i = 0; i < (int)line.options.size(); i++)
        {
            int oy = sh / 2 + 60 + i * 35;
            if (mx >= 60 && mx <= sw - 60 && my >= oy && my <= oy + 30)
            {
                selectedResponse = line.options[i].response;
                lastAnswerCorrect = line.options[i].isCorrect;
                showingResponse = true;
                responseTimer = 2.5f;
                return;
            }
        }
    }
    else
    {
        currentLine++;
        if (currentLine >= (int)lines.size()) active = false;
    }
}

void DialogueSystem::OnKeyDown(int key)
{
    if (!active || currentLine >= (int)lines.size()) return;
    if (showingResponse) return;

    const DialogueLine& line = lines[currentLine];
    if (!line.hasOptions)
    {
        currentLine++;
        if (currentLine >= (int)lines.size()) active = false;
    }
}

bool DialogueSystem::IsFinished() const { return !active; }
bool DialogueSystem::WasLastAnswerCorrect() const { return lastAnswerCorrect; }
