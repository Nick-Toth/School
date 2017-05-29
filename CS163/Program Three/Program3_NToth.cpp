/*****************************************************************
// File Name:  Program3_NToth.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 24nd, 2017
// Class:      CS163
// Program #:  3
//
// Overview: This is the main / testing file for the third
// programming assignment. The focus of the third assignment is to
// create a hash table for managing a large set of events (which
// was contributed to by the whole class). The functions in this
// file are used to test out the various features of the hash table.
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

// Menu handler for retrieving events with a keyword.
void handleRetrieve(const Table & table);
// Menu handler for removing old events.
void handleRemove(Table & table);
// Menu handler for displaying events with a keyword.
void handleDisplayKey(const Table & table);
// Menu handler for displaying all events.
void handleDisplayAll(const Table & table);

// ******************* |
// Testing Functions * |
// ******************* V

// Reads in an alternate filename from the console.
void readInFilename(char *& filename);

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

// Read in a keyword to be searched.
void readKeyword(char *& keyword);

// Read in a date to be searched.
void readDate(char *& date);

// Set keywords to lower space, remove end spaces.
void formatKeyword(char *& keyword);

// Convert date from yyyy-mm-dd to dd/mm/yyyy.
void formatDate(char date[]);

// Read in the number of events to retrieve.
int numEvents(void);



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
  // Display out program header message.
  cout << "\n ----------------- Event Management Program ----------------- \n" << endl;

  // Loop control variable set to true when
  // the user selects quit from the menu.
  bool quit = false;

        // Stores the user's menu selection.
  short selection = 0,
        // General index counter.
        index = 0;

  // Number of events.
  int event_count = 0;

         // Contains the name of the data file to be used.
  char * filename = NULL;

  // (Get?), Store the name of the data file to be used.
  setFilename(filename, argc, argv);

  // Contains the events loaded from
  // the external data file, filename.
  Event EVENT_BUFFER[EVENT_BUFFER_SIZE];

  // Load events into the event buffer.
  event_count = readData(EVENT_BUFFER, filename);

  // Create table of events.
  Table table;

  // Read in the data from filename. If the file is
  // empty (readData returns line count), quit.
  if(event_count < 1)
  {
    quit = true;
    cout << "\n\tError :: " << filename << " is empty! Exiting..." << endl;
  }
  else
  {
    // Display alert that table is being filled.
    cout << "\n  Filling event table with contents of "
         << filename << '.' << endl << endl;

    // Fill table with events in EVENT_BUFFER.
    for(; index < event_count; ++index)
      table.insert(EVENT_BUFFER[index]);

    // Display an alert that the table has been filled.
    cout << "  The table has been initialized!" << endl;
  }

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
      case 1: // Retrieve events with a keyword.
        handleRetrieve(table);
        break;
      case 2: // Remove old events.
        handleRemove(table);
        break;
      case 3: // Display events with keyword.
        handleDisplayKey(table);
        break;
      case 4: // Display all events.
        handleDisplayAll(table);
        break;
      default: // 5 - Quit the program.
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

  // Deallocate dynamic char[]s. CHECK NULL FIRST!!!

  // Display out program footer message.
  cout << "\n ------------------------ Thank You! ------------------------ " << endl << endl;

  return 0;
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
  short max_selections = 5;

  // Print out the program menu.
  cout << "\n  Make a selection from the menu." << endl << endl << "    "
       << "[1] => Retrieve events with a keyword." << endl << "    "
       << "[2] => Remove old events." << endl << "    "
       << "[3] => Display all events with a keyword." << endl << "    "
       << "[4] => Display all events." << endl << "    "
       << "[5] => Exit the program" << endl;

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
// Executes when retrieve [1] is selected from
// the main menu. Reads in a keyword and fills
// an array of events with that keyword. Like
// all handlers, this function only takes the
// table from main as its only parameter.
//
// ********************************************/
void handleRetrieve(const Table & table)
{

      // The size of the array of events to be filled.
  int event_max = numEvents(),
      // event_max, updated by the retrieve function.
      event_matches = event_max,
      // Return value from the table's retrieve function.
      retrieve_ret = 0,
      // Index counter for displaying matches.
      index = 0;

  // Array of events to be filled.
  Event returned_events[event_max];

  // Pointer for a keyword entry.
  char * keyword = NULL;

  // Get a keyword to search.
  readKeyword(keyword);

  // Display message that events will be retrieved.
  cout << "\n\tRetrieving up to " << event_max
       << " events with the keyword: " << keyword
       << endl << endl << '\t';

  // Retrieve events matching the specified keyword.
  retrieve_ret = table.retrieve(keyword, returned_events, event_matches);

  // If no events were found..
  if(retrieve_ret == 0)
    // Display an alert that no events exist with the specified keyword.
    cout << "There are no events with the keyword: " << keyword << endl;
  else
  {
    // Display the number of matches, and an alert that the events will be printed.
    cout << "A total of " << event_matches << '/' << event_max
         << " events contained the keyword: " << keyword << endl
         << "\n\t Displaying matched events" << endl;

    // Display each matched event.
    for(; index < event_matches; ++index) returned_events[index].display();
  }

  // Delete and nullify the memory allocated with keyword. 
  delete[] keyword;
  keyword = NULL;

  return;
}



/* ********************************************
// Menu handler for retrieving events with a
// keyword. Like all handlers, this function
// only takes the table from main as its only
// parameter.
//
// ********************************************/
void handleRemove(Table & table)
{
  // Return value from Table's display method.
  int remove_old_ret = 0;

  // Pointer for a date entry.
  char * date = NULL;
  // Get a date to search.
  readDate(date);

  // Display message that events will be printed.
  cout << "\n\tDeleting old events from before: " << date << '.'
       << endl << endl;

  // Display the table.
  remove_old_ret = table.removeOld(date);

  // If display returned 0..
  if(remove_old_ret == 0)
    // Display an alert that there are no
    // events older than the specified date.
    cout << "\nThere are no events from before " << date << '.' << endl;
  else
    cout << "\nSuccessfully removed " << remove_old_ret
         << " events from before " << date << '.' << endl << endl;

  // Delete and nullify the memory allocated with date. 
  delete[] date;
  date = NULL;

  return;
}



/* ********************************************
// Menu handler for displaying events with
// a keyword. Like all handlers, this function
// only takes the table from main as its only
// parameter.
//
// ********************************************/
void handleDisplayKey(const Table & table)
{
  // Return value from Table's display method.
  int disp_table_ret = 0;

  // Pointer for a keyword entry.
  char * keyword = NULL;
  // Get a keyword to search.
  readKeyword(keyword);

  // Display message that events will be printed.
  cout << "\n\tDisplaying all events.." << endl << endl;

  // Display the table.
  disp_table_ret = table.display(keyword);

  // If display returned 0..
  if(disp_table_ret == 0)
    // Display an alert that no events exist with the specified keyword.
    cout << "\nThere are no events with the keyword: " << keyword << endl;

  // Delete and nullify the memory allocated with keyword. 
  delete[] keyword;
  keyword = NULL;

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
// If 'N' is entered, false is returned. If the
// entry is invalid, an error message is printed
// and this function is returned (trys again).
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
    if(confirmChoice()) readInFilename(filename);
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
// If the user chooses to enter a filename in
// the console. See setFilename for more info.
//
// ********************************************/
void readInFilename(char *& filename)
{
  // If the filename is not NULL, return.
  if(filename) return;

  // Buffer for reading in the filename.
  char FILENAME_BUFFER[64];

  // Display prompt to enter a data file name.
  cout << "\n  Please enter the name of the data file: ";

  // Read in the filename.
  cin.get(FILENAME_BUFFER, 63, '\n');
  cin.ignore(128, '\n');
  cout << endl;

  // Allocate memory for the filename.
  filename = new char[strlen(FILENAME_BUFFER) + 1];
  // Store the contents of the filename
  // buffer into the filename array.
  strcpy(filename, FILENAME_BUFFER);

  // Display recieved filename.
  cout << "  Using data file: " << filename << endl << endl
       << "  Is this correct?" << endl;

  // Prompt for confirmation. If confirmed,
  // return to calling function.
  if(confirmChoice()) return;

  // Display alert that the function will execute again.
  cout << "  Trying again!" << endl;

  // Otherwise..
  // Delete & nullify the filename.
  delete filename;
  filename = NULL;

  // Keep trying.
  return readInFilename(filename);
}



/* ********************************************
// Reads in date from the event file (given by
// setFilename & readInFilename). Fills the 
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
    delete filename; filename = NULL;

    // Read in a new filename.
    readInFilename(filename);

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

    // If the current element is a date, format it.
    if(current_data_num == 2) formatDate(current_data_buf);

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

    // Format the event keyword for hashing.
    formatKeyword((*data_elements[current_data_num]));

    // Increment the data element counter.
    ++current_data_num;
    // Increment the start index (to skip the pipe).
    ++start_index;
  }

  return;
}



/* ********************************************
// Reads in a keyword to be searched in the
// hash table.
//
// ********************************************/
void readKeyword(char *& keyword)
{
  // If the keyword is not NULL, return.
  if(keyword) return;

  // Buffer for reading in the keyword.
  char KEYWORD_BUFFER[64];

  // Display prompt to enter a keyword.
  cout << "\n  Please enter the keyword: ";

  // Read in the keyword.
  cin.get(KEYWORD_BUFFER, 63, '\n');
  cin.ignore(128, '\n');
  cout << endl;

  // Allocate memory for the keyword.
  keyword = new char[strlen(KEYWORD_BUFFER) + 1];

  // Store the contents of the keyword
  // buffer into the keyword array.
  strcpy(keyword, KEYWORD_BUFFER);

  // Display recieved keyword.
  cout << "  Using keyword: " << keyword << endl << endl
       << "  Is this correct?" << endl;

  // Prompt for confirmation. If confirmed,
  // return to calling function.
  if(confirmChoice()) return;

  // Display alert that the function will execute again.
  cout << "  Trying again!" << endl;

  // Otherwise..
  // Delete & nullify the keyword.
  delete keyword;
  keyword = NULL;

  // Keep trying.
  return readKeyword(keyword);
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
// Reads in a date for removing old events from
// the table.
//
// ********************************************/
void readDate(char *& date)
{
  // If the date is not NULL, return.
  if(date) return;

  // Buffer for reading in the date.
  char DATE_BUFFER[64];

  // Display prompt to enter a date.
  cout << "\n  Please enter a date in the format: YYYY-MM-DD"
       << endl << endl
       << "\n  Enter date: ";

  // Read in the date.
  cin.get(DATE_BUFFER, 63, '\n');
  cin.ignore(128, '\n');
  cout << endl;

  // Allocate memory for the date.
  date = new char[strlen(DATE_BUFFER) + 1];

  // Format the date.
  formatDate(DATE_BUFFER);

  // Store the contents of the date
  // buffer into the date array.
  strcpy(date, DATE_BUFFER);

  // Display recieved date.
  cout << "  Using date: " << date << endl << endl
       << "  Is this correct?" << endl;

  // Prompt for confirmation. If confirmed,
  // return to calling function.
  if(confirmChoice()) return;

  // Display alert that the function will execute again.
  cout << "  Trying again!" << endl;

  // Otherwise..
  // Delete & nullify the date.
  delete date;
  date = NULL;

  // Keep trying.
  return readDate(date);
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



/* ********************************************
// Formats date entries from the console and
// the data file. Converts from yyyy-mm-dd to
// dd/mm/yyyy.
//
// ********************************************/
void formatDateAlt(char date[])
{
      // Number of characters in date[].
  int date_length = strlen(date),
      // Array of indices of date to be used
      // in switching letters around.
      switch_ord[] = {8,9,0,5,6,0,0,1,2,3},
      // date index counter.
      ch_index = 0;

  // Temporary char[] to store
  // the converted date.
  char temp[date_length + 1];

  // For every character in the date..
  for(; ch_index < date_length; ++ch_index)
  {
    // If the current character index is the
    // location of a delimiter..
    if(ch_index == 2 || ch_index == 5)
      // Set the delimiter.
      temp[ch_index] = '/';
    else
      // Set temp's current index to date's switch index.
      temp[ch_index] = date[switch_ord[ch_index]];
  }

  // Set the end to null terminating character.
  temp[date_length] = '\0';

  // Copy the contents of temp into date.
  strcpy(date, temp);

  return;
}



/* ********************************************
// Formats date entries from the console and
// the data file. Ensures that all dates have
// two characters for the day, two for the
// month, and four for the year. For example..
// many dates in the file have yyyy-d-mm,
// yyyy-dd-m or yyyy-d-m, rather than
// yyyy-dd-mm (which is necessary for the date
// comparing function in the table). God.. this
// function is aweful.
//
// ********************************************/
void formatDate(char date[])
{
      // Get the length of the date.
  int date_len = strlen(date),
      // Date index counter.
      ch = 0;

  // If the date is the correct length,
  // finish formatting with formatDateAlt.
  if(date_len == 10) return formatDateAlt(date);

  // Create a temporary array for
  // the properly formatted date.
  char temp[11];
  // Set the end to null terminating character.
  temp[10] = '\0';

  // Set the year (and first delimiter).
  for(; ch < 5; ++ch)
    temp[ch] = date[ch];

  // If only one character is missing..
  if(date_len == 9)
  {
    // If it's the month..
    if(isdigit(date[5]) && !isdigit(date[6]))
    {
      // Switch accordingly.
      temp[5] = '0';
      temp[6] =  date[5];
      temp[7] = '-';
      temp[8] = date[7];
    }
    // If it's the day
    else
    {
      // Switch accordingly.
      temp[5] = date[5];
      temp[6] = date[6];
      temp[7] = date[7];
      temp[8] = '0';
    }
    temp[9] = date[8];
  }
  // If two characters are missing..
  else
  {
    // Switch accordingly.
    temp[5] = '0';
    temp[6] =  date[5];
    temp[7] = '-';
    temp[8] = '0';
    temp[9] = date[7];
  }

  // Copy the temp into date..
  strcpy(date, temp);

  // Finish formatting with formatDateAlt.
  return formatDateAlt(date);;
}
