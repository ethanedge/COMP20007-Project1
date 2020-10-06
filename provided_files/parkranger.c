/* * * * * * *
 * Park Ranger module for Assignment 1
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Ethan Edge
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "parkranger.h"
#include "util.h"
#include "deque.h"

#define NUM_ROOTS     1
#define ROOT_NODE     0
#define UNEXPLORED    false

// This function must read in a ski slope map and determine whether or not
// it is possible for the park ranger to trim all of the trees on the ski slope
// in a single run starting from the top of the mountain.
//
// The ski slope map is provided via stdin in the following format:
//
//   n m
//   from to
//   from to
//   ...
//   from to
//
// Here n denotes the number of trees that need trimming, which are labelled
// {1, ..., n}. The integer m denotes the number "reachable pairs" of trees.
// There are exactly m lines which follow, each containing a (from, to) pair
// which indicates that tree `to` is directly reachable from tree `from`.
// `from` and `to` are integers in the range {0, ..., n}, where {1, ..., n}
// denote the trees and 0 denotes the top of the mountain.
//
// For example the following input represents a ski slope with 3 trees and
// 4 reachable pairs of trees.
//
// input:            map:          0
//   3 4                          / \
//   0 1                         /  2
//   0 2                        / /
//   2 1                        1
//   1 3                          \
//                                 3
//
// In this example your program should return `true` as there is a way to trim
// all trees in a single run. This run is (0, 2, 1, 3).
//
// Your function should must:
//  - Read in this data from stdin
//  - Store this data in an appropriate data structure
//  - Run the algorithm you have designed to solve this problem
//  - Do any clean up required (e.g., free allocated memory)
//  - Return `true` or `false` (included in the stdbool.h library)
//
// For full marks your algorithm must run in O(n + m) time.
bool is_single_run_possible() {

  /* Creates an adjacency list and populates it */
  int n;
  Deque **adj_list = read_in_input(&n);

  /* Perform a depth-first search to get a topological ordering */
  Deque *topological_sort = new_deque();
  if(topological_DFS(adj_list, topological_sort, n) == false) {
    /* Graph isn't connected, single run isn't possible. */
    return false;
  }

  /* Check if topological sort is a valid run */
  return is_topological_a_run(topological_sort, adj_list, n);
}

Deque** read_in_input(int *n) {
  int u, v;               // From vertex u to v
  int first = true;       // Is it the first line?
  int temp1, temp2;       // Temporarily holds the read values
  int i;
  Deque **adj_list;

  /* Reads in all the vertices and edges into adj_list */
  while((scanf("%d %d", &temp1, &temp2)) != EOF) {

    /* Read in number of vertices and edges and allocate adj_list */
    if (first) {

      first = false;

      *n = temp1 + NUM_ROOTS;

      adj_list = malloc((*n)*sizeof(**adj_list));
      assert(adj_list != NULL);

      for(i = 0; i < *n; i++) {
        adj_list[i] = new_deque();
        assert(adj_list[i] != NULL);
      }

    }
    else {
      u = temp1;
      v = temp2;
      deque_push(adj_list[u], v);
    }
  }

  return adj_list;
}

bool topological_DFS(Deque **adj_list, Deque *topological_sort, int n) {

  /* Takes in an adjacency list, performs a depth-first search, keeps track */
  /* of popped values in a topological_sort Deque. Also takes in said */
  /* topological_sort Deque and an integer n which holds amount of vertices. */

  /* Returns false if graph isn't connected which automatically makes a */
  /* single run impossible. Otherwise returns true. */

  /* Holds whether we've explored a node */
  int* explored = malloc(n*sizeof(*explored));
  int i;
  int count = 0;     // How many nodes have we explored?
  for(i = 0; i < n; i++) {
    explored[i] = UNEXPLORED;
  }

  /* Utilise a deque as a stack by only using push() and pop() */
  Deque *DFS_stack = new_deque();

  /* Explore the root node */
  DFS_explore(ROOT_NODE, adj_list, explored, DFS_stack, topological_sort,
              &count);

  free(explored);
  /* Individual nodes were freed in the popping process */
  free(DFS_stack);

  /* Will return false if graph isn't connected */
  if(count != n) {
    return false;
  }
  return true;
}

void DFS_explore(Data vertex, Deque **adj_list, int* explored, Deque *DFS_stack,
                 Deque *topological_sort, int *count) {

  /* We've now seen this node and we need to mark this point so we can
  backtrack */
  deque_push(DFS_stack, vertex);
  explored[vertex] = ++(*count);

  /* Explore all sub-vertices of this vertex */
  Node* temp = adj_list[vertex]->top;
  while(temp != NULL) {
      Data n_vertex = temp->data;
      if (explored[n_vertex] == UNEXPLORED) {
          DFS_explore(n_vertex, adj_list, explored, DFS_stack, topological_sort,
                      count);
      }
      temp = temp->next;
  }

  /* Finished exploring the vertex, put it in the topological sort and forget */
  deque_push(topological_sort, deque_pop(DFS_stack));
}

bool is_topological_a_run(Deque *topological_sort, Deque **adj_list, int n) {

  /* Takes in a topological_sort Deque as well as an adjacency list. */
  /* Goes through the topological_sort's pairs of adjacent vertices */
  /* (u,v) and ensures there exists an edge u->v in the adjacency list. */
  /* Returns false if a single edge doesn't exist, otherwise returns true. */

  /* temp_topo_u/v traverse the topological sort and find u/v respectively */
  Node *temp_topo_u = topological_sort->top;
  Node *temp_topo_v = topological_sort->top->next;

  /* Vertices u and v, and a boolean value of if an edge u->v exists */
  Data u,v;
  bool edge;

  /* Traverse the topological sort list and ensure each edge u->v exists */
  while(temp_topo_v != NULL) {
    u = temp_topo_u->data;
    v = temp_topo_v->data;

    edge = false;

    /* Explores all the edges of a vertex u */
    Node *temp_edges = adj_list[u]->top;
    while(temp_edges != NULL) {

      if(temp_edges->data == v) {
        edge = true;
      }

      temp_edges = temp_edges->next;
    }

    /* No edge between u->v, no single run possible */
    if(edge == false) {
      free_deque(topological_sort);
      return false;
    }

    temp_topo_u = temp_topo_u->next;
    temp_topo_v = temp_topo_v->next;
  }

  free_deque(topological_sort);
  free_adj_list(adj_list, n);
  return true;
}

void free_adj_list(Deque **adj_list, int n) {
  /* Takes in the adjacency list and number of vertices and frees all */
  /* the memory associated. */

  int i;
  for(i = 0; i < n; i++) {
    free_deque(adj_list[i]);
  }

  free(adj_list);
}



























