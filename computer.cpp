#include "include/computer.h"
#include "include/data_loader.h"
#include <cstdio>
#include <cstring>

Computer::Computer()
    : currentPage(COMP_desktop), selectedFile(-1), active(false),
      passwordInputActive(false), passwordTarget(0), scrollOffset(0),
      notificationTimer(0.0f)
{
    cctvPassword = "NCFEACL2011";
    picturesPassword = "SUSPHOTOS";
}

void Computer::Init()
{
    cctvFiles = {
        {"02-23-2011-main-entrance.mp4", false, false, "", ""},
        {"02-24-2011-main-entrance.mp4", false, false, "", ""},
        {"02-25-2011-main-entrance.mp4", false, false, "", ""},
        {"02-26-2011-main-entrance.mp4", false, false, "", ""}
    };

    documentFiles = {
        {"daily-customer-report-02-23-2011.docx", false, false,
         "NCFEACL Daily Report - Feb 23, 2011\n\nVisitor count: 342\n\nNormal day. No incidents.\nStaff: 28 on duty\n\nAll exhibits operational.\nFuzzy mascot in good condition.", ""},
        {"daily-customer-report-02-24-2011.docx", false, false,
         "NCFEACL Daily Report - Feb 24, 2011\n\nVisitor count: 1,247\n\nABNORMALLY HIGH. Unsure why.\nSeems like a field trip from Lincoln Elementary.\n\nStaff reports strange noises overnight.\nFuzzy was found in different position than last night.", ""},
        {"daily-customer-report-02-25-2011.docx", false, false,
         "NCFEACL Daily Report - Feb 25, 2011\n\nVisitor count: 12\n\nABNORMALLY LOW. People are avoiding us.\n\nMiss Hawk exhibit acting strange.\nTeacher Melody's voice recordings playing at night.\n\nDO NOT ENTER AFTER 6PM.", ""}
    };

    passwordFiles = {
        {"PinManager.exe", false, false, "", ""},
        {"PasswordManager.exe", false, false, "", ""}
    };

    pictureFiles = {
        {"trees.png", false, false, "", ""},
        {"kids.png", false, false, "", ""}
    };

    susFiles = {
        {"fuzzy-moved.png", false, false, "", ""},
        {"what.png", false, false, "", ""},
        {"blood-in-factory.png", false, false, "", ""}
    };
}

void Computer::LoadPasswords(const DataLoader& loader)
{
    PasswordsData pw = loader.GetPasswords();
    if (!pw.cctv.empty()) cctvPassword = pw.cctv;
    if (!pw.pictures.empty()) picturesPassword = pw.pictures;
}

void Computer::Update(float dt)
{
    if (notificationTimer > 0.0f) notificationTimer -= dt;
}

void Computer::Draw()
{
    if (!active) return;

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    Color xpBlue = {0, 0, 128, 255};

    DrawRectangle(0, 0, sw, sh, xpBlue);

    switch (currentPage)
    {
        case COMP_desktop: DrawDesktop(); break;
        case COMP_CCTV:
        case COMP_cctv_locked:
            DrawFolder("CCTV Footage", cctvFiles);
            break;
        case COMP_documents:
            DrawFolder("Documents", documentFiles);
            break;
        case COMP_passwords:
            DrawFolder("Passwords", passwordFiles);
            break;
        case COMP_pictures:
        case COMP_pictures_locked:
            DrawFolder("Pictures", pictureFiles);
            break;
        case COMP_video_player:
            DrawVideoPlayer("CCTV Footage");
            break;
        case COMP_document_reader:
            DrawDocumentReader("Document", "");
            break;
        case COMP_folder_view:
            DrawFolder("Pictures > sus", susFiles);
            break;
    }

    if (passwordInputActive) DrawPasswordPrompt();

    DrawTaskbar();

    if (notificationTimer > 0.0f && !notification.empty())
    {
        int nw = MeasureText(notification.c_str(), 16) + 20;
        DrawRectangle(sw / 2 - nw / 2, 80, nw, 30, Color{200, 200, 0, 255});
        DrawText(notification.c_str(), sw / 2 - nw / 2 + 10, 86, 16, BLACK);
    }
}

void Computer::DrawTaskbar()
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    DrawRectangle(0, sh - 30, sw, 30, Color{0, 128, 0, 255});

    DrawRectangle(4, sh - 27, 80, 23, Color{0, 160, 0, 255});
    DrawText("Start", 12, sh - 23, 14, WHITE);

    const char* timeStr = "NCFEACL PC";
    DrawText(timeStr, sw - 120, sh - 23, 12, WHITE);

    DrawRectangle(sw - 200, sh - 25, 70, 18, Color{0, 100, 0, 255});
    DrawText("[ESC] Exit", sw - 196, sh - 23, 10, WHITE);
}

void Computer::DrawDesktop()
{
    int sh = GetScreenHeight();

    struct DesktopIcon { const char* name; int x; int y; int folderIndex; };
    DesktopIcon icons[] = {
        {"CCTV",     40,  40,  0},
        {"Documents", 40, 120,  1},
        {"Passwords",  40, 200,  2},
        {"Pictures",   40, 280,  3}
    };

    for (int i = 0; i < 4; i++)
    {
        int ix = icons[i].x;
        int iy = icons[i].y;

        bool hovered = (GetMouseX() >= ix && GetMouseX() <= ix + 64 &&
                        GetMouseY() >= iy && GetMouseY() <= iy + 64);

        if (hovered) DrawRectangle(ix - 2, iy - 2, 68, 68, Color{0, 0, 200, 100});
        DrawRectangle(ix, iy, 64, 64, Color{0, 0, 180, 200});
        DrawRectangle(ix + 8, iy + 4, 48, 40, Color{192, 192, 192, 255});

        if (icons[i].folderIndex == 0 || icons[i].folderIndex == 3)
        {
            DrawRectangle(ix + 32, iy + 24, 20, 16, Color{200, 200, 0, 255});
            DrawText("LCK", ix + 36, iy + 28, 8, BLACK);
        }

        int nameW = MeasureText(icons[i].name, 10);
        DrawText(icons[i].name, ix + 32 - nameW / 2, iy + 48, 10, WHITE);
    }

    DrawText("NCFEACL Computer System", 40, sh - 80, 16, Color{200, 200, 200, 255});
    DrawText("Windows XP (Educational License)", 40, sh - 60, 12, Color{180, 180, 180, 200});
}

void Computer::DrawFolder(const std::string& title, const std::vector<FileEntry>& files)
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    DrawRectangle(20, 20, sw - 40, 50, Color{0, 0, 128, 255});
    DrawRectangle(20, 70, sw - 40, 30, Color{192, 192, 192, 255});

    const char* backText = "< Back";
    bool backHovered = (GetMouseX() >= 24 && GetMouseX() <= 90 && GetMouseY() >= 72 && GetMouseY() <= 98);
    if (backHovered) DrawRectangle(24, 72, 66, 26, Color{0, 0, 200, 100});
    DrawText(backText, 28, 78, 14, BLACK);

    DrawText(title.c_str(), 28, 32, 20, Color{255, 255, 255, 255});

    DrawRectangle(20, 100, sw - 40, sh - 150, Color{255, 255, 255, 255});

    for (int i = 0; i < (int)files.size(); i++)
    {
        int fy = 110 + i * 30;

        bool hovered = (GetMouseX() >= 20 && GetMouseX() <= sw - 20 &&
                        GetMouseY() >= fy && GetMouseY() <= fy + 28);

        if (hovered) DrawRectangle(22, fy, sw - 44, 28, Color{0, 0, 200, 60});

        const char* icon = files[i].isFolder ? "[DIR]" : "[FILE]";
        if (files[i].isLocked) icon = "[LOCKED]";

        DrawText(icon, 28, fy + 6, 12, files[i].isLocked ? RED : GRAY);
        DrawText(files[i].name.c_str(), 90, fy + 6, 14, BLACK);
    }
}

void Computer::DrawPasswordPrompt()
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    DrawRectangle(sw / 2 - 150, sh / 2 - 60, 300, 120, Color{192, 192, 192, 255});
    DrawRectangleLines(sw / 2 - 150, sh / 2 - 60, 300, 120, BLACK);

    const char* prompt = "Enter Password:";
    DrawText(prompt, sw / 2 - 130, sh / 2 - 45, 16, BLACK);

    DrawRectangle(sw / 2 - 130, sh / 2 - 20, 260, 24, WHITE);
    DrawRectangleLines(sw / 2 - 130, sh / 2 - 20, 260, 24, BLACK);
    DrawText(enteredPassword.c_str(), sw / 2 - 124, sh / 2 - 14, 16, BLACK);

    DrawRectangle(sw / 2 - 60, sh / 2 + 20, 50, 22, Color{0, 128, 0, 255});
    DrawText("OK", sw / 2 - 48, sh / 2 + 24, 14, WHITE);

    DrawRectangle(sw / 2 + 10, sh / 2 + 20, 50, 22, Color{192, 0, 0, 255});
    DrawText("Cancel", sw / 2 + 16, sh / 2 + 24, 12, WHITE);
}

void Computer::DrawVideoPlayer(const std::string& title)
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    DrawRectangle(40, 40, sw - 80, sh - 120, BLACK);

    const char* vtitle = title.c_str();
    DrawText(vtitle, 50, 50, 16, WHITE);

    DrawText("Loading video...", sw / 2 - 60, sh / 2 - 10, 16, GRAY);

    DrawRectangle(40, sh - 70, sw - 80, 30, Color{40, 40, 40, 255});
    DrawRectangle(45, sh - 65, 100, 20, Color{200, 0, 0, 255});
    DrawText("|| Pause", 55, sh - 61, 12, WHITE);
}

void Computer::DrawDocumentReader(const std::string& title, const std::string& content)
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    DrawRectangle(40, 40, sw - 80, sh - 120, WHITE);
    DrawRectangleLines(40, 40, sw - 80, sh - 120, BLACK);

    DrawText(title.c_str(), 55, 55, 16, BLACK);
    DrawLine(55, 75, sw - 55, 75, GRAY);

    if (!content.empty())
    {
        int y = 85;
        const char* ptr = content.c_str();
        char line[256];

        while (*ptr && y < sh - 100)
        {
            int i = 0;
            while (*ptr && *ptr != '\n' && i < 255) { line[i++] = *ptr++; }
            line[i] = 0;
            if (*ptr == '\n') ptr++;

            DrawText(line, 55, y, 14, BLACK);
            y += 20;
        }
    }
}

void Computer::OnKeyDown(int key)
{
    if (passwordInputActive)
    {
        if (key == KEY_BACKSPACE && !enteredPassword.empty())
        {
            enteredPassword.pop_back();
        }
        else if (key == KEY_ENTER)
        {
            TryPassword();
        }
        else if (key == KEY_ESCAPE)
        {
            passwordInputActive = false;
            enteredPassword.clear();
        }
        return;
    }

    if (key == KEY_ESCAPE)
    {
        active = false;
        return;
    }
}

void Computer::OnCharInput(char c)
{
    if (passwordInputActive)
    {
        if (enteredPassword.size() < 32)
        {
            enteredPassword += c;
        }
    }
}

void Computer::OnMouseClick(int mx, int my)
{
    if (passwordInputActive)
    {
        if (mx >= GetScreenWidth() / 2 - 60 && mx <= GetScreenWidth() / 2 - 10 &&
            my >= GetScreenHeight() / 2 + 20 && my <= GetScreenHeight() / 2 + 42)
        {
            TryPassword();
        }
        else if (mx >= GetScreenWidth() / 2 + 10 && mx <= GetScreenWidth() / 2 + 60 &&
                 my >= GetScreenHeight() / 2 + 20 && my <= GetScreenHeight() / 2 + 42)
        {
            passwordInputActive = false;
            enteredPassword.clear();
        }
        return;
    }

    int sw = GetScreenWidth();

    if (currentPage == COMP_desktop)
    {
        struct IconHit { int x, y, folder; };
        IconHit icons[] = {{40,40,0},{40,120,1},{40,200,2},{40,280,3}};

        for (int i = 0; i < 4; i++)
        {
            if (mx >= icons[i].x && mx <= icons[i].x + 64 &&
                my >= icons[i].y && my <= icons[i].y + 64)
            {
                selectedFile = icons[i].folder;

                if (icons[i].folder == 0)
                {
                    currentPage = COMP_cctv_locked;
                    passwordTarget = 0;
                    passwordInputActive = true;
                    enteredPassword.clear();
                }
                else if (icons[i].folder == 1) currentPage = COMP_documents;
                else if (icons[i].folder == 2) currentPage = COMP_passwords;
                else if (icons[i].folder == 3)
                {
                    currentPage = COMP_pictures_locked;
                    passwordTarget = 1;
                    passwordInputActive = true;
                    enteredPassword.clear();
                }
                return;
            }
        }
        return;
    }

    if (currentPage == COMP_cctv_locked || currentPage == COMP_pictures_locked) return;

    bool backHovered = (mx >= 24 && mx <= 90 && my >= 72 && my <= 98);
    if (backHovered)
    {
        currentPage = COMP_desktop;
        return;
    }

    if (currentPage == COMP_documents || currentPage == COMP_passwords ||
        currentPage == COMP_CCTV || currentPage == COMP_pictures ||
        currentPage == COMP_folder_view)
    {
        const std::vector<FileEntry>* files = nullptr;
        if (currentPage == COMP_documents) files = &documentFiles;
        else if (currentPage == COMP_passwords) files = &passwordFiles;
        else if (currentPage == COMP_CCTV) files = &cctvFiles;
        else if (currentPage == COMP_pictures) files = &pictureFiles;
        else if (currentPage == COMP_folder_view) files = &susFiles;

        if (files)
        {
            for (int i = 0; i < (int)files->size(); i++)
            {
                int fy = 110 + i * 30;
                if (mx >= 20 && mx <= sw - 20 && my >= fy && my <= fy + 28)
                {
                    if ((*files)[i].name == "PasswordManager.exe" || (*files)[i].name == "PinManager.exe")
                    {
                        notification = "Application not available in demo.";
                        notificationTimer = 3.0f;
                    }
                    else if (currentPage == COMP_documents || currentPage == COMP_CCTV)
                    {
                        if (!(*files)[i].content.empty())
                        {
                            selectedFile = i;
                            currentPage = COMP_document_reader;
                        }
                        else
                        {
                            notification = "Cannot open " + (*files)[i].name;
                            notificationTimer = 2.0f;
                        }
                    }
                    else
                    {
                        notification = "Cannot open " + (*files)[i].name;
                        notificationTimer = 2.0f;
                    }
                    return;
                }
            }
        }
    }
}

void Computer::TryPassword()
{
    std::string correct;
    if (passwordTarget == 0) correct = cctvPassword;
    else correct = picturesPassword;

    if (enteredPassword == correct)
    {
        if (passwordTarget == 0) currentPage = COMP_CCTV;
        else currentPage = COMP_pictures;
        notification = "Access granted!";
        notificationTimer = 2.0f;
    }
    else
    {
        notification = "Wrong password!";
        notificationTimer = 2.0f;
    }

    passwordInputActive = false;
    enteredPassword.clear();
}
