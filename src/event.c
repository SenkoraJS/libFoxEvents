#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>

#include "event.h"

FoxEventQueue *fox_event_queue_create() {
    FoxEventQueue *queue = malloc(sizeof(FoxEventQueue));
    queue->event = NULL;
    queue->next = NULL;
    return queue;
}

void fox_event_queue_add(FoxEventQueue *queue, FoxEvent *event) {
    FoxEventQueue *copy = queue;
    while (copy->next) {
        copy = copy->next;
    }
    copy->next = fox_event_queue_create();
    copy->event = event;
}

void fox_event_queue_remove(FoxEventQueue *queue, FoxEvent *event) {
    FoxEventQueue *copy = queue;
    while (copy->next) {
        if (copy->event == event) {
            free(copy->event);
            copy->event = NULL;
            break;
        }
        copy = copy->next;
    }
}

void fox_event_queue_remove_id(FoxEventQueue *queue, int id) {
    FoxEventQueue *copy = queue;
    while (copy->next) {
        if (copy->event && copy->event->id == id) {
            free(copy->event);
            copy->event = NULL;
            break;
        }
        copy = copy->next;
    }
}

void fox_event_queue_repair(FoxEventQueue *queue) {
    FoxEventQueue *copy = queue;
    while (copy->next) {
        FoxEvent *event = copy->event;
        if (!event) {
            FoxEventQueue *tmp = copy->next;
            copy = tmp->next;
            continue;
        }
        copy = copy->next;
    }
}

void fox_event_queue_run(FoxEventQueue *queue, uint64_t time) {
    FoxEventQueue *copy = queue;
    while (copy->next) {
        FoxEvent *event = copy->event;
        if (!event) {
            copy = copy->next;
            continue;
        }

        if (time < event->time && !event->repeat) {
            copy = copy->next;
            continue;
        }
        if (event->repeat) {
            uint64_t last_time = event->last_run;
            if (time - last_time < event->time) {
                copy = copy->next;
                continue;
            }
            event->last_run = time;
            event->event(event->args);
        } else {
            event->event(event->args);
            fox_event_queue_remove(queue, event);
        }
        copy = copy->next;
    }
    fox_event_queue_repair(queue);
}

void fox_event_queue_destroy(FoxEventQueue *queue) {
    FoxEventQueue *copy = queue;
    while (copy->next) {
        FoxEventQueue *tmp = copy->next;
        free(copy);
        copy = tmp;
    }
    free(copy);
}

short fox_event_queue_empty(FoxEventQueue *queue) {
    FoxEventQueue *copy = queue;
    while (copy->next) {
        if (copy->event) {
            return 0;
        }
        copy = copy->next;
    }
    return 1;
}

uint64_t Fox_Events_getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

FoxEvent *fox_event_create(uint64_t time, short repeat, void (*event)(void *args), void *args, int id) {
    FoxEvent *fox_event = malloc(sizeof(FoxEvent));
    if (!repeat) {
        fox_event->time = time + Fox_Events_getCurrentTime();
    } else {
        fox_event->time = time;
        fox_event->last_run = Fox_Events_getCurrentTime();
    }
    fox_event->repeat = repeat;
    fox_event->event = event;
    fox_event->args = args;
    fox_event->last_run = Fox_Events_getCurrentTime();
    fox_event->id = id;
    return fox_event;
}
