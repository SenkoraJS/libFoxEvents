#include "src/event.h"
#include <stdio.h>
#include <sys/time.h>

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
    FoxEventQueue *queue = fox_event_queue_create();

    FoxEvent *event = fox_event_create(1000, 1, func, NULL);
    fox_event_queue_add(queue, event);

    FoxEvent *event2 = fox_event_create(100, 1, func2, NULL);
    fox_event_queue_add(queue, event2);

    while (!fox_event_queue_empty(queue)) {
        queue->current_time = getCurrentTime();
        fox_event_queue_run(queue);
    }
    fox_event_queue_destroy(queue);
    return 0;
}