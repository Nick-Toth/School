/*****************************************************************
// File Name:  Program4_NToth.cpp
// Created By: Nicholas Toth
// E-Mail:     ntoth@pdx.edu
// Date:       3/10/2017
// Class:      CS162
// Program #:  4
//
// Overview: The purpose of this program is to help the user 
// maintain a personal record of jobs, interviews, or other 
// related information. The user may store up to 128 jobs in each 
// job object, and a total of 10 job objects per session. The limit
// for the number of jobs per job object can easily be modified 
// using the MAX_LOAD_SIZE constant in the Job_List header file. 
// The limit for the number of job objects can easily be modified 
// using the MAX_FILES_OPEN constant. As for functionality, the 
// program menu offers the user numerous options for managing jobs,
// i.e. loading/creating files, printing files, editing files. Jobs
// consist of six fields. The menu will loop until the user selects
// to quit. 
//
// ***************************************************************/

#include "Job_List.cpp"

// The names of the external file containing
// job data that I created for testing.
const char INTERVIEWS_FILENAME[] = "interviews.txt",
           JOBS_FILENAME[]       = "jobs.txt";

            // Probably overkill maximum filename length.
const short FILE_NAME_SIZE = 127,
            // Also probably overkill maximum number of
	    // open files.
            MAX_FILES_OPEN = 10;

// Greet user.
void beginProgram(void);
// Print main program menu, get user selections. 
short menuController(void);
// Prompt user to press enter to continue.
void continueMenu(void);
// Prompt for/read in the name of a file to open/create. 
void getFileToOpen(char **filename, short file_count);
// Prompt user to select a job to edit/display.
short selectFile(char **filenames, short file_count);
// Prompt user to confirm any changes made during the current session.
bool confirmWrite(void);


// Where the magic happens.
int main()
{
       // True when changes have been made.
  bool changes = false,
       // True when the user selects quit from the main program menu.
       quit    = false,
       // Used to determine whether or not changes have been made 
       // when a new file is created.
       alt    = false;

        // User's selection from the main menu. 
  short menu_selection = 0,
	// User's selected file for edit/print.
        file_selection = 0,
	// Number of open files.
        file_count     = 0,
	// Index counter for allocating/deallocating memory.
        index          = 0;

  // Array of strings containing filenames
  char **files = new char*[MAX_FILES_OPEN];
  // Allocating memory for filenames
  for(; index < MAX_FILES_OPEN; ++index)
    files[index] = new char[FILE_NAME_SIZE + 1];

  // Array of job objects created by the user. Up to 10.
  Job_List *j_lsts = new Job_List[MAX_FILES_OPEN];

  // While the user has not selected to quit the program..
  while(!quit)
  {
    // Print out the menu, read in the user's selected operation.
    menu_selection = menuController();
    // Execute user's selection.
    switch(menu_selection)
    {
      case 0: // Load a file
	// Get the name of a file to open from the user. Add tp files array.
        getFileToOpen(files, file_count);
	// This next bit seems weird, but it's important. If the user
	// creates a file and then opens a file (if Load returns directly
	// to changes), changes becomes false again. This keeps changes 
	// once it's set to true. 
        if(!changes) // Load/create a file. Read/initialize
	  changes = j_lsts[file_count].Load(files[file_count]);
	else // Alternate load/create.
	  alt = j_lsts[file_count].Load(files[file_count]);
	++file_count; // Increment the file tracker.
	break;

      case 1: // Edit a file
	// Get user's selection for which file to edit.
        file_selection = selectFile(files, file_count);
	// Edit user's selected file.
        j_lsts[file_selection].Edit();
	// Remember that changes have been made.
        changes = true;
	break;

      case 2: // Add a job
	// Get user's selection for which job object to edit.
        file_selection = selectFile(files, file_count);
	// Read in a new entry from the user to their selected job object.
        j_lsts[file_selection].Read();
	// Remember that changes have been made.
	changes = true;
	break;

      case 3: // Print a file
	// Get user's selection for which job object to edit.
        file_selection = selectFile(files, file_count);
	// Display the contents of the user's selected file.
        j_lsts[file_selection].Display();
	break;

      case 4: // Quit the program.
        quit = true;
        break;
    }

    // If the user has not selected to quit the program..
    if(!quit)
      // Prompt to press enter to continue.
      continueMenu();
  }

  // If changes have been made and the user confirms those changes..
  if(changes && confirmWrite())
  {
    for(index = 0; index < file_count; ++index)
      // Write the interviews to the interviews file.
      j_lsts[index].Save(files[index]);
    
    // Alert the user that the changes have been made.
    cout << "\nChanges written!" << endl;
  }

  // What even are memory leaks? ;)
  for(index = 0; index < MAX_FILES_OPEN; ++index)
    delete[] files[index];
  delete[] files;
  delete[] j_lsts;

  // Peace.
  cout << "\nGoodbye!" << endl << endl;

  return 0; // Fin.
}



/*******************************************************
// Obviously a very useful function.. Here in case 
// program is expanded and more initial functionality 
// is desired.
//
// *****************************************************/
void beginProgram(void)
{
  cout << "\nWelcome to your personal job & interview log." << endl;

  return;
}



/*******************************************************
// Prints out the program menu and reads in a selection
// from the user. Selection returned to main.
//
// *****************************************************/
short menuController(void)
{
  // The user's selection from the menu.
  short selection = -1;

  // Print out the menu options
  cout << "\nPlease select an option from the following menu" << endl
       // Load/create new file.
       << "\n\t[1] => Load a file." << endl
       // Edit one of the loaded files.
       << "\t[2] => Edit a file." << endl
       // Add a new job to one of the job objects.
       << "\t[3] => Add a new job." << endl
       // Print the contents of one of the job objects.
       << "\t[4] => Print a file." << endl
       // Terminate the program.
       << "\t[5] => Quit the program" << endl;

  //While the user's selection is invalid (or first attempt..
  while(selection < 1 || selection > 6)
  {
    // Prompt the user to enter their menu selection.
    cout << "\nEnter the number corresponding to your selection: ";
    // Read in the user's selection
    cin >> selection;
    cin.ignore(100,'\n');
    // If the user has entered an invalid selection..
    if(selection < 1 || selection > 6)
      // Alert the user that their entry was invalid.
      cout << "\nInvalid entry! Please enter a number from 1 through 6." << endl;
  }
  // Return the selection of the user to main.
  // Subtract 1 because menu options start at 1, not 0.
  return --selection;
}



/*******************************************************
// Prompts the user to press enter to continue.
//
// *****************************************************/
void continueMenu(void)
{
  // Prompt the user to press enter when they're
  // ready to move back to the menu.
  cout << "\nPress enter to continue...";
  // Wait for the user to press enter.
  cin.ignore(100,'\n');

  return;
}



/*******************************************************
// Prompts for/reads in the name of a file to open from
// the user. filenames is an array of all the files
// opened by the user. file_count is the number of 
// opened files.
//
// *****************************************************/
void getFileToOpen(char **filenames, short file_count)
{
    // If the user has opened the max number of files
    if(file_count >= MAX_FILES_OPEN)
    {
      // Tell user to calm down.
      cout << "Too many files open." << endl;
      return; // And exit.
    }

    // Prompt the user to enter their decision.
    cout << "\nPlease enter the name of the file (including" << endl
         << "its extension) that you would like to open"    << endl
         << "\nEnter the filename: ";

    // Read in the user's response.
    cin >> filenames[file_count];
    cin.ignore(100,'\n');
  
    return;
}



/*******************************************************
// Whenever a file is edited/displays, this function is
// called to determine which job object should be 
// operated on. filenames is an array of all the files
// opened by the user. file_count is the number of 
// opened files.
//
// *****************************************************/
short selectFile(char **filenames, short file_count)
{      
        // User's selected file.
  short selection = 0,
	// Index counter for traversing filenames. 
        index     = 0;
 
  // Alert user to select a file.
  cout << "Please select a file from the list: " << endl << endl;

  // For every open file (not actually open.. to be clear.)..
  for(; index < file_count; ++index)
    // print the name of the file and a number assigned to it.
    cout << "\t[" << (index+1) << "] => " << filenames[index] << endl;

  do
  {
    // Prompt user to make selection..
    cout << "\nFile Selection: ";
    // Read in user's selection.
    cin >> selection;
    cin.ignore(100,'\n');
    // If the selection is invalid..
    if(selection < 1 || selection > (file_count+1))
      // Tell the user that their selection is invalid.
      cout << "Invalid entry. Please enter a number from 1 to " << (file_count+1) << endl;
  }
  // KEEP TRYING UNTIL YOU GET IT RIGHT! 
  while(selection < 1 || selection > (file_count+1));

  // Return decrememnted selection because filename choices started at 1.
  return --selection;
}



/*******************************************************
// Prompts the user to confirm any changes made during
// the life of the program. Reads in their answer and
// returns true or false to main, which then decides
// whether to write or not.
//
// *****************************************************/
bool confirmWrite(void)
{
  // Contains the character entered by the user to
  // confirm writing changes. 'Y' or 'N'
  char confirmation = ' ';

  // While the entry is not a 'Y' or an 'N'..
  while(confirmation != 'Y' && confirmation != 'N')
  {
    // Prompt the user to enter their decision.
    cout << "\nDo you want to save your changes?(Y/N): ";

    // Read in the user's response.
    cin >> confirmation;
    cin.ignore(100,'\n');
    confirmation = toupper(confirmation);

    // If the user has not entered a valid response..
    if(confirmation != 'Y' && confirmation != 'N')
      // Tell the user that they're stupid, and to try again.
      cout << "\nInvalid entry! Please enter Y for Yes or N for No." << endl;
  }
  // If the user does not want to write changes..
  if(confirmation == 'N')
    // False -> don't write
    return false;
  // Otherwise (true) -> do write.
  return true;
}
