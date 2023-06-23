#include "src/foxevents.hpp"
#include <stdio.h>
#include <sys/time.h>

using namespace foxevents;

uint64_t getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void func(void *args) {
    printf("Hello, world!\n");
}

void func2(void *args) {
    printf("Hello, world! 2\n");
}

int main() {
    FoxEventQueue queue;

    FoxEvent *event2 = new FoxEvent(100, 0, func2, NULL, 1);
    queue.add(event2);
    FoxEvent *event = new FoxEvent(1000, 0, func, NULL, 2);
    queue.add(event);

    while (!queue.empty()) {
        queue.run(getCurrentTime());
    }

    return 0;
}
