/* Name, hw9.c, CS24000, Spring 2023
 * Last updated Feb 11, 2023
 */

/* Add any includes here */

#include "hw9.h"

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* Define your global variables here */

/* Define your functions here */

/*
 * This function creates the collection given the inputs of the list we are
 * inputting the collection into and the name of the collection. From there,
 * we do some asserts to make sure our inputs are valid, and then use malloc
 * and string functions or just general assigning to create our collection.
 */

void create_collection(char *collection_name, collection_t **new_collection) {
  assert(collection_name != NULL);
  assert(new_collection != NULL);
  assert(*new_collection == NULL);

  *new_collection = malloc(sizeof(collection_t));
  assert((*new_collection) != NULL);
  (*new_collection)->collection_name = malloc(strlen(collection_name) + 1);
  assert((*new_collection)->collection_name != NULL);
  strcpy((*new_collection)->collection_name, collection_name);
  (*new_collection)->book_count = 0;
  (*new_collection)->books = NULL;
  (*new_collection)->prev_collection = NULL;
  (*new_collection)->next_collection = NULL;

} /* create_collection() */

/*
 * This function adds a book to the collection given as an input, and we are
 * given the title, author, genre, and condition of the book to create and add
 * to our collection. We do some assert checks, and then create memory for the
 * book and add the characteristics to it, and then put the book at the head of
 * the collection list of books.
 */

void add_book(collection_t *collection, char *title, char *author, 
              enum genre_t genre, enum condition_t condition) {
  assert(collection != NULL);
  assert(title != NULL);
  assert(author != NULL);

  int ok_genre = 0;
  switch (genre) {
    case NOVEL:
      ok_genre = 1;
      break;
    case NARRATIVE:
      ok_genre = 1;
      break;
    case SCI_FI:
      ok_genre = 1;
      break;
    case NON_FICTION:
      ok_genre = 1;
      break;
    case ADVENTURE:
      ok_genre = 1;
      break;
  }
  assert(ok_genre == 1);

  int ok_condition = 0;
  switch (condition) {
    case NEW:
      ok_condition = 1;
      break;
    case GOOD:
      ok_condition = 1;
      break;
    case FAIR:
      ok_condition = 1;
      break;
    case POOR:
      ok_condition = 1;
      break;
  }
  assert(ok_condition == 1);

  book_t *book_ptr = malloc(sizeof(book_t));
  assert(book_ptr != NULL);
  book_ptr->title = malloc(strlen(title) + 1);
  assert(book_ptr->title != NULL);
  strcpy(book_ptr->title, title);
  book_ptr->author = malloc(strlen(author) + 1);
  assert(book_ptr->author != NULL);
  strcpy(book_ptr->author, author);
  book_ptr->genre = genre;
  book_ptr->condition = condition;
  book_ptr->check_out_count = 0;

  book_ptr->prev_book = NULL;
  book_ptr->next_book = collection->books;
  if (collection->books != NULL) {
    collection->books->prev_book = book_ptr;
  }
  collection->books = book_ptr;
  collection->book_count++;
} /* add_book() */

/*
 * This function adds the collection given as an input to the list, which is
 * also given as input. We need to do this and make sure that our collection
 * is added in alphabetical order. We do this by going through the list and
 * using strcmp() to find the correct place to insert the collection, and
 * when we are done we make sure our list goes back to the head of the
 * collections list.
 */

void add_collection(collection_t **list, collection_t *collection) {
  assert(list != NULL);
  assert(collection != NULL);

  collection_t *head = *list;
  if (*list == NULL) {
    *list = collection;
    return;
  }
  while (head != NULL) {
    if (strcmp(collection->collection_name, head->collection_name) <= 0) {
      if (head->prev_collection != NULL) {
        head->prev_collection->next_collection = collection;
        collection->prev_collection = head->prev_collection;
        collection->next_collection = head;
        head->prev_collection = collection;
        break;
      }
      else {
        collection->prev_collection = NULL;
        collection->next_collection = head;
        head->prev_collection = collection;
        head = collection;
        *list = head;
        break;
      }
    }
    else {
      if (head->next_collection == NULL) {
        collection->next_collection = NULL;
        collection->prev_collection = head;
        head->next_collection = collection;

        break;
      }
      head = head->next_collection;
    }
  }
  while ((*list)->prev_collection != NULL) {
    (*list) = (*list)->prev_collection;
  }
} /* add_collection() */

/*
 * This function deletes the collection specificed in the input and makes sure
 * that we change the orientation of the list so that we don't have a
 * disconnected list. Once we remove it from the list, we then deallocate the
 * contents of the book list inside the collection and the collection itself.
 */

void delete_collection(collection_t **collection) {
  assert(collection != NULL);
  assert(*collection != NULL);

  collection_t *del_collec = *collection;

  if ((del_collec->prev_collection == NULL) && (del_collec->next_collection == NULL)) {
    free(del_collec->collection_name);

    book_t *book = del_collec->books;
    while (book != NULL) {
      book_t *next_book = book->next_book;
      free(book->title);
      free(book->author);
      free(book);
      book = next_book;
    }

    free(del_collec);
    *collection = NULL;
    return;
  }
  else if ((del_collec->prev_collection == NULL) && (del_collec->next_collection != NULL)) {
    *collection = del_collec->next_collection;
    del_collec->next_collection->prev_collection = NULL;
    del_collec->next_collection = NULL;
  }
  else if ((del_collec->prev_collection != NULL) && (del_collec->next_collection == NULL)) {
    *collection = del_collec->prev_collection;
    del_collec->prev_collection->next_collection = NULL;
    del_collec->prev_collection = NULL;
  }
  else {
    *collection = del_collec->prev_collection;
    del_collec->prev_collection->next_collection = del_collec->next_collection;
    del_collec->next_collection->prev_collection = del_collec->prev_collection;
  }

  free(del_collec->collection_name);

  book_t *book = del_collec->books;
  while (book != NULL) {
    book_t *next_book = book->next_book;
    free(book->title);
    free(book->author);
    free(book);
    book = next_book;
  }

  free(del_collec);
  while ((*collection)->prev_collection != NULL) {
    *collection = (*collection)->prev_collection;
  }
} /* delete_collection() */

/*
 * This function moves a specified book, given by the name of the book, from
 * the collection to a new collection specificied by a name. We do this by
 * first finding the new collection, and the book. We remove the book from
 * the old list, and then insert it into the new one. We return
 * NO_SUCH_COLLECTION if we can't find the new collection and NO_SUCH_BOOK,
 * if the book doesn't exist.
 */

int move_book(collection_t *collection, char *book_name, char *new_collection) {
  assert(collection != NULL);
  assert(book_name != NULL);
  assert(new_collection != NULL);

  while (collection->prev_collection != NULL) {
    collection = collection->prev_collection;
  }

  collection_t *found_collection = NULL;
  collection_t *curr_collection = collection;
  while (curr_collection != NULL) {
    if (strcmp(curr_collection->collection_name, new_collection) == 0) {
      found_collection = curr_collection;
      break;
    }
    curr_collection = curr_collection->next_collection;
  }

  if (found_collection == NULL) {
    return NO_SUCH_COLLECTION;
  }

  curr_collection = collection;
  collection_t *book_collection = NULL;
  book_t *book = NULL;
  while (curr_collection != NULL) {
    book_t *curr_book = curr_collection->books;
    while (curr_book != NULL) {
      if (strcmp(curr_book->title, book_name) == 0) {
        book = curr_book;
        book_collection = curr_collection;
        break;
      }
      curr_book = curr_book->next_book;
    }
    if (book != NULL) {
      break;
    }
    curr_collection = curr_collection->next_collection;
  }

  if (book == NULL) {
    return NO_SUCH_BOOK;
  }

  if ((book->prev_book == NULL) && (book->next_book == NULL)) {
    book_collection->books = NULL;
    book_collection->book_count--;
    book->prev_book = NULL;
    book->next_book = found_collection->books;
    if (found_collection->books != NULL) {
      found_collection->books->prev_book = book;
    }
    found_collection->books = book;
    found_collection->book_count++;
    return OK;
  }
  else if ((book->prev_book == NULL) && (book->next_book != NULL)) {
    book_collection->books = book->next_book;
    book->next_book->prev_book = NULL;
    book->next_book = NULL;
  }
  else if ((book->prev_book != NULL) && (book->next_book == NULL)) {
    book->prev_book->next_book = NULL;
    book->prev_book = NULL;
  }
  else {
    book->prev_book->next_book = book->next_book;
    book->next_book->prev_book = book->prev_book;
  }
  book_collection->book_count--;

  book->prev_book = NULL;
  book->next_book = found_collection->books;
  if (found_collection->books != NULL) {
    found_collection->books->prev_book = book;
  }
  found_collection->books = book;
  found_collection->book_count++;

  return OK;
} /* move_book() */

/*
 * This function alphabetizes the book list, given as the input, in increasing
 * order, from for example 'A' to 'Z'. We do this by having two while loops
 * and sort of doing a bubble sort, by going in order and for each book
 * swapping it with other books if it should come before. And we keep on doing
 * that until our book list is sorted alphabetically.
 */

void alphabetize_books(book_t **head_book) {
  assert(head_book != NULL);


  book_t *curr_book = *head_book;
  if (curr_book == NULL) {
    return;
  }
  book_t *prev_book = NULL;
  book_t *temp_book = NULL;
  while (curr_book->prev_book != NULL) {
    curr_book = curr_book->prev_book;
  }

  while (curr_book != NULL) {
    temp_book = curr_book;
    prev_book = curr_book->prev_book;

    while ((prev_book != NULL) && (strcmp(temp_book->title, prev_book->title) < 0)) {
      prev_book->next_book = temp_book->next_book;
      temp_book->prev_book = prev_book->prev_book;
      if (temp_book->next_book != NULL) {
        temp_book->next_book->prev_book = prev_book;
      }
      temp_book->next_book = prev_book;
      prev_book->prev_book = temp_book;

      if (temp_book->prev_book != NULL) {
        temp_book->prev_book->next_book = temp_book;
      }
      prev_book = temp_book->prev_book;
      if (prev_book == NULL) {
        break;
      }
      temp_book = prev_book->next_book;
    }

    curr_book = curr_book->next_book;
  }
  while ((*head_book)->prev_book != NULL) {
    *head_book = (*head_book)->prev_book;
  }
} /* alphabetize_books() */

/*
 * This function finds the total checkouts a specific collection has, which is
 * given as an input, restricted to a specific condition each book must be in.
 * It does this by iterating through the books list, checking the condition,
 * and then adding the total checkouts for the books that met the requirements.
 */

int count_total_checkouts(collection_t *collection, enum condition_t condition) {
  assert(collection != NULL);

  int ok_condition = 0;
  switch (condition) {
    case NEW:
      ok_condition = 1;
      break;
    case GOOD:
      ok_condition = 1;
      break;
    case FAIR:
      ok_condition = 1;
      break;
    case POOR:
      ok_condition = 1;
      break;
  }
  assert(ok_condition == 1);

  int num_books = 0;
  int total_checkouts = 0;

  book_t *book = collection->books;
  while (book != NULL) {
    if (book->condition == condition) {
      num_books++;
      total_checkouts += book->check_out_count;
    }
    book = book->next_book;
  }

  if (num_books == 0) {
    return 0;
  }
  return total_checkouts;
} /* count_total_checkouts() */

/*
 * This function finds the most common genre a specific collection, which is
 * given as the input, has. This is done by finding the total number of books
 * each genre has based on iterating through each book in the collection, and
 * then finding the max. There is a specific order for checking the max because
 * should any two book counts be equal, we want to return the one with a lower
 * enum.
 */

enum genre_t most_common_genre(collection_t *collection) {
  assert(collection != NULL);
  int max = 0;
  enum genre_t genre = 0;
  int novel_count = 0;
  int narrative_count = 0;
  int sci_fi_count = 0;
  int non_fiction_count = 0;
  int adventure_count = 0;
  int book_count = 0;

  book_t *book = collection->books;
  while (book != NULL) {
    switch (book->genre) {
      case NOVEL:
        book_count++;
        novel_count++;
        break;
      case NARRATIVE:
        book_count++;
        narrative_count++;
        break;
      case SCI_FI:
        book_count++;
        sci_fi_count++;
        break;
      case NON_FICTION:
        book_count++;
        non_fiction_count++;
        break;
      case ADVENTURE:
        book_count++;
        adventure_count++;
        break;
    }
    book = book->next_book;
  }
  if (novel_count > max) {
    max = novel_count;
    genre = NOVEL;
  }
  if (narrative_count > max) {
    max = narrative_count;
    genre = NARRATIVE;
  }
  if (sci_fi_count > max) {
    max = sci_fi_count;
    genre = SCI_FI;
  }
  if (non_fiction_count > max) {
    max = non_fiction_count;
    genre = NON_FICTION;
  }
  if (adventure_count > max) {
    max = adventure_count;
    genre = ADVENTURE;
  }

  if (book_count == 0) {
    return NO_SUCH_BOOK;
  }

  return genre;

} /* most_common_genre() */

/*
 * This function get rids of the book in a specific collection, given as the
 * input, that has the least amount of checkouts. We do this by first finding
 * which book has the least amount of checkouts. From there, we remove it from
 * the list depending on what position it is in and then deallocate the book.
 */

int decommission_book(collection_t *collection) {
  assert(collection != NULL);
  book_t *book = collection->books;
  book_t *least_book = NULL;
  int least_checkout = 0;
  if (book == NULL) {
    return NO_SUCH_BOOK;
  }
  else {
    least_checkout = book->check_out_count;
    least_book = book;
    book = book->next_book;
  }
  while (book != NULL) {
    if (book->check_out_count < least_checkout) {
      least_checkout = book->check_out_count;
      least_book = book;
    }
    book = book->next_book;
  }

  if ((least_book->next_book == NULL) && (least_book->prev_book == NULL)) {
    collection->book_count--;
    free(least_book->title);
    free(least_book->author);
    free(least_book);
    collection->books = NULL;
    return least_checkout;
  }
  else if ((least_book->prev_book == NULL) && (least_book->next_book != NULL)) {
    least_book->next_book->prev_book = NULL;
    collection->books = least_book->next_book;
    least_book->next_book = NULL;
  }
  else if ((least_book->prev_book != NULL) && (least_book->next_book == NULL)) {
    least_book->prev_book->next_book = NULL;
    least_book->prev_book = NULL;
  }
  else {
    least_book->prev_book->next_book = least_book->next_book;
    least_book->next_book->prev_book = least_book->prev_book;
  }
  collection->book_count--;
  free(least_book->title);
  free(least_book->author);
  free(least_book);

  return least_checkout;
} /* decommission_book() */

/*
 * This function deletes all the collections from the specificied list. We do
 * this by just calling the delete_collection function we wrote before until
 * the list becomes null. The delete_collection function does all the work for
 * us.
 */

void delete_collections(collection_t **list) {
  assert(list != NULL);
  assert(*list != NULL);

  while (*list != NULL) {
    delete_collection(list);
  }
  *list = NULL;
} /* delete_collections() */

/* Remember, you don't need a main function!
 * It is provided by hw9_main.c or hw9_test.o
 */