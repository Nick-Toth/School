/*****************************************************************
// File Name:  Program2_NToth.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 10th, 2017
// Class:      CS163
// Program #:  2
//
// Overview: This is the testing file for the airport management
// program. Generally, this file is used to test the queue and
// stack ADTs, located in the Queue.h/cpp and Stack.h/cpp files,
// as well as the Passenger struct (Passenger.h/cpp), which serves
// as their primary data. The program laid out in this file
// illustrates the intended flow of these ADTs, respectively.
// Moreover, this file contains a suite of tools which simplify
// the management of the Queue, Stack and Passenger ADTs.
//
// ***************************************************************/

#include <iostream>
#include <fstream>

#include "Queue.h" // Includes Passenger.h and cmath.
#include "Stack.h"


using namespace std;


                     // The maximum length for a passenger's name.
const unsigned short MAX_NAME_LEN = 128,
                     // The number of passengers to use in simulations.
                     PASSENGER_SIM_LEN = 100,
                     // The maximum length for a passenger's seat number.
                     MAX_SEAT_NO_LEN = 8;


// *****************  |
// Menu & Handlers *  |
// *****************  V

// Print out the main program menu,read in & return selection.
void menuController(short & selection);

// Prompt user to press enter to continue.
void continueMenu(void);

// Menu selection handler for arrival simulation. (Option 1)
void handleArrival( Passenger passengers[PASSENGER_SIM_LEN + 1],
                    Queue & check_in_q);

// Menu selection handler for display check in line. (Option 2)
void handlePrintCheckin(Queue & check_in_q);

// Menu selection handler for gate line-up simulation. (Option 3)
void handleGateLineup( Queue & check_in_q,
                       Queue & gate_q);

// Menu selection handler for display gate line. (Option 4)
void handlePrintGate(Queue & gate_q);

// Menu selection handler for passenger boarding simulation. (Option 5)
void handleBoarding( Queue & gate_q,
                     Stack & the_plane);

// Menu selection handler for display the plane. (Option 6)
void handlePrintPlane(Stack & the_plane);


// *******************  |
// Testing functions *  |
// *******************  V

// Loads the fundamental specifications for running the simulations.
void loadParams( short & num_seats,
                 short & num_rows,
                 Passenger passengers[PASSENGER_SIM_LEN] );

// Prompt for and read in a number to item_loc.
void loadUNum( const char * item_name,
               short & item_loc );

// Prompt for and read passenger info.
void loadPassenger( Passenger * passenger,
                    const char * fst_open_row_s,
                    short num_seats );

// Display error messages for input operations.
void entryError( const char *item,
                 short error_code,
                 short max = 0 );

// Prompt the user to enter Y or N. Return true for Y.
bool confirmChoice(void);

// Initializes an array of passengers.
void generatePassengers( Passenger passengers[PASSENGER_SIM_LEN + 1],
                         short num_seats,
                         short num_rows );

// Reads in passenger names for simulations.
void readInNames(char names[PASSENGER_SIM_LEN][MAX_NAME_LEN + 1]);

// Assigns passenger seat numbers for simulations.
void assignSeats( short num_seats,
                  short num_rows,
                  char **& seats );

// Converts a number to a string of characters and
// inserts the number into an array.
void insertNumStr( short num_to_insert,
                   char *& array );

// Returns the number of digits in some number.
short numDigits(short num);

// Display each of the passengers in the Passengers array.
void printPassengerArray(Passenger passengers[PASSENGER_SIM_LEN + 1]);


int main(void)
{
  // Loop control variable set to true when
  // the user selects quit from the menu.
  bool quit = false;

        // Stores the user's menu selection.
  short selection = 0,
        // The number of seats per row.
        seats = 0,
        // The number of rows in the plane.
        rows = 0;

  // Passenger array for simulations. Allocate enough
  // memory for PASSENGER_SIM_LEN auto-generated
  // passengers, and an additional passenger created
  // in the console.
  Passenger * passengers = new Passenger[PASSENGER_SIM_LEN + 1];

  // Print program header.
  cout << "\n ----------------- Flight Management Program ----------------- " << endl;

  // Load simulation specifications -
  // seats, rows & passengers.
  loadParams(seats, rows, passengers);

        // Create a new queue for simulating the check in line.
  Queue check_in_q,
        // Create a new queue for simulating gate line.
        gate_q;

  // Create a new stack for simulating the plane.
  Stack the_plane(seats);

  // Main program loop.
  do
  {
    // Print the program's main menu,
    // read in the user's selection.
    menuController(selection);

    // Execute the handler function
    // for the user's selection.
    switch(selection)
    {
      case 1: // Arrival simulation.
        handleArrival(passengers, check_in_q);
        // Deallocate passenger memory.
        delete[] passengers;
        passengers = NULL;
        break;

      case 2: // Display check in line.
        handlePrintCheckin(check_in_q);
        break;

      case 3: // Gate line-up simulation.
        handleGateLineup(check_in_q, gate_q);
        break;

      case 4: // Display gate line.
        handlePrintGate(gate_q);
        break;

      case 5: // Boarding simulation.
        handleBoarding(gate_q, the_plane);
        break;

      case 6: // Display the plane.
        handlePrintPlane(the_plane);
        break;

      default: // 7 - Quit the program.
        quit = true;
        break;
    } // End switch.

    // Reset selection.
    selection = 0;

    // If anything other than quit has
    // been selected from the menu..
    if(!quit)
      // Prompt the user to press enter
      // to continue, wait, continue.
      continueMenu();
  }

  // Continue running the program until
  // quit is selected from the menu.
  while(!quit);

  // Quit the program.
  cout << "\n  Goodbye!\n" << endl;

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
  short max_selections = 7;

  // Print out the program menu.
  cout << "\n  Make a selection from the menu." << endl << endl << "    "
       // Tell main to execute the handleArrival function.
       << "[1] => Simulate passenger arrival." << endl << "    "
       // Tell main to execute the handlePrintCheckin function.
       << "[2] => Display passengers in check-in line." << endl << "    "
       // Tell main to execure the handleGateLineup function.
       << "[3] => Simulate gate assembly." << endl << "    "
       // Tell main to execute the handlePrintGate function.
       << "[4] => Display passengers at the gate." << endl << "    "
       // Tell main to execute the handleBoarding function.
       << "[5] => Simulate passenger boarding." << endl << "    "
       // Tell main to execute the handlePrintPlane function.
       << "[6] => Display the contents of the plane." << endl << "    "
       // Tell main to exit the program.
       << "[7] => Quit the program." << endl;

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
      entryError("Selection", 2, max_selections);
  }
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
// This is the handler for whenever the arrival
// simulation option (1) is selected from the
// main program menu.
//
// @param passengers: The array of passengers
// to be enqueued into the check_in_q.
//
// @param check_in_q: The queue to be initialized
// with the passengers in the passengers array.
//
// ********************************************/
void handleArrival( Passenger passengers[PASSENGER_SIM_LEN + 1],
                    Queue & check_in_q )
{
  if(!passengers)
  {
    cout << "\n  Passenger arrival already complete!" << endl;
    return;
  }
  // General index counter.
  short g_index = 0;

  // Transfer all of the passengers in the
  // passengers array into the queue.
  for(; g_index < PASSENGER_SIM_LEN + 1; ++g_index)
    check_in_q.enqueue(passengers[g_index]);
  
  // Print alert that the arrival queue has been filled.
  cout << "\n  Arrival complete."
       << endl << endl;

  // Print queue - For debugging.
  // handlePrintCheckin(check_in_q);
}



/* ********************************************
// This is the handler for whenever the print
// check in line option (2) is selected from the
// main program menu.
//
// @param  check_in_q: The queue to be displayed.
//
// ********************************************/
void handlePrintCheckin(Queue & check_in_q)
{
  // Print some whitespace.
  cout << "\n  ";

  // Check whether the queue is empty.
  if(check_in_q.isEmpty())
    cout << "There are no passengers in the check in line!";
  else
  {
    cout << "Printing the check in line order.\n" << endl;
    check_in_q.display();
    cout << "\n  End of the check in line.";
  }

  // Print some whitespace.
  cout << endl;

  return;
}



/* ********************************************
// This is the handler for whenever the gate
// line-up simulation option (3) is selected
// from the main program menu.
//
// @param check_in_q: The queue to be
// transferred into the gate_q.
//
// @param gate_q: The queue to be initialized
// with the passengers in the check_in_q.
//
// ********************************************/
void handleGateLineup( Queue & check_in_q,
                       Queue & gate_q )
{

  // If the check in queue is empty..
  if(check_in_q.isEmpty())
  {
    // Print alert that no passengers can be transferred.
    cout << "\n  There are no passengers in the check in line."
         << endl << endl;

    // Return to main.
    return;
  }

  // Temporary passenger for transferring
  // from the check_in_q to the gate_q.
  Passenger temp;

  // While there are unadded passengers..
  while(!check_in_q.isEmpty())
  {
    // Dequeue from the check in queue.
    check_in_q.dequeue(temp);
    // Enqueue into the gate queue.
    gate_q.enqueue(temp);
  }

  // Print alert that the gate queue has been filled.
  cout << "\n  Gate lineup complete."
       << endl << endl;

  // Print gate queue - For debugging.
  // handlePrintGate(gate_q);

  // Print queue - For debugging.
  // handlePrintCheckin(check_in_q);

  return;
}



/* ********************************************
// This is the handler for whenever the print
// gate line option (4) is selected from the
// main program menu.
//
// @param gate_q: The queue to be displayed.
//
// ********************************************/
void handlePrintGate(Queue & gate_q)
{
  // Print some whitespace.
  cout << "\n  ";

  // Check whether the queue is empty.
  if(gate_q.isEmpty())
    cout << "There are no passengers at the gate!";
  else
  {
    cout << "\n  Printing the gate line order.\n" << endl;
    gate_q.display();
    cout << "\n  End of the gate line.";
  }

  // Print some whitespace.
  cout << endl;

  return;
}



/* ********************************************
// This is the handler for whenever the boarding
// simulation option (5) is selected from the
// main program menu.
//
// @param gate_q: The queue to be unloaded into
// the plane.
//
// @param the_plane: The stack to be filled with
// the passengers in the qate_q.
//
// ********************************************/
void handleBoarding( Queue & gate_q,
                     Stack & the_plane )
{
  // Init the passenger counter.
  the_plane.setNumPassengers(gate_q.getNumPassengers());

  // If the check in queue is empty..
  if(gate_q.isEmpty() || !the_plane.isEmpty())
  {
    // Print alert that no passengers can be transferred.
    cout << "\n  There are no passengers at the gate."
         << endl << endl;
    // Return to main.
    return;
  }

  // Temporary passenger for transferring
  // from the gate_q to the_plane.
  Passenger temp;

  // While there are unadded passengers..
  while(!gate_q.isEmpty())
  {
    // Dequeue from the gate queue.
    gate_q.dequeue(temp);
    // Push onto the plane.
    the_plane.push(temp);
  }

  // Print alert that the passenger stack has been filled.
  cout << "\n  Passenger barding complete."
       << endl << endl;

  // Print gate queue - For debugging.
  //handlePrintGate(gate_q);

  // Print queue - For debugging.
  //handlePrintPlane(the_plane);

  return;
}



/* ********************************************
// This is the handler for whenever the display
// passengers on the plane option (6) is
// selected from the main program menu.
//
// @param the_plane: The stack to be displayed.
//
// ********************************************/
void handlePrintPlane(Stack & the_plane)
{
  // Print some whitespace.
  cout << "\n  ";

  // Check whether the plane is empty.
  if(the_plane.isEmpty())
    cout << "There are no passengers on the plane!";
  else
  {
    cout << "\n  Printing the passengers on the plane.\n" << endl;
    the_plane.display();
    cout << "\n  There are not more passengers on the plane.";
  }

  // Print some whitespace.
  cout << endl;

}



/* ********************************************
// Initializes the primary specifications for
// running the test simulations.
//
// @param num_seats: The location where the
// number of seats will be initialized.
//
// @param num_rows: The location where the
// number of rows will be initialized.
//
// @param passengers: The location where the
// simulation passengers will be initialized.
//
// ********************************************/
void loadParams( short & num_seats,
                 short & num_rows,
                 Passenger passengers[PASSENGER_SIM_LEN + 1] )
{

  // The number of passengers in the simulation.
  const short psngr_cnt = PASSENGER_SIM_LEN + 1;

  // Prompt for, read in numbers for rows and seats.
  cout << "\n  To begin, please initialize the numbers of rows and seats." << endl
       << "\n  This simulator will be using " << psngr_cnt
       << " passengers, so please enter" << endl
       << "  a row and seat pair whose product is at least " << psngr_cnt << '.' << endl
       << "  Also node that your row entry will count for both sides." << endl
       << "  e.g. 60 means 30 per side." << endl
       << "  Your seat entry will count for one side." << endl
       << "  e.g. 3 means 3 seats per row per side, for a total of" << endl
       << "  3 * 2 * row entry seats." << endl;

  do
   {
    // Load number of rows.
    loadUNum("rows", num_rows);

    // Load number of seats.
    loadUNum("seats per row", num_seats);

    // If the product of the seat and row entries
    // is not at least as big as psngr_cnt..
    if( (num_rows * num_seats) < psngr_cnt )
      // Print out an error message.
      entryError(NULL, 4, psngr_cnt);
  }
  // Keep trying until the product of rows
  // and seats is larger than psngr_cnt.
  while( (num_rows * num_seats) < psngr_cnt );

  // Initialize the passengers array for simulations.
  generatePassengers(passengers, num_seats, num_rows);

  // Display the number of passengers that have been created.
  // Display a prompt to enter an additional passenger.
  cout << "\n  " << (psngr_cnt - 1) << " simulation passengers have been created!\n\n"
       << "  Would you like to add a passenger?";

  // Prompt the user to enter Y or N. If the user enters Y, load
  // a new passenger into the last index of the
  if(confirmChoice())
    // Load the new passenger into the last index of
    // the passengers array.
    loadPassenger((passengers + PASSENGER_SIM_LEN), passengers[PASSENGER_SIM_LEN - 1].seat_num, num_seats);

  // Display the passenger array.
  // printPassengerArray(passengers);

  return;
}



/* ********************************************
// Prompts for and reads in a positive whole
// number for some item.
//
// @param item_name: The name of the item to
// be set.
//
// @param item_loc: The location where the
// number will be stored.
//
// ********************************************/
void loadUNum(const char * item_name, short & item_loc)
{
  // Reset item_loc.
  item_loc = 0;

  // Try to read in a number.
  do
  {
    // Display a prompt to enter a number.
    cout << "\n  Please enter the number of " << item_name << ": ";

    // Read in the entry.
    cin >> item_loc;
    cin.ignore(100, '\n');
    cout << endl;

    // If the entry is invalid..
    if(item_loc < 1)
      // Report error to console.
      entryError(item_name, 1);
  }
  // Continue until a valid number is entered.
  while(item_loc < 1);

  // Display the final entry.
  cout << "  You have entered " << item_loc
       << ' ' << item_name << '.' << endl << endl;

  // Ask for confirmation.
  // If the user wants to try again (false), return this function.
  if(!confirmChoice()) return loadUNum(item_name, item_loc);

  return;
}



/* ********************************************
// Prompts for and reads in the name and seat
// number for a passenger.
//
// @param passenger: The location where a new
// passenger will be initialized.
//
// @param fst_open_row_s: The first open row in
// which a passenger can be seated (string).
//
// ********************************************/
void loadPassenger( Passenger * passenger,
                    const char * fst_open_row_s,
                    short num_seats )
{

       // Buffer for the new passenger's name.
  char name_buffer[MAX_NAME_LEN + 1],
       // Buffer for the new passenger's seat number.
       seat_buffer[MAX_SEAT_NO_LEN + 1],
       // Passenger's seat entry alphabetic component.
       seat_entry[MAX_SEAT_NO_LEN + 1] = {'\0'};

        // The first open row in which a passenger
        // can be seated (numeric).
  int fst_open_row_n = parseInt(fst_open_row_s) - 1,
        // Entered row number.
        row_entry = 0,
        // Value returned from parseAlpha.
        alpha_ret,
        // The correct number of alphabetic characters.
        num_alpha = (num_seats / 26 + 1);

  // Loop control variable.
  bool try_again = false;

  // Initialize passenger's name.
  // Display prompt to enter passenger name.
  cout << "\n  Please enter the name of the passenger: ";
  // Read in passenger's name.
  cin.get(name_buffer, MAX_NAME_LEN, '\n');
  cin.ignore(100,'\n');
  cout << endl;

  // Initialize passenger's seat number.
  do
  {
    // Reset local fields.
    memset(seat_buffer, 0, strlen(seat_buffer));
    memset(seat_entry, 0, strlen(seat_entry));
    alpha_ret = 0;
    try_again = false;

    // Display prompt for passenger's seat number.
    cout << "\n  " << endl
         << "\n  Please enter the passenger's seat number.."
         << "\n  You may select a seat anywhere in rows 1 through " << fst_open_row_n
         << ".\n  And a seat from \'A\' through \'" << static_cast<char>(('A' + ((num_seats - 1) % 26 ))) << "\' "
         << "\n  For example, if you want a window seat in row 13, enter 13A."
         << "\n\n    Seat: ";

    // Read in passenger's seat number.
    cin.get(seat_buffer, MAX_SEAT_NO_LEN, '\n');
    cin.ignore(100,'\n');
    cout << endl;

    // Get the row number from the seat entry.
    row_entry = parseInt(seat_buffer);

    // Get the seat letter from the seat entry.
    alpha_ret = parseAlpha(seat_buffer, seat_entry);

    // If the entry number was not in the valid range
    if(row_entry < 1 || row_entry > fst_open_row_n)
    {
      // Print an error message.
      entryError("row", 2, fst_open_row_n);

      // Try again.
      try_again = true;
    }

    // If too many alphabetic characters were entered..
    if(alpha_ret != num_alpha && !try_again)
    {
      // Print an error message.
      entryError("seat", 6, num_alpha);
      // Try again.
      try_again = true;
    }

    // Check validity of each alphabetic character
    do
    {
      // If the seat entry is/entries are not in the correct range..
      if(seat_entry[alpha_ret-1] > ('A' + ((num_seats - 1) % 26 )) && !try_again)
      {
        // Print an error message.
        entryError("seat", 5, num_seats-1);
        // Try again.
        try_again = true;
      }
    }
    // Continue while there are unchecked characters.
    while(--alpha_ret > 0 && !try_again);
  }
  // Keep trying until the entry is valid.
  while(try_again);

  // Initialize the passenger with the contents
  // of the name and seat buffers.
  passenger->setupPassenger(name_buffer, seat_buffer);

  // Display, confirm passenger.
  cout << "\n  Passenger Created!" << endl << endl
       << "    Name: " << passenger->name << endl
       << "    Seat: " << passenger->seat_num << endl << endl;

  // Ask the user to confirm. If the user does not
  // want to keep their entry, try again.
  if(!confirmChoice()) return loadPassenger(passenger, fst_open_row_s, num_seats);

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
  // entry error message #3.
  entryError(NULL, 3, 0);

  // Try again.
  return confirmChoice();
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
//      1 => Invalid Entry - Not unsigned int.
//      2 => Invalid Entry - Not in range 1, max.
//      3 => Invalid Entry - Not 'Y' or 'N'
//      4 => Invalid Entry - Not large enough.
//      5 => Invalid Entry - Not in range A,
//                           A + max.
//      6 => Invalid Entry - Too many alpha
//                           characters.
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
    case 1: // Invalid row/seat entry.
      cout << "Invalid Entry => " << item
           << " must be a positive whole number";
      break;

    case 2: // Invalid menu/new passenger row selection.
      cout << "Invalid Entry => " << item
           << " must be a number from 1 to " << max;
      break;

    case 3: // Invalid confirmation entry.
      cout << "Invalid Entry => Please enter Y to confirm or N to retry..";
      break;

    case 4: // Invalid row/seat product. (Must be > PASSENGER_SIM_LEN + 1).
      cout << "Invalid Entry => Please enter numbers whose product is at least " << max;
      break;

    case 5: // Invalid seat alpha entry.
      cout << "Invalid Entry => Please enter " << item
           << " alphabetic characters from 'A' through " << static_cast<char>('A' + max);
      break;

    case 6: // Invalid seat alpha entry.
      cout << "Invalid Entry => " << item << " must have " << max << " alphabetic characters..";
           break;

    default: // Some other undefined entry error.
      cout << "Undefined..";
      break;
  }

  // Glorious whitespace.
  cout << endl;

  return;
}



/* ************************************************************
// Reads in names from the Names.txt file for use in simulating
// passengers.
//
// @param names: The location to which passenger names will be
// written.
//
// ************************************************************/
void readInNames(char names[PASSENGER_SIM_LEN][MAX_NAME_LEN + 1])
{
  // Open the file containing passenger
  // (game of thrones character) names.
  ifstream name_file("Names.txt");

  // Index counter for adding names to
  // the names array.
  short name_index = 0;

  // Buffer for reading in lines from
  // the Names.txt file.
  char line_buf[128];

  // While the file isn't empty and the
  // name index counter is less than 100..
  //
  // ** Note that the name_index size
  // condition is only here in case
  // someone messes with the names file.
  while(name_file.good() && name_index < PASSENGER_SIM_LEN)
  {
    // Read in the next line.
    name_file.get(line_buf, 128, '\n');
    name_file.ignore(100, '\n');

    // If the line isn't a comment..
    if( line_buf[0] != '/' )
    {
      // Copy the contents of the buffer into
      // the current index of the names array.
      strcpy(names[name_index], line_buf);

      // Increment the names index counter.
      ++name_index;
    }
  }
}



/* ************************************************************
// Initializes an array of passengers with names from the
// Names.txt file, and seat numbers generated by the assignSeats
// function.
//
//
// @param passengers: The array of passengers to be simulated.
//
// @param num_seats: The number of seats in a row. (one side).
//
// @param num_rows: The number of rows on the plane.
//
// ************************************************************/
void generatePassengers( Passenger passengers[PASSENGER_SIM_LEN + 1],
                         short num_seats,
                         short num_rows )
{

        // General index counter.
  short g_index = 0,
        // The max size of a passenger's seat number is the number
        // of digits in the largest row number added to the maximum
        // number of alpha chars in a seat (Adding 1 would probably
        // be just fine, but who knows, maybe there will one day be
        // a plane with rows of more than 26 seats).
        seat_num_size = numDigits(num_rows) + 1 + static_cast<short>(num_seats / 26);

  // Array of passenger names for simulation.
  char passengerNames[PASSENGER_SIM_LEN][MAX_NAME_LEN + 1],
       // Array of passenger seat numbers for simulation.
       ** passengerSeats = new char *[PASSENGER_SIM_LEN];

  // Allocate memory for passenger seats.
  for(; g_index < PASSENGER_SIM_LEN; ++g_index)
    passengerSeats[g_index] = new char[seat_num_size + 1];

  // Read in passenger names from the Names.txt
  // file into the passengerNames array.
  readInNames(passengerNames);
  // Assign seat numbers
  assignSeats(num_seats, num_rows, passengerSeats);

  // For every passenger specified by the PASSENGER_SIM_LEN..
  for(g_index = 0; g_index < PASSENGER_SIM_LEN; ++g_index)
    // Load the current passenger with the corresponding name and
    // seat in the passengerNames and passengerSeats matrices.
    passengers[g_index].setupPassenger( passengerNames[g_index], passengerSeats[g_index] );

  // For each array of characters in the passengerSeats matrix..
  for(g_index = 0; g_index < PASSENGER_SIM_LEN; ++g_index)
    // Deallocate the memory of the current passenger seat.
    delete[] passengerSeats[g_index];

  // Deallocate & nullify the remaining passengerSeats memory.
  delete[] passengerSeats;
  passengerSeats = NULL;

  return;
}



/* ************************************************************
// Initializes a matrix of characters with seat assignments
// for passengers. The seats have two components. The first
// component is a string of numbers, which represent the row
// in which a passenger will be seated. Odd and even numbers
// correspond to alternate sides of the plane. The second
// component is a string of alphabetic characters represending
// the specific seat in the seat's row. There will probably
// only be one alpha char - unless a row (on one side) has
// more than 26 seats - but this would work for such a
// preposterous plane.
//
// Seats are assigned such that passengers are evenly
// distributed on both sides of the plane. Assignments start
// at the back of the plane (num_rows), and work towards the
// front. Similarly, seat assignments start at the window and
// work towards the isle. For example, in a plane with 60
// rows and 3 seats per row, seats would be assigned in the
// following pattern:
//
//    60A - Row 60 (right of 59), First seat.
//    59A - Row 59 (left of 60),  First seat.
//    60B - Row 60, Second seat
//    ...
//    59C - Row 59, Last seat.
//
//    58A - Row 58 (right of 57), First seat.
//    57A - Row 57 (left of 58), First seat.
//    ...
//    1C - front, left, isle.
//
// @param num_seats: The number of seats in a row. (one side).
//
// @param num_rows: The number of rows on the plane.
//
// @param seats: The location where seats will be added.
//
// ************************************************************/
void assignSeats( short num_seats,
                  short num_rows,
                  char **& seats )
{

        // Switches between 0 and 1 with each insertion
        // so that passengers are evenly distributed on
        // each side of the plane.
  short next_side = 0,
        // The number of digits in the current row.
        // Used as a starting index for the alphabetic
        // component of a seat number.
        row_digits = 0,
        // Counts up the the total number of rows, by 2,
        // until all passengers are added. Used in
        // calculating the current row to seat a
        // passenger.
        row_count = 0,
        // Loop control for adding alphabetic characters.
        alpha_added = num_seats,
        // Counter for adding alpha characters to the seat
        // number. Incremented by 1 until == (seats/26).
        alpha_count = 0,
        // The row in which a passenger will be seated.
        current_row = 0,
        // Used to keep track of what seat in the current
        // row is being used. Reset to 0 whenever a row
        // is filled.
        current_seat = 0,
        // Index counter for inserting seats into the
        // seats array.
        current_passenger = 0,
        // The number of seats in a row across the whole
        // plane. Sum of left and right sides.
        seats_in_full_row = 2 * num_seats;

  // The character specifying the specific seat in the
  // row where the passenger will be sitting.
  char alpha = '\0';

  // While there are passengers without seats..
  while(current_passenger < PASSENGER_SIM_LEN)
  {
    // If the current seat for the current row is
    // less than the max..
    if(current_seat < seats_in_full_row)
    {
      // Set the numeric component of the seat number..

      // The current passenger's seat number will be
      // the number of filled rows and the side switch
      // subtracted from the total number of rows.
      current_row = num_rows - next_side - row_count;
      // Add the numeric component of the seat number to
      // the current passenger's seat in the seats array.
      insertNumStr(current_row, seats[current_passenger]);
      // If the next side is currently 0 (right), set it
      // to 1 (left). Otherwise, set it back to 0. etc...
      next_side = (next_side == 0)? 1:0;

      // Set the alphabetic component of the seat number.

      // Get the number of digits in the current row.
      row_digits = numDigits(current_row);

      // While there are still alphabetic characters to
      // add to the passenger seat number..
      while(alpha_added > 0)
      {

        // Set the current alphabetic character to 'A' +
        // the value of the outer loop counter (passenger
        // counter) mod the total number of seats mod 26.
        // This modding is done to shift letters after the
        // alphabet is traversed, and to ensure that the
        // characters never go past 'Z'.
        alpha = static_cast<char>( 'A' + (current_passenger % num_seats) % 26);

        // Set the current passenger's current alpha
        // character to alpha.
        seats[current_passenger][row_digits + alpha_count] = alpha;
        // Increment the alphabetic character counter.
        ++alpha_count;
        // Subtract the length of the alphabet from 'A'.
        alpha_added -= 26;
      }

      // Reset alpha_added.
      alpha_added = num_seats;
      alpha_count = 0;

      // Increment the current seat tracker for the
      // current row.
      ++current_seat;
      // Increment the passenger index counter.
      ++current_passenger;
    }
    // If the current row is full..
    else
    {
      // Increment the row counter by two.
      row_count += 2;
      // Reset the row counter to 0.
      current_seat = 0;
    }

  } // End While.

}



/* ************************************************************
// Inserts a number into the beginning of an array of characters.
//
// @param num_to_insert: The number to
// insert into the array of characters.
//
// @param array: The array in which num_to
// _insert will be inserted.
//
// ************************************************************/
void insertNumStr(short num_to_insert, char *& array)
{
        // The number of digits in num_to_insert.
        // Used for loop control.
  short digits = numDigits(num_to_insert),
        // Loop control variable.
        digit_cnt = 0,
        // The current digit to be inserted into
        // the array with each loop iteration.
        current_digit = 0;

  // For each digit in num_to_insert..
  for(; digit_cnt < digits; ++digit_cnt)
  {
    // Set the current digit to be inserted to
    // the digit in the ones place of num_to_insert.
    current_digit = num_to_insert % 10;
    // Truncate the ones place of num_to_insert.
    num_to_insert = (num_to_insert - current_digit) / 10;
    // Insert the number from the ones place of
    // num_to_insert into the ones place index of
    // the array.
    array[ digits - digit_cnt - 1] = static_cast<char>('0' + current_digit);
  }
}



/* ************************************************************
// Counts the number of digits in a number.
//
// @param num: The number whose digits
// will be counted.
//
// @return: The number of digits in num.
//
// ************************************************************/
short numDigits(short num)
{
  // Digit counter.
  short digits = 0;

  // Increment the digit counter and divide the
  // num param by the base (10) until the digits
  // have all been counter.
  do
  {
    num /= 10;
    ++digits;
  }
  while(num);

  // Return the number of digits in the number.
  return digits;
}



/* ************************************************************
// Calls the Passenger struct's display function on each of
// the passengers in some Passenger array. Here for debugging.
//
// @param passengers: The array of passengers to be printed.
//
// ************************************************************/
void printPassengerArray(Passenger passengers[PASSENGER_SIM_LEN + 1])
{
  // Print whitespace and header.
  cout << "\n\n\tDisplaying Passengers:\n" << endl;

  // Call the display function on each Passenger in the array.
  for(short idx = 0; idx < PASSENGER_SIM_LEN + 1; ++idx)
    passengers[idx].display();

  // Print whitespace and footer.
  cout << "\n\n\tEnd of Passengers" << endl << endl;

  return;
}