#ifndef FOXEVENTS
#define FOXEVENTS

#include <stdint.h>
#include <vector>

namespace foxevents {
    class FoxEvent {
        public:
            FoxEvent(uint64_t time, short repeat, void (*event)(void *args), void *args, int id);
            uint64_t time;
            short repeat;
            void (*event)(void *args);
            void *args;
            uint64_t last_run;
            unsigned int id;
    };

    class FoxEventQueue {
        public:
            FoxEventQueue();
            void add(FoxEvent *event);
            void remove(FoxEvent *event);
            void remove_id(int id);
            void run(uint64_t time);
            short empty();
        private:
            std::vector<FoxEvent *> events;
    };
}

#endif
