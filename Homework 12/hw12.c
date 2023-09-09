#include "hw12.h"

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* HELPER FUNCTIONS */

/*
 * Helper function to count the number of matching nodes 
 */

void count_matches(node_t *node, void *data, int *count) {
  if (node == NULL) {
    return;
  }

  int cmp = node->compare(node->data, data);
  if (cmp == 0) {
    (*count)++;
  }

  count_matches(node->left_child, data, count);
  count_matches(node->right_child, data, count);
} /* count_matches() */

/*
 * Helper function to fill the array with matching nodes 
 */

void fill_array(node_t *node, void *data, node_t **array_of_dups, int *index) {
  if (node == NULL) {
    return;
  }

  int cmp = node->compare(node->data, data);
  if (cmp == 0) {
    array_of_dups[*index] = node;
    (*index)++;
  }

  fill_array(node->left_child, data, array_of_dups, index);
  fill_array(node->right_child, data, array_of_dups, index);
} /* fill_array() */

/* IMPLEMENTED FUNCTIONS */

/*
 * Function to print the internal data of a app_t struct passed as a void
 * pointer to standard out
 */

void print_app_data(void *p) {
  app_t *ptr = (app_t *) p;
  assert(ptr && ptr->name);

  printf("%s: %dMB, $%.2f, %.2f/5\n", ptr->name, ptr->size,
         ptr->price, ptr->rating);
} /* print_app_data() */

/*
 * Function to print the internal data of a phone_t struct passed as a void
 * pointer to standard out
 */

void print_phone_data(void *p) {
  phone_t *ptr = (phone_t *) p;
  assert(ptr && ptr->model);

  printf("%s: %dGB, released %d, $%.2f, %.2f hours\n", ptr->model,
         ptr->storage, ptr->release_year, ptr->price, ptr->battery_life);
} /* print_phone_data() */

/*
 * Function to print a tree node by dynamically dispatching the print function
 * stored in the node on the data stored in the node
 */

void print_node(node_t *n_ptr) {
  assert(n_ptr && n_ptr->data && n_ptr->print);

  (n_ptr->print)(n_ptr->data);
} /* print_node() */

/*
 * Function to recursively print the tree using an in-order traversal
 */

void print_tree(node_t *root) {
  if (!root) {
    return;
  }

  print_tree(root->left_child);
  print_node(root);
  print_tree(root->right_child);

} /* print_tree() */

/* define the rest of the functions below (you can delete this comment) */

/*
 * Function to delete the entire tree in postfix traversal order
 */

void delete_tree(node_t **root) {
  assert(root);

  if (*root == NULL) {
    return;
  }
  delete_tree(&((*root)->left_child));
  delete_tree(&((*root)->right_child));
  (*root)->delete(&((*root)->data));
  free(*root);
  *root = NULL;
} /* delete_tree() */

/*
 * Functions of the Tree-nodes
 */

/*
 * Function to create a node_t node, given the parameters that will make up
 * the node, and allocating memory for the node.
 */

void create_node(node_t **node, void *data, void (*print)(void*),
                 void (*delete)(void **), int (*compare)(void *, void *)) {
  assert(node && data && print && delete && compare);
  assert(*node == NULL);
  *node = malloc(sizeof(node_t));
  assert(*node);
  (*node)->data = data;
  (*node)->print = print;
  (*node)->delete = delete;
  (*node)->compare = compare;
} /* create_node() */

/*
 * Deleting a singular node that is isolated by not having a left or right
 * child. We then deallocate the node and all the data within it.
 */

void delete_node(node_t **node) {
  assert(node && *node);
  assert(!((*node)->left_child) && !((*node)->right_child));

  (*node)->delete(&((*node)->data));
  free(*node);
} /* delete_node() */

/*
 * Function to insert a node, given as a paramater, into a tree that is also
 * given as a parameter. We insert the node using prefix traversal.
 */

void insert_node(node_t **root, node_t *node) {
  assert(root && node);
  if (*root == NULL) {
    *root = node;
    return;
  }

  if ((*root)->compare(node->data, (*root)->data) <= 0) {
    insert_node(&((*root)->left_child), node);
  }
  else {
    insert_node(&((*root)->right_child), node);
  }
} /* insert_node() */

/*
 * Function that removes a node given as a parameter from the tree if it exists
 * in the tree. We then also need to restructure the tree to make sure it keeps
 * its structure.
 */

void remove_node(node_t **root, node_t *node) {
  assert(root && node);

  if (*root == NULL) {
    return;
  }

  node_t *node_deleted = *root;
  if ((node_deleted) == node) {
    node_t *left_child = node_deleted->left_child;
    node_t *right_child = node_deleted->right_child;

    if (left_child != NULL) {
      node_t *rightmost_node = left_child;
      while (rightmost_node->right_child != NULL) {
        rightmost_node = rightmost_node->right_child;
      }
      rightmost_node->right_child = right_child;
      *root = left_child;
    }
    else {
      *root = right_child;
    }
    node_deleted->left_child = NULL;
    node_deleted->right_child = NULL;
    delete_node(&(node_deleted));
    return;
  }
  else if ((node_deleted)->compare(node->data, (node_deleted)->data) <= 0) {
    remove_node(&((*root)->left_child), node);
  }
  else {
    remove_node(&((*root)->right_child), node);
  }
} /* remove_node() */

/*
 * Function to find nodes in the tree whose data matches the data given as an
 * input and put them into an array that is returned.
 */

node_t **find_nodes(node_t *root, void *data, int *value) {
  assert(root && data && value);

  int count = 0;
  count_matches(root, data, &count);
  if (count == 0) {
    return NULL;
  }

  node_t **array_of_dups = malloc(count * sizeof(struct node *));
  assert(array_of_dups);

  int index = 0;
  fill_array(root, data, array_of_dups, &index);
  *value = count;

  return array_of_dups;
} /* find_nodes() */

/*
 * Functions for accessing/manipulating node-data
 */

/* data access/manipulation functions for app structure */

/*
 * Functiont to allocate and initialize an app_t node with the information
 * given as inputs.
 */

void create_app_data(void **node, const char *name, int size, float price, float rating) {
  assert(node != NULL);
  assert(name != NULL);
  assert(*node == NULL);

  app_t *new_node = (app_t *) malloc(sizeof(app_t));
  assert(new_node);
  new_node->name = (char *) malloc(strlen(name) + 1);
  assert(new_node->name);
  strcpy(new_node->name, name);

  new_node->size = size;
  new_node->price = price;
  new_node->rating = rating;

  *node = (void *) new_node;
} /* create_app_data() */

/*
 * Function to deallocate and free an app_t node given
 */

void delete_app_data(void **app) {
  assert(app && *app);
  app_t *node = (app_t *) (*app);
  free(node->name);
  free(node);
  *app = NULL;

} /* delete_app_data() */

/*
 * Function to compare two app_t nodes and return a corresponding number, 1, 0,
 * or -1 depending on if arg 1 comes after, before, or is equal to arg2.
 */

int  compare_app_data(void *node1, void *node2) {
  assert(node1 && node2);
  app_t *app1 = (app_t *) node1;
  app_t *app2 = (app_t *) node2;

  float rating1 = (app1->rating) / ((app1->size) * (app1->price + 1));
  float rating2 = (app2->rating) / ((app2->size) * (app2->price + 1));

  if (rating1 > rating2) {
    return 1;
  }
  else if (rating1 == rating2) {
    return 0;
  }
  else {
    return -1;
  }
} /* compare_app_data() */

/* data access/manipulation functions for phone structure */

/*
 * Function to allocate and initialize a phone_t node given the
 * information given as the parameters.
 */

void create_phone_data(void **node, const char *model, int storage,
                       int release_year, float price, float battery_life) {
  assert(node && model);
  assert(!(*node));

  phone_t *phone = malloc(sizeof(phone_t));
  assert(phone);
  phone->model = malloc(strlen(model) + 1);
  assert(phone->model);
  strcpy(phone->model, model);

  phone->storage = storage;
  phone->release_year = release_year;
  phone->price = price;
  phone->battery_life = battery_life;

  *node = (void *) phone;
} /* create_phone_data() */

/*
 * Function to deallocate and free the phone_t node
 */

void delete_phone_data(void **node) {
  assert(node && *node);
  phone_t *phone = (phone_t *) (*node);
  free(phone->model);
  free(phone);
  *node = NULL;
} /* delete_phone_data() */

/*
 * Function to compare two phone arguments and return either -1, 0, or 1
 * depending on if arg1 is before, equal, or after arg 2.
 */

int  compare_phone_data(void *node1, void *node2) {
  assert(node1 && node2);
  phone_t *phone1 = (phone_t *) node1;
  phone_t *phone2 = (phone_t *) node2;

  float rating1 = (phone1->storage * phone1->battery_life) /
                  (phone1->price * (CURRENT_YEAR - phone1->release_year));
  float rating2 = (phone2->storage * phone2->battery_life) /
                  (phone2->price * (CURRENT_YEAR - phone2->release_year));

  if (rating1 > rating2) {
    return 1;
  }
  else if (rating1 == rating2) {
    return 0;
  }
  else {
    return -1;
  }
} /* compare_phone_data() */