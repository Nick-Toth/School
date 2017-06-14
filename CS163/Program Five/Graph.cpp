/*****************************************************************
// File Name:  Graph.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       June 9th, 2017
// Class:      CS163
// Program #:  5
//
// Overview: This is the implementation file for the Graph for
// programming assignment #5. Essentially, this graph is used to
// represent trails surrounding Portland, and the relationships
// between them. See function comments in this file, and the Graph.h
// file for more information.
//
// ***************************************************************/


#include "Graph.h"



/* ********************************************
// Initializes a new Trailhead. The location
// and trail length are both optional. By
// default, loc_name = NULL and length = 0.
//
// ********************************************/
Trailhead :: Trailhead( char * loc_name,
                        int length )
{
  // If a (non-NULL) name was passed in..
  if(loc_name)
  {
    // Allocate memory for, and copy the name.
    location = new char[strlen(loc_name) + 1];
    strcpy(location, loc_name);
  }

  // Initialize location to NULL.
  else location = NULL;

  // Initialize remaining pointers to NULL.
  description = NULL;

  // Set trail_length to length, default is 0.
  trail_length = length;

  return;
}



/* ********************************************
// Copies the contents of a preexisting
// Trailhead object.
//
// ********************************************/
Trailhead :: Trailhead(const Trailhead & t_head_src)
{
  // If the passed trailhead is NULL..
  if(!t_head_src.location)
  {
    // Initialize pointers to NULL.
    location = NULL;
    description = NULL;

    // Exit!
    return;
  }

  // Allocate memory proportional to the
  // source Trailhead's members to the
  // corresponding members to this Trailhead.
  location = new char[strlen(t_head_src.location) + 1];
  // Copy the contents of the source Trailhead's members
  // to the corresponding members to this Trailhead.
  strcpy(location, t_head_src.location);

  // If the description hasn't been set (this trailhead
  // is for an intersection of two existing trails), return.
  if(!t_head_src.description) return;

  // Copy the length of the source Trailhead.
  trail_length = t_head_src.trail_length;

  // Allocate memory for the description.
  description = new char[strlen(t_head_src.description) + 1];
  // Copy t_head_src's description.
  strcpy(description, t_head_src.description);

  return;
}



/* ********************************************
// Deallocates all dynamic memory in a given
// Trailhead.
//
// ********************************************/
Trailhead :: ~Trailhead(void)
{
  // If the location is not NULL..
  if(location)
  {
    // Delete and nullify it.
    delete[] location;
    location = NULL;
  }

  // If the description is not NULL..
  if(description)
  {
    // Delete and nullify it.
    delete[] description;
    description = NULL;
  }

  // Reset trail length to 0.
  trail_length = 0;

  return;
}



/* ********************************************
// Prints out the contents of a trailhead, or,
// if the trailhead is being used to store
// an intersection between trailheads (indicated
// by a trail_length of -1), set the fst and snd
// parameters to the indices of the trailheads
// in the adjacency list which intersect.
//
// ********************************************/
bool Trailhead :: displayTrailhead( int & fst,
                                    int & snd )
{
  // If the Trailhead's members are
  // NULL, return false immediately!
  if(!location || !description) return false;

  // If the trail_length indicates that this Trailhead
  // contains data for a trailhead intersection..
  if(trail_length == -1)
  {
    // Set the fst parameter to the integer representation
    // of location (index of first trail in intersection).
    fst = atoi(location);
    // Set the snd parameter to the integer representation
    // of description (index of second trail in intersection).
    snd = atoi(description);

    // Return false (Must be printed by Graph, as this
    // struct cannot access the adjacency list containing
    // the Trailheads specified by location and description).
    return false;
  }

  // Display the Trailhead data.
  std::cout << "\n\tName: " << location << std::endl
            << "\tLength: " << trail_length << " Kilometers." << std::endl
            << "\tDescription: " << description
            << std::endl << std::endl;

  // Report success!
  return true;
}



/* ********************************************
// Sets up a the Trailhead on which this
// function is called to store an intersection
// between two other trailheads. Character
// representations of the indices in a Graph's
// adjacency list which contain the intersecting
// trails are passed in, and stored in location
// and description, and the trail length is set
// to -1.
//
// ********************************************/
bool Trailhead :: setupAsIntersection( char * from_idx,
                                       char * to_idx )
{
  // If either parameter is NULL, return false.
  if(!from_idx || !to_idx) return false;

  // Delete any dynamic memory.
  if(location) delete[] location;
  if(description) delete[] description;

  // Reset trail_length to -1 to indicate that
  // the trailhead contains an intersection.
  trail_length = -1;

  // Allocate memory for, and initialize the location, and description
  // which will be used to store character representations
  // of the indices which store the two trails which intersect.
  location = new char[strlen(from_idx) + 1];
  description = new char[strlen(to_idx) + 1];
  strcpy(location, from_idx);
  strcpy(description, to_idx);

  // Report success.
  return true;
}



/* ********************************************
// Deallocates the edgelist in some vertex.
//
// ********************************************/
Node :: ~Node(void)
{
  // Delete and nullify the next node in the edgelist.
  if(next)
  {
    delete next;
    next = NULL;
  }

  // Set vertex pointer to NULL.
  adj_vtex = NULL;

  return;
}



/* ********************************************
// Default Graph constructor - initializes the
// adjacency list memory, sets pointers to NULL,
// and integer fields to 0.
//
// ********************************************/
Graph :: Graph(int list_max) : list_max(list_max)
{
  // Index counter for initializing the adjacency list.
  int index = 0;

  // Initialize current vertex and edge counters to 0.
  list_cur = 0;
  edge_cur = 0;

  // Allocate memory for a new array of vertices.
  adjacency_list = new Vertex[list_max];

  // Set all vertex pointers to NULL.
  for(; index < list_max; ++index)
  {
    adjacency_list[index].data = NULL;
    adjacency_list[index].head = NULL;
  }

  return;
}



/* ********************************************
// Copy constructor for potential future use.
//
// ********************************************/
Graph :: Graph(const Graph & Graph_src)
{ return; }



/* ********************************************
// Deallocates all dynamic memory used in the
// Graph on which this function is called.
//
// ********************************************/
Graph :: ~Graph(void)
{
  // General index counter for deallocating memory.
  short index = 0;

  // If the adjacency list is NULL, there is no
  // dynamic memory to deallocate. Return.
  if(!adjacency_list) return;

	// For every vertex in the adjacency list..
  for(; index < list_cur; ++index)
  {
    // If the current vertex isn't NULL..
    if(adjacency_list + index)
    {
      // If the current vertex has
      // a non-null trailhead..
      if(adjacency_list[index].data)
      {
        // Delete and nullify the Trailhead in the
        // adjacency list at the current index.
        delete adjacency_list[index].data;
        adjacency_list[index].data = NULL;
      }
      // If the trailhead has
      // a non-null edgelist..
      if(adjacency_list[index].head)
      {
        // Delete and nullify the edgelist in the 
        // adjacency list at the current index.
        delete adjacency_list[index].head;
        adjacency_list[index].head = NULL;
      }
    }
  }

  // Delete and nullify the adjacency list.
  delete[] adjacency_list;
  adjacency_list = NULL;

  return;
}



/* ********************************************
// Inserts a new Trailhead into the next open
// index of the adjacency list. If there is no
// room for another vertex, returns -1. If the
// insertion is successfull, return the new
// size.
//
// ********************************************/
int Graph :: newVertex(const Trailhead & new_trailhead)
{

  // If the list is full, return error code -1.
  if(list_cur == list_max) return -1;

  // Copy the new trailhead info into the
  // adjacency_list at the next open index.
  adjacency_list[list_cur].data = new Trailhead(new_trailhead);

  // Increment the vertex counter.
  ++list_cur;

  // Return the new size of the list.
  return list_cur;
}



/* ********************************************
// Creates an edge between two vertices,
// specified by index in from_vtex and to_vtex.
// edge_length is the length of the edge.
//
// ********************************************/
int Graph :: joinVertices( int from_vtex,
                           int to_vtex,
                           int edge_length )
{

  // If the graph is empty, or either of the
  // vertices are NULL, return error code 0.
  if( !adjacency_list ||
      !adjacency_list[from_vtex].data ||
      !adjacency_list[to_vtex].data ) return 0;

  // Stores the length of the edgelist in
  // the vertex given by from_vtex to return.
  int adjacent = 0;

  // Temp node for traversing the edge list
  // of the vertex given by from_vtex. Also used
  // to allocate the new Node, where applicable.
  Node * cur_edge = adjacency_list[from_vtex].head;

  // Traverse the edge list to check if the vertex
  // vertex given by to_vtex already exists in the
  // edge list of the vertex given by from_vtex.
  while(cur_edge)
  {
    // Increment the edgelist length counter.
    ++adjacent;

    // If it does already exist in the list..
    if(!strcmp(adjacency_list[to_vtex].data->location, cur_edge->adj_vtex->data->location) &&
       !strcmp(adjacency_list[to_vtex].data->description, cur_edge->adj_vtex->data->description) )
         // Return the (NEGATIVE!!) index where the
         // vertex given by to_vtex already exists in
         // the edgelist of the vertex given by from_vtex.
         return -adjacent;

    // Update edgelist traversal node.
    cur_edge = cur_edge->next;
  }

  // Create a new node.
  cur_edge = new Node;
  // Insert it at the front of from_vtex's edge list.
  cur_edge->next = adjacency_list[from_vtex].head;

  // Set new node's chain length to the
  // current head's chain length + 1.
  if(adjacency_list[from_vtex].head) cur_edge->chain_len = adjacency_list[from_vtex].head->chain_len + 1;
  else cur_edge->chain_len = 1;

  // Set the edge node vertex pointer to the vertex given by to_vtex.
  cur_edge->adj_vtex = adjacency_list + to_vtex;
  // Store the length of the edge.
  cur_edge->edge_len = edge_length;

  // Set the head of the edge list to the new edge node.
  adjacency_list[from_vtex].head = cur_edge;

  // Return the (updated) length
  // of from_vtex's edgelist.
  return adjacent + 1;
}



/* ********************************************
// Public displayPaths (char *) - Displays all
// possible paths through the graph, from some
// starting vertex.
//
// ********************************************/
int Graph :: displayPaths(const char * start_vtex) const
{
  // If the specified trail name is NULL, return error code -2.
  if(!start_vtex) return -2;
  // If the adjacency list is NULL, return error code -1.
  if(!adjacency_list) return -1;

  // Index counter for traversing the adjacency list.
  int index = 0;
  // Loop control.
  bool match = false;

  // Iterate through the adjacency list.
  while(match && index < list_cur && adjacency_list[index++].data->trail_length != -1)
    // Update match with the result of comparing the current
    // vertex in the adjacency list and the char * parameter.
    match = !strcmp(adjacency_list[index++].data->location, start_vtex);

  // If the specified vertex has no adjacent vertices, return 0.
  if(!match || !adjacency_list[index - 1].head) return 0;

  // Already visited vertices.
  Vertex ** av_list[list_cur];
  // Init av_list to NULL.
  for(short i = 0; i<list_cur; ++i) av_list[i] = NULL;

  // Call the private display paths function with the specified vertex.
  return displayPaths(adjacency_list + (index-1), av_list, 0, 1);
}



/* ********************************************
// Public displayPaths (int) - Displays all
// possible paths through the graph, from some
// starting vertex.
//
// ********************************************/
int Graph :: displayPaths(int vtex_index) const
{
  // If the specified index is out of range, return error code -2.
  if(vtex_index < 0 || vtex_index > list_cur - 1) return -2;

  // If the adjacency list is NULL, return error code -1.
  if(!adjacency_list) return -1;

  // If the specified vertex has no adjacent vertices, return 0.
  if(!adjacency_list[vtex_index].head) return 0;

  // Already visited vertices.
  Vertex ** av_list[list_cur];
  // Init av_list to NULL.
  for(short i = 0; i<list_cur; ++i) av_list[i] = NULL;

  // Display the paths from the specified vertex.
  return displayPaths(adjacency_list + vtex_index, av_list, 0, 1);
}



/* ********************************************
// Private displayPaths - Displays all possible
// paths through the graph, from some starting
// vertex.
//
// ********************************************/
int Graph :: displayPaths( Vertex * start_vtex,
                           Vertex ** av_list[],
                           int cur_av,
                           int indent_count ) const
{

  // If anything important is NULL, return 0.
  if(!start_vtex || !start_vtex->data || !av_list) return 0;

      // Counter for the number of paths - for displaying labels.
  int path_count = 0,
      // Total number of paths to be returned.
      display_path_sum = 0,
      // Counter for inserting av_locations.
      av_count = 0,
      // General index counter.
      g_index = 0;

  // Store a newline, and an indent of indent_count
  // * 2 spaces. This is used to indent lines based
  // on the depth of the path from the original vertex.
  char * ident = indent(indent_count);

  // Ensure that the current vertex has not already
  // been displayed as part of a previous path.
  for(; g_index < list_cur && av_list[g_index]; ++g_index)
    if( start_vtex == *av_list[g_index])
      return 0;

  // Add a pointer to the current vertex
  // to the array of visited vertices.
  av_list[cur_av++] = & start_vtex;

  // Determine if this vertex is a
  // trailhead or an intersection.
  bool is_isec = start_vtex->data->trail_length == -1;

  // Initialize a node for traversing the
  // edge list of the current vertex.
  Node * traversal_node = start_vtex->head;

  // Nothing in the edge list -> nothing to display.
  if(!traversal_node) return 0;

  // Display the vertex (start_vtex) from
  // which all paths are being printed.

  // If the current vertex is an intersection..
  if(is_isec)
  {
    // Display the intersection.
    std::cout << "From the intersection: "
              << ident << std::endl;
    displayIsec(start_vtex);
  }
  // Otherwise (the current vertex is a trailhead), display the trailhead.
  else std::cout << "From the trailhead: " << start_vtex->data->location << std::endl;

  // Display pre-edgelist label.
  std::cout << ident << "  you can go to:" << std::endl;

  // Loop through the edge list of the current vertex.
  do
  {
    // Display a label for the current path option.
    std::cout << ident
              << "Location #" << (path_count + 1) << ':' << std::endl
              << ident;

    // If the current vertex is an intersection..
    if(traversal_node->adj_vtex->data->trail_length == -1)
      // Display the intersection.
      displayIsec(traversal_node->adj_vtex);
    // Otherwise (the current vertex is a trailhead.)
    else
      // Display the trailhead.
      std::cout << "      " << traversal_node->adj_vtex->data->location
                << std::endl;

    // Insert the current vertex into the visited list.
    av_list[av_count++] = &traversal_node->adj_vtex;
  
    // Call this function on the current vertex.
    display_path_sum = 1 + displayPaths(traversal_node->adj_vtex, av_list, cur_av, indent_count + 1);

    // Increment the path label counter.
    ++path_count;
    // Store the next node in the edge list.
    traversal_node = traversal_node->next;
  }
  // Continue until the edgelist has been fully traversed.
  while(traversal_node);

  // Erase the memory used for indenting lines.
  delete[] ident; ident = NULL;

  // Return the total number of paths.
  return display_path_sum;
}



/* ********************************************
// Returns a pointer to an array of indent_num
// spaces to be printed before lines in the
// displayPaths function.
//
// ********************************************/
char * Graph :: indent(int indent_num) const
{
  // Create an array for storing the indent.
  char * indent = new char[indent_num * 2 + 2];
  
  // Index counter for initializing the indent array.
  int i = 1;

  // Set the first element to new line.
  indent[0] = '\n';

  // Fill the indent array with spaces.
  for(; i < indent_num * 2; ++i)
    *(indent + i) = ' ';

  // Terminate the indent.
  *(indent + i) = '\0';

  // Return the indent.
  return indent;
}



/* ********************************************
// Public displayAdjacent (char *) - Display all
// adjacent vertices to a given starting vertex,
// by index.
//
// ********************************************/
int Graph :: displayAdjacent(const char * start_vtex) const
{
  // If the start vertex is NULL, return error code -3.
  if(!start_vtex) return -3;
  // If the adjacency list is NULL or empty, return error code -2.
  if(!adjacency_list || list_cur < 1) return -2;

      // General index counter.
  int index = 0,
      // Sum of adjacent vertices to be returned.
      return_sum = 0;

  // Loop control / exit control for locating specified vertex.
  bool match = false;

  // For every vertex in the adjacency list, until all
  // entries have been checked or a match is found..
  while(index < list_cur && !match)
    // Set match to the comparison between the specified
    // vertex label and the current vertex's trailhead label
    match = !strcmp(adjacency_list[index++].data->location, start_vtex);

  // If no match was found, return error code -1
  // ( the specified vertex does not exist ).
  if(!match) return -1;

  // Decrement the index counter to get the
  // correct index of the located vertex.
  --index;

  // If the edgelist is NULL, and does not
  // contain an intersection report no edges.
  if(!adjacency_list[index].head && adjacency_list[index].data->trail_length != -1) return 0;

  // Display pre-adjacent vertices header.
  std::cout << "\n  Displaying all trailheads and trail intersections near ";

  // If the specified vertex is an intersection..
  if(adjacency_list[index].data->trail_length == -1)
  {
    // Display intersection-addition to the pre-adjacent vertices header.
    std::cout << "\n  the intersection of the following trails: " << std::endl;
    // Display the intersection.
    displayIsec(adjacency_list + index);
    // Add 2 to the return sum.
    return_sum = 2;
  }
  // Display trailhead-addition to the pre-adjacent vertices header.
  else std::cout << adjacency_list[index].data->location << std::endl;

  // Display the adjacent vertices to the specified vertex.
  return return_sum + displayAdjacent(adjacency_list[index].head, 1);
}



/* ********************************************
// Public displayAdjacent (int) - Display all
// adjacent vertices to a given starting vertex,
// by index.
//
// ********************************************/
int Graph :: displayAdjacent(int vtex_index) const
{
  // If the vertex index is invalid, return error code -2.
  if(vtex_index < 0 || vtex_index > list_cur - 1) return -2;

  // If the adjacency list is NULL or empty, return error code -1.
  if(!adjacency_list || list_cur < 1) return -1;

  // If the edge list for the specified vertex is NULL, and
  // the vertex is not an intersection, return error code -1.
  if(!adjacency_list[vtex_index].head && adjacency_list[vtex_index].data->trail_length != -1) return 0;

  // Sum of adjacent vertices to be returned.
  int return_sum = 0;

  // Display pre-adjacent vertices header.
  std::cout << "\n  Displaying all trailheads and trail intersections near ";

  // If the specified vertex is an intersection..
  if(adjacency_list[vtex_index].data->trail_length == -1)
  {
    // Display intersection-addition to the pre-adjacent vertices header.
    std::cout << "\n  the intersection of the following trails: " << std::endl;
    // Display the intersection.
    displayIsec(adjacency_list + vtex_index);
    // Add 2 to the return sum.
    return_sum = 2;
  }
  // Display trailhead-addition to the pre-adjacent vertices header.
  else std::cout << adjacency_list[vtex_index].data->location << std::endl;

  // Display the adjacent vertices to the specified vertex.
  return return_sum + displayAdjacent(adjacency_list[vtex_index].head, 1);
}



/* ********************************************
// Private displayAdjacent - Displays the
// adjacency list for a given vertex starting at
// head. This function is called by both of the
// public displayAdjacent with the head of the
// ad list for the starting vertex.
//
// ********************************************/
int Graph :: displayAdjacent( Node * head,
                              int count ) const
{
  // If the current edgelist node is NULL, or its vertex's
  // is NULL, or its vertex's data is NULL, return 0.
  if(!head || !head->adj_vtex || !head->adj_vtex->data) return 0;

  // If the current node's trailhead stores an intersection..
  if(head->adj_vtex->data->trail_length == -1)
  {
    // Display a label, and the intersection.
    std::cout << "\n   Adjacent location # " << count
              << " is an intersection between two other trails."
              << std::endl;
    displayIsec(head->adj_vtex);
  }
  // Otherwise ( the current node's trailhead is not an intersection)..
  else
  {
    // Display the trailhead.
    std::cout << "\n   Displaying adjacent location # " << count 
              << std::endl;
    head->adj_vtex->data->displayTrailhead(count, count);
  }

  // Call recursively on the next node in the edgelist.
  // Add 1 to the number of adjacent vertices.
  return 1 + displayAdjacent(head->next, count + 1);
}



/* ********************************************
// Displays all vertices in the order they were
// inserted. Return list_cur.
//
// ********************************************/
int Graph :: displayVertices(void) const
{
      // Index counter for traversing the adjacency list.
  int index = 0,
      // Indices used for displaying intersecting trails.
      f_dex = 0, // First.
      s_dex = 0; // Second.

  // If the Graph is empty, return 0.
  if(list_cur == 0 || !adjacency_list) return 0;

  // Display an alert that the trailheads will all be displayed.
  std::cout << "\n  Displaying all vertices in the order they were inserted." << std::endl;

  // For every trailhead..
  for(; index < list_cur ; ++index)
  {
    // Reset the intersecting trail indices.
    f_dex = 0; s_dex = 0;

    // Display a label for the current vertex number.
    std::cout << "\n\n    Vertex #" << index + 1 << ':' << std::endl;

    // Display the vertex. If display fails and the intersection indices
    // are not the same (really, what's important is that they are not
    // both zero, but this is cheaper and won't cause any issues)..
    if(!adjacency_list[index].data->displayTrailhead(f_dex, s_dex) && f_dex != s_dex)
    {
                // Display an alert that the current vertex is an intersection.
      std::cout << "\n      This vertex is an intersection of two trailheads! " << std::endl
                // Display a label for the first trailhead in the intersection.
                << "\n      The first trailhead is:" << std::endl;

      // Display the first trailhead in the intersection.
      adjacency_list[f_dex].data->displayTrailhead(f_dex, s_dex);

      // Display a label for the second trailhead in the intersection.
      std::cout << "\n      The second trailhead is:" << std::endl;
      // Display the second trailhead in the intersection.
      adjacency_list[s_dex].data->displayTrailhead(f_dex, s_dex);
    }
  }

  // Return the number of vertices.
  return list_cur;
}



/* ********************************************
// Displays all Trailheads in the order they
// were inserted. Return an array containing
// the indices of displayed trailheads.
//
// ********************************************/
int * Graph :: displayTrailheads(void) const
{
      // Index counter for traversing the adjacency list.
  int index = 0,
      // Used only as necessary arguments for displayTrailhead.
      x = 0, y = 0,
      // Used to display a trailhead number.
      thead_label_num = 1,
      // Array containing the indices of displayed trailheads.
      * thead_idcs = new int[list_cur];

  // Init number of intersections to 0.
  thead_idcs[0] = 0;

  // If the Graph is empty, return 0.
  if(list_cur == 0) return thead_idcs;
  
  // Display an alert that the trailheads will all be displayed.
  std::cout << "\n  Displaying all trailheads in the order they were inserted." << std::endl;

  // For every trailhead..
  for(; index < list_cur ; ++index)
  {
    // Check that the current entry is not an intersection..
    if(adjacency_list[index].data->trail_length != -1)
    {
      // Display the current Trailhead #.
      std::cout << "\n\n    Trailhead #" << thead_label_num << ':' << std::endl;

      // Display the current trailhead.
      adjacency_list[index].data->displayTrailhead(x, y);

      // Insert the index of this trailhead.
      thead_idcs[thead_label_num] = index;

      // Increment the label number.
      ++thead_label_num;
    }
  }

  // Update the array's first value to the
  // number of trailheads. i.e. its size.
  thead_idcs[0] = (thead_label_num - 1);

  // Return the array of indices.
  return thead_idcs;
}



/* ********************************************
// Displays all Trailhead intersections in the
// order they were inserted. Return an array
// containing the indices of displayed
// intersections.
//
// ********************************************/
int * Graph :: displayIntersections(void) const
{

      // Index counter for traversing the adjacency list.
  int index = 0,
      // Indices used for displaying intersecting trails.
      f_dex = 0, // First.
      s_dex = 0, // Second.
      // Contains number for intersection label.
      label_num = 1,
      // Array containing the indices of displayed intersections.
      * isec_idcs = new int[list_cur];

  // Init number of intersections to 0.
  isec_idcs[0] = 0;

  // If the Graph is empty, return 0.
  if(list_cur == 0) return isec_idcs;

  // Display an alert that the trailheads will all be displayed.
  std::cout << "\n  Displaying all intersections in the order they were inserted." << std::endl;

  // For every trailhead..
  for(; index < list_cur; ++index)
        // If the current Vertex's Trailhead has
        // a length of 1 (it is an intersection)..
    if( adjacency_list[index].data->trail_length == -1 &&
        // Get the indices of the intersecting
        // trails in the adjacency list.
        !adjacency_list[index].data->displayTrailhead(f_dex, s_dex) &&
        // If the indices are not the same (as explained
        // in displayVertices what's important
        // is that they are not both zero, but this
        // is cheaper and won't cause any issues)..
        f_dex != s_dex )
    {

                // Display a label for the intersection being displayed.
      std::cout << "\n\n    Intersection #" << label_num << ':'
                << std::endl << std::endl
                // Display a label for the first trail in the intersection.
                << "      The first trailhead is:" << std::endl;

      // Display the first trail in the intersection.
      adjacency_list[f_dex].data->displayTrailhead(f_dex, s_dex);

      // Display a label for the second trail in the intersection.
      std::cout << "\n      The second trailhead is:" << std::endl;
      // Display the second trail in the intersection.
      adjacency_list[s_dex].data->displayTrailhead(f_dex, s_dex);

      // Insert the index of this intersection.
      isec_idcs[label_num] = index;

      // Increment the intersection label counter.
      ++label_num;
    }

  // Update the array's first value to the
  // number of intersections. i.e. its size.
  isec_idcs[0] = (label_num-1);

  // Return the number of entries..
  return isec_idcs;
}



/* ********************************************
// Display an intersection of two trails.
//
// ********************************************/
int Graph :: displayIsec(Vertex * vtex) const
{
  // If the adjacency list is NULL, return error code -2.
  if(!adjacency_list) return -2;
  // If the specified vertex, or its data is NULL, or the vertex contains
  // specifications for a trail intersection, return error code -1.
  if(!vtex || !vtex->data || vtex->data->trail_length != -1) return -1;

  // Store the indices of the trailhead vertices of the paths
  // which constitute the intersection represented in vtex.
  int first = 0, second = 0;
  vtex->data->displayTrailhead(first, second);

  // If the index retrieval failed, report failure.
  if(first == 0 && second == 0) return 0;

  // Display the intersecting trails.
  std::cout << "\n    Displaying Intersection of trails: " << std::endl
            << "\n      Trail #1: "<< std::endl;
  adjacency_list[first].data->displayTrailhead(first, second);
  std::cout << "\n      Trail #2: " << std::endl;
  adjacency_list[second].data->displayTrailhead(first, second);

  // Report success.
  return 1;
}



/* ********************************************
// Returns the number of vertices in the graph.
//
// ********************************************/
int Graph :: vertexCount(void)
{ return list_cur; }



/* ********************************************
// Returns true if index is in range of the
// current Graph size, and the vertex at index
// is a regular Trailhead..
//
// ********************************************/
bool Graph :: isThead(int index)
{ 
  // If the index is valid
  if( index < 0 ||
      index > list_cur - 1 ||
      // or if theh data at the
      // specified vertex is NULL
      !adjacency_list[index].data ||
      // or if the specified vertex contains
      // an intersection, return false.
      adjacency_list[index].data->trail_length == -1) return false;

  return true;
}



/* ********************************************
// Returns true if index is in range of the
// current Graph size, and the vertex at index
// is an intersection.
//
// ********************************************/
bool Graph :: isIsec(int index)
{ 
  // If the index is valid
  if( index < 0 ||
      index > list_cur - 1 ||
      // or if theh data at the
      // specified vertex is NULL
      !adjacency_list[index].data ||
      // or if the specified vertex does not
      // contain an intersection, return false.
      adjacency_list[index].data->trail_length != -1) return false;

  return true;
}



/* ********************************************
// Returns the index of the trailhead from
// which the edge is drawn in the intersection
// specified by the index parameter.
//
// ********************************************/
int Graph :: isecFrom(int index) const
{
  // If the index is out of range, return error code -3.
  if(index < 0 || index > list_cur - 1) return -3;
  // If the data at the vertex at the specified index is
  // NULL, return error code -2.
  if(!adjacency_list[index].data) return -2;
  // If the vertex at the specified index does not represent
  // a trail intersection, return error code -1.
  if(adjacency_list[index].data->trail_length != -1) return -1;

  // Store the index of the trailhead from which the edge is drawn.
  index = atoi(adjacency_list[index].data->location);

  // Return the length of the trail specified above.
  return adjacency_list[index].data->trail_length;
}



/* ********************************************
// Returns true if the Graph is full.
//
// ********************************************/
bool Graph :: isFull(void)
{ return list_cur == list_max; }
