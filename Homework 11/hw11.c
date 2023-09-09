#include "hw11.h"

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* HELPER FUNCTIONS */

/*
 * This function acts as a helper function and compares the times of the
 * two time structs given.
 */

int timecmp(time_struct_t time1, time_struct_t time2) {
  if (time1.year > time2.year) {
    return 1;
  }
  else if (time1.year < time2.year) {
    return -1;
  }
  else if (time1.month > time2.month) {
    return 1;
  }
  else if (time1.month < time2.month) {
    return -1;
  }
  else if (time1.day > time2.day) {
    return 1;
  }
  else if (time1.day < time2.day) {
    return -1;
  }
  else if (time1.hour > time2.hour) {
    return 1;
  }
  else if (time1.hour < time2.hour) {
    return -1;
  }
  else if (time1.minute > time2.minute) {
    return 1;
  }
  else if (time1.minute < time2.minute) {
    return -1;
  }
  else {
    return 0;
  }
} /* timecmp() */

/*
 * This function just checks and uses timecmp to see if time1 is later than
 * time 2. This is also a helper function
 */

int is_later(time_struct_t time1, time_struct_t time2) {
  return timecmp(time1, time2) == 1;
} /* is_later() */

/*
 * This function calculates the duration of a given event by taking the start
 * and end time as arguments.
 */

long duration(time_struct_t time1, time_struct_t time2) {
  long minutes = time1.hour * 60 + time1.minute + time1.year * 12 * 30 * 24 * 60
                 + time1.month * 30 * 24 * 60 + time1.day * 24 * 60;
  long minutes2 = time2.hour * 60 + time2.minute + time2.year * 12 * 30 * 24 * 60
                 + time2.month * 30 * 24 * 60 + time2.day * 24 * 60;
  return minutes - minutes2;
} /* duration() */

/* IMPLEMENTED FUNCTIONS */

/*
 * This function creates an event given the argument inputs.
 */

void create_event(event_info_t **new_event, char *name, time_struct_t start_time, time_struct_t end_time) {
  assert(new_event != NULL);
  assert(*new_event == NULL);
  assert(name != NULL);

  event_info_t *event = malloc(sizeof(event_info_t));
  assert(event != NULL);
  event->name = malloc(strlen(name) + 1);
  assert(event->name != NULL);
  strcpy(event->name, name);
  event->start_time = start_time;
  event->end_time = end_time;

  *new_event = event;
} /* create_event() */

/*
 * This function adds the event into the tree given as an argument.
 */

void add_event(tree_node_t **event_schedule, event_info_t *event_info) {
  assert(event_schedule != NULL);
  assert(event_info != NULL);

  tree_node_t *new_node = (tree_node_t *) malloc(sizeof(tree_node_t));
  assert(new_node != NULL);
  new_node->event_info = event_info;
  new_node->left_child_ptr = NULL;
  new_node->right_child_ptr = NULL;

  if (*event_schedule == NULL) {
    *event_schedule = new_node;
    return;
  }

  // Find the appropriate location to insert the new node in the tree
  tree_node_t *current_node = *event_schedule;
  while (current_node != NULL) {
    if (is_later(current_node->event_info->start_time, event_info->start_time)) {
      if (current_node->left_child_ptr == NULL) {
        current_node->left_child_ptr = new_node;
        return;
      }
      current_node = current_node->left_child_ptr;
    }
    else {
      if (current_node->right_child_ptr == NULL) {
        current_node->right_child_ptr = new_node;
        return;
      }
      current_node = current_node->right_child_ptr;
    }
  }
} /* add_event() */

/*
 * This function counts the number of expired events in the event schedule tree
 * by comparing them using the islater function and then counting the number
 * of expired events.
 */

int count_expired_events(tree_node_t *event_schedule, time_struct_t current_time) {
  int count = 0;

  if (event_schedule == NULL) {
    return count;
  }

  count += count_expired_events(event_schedule->left_child_ptr, current_time);
  if (is_later(current_time, event_schedule->event_info->end_time)) {
    count++;
  }
  count += count_expired_events(event_schedule->right_child_ptr, current_time);
  return count;
} /* count_expired_events() */

/*
 * This function adds different events in the tree, depending on a certain
 * criteria, to the linked list given as a second argument. We make sure that
 * we insert only the correct trees into the list and make sure that the list
 * stays in the correct order.
 */

void add_to_calendar(tree_node_t* tree, list_node_t** list) {
  assert(list != NULL);
  if (tree == NULL) {
    return;
  }

  /*while ((*list) && (*list)->prev_event) {
    *list = (*list)->prev_event;
  }

  add_to_calendar(tree->left_child_ptr, list);

  event_info_t* event = tree->event_info;
  list_node_t* new_node = malloc(sizeof(list_node_t));
  new_node->event_info = event;

  // Check if the event can be added to the calendar
  if (*list == NULL) {
    // If the list is empty, add the event to the beginning of the list
    new_node->next_event = NULL;
    new_node->prev_event = NULL;
    *list = new_node;
  }
  else {
    list_node_t *curr = *list;
    while (curr->next_event != NULL) {
      // Check if the event can be added before the current event in the list
      if (curr->prev_event == NULL) {
        if (timecmp(event->end_time, curr->event_info->start_time) < 0) {
          new_node->prev_event = NULL;
                new_node->next_event = curr;
                curr->prev_event = new_node;
                *list = new_node;
                break;
              }
              else {
                curr = curr->next_event;
              }
            }
            else if ((timecmp(event->end_time, curr->event_info->start_time) < 0) &&
               (timecmp(event->start_time, curr->prev_event->event_info->end_time) > 0)) {
                new_node->prev_event = curr->prev_event;
                new_node->next_event = curr;
                curr->prev_event->next_event = new_node;
                curr->prev_event = new_node;
                break;
            }
            else {
              curr = curr->next_event;
            }
        }
        if (curr->next_event == NULL) {
          // If the event cannot be added before any event in the list, add it to the end of the list
          if (timecmp(event->start_time, curr->event_info->end_time) > 0) {
            new_node->next_event = NULL;
            new_node->prev_event = curr;
            curr->next_event = new_node;
          }
          else if (timecmp(event->end_time, curr->event_info->start_time) < 0) {
            new_node->prev_event = curr->prev_event;
            new_node->next_event = curr;
            curr->prev_event->next_event = new_node;
            curr->prev_event = new_node;
          }
        }
    }
    add_to_calendar(tree->right_child_ptr, list);
    while ((*list) && (*list)->prev_event != NULL) {
      *list = (*list)->prev_event;
    }*/

} /* add_to_calendar() */

/*
 * This function frees up the schedule for the list if the space between the
 * events is less than the minimum time allowed
 */

void free_up_schedule(list_node_t **calendar, int min_time) {
  assert(calendar != NULL);
  list_node_t *current_node = *calendar;
  while ((current_node != NULL) && (current_node->next_event != NULL)) {
    long curr_duration = duration(
                         current_node->next_event->event_info->start_time,
                         current_node->event_info->end_time);
    if (curr_duration < min_time) {
      // remove the next event from the calendar
      list_node_t *node_to_delete = current_node->next_event;
      current_node->next_event = node_to_delete->next_event;
      if (node_to_delete->next_event != NULL) {
        node_to_delete->next_event->prev_event = current_node;
      }
      free(node_to_delete);
    }
    else {
      current_node = current_node->next_event;
    }
  }
} /* free_up_schedule() */

/*
 * This function deletes the entire tree event schedule using recursion
 */

void delete_event_schedule(tree_node_t **event_schedule) {
  assert(event_schedule != NULL);

  if (*event_schedule == NULL) {
    return;
  }

  delete_event_schedule(&((*event_schedule)->left_child_ptr));
  delete_event_schedule(&((*event_schedule)->right_child_ptr));

  free((*event_schedule)->event_info->name);
  free((*event_schedule)->event_info);
  free(*event_schedule);

  *event_schedule = NULL;

} /* delete_event_schedule() */