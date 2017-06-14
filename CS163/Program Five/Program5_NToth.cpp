/*****************************************************************
// File Name:  Program5_NToth.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       June 9th, 2017
// Class:      CS163
// Program #:  5
//
// Overview: This is the main / testing file for the fifth
// programming assignment. The focus of the fifth assignment is to
// create a graph for navigating local hiking trails. The functions
// in this file are used to test out the various features of the
// graph. See Grapg.cpp/h for more informaton.
//
// ***************************************************************/


// For reading trail data.
#include <fstream>
// For initializing edges.
#include <ctime>
// For fmod.
#include <cmath>

#include "Graph.h" // Includes iostream, cstring, cstdlib


using namespace std;


             // Filename for the text file containing trailhead names
             // and descriptions. See Trailheads.txt for more information.
const char * TRAIL_INFO_FILENAME = "Trailheads.txt",
             // Filename for the file containing trail intersections.
             // See Trail_Intersections.txt for more information.
           * TRAIL_INT_FILENAME  = "Trail_Intersections.txt",
             // Filename for the file containing trail lengths.
             // See Trail_Lengths.txt for more information.
           * TRAIL_LEN_FILENAME = "Trail_Lengths.txt";


// The number of trailheads in the datafile.
const int DATA_FILE_SIZE = 18;


// Tuple for storing intersection vertices.
struct Vtex_Tup
{
  // The indices of the first and
  // second trails in an intersection.
  int fst, snd;

  // Setup the indices with the given parameters.
  void set(int fst, int snd)
    //
    {this->fst = fst; this->snd = snd;}
};


// *****************  |
// Menu & Handlers *  |
// *****************  V

// Display the menu / read in menu selections.
void menuController(int & selection);

// Prompt for enter to continue.
void continueMenu(void);

// Insert a new vertex.
void handleInsertVtex(Graph & graph);
// Create a new edge between two vertices.
void handleJoinVtex(Graph & graph);
// Display all possible trails from a given vertex.
void handleDisplayTrails(Graph & graph);
// Display the possible immediate trailheads
// from a given starting trailhead.
void handleDisplayNear(Graph & graph);
// Display all of the trailheads.
int * handleDisplayTheads(Graph & graph);
// Display all of the trail intersections.
int * handleDisplayIsecs(Graph & graph);

// ******************* |
// Testing Functions * |
// ******************* V

// Prompt the user to enter Y or N. Return true for Y.
bool confirmChoice(char fst_choice, char snd_choice);

// Initialize the graph.
Graph * initGraph(void);

// Loads trailhead information.
int loadTrailheads( Trailhead TRAILHEAD_BUFFER[DATA_FILE_SIZE],
                    int TRAIL_LENGTH_BUFFER[],
                    int trail_length );

// Load trail intersections.
int loadIntersections(int INTERSECTION_BUFFER[DATA_FILE_SIZE][DATA_FILE_SIZE]);

// Load lengths of the trails from
// the external trail length file.
int loadTrailLengths(int TRAIL_LENGTH_BUFFER[]);

// Initialize an array of tuples with
// intersecting path indices.
int texTupInit( Vtex_Tup texs[],
                int INTERSECTION_BUFFER[DATA_FILE_SIZE][DATA_FILE_SIZE],
                int intersections );

// Inserts edges between trailheads
// and trailhead intersections.
int insertEdges( Graph *& graph,
                 Vtex_Tup texs[],
                 int intersections,
                 Trailhead TRAILHEAD_BUFFER[DATA_FILE_SIZE] );

// Read in a char[] - for initializing trailheads.
void readValue( char *& data_loc,
                const char * data_type,
                int max_len );

// Read in a number, with the specified title.
int readNum(const char * data_type);

// Reads in a vertex (Trailhead or intersection) from the user.
int vtexSelection( const char * tex_type,
                   Graph & graph );

// Checks for / handles graph display error..
void displayLocationError(int return_val);



int main(void)
{
  // Allocate and initialize a new graph.
  Graph * graph = initGraph();

  // User's menu selection.
  int selection = 0;

  // Main loop control variable.
  bool quit = false;

  // Display out program header message.
  cout << "\n ----------------- Event Management Program ----------------- \n" << endl;

	// Pointer to an array of integers returned by handleDisplayTheads
	// and handleDisplayIsecs - Used in other display functions to store
	// indices for user selection. Must be simply be deallocated here
	// (and in those functions, for that matter).
	int * disp_index_ret = NULL;

  do
  {
    // Print the program's main menu, read in the user's selection.
    menuController(selection);

    // Execute the handler function for the user's selection.
    switch(selection)
    {
      case 1: // Execute new vertex handler.
        handleInsertVtex(*graph);
        break;
      case 2: // Execute new edge handler.
        handleJoinVtex(*graph);
        break;
      case 3: // Execute display trails handler.
        handleDisplayTrails(*graph);
        break;
      case 4: // Execute display adjacent handler.
        handleDisplayNear(*graph);
        break;
      case 5: // Execute display all trailheads handler.
        disp_index_ret = handleDisplayTheads(*graph);
	// If display returned an array, deallocate and nullify it.
	if(disp_index_ret)
	{ delete[] disp_index_ret; disp_index_ret = NULL; }
        break;
      case 6: // Execute display all intersections handler.
        disp_index_ret = handleDisplayIsecs(*graph);
	// If display returned an array, deallocate and nullify it.
	if(disp_index_ret)
	{ delete[] disp_index_ret; disp_index_ret = NULL; }
        break;
      default: // 7 - Quit the program.
          quit = true;
        break;

    }

    // Reset selection.
    selection = 0;

    // If anything other than quit has
    // been selected from the menu..
    if(!quit)
      // Prompt the user to press enter
      // to continue, wait, continue.
      continueMenu();
  }
  // Run the program until quit is selected from the menu.
  while(!quit);

  // Deallocate the graph.
  delete graph; graph = NULL;

  // Display out program footer message.
  cout << "\n ------------------------ Thank You! ------------------------ " << endl << endl;

  return 0;
}



/* ***************************************************************
// Prints the program testing menu, reads in selections.
//
// @param selection: The location where the menu selection is
// stored.
//
// ***************************************************************/
void menuController(int & selection)
{
  // The number of selections in the menu.
  short max_selections = 7;

  // Print out the program menu.
  cout << "\n  Make a selection from the menu." << endl << endl << "    "
       << "[1] => Insert a new location." << endl << "    "
       << "[2] => Join two locations." << endl << "    "
       << "[3] => Display all trails from a starting location." << endl << "    "
       << "[4] => Display the adjacent locations." << endl << "    "
       << "[5] => Display all of the trailheads." << endl << "    "
       << "[6] => Display all of the trail intersections." << endl << "    "
       << "[7] => Exit the program" << endl;

  // If the user's selection has not been read in, or
  // was invalid, read in the user's menu selection.
  while(selection < 1 || selection > max_selections)
  {
    // Prompt the user to enter a selection
    cout << "\n  Enter the number corresponding to your selection: ";

    // Read in the user's selection
    cin >> selection;
    cin.ignore(100,'\n');

    // If the user's selection as invalid..
    if(selection < 1 || selection > max_selections)
      // Display an error message.
      cout << "\n  Invalid Entry! Please enter a number from 1 to " << max_selections
           << ".." << endl;
  }

  return;
}



/* ***************************************************************
// Displays prompt to enter confirmation, reads in and returns an
// entry.
//
// @param fst_choice: The first option char ( e.g. Y in (Y/N) ).
//
// @param snd_choice: The second option char ( e.g. N in (Y/N) ).
//
// @return: True if fst_choice is entered
//
// ***************************************************************/
bool confirmChoice(char fst_choice, char snd_choice)
{
  // Confirmation entry.
  char confirmation = '\0';

  // Prompt user to enter confirmation.
  cout << "\n  Please confirm (" << fst_choice << '/' << snd_choice << "): ";

  // Read in confirmation.
  cin >> confirmation;
  cin.ignore(100,'\n');
  cout << endl;

  // Convert entry to uppercase.
  confirmation = toupper(confirmation);

  // If the user has selected fst_choice, return true.
  if(confirmation == toupper(fst_choice)) return true;
  // If the user has selected snd_choice, return false.
  if(confirmation == toupper(snd_choice)) return false;

  // Display an alert that the entry is invalid.
  cout << "\n  Invalid entry! Please enter a \'"
       << fst_choice << "\' or an \'"
       << snd_choice << "\'.." << endl;

  // Try again.
  return confirmChoice(fst_choice, snd_choice);
}



/* ***************************************************************
// Prompts for entry to continue.
//
// ***************************************************************/
void continueMenu(void)
{
  // Prompt user to press enter.
  cout << "\n  Press enter to continue...";
  // Wait until user continues.
  cin.ignore(100,'\n');

  return;
}



/* ***************************************************************
// Menu handler for creating and inserting a new trailhead into
// the graph.
//
// ***************************************************************/
void handleInsertVtex(Graph & graph)
{
  // Check if the Graph is full..
  if(graph.isFull())
  {
    // Display an alert that the Graph is full.
    cout << "\n\tCannot insert new trailheads - graph is full!" << endl;
    return;
  }

  // Create a new Trailhead
  Trailhead new_thead;

  // Read in new trailhead info.
  readValue(new_thead.location, "Trailhead name", 64);
  readValue(new_thead.description, "Trailhead description", 512);
  new_thead.trail_length = readNum("Trail length");

  // Display alert that new trailhead is bring created.
  cout << "\n  Creating new Trailhead.." << endl;

  // Try to insert the new vertex.
  int insert_ret = graph.newVertex(new_thead);

  // Display an alert that a new trailhead was inserted.
  cout << "\n  Success! New graph size => " << insert_ret << endl;

  return;
}



/* ***************************************************************
// Menu handler for creating a new edge between two vertices in
// the graph.
//
// ***************************************************************/
void handleJoinVtex(Graph & graph)
{

      // The indices of the vertices being joined.
  int from_idx = 0, to_idx = 0,
      // The length of the edge between the specified vertices.
      edge_len = 0,
      // Return value from the graph's displayVertices function.
      graph_size = 0,
      // Return value from creating a new edge.
      create_edge_ret = 0;

  // Display information about selecting a location.
  cout << "\n  Here is a list of trailheads and trail intersections for" << endl
       << "  you to choose from. Please enter the numbers corresponding" << endl
       << "  the trailheads/intersections the you want to join.\n" << endl
       << "  Trailhead / intersection numbers are displayed above their" << endl
       << "  names and descriptions in a line formatted as follows:\n" << endl
       << "\tVertex # THE NUMBER" << endl << endl;

  // Prompt user to press enter to continue.
  continueMenu();

  // Display the graph, store the size.
  graph_size = graph.displayVertices();

  // If the display failed (the graph is empty)..
  if(graph_size == 0)
  {
    // Display failure message.
    cout << "\n  The graph is empty! Returning to the menu.." << endl;
    // Exit.
    return;
  }

  // Read in the first selection.
  do
  {
    from_idx = readNum("first trailhead or trail intersection number");

    // If the entry was too large, display error message.
    if(from_idx > graph_size) cout << "  Invalid selection, please try again!" << endl;
  }
  // Continue until the entry is valid.
  while(from_idx > graph_size);

  // Decrement first entry. (Read in starting at 1.. index @ 0.)
  --from_idx;

  // Read in the first selection.
  do
  {
    to_idx = readNum("second trailhead or trail intersection number");
    // If the entry was too large, display error message.
    if(to_idx > graph_size) cout << "  Invalid selection, please try again!" << endl;
  }
  // Continue until the entry is valid.
  while(to_idx > graph_size);

  // Decrement second entry. (Read in starting at 1.. index @ 0.)
  --to_idx;

  // Read in the edge length.
  edge_len = readNum("distance between the two vertices, in kilometers");

  // Try to create the new edge.
  create_edge_ret = graph.joinVertices(from_idx, to_idx, edge_len);

  // Display success / error message.
  if(create_edge_ret == 0) cout << "\n  Failed to join the specified vertices!" << endl;
  else cout << "\n Verticies successfully connected!" << endl;
  cout << "\n\n  Returning to the menu.." << endl;

  return;
}



/* ***************************************************************
// Menu handler for displaying all trails from a given starting
// vertex in the graph.
//
// ***************************************************************/
void handleDisplayTrails(Graph & graph)
{
  // Prompt for location type entry. This is sort of
  // unnecessary, but it does make less messy output.
  cout << "\n  Are you at a trailhead (T), or a trail intersection(I)?" << endl;

  // Store the corresponding label to the user's selection.
  const char * label = confirmChoice('T', 'I')? "trailhead" : "intersection";

      // User's selected vertex.
  int vertex_num = vtexSelection(label, graph),
      // Graph's displayAdjacent return value.
      display_ret = 0;

  // If the graph is empty, return.
  if(vertex_num == -1) display_ret = -1;
  // Try to display the adjacent vertices in the graph.
  else display_ret = graph.displayPaths(vertex_num);

  // Check for / handle display error..
  displayLocationError(display_ret);

  return;
}



/* ***************************************************************
// Menu handler for displaying all of the immediate vertices
// adjacent to a given starting vertex.
//
// ***************************************************************/
void handleDisplayNear(Graph & graph)
{

  // Prompt for location type entry. This is sort of
  // unnecessary, but it does make less messy output.
  cout << "\n  Are you at a trailhead (T), or a trail intersection(I)?" << endl;

  // Store the label for the type specified by the user.
  const char * label = confirmChoice('T', 'I')? "trailhead" : "intersection";

      // User's selected vertex.
  int vertex_num = vtexSelection(label, graph),
      // Graph's displayAdjacent return value.
      display_ret = 0;

  // If the graph is empty, return.
  if(vertex_num == -1) display_ret = -1;
  // Try to display the adjacent vertices in the graph.
  else display_ret = graph.displayAdjacent(vertex_num);

  // Check for / handle display error..
  displayLocationError(display_ret);

  return;
}



/* ***************************************************************
// Display error message for graph display methods.
//
// ***************************************************************/
void displayLocationError(int return_val)
{
  // Display some whitespace.
  cout << endl;

  if(return_val < 1)
    // Display an error message corresponding to the returned error code.
    switch(return_val)
    {
      case -2: // Empty graph.
        cout << "    Display Failed! The Graph is empty.." << endl;
        break;
      case -1: // Invalid vertex.
        cout << "    Display Failed! Invalid location." << endl;
        break;
      case 0: // No adjacent areas.
        cout << "    There are no trailheads near your specified location." << endl;
        break;
    }

  return;
}



/* ***************************************************************
// Read in a selected vertex from the user.
//
// ***************************************************************/
int vtexSelection( const char * tex_type,
                   Graph & graph )
{
  // If the vertex type is NULL, return error code -3.
  if(!tex_type) return -3;

  // Determine if the vertex type is a trailhead or an intersection.
  bool thead = !strcmp(tex_type, "trailhead");
  if(!thead && strcmp(tex_type, "intersection")) return -2;

      // User's selected vertex.
  int vertex_num = 0,
        // Array of returned indices for specified vertex type. Because
        // trailheads and intersections are handled differently, and
        // because they can be mixed in the graph's adjacency list, they
        // may display with a number that does not correspond to their index.
      * indices = NULL;

  // Confirmation for trailhead vs intersection.
  bool confirmed = false;

  // Display information about selecting a location.
  cout << "\n  Here is a list of " << tex_type << "s for you to choose from. Please" << endl
      << "  enter the number corresponding to the " << tex_type << " you want" << endl
      << "  to display surrounding trailheads and intersections from." << endl << endl
      << "  " << tex_type << " numbers are displayed above their names and descriptions" << endl
      << "  in a line formatted as follows:    " << tex_type << " # THE NUMBER" << endl;

  // Prompt user to press enter to continue.
  continueMenu();

  // Try to display the trailheads / intersections.
  indices = thead? handleDisplayTheads(graph) : handleDisplayIsecs(graph);

  // Report failure.
  if(!indices) return -1;

  // Read in the selected index.
  vertex_num = readNum(tex_type);

	// If the selection is invalid, return -1.
  if(vertex_num > indices[0])
  {
    // Deallocate the array of indices.
    delete[] indices; indices = NULL;
    return -1;
  }

  // Read in the user's selection.
  vertex_num = indices[vertex_num];

  // Confirm that the supplied index corresponds to a vertex of the specified type.
  confirmed = thead? graph.isThead(vertex_num) : graph.isIsec(vertex_num);

  // Deallocate the array of indices.
  delete[] indices; indices = NULL;

  // If the supplied index was incorrect..
  if(!confirmed)
  {
    // Display failure message
    cout << "\n\t  Invalid " << tex_type << " number. Please try again.." << endl;
    // Try again.
    return vtexSelection(tex_type, graph);
  }

  // If correct, return the selection.
  return vertex_num;
}



/* ***************************************************************
// Menu handler for displaying all of the trailheads in the graph.
//
// ***************************************************************/
int * handleDisplayTheads(Graph & graph)
{
  // Display some whitespace.
  cout << endl;

  // Try to display intersections, store the return val.
  int * graph_disp_ret = graph.displayTrailheads();

  // If the function returns 0 (no trailheads)..
  if(graph_disp_ret && graph_disp_ret[0] == 0)
  {
    // Display an alert that there are no trailheads in the graph.
    cout << "\n\t There are no trailheads to display!" << endl;

    // Deallocate the array of indices.
    delete[] graph_disp_ret; graph_disp_ret = NULL;
  }

  // Return the array.
  return graph_disp_ret;
}



/* ***************************************************************
// Display all of the trail intersections in the graph.
//
// ***************************************************************/
int * handleDisplayIsecs(Graph & graph)
{
  // Display some whitespace.
  cout << endl;

  // Try to display intersections, store the returned indices.
  int * graph_disp_ret = graph.displayIntersections();

  // If the function returns 0 (no intersections)..
  if(graph_disp_ret && graph_disp_ret[0] == 0)
  {
    // Display an alert that there are no intersections in the graph.
    cout << "\n\t There are no intersections to display!" << endl;

    // Deallocate the array of indices.
    delete[] graph_disp_ret; graph_disp_ret = NULL;
  }

  // Return the array.
  return graph_disp_ret;
}



/* ********************************************
// Reads in various types of data for creating
// traiheads and intersections.
//
// ********************************************/
void readValue( char *& data_loc,
                const char * data_type,
                int max_len )
{
  // If data_loc is not NULL, or the data type name is NULL.
  if(data_loc || !data_type) return;

  // Buffer for reading in the data_loc.
  char DATA_BUFFER[max_len];

  // Display prompt to enter a data_loc.
  cout << "\n  Please enter the " << data_type << ": ";

  // Read in the data_loc.
  cin.get(DATA_BUFFER, max_len, '\n');
  cin.ignore(128, '\n');
  cout << endl;

  // Allocate memory for the data_loc.
  data_loc = new char[strlen(DATA_BUFFER) + 1];

  // Store the contents of the data_loc
  // buffer into the data_loc array.
  strcpy(data_loc, DATA_BUFFER);

  // Display recieved data_loc.
  cout << endl << "      Using " << data_type << ": " << data_loc << endl << endl
       << "      Is this correct?" << endl;

  // Prompt for confirmation. If confirmed,
  // return to calling function.
  if(confirmChoice('Y', 'N')) return;

  // Display alert that the function will execute again.
  cout << "  Trying again!" << endl;

  // Otherwise..
  // Delete & nullify data_loc.
  delete[] data_loc;
  data_loc = NULL;

  // Keep trying.
  return readValue(data_loc, data_type, max_len);
}



/* ***************************************************************
// Read in and return a number, using the specified title.
//
// ***************************************************************/
int readNum(const char * data_type)
{
  // If the data type name is NULL.
  if(!data_type) return 0;

  // Location for the user's entry.
  int entry = 0;

  // Prompt for number entry.
  cout << endl << "\n\tPlease enter a positive whole number to initialize the "
       << data_type << ".." << endl << endl << endl
       << "    Enter number: ";

  // Read in the number.
  cin >> entry;
  cin.ignore(100, '\n');

  // If the entry is invalid, alert the user and try again.
  if(entry < 0)
    cout << "\n\tInvalid entry. Must be a positive whole number.."
         << endl << endl;
  else
  {
    // Display the user's entry.
    cout << endl << "      Using " << entry << '.' << endl
        << "      Is this correct?" << endl<< endl;

    // Prompt for confirmation. If confirmed,
    // return to calling function.
    if(confirmChoice('Y', 'N')) return entry;

    // Alert the user that the function will execute again.
    else cout << "\n  Retrying.." << endl << endl;
  }
  return readNum(data_type);
}



/* ***************************************************************
// Allocates and initializes a Graph with the contents of the data
// files. Returns a pointer to the new graph.
//
// ***************************************************************/
Graph * initGraph(void)
{
  // Temporary location for loading the trailhead data.
  Trailhead TRAILHEAD_BUFFER[DATA_FILE_SIZE];

  // General index counters for initializing the
  // intersection buffer / inserting into the graph.
  int index = 0, jndex = 0;

  // Temporary location for loading the trail intersections.
  // * DATA_FILE_SIZE^2 is the maximum number of
  //   possible intersections between two trails *
  int INTERSECTION_BUFFER[DATA_FILE_SIZE][DATA_FILE_SIZE],
      // Temporary location for storing trail lengths.
      TRAIL_LEN_BUFFER[DATA_FILE_SIZE];

  // Set INTERSECTION_BUFFER to all 0.
  for(index = 0; index < DATA_FILE_SIZE; ++index)
    for(jndex = 0; jndex < DATA_FILE_SIZE; ++jndex)
      INTERSECTION_BUFFER[index][jndex] = 0;

      // Read in trailhead data to the TRAILHEAD_BUFFER.
      // Store the number of loaded data elements.
  int trail_lengths = loadTrailLengths(TRAIL_LEN_BUFFER),
      trailheads = loadTrailheads(TRAILHEAD_BUFFER, TRAIL_LEN_BUFFER, trail_lengths),
      intersections = loadIntersections(INTERSECTION_BUFFER),
      // The number of unique intersections.
      uni_tex_count = 0,
      // The size of the trailhead intersection tuple array.
      texs_size = (intersections % 2 == 0)? intersections/2 : intersections/2 + 1;

  // Array for storing intersection vertices.
  // For intersections of two trails, the max is
  // intersections / 2 (rounded up for % 2 = 1).
  Vtex_Tup texs[texs_size];
  // Fill the Vtex_Tup array.
  uni_tex_count = texTupInit(texs, INTERSECTION_BUFFER, intersections);

  // Allocate the graph. Add 5 for post init test-insertions.
  Graph * graph = new Graph(trailheads + intersections + 5);

  // Insert the trailhead data into the graph.
  for(index = 0; index < trailheads; ++index)
  {
    // Insert the current trailhead.
    graph->newVertex(TRAILHEAD_BUFFER[index]);

    // Reset for traversing the INTERSECTION_BUFFER..
    jndex = 0;
  }

  // Temporary Trailhead for creating intersections.
  Trailhead thead_int;
  // Buffer for creating intersection strings.
  char F_TEX_BUF[10], T_TEX_BUF[10];

  // Insert trail intersections into the graph.
  for(index = 0; index < uni_tex_count; ++index)
  {
    // Store the indices of the intersecting trails in
    // char[]s to intialize the new intersection vertex.
    sprintf(F_TEX_BUF, "%d", texs[index].fst);
    sprintf(T_TEX_BUF, "%d", texs[index].snd);

    // Setup a new intersection vertex.
    thead_int.setupAsIntersection(F_TEX_BUF, T_TEX_BUF);

    // Insert the new intersection vertex
    graph->newVertex(thead_int);
  }

  // Initialize the Graph edges.
  insertEdges( graph, texs, texs_size, TRAILHEAD_BUFFER );

  // Return the new graph.
  return graph;
}



/* ***************************************************************
// Loads trailhead data from the datafile whose name is given by
// TRAIL_INFO_FILENAME. Returns the number of loaded trailheads.
//
// ***************************************************************/
int loadTrailheads( Trailhead TRAILHEAD_BUFFER[DATA_FILE_SIZE],
                    int TRAIL_LENGTH_BUFFER[],
                    int trail_length )
{
  // Open the trailhead data file.
  ifstream thead_file(TRAIL_INFO_FILENAME);

       // Buffer for reading in lines from the trailhead file.
       // Init with % used for loop control to skip datafile comments.
  char LINE_BUFFER[1000] = {'%'},
       // Buffer for separating trailhead names / descriptions.
       NAME_BUFFER[100];

      // Counter for inserting trailheads.
  int thead_count = 0,
      // Index counter for scanning lines.
      ch_index = 0;

	// Index counter for the description.
	unsigned int des_index = 0;

  // Until the file has been fully scanned..
  while(thead_file.good())
  {
    // Reset character index counter.
    ch_index = 0;
    des_index = 0;

    // If this is the first line to be read in,
    // or if the last line was a comment..
    do
    {
      // Read in a line from the trailhead file.
      thead_file.get(LINE_BUFFER, 1000, '\n');
      thead_file.ignore(100, '\n');
    }
    // Continue for all comments.
    while(LINE_BUFFER[0] == '%');

    // Get / Insert trailhead name.
    while(LINE_BUFFER[ch_index] != '~' && ch_index < 100)
    {
      // Add the current character to the name buffer.
      NAME_BUFFER[ch_index] = LINE_BUFFER[ch_index];

      // Increment the character index counter.
      ++ch_index;
    }

    // Cut off any extra text.
    NAME_BUFFER[ch_index] = '\0';

    // Allocate memory for the trailhead name.
    TRAILHEAD_BUFFER[thead_count].location = new char[strlen(NAME_BUFFER) + 1];

    // Insert all the names in the name
    // buffer into the trailhead buffer.
    while(des_index < strlen(NAME_BUFFER) + 1)
    {
      // Insert the current name character.
      TRAILHEAD_BUFFER[thead_count].location[des_index] = NAME_BUFFER[des_index];
      // Increment the destination character index counter.
      ++des_index;
    }

    // Allocate the memory for the remaining
    // buffer text to the trailhead description.
    TRAILHEAD_BUFFER[thead_count].description = new char[strlen(LINE_BUFFER) - strlen(NAME_BUFFER) + 1];

    // Set the trail length of the current trailhead
    // to the current trail length buffer value.
    TRAILHEAD_BUFFER[thead_count].trail_length = TRAIL_LENGTH_BUFFER[thead_count];

    // Increment the buffer counter by two so
    // that the tilda and space aren't copied.
    ch_index += 2;
    // Reset destination character index.
    des_index = 0;

    // Insert trailhead description.
    while(des_index < strlen(LINE_BUFFER) - strlen(NAME_BUFFER) + 1)
    {
      // Add the current character to the name buffer.
      TRAILHEAD_BUFFER[thead_count].description[des_index] = LINE_BUFFER[ch_index];
      // Increment the character index counters.
      ++ch_index; ++des_index;
    }

    // Increment the trailhead counter.
    ++thead_count;
  }

  // Close the data file.
  thead_file.close();

  // Return the number of loaded trailheads.
  return thead_count;
}



/* ***************************************************************
// Loads trail intersection data from the file whose name is given
// by TRAIL_INT_FILENAME. Returns the number of intersections for
// which intersections were provided. * Should simply be
// DATA_FILE_SIZE. *
//
// ***************************************************************/
int loadIntersections(int INTERSECTION_BUFFER[DATA_FILE_SIZE][DATA_FILE_SIZE])
{
  // Open the trailhead data file.
  ifstream intsec_file(TRAIL_INT_FILENAME);

      // Index of the current trail for which
      // intersections are being read in.
  int current_trail = 0,
      // Index of the current intersection
      // being read in for a given trail.
      current_intsec = 0;

	       // Index counter for filling INTSEC_BUFFER.
  unsigned int current_intsec_ch = 0,
	       // Current index of the line being read in.
	       intsec_ch = 0;

       // Buffer for parsing individual intersections.
  char INTSEC_BUFFER[10],
       // Buffer for reading in lines from the file. Init with
       // % used for loop control to skip datafile comments.
       LINE_BUFFER[100] = {'%'};

  // While there is unread intersection data..
  while(intsec_file.good())
  {
    // If this is the first line to be read in,
    // or if the last line was a comment..
    do
    {
      // Read in a line from the trailhead file.
      intsec_file.get(LINE_BUFFER, 100, '\n');
      intsec_file.ignore(100, '\n');
    }
    // Skip header comments.
    while(LINE_BUFFER[0] == '%');

    // Reset buffer index counters.
    current_intsec = 0;
    current_intsec_ch = 0;
    intsec_ch = 0;

    // For readability, the intersection file
    // contains arrows showing the relationship
    // between trails (by number - same order as
    // in the info file) and the number of the
    // trail with which they intersect. Go past
    // the arrow for the current line.
    while(intsec_ch == 0 || LINE_BUFFER[intsec_ch-1] != '>') ++intsec_ch;

    // Read in the intersections.
    while(intsec_ch < strlen(LINE_BUFFER))
    {
      // Reset INTSEC_BUFFER index counter.
      current_intsec_ch = 0;

      // Read in an individual number for the current
      // intersection for the current trail.
      while(LINE_BUFFER[intsec_ch] != ',' && LINE_BUFFER[intsec_ch] != '\0' && intsec_ch < strlen(LINE_BUFFER))
      {
        // Store the current character in the intersection buffer.
        INTSEC_BUFFER[current_intsec_ch] = LINE_BUFFER[intsec_ch];

        // Increment the intersection buffer indices.
        ++current_intsec_ch;
        ++intsec_ch;
      }

      // Terminate the end of the intersection buffer.
      INTSEC_BUFFER[current_intsec_ch] = '\0';
      // Store the integer value of the intersection in INTERSECTION_BUFFER.
      INTERSECTION_BUFFER[current_trail][current_intsec] = atoi(INTSEC_BUFFER);

      // Increment the current intersection index.
      ++current_intsec;
      // Increment the intersection buffer
      // indices (to escape commas).
      ++current_intsec_ch;
      ++intsec_ch;
    }

    // Increment the trail index counter.
    ++current_trail;
  }

  // Close the data file.
  intsec_file.close();

  // Return the number of trails for which
  // intersections were successfully read in.
  return current_trail;
}



/* ***************************************************************
// Loads trail lengths from the file whose name is given by
// TRAIL_LEN_FILENAME. Returns the number of loaded lengths.
//
// ***************************************************************/
int loadTrailLengths(int TRAIL_LENGTH_BUFFER[])
{

  // Length buffer index counter
  int l_index = 0;

  // Buffer for reading in lines from the file. Init with
  // % used for loop control to skip datafile comments.
  char LINE_BUFFER[100] = {'%'};

  // Open the trailhead data file.
  ifstream len_file(TRAIL_LEN_FILENAME);

  // Skip comments, anything else, until #
  // which indicates that the line contains lengths.
  while(len_file.good() && LINE_BUFFER[0] != '#')
    len_file >> LINE_BUFFER;

  // Read the line containing lengths
  // into the TRAIL_LENGTH_BUFFER.
  while(len_file.good())
    len_file >> TRAIL_LENGTH_BUFFER[l_index++];

  // Close the data file.
  len_file.close();

  // Return the number of loaded trail lengths.
  return l_index;
}



/* ***************************************************************
// Initialize an array of tuples with intersecting path indices.
//
// @param texs: The tuple location to be filled with intersecting
// path indices.
//
// @param INTERSECTION_BUFFER: The location where intersections
// have been temporarily stored.
//
// @param intersections: The number of intersections loaded into
// INTERSECTION_BUFFER.
//
// ***************************************************************/
int texTupInit( Vtex_Tup texs[],
                int INTERSECTION_BUFFER[DATA_FILE_SIZE][DATA_FILE_SIZE],
                int intersections )
{

      // Index counters for the rows of the intersections matrix.
  int i = 0,
      // Index counters for the columns of the intersections matrix.
      j = 0,
      // Total unique intersections. Index counter, and return value.
      tex_count = 0;

  // For every trailhead..
  for(; i < intersections-1; ++i)
    // While there are unentered trailhead intersections..
    for(j = 0; j < intersections-1 && INTERSECTION_BUFFER[i][j] != 0; ++j)
      // If the current intersection has not already been added from
      // an earlier trailhead vector containing the same intersection..
      if(i < INTERSECTION_BUFFER[i][j])
        // Insert the unique intersection.
        texs[tex_count++].set(i, INTERSECTION_BUFFER[i][j] - 1);

  // Return the number of unique intersections.
  return tex_count;
}



/* ***************************************************************
// Rounds a floating point number to the nearest whole number.
// Because most of the path lengths are small numbers (using
// kilometers), always truncating floating points results in a
// poor range of edge lengths. i.e. This function is used to
// improve the range of edge lengths.
//
//
// ***************************************************************/
int roundRnum(float rnum)
{ return fmod(rnum, 1) > 0.5? rnum + 1 : rnum; }



/* ***************************************************************
// Returns a random number between 1 and max to be used a an edge
// length between vertices in the path graph.
//
// ***************************************************************/
int randLen(int max)
{
  // Calculate a decimal coefficient
  // to calculate a fraction of max.
  float rand_coef = static_cast<float>((rand() % 100) + 1) / 100;

  // Calculate rounded value of the product
  // of another random number, mod max.
  int return_val = roundRnum(rand_coef * max);

  // If the length is zero, return 1 instead.
  // Otherwise, return the rand_coef.
  return return_val == 0 || return_val % max == 0 ? 1 : return_val % max;
}



/* ***************************************************************
// Returns either a 0 or 1. This is exclusively used by insertEdges
// for initializing adjacent vertices in the graph, and for
// initializing their lengths.
//
//
// ***************************************************************/
bool randBool(void)
{ return rand() % 2; }



/* ***************************************************************
// Initializes the graph with semi-randomized edges. These edges
// are drawn not only between trailhead vertices, but also between
// the vertices created by trail intersections. Essentially, the
// outer-most loop in this function goes through two iterations,
// each of which has its own distinct purpose. The first itertion
// initializes edges between trailheads. Edges involving
// intersections are handled in the second iteration. See the
// in-function comments for more specific information.
//
// I thought about creating a default set of edges to insert every
// time the program loads, but I figured creating an edge generator
// would allow for more comprehensive testing. There's a lot going
// on in this function, and frankly, it's pretty convoluted. There
// is, however, an enormity of comments within the function, which
// should make it relatively easy to follow.
//
// ***************************************************************/
int insertEdges( Graph *& graph,
                 Vtex_Tup texs[],
                 int intersections,
                 Trailhead TRAILHEAD_BUFFER[DATA_FILE_SIZE] )
{

      // The lengths of the trails from and to which the edge is directed.
  int from_len = 0, to_len = 0,
      // The indices of the trailheads/intersections in TRAILHEAD_BUFFER which
      // constitute the vertices of the edge being inserted.
      from_idx = 0, to_idx = 0,
      // Iteration vounter for initializing edges (inner-most loop).
      edge_iter = 0,
      // Index counter for adding edges between vertices.
      isec_index = 0,
      // Modifier for initializing with intersections. Essentially, index_mod is set
      // to the number of non-intersection vertices after all non-intersection vertices
      // have been traversed. Then, when traversing intersection vertices, index_mod
      // is added (sometimes, using randBool()) to offset the index (which is based
      // off of isec_index ( i.e. starts at 0)) in the adjacency list of the graph. 
      index_mod = 0,
      // As described above in the function header comment, this function does two major
      // rounds of edge initialization, each with their own iteration. There is a small
      // amount of extra stuff to do in preparation for the second iteration, and small
      // amount of extra stuff to do in the second iteration, and this variable is used
      // to keep track of which iteration the outer-most loop is currently in.
      iteration = 0,
      // The number of iterations to loop in the first inner-loop,
      // depending on the iteration of the outer-loop.
      current_max = intersections;

  // Initialize srand.
  srand(time(0));

  // Initialize Graph edges.
  do
  {
    // For every element in (the length of trailheads for iteration 1)
    //                      (the number of intersections for iteration 2).
    for(isec_index = 0; isec_index < current_max; ++isec_index)
    {
      // Store the length of the first trailhead in the intersection given by texs' fst
      // value at the index of the current iteration (mod intersections for overflow).
      from_len = TRAILHEAD_BUFFER[texs[isec_index % intersections].fst].trail_length;
      // Store the length of the second trailhead in the intersection given by texs' snd
      // value at the index of the current iteration (mod intersections for overflow).
      to_len = TRAILHEAD_BUFFER[texs[isec_index % intersections].snd].trail_length;

      // Reset the random edge length index counter.
      edge_iter = 0;

      // Do three passes of edge insertions. For each pass, use the next random edge
      // length. Also, depending on the iteration (OF THIS LOOP AND THE OUTER-LOOP),
      // switch up the ordering of fields (and there initializations). This is not 
      // necessary, but it maximizes the randomness of the edge links, which consequently
      // increases the likely number of intersection & non-intersection edge combinations.
      while(edge_iter < 3)
      {
        // If this loop is not in its last iteration..
        // set from_idx to the current index of the outer loop. Otherwise, set it
        // to the index of the the first trailhead in the intersection given by the
        // fst value in the intersection tuple array at the index of the current
        // iteration of the outer-most loop (mod intersections for overflow).
        from_idx = edge_iter < 2? isec_index : texs[isec_index % intersections].fst;
        // set from_idx to the current index of the outer loop. Otherwise, set it
        // to the index of the the first trailhead in the intersection given by the
        // fst value in the intersection tuple array at the index of the current
        // iteration of the outer-most loop (mod intersections for overflow).
        to_idx = edge_iter % 2 == 0? texs[isec_index % intersections].snd : texs[isec_index % intersections].fst;

        // If the outer-most loop is in its 2nd iteration ('iteration' starts at
        // 0), swap from_idx and to_idx, and, depending on the return values from
        // randBool(), offset the index by index_mod. This is the operation which
        // increases the number of intersection & non-intersection edge combinations.
        if(iteration == 1)
        {
          // Add to_idx to from_idx, plus index_mod if randBool().
          from_idx += randBool()? to_idx + index_mod : to_idx;
          // Set to_idx to difference between the new from_idx and the current to_idx.
          // to_idx now = the previous value of from_idx, or that value plus the offset.
          to_idx = from_idx - to_idx;
          // Set from_idx to to_idx less than its current value. If randBool(), then
          // subtract index_mod from to_idx before subtracting from from_idx.
          from_idx -= randBool()? to_idx - index_mod: to_idx;
        }

        // If the indices are different (we don't want vertices with edges to themselves)..
        if(from_idx != to_idx)
        {
          // If from_idx is greater than the number of trailheads (it's the index for an
          // intersection), get the index of the first trailhead in the intersection from
          // Graph's isecFrom method. Otherwise, just use from_idx. Once the correct index
          // is selected, store the length of the trail in TRAILHEAD_BUFFER at that index.
          from_len = TRAILHEAD_BUFFER[ graph->isecFrom(from_idx) < 0 ?
                   from_idx : graph->isecFrom(from_idx) ].trail_length;

          // If to_idx is greater than the number of trailheads (it's the index for an
          // intersection), get the index of the first trailhead in the intersection from
          // Graph's isecFrom method. Otherwise, just use to_idx. Once the correct index
          // is selected, store the length of the trail in TRAILHEAD_BUFFER at that index.
          to_len = TRAILHEAD_BUFFER[ graph->isecFrom(to_idx) < 0 ?
                   to_idx : graph->isecFrom(to_idx) ].trail_length;

          // Depending on the return values of randBool(), join the vertex
          // given by from_idx to the vertex given by to_idx, and or the
          // vertex given by to_idx to the vertex given by from_idx.

          // Join the vertex given by from_idx to the vertex given by to_idx,
          // with a length equal to some fraction of the length of from_len.
          if(randBool()) graph->joinVertices( from_idx, to_idx, randLen(from_len));
          // Join the vertex given by to_idx to the vertex given by from_idx,
          // with a length equal to some fraction of the length of to_len.
          if(randBool()) graph->joinVertices( to_idx, from_idx, randLen(to_len));
        }

        // Increment the edge insertion iteration counter.
        ++edge_iter;
      }
    }

    // If this is the end of the first iteration (outer-most loop)..
    if(iteration == 0)
    {
      // Init index_mod for the 2nd iteration.
      index_mod = DATA_FILE_SIZE;
      // Init current_max to the number of intersection vertices.
      current_max = ( graph->vertexCount() - index_mod ) / 2;
    }

    // Increment the iteration counter.
    ++iteration;
  }
  // Perform loop for two iterations.
  while(iteration < 2);

  // Report success.
  return 0;
}
