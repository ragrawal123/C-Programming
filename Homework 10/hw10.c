#include "hw10.h"

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* Helper Functions */

/*
 * This function is the helper function for the prefix traversal function
 * we are doing in our traverse_employees implemented function. This reads the
 * node in N-L-R order and is recursive in order to find the node having to do
 * with the kth number.
 */

employee_t *traverse_prefix(employee_t *node, int *k) {
  if (node == NULL) {
    return NULL;
  }

  if (*k == 0) {
    return node;
  }

  (*k)--;
  employee_t *left_child = traverse_prefix(node->left_child_ptr, k);

  if (left_child != NULL) {
    return left_child;
  }

  return traverse_prefix(node->right_child_ptr, k);
} /* traverse_prefix() */

/*
 * This function is the helper function for the postfix traversal function
 * we are doing in our traverse_employees implemented function. This reads the
 * node in L-R-N order and is recursive in order to find the node having to do
 * with the kth number.
 */

employee_t *traverse_postfix(employee_t *node, int *k) {
  if (node == NULL) {
    return NULL;
  }

  employee_t *left_child = traverse_postfix(node->left_child_ptr, k);
  if (left_child != NULL) {
    return left_child;
  }

  employee_t *right_child = traverse_postfix(node->right_child_ptr, k);
  if (right_child != NULL) {
    return right_child;
  }

  if (*k == 0) {
    return node;
  }

  (*k)--;
  return NULL;

} /* traverse_postfix() */

/*
 * This function is the helper function for the inorder traversal function
 * we are doing in our traverse_employees implemented function. This reads the
 * node in L-N-R order and is recursive in order to find the node having to do
 * with the kth number.
 */

employee_t *traverse_inorder(employee_t *node, int *k) {
  if (node == NULL) {
    return NULL;
  }

  employee_t *left_child = traverse_inorder(node->left_child_ptr, k);

  if (left_child != NULL) {
    return left_child;
  }

  if (*k == 0) {
    return node;
  }

  (*k)--;
  return traverse_inorder(node->right_child_ptr, k);
} /* traverse_inorder() */

/*
 * This function is the helper function for the reverse order traversal
 * function we are doing in our traverse_employees implemented function.
 * This reads the node in R-N-L order and is recursive in order to find
 * the node having to do with the kth number.
 */

employee_t *traverse_reverse(employee_t *node, int *k) {
  if (node == NULL) {
    return NULL;
  }

  employee_t *right_child = traverse_reverse(node->right_child_ptr, k);

  if (right_child != NULL) {
    return right_child;
  }

  if (*k == 0) {
    return node;
  }

  (*k)--;
  return traverse_reverse(node->left_child_ptr, k);
} /* traverse_reverse() */

/* Implemented Functions */

/*
 * This function creates an employee node for our tree given the input of a
 * name. This is done by allocating memory for the node and setting the
 * respective properties of the node correctly. We then return that employee
 * node.
 */

employee_t *create_employee(char *name) {
  assert(name != NULL);

  employee_t *employee = malloc(sizeof(employee_t));
  assert(employee != NULL);

  employee->name = malloc(strlen(name) + 1);
  assert(employee->name != NULL);
  strcpy(employee->name, name);
  employee->left_child_ptr = NULL;
  employee->right_child_ptr = NULL;

  return employee;
} /* create_employee() */

/*
 * This function inserts an employee node, given as an input, into the tree
 * that is given by the root node and is a binary search tree. In order to
 * insert it correctly, we are making sure the node is put in correctly
 * based on if the name should come before or after different node names.
 * This function also uses recursion to make it easier to read and understand.
 */

void insert_employee(employee_t **root, employee_t *employee) {
  assert(root != NULL);
  assert(employee != NULL);
  assert(employee->left_child_ptr == NULL);
  assert(employee->right_child_ptr == NULL);
  if (*root == NULL) {
    *root = employee;
    return;
  }

  if (strcmp(employee->name, (*root)->name) <= 0) {
    insert_employee(&((*root)->left_child_ptr), employee);
  }
  else {
    insert_employee(&((*root)->right_child_ptr), employee);
  }
} /* insert_employee() */

/*
 * This function takes in a root of a binary search tree and the name of the
 * node we are trying to find as the input. It goes through the tree and in
 * prefix order, checks if the name of the current node and our input is the
 * same. If it's not, we check if it's left and recursively call the function
 * action for the left child of the current node. Otherwise we call right
 * instead. We do this until we find the employee node.
 */

employee_t *find_employee(employee_t *root, char *name) {
  assert(name != NULL);

  if (root == NULL) {
    return NULL;
  }

  if (strcmp(name, root->name) == 0) {
    return root;
  }
  else if (strcmp(name, root->name) < 0) {
    return find_employee(root->left_child_ptr, name);
  }
  else {
    return find_employee(root->right_child_ptr, name);
  }
} /* find_employee() */

/*
 * This function uses post-fix traversal to go through the tree with the root
 * node, which is given as the input and recursively call the function again
 * until we reach the end nodes and then deallocate them. We do that until
 * the entire tree is deleted. We do not return anything in this function.
 */

void delete_tree(employee_t **root) {
  assert(root != NULL);
  if (*root == NULL) {
    return;
  }

  delete_tree(&((*root)->left_child_ptr));
  delete_tree(&((*root)->right_child_ptr));
  free((*root)->name);
  free(*root);
  *root = NULL;
} /* delete_tree() */

/*
 * This is the implemented function for traverse employee that takes in the
 * input of the root of the binary search tree we are traversing, the order
 * we want to do, and the kth employee we are trying to find. We do this
 * by actually using helper functions for each traversal order depending
 * on what they are. All the work will be done in those helper functions.
 */

employee_t *traverse_employees(employee_t *root, int order, int k_employee) {
  assert(root != NULL);
  assert((order >= 1) && (order <= 4));
  assert(k_employee >= 0);

  if (order == PREFIX) {
    return traverse_prefix(root, &k_employee);
  }
  else if (order == POSTFIX) {
    return traverse_postfix(root, &k_employee);
  }
  else if (order == INORDER) {
    return traverse_inorder(root, &k_employee);
  }
  else if (order == REVERSE) {
    return traverse_reverse(root, &k_employee);
  }

  return NULL;
} /* traverse_employees() */

/*
 * This function returns the employee in the binary search tree, which is given
 * as the first input, that comes just before the name given as the second arg.
 * This is done by implementing a while loop and first finding the proper
 * employee node and setting a previous employee node to the old employee
 * node if needed. Then, we go through another while loop and find the proper
 * employee.
 */

employee_t *previous_employee(employee_t *employee, char *name) {
  assert(name != NULL);
  assert(employee != NULL);

  employee_t *prev = NULL;
  while ((employee != NULL) && (strcmp(name, employee->name) != 0)) {
    if (strcmp(name, employee->name) < 0) {
      employee = employee->left_child_ptr;
    }
    else {
      prev = employee;
      employee = employee->right_child_ptr;
    }
  }

  if (employee == NULL) {
    return NULL;
  }

  if (employee->left_child_ptr != NULL) {
    prev = employee->left_child_ptr;
    while (prev->right_child_ptr != NULL) {
      prev = prev->right_child_ptr;
    }
  }

  return prev;
} /* previous_employee() */