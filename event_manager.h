#ifndef FUZZYPLAYTIME_EVENT_MANAGER_H
#define FUZZYPLAYTIME_EVENT_MANAGER_H

#include "asset_manager.h"
#include <cstring>

namespace event_manager {
    void play_event(const char* event);

    namespace wakeup_cutscene {
        inline const char* name = "WakeUpCutScene";
        inline int duration = 0;
        inline bool playing = false;
        void play_animation();
        void pause_animation();
        void stop_animation();

        void init();
    }
}

#endif // FUZZYPLAYTIME_EVENT_MANAGER_H
