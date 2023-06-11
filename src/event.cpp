#include "event.hpp"
#include <cstdint>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>

uint64_t Fox_Events_getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

namespace foxevents {
    FoxEvent::FoxEvent(uint64_t time, short repeat, void (*event)(void *args), void *args, int id) {
        if (!repeat) {
            this->time = time + Fox_Events_getCurrentTime();
        } else {
            this->time = time;
        }
        this->repeat = repeat;
        this->event = event;
        this->args = args;
        this->id = id;
        this->last_run = Fox_Events_getCurrentTime();
    }

    FoxEventQueue::FoxEventQueue() {
        this->events = std::vector<FoxEvent *>();
    }

    void FoxEventQueue::add(FoxEvent *event) {
        this->events.push_back(event);
    }

    void FoxEventQueue::remove(FoxEvent *event) {
        for (unsigned int i = 0; i < this->events.size(); i++) {
            if (this->events[i] == event) {
                this->events.erase(this->events.begin() + i);
                return;
            }
        }
    }

    void FoxEventQueue::remove_id(int id) {
        for (unsigned int i = 0; i < this->events.size(); i++) {
            if (this->events[i]->id == id) {
                this->events.erase(this->events.begin() + i);
                return;
            }
        }
    }

    void FoxEventQueue::run(uint64_t time) {
        for (unsigned int i = 0; i < this->events.size(); i++) {
            FoxEvent *event = this->events[i];
            if (time < event->time && !event->repeat) continue;
            if (event->repeat) {
                uint64_t last_time = event->last_run;
                if (time - last_time < event->time) continue;
                event->last_run = time;
                event->event(event->args);
            } else {
                event->event(event->args);
                this->events.erase(this->events.begin() + i);
            }
        }
    }

    short FoxEventQueue::empty() {
        return this->events.size() == 0;
    }
}
