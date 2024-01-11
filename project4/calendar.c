/*
author: Wonjae Kim
UID: 117283365
Directory ID: wkim1128
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calendar.h"
#include "event.h"

int init_calendar(const char *name, int days,
                  int (*comp_func)(const void *p1, const void *p2),
                  void (*free_info_func)(void *), Calendar **calendar) {

    /* check if the parameter is valid or not */
    if (days < 1 || name == NULL || calendar == NULL) {
        return FAILURE;
    }
    /* allocate the memory */
    (*calendar) = malloc(sizeof(Calendar));
    (*calendar)->name = malloc(strlen(name) + 1);
    (*calendar)->events = calloc(days, sizeof(Event *));

    /* check if the memory allocation or the input is invalid */
    if (*calendar == NULL || (*calendar)->name == NULL ||
        (*calendar)->events == NULL) {

        return FAILURE;

    } else {
        /* copy and put the value */
        strcpy((*calendar)->name, name);
        (*calendar)->days = days;
        (*calendar)->total_events = 0;
        (*calendar)->comp_func = comp_func;
        (*calendar)->free_info_func = free_info_func;

        return SUCCESS;
    }
}

int print_calendar(Calendar *calendar, FILE *output, int print_all_info) {
    Event *currentEvent;
    int i;
    /* check if the parameter is valid or not */
    if (calendar == NULL || output == NULL) {
        return FAILURE;
    }
    /* if print all info is valid, print */
    if (print_all_info) {
        fprintf(output, "Calendar's Name: \"%s\"\n", calendar->name);
        fprintf(output, "Days: %d\n", calendar->days);
        fprintf(output, "Total Events: %d\n\n", calendar->total_events);
    }

    printf("**** Events ****\n");

    /* if total event bigger than 0, print events */
    if (calendar->total_events > 0) {
        for (i = 0; i < calendar->days; i++) {
            currentEvent = calendar->events[i];
            fprintf(output, "Day %d\n", i + 1);

            while (currentEvent != NULL) {
                
                fprintf(output,
                        "Event's Name: \"%s\", Start_time: %d, Duration: %d\n",
                        currentEvent->name, currentEvent->start_time,
                        currentEvent->duration_minutes);

                currentEvent = currentEvent->next;
            }
        }
    }
    return SUCCESS;
}

int add_event(Calendar *calendar, const char *name, int start_time,
              int duration, void *info, int day) {

    Event *current;
    Event *new_event = malloc(sizeof(Event));
    Event *prev = NULL;

    /* check if the parameter is valid or not */
    if (calendar == NULL || name == NULL || day < 1 || day > calendar->days ||
        start_time < 0 || start_time > 2400 || duration <= 0) {
        return FAILURE;
    }

    current = calendar->events[day - 1];

    /* if curret event is the same, return FAILURE */
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return FAILURE;
        }
        current = current->next;
    }
    /* if event is NULL, return failure */
    if (new_event == NULL) {
        return FAILURE;
    }

    new_event->name = malloc(strlen(name) + 1);

    /* if name is null, free memory and return failure */
    if (new_event->name == NULL) {
        free(new_event);
        return FAILURE;
    }

    /* assign the value */
    strcpy(new_event->name, name);
    new_event->start_time = start_time;
    new_event->duration_minutes = duration;
    new_event->info = info;
    new_event->next = NULL;

    /*Insert the new event in the correct position*/
    current = calendar->events[day - 1];
    while (current != NULL && calendar->comp_func(current, new_event) < 0) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {
        /*inset beginning*/
        new_event->next = calendar->events[day - 1];
        calendar->events[day - 1] = new_event;
    } else {
        new_event->next = current;
        prev->next = new_event;
    }

    calendar->total_events++;
    return SUCCESS;
}

int find_event(Calendar *calendar, const char *name, Event **event) {
    Event *current;
    int i;

    /* check if the parameter is valid or not */
    if (calendar == NULL || name == NULL || event == NULL) {
        return FAILURE;
    }

    /* from beginning, find out the same name event */
    for (i = 0; i < calendar->days; i++) {
        current = calendar->events[i];
        while (current != NULL) {
            if (strcmp(current->name, name) == 0) {
                /*found event*/
                *event = current;
                return SUCCESS;
            }
            current = current->next;
        }
    }
    return FAILURE;
}

int find_event_in_day(Calendar *calendar, const char *name,
                      int day, Event **event) {
    Event *current;

    /* check if the parameter is valid or not */
    if (calendar == NULL || name == NULL || day < 1 ||
        day > calendar->days || event == NULL) {
        return FAILURE;
    }

    current = calendar->events[day - 1];

    /* serach for the event in the specified day */
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            /* event found*/
            *event = current;
            return SUCCESS;
        }
        current = current->next;
    }
    return FAILURE;
}

int remove_event(Calendar *calendar, const char *name) {
    Event *current;
    Event *prev = NULL;
    int i;

    /* check if the parameter is valid or not */
    if (calendar == NULL || name == NULL) {
        return FAILURE;
    }

    for (i = 0; i < calendar->days; i++) {
        current = calendar->events[i];
        prev = NULL;

        while (current != NULL) {
            if (strcmp(current->name, name) == 0) {
                /* Remove event */
                if (prev == NULL) {
                    /* Event is the first in the list */
                    calendar->events[i] = current->next;
                } else if (current->info != NULL && 
                           calendar->free_info_func != NULL) {

                    calendar->free_info_func(current->info);

                } else {
                    /* Event is not the first in the list */
                    prev->next = current->next;
                }

                free(current->name);
                free(current);
                calendar->total_events--;

                return SUCCESS;
            }

            prev = current;
            current = current->next;

        }
    }

    return FAILURE;

}

void *get_event_info(Calendar *calendar, const char *name) {
    Event *current;
    int i;

    for (i = 0; i < calendar->days; i++) {
        current = calendar->events[i];

        while (current != NULL) {
            if (strcmp(current->name, name) == 0) {
                /*event found*/
                return current->info;
            }
            current = current->next;
        }
    }
}

int clear_day(Calendar *calendar, int day) {
    Event *current, *temp;

    /* check if the parameter is valid or not */
    if (calendar == NULL || day < 1 || day > calendar->days) {
        return FAILURE;
    }

    current = calendar->events[day - 1];

    while (current != NULL) {
        temp = current;
        current = current->next;

        /* if free info func exist, free info */
        if (calendar->free_info_func != NULL && temp->info != NULL) {
            calendar->free_info_func(temp->info);
        }

        free(temp->name);
        free(temp);
        /* decrement the total event */
        calendar->total_events--;
    }
    calendar->events[day - 1] = NULL;
    return SUCCESS;
}

int clear_calendar(Calendar *calendar) {
    int i;

    /* check if the parameter is valid or not */
    if (calendar == NULL) {
        return FAILURE;
    }

    /* clear day */
    for (i = 0; i < calendar->days; i++) {
        clear_day(calendar, i + 1);
    }

    return SUCCESS;

}

int destroy_calendar(Calendar *calendar) {

    /* check if the parameter is valid or not */
    if (calendar == NULL) {
        return FAILURE;
    }

    /* free everything */
    clear_calendar(calendar);

    free(calendar->name);

    free(calendar->events);

    free(calendar);

    return SUCCESS;
}