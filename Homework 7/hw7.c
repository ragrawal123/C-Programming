/* Name, hw7.c, CS 24000, Spring 2023
 * Last updated Fab 5, 2023
 */

/* Add any includes here */
#include "hw7.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


/* Define your global variables here */

int g_course_count = 0;
course_t g_course_array[MAX_COURSES] = {};

/* Define your functions here */

/*
 * This function read_courses takes in the file name as the parameter.
 * This file name contains data on different courses in some database, and each
 * course has prerequisites, the type of course it is, and the number of credit
 * hours it has all along with it in a formatted file. In order to read this
 * data properly into our global array, which we will use later, we need to
 * read each data point in individually. If the read-in was successful, we
 * increment the number of courses we read by 1 and at the end of the function,
 * we return the number of courses read. We are also setting all the pointers
 * to NULL right now and link them later.
 */

int read_courses(char *file_name) {
  //assert that the argument is valid, if so open file and check for errors
  assert(file_name != NULL);
  FILE *fp = NULL;
  fp = fopen(file_name, "r");
  if (!fp) {
    return NON_READABLE_FILE;
  }

  int courses_read = 0;
  int course_count = 0;
  while (1) {

    //check at beginning of while loop if courses_read is greater than allowed

    if (courses_read > MAX_COURSES) {
      fclose(fp);
      fp = NULL;
      return TOO_MUCH_DATA;
    }

    //read in first data, name of course

    char buff[MAX_BUFF_LEN] = "";
    int success = 0;
    success = fscanf(fp, "%199[^|]|", buff);
    if (success == 0) {
      fclose(fp);
      fp = NULL;
      return BAD_RECORD;
    }
    if (success == EOF) {
      break;

    }

    //copy the buffer into array, only grab MAX_NAME_LEN-1 amount
    strncpy(g_course_array[course_count].course_name, buff, MAX_NAME_LEN - 1);
    g_course_array[course_count].course_name[MAX_NAME_LEN] = '\0';
    int counter = 0;

    /* use another while loop to read in prereqs since don't know how many there
       are in the data. */

    while (1) {

      /* read in the prereq into a buffer and check whether amount of prereqs
         is greater than what is allowed */

      char prereq_buff[MAX_BUFF_LEN] = "";
      success = fscanf(fp, "?%199[^?|]", prereq_buff);
      counter++;
      if (counter > MAX_PREREQ) {
        fclose(fp);
        fp = NULL;
        return BAD_RECORD;
      }
      if (counter == MAX_PREREQ) {

        //use strncpy to get the prereq from the buffer into our global array

        strncpy(g_course_array[course_count].prerequisites[counter - 1],
          prereq_buff, MAX_NAME_LEN - 1);
        g_course_array[course_count].prerequisites[counter - 1][MAX_NAME_LEN]
          = '\0';
        break;
      }
      else {

        //use strncpy to get the prereq from the buffer into our global array

        strncpy(g_course_array[course_count].prerequisites[counter - 1],
          prereq_buff, MAX_NAME_LEN - 1);
        g_course_array[course_count].prerequisites[counter - 1][MAX_NAME_LEN]
          = '\0';
      }

      g_course_array[course_count].prerequisite_course_ptrs[counter - 1] = NULL;
    }

    //read in the content type

    unsigned int type = 0;
    success = fscanf(fp, "|%u", &type);
    if (success == 0) {
      fclose(fp);
      fp = NULL;
      return BAD_RECORD;
    }
    switch (type) {
      case LECTURE:
        g_course_array[course_count].course_type = LECTURE;
        break;
      case PSO:
        g_course_array[course_count].course_type = PSO;
        break;
      case LABORATORY:
        g_course_array[course_count].course_type = LABORATORY;
        break;
      case RECITATION:
        g_course_array[course_count].course_type = RECITATION;
        break;
      case INDEPENDENT:
        g_course_array[course_count].course_type = INDEPENDENT;
        break;
    }

    //read in the credit hours

    success = fscanf(fp, "|%d\n", &g_course_array[course_count].credit_hours);
    if (success == 0) {
      fclose(fp);
      fp = NULL;
      return BAD_RECORD;
    }
    courses_read++;
    course_count++;
  }

  if (courses_read == 0) {
    fclose(fp);
    fp = NULL;
    return NO_COURSES;
  }
  g_course_count = courses_read;
  fclose(fp);
  fp = NULL;
  return courses_read;

} /* read_courses() */

/*
 * This function takes no parameters and returns nothing. It returns nothing as
 * well. What this function does is that it goes through the g_course_array
 * that we read in before and goes through every pointer for each structure
 * and chains courses to their respective prerequisites, which are also courses
 * inside of the array. If that course is not there or the prereq is an empty
 * string, then the pointer will point to nothing and just be NULL.
 */

void link_courses() {
  int found_course = 0;
  for (int i = 0; i < g_course_count; i++) {
    for (int j = 0; j < MAX_PREREQ; j++) {
      char *prereq = g_course_array[i].prerequisites[j];
      if (strcmp(prereq , "") == 0) {
        g_course_array[i].prerequisite_course_ptrs[j] = NULL;
      }
      for (int k = 0; k < g_course_count; k++) {
        if ((strncmp(prereq, g_course_array[k].course_name,
            strlen(prereq)) == 0) && (strcmp(prereq , "") != 0)) {
          g_course_array[i].prerequisite_course_ptrs[j] = &g_course_array[k];
          found_course++;
          break;
        }
      }
      if (found_course == 0) {
        g_course_array[i].prerequisite_course_ptrs[j] = NULL;
      }
    }
  }
} /* link_courses() */

/*
 * This function finds the longest chain of a course, respective to the prefix
 * course code that is given, and returns the length of the longest chain of
 * courses in our respective array. This is done by first checking whether
 * the course code yields a valid course in our array. If it doesn't we return
 * NO_COURSES. If it does, we move on to check our first prerequisites for that
 * course. For each prerequisite, we loop infinitely until the prerequisite
 * equals NULL and each time we will increase the chain value because if the
 * prerequisite is not NULL then that means there is a course in the chain
 * there.
 */

int find_longest_chain(char *course_code) {
  assert(course_code != NULL);
  int longest_chain = 0;
  int correct_course = -1;
  for (int i = 0; i < g_course_count; i++) {
    if (strncmp(g_course_array[i].course_name,
        course_code, strlen(course_code)) == 0) {
      correct_course = i;
    }
  }
  if (correct_course == -1) {
    return NO_COURSES;
  }
  for (int j = 0; j < MAX_PREREQ; j++) {
    int chain = 0;
    struct course *prerequisite =
      g_course_array[correct_course].prerequisite_course_ptrs[j];
    while (prerequisite != NULL) {
      chain++;
      prerequisite = prerequisite->prerequisite_course_ptrs[j];
    }
    if (chain > longest_chain) {
      longest_chain = chain;
    }
  }
  return longest_chain;
} /* find_longest_chain() */

/*
 * This function does a similar function to the find_longest_chain() function.
 * It takes in an index, and if valid, this index will be some course in our
 * array. From there, we will check each chain of prerequisites and find the
 * total number of credit hours each chain has. Each time we are going to check
 * for a max and return that max value. We do this in a similar process to the
 * previous function, where we check each prerequisite. If not NULL, we
 * increase the number of credit hours and check once done with the while loop.
 * We will then return the max credit hours for that chain.
 */

int find_max_hours_chain(int index) {
  assert((index >= 0) && (index < g_course_count));
  int max_hours = 0;
  for (int i = 0; i < MAX_PREREQ; i++) {
    int credit_hours = 0;
    struct course *prerequisite =
      g_course_array[index].prerequisite_course_ptrs[i];
    while (prerequisite != NULL) {
      credit_hours += prerequisite->credit_hours;
      prerequisite = prerequisite->prerequisite_course_ptrs[i];
    }
    if (credit_hours > max_hours) {
      max_hours = credit_hours;
    }
  }
  return max_hours;
} /* find_max_hours_chain() */


/* Remember, you don't need a main function!
 * It is provided by hw7_main.c or hw7_test.o
 */