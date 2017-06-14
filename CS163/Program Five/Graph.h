/*****************************************************************
// File Name:  Graph.h
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       June 9th, 2017
// Class:      CS163
// Program #:  5
//
// Overview: This is the declaration file for the Graph for
// programming assignment #5. Essentially, this graph is used to
// represent trails surrounding Portland, and the relationships
// between them. See function comments in the Graph.cpp file for
// more information.
//
// ***************************************************************/


#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <cstring>
#include <cstdlib>



/* ********************************************
// The data type to be represented in the Graph.
// Function definitions are located in Graph.cpp.
//
// ********************************************/
struct Trailhead
{
  // Allocate Trailhead members.
  Trailhead( char * loc_name = NULL,
             int length = 0 );

  // Copies the passed Trailhead's data.
  Trailhead( const Trailhead & t_head_src);

  // Deallocate Trailhead memory.
  ~Trailhead(void);

  // Display trailhead data.
  bool displayTrailhead( int & fst,
                         int & snd);

  // Setup Trailhead to store a trail intersection.
  bool setupAsIntersection( char * loc_name,
                            char * loc_desc );

         // The title of the location.
  char * location,
         // A brief description of the location.
       * description;

  // The approximate length of the trail.
  int trail_length;

};



/* ********************************************
// The data type of the adjacency list in the
// Graph class.
//
// ********************************************/
struct Vertex
{
  // Name and description of a trailhead.
  struct Trailhead * data;

  // Head node for the headlist.
  // Null if the vertex has no adjacent vertices.
  struct Node * head;
};



/* ********************************************
// The node for the edge lists in the vertices
// in the Graph class.
//
// ********************************************/
struct Node
{
  // The adjacent vertex to the vertex
  // whose edge list contains this node.
  struct Vertex * adj_vtex;

  // Deallocates Node memory.
  ~Node(void);

      // Length from the vertex which contains this node in
      // its edge list to the vertex specified by adj_vtex.
  int edge_len,
      // The length of the edgelist, starting
      // at, and including a given node.
      chain_len;

  // The next node in the edgelist.
  struct Node * next;
};



/* ********************************************
// A Graph representing a set of Portland trails,
// and the relationships between them.
//
// ********************************************/
class Graph
{
  public:

    // Initializes graph attributes.
    // Sets default size to 20.
    Graph(int list_max = 20);
    // Copy constructor for possible future use.
    Graph(const Graph & Graph_src);
    // Deallocates graph memory.
    ~Graph(void);

    // Create and insert a new vertex with
    // the specified trailhead information.
    int newVertex(const Trailhead & new_trailhead);

    // Join two existing vertices by name
    // using an edge list. src -> dest.
    int joinVertices( int from_vtex,
                      int to_vtex,
                      int edge_length );

    // Display all possible paths from a given vertex (with
    // a trailhead titled start_loc_name) to every other vertex.
    int displayPaths(const char * start_loc_name) const;
    // Display all possible paths from a given
    // vertex, by index, to every other vertex.
    int displayPaths(int vtex_index) const;

    // Display all adjacent vertices to a given starting
    // vertex (with a trailhead titled start_loc_name).
    int displayAdjacent(const char * start_loc_name) const;
    // Display all adjacent vertices to a
    // given starting vertex, by vtex_index.
    int displayAdjacent(int vtex_index) const;

    // Display all of the vertices (trailheads and
    // intersections) in the order they were inserted.
    int displayVertices(void) const;
    // Display all Trailheads in the order the were inserted.
    int * displayTrailheads(void) const;
    // Display all intersecting trails in the order they were inserted.
    int * displayIntersections(void) const;

    // Returns the number of vertices.
    int vertexCount(void);

    // Returns the length of the first trail in some intersection.
    int isecFrom(int index) const;
    // True if the adjacency list is full.
    bool isFull(void);

    // True if the vertex at index is a trailhead.
    bool isThead(int index);
    // True if the vertex at index is an intersection.
    bool isIsec(int index);


  private:

    // Displays all possible paths through
    // the graph, from some starting vertex.
    int displayPaths( Vertex * start_vtex,
                      Vertex ** av_list[],
                      int cur_av,
                      int indent ) const;

    // Displays the adjacency list for a given vertex starting at head.
    // This function is called by both of the public displayAdjacent
    // with the head of the ad list for the starting vertex.
    int displayAdjacent( Node * head,
                         int count ) const;

    // Displays an intersection of two trails.
    int displayIsec(Vertex * vtex) const;

    // Returns a pointer to an array of 2 * indent_num spaces.
    // Used to display an indent for lines in displayPaths.
    char * indent(int indent_num) const;

    // List of all adjacent vertices.
    Vertex * adjacency_list;

        // The maximum number of vertices.
    int list_max,
        // The current number of vertices.
        list_cur,
        // The current number of edges.
        edge_cur;
};
#endif // GRAPH_H