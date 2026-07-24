#include "include/story.h"
#include "include/computer.h"
#include "include/event_manager.h"
#include <cstring>

void event_manager::play_event(const char* event) {
    if (strcmp(event, event_manager::wakeup_cutscene::name) == 0) {
        event_manager::wakeup_cutscene::init();
    }
}

void event_manager::wakeup_cutscene::init() {
    playing = false;
    duration = animation_length("./assets/img/cutscenes/wakeup_cutscene.mp4");
}

void event_manager::wakeup_cutscene::play_animation() {
    playing = true;
}

void event_manager::wakeup_cutscene::pause_animation() {
    playing = false;
}

void event_manager::wakeup_cutscene::stop_animation() {
    playing = false;
}
