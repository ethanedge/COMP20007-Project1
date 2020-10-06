/* * * * * * *
 * Deque module (i.e., double ended queue) for Assignment 1
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Ethan Edge
 */

// You must not change any of the code already provided in this file, such as
// type definitions, constants or functions.
//
// You may, however, add additional functions and/or types which you may need
// while implementing your algorithms and data structures.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "deque.h"
#include "util.h"

#define INITIAL 5

// Create a new empty Deque and return a pointer to it
Deque *new_deque() {
	Deque *deque = malloc(sizeof(*deque));
	assert(deque);

	deque->top = NULL;
	deque->bottom = NULL;
	deque->size = 0;

	return deque;
}

// Free the memory associated with a Deque
void free_deque(Deque *deque) {
  // Remove (and thus free) all of the nodes in the Deque.
  while (deque->size > 0) {
    deque_remove(deque);
  }

	// Free the deque struct itself
	free(deque);
}

// Create a new Node with a given piece of data
Node *new_node(Data data) {
  Node *node = malloc(sizeof(*node));
  assert(node);

  node->next = NULL;
  node->prev = NULL;
  node->data = data;

  return node;
}

// Free the memory associated with a Node
void free_node(Node *node) {
  free(node);
}

// Add an element to the top of a Deque
void deque_push(Deque *deque, Data data) {
  Node *new = new_node(data);

  if (deque->size > 0) {
    new->next = deque->top;
    deque->top->prev = new;
  } else {
    // If the Deque was initially empty then new is both the top and bottom
    deque->bottom = new;
  }

  deque->top = new;
  deque->size++;
}

// Add an element to the bottom of a Deque
void deque_insert(Deque *deque, Data data) {
  Node *new = new_node(data);

  if (deque->size > 0) {
    new->prev = deque->bottom;
    deque->bottom->next = new;
  } else {
    // If the Deque was initially empty then new is both the top and bottom
    deque->top = new;
  }

  deque->bottom = new;
  deque->size++;
}

// Remove and return the top element from a Deque
Data deque_pop(Deque *deque) {
  if (deque->size == 0) {
    exit_with_error("can't pop from empty Deque");
  }

  Data data = deque->top->data;
  Node *old_top = deque->top;

  if (deque->size == 1) {
    deque->top = NULL;
    deque->bottom = NULL;
  } else {
    deque->top = old_top->next;
    deque->top->prev = NULL;
  }

  deque->size--;

  free(old_top);

  return data;
}

// Remove and return the bottom element from a Deque
Data deque_remove(Deque *deque) {
  if (deque->size == 0) {
    exit_with_error("can't remove from empty Deque");
  }

  Data data = deque->bottom->data;
  Node *old_bottom = deque->bottom;

  if (deque->size == 1) {
    deque->top = NULL;
    deque->bottom = NULL;
  } else {
    deque->bottom = old_bottom->prev;
    deque->bottom->next = NULL;
  }

  deque->size--;

  free(old_bottom);

  return data;
}

// Return the number of elements in a Deque
int deque_size(Deque *deque) {
  return deque->size;
}

// Print the Deque on its own line with the following format:
//   [x_1, x_2, ..., x_n]
//     ^              ^
//    top           bottom
void print_deque(Deque *deque) {
  Node *current = deque->top;
  int i = 0;

  printf("[");

  while (current) {
    printf("%d", current->data);
    // Print a comma unless we just printed the final element
    if (i < deque->size - 1) {
      printf(", ");
    }
    current = current->next;
    i++;
  }

  printf("]\n");
}

// Reverse the Deque using an iterative approach
void iterative_reverse(Deque *deque) {

  /* Takes in a pointer to a deque, uses an iterative approach in order */
  /* to reverse the deque's content's. */
  /* Alters the original deque using its pointers, returns nothing. */

  if (deque->size == 0) {
    exit_with_error("can't reverse empty Deque");
  }

  Node *temp; // holds next pointer temporarily
  Node *traversal = deque->top;  // our main traversal node

  while (traversal != NULL) {
    /* Changes node's pointers */
    temp = traversal->next;
    traversal->next = traversal->prev;
    traversal->prev = temp;

    /* Move to the next node */
    traversal = temp;
  }

  /* Reverse top/bottom pointers */
  temp = deque->top;
  deque->top = deque->bottom;
  deque->bottom = temp;

}

// Reverse the Deque using a recursive approach
void recursive_reverse(Deque *deque) {

  /* Takes in a pointer to a deque, uses a recursive approach in order */
  /* to reverse the deque's content's. */
  /* Alters the original deque using its pointers, returns nothing. */

  if (deque->size == 0) {
    exit_with_error("can't reverse empty Deque");
  }

    /* Holds our next and traversal node pointers */
    Node *temp = deque->top;
    Node *traversal = deque->top;

    /* Starts the recursive call to reverse the nodes' pointers */
    recursive_reverse_sub(traversal, temp);

    /* Reverse top/bottom pointers */
    temp = deque->top;
    deque->top = deque->bottom;
    deque->bottom = temp;
}

// Reverse the Deque using a recursive approach
void recursive_reverse_sub(Node *traversal, Node *temp) {

    /* The sub function that is called recursively for recursive_reverse. */
    /* Takes in a current traversal node and a temp node in order to */
    /* reverse the 'prev' and 'next' pointers of the node, returns nothing. */

    /* Serves as a base case, stops when we get to the end of the deque, */
    /* otherwise keeps traversing recursively. */
    if (traversal != NULL) {
        /* Changes node's pointers */
        temp = traversal->next;
        traversal->next = traversal->prev;
        traversal->prev = temp;

        /* Calls the function again */
        recursive_reverse_sub(temp, temp);
    }
}

// Split the Deque given a critical value k, such that the Deque contains
// all elements greater than equal to k above (i.e., closer to the top)
// the elements less than k.
//
// Within the two parts of the array (>= k and < k) the elements should
// be in their original order.
//
// This function must run in linear time.

void split_deque(Deque *deque, int k) {

  int curr_great, curr_less;         /* Current items inside array */
  curr_great = curr_less = 0;

  int tot_great, tot_less;           /* Total array size */
  tot_great = tot_less = INITIAL;

  int *less_k, *great_k;             /* Arrays to hold our numbers */

  less_k = malloc(tot_less*sizeof(*less_k));
  assert(less_k != NULL);
  great_k = malloc(tot_great*sizeof(*great_k));
  assert(great_k != NULL);

  Node *temp = deque->top;
  int i;

  if (deque->size == 0) {
    exit_with_error("can't reverse empty Deque");
  }

 /* Sorts our deque numbers into less than or greater than or equal to arrays */
 for(i = 0; i < deque->size; i++) {
    if (temp->data < k) {
      if (curr_less == (tot_less - 1)) {
        tot_less *= 2;
        less_k = realloc(less_k, tot_less*sizeof(*less_k));
      }
      less_k[curr_less++] = temp->data;
    }
    else {
      if (curr_great == (tot_great - 1)) {
        tot_great *= 2;
        great_k = realloc(great_k, tot_great*sizeof(*great_k));
      }
      great_k[curr_great++] = temp->data;
    }

    temp = temp->next;
  }

  /* Goes through both arrays and puts their values into the deque */
  /* now in the required order. */
  temp = deque->top;

 for(i = 0; i < curr_great; i++) {
    temp->data = great_k[i];
    temp = temp->next;
 }

  for(i = 0; i < curr_less; i++) {
    temp->data = less_k[i];
    temp = temp->next;
  }

 free(less_k);
 free(great_k);

}

// TODO: Add any other functions you might need for your Deque module
