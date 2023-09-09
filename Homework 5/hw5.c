/* Name, hw5.c, CS 24000, Spring 2023
 * Last updated Dec 27, 2022
 */

/* Add any includes here */
#include "hw5.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define your functions here */

/*
 * This function checks if the character inputted is an alpha-numeric
 * character. This is done by checking whether the ASCII value of the
 * character is either a number or a letter. If it is, the function
 * returns 1 to the assert and 0 otherwise.
 */

int is_alpha_numeric(char character) {
  if (((character >= '0') && (character <= '9')) ||
    ((character >= 'a') && (character <= 'z')) ||
    ((character >= 'A') && (character <= 'Z'))) {
    return 1;
  }
  return 0;
} /* is_alpha_numeric() */

/*
 * This function is used for the storage assert to check whether
 * the storage for the cupboard is valid or not. This makes our job easier
 * to check whether the storage is valid by using this helper function.
 */

int check_storage(cupboard_t cupboard) {
  int bad_storage = 0;
  for (int i = 0; i < N_HEIGHT; i++) {
    for (int j = 0; j < N_WIDTH; j++) {
      if (cupboard.storage[i][j] < 0) {
        bad_storage = 1;
      }
    }
  }
  return bad_storage;
} /* check_storage() */

/*
 * This function is a helper function that gets the current weight inside a
 * cupboard that is given as a parameter. It does this by looping through the
 * double for loop and adding the individual weights up. It returns the current
 * weight value.
 */

float get_curr_weight(cupboard_t cupboard) {
  float curr_weight = 0;
  for (int i = 0; i < N_HEIGHT; i++) {
    for (int j = 0; j < N_WIDTH; j++) {
      curr_weight += cupboard.storage[i][j];
    }
  }
  return curr_weight;
} /* get_curr_weight() */

/*
 * This function reads the cupboard. It takes in the open file and the
 * index we want to read as its parameter. It does some assert and error checks
 * and then reads from the binary file into the cupboard address. It then
 * returns that cupboard.
 */

cupboard_t read_cupboard(FILE *fp, int index) {
  assert(fp != NULL);
  assert(index >= 0);
  cupboard_t cupboard = {};

  if (fseek(fp, index * sizeof(cupboard_t), SEEK_SET) != 0) {
    return BAD_CUPBOARD;
  }

  if (fread(&cupboard, sizeof(cupboard_t), 1, fp) != 1) {
    return BAD_CUPBOARD;
  }

  return cupboard;
} /* read_cupboard() */

/*
 * This function writes the cupboard back out to the opened file. It takes in
 * the file, the cupboard, and the index we are writing to as its parameters.
 * It does a series of assert checks and then does the writing to the file
 * using fwrite().
 */

int write_cupboard(FILE *fp, cupboard_t cupboard, int index) {
  assert(fp != NULL);
  assert(index >= 0);

  assert(strlen(cupboard.id) == MAX_ID_LEN - 1);
  assert(is_alpha_numeric(cupboard.id[0]) == 1);
  assert(is_alpha_numeric(cupboard.id[1]) == 1);
  assert(is_alpha_numeric(cupboard.id[2]) == 1);
  assert(is_alpha_numeric(cupboard.id[3]) == 1);

  assert(cupboard.max_weight > 0);
  assert(cupboard.age >= 0);

  int content_success = 0;
  switch (cupboard.content) {
    case FOOD:
      content_success = 1;
      break;
    case CLOTH:
      content_success = 1;
      break;
    case ELECTRONICS:
      content_success = 1;
      break;
  }
  assert(content_success == 1);

  int material_success = 0;
  switch (cupboard.material) {
    case STEEL:
      material_success = 1;
      break;
    case WOOD:
      material_success = 1;
      break;
    case PLASTIC:
      material_success = 1;
      break;
    case CARDBOARD:
      material_success = 1;
      break;
  }
  assert(material_success == 1);

  assert(check_storage(cupboard) == 0);
  //do ftell and check if index * sizeof is greater than

  fseek(fp, 0, SEEK_END);
  int num_cupboards = ftell(fp) / sizeof(cupboard_t);
  fseek(fp, 0, SEEK_SET);
  if (index >= num_cupboards) {
    return WRITE_ERROR;
  }

  if (fseek(fp, index * sizeof(cupboard_t), SEEK_SET) != 0) {
    return WRITE_ERROR;
  }

  if (fwrite(&cupboard, sizeof(cupboard_t), 1, fp) != 1) {
    return WRITE_ERROR;
  }
  return OK;
} /* write_cupboard() */

/*
 * This function finds the actual max weight a cupboard can hold given
 * the material it is made from due to depreciation. It takes the file and the
 * id as a parameter and returns the max weight the function can hold. This
 * is done through a for loop and doing checks and math calculations to find
 * the actual max weight.
 */

float find_actual_max_weight(FILE *fp, char *id) {
  assert(fp != NULL);
  assert(id != NULL);

  cupboard_t cupboard = {};
  int success = 0;
  float max_weight = 0.0;

  fseek(fp, 0, SEEK_END);
  long len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  int iterations = len / sizeof(cupboard_t);
  for (int i = 0; i < iterations; i++) {
    cupboard = read_cupboard(fp, i);
    if (strcmp(cupboard.id , id) == 0) {
      success++;
      max_weight = cupboard.max_weight;
      int age = cupboard.age;

      switch (cupboard.material) {
        case STEEL:
          max_weight *= pow(0.95, (ceil((float) age / 5.0)));
          break;
        case WOOD:
          max_weight *= pow(0.90, (ceil((float) age / 7.0)));
          break;
        case PLASTIC:
          max_weight *= pow(0.88, (ceil((float) age / 3.0)));
          break;
        case CARDBOARD:
          max_weight *= pow(0.75, (ceil((float) age / 2.0)));
          break;
      }
      return max_weight;
    }
  }

  if (success == 0) {
    return NO_SUCH_CUPBOARD;
  }

  return max_weight;

} /* find_actual_max_weight() */

/*
 * This function finds the first overloaded cupboard in the file.
 * An overloaded cupboard means that the current weight it is holding
 * is greater than the actual max weight the cupboard is allowed to hold.
 * It takes the file as the parameter and returns the index of the cupboard.
 */

int find_overloaded_cupboard(FILE *fp) {
  assert(fp != NULL);

  cupboard_t cupboard = {};

  fseek(fp, 0, SEEK_END);
  long len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  int iterations = len / sizeof(cupboard_t);

  float max_weight = 0 ;
  float actual_weight = 0;
  for (int i = 0; i < iterations; i++) {
    cupboard = read_cupboard(fp, i);
    max_weight = find_actual_max_weight(fp, cupboard.id);
    actual_weight = get_curr_weight(cupboard);
    if (actual_weight > max_weight) {
      return i;
    }
  }
  return OK;
} /* find_overloaded_cupboard() */

/*
 * This function finds the potential storage space all cupboards have in the
 * file for the given content type. It does this through a series of checks
 * and then finds the potential storage space allowed. It returns that value.
 */

float potential_storage_space(FILE *fp, enum content_t given_content) {
  assert(fp != NULL);
  int content_success = 0;
  switch (given_content) {
    case FOOD:
      content_success = 1;
      break;
    case CLOTH:
      content_success = 1;
      break;
    case ELECTRONICS:
      content_success = 1;
      break;
  }
  assert(content_success == 1);

  fseek(fp, 0, SEEK_END);
  long len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  cupboard_t cupboard = {};

  int iterations = len / sizeof(cupboard_t);
  float total_weight_allowed = 0;
  float current_weight = 0;
  int success = 0;
  float difference = 0;
  for (int i = 0; i < iterations; i++) {
    cupboard = read_cupboard(fp, i);
    if (cupboard.content == given_content) {
      success++;
      total_weight_allowed = find_actual_max_weight(fp, cupboard.id);
      current_weight = get_curr_weight(cupboard);
      if (current_weight > total_weight_allowed) {
        difference += 0;
      }
      else {
        difference += (total_weight_allowed - current_weight);
      }
    }
  }

  if (success == 0) {
    return NO_SUCH_CONTENT;
  }

  return difference;
} /* potential_storage_space() */

/*
 * This function places an object to a cupboard that matches with a certain id
 * given parameters of the id, the objects width and height and mass and the
 * position you want to put it in. This function does a series of assert and
 * error checks and then returns OK if we are able to put the object into
 * the cupboard matching the ID.
 */

int place_object(FILE *fp, int width, int height, int mass, char *id,
                 int x_position, int y_position) {

  assert(fp != NULL);
  assert(width > 0);
  assert(height > 0);
  assert(mass > 0);
  assert(id != NULL);
  assert(strlen(id) == MAX_ID_LEN - 1);
  assert(is_alpha_numeric(id[0]) == 1);
  assert(is_alpha_numeric(id[1]) == 1);
  assert(is_alpha_numeric(id[2]) == 1);
  assert(is_alpha_numeric(id[3]) == 1);
  assert((x_position >= 0) && (x_position < N_WIDTH));
  assert((y_position >= 0) && (y_position < N_HEIGHT));
  fseek(fp, 0, SEEK_END);
  long len = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  int iterations = len / sizeof(cupboard_t);
  cupboard_t cupboard = {};
  int success = 0;

  float current_weight = mass + get_curr_weight(cupboard);

  if ((x_position + width) > N_WIDTH) {
    return SPACE_ISSUE;
  }
  if ((y_position + height) > N_HEIGHT) {
    return SPACE_ISSUE;
  }

  for (int i = 0; i < iterations; i++) {
    cupboard = read_cupboard(fp, i);
    if (strcmp(id, cupboard.id) == 0) {
      success++;
      if (current_weight > find_actual_max_weight(fp, cupboard.id)) {
        return WEIGHT_ISSUE;
      }
      for (int j = x_position; j < x_position + width; j++) {
        for (int k = y_position; k < y_position + height; k++) {
          //cupboard.storage[k][j] > 0 is space issue
          if (cupboard.storage[k][j] == 0) {
            cupboard.storage[k][j] = (float) mass / (height * width);
          }
          else {
            return SPACE_ISSUE;
          }
        }
      }
      write_cupboard(fp, cupboard, i);
    }
  }

  if (success == 0) {
    return NO_SUCH_CUPBOARD;
  }
  return OK;
} /* place_object() */

/* Remember, you don't need a main function!
 * It is provided by hw5_main.c or hw5_test.o
 */