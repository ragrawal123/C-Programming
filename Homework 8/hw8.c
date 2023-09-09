/* Name, hw8.c, CS 24000, Spring 2023
 * Last updated Feb 4, 2023
 */

/* Add any includes here */
#include "hw8.h"

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* Define your global variables here */

/* Define your functions here */

/*
 * This function takes inputs of the head_club, the name of the club we are
 * inserting into, the name of the player they are inserting into, and the
 * salary of the new player. We then create the new player, and the input
 * that player correctly into the list of players for the specified club.
 * we then return any errors if there any or OK if the process went well.
 * we also make sure to update the player count.
 */

int insert_player(club_t *head_club, char *club_name, char *player_name, int salary) {
  assert(head_club != NULL);
  assert(club_name != NULL);
  assert(player_name != NULL);
  assert(salary > 0);

  club_t *club = head_club;
  while ((club != NULL) && (strcmp(club->club_name, club_name) != 0)) {
    club = club->next_club;
  }
  if (club == NULL) {
    return NO_SUCH_CLUB;
  }

  player_t *player_ptr = NULL;
  player_ptr = malloc(sizeof(player_t));
  assert(player_ptr != NULL);
  player_ptr->name = malloc(strlen(player_name) + 1);
  strcpy(player_ptr->name, player_name);
  player_ptr->salary = salary;
  player_ptr->next_player = NULL;

  player_t *prev_player = NULL;
  player_t *curr_player = club->players;
  while ((curr_player != NULL) && (curr_player->salary > salary)) {
    prev_player = curr_player;
    curr_player = curr_player->next_player;
  }
  // Insert the new player at the correct position
  if (prev_player == NULL) { // Insert at the head of the list
    player_ptr->next_player = club->players;
    club->players = player_ptr;
  }
  else {
    player_ptr->next_player = prev_player->next_player;
    prev_player->next_player = player_ptr;
  }
  club->player_count++;
  return OK;

} /* insert_player() */

/*
 * This function takes inputs of the head_club, the name of the club we are
 * retiring from, and the name of the player we are retiring.
 * We then retire that player correctly from the list of
 * players for the specified club.
 * We then return any errors if there any or OK if the process went well.
 * we also make sure to update the player count.
 */

int retire_player(club_t *head_club, char *club_name, char *player_name) {
  assert(head_club != NULL);
  assert(club_name != NULL);
  assert(player_name != NULL);

  club_t *club = head_club;
  while ((club != NULL) && (strcmp(club->club_name, club_name) != 0)) {
    club = club->next_club;
  }
  if (club == NULL) {
    return NO_SUCH_CLUB;
  }

  player_t *prev_player = NULL;
  player_t *player = club->players;
  player_t *max_player = NULL;

  while (player != NULL) {
    if (strcmp(player->name, player_name) == 0) {
      max_player = player;
      break;
    }
    prev_player = player;
    player = player->next_player;
  }

  if (max_player == NULL) {
    return NO_SUCH_PLAYER;
  }
  if (max_player == club->players) {
    club->players = club->players->next_player;
  }
  else {
    prev_player->next_player = max_player->next_player;
  }

  club->player_count--;

  free(max_player->name);
  max_player->name = NULL;
  free(max_player);
  max_player = NULL;

  return OK;
} /* retire_player() */

/*
 * This function takes the input of the head_club, the current club the player
 * is in, the new club they want the player to go into, and the name of the
 * player that is transferring. We do our assert checks and also see if the
 * transfer is an invalid tranfser. We then go through and do our transfer
 * process by finding the right player and then calling the insert and retire
 * functions.
 */

int transfer_player(club_t *head_club, char *curr_club, char *new_club, char *player_name) {
  assert(head_club != NULL);
  assert(curr_club != NULL);
  assert(new_club != NULL);
  assert(player_name != NULL);

  club_t *club = head_club;
  if (strcmp(curr_club, new_club) == 0) {
    return INVALID_TRANSFER;
  }

  while ((club != NULL) && (strcmp(club->club_name, curr_club) != 0)) {
    club = club->next_club;
  }
  if (club == NULL) {
    return NO_SUCH_CLUB;
  }

  player_t *player = club->players;
  int player_salary = 0;

  while (player != NULL) {
    if (strcmp(player->name, player_name) == 0) {
      player_salary = player->salary;
      break;
    }
    player = player->next_player;
  }

  if (player == NULL) {
    return NO_SUCH_PLAYER;
  }

  insert_player(head_club, new_club, player_name, player_salary);
  retire_player(head_club, curr_club, player_name);
  return OK;

} /* transfer_player() */

/*
 * This function finds the dead clubs in the list given and deallocates them.
 * It takes the head club as it inputs and traverses through the linked list,
 * deallocating everything we need. We do this for all the clubs with no
 * players and return the head of the new list.
 */

club_t *find_dead_clubs(club_t *head_club) {
  assert(head_club != NULL);
  club_t *club = head_club;
  club_t *prev_club = NULL;
  while (club != NULL) {
    if (club->player_count == 0) {
      if (prev_club == NULL) {
        head_club = head_club->next_club;
      }
      else {
        prev_club->next_club = club->next_club;
      }
      free(club->club_name);
      free(club->coach);

      player_t *player = club->players;
      while (player != NULL) {
        player_t *next_player = player->next_player;
        free(player->name);
        free(player);
        player = next_player;
      }
      club_t *temp = club;
      club = club->next_club;
      free(temp);
    }
    else {
      prev_club = club;
      club = club->next_club;
    }
  }
  return head_club;
} /* find_dead_clubs() */

/*
 * This function finds the most valuable club in the linked list given by
 * the head_club as the input. We do this by finding the most valuable club,
 * determined by a formula given, and we then return the most valuable club.
 */

club_t *most_valuable_club(club_t *head_club) {
  assert(head_club != NULL);
  club_t *club = head_club;
  club_t *mv_club = NULL;
  float most_value = 0;
  float goals_scored = 0;
  float goals_lost = 0;
  while (club != NULL) {
    goals_scored = club->goals_scored;
    goals_lost = club->goals_lost;
    player_t *player = club->players;
    double total_salary = 0;
    while (player != NULL) {
      double salary = player->salary;
      total_salary += salary;
      player = player->next_player;
    }
    float value = (goals_scored / (goals_lost + 1)) * total_salary;
    if (value > most_value) {
      most_value = value;
      mv_club = club;
    }
    club = club->next_club;
  }
  return mv_club;
} /* most_valuable_club() */

/*
 * This function deletes the club given by the input club_name from the list
 * head_club which is another input. We do that by traversing, checking the
 * name, and then deallocating and removing the club.
 */

club_t *delete_club(club_t *head_club, char *club_name) {
  assert(head_club != NULL);
  assert(club_name != NULL);

  club_t *club = head_club;
  club_t *prev_club = NULL;
  while (club != NULL) {
    if (strcmp(club->club_name, club_name) == 0) {
      if (prev_club == NULL) {
        head_club = head_club->next_club;
      }
      else {
        prev_club->next_club = club->next_club;
      }
      free(club->club_name);
      free(club->coach);

      player_t *player = club->players;
      while (player != NULL) {
        player_t *next_player = player->next_player;
        free(player->name);
        free(player);
        player = next_player;
      }
      club_t *temp = club;
      club = club->next_club;
      free(temp);
    }
    else {
      prev_club = club;
      club = club->next_club;
    }
  }
  return head_club;
} /* delete_club() */

/* Remember, you don't need a main function!
 * It is provided by hw8_main.c or hw8_test.o
 */