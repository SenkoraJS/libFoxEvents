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
    unsigned int id;
} FoxEvent;

typedef struct _FoxEventQueue {
    FoxEvent *event;
    struct _FoxEventQueue *next;
} FoxEventQueue;

FoxEventQueue *fox_event_queue_create();
void fox_event_queue_destroy(FoxEventQueue *queue);
void fox_event_queue_add(FoxEventQueue *queue, FoxEvent *event);
void fox_event_queue_remove(FoxEventQueue *queue, FoxEvent *event);
void fox_event_queue_run(FoxEventQueue *queue, uint64_t time);
short fox_event_queue_empty(FoxEventQueue *queue);
void fox_event_queue_repair(FoxEventQueue *queue);

FoxEvent *fox_event_create(uint64_t time, short repeat, void (*event)(void *args), void *args);
#endif
