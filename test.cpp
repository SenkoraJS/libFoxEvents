#include "src/foxevents.hpp"
#include <stdio.h>
#include <sys/time.h>
#include <chrono>

using namespace foxevents;

uint64_t getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void print(void *data) {
    int *num = (int *) data;
    for (int i = 0; i < 1'000'000'000; i++);
    printf("%d\n", *num);
}

uint64_t millis()
{
    uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return ms; 
}

void func(void *args) {
    printf("Hello, world!\n");
}

void func2(void *args) {
    printf("Hello, world! 2\n");
}

int main() {
    FoxEventQueue queue(10);

    FoxEvent *event2 = new FoxEvent(100, 0, func2, NULL, 1);
    queue.add(event2);
    FoxEvent *event = new FoxEvent(6000, 0, func, NULL, 2);
    queue.add(event);

    for (int i = 0; i < 10; i++) {
        FoxEvent *ev = new FoxEvent(0, 0, print, new int(i), i + 3);
        queue.runInBackGround(ev);
    }

    while (!queue.empty()) {
        if (!queue.empty()) {
            queue.run(getCurrentTime());
        }
    }

    while(!queue.isThreadPoolEmpty());
    queue.endThreadPool();

    return 0;
}
