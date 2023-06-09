#ifndef FOXEVENTS
#define FOXEVENTS

#include <stdint.h>

typedef struct {
    uint64_t time;
    // if repeat, time is used as delay between action
    short repeat;
    void (*event)(void *args);
    void *args;
    uint64_t last_run;
} FoxEvent;

typedef struct {
    FoxEvent **events;
    int size;
    uint64_t current_time;
} FoxEventQueue;

FoxEventQueue *fox_event_queue_create();
void fox_event_queue_destroy(FoxEventQueue *queue);
void fox_event_queue_add(FoxEventQueue *queue, FoxEvent *event);
void fox_event_queue_remove(FoxEventQueue *queue, FoxEvent *event);
void fox_event_queue_run(FoxEventQueue *queue);
short fox_event_queue_empty(FoxEventQueue *queue);

FoxEvent *fox_event_create(uint64_t time, short repeat, void (*event)(void *args), void *args);
#endif
