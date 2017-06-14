/*****************************************************************
// File Name:  Program5_NToth.cpp
// Created By: Nicholas Toth
// E-Mail:     ntoth@pdx.edu
// Date:       3/16/2017
// Class:      CS162
// Program #:  5
//
// Overview: The purpose of this program is to aid some user in
// maintaining some list of tasks required in order to accomplish
// some objective. When the program starts, the contents of some
// tasklist file are loaded into a task list object. Or, if the
// file does not exist, a new task list object is created and the
// user must initialize it with a label. The head task is always a
// label (as is the second line of the input file). The user can
// print, add, modify and remove tasks from the list. Finally,
// when the user selects quit from the program menu, the user will
// be asked to confirm any changes made. If the user selects to
// write the changes, then the same file that was read in will be
// erased and written to. After writing, or if the user selected
// not to save changes, dynamic memory is deallocated, and the
// program is terminated.
// 
// ***************************************************************/

#include "TaskList.cpp"

using namespace std;

// Example task file
const char EXAMPLE_FILENAME[] = "ExampleTask.txt";

// Greet the user / potential setup
void beginProgram(void);
// Prompt the user to press enter to continue.
void continueMenu(void);
// Print out the progrm's main menu. Read in selection.
short menuController(void);
// Print out all tasks, prompt user to select a specific task to be operated
// on (after operation is selected from the main menu), read in selection.
short selectTask(TaskList &tl);
// Prompt the user to confirm any changes made during the life of the program
bool confirmWrite(void);
// Once changes = true, it must stay that way. This function ensures
// that changes does not get set back to false once it's true.
void updateChanges(bool &changes, bool update);


int main()
{
  // The user's list of tasks.
  TaskList *tl = new TaskList;
       // changes is used to determine whether or not the external
       // file should be written to at the program's termination.
  bool changes = false,
       // quit controls the main loop of the program. Once the user
       // selects quit from the menu, quit becomes true.
       quit = false;
        // The user's current selection from the program's main menu.
  short menu_selection = -1,
	// The task selected by the user to be operated on.
	task_selection = -1;

  // Open and read in the contents of the file/create new list. If the
  // file already exists, changes = false. If new file is created,
  // changes = true. Currently, the filename must be defined in the program.
  changes = tl->readInTaskFile(EXAMPLE_FILENAME);

  // Greet the user.
  beginProgram();

  // While the user has not selected quit from the menu..
  while(!quit)
  {
    // Print out the menu, read in the user's selected operation.
    menu_selection = menuController();

    // Execute user's selection.
    switch(menu_selection)
    {

      // The user has decided to print all existing tasks.
      case 0:
        // Print all tasks.
        tl->displayTasks();
        break;

      // The user has decided to print a specific task.
      case 1:
	// If no tasks exist, alert the user. If only one task exists, print
        // it. Othewise, notify the user of the number of tasks that exist.
        // Read in the number corresponding the the task that they would like
        // to print. Print out the selected task.
        tl->displayTask();
        break;

      // The user has decided to add a new task to the list.
      case 2:
        // Prompt for/read in and set the new task. Update changes.
        updateChanges( changes, tl->newTask() );
        break;

      // The user has decided to edit an existing task.
      case 3:
        // Ask the user to select a task to edit.
      	task_selection = selectTask(*tl);
        // Prompt for/read in and set the updated task info to the selected
	// Task. Update changes.
        updateChanges( changes, tl->editTask(task_selection) );
        break;

      // The user has decided to edit the task list's label (head).
      case 4:
        // Prompt for/read in and update the label. Update changes.
        updateChanges( changes, tl->editLabel() );
        break;

      // The user has selected to remove the last task from the program menu.
      case 5:
        // Remove the last task (if there are any tasks. Does not remove the
	// label). Update changes.
        updateChanges( changes, tl->removeTask() );
        break;

      // The user has selected to quit.
      default:
        // Set the loop conditional to false.
        quit = true;
        break;
    }

    // Unless the user has selected to quit..
    if(!quit)
      // Prompt the user to press enter to continue.
      continueMenu();
  }

  // If changes have been made, and the user confirms those changes..
  if(changes && confirmWrite())
    // Write the changes to the external file.
    tl->writeToTaskFile(EXAMPLE_FILENAME);

  // Deallocate the TaskList.
  delete tl;
  return 0; // Fin.
}



/*******************************************
// Prints out the program menu and reads in
// a selection from the user. Selection
// returned to main.
// 
// *****************************************/
short menuController(void)
{
  // The user's selection from the menu.
  short selection = -1;

  // Print out the menu options
  cout << "\nPlease select an option from the following menu.." << endl
       // Notify the user to enter 1 to print the entire task list.
       << "\n\t[1] => Print all tasks." << endl
       // Notify the user to enter 2 to print a specific task.
       << "\t[2] => Print specific task." << endl
       // Notify the user to enter 3 to add a new task to the end of the list.
       << "\t[3] => Add a new task." << endl
       // Notify the user to enter 4 to edit an existing task.
       << "\t[4] => Edit a task." << endl
       // Notify the user to enter 5 to edit the task list's label.
       << "\t[5] => Edit task label." << endl
       // Notify the user to enter 6 to remove the last task from the list.
       << "\t[6] => Remove last task." << endl
       // Notify the user to enter 7 to terminate the program.
       << "\t[7] => Quit the program." << endl;

  //While the user's selection is invalid (or first attempt)..
  while(selection < 1 || selection > 7)
  {
    // Prompt the user to enter their menu selection.
    cout << "\nEnter the number corresponding to your selection: ";

    // Read in the user's selection
    cin >> selection;
    cin.ignore(100,'\n');

    // If the user has entered an invalid selection..
    if(selection < 1 || selection > 7)
      // Alert the user that their entry was invalid.
      cout << "\nInvalid entry! Please enter a number from 1 through 7." << endl;
  }

  // Decrement and return the selection of the user to main.
  return --selection;
}



/*******************************************
// Aid the user in selecting a specific
// task to operate on. Print out all of the
// tasks, read in numeric selection. The
// number is used to traverse the task list.
// 
// *****************************************/
short selectTask(TaskList &tl)
{
        // The user's selection.
  short selection = 0,
        // The number of tasks in the list.
        task_count = tl.getSize()-1; // -1 because of label.

  // If there are no tasks..
  if(task_count < 1)
  {
    // Alert the user that there are no tasks to choose from.
    cout << "\nThere are no tasks to choose from." << endl;
    // Return default(invalid) selection.
    return -1;
  }

  // Alert the user that tasks are being printed..
  cout << "\nSelect a task from the following:" << endl;
  // Display the tasks.
  tl.displayTasks();

  // While the selection is invalid..
  while(selection < 1 || selection > task_count)
  {
    // Prompt the user to enter their menu selection.
    cout << "\nEnter the number corresponding to your selection: ";

    // Read in the user's selection
    cin >> selection;
    cin.ignore(100,'\n');

    // If the user has entered an invalid selection..
    if(selection < 1 || selection > task_count)
      // Alert the user that their entry was invalid.
      cout << "\nInvalid entry! Please enter a number from 1 through "
           << task_count << '.' << endl;
  }

  // Return the selection of the user to main.
  return selection;
}



/*******************************************
// Obviously a very useful function.. Here
// in case program is expanded and more
// initial functionality is desired.
// 
// *****************************************/
void beginProgram(void)
{
  cout << "\nWelcome to your personal task list utility." << endl;
  return;
}



/*******************************************
// Prompts the user to press enter to
// continue.
// 
// *****************************************/
void continueMenu(void)
{
  // Prompt the user to press enter when they're ready to move back to the menu.
  cout << "\nPress enter to continue...";

  // Wait for the user to press enter.
  cin.ignore(100,'\n');

  return;
}



/*******************************************
// Prompts the user to confirm any changes
// made during the life of the program.
// Reads in their answer and returns true
// or false to main, which then decides
// whether to write or not.
// 
// *****************************************/
bool confirmWrite(void)
{
  // Contains the character entered by the user to confirm writing changes.
  char confirmation = ' ';

  // While the entry is not a 'Y' or an 'N'..
  while(confirmation != 'Y' && confirmation != 'N')
  {
    // Prompt the user to enter their decision.
    cout << "\nDo you want to save your changes?(Y/N): ";

    // Read in the user's response.
    cin >> confirmation;
    cin.ignore(100,'\n');
    // Convert to uppercase.
    confirmation = toupper(confirmation);

    // If the user has not entered a valid response..
    if(confirmation != 'Y' && confirmation != 'N')
      // Ask them to enter a new response.
      cout << "\nInvalid entry! Please enter Y for Yes or N for No." << endl;
  }

  // If the user does not want to write their changes..
  if(confirmation == 'N')
    // False, don't write.
    return false;
  // Otherwise (true), write.
  return true;
}



/*******************************************
// Used to perserve the changes bool as
// true, once set. Without this function,
// it's very possible that changes will be
// made, and then a user will select an
// operation that might cause changes, but
// end up not making changes (which would
// set changes back to false).
// 
// *****************************************/
void updateChanges(bool &changes, bool update)
{
  // If changes is false..
  if(!changes)
    // set changes to the value returned by the function
    // stored in update.
    changes = update;

  return;
}
