#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/* Description here: This test checks ...  */
static int test1() {
   int days = 7;
   Calendar *calendar;

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}


/* destroy_calendar conatins clear_calendar, 
   and clear_calendar contains clear_day */
static int test2() {
    Calendar *calendar;
    Event *event;
    int days = 30;
    const char *eventName = "Meeting";
    int startTime = 10;
    int duration = 60;
    int day = 5;

    /* check init_calender */
    if (init_calendar("Work", days, comp_minutes, NULL, &calendar) != SUCCESS) {
        return FAILURE;
    }

      /* check add event */
    if (add_event(calendar, eventName, startTime, duration, NULL, day) != SUCCESS) {
        destroy_calendar(calendar);
        return FAILURE;
    }

      /* check find event in day */
    if (find_event_in_day(calendar, eventName, day, &event) != SUCCESS) {
        destroy_calendar(calendar);
        return FAILURE;
    }

      /* check destroy calendar */
    if (destroy_calendar(calendar) != SUCCESS) {
        return FAILURE;
    }

    return SUCCESS;
}

/* destroy_calendar conatins clear_calendar, 
   and clear_calendar contains clear_day */
static int test3() {
    Calendar *calendar;
    int days = 30;
    int result;

    
    if (init_calendar("Work", days, comp_minutes, NULL, &calendar) != SUCCESS) {
        return FAILURE;
    }

      /* check add multiple events */
    if (add_event(calendar, "review meeting", 800, 80, NULL, 1) != SUCCESS ||
        add_event(calendar, "group meeting", 500, 60, NULL, 1) != SUCCESS ||
        add_event(calendar, "lunch", 1200, 45, NULL, 3) != SUCCESS) {
        destroy_calendar(calendar);
        return FAILURE;
    }

      /* check remove event */
    if (remove_event(calendar, "group meeting") != SUCCESS) {
        destroy_calendar(calendar);
        return FAILURE;
    }

     /* check find event */
    result = find_event(calendar, "group meeting", NULL);
    if (result != FAILURE) { 
        destroy_calendar(calendar);
        return FAILURE;
    }

     /* check destroy calendar */
    if (destroy_calendar(calendar) != SUCCESS) {
        return FAILURE;
    }

    return SUCCESS;
}

/* destroy_calendar conatins clear_calendar, 
   and clear_calendar contains clear_day */
static int test4() {
    Calendar *calendar;
    int days = 30;
    const char *info = "Meeting Room B"; 
    const char *retrievedInfo;

     /* check comp_minutes with init calendar */
    if (init_calendar("Work", days, comp_minutes, NULL, &calendar) != SUCCESS) {
        return FAILURE;
    }

    if (add_event(calendar, "team meeting", 1000, 60, (void *)info, 2) != SUCCESS) {
        destroy_calendar(calendar);
        return FAILURE;
    }

    retrievedInfo = (const char *)get_event_info(calendar, "team meeting");
    if (retrievedInfo == NULL || strcmp(retrievedInfo, info) != 0) {
        destroy_calendar(calendar);
        return FAILURE;
    }


    if (destroy_calendar(calendar) != SUCCESS) {
        return FAILURE;
    }

    return SUCCESS;
}


int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   if (test1() == FAILURE) result = FAILURE;
   printf("Test1 Success\n");
   if (test2() == FAILURE) result = FAILURE;
    printf("Test2 Success\n");
   if (test3() == FAILURE) result = FAILURE;
    printf("Test3 Success\n");
     if (test4() == FAILURE) result = FAILURE;
    printf("Test4 Success\n");
   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
