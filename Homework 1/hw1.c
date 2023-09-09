/* Name, hw1.c, CS 24000, Spring 2023
 * Last updated Nov 15, 2022
 */

/* Add any includes here */
#include "hw1.h"

int g_recaman_array[ARRAY_SIZE];

/* Define create_recaman_sequence here */

/*
 * This function creates the recaman sequence given the starting
 * argument that is to be the first number in the sequence and
 * the number of elements to be generated for the recaman
 * sequence. It checks whether all the inputs for the function
 * are valid, and then loops through the number of elements to
 * calculate the recaman sequence.
 */

int create_recaman_sequence(int starting_arg, int num_elements) {
  if ((num_elements > ARRAY_SIZE) || (num_elements <= 0)) {
    return RECAMAN_ERROR;
  }
  else if (starting_arg < 0) {
    return RECAMAN_ERROR;
  }
  else {
    g_recaman_array[0] = starting_arg;
    for (int i = 1; i < num_elements; i++) {
      int next_num = g_recaman_array[i - 1] - i;

      for (int j = 0; j < i; j++) {
        if ((next_num == g_recaman_array[j]) || (next_num <= 0)) {
          next_num = g_recaman_array[i - 1] + i;
          break;
        }
      }
      g_recaman_array[i] = next_num;
    }
  }
  return RECAMAN_CORRECT;
} /* create_recaman_sequence() */

/* Define check_recaman_sequence here */

/*
 * This function checks whether the recaman sequence given by
 * the user is valid or not given the number of elements, which
 * is represented in this function by the limit.
 */

int check_recaman_sequence(int limit) {
  if (limit <= 0) {
    return RECAMAN_ERROR;
  }
  else if (limit > ARRAY_SIZE) {
    return RECAMAN_ERROR;
  }
  if (g_recaman_array[0] < 0) {
    return 0;
  }
  for (int i = 1; i < limit; i++) {
    int curr_num = g_recaman_array[i - 1] - i;
    for (int j = 0; j < i; j++) {
      if ((curr_num == g_recaman_array[j]) || (curr_num <= 0)) {
        curr_num = g_recaman_array[i - 1] + i;
        break;
      }
    }
    if (curr_num != g_recaman_array[i]) {
      return i;
    }
  }
  return RECAMAN_CORRECT;
} /* check_recaman_sequence() */

/* Remember, you don't need a main function!
 * It is provided by hw1_main.c or hw1_test.o
 */