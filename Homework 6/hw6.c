/* Name, hw6.c, CS 24000, Spring 2023
 * Last updated Jan 2, 2023
 */

/* Add any includes here */
#include "hw6.h"

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
 * we return the number of courses read.
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
 * This course will find the max credit hours out of all the prerequisites
 * restricted to a specific type of course for the course given as a parameter.
 * The first thing we do is check whether our course name and our content type
 * are valid or not using assert. We also check whether we actually have any
 * courses to iterate through. After that, we start iterating through our
 * g_course_array and first find a course in our array that matches our course
 * name. If we don't, then we return NOT_FOUND. After that, we need to iterate
 * through the number of prereqs. For this, we just iterate through MAX_PREREQS
 * and only count the number of prereqs that are not an empty string. From
 * there, we find the prereqs in the array, if at least one is not then we
 * return NOT_FOUND. Then we find whether the type for our prereqs matches
 * the type given as a parameter, and then return the max credit hours for
 * those prereqs that meet those conditions.
 */

int find_prerequisites(char *course_name, enum course_type_t type) {
  assert(course_name != NULL);
  int type_is_ok = 0;
  switch (type) {
    case LECTURE:
      type_is_ok = 1;
      break;
    case PSO:
      type_is_ok = 1;
      break;
    case LABORATORY:
      type_is_ok = 1;
      break;
    case RECITATION:
      type_is_ok = 1;
      break;
    case INDEPENDENT:
      type_is_ok = 1;
      break;
  }
  assert(type_is_ok == 1);

  if (g_course_count == 0) {
    return NO_COURSES;
  }

  int max_hours = 0;
  int credit_hours = 0;
  int course_found = 0;
  int type_found = 0;
  int prereq_found = 0;
  int num_prereq = 0;

  for (int i = 0; i < g_course_count; i++) {
    if (strncmp(course_name, g_course_array[i].course_name,
        strlen(course_name)) == 0) {
      course_found++;
      for (int j = 0; j < MAX_PREREQ; j++) {
        char *prereq = g_course_array[i].prerequisites[j];
        if (strcmp(prereq, "") != 0) {
          num_prereq++;
        }
        for (int k = 0; k < g_course_count; k++) {
          if ((strncmp(prereq, g_course_array[k].course_name,
              strlen(prereq)) == 0) && (strcmp(prereq , "") != 0)) {
            prereq_found++;
            if (g_course_array[k].course_type == type) {
              credit_hours = g_course_array[k].credit_hours;
              type_found++;
            }
            if (credit_hours > max_hours) {
              max_hours = credit_hours;
            }
            break;
          }
        }
      }
      break;
    }
  }
  if ((course_found == 0) || (prereq_found < num_prereq)) {
    return NOT_FOUND;
  }

  if (type_found == 0) {
    return 0;
  }

  return max_hours;
} /* find_prerequisites() */

/* Remember, you don't need a main function!
 * It is provided by hw6_main.c or hw6_test.o
 */