/* Name, hw2.c, CS 24000, Spring 2023
 * Last updated December 3, 2022
 */

/* Add any includes here */
#include "hw2.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

/* Define your functions here */

/*
 * This function calculates the average speed for a manufacturer
 * from a file containing information for different racing teams
 * and their cars. It takes the file which has the different cars'
 * information and the manufacturer we are trying to find the average
 * speed of their cars for.
 */

float average_speed_of_manufacturer(char *file_name, char *manufacturer) {
  FILE *fp = NULL;
  fp = fopen(file_name, "r");
  if (!fp) {
    return FILE_READ_ERR; //return error if file doesn't exist
  }
  //declare vars for use in reading info from file
  char id_buffer[MAX_ID_LENGTH];
  char manf_buffer[MAX_FIELD_LENGTH];
  float test_distance = 0.0;
  float test_time = 0.0;
  int pitstops = 0;
  int distance = 0;
  float pitstop_time = 0.0;
  float cost = 0.0;
  double breakdown_rate = 0.0;
  float avg_speed = 0.0;
  int car_count = 0;
  int success = 0;
  while (1) {
    success = fscanf(fp, "%4[^,],%49[^,],%fkm,%fs,%d/%d/%f,$%f,%lf%%\n",
      id_buffer, manf_buffer, &test_distance, &test_time, &pitstops, &distance,
      &pitstop_time, &cost, &breakdown_rate); //read information from file
    if ((success < 9) && (success != -1)) {
      fclose(fp);
      fp = NULL;
      return BAD_RECORD; //return error if have corrupted data points in file
    }
    if (success == -1) {
      break; //break out of while loop if EOF
    }
    if (strcmp(manf_buffer, manufacturer) == 0) {
      car_count++;
      avg_speed += ((test_distance * 1000) / test_time); //compute average speed
    }
  }
  if (car_count == 0) {
    fclose(fp);
    fp = NULL;
    return NO_DATA_POINTS; //return error if no cars for given manufacturer
  }
  avg_speed = avg_speed / car_count;
  fclose(fp);
  fp = NULL;
  return avg_speed;
} /* average_speed_of_manufacturer() */

/*
 * This function calculates the expected number of pitstops for the given id,
 * which is given as a parameter and corresponds to an entry in the file. It
 * is found by dividing the distance from the RACE LENGTH, and then
 * multiplying by the amount of pitstops per distance. It takes in the file
 * name and the id we are looking for as parameters.
 */

int expected_pitstops(char *file_name, char *id) {
  FILE *fp = NULL;
  fp = fopen(file_name, "r");
  if (!fp) {
    return FILE_READ_ERR; //return error if file doesn't exist
  }
  //declare vars for use in reading info from file
  char id_buffer[MAX_ID_LENGTH];
  char manf_buffer[MAX_FIELD_LENGTH];
  float test_distance = 0.0;
  float test_time = 0.0;
  int pitstops = 0;
  int distance = 0;
  float pitstop_time = 0.0;
  float cost = 0.0;
  double breakdown_rate = 0.0;
  int success = 0;
  float expected_pitstops = 0.0;
  int id_count = 0;
  if (strlen(id) > MAX_ID_LENGTH) {
    fclose(fp);
    fp = NULL;
    return BAD_ID;
  }
  for (int i = 0; i < MAX_ID_LENGTH - 1; i++) {
    if (isdigit(id[i]) == 0) {
      fclose(fp);
      fp = NULL;
      return BAD_ID;
    }
  }
  while (1) {
    success = fscanf(fp, "%4[^,],%49[^,],%fkm,%fs,%d/%d/%f,$%f,%lf%%\n",
      id_buffer, manf_buffer, &test_distance, &test_time, &pitstops, &distance,
      &pitstop_time, &cost, &breakdown_rate); //read information from file
    if ((success < 9) && (success != -1)) {
      fclose(fp);
      fp = NULL;
      return BAD_RECORD;
      //return error if there are corrupted data points in file
    }
    if (success == -1) {
      break; //break out of while loop if EOF
    }
    if (strcmp(id_buffer, id) == 0) {
      id_count++;
      expected_pitstops = (RACE_LENGTH / (float)distance) * pitstops;
      expected_pitstops = ceil(expected_pitstops);
    }
  }
  if (id_count == 0) {
    fclose(fp);
    fp = NULL;
    return NO_DATA_POINTS;
  }
  fclose(fp);
  fp = NULL;
  return (int)expected_pitstops;
} /* expected_pitstops() */

/*
 * This function finds the max total pitstop time for the given manufacturer
 * that is given as a parameter. It is found by traversing through the file,
 * calculating the total pitstop time for each entry corresponding to the
 * manufacturer, and then returning the max one. It takes the file name and
 * the manufacturer name as parameters.
 */

float find_max_total_pitstop(char *file_name, char *manufacturer) {
  FILE *fp = NULL;
  fp = fopen(file_name, "r");
  if (!fp) {
    return FILE_READ_ERR; //return error if file doesn't exist
  }
  //declare vars for use in reading info from file
  char id_buffer[MAX_ID_LENGTH];
  char manf_buffer[MAX_FIELD_LENGTH];
  float test_distance = 0.0;
  float test_time = 0.0;
  int pitstops = 0;
  int distance = 0;
  float pitstop_time = 0.0;
  float cost = 0.0;
  double breakdown_rate = 0.0;
  int pitstop_count = 0;
  float total_time = 0.0;
  float temp_max = 0.0;
  int success = 0;
  while (1) {
    success = fscanf(fp, "%4[^,],%49[^,],%fkm,%fs,%d/%d/%f,$%f,%lf%%\n",
      id_buffer, manf_buffer, &test_distance, &test_time, &pitstops, &distance,
      &pitstop_time, &cost, &breakdown_rate); //read information from file
    if ((success < 9) && (success != -1)) {
      fclose(fp);
      fp = NULL;
      return BAD_RECORD; //return error if have corrupted data points in file
    }
    if (success == -1) {
      break; //break out of while loop if EOF
    }
    if (strcmp(manf_buffer, manufacturer) == 0) {
      pitstop_count++;
      total_time = (float)expected_pitstops(file_name, id_buffer)
      * pitstop_time;
      if (total_time > temp_max) {
        temp_max = total_time;
      }
    }
  }
  if (pitstop_count == 0) {
    fclose(fp);
    fp = NULL;
    return NO_DATA_POINTS;
    //return error if there are no data
  }
  fclose(fp);
  fp = NULL;
  total_time = temp_max;
  return total_time;
} /* find_max_total_pitstop() */

/*
 * This function generates a cost report for a given file. What this means is
 * that for every entry in the in_file, There will be a expected cost report
 * generated that finds the expected breakdown cost for every entry. This is
 * done by multiplying the cost by the breakdown rate. This function takes in
 * the in_file we are reading from and the out_file we are writing to as
 * parameters.
 */

int generate_expected_cost_report(char *in_file, char *out_file) {
  FILE *fp_in = NULL;
  fp_in = fopen(in_file, "r");
  if (!fp_in) {
    return FILE_READ_ERR; //return error if file doesn't exist
  }
  FILE *fp_out = NULL;
  fp_out = fopen(out_file, "w");
  if (!fp_out) {
    fclose(fp_in);
    fp_in = NULL;
    return FILE_WRITE_ERR;
  }
  //declare vars for use in reading info from file
  char id_buffer[MAX_ID_LENGTH];
  char manf_buffer[MAX_FIELD_LENGTH];
  float test_distance = 0.0;
  float test_time = 0.0;
  int pitstops = 0;
  int distance = 0;
  float pitstop_time = 0.0;
  float cost = 0.0;
  double breakdown_rate = 0.0;
  int data_count = 0;
  float expected_br_cost = 0.0;
  int success = 0;
  while (1) {
    success = fscanf(fp_in, "%4[^,],%49[^,],%fkm,%fs,%d/%d/%f,$%f,%lf%%\n",
      id_buffer, manf_buffer, &test_distance, &test_time, &pitstops, &distance,
      &pitstop_time, &cost, &breakdown_rate); //read information from file
    if ((success < 9) && (success != -1)) {
      fclose(fp_in);
      fclose(fp_out);
      fp_in = NULL;
      fp_out = NULL;
      return BAD_RECORD; //return error if have corrupted data points in file
    }
    if (success == -1) {
      break; //break out of while loop if EOF
    }
    data_count++;
    expected_br_cost = cost * (breakdown_rate / 100.0);
    fprintf(fp_out,
      "Car number %s made by %s has a breakdown cost expectancy of $%.2f.\n",
      id_buffer, manf_buffer, expected_br_cost);
  }
  if (data_count == 0) {
    fclose(fp_in);
    fclose(fp_out);
    fp_in = NULL;
    fp_out = NULL;
    return NO_DATA_POINTS;
  }
  fclose(fp_in);
  fclose(fp_out);
  fp_in = NULL;
  fp_out = NULL;
  return OK;
} /* generate_expected_cost_report() */



/* Remember, you don't need a main function!
 * It is provided by hw2_main.c or hw2_test.o
 /