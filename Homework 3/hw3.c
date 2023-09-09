/* Name, hw3.c, CS 24000, Spring 2023
 * Last updated Dec 8, 2022
 */

/* Add any includes here */
#include "hw3.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

/* Define your global variables here */
int g_player_count = 0;
char g_player_name[MAX_PLAYERS][NUM_ATTR][MAX_NAME_LEN] = {0};
int g_attributes[MAX_PLAYERS][NUM_ATTR] = {0};
int g_shooting[MAX_PLAYERS][NUM_COLS] = {0};
int g_statistics[MAX_PLAYERS][NUM_COLS] = {0};

/* Define your functions here */

/*
 * This function reads in the data from the input file and assigns the
 * data into different variables and arrays. It takes in the input file
 * as its input, and returns the number of players read. It utilizes a
 * number of different checks in order to make sure that the table is read
 * correctly.
 */

int read_tables(char *in_file) {
  //open file
  FILE *fp = NULL;
  fp = fopen(in_file, "r");
  if (!fp) {
    return FILE_READ_ERR;
  }
  //declare variables
  char player_name_buff[MAX_NAME_LEN] = "";
  char pos_buff[MAX_NAME_LEN] = "";
  char height_unit_buff[MAX_UNIT_LEN] = "";
  char weight_unit_buff[MAX_UNIT_LEN] = "";
  int height = 0;
  int weight = 0;
  int att_2 = 0;
  int made_2 = 0;
  int att_3 = 0;
  int made_3 = 0;
  int att_ft = 0;
  int made_ft = 0;
  int rebounds = 0;
  int assists = 0;
  int steals = 0;
  int blocks = 0;
  int turnovers = 0;
  int fouls = 0;
  int success = 0;
  int i = 0;
  int players_read = 0;
  //loop to read all the data
  while (1) {
    //read in data
    success = fscanf(fp,
      "%49[^,],%49[^|]|%d%3[^,],%d%3[^|]|%d,%d,%d,%d,%d,%d|%d,%d,%d,%d,%d,%d\n",
      player_name_buff, pos_buff, &height, height_unit_buff, &weight,
      weight_unit_buff, &att_2, &made_2, &att_3, &made_3, &att_ft, &made_ft,
      &rebounds, &assists, &steals, &blocks, &turnovers, &fouls);

    //check if players read so far is greater than amount allowed
    if (players_read >= MAX_PLAYERS) {
      g_player_count = players_read;
      fclose(fp);
      fp = NULL;
      return OUT_OF_BOUNDS;
    }
    //check for end of file
    if (success == EOF) {
      break;
    }
    //check for bad data
    if ((success < 18) && (success != -1)) {
      g_player_count = players_read;
      fclose(fp);
      fp = NULL;
      return RECORD_ERROR;
    }
    else if ((strcmp(height_unit_buff, "in") != 0)
      && (strcmp(height_unit_buff, "cm") != 0)) {
      g_player_count = players_read;
      fclose(fp);
      fp = NULL;
      return RECORD_ERROR;
    }
    else if ((strcmp(weight_unit_buff, "kg") != 0)
      && ((strcmp(weight_unit_buff, "lbs")) != 0)) {
      g_player_count = players_read;
      fclose(fp);
      fp = NULL;
      return RECORD_ERROR;
    }

    if ((att_2 < 0) || (made_2 < 0) || (att_3 < 0) || (made_3 < 0)
      || (att_ft < 0) || (made_ft < 0)) {
      g_player_count = players_read;
      fclose(fp);
      fp = NULL;
      return BAD_DATA;
    }

    if ((att_2 < made_2) || (att_3 < made_3) || (att_ft < made_ft)) {
      fclose(fp);
      fp = NULL;
      return BAD_DATA;
    }

    //convert to in and lbs if needed
    if (strcmp(height_unit_buff, "in") != 0) {
      height = (int) ((height * IN_PER_CM) + 0.5);
    }

    if (strcmp(weight_unit_buff, "lbs") != 0) {
      weight = (int)((weight * LB_PER_KG) + 0.5);
    }

    //assign data to arrays
    strncpy(g_player_name[i][0], player_name_buff, MAX_NAME_LEN);
    strncpy(g_player_name[i][1], pos_buff, MAX_NAME_LEN);
    g_attributes[i][0] = height;
    g_attributes[i][1] = weight;
    for (int j = 0; j < NUM_COLS; j++) {
      switch (j) {
        case 0:
          g_shooting[i][j] = att_2;
          g_statistics[i][j] = rebounds;
          break;
        case 1:
          g_shooting[i][j] = made_2;
          g_statistics[i][j] = assists;
          break;
        case 2:
          g_shooting[i][j] = att_3;
          g_statistics[i][j] = steals;
          break;
        case 3:
          g_shooting[i][j] = made_3;
          g_statistics[i][j] = blocks;
          break;
        case 4:
          g_shooting[i][j] = att_ft;
          g_statistics[i][j] = turnovers;
          break;
        case 5:
          g_shooting[i][j] = made_ft;
          g_statistics[i][j] = fouls;
          break;
      }
    }
    players_read++;
    i++;
  }

  if (players_read == 0) {
    g_player_count = players_read;
    fclose(fp);
    fp = NULL;
    return NO_DATA_POINTS;
  }

  fclose(fp);
  fp = NULL;
  g_player_count = players_read;
  return players_read;

} /* read_tables() */

/*
 * This function finds the top scorer in the data given from the arrays.
 * It takes in nothing as its input and outputs the index of the player who
 * scored the most.
 */

int find_top_scorer(void) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }

  int max = 0;
  int index = 0;
  for (int i = 0; i < g_player_count; i++) {
    int total_points = 0;
    for (int j = 0; j < NUM_COLS; j++) {
      switch (j) {
        case 1:
          total_points += (g_shooting[i][j] * 2);
          break;
        case 3:
          total_points += (g_shooting[i][j] * 3);
          break;
        case 5:
          total_points += (g_shooting[i][j] * 1);
          break;
      }
    }
    if (total_points > max) {
      max = total_points;
      index = i;
    }
    else if (total_points == max) {
      if (i < index) {
        index = i;
      }
    }
    //if the max and total points are the same, get the index that came first
  }
  return index;
} /* find_top_scorer() */

/*
 * This function finds the most accurate player from the data given.
 * This is done by calculating the made shots and dividing it from
 * the attempted shots to find their shooting percentage. From there
 * we find the index of the player who has the most accurate shot. This
 * function takes nothing as its input and outputs the index of the most
 * accurate player.
 */

int find_most_accurate_player(void) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }

  float max = 0.0;
  int has_attempted = 0;
  int index = 0;

  for (int i = 0; i < g_player_count; i++) {
    float percentage = 0.0;
    float attempted_shots = 0.0;
    float made_shots = 0.0;
    for (int j = 0; j < NUM_COLS; j++) {
      switch (j) {
        case 0:
          attempted_shots += g_shooting[i][j];
          break;
        case 1:
          made_shots += g_shooting[i][j];
          break;
        case 2:
          attempted_shots += g_shooting[i][j];
          break;
        case 3:
          made_shots += g_shooting[i][j];
          break;
        case 4:
          attempted_shots += g_shooting[i][j];
          break;
        case 5:
          made_shots += g_shooting[i][j];
          break;
      }
    }
    if (attempted_shots != 0) {
      has_attempted++;
    }
    //calculate percentage of shots made
    percentage = (made_shots / attempted_shots) * 100.0;
    if (percentage > max) {
      max = percentage;
      index = i;
    }
    else if (percentage == max) {
      //if max and percentage are same, do first tie breaker from most attempts
      int old_index_attempts = g_shooting[index][0] + g_shooting[index][2] +
        g_shooting[index][4];
      int new_index_attempts = g_shooting[i][0] + g_shooting[i][2] +
        g_shooting[i][4];
      if (new_index_attempts > old_index_attempts) {
        index = i;
      }
      else if (new_index_attempts == old_index_attempts) {
        //if attempts made are same, then do tiebreaker for shortest height
        if (g_attributes[i][1] > g_attributes[index][1]) {
          index = i;
        }
        else if (g_attributes[i][1] == g_attributes[index][1]) {
          if (i < index) {
            index = i;
          }
        }
      }
    }
  }

  if (has_attempted == 0) {
    return NO_ATTEMPTS;
  }
  return index;
} /* find_most_accurate_player() */

/*
 * This function counts the number of players in the given weight range.
 * This is done by checking for each player if their weight from the
 * g_attribute[][] array is within the range, which is inputted through
 * the lower and upper bound. We do checks to make sure the bounds are valid
 * and then return the number of players in the weight range.
 */

int count_players_in_weight_range(int lower_bound, int upper_bound) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  if (lower_bound > upper_bound) {
    return BAD_RANGE;
  }

  int num_players = 0;
  for (int i = 0; i < g_player_count; i++) {
    //check if within weight range
    if ((g_attributes[i][1] >= lower_bound) &&
      (g_attributes[i][1] <= upper_bound)) {
      num_players++;
    }
  }
  if (num_players == 0) {
    return NO_SUCH_DATA;
  }
  return num_players;
} /* count_players_in_weight_range() */

/*
 * This function returns the maximum number of attempts some player in a
 * certain position has made from the data given and stored in the different
 * arrays. This is done by calculating the number of total attempts for 2pt,
 * 3pt, and ft, and then finding the max.
 */

int find_most_attempts_player(char* given_position) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }

  int is_in_position = 0;
  int max = 0;
  for (int i = 0; i < g_player_count; i++) {
    int total_attempts = 0;
    if (strcasecmp(g_player_name[i][1], given_position) == 0) {
      is_in_position++;
      total_attempts = g_shooting[i][0] + g_shooting[i][2] + g_shooting[i][4];
      if (total_attempts > max) {
        max = total_attempts;
      }
    }
  }
  if (is_in_position == 0) {
    return NO_SUCH_DATA;
  }
  return max;
} /* find_most_attempts_player() */

/*
 * This function calculates the grade given to a certain player given as the
 * input player_name from the coach. This is done using a formula from the
 * coach and returns the grade. If the players grade is lower than 0, then
 * BAD_PLAYER is returned.
 */

float calculate_coach_grade(char* player_name) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }

  int is_player = 0;
  float points_alloted = 0.0;
  float divisor = 1.0;
  float grade = 0.0;
  for (int i = 0; i < g_player_count; i++) {
    if (strcasecmp(player_name, g_player_name[i][0]) == 0) {
      is_player++;
      for (int j = 0; j < NUM_COLS; j++) {
        //allotting values to the variables for the calculation
        switch (j) {
          case 0:
            points_alloted += (g_statistics[i][j] * 25);
            divisor += g_statistics[i][j];
            break;
          case 1:
            points_alloted += (g_statistics[i][j] * 20);
            divisor += g_statistics[i][j];
            break;
          case 2:
            points_alloted += (g_statistics[i][j] * 35);
            divisor += g_statistics[i][j];
            break;
          case 3:
            points_alloted += (g_statistics[i][j] * 25);
            divisor += g_statistics[i][j];
            break;
          case 4:
            points_alloted -= (g_statistics[i][j] * 15);
            divisor += g_statistics[i][j];
            break;
          case 5:
            points_alloted -= (g_statistics[i][j] * 20);
            divisor += g_statistics[i][j];
            break;
        }
      }
      grade = points_alloted / divisor;
      if (grade < 0.0) {
        return BAD_PLAYER;
      }
      return grade;
    }
  }

  if (is_player == 0) {
    return NO_SUCH_DATA;
  }
  return grade;

} /* calculate_coach_grade() */

/*
 * This function calculates the grades for all of the players in the given
 * global array and then writes them to an output file that is given as
 * out_file. Checks are done to make sure that there is no invalid input
 * and this function utilizes calculate_coach_grade() to make it easier
 * The function returns OK if the writing process was done successfully.
 */

int calculate_all_players_grade(char* out_file) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  FILE *fp = NULL;
  fp = fopen(out_file, "w");
  if (!fp) {
    return FILE_WRITE_ERR;
  }
  for (int i = 0; i < g_player_count; i++) {
    float grade = calculate_coach_grade(g_player_name[i][0]);
    if (grade == BAD_PLAYER) {
      grade = 0.0;
    }
    fprintf(fp, "%s: %.2f\n", g_player_name[i][0], grade);
  }
  fclose(fp);
  fp = NULL;
  return OK;
} /* calculate_all_players_grade() */

/*
 * This function writes the data stored in the global arrays back into a
 * different output file that is given as the out_file variable. It also takes
 * start_row and end_row as inputs as we are writing data for certain players
 * given the row they are in and if they are in the range. This function does
 * some checks to make sure that there are no errors and returns OK if the
 * writing process was done correctly.
 */

int write_tables(char *out_file, int start_row, int end_row) {
  if (g_player_count == 0) {
    return NO_DATA_POINTS;
  }
  if ((start_row < 0) || (end_row >= g_player_count) || (end_row < start_row)) {
    return BAD_ROW;
  }
  FILE *fp = fopen(out_file, "w");
  if (!fp) {
    return FILE_WRITE_ERR;
  }
  //some variables needed
  int att_2 = 0;
  int made_2 = 0;
  int att_3 = 0;
  int made_3 = 0;
  int att_ft = 0;
  int made_ft = 0;
  int rebounds = 0;
  int assists = 0;
  int steals = 0;
  int blocks = 0;
  int turnovers = 0;
  int fouls = 0;
  for (int i = start_row; i <= end_row; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      switch (j) {
        case 0:
          att_2 = g_shooting[i][j];
          rebounds = g_statistics[i][j];
          break;
        case 1:
          made_2 = g_shooting[i][j];
          assists = g_statistics[i][j];
          break;
        case 2:
          att_3 = g_shooting[i][j];
          steals = g_statistics[i][j];
          break;
        case 3:
          made_3 = g_shooting[i][j];
          blocks = g_statistics[i][j];
          break;
        case 4:
          att_ft = g_shooting[i][j];
          turnovers = g_statistics[i][j];
          break;
        case 5:
          made_ft = g_shooting[i][j];
          fouls = g_statistics[i][j];
          break;
      }
    }
    //writing to file
    fprintf(fp, "%s,%s|%d%s,%d%s|%d,%d,%d,%d,%d,%d|%d,%d,%d,%d,%d,%d\n",
      g_player_name[i][0], g_player_name[i][1], g_attributes[i][0], "in",
      g_attributes[i][1], "lbs", att_2, made_2, att_3, made_3, att_ft, made_ft,
      rebounds, assists, steals, blocks, turnovers, fouls);
  }
  fclose(fp);
  fp = NULL;
  return OK;

} /* write_tables() */

/* Remember, you don't need a main function!
 * It is provided by hw3_main.c or hw3_test.o
 */