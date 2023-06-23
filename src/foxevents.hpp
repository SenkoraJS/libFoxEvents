#ifndef FOXEVENTS
#define FOXEVENTS

#include <stdint.h>
#include <vector>
#include <memory>
#include <functional>
#include <foxworkers.hpp>

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
        private:
            std::unique_ptr<foxworkers::FoxHole> threadPool;
            std::vector<FoxEvent *> events;

        public:
            FoxEventQueue(int threadCount);
            void add(FoxEvent *event);
            void remove(FoxEvent *event);
            void remove_id(int id);
            void run(uint64_t time);
            short empty();

            // thread pool
            void runInBackGround(FoxEvent *event);
            bool isThreadPoolEmpty();
            void endThreadPool();
    };
}

#endif
