/*****************************************************************
// File Name:  Program4_NToth.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       June 2nd, 2017
// Class:      CS163
// Program #:  4
//
// Overview: This is the main / testing file for the fourth
// programming assignment. The focus of the fourth assignment is to
// create a tree for managing a large set of events (which was
// contributed to by the whole class). The functions in this
// file are used to test out the various features of the tree.
// See Event.cpp/h and Table.cpp/h for more informaton.
//
// ***************************************************************/


#include <fstream>
#include "Table.h" // Includes Event.h, iostream, cstring.

using namespace std;

// The name of the data file to be scanned on
// launch if no alternative file name is given.
const char DEFAULT_DATA_FILE[] = "Test_Data.txt";

// Max number of test events.
const unsigned short EVENT_BUFFER_SIZE = 2048;


// *****************  |
// Menu & Handlers *  |
// *****************  V

// Print out the main program menu,read in & return selection.
void menuController(short & selection);

// Prompt user to press enter to continue.
void continueMenu(void);

// Read in and insert a new event from the user.
void handleInsert(Table & table);
// Menu handler for retrieving events by name.
void handleRetrieve(const Table & table);
// Menu handler for removing events with a particular keyword.
void handleRemoveKey(Table & table);
// Menu handler for removing events by name.
void handleRemoveName(Table & table);
// Menu handler for displaying all events.
void handleDisplayAll(const Table & table);


// ******************* |
// Testing Functions * |
// ******************* V

// Allocates memory for and initializes filename, based
// on run time argument / reading in, or default.
void setFilename( char *& filename,
                  int argc,
                  char ** argv );

// Display error messages for input operations.
void entryError( const char *item,
                 short error_code,
                 short max = 0 );

// Prompt the user to enter Y or N. Return true for Y.
bool confirmChoice(void);

// Reads in the test data to be inserted in the table.
int readData( Event EVENT_BUFFER[],
              char *& filename );

// Verify that the data file exists.
void verifyFile( char *& filename,
                 ifstream & event_file );

// Parse the event data for a given line of the data file.
void parseEvent( Event & event_data,
                 const char LINE_BUFFER[] );

// Read in various types of data for
// searching / creating new events.
void readValue( char *& data_loc,
                const char * data_type,
                int max_len );

// strcat for two const char* s.
char * copycat( const char * str_one,
                const char * str_two);

// Set keywords to lower space, remove end spaces.
void formatKeyword(char *& keyword);

// Read in the number of events to retrieve.
int numEvents(void);

// Initialize the table with the contents of EVENT_BUFFER
void initTable( Table & table,
                char * filename,
                bool & quit );

// Get the number of keywords to be used for the new event.
int numKeywords(void);



/* ********************************************
// Main optionally takes a filename as an
// argument. If no entry is provided, the user
// will be prompted to enter a filename. If no
// filename is given, the program will use the
// default data file, Test_Data.txt.
//
// ********************************************/
int main( int argc,
          char ** argv )
{
  // Loop control variable set to true when
  // the user selects quit from the menu.
  bool quit = false;

  // Stores the user's menu selection.
  short selection = 0;

  // Contains the name of the data file to be used.
  char * filename = NULL;

  // (Get?), Store the name of the data file to be used.
  setFilename(filename, argc, argv);

  // Create table of events.
  Table table;

  // Initialize the table.
  initTable(table, filename, quit);

  // Run the program until quit is selected from the menu.
  while(!quit)
  {
    // Print the program's main menu,
    // read in the user's selection.
    menuController(selection);

    // Execute the handler function
    // for the user's selection.
    switch(selection)
    {
      case 1:
        handleInsert(table);
        break;
      case 2:
        handleRetrieve(table);
        break;
      case 3:
        handleRemoveName(table);
        break;
      case 4:
        handleRemoveKey(table);
        break;
      case 5:
        handleDisplayAll(table);
        break;
      default: // 6 - Quit the program.
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

  // Deallocate filename memory.
  delete[] filename; filename = NULL;

  // Display out program footer message.
  cout << "\n ------------------------ Thank You! ------------------------ " << endl << endl;

  return 0;
}



/* ********************************************
// Initialize the table with the events in the
// external data file.
//
// ********************************************/
void initTable( Table & table,
                char * filename,
                bool & quit )
{

  // Contains the events loaded from
  // the external data file, filename.
  Event EVENT_BUFFER[EVENT_BUFFER_SIZE];

      // Load events into the event buffer.
  int event_count = readData(EVENT_BUFFER, filename),
      // General index counter.
      index = 0;

  // Read in the data from filename. If the file is
  // empty (readData returns line count), quit.
  if(event_count < 1)
  {
    quit = true;
    cout << "\n\tError :: " << filename << " is empty! Exiting..." << endl;
  }
  else
  {
    // Display out program header message.
    cout << "\n ----------------- Event Management Program ----------------- \n" << endl;

    // Display alert that table is being filled.
    cout << "\n  Filling event table with contents of "
          << filename << '.' << endl << endl;

    // Fill table with events in EVENT_BUFFER.
    for(; index < event_count; ++index)
      table.insert(EVENT_BUFFER[index]);

    // Display an alert that the table has been filled.
    cout << "  The table has been initialized!" << endl;
  }

	return;
}



/* ********************************************
// Prints the program testing menu, reads in
// a selection.
//
// @param selection: The location where the
// menu selection is stored.
//
// ********************************************/
void menuController(short & selection)
{
  // The number of selections in the menu.
  short max_selections = 6;

  // Print out the program menu.
  cout << "\n  Make a selection from the menu." << endl << endl << "    "
       << "[1] => Insert a new event." << endl << "    "
       << "[2] => Retrieve an event by name." << endl << "    "
       << "[3] => Remove an event by name." << endl << "    "
       << "[4] => Remove all events with a keyword." << endl << "    "
       << "[5] => Display all events alphabetically." << endl << "    "
       << "[6] => Exit the program" << endl;

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
      entryError("Selection", 1, max_selections);
  }
}



/* ********************************************
// Creates a new event object, and inserts it
// into the table.
//
// ********************************************/
void handleInsert(Table & table)
{
  // Prompt the user to confirm before creating and inserting new event.
  cout <<  "\n\tAre you sure you want to create and insert a new event?"
       << endl << endl << "\t";

  // If the user chooses to cancel..
  if(!confirmChoice())
  {
    // Display alert that insert is being canceled.
    cout << "\n\tCanceling insert.." << endl;
    // Return to the calling function.
    return;
  }

  // New event to be created.
  Event new_event;

      // Index counter for inserting event data.
  int member = 0,
      // Number of keywords to be entered.
      num_keywords = 0,
      // Return value from table's insert function.
      insert_ret = 0,
      // Max size for a given entry.
      entry_len = 0;

  // Number of (non-keywords) event members.
  const static int num_labels = 5;
  // Labels for all (non-keywords) event members.
  const char * EVENT_LABELS[] = {"name", "location", "date", "start time", "description"};

  // Array of pointers to the data pointers (except
  // keywords) in the new event being filled.
  char ** data_elements[] = { & new_event.name,
                              & new_event.location,
                              & new_event.date,
                              & new_event.start_time,
                              & new_event.description };

  // Fill the event's (non-keywords) event members.
  for(; member < num_labels; ++member)
  {
    // Create a label for the entry.
    const char * entry_label = copycat("new event ", EVENT_LABELS[member]);
    // If the current data element is a description,
    // max length is 512. Otherwise, it's 64.
    entry_len = ( strcmp(EVENT_LABELS[member],"description") )? 64 : 512;

    // Read in current iteration's data to the new event.
    readValue( * data_elements[member], entry_label, entry_len);

    // Deallocate the entry label.
    delete[] entry_label; entry_label = NULL;
  }

  // Get the number of keywords to be entered.
  num_keywords = numKeywords();

  // For every keyword to be entered..
  for(member = 0; member < num_keywords; ++member)
  {
    // Get the char for the current keyword number.
    const char key_num[] = { static_cast<char>('0' + (member + 1)), '\0'};

    // Create a label for the current keyword entry prompt.
    const char * key_label = copycat("keyword #", key_num);

    // Read in the keyword entry to the new event.
    readValue(new_event.keywords[member], key_label, 32);

    // Deallocate the key label.
    delete[] key_label; key_label = NULL;
  }

  // Insert the new event.
  insert_ret = table.insert(new_event);

  // If the insertion failed.. (Which it wont. This is for potential debugging).
  if(insert_ret == -1)
    // Display alert that the insertion failed.
    cout << "\n\tFailed to insert new event.. Returning to menu!" << endl;
  else
    // Display success message.
    cout << "\n\tSuccessfully inserted new event: " << new_event.name << endl;

  return;
}



/* ********************************************
// Concatenates two const char *s. Returns a
// pointer to a new, dynamically allocated
// char *.
//
// ********************************************/
char * copycat(const char * str_one, const char * str_two)
{
  // If either string is NULL, return NULL.
  if(!str_one || !str_two) return NULL;

  // Get the sum of the lengths of both entries.
  int cat_len = strlen(str_one) + strlen(str_two);
  // Allocate a new string for the concatenation.
  char * cat_str = new char[cat_len + 1];

  // Copy the first string into the new string.
  strcpy(cat_str, str_one);
  // Concat the copied string and the second string.
  strcat(cat_str, str_two);

  // Return the new, concatenated string.
  return cat_str;
}



/* ********************************************
// Reads in the number of keywords to be added
// to the new event.
//
// ********************************************/
int numKeywords(void)
{
  // Entry for the number of keywords to be entered.
  int entry = 0;

  do
  {
    // Prompt for keywords number entry.
    cout << "\n\tPlease enter the number of keywords "
         << "that you would like to add to the new event"
         << "\n\tNumber of keywords (between 1 and 5): ";

    // Read in number.
    cin >> entry;
    cin.ignore(100, '\n');

    // If the entry isn't valid..
    if(entry < 1 || entry > 5)
      // Display alert that another entry must be made.
      cout << "\n\tInvalid entry! Please enter a number between 1 and 5!" << endl;
  }
  // Keep trying until entry is valid.
  while(entry < 1 || entry > 5);
  return entry;
}



/* ********************************************
// Executes when retrieve [1] is selected from
// the main menu. Reads in a keyword and fills
// an array of events with that keyword. Like
// all handlers, this function only takes the
// table from main as its only parameter.
//
// ********************************************/
void handleRetrieve(const Table & table)
{
  // Return value from Table's retrieve method.
  int retrieve_ret = 0;

  // Pointer for storing the name
  // of the event to be retrieved.
  char * event_name = NULL;

  // The location to store the retrieved event.
  Event retrieved_event;

  // Get the name of the event to be retrieved.
  readValue(event_name, "event name", 64);

  // Display message that event will be retrieved.
  cout << "\n\tRetrieving the event: " << event_name << '.'
       << endl << endl;

  // Try to retrieve the event.
  retrieve_ret = table.retrieve(event_name, retrieved_event);

  // If retrieve was unsuccessful..
  if(retrieve_ret == 0)
    // Display an alert that the event wasn't found.
    cout << "\n  There is no event named " << event_name << '.' << endl;
  else
  {
    // Display success message.
    cout << "\n  Successfully retrieved: " << event_name << '.' << endl
         << "\n  Would you like to display the retrieved event?" << endl;

    // If the user wants to display
    // the retrieved event..
    if(confirmChoice())
    {
      // Display the retrieved event.
      cout << "\n  Displaying event.." << endl << endl;
      retrieved_event.display();
    }
    else
      cout << "\n  Returning to the menu.." << endl << endl;
  }

  // Delete and nullify the memory allocated with event_name.
  delete[] event_name;
  event_name = NULL;

  return;
}



/* ********************************************
// Menu handler for removing events from the
// table by keyword.
//
// ********************************************/
void handleRemoveKey(Table & table)
{
  // Return value from Table's remove by key method.
  int remove_key_ret = 0;

  // Pointer for a date entry.
  char * keyword = NULL;

  // Get a keyword to search.
  readValue(keyword, "keyword", 64);

  // Display message that events will be removed.
  cout << "\n\tDeleting events with the keyword: " << keyword << '.'
       << endl << endl;

  // Delete keyword from the table.
  remove_key_ret = table.removeEventKey(keyword);

  // If remove by keyword returned 0..
  if(remove_key_ret == 0)
    // Display an alert that there are no
    // events older than the specified date.
    cout << "\nThere are no events with the keyword: " << keyword;
  else
    cout << "\nSuccessfully removed " << remove_key_ret
         << " events with the keyword: " << keyword;

  // Display punctuation & whitespace.
  cout << '!' << endl << endl;

  // Delete and nullify the memory allocated with keyword.
  delete[] keyword;
  keyword = NULL;

  return;
}



/* ********************************************
// Menu handler for removing an event from the
// table by name.
//
// ********************************************/
void handleRemoveName(Table & table)
{
  // Return value from Table's remove by name method.
  int remove_ret = 0;

  // Pointer for storing the name
  // of the event to be deleted.
  char * event_name = NULL;

  // Get the name of the event to be removed.
  readValue(event_name, "event name", 64);

  // Display message that event will be deleted.
  cout << "\n\tDeleting the event: " << event_name << '.'
       << endl << endl;

  // Try to delete the event.
  remove_ret = table.removeEventName(event_name);

  // If remove was unsuccessful..
  if(remove_ret == 0)
    // Display an alert that the event wasn't found.
    cout << "\nThere is no event named " << event_name;
  else
    // Display success message.
    cout << "\n\tSuccessfully removed: " << event_name << endl
         << "\tremoveEventName returned: " << remove_ret;

  // Display punctuation & whitespace.
  cout << '!' << endl << endl;

  // Delete and nullify the memory allocated with event_name. 
  delete[] event_name;
  event_name = NULL;

  return;
}



/* ********************************************
// Menu handler for displaying all events in
// the table. Like all handlers, this function
// only takes the table from main as its only
// parameter.
//
// ********************************************/
void handleDisplayAll(const Table & table)
{
  // Return value from Table's display method.
  int disp_table_ret = 0;

  // Display alert that events will be printed.
  cout << "\n\tDisplaying all unique events in the table!" << endl;

  // Display the table.
  disp_table_ret = table.display();

  // If display returned 0..
  if(disp_table_ret == 0)
    // Display an alert that no events exist.
    cout << "\tThere are no events to display!" << endl << endl;

  // Display the number of displayed events.
  cout << "\n\tEvents Displayed: " << disp_table_ret << endl << endl;

  return;
}



/* ********************************************
// Prompt's user to press enter to continue
// so they can read output before going back
// to the main menu.
//
// ********************************************/
void continueMenu(void)
{
  // Prompt user to press enter.
  cout << "\n  Press enter to continue...";
  // Wait until user continues.
  cin.ignore(100,'\n');
  return;
}



/* ********************************************
// Displays an error message regarding an
// invalid entry.
//
// @param item: The name of the invalid entry
// location. e.g. Invalid seat number of seats
// => item = "seats".
//
// @param error_code: The specific error to
// report. Error Codes:
//
//      1 => Invalid Entry - Not in range 1, max.
//      2 => Invalid Entry - Not 'Y' or 'N'.
//      ∞ => Undefined.
//
// @param max: The upper limit for a range
// error.
//
// ********************************************/
void entryError( const char * item,
                 short error_code,
                 short max )
{
  // Print error alert.
  cout << "  Error :: ";

  // Print error message for the given error code.
  // See function comment for error_code.
  switch(error_code)
  {
    case 1: // Invalid menu selection.
      cout << "Invalid Entry => " << item
           << " must be a number from 1 to " << max;
      break;

    case 2: // Invalid confirmation entry.
      cout << "Invalid Entry => Please enter Y to confirm or N to retry..";
      break;

    default: // Some other undefined entry error.
      cout << "Undefined..";
      break;
  }

  // Glorious whitespace.
  cout << endl;

  return;
}



/* ********************************************
// Displays prompt to enter confirmation, reads
// in and returns an entry.
//
// @return: If 'Y' is entered, true is returned.
// If the entry is invalid, an error message is
// printed and this function is returned (trys
// again).
//
// ********************************************/
bool confirmChoice(void)
{
  // Confirmation to initialize/redo passenger.
  char confirmation = '\0'; // N for redo. Y for keep.

  // Prompt user to enter confirmation.
  cout << "  Please confirm (Y/N): ";

  // Read in confirmation.
  cin >> confirmation;
  cin.ignore(100,'\n');
  cout << endl;

  // Convert entry to uppercase.
  confirmation = toupper(confirmation);

  // If the user has selected Y, return true.
  if(confirmation == 'Y') return true;
  // If the user has selected N, return false.
  if(confirmation == 'N') return false;

  // If the user's entry is invalid, print invalid
  // entry error message #2.
  entryError(NULL, 2, 0);

  // Try again.
  return confirmChoice();
}



/* ********************************************
// Used to determine whether or not a filename
// has been given to main, to read in a filename,
// or to use the defailt filename.
//
// ********************************************/
void setFilename( char *& filename,
                  int argc,
                  char ** argv )
{
  // If no filename was given to main..
  if(argc < 2)
  {
    // Display prompt asking whether to use the default filename.
    cout << "  Would you like to enter a file to load?" << endl
         << "  If not, " << DEFAULT_DATA_FILE << " will be used." << endl;

    // If Y is entered, read in a filename to filename.
    if(confirmChoice()) readValue(filename, "data file name", 64);
    // If no is entered, set the filename to DEFAULT_DATA_FILE.
    else
    {
      // Allocate memory for the filename.
      filename = new char[strlen(DEFAULT_DATA_FILE) + 1];
      // Copy the contents of the filename.
      strcpy(filename, DEFAULT_DATA_FILE);
    }
  }
  // Otherwise, if main did get a filename argument..
  else
  {
    // Allocate memory for the filename.
    filename = new char[strlen(argv[1]) + 1];
    // Copy the contents of the filename.
    strcpy(filename, argv[1]);
  }

  return;
}



/* ********************************************
// Reads in date from the event file (given by
// setFilename & readValue). Fills the 
// EVENT_BUFFER with all of the events (up to
// EVENT_BUFFER_SIZE) in the file.
//
// ********************************************/
int readData( Event EVENT_BUFFER[],
              char *& filename )
{
  // Open the file named filename.
  ifstream event_file(filename);

  // Verify that the file contains data.
  verifyFile(filename, event_file);

  // Max length of a line from the event data file.
  const unsigned short line_buf_size = 1023;
  // Total event counter.
  unsigned short event_count = 0;

  // Buffer for reading in lines from the event data file.
  char LINE_BUFFER[line_buf_size + 1];

  // While the event file has unstreamed lines..
  while(event_file.good())
  {
    // Read in a line from the event data file.
    event_file.get(LINE_BUFFER, line_buf_size, '\n');
    event_file.ignore(line_buf_size, '\n');

    // Parse the event line, and copy its
    // contents into the current event.
    parseEvent(EVENT_BUFFER[event_count], LINE_BUFFER);

    // Increment the event counter.
    ++event_count;
  }

  // Close the event data file.
  event_file.close();

  // Return the number of loaded events. 
  return event_count;
}



/* ********************************************
// Handles invalid filename entries.
//
// ********************************************/
void verifyFile( char *& filename,
                 ifstream & event_file)
{
  // While the current file doesn't contain data..
  while(!event_file.good())
  {
    // Display alert that the specified file does not exist.
    cout << "\n  File with name: " << filename << " does not exist." << endl
         << "  You will need to enter a correct filename.";

    // Delete & nullify the specified filename.
    delete[] filename; filename = NULL;

    // Read in a new filename.
    readValue(filename, "data file name", 64);

    // Close the previous event file.
    event_file.close();

    // Open the new event file.
    event_file.open(filename);
  }

  return;
}



/* ********************************************
// Parses the contents of a given event from
// in the data file, and initializes the fields
// of event_data with its contents.
//
// ********************************************/
void parseEvent( Event & event_data,
                 const char LINE_BUFFER[] )
{
        // Counter for determining what the
        // current element of event_data is.
  short current_data_num = 0,
        // The start index for the next event
        // datum in LINE_BUFFER to be loaded.
        start_index = 0,
        // Index counter for adding characters from
        // the line buffer to the data buffer.
        current_buf_index = 0,
        // Max size for a specific data field.
        data_size = 1023,
        // General index counter.
        g_index = 0;

  // The data for the current event element to be set.
  char current_data_buf[data_size + 1];

	event_data.isNULL = false;

  // Array of pointers to the data pointers (except
  // keywords) in the current event being filled.
  char ** data_elements[] = { & event_data.name,
                              & event_data.location,
                              & event_data.date,
                              & event_data.start_time,
                              & event_data.description };

  // Parse all data, except keywords.
  while(current_data_num < 5)
  {
    // Reset the data buffer index counter.
    current_buf_index = 0;

    // Until a pipe is encountered..
    while(LINE_BUFFER[start_index] != '|' && current_buf_index < data_size)
    {
      // Copy the current char into the current data 
      current_data_buf[current_buf_index] = LINE_BUFFER[start_index];

      // Increment index counters.
      ++start_index;
      ++current_buf_index;
    }

    // Store terminating char in data buffer.
    current_data_buf[current_buf_index] = '\0';

    // Allocate and initialize the current data.
    (* data_elements[current_data_num]) = new char[strlen(current_data_buf) + 1];
    strcpy((* data_elements[current_data_num]), current_data_buf);

    // Increment the data element counter.
    ++current_data_num;
    // Increment the start index (to skip the pipe).
    ++start_index;
  }

  // Reset data_elements to be an array of pointers to
  // keyword pointers for the current event being filled.
  for(; g_index < 5; ++g_index) data_elements[g_index] = & event_data.keywords[g_index];

  // Parse keywords.
  current_data_num = 0;

  // While there are keywords to be added..
  while(current_data_num < 5 && LINE_BUFFER[start_index - 1] != '\0')
  {
    // Reset the data buffer index counter.
    current_buf_index = 0;

    // Until a comma or terminating char is incountered..
    while(LINE_BUFFER[start_index] != ',' && LINE_BUFFER[start_index] != '\0' && current_buf_index < data_size)
    {
      // Copy the current char into the current data.
      current_data_buf[current_buf_index] = LINE_BUFFER[start_index];

      // Increment index counters.
      ++start_index;
      current_buf_index++;
    }

    // Store terminating char in data buffer.
    current_data_buf[current_buf_index] = '\0';

    // Allocate and initialize the current keyword.
    (*data_elements[current_data_num]) = new char[strlen(current_data_buf) + 1];
    strcpy((*data_elements[current_data_num]), current_data_buf);

    // Format the event keyword for removal.
    formatKeyword((*data_elements[current_data_num]));

    // Increment the data element counter.
    ++current_data_num;
    // Increment the start index (to skip the pipe).
    ++start_index;
  }

  return;
}



/* ********************************************
// Reads in various types of data for creating
// and searching events.
//
// ********************************************/
void readValue( char *& data_loc,
                const char * data_type,
                int max_len )
{
  // If data_loc is not NULL, exit.
  if(data_loc) return;

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
  cout << "  Using " << data_type << ": " << data_loc << endl << endl
       << "  Is this correct?" << endl;

  // Prompt for confirmation. If confirmed,
  // return to calling function.
  if(confirmChoice()) return;

  // Display alert that the function will execute again.
  cout << "  Trying again!" << endl;

  // Otherwise..
  // Delete & nullify data_loc.
  delete[] data_loc;
  data_loc = NULL;

  // Keep trying.
  return readValue(data_loc, data_type, max_len);
}



/* ********************************************
// Reads in the number of events to copy.
//
// ********************************************/
int numEvents(void)
{
  // The user's entry.
  int entry = 0;

  // While the entry is invalid..
  while(entry < 1)
  {
    // Prompt to enter number of events to retrieve.
    cout << "\n\tPlease enter the number of events to retrieve: ";

    // Read in the entry.
    cin >> entry;
    cin.ignore(100, '\n');

    // If the entry is invalid..
    if(entry < 1)
      // Display an alert that the entry was invalid.
      cout << "\n\tInvalid entry. Please enter a positive natural number.."
           << endl << endl;
  }

  // Return the entry.
  return entry;
}



/* ********************************************
// Formats keyword entries from the console and
// from the data file. Lower case, no trailing
// spaces, etc..
//
// ********************************************/
void formatKeyword(char *& keyword)
{
      // Index counter for traversing data.
  int k_index = 0,
      // Length of the keyword.
      s_length = 0;

  // While the first character in keywords is a space..
  while(keyword[0] == ' ')
  {
    // Get the length of the keyword.
    s_length = strlen(keyword);
    // If the length is greater than 1..
    if(s_length > 1)
      // Sever the space from the beginning of the keyword.
      memmove(keyword, keyword + 1, s_length);
  }

  // While the keyword index is less than the 
  while(k_index < s_length)
  {
    // Get 1 less than the length of the keyword.
    s_length = strlen(keyword) - 1;

    // While the end of the keyword is a space..
    while(keyword[s_length] == ' ')
    {
      // Move null terminating character to the space.
      keyword[s_length] = '\0';
      // Decrement the string length.
      --s_length;
    }

    // If the keyword index is less than the current length..
    if(k_index < s_length)
      // Set the current character to lower case.
      keyword[k_index] = tolower(keyword[k_index]);
    // Increment the keyword index counter.
    ++k_index;
  }
}
