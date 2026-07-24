#pragma once

#include "raylib.h"
#include <string>
#include <vector>

class DataLoader;

enum ComputerPage {
    COMP_desktop,
    COMP_CCTV,
    COMP_documents,
    COMP_passwords,
    COMP_pictures,
    COMP_cctv_locked,
    COMP_pictures_locked,
    COMP_video_player,
    COMP_document_reader,
    COMP_folder_view
};

struct FileEntry {
    std::string name;
    bool isFolder;
    bool isLocked;
    std::string content;
    std::string password;
};

class Computer {
public:
    ComputerPage currentPage;
    int selectedFile;
    bool active;
    std::string enteredPassword;
    bool passwordInputActive;
    int passwordTarget;
    int scrollOffset;
    std::string notification;
    float notificationTimer;

    Computer();
    void Init();
    void LoadPasswords(const DataLoader& loader);
    void Update(float dt);
    void Draw();

    void OnKeyDown(int key);
    void OnCharInput(char c);
    void OnMouseClick(int mx, int my);

private:
    void DrawTaskbar();
    void DrawDesktop();
    void DrawFolder(const std::string& title, const std::vector<FileEntry>& files);
    void DrawPasswordPrompt();
    void DrawVideoPlayer(const std::string& title);
    void DrawDocumentReader(const std::string& title, const std::string& content);

    void OpenFile(const std::string& name);
    void TryPassword();

    std::vector<FileEntry> cctvFiles;
    std::vector<FileEntry> documentFiles;
    std::vector<FileEntry> passwordFiles;
    std::vector<FileEntry> pictureFiles;
    std::vector<FileEntry> susFiles;

    std::string cctvPassword;
    std::string picturesPassword;
};
