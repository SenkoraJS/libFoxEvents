#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>

#include "event.h"

FoxEventQueue *fox_event_queue_create() {
    FoxEventQueue *queue = malloc(sizeof(FoxEventQueue));
    queue->size = 0;
    queue->events = malloc(sizeof(FoxEvent));
    return queue;
}

void fox_event_queue_add(FoxEventQueue *queue, FoxEvent *event) {
    queue->size++;
    queue->events = realloc(queue->events, sizeof(FoxEvent) * queue->size);
    queue->events[queue->size - 1] = event;
}

void fox_event_queue_remove(FoxEventQueue *queue, FoxEvent *event) {
    for (int i = 0; i < queue->size; i++) {
        if (queue->events[i] == event) {
            queue->events[i] = NULL;
            break;
        }
    }
}

void fox_event_queue_run(FoxEventQueue *queue) {
    for (int i = 0; i < queue->size; i++) {
        if (!queue->events[i]) continue;
        if (queue->current_time < queue->events[i]->time && !queue->events[i]->repeat) continue;
        if (queue->events[i]->repeat) {
            uint64_t last_time = queue->events[i]->last_run;
            if (queue->current_time - last_time < queue->events[i]->time) continue;
            queue->events[i]->last_run = queue->current_time;
            queue->events[i]->event(queue->events[i]->args);
        } else {
            queue->events[i]->event(queue->events[i]->args);
            fox_event_queue_remove(queue, queue->events[i]);
        }
    }
}

void fox_event_queue_destroy(FoxEventQueue *queue) {
    for (int i = 0; i < queue->size; i++) {
        if (!queue->events[i]) continue;
        free(queue->events[i]);
    }
    free(queue->events);
    free(queue);
}

short fox_event_queue_empty(FoxEventQueue *queue) {
    short empty = 1;
    for (int i = 0; i < queue->size; i++) {
        if (queue->events[i]) {
            empty = 0;
            break;
        }
    }
    return empty;
}

uint64_t Fox_Events_getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

FoxEvent *fox_event_create(uint64_t time, short repeat, void (*event)(void *args), void *args) {
    FoxEvent *fox_event = malloc(sizeof(FoxEvent));
    fox_event->time = time;
    fox_event->repeat = repeat;
    fox_event->event = event;
    fox_event->args = args;
    fox_event->last_run = Fox_Events_getCurrentTime();
    return fox_event;
}
