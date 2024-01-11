#if !defined(EVENT_H)
#define EVENT_H

typedef struct event {
   char *name;
   int start_time, duration_minutes;
   void *info;
   struct event *next;
} Event;

#endif

/*

typedef struct calendar {
  char *name;
  Event **events;
  int days, total_events;
  int (*comp_func)(const void *, const void *);
  void (*free_info_func)(void *);
} Calendar;
*/