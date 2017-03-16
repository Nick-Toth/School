/*****************************************************************
// File Name:  TaskList.cpp
// Created By: Nicholas Toth
// E-Mail:     ntoth@pdx.edu
// Date:       3/16/2017
// Program #:  5
//
// Overview: This is the implementation file for the TaskList
// class (as well as the Task struct). As explained in the Main
// file, TaskList is a utility for maintaining a list of tasks
// which must be completed in order to accomplish some goal. This
// file implements the functions that allow a TaskList object to
// be operated on. Essentially, these tasks include printing,
// editing, adding, and removing tasks.
// 
// ***************************************************************/

#include "TaskList.h"


/*******************************************
// The TaskList constructor is responsible
// for initializing the size of the list,
// as well as the first task.
// 
// *****************************************/
TaskList::TaskList(void)
{
  task_count = 0;
  head = NULL;

  return;
}



/*******************************************
// TaskList copy constructor.
// 
// *****************************************/
TaskList::TaskList(const TaskList &tl)
{ return; }



/*******************************************
// The TaskList destructor is responsible
// for calling the destructor of the first
// Task object in the list. From the Task
// destructor, the Task's description is
// deallocated, and the next Task's
// destructor is called.
// 
// *****************************************/
TaskList::~TaskList(void)
{
  // Call the destructor of the first Task.
  delete head;
  return;
}



/*******************************************
// pushTask is used to insert a new task to
// the end of the TaskList object. This is
// not called in main (although I consciously
// kept the function public because it
// certainly could be), but rather, it is
// delegated to by newTask and readInTaskFile.
// 
// *****************************************/
void TaskList::pushTask(char *taskbuf)
{
  // Create a pointer to a new task object.
  Task *temp = new Task;
  // Set the new task object to store a task description of the length
  // of the description read in by the user, plus one for \0.
  temp->task = new char[strlen(taskbuf)+1];
  // Tasks are inserted at the end, so temp's next will be the last
  // Task in the list.
  temp->next = NULL;

  // Copy the contents of the task description buffer into the new task.
  strcpy(temp->task,taskbuf);

  // If the TaskList is empty..
  if(!head)
    // the new task is the only Task, and therefore, the list's label.
    head = temp;

  // Otherwise (there is already a label & possibly tasks)..
  else
  {
    // Create a pointer to head for traversal.
    Task *current = head;

    // Until the end of the list..
    while(current->next != NULL)
      // Set the current Task to the next task.
      current = current->next;

    // Set the new task to the end of the list.
    current->next = temp;
  }

  // Increment the task counter.
  ++task_count;
  return;
}



/*******************************************
// removeTask is used to remove the last
// Task from the list. If the list is empty,
// or there is only a label, the user is
// notified and the function returns false
// (to changes). Otherwise, the last Task
// is removed.
// 
// *****************************************/
bool TaskList::removeTask(void)
{
       // Perservation pointer.
  Task *temptwo = NULL,
       // Traversal pointer.
       *temp    = NULL;

  // If the list is empty, or there is only a label..
  if(!head || !head->next)
  {
    // Alert the user that no tasks can be removed.
    std::cout << "\nNo tasks to remove." << std::endl;
    // Return false to changes.
    return false;
  }

  // Set the first temporary pointer to head.
  temp = head;

  // While the Task after the Next task exists..
  while(temp->next->next)
    // Update the traversal pointer to the next task.
    temp = temp->next;

  // Store the last Task.
  temptwo = temp->next;
  // Deallocate the last Task.
  delete temptwo;
  // Set the last task to null.
  temp->next = NULL;

  // decrement the number of Tasks.
  --task_count;
  // set changes to true.
  return true;
}



/*******************************************
// This function is called by main to insert
// a new Task object to the list. The user
// is prompted to enter a description for
// the new task, their description is read
// in, and pushTask creates and adds a new
// Task with the description to the list.
// 
// *****************************************/
bool TaskList::newTask(void)
{
  // Buffer for the user's description of the Task.
  char *taskbuffer = new char[LINE_BUFFER_SIZE];

  // Prompt the user to enter their description of the Task.
  std::cout << "Please enter information for the new task." << std::endl
            << "Task: ";

  // Read in the user's description of the new Task.
  std::cin.get(taskbuffer, LINE_BUFFER_SIZE, '\n');
  std::cin.ignore(100,'\n');
  // Add the new Task to the end of the list.
  pushTask(taskbuffer);

  // Alert the user that a new Task has been added.
  std::cout << "\nNew task added!" << std::endl;

  // Return true to changes.
  return true;
}



/*******************************************
// editTask takes a number which corresponds
// to a Task in the list (previously selected
// by the user), traverses to that Task,
// and updates the user's description.
// 
// *****************************************/
bool TaskList::editTask(short task_selection)
{
  // List traversal counter.
  short task_counter = 0;
  // Buffer for the user's new Task description.
  char *updated_task = new char[LINE_BUFFER_SIZE];
  // temporary task pointer for traversal.
  Task *temp = NULL;

  // If the list is empty (or there is only a label), return false to changes.
  // Note that there is a separate function for modifying the label.
  if(!head || !head->next)
    return false;

  // Initialized temp to head->next because head contains the label.
  temp = head->next;

  // While there is a next task, and the user's selection has not been traversed
  // to.. (This is redundant, temp->next should absolutely be true. Still, because
  // the selection is passed in, it's worth having the redundancy).
  while(temp->next && task_selection > ++task_counter)
    // Update the temp pointer to the next Task object.
    temp = temp->next;

  // Prompt the user to enter updated information for the task.
  std::cout << "Please enter updated information for task #"
            << task_selection << '.' << std::endl << "Task: ";

  // Read in the user's updated Task description.
  std::cin.get(updated_task, LINE_BUFFER_SIZE, '\n');
  std::cin.ignore(100,'\n');

  // deallocate the previous description.
  delete[] temp->task;
  // set the updated Task description.
  temp->task = updated_task;

  // Alert the user that the task has been updated.
  std::cout << "\nTask updated!" << std::endl;

  // Return true to changes.
  return true;
}



/*******************************************
// editLabel is used to modify the user's
// description of the list. The label is
// stored in head.
// 
// *****************************************/
bool TaskList::editLabel(void)
{
  // buffer for the user's new label.
  char *updated_label = new char[LINE_BUFFER_SIZE];

  // If the list is empty, return false to changes.
  if(!head)
    return false;

  // Prompt the user to enter a new label.
  std::cout << "\nPlease enter an updated task label." << std::endl
            << "Label: ";

  // Read in the user's updated label.
  std::cin.get(updated_label, LINE_BUFFER_SIZE, '\n');
  std::cin.ignore(100,'\n');

  // Delete the previous label.
  delete[] head->task;
  // Update with the new label.
  head->task = updated_label;

  // Alert the user that the label has been updated.
  std::cout << "\nLabel updated!" << std::endl;

  // Return true to changes.
  return true;
}



/*******************************************
// displayTasks is used to display the whole
// TaskList.
// 
// *****************************************/
void TaskList::displayTasks(void)
{
  // If the list is empty, alert the user that there are no tasks to
  // display. Consider changing to !head || !head->next because if
  // !head->next. only a label will print.
  if(!head)
  {
    std::cout << "\nNo tasks to display!" << std::endl;
    return;
  }

  // Set a tempory Task pointer to head for traversal.
  Task *temp = head;
  // Counter for displaying the Tasks from 1 to length.
  short cnt = 0; // Starts at 0 because printed with ++.

  // Print out the label for the list.
  std::cout << '\n' << temp->task << ':' << std::endl;

  // Set the traversal pointer to the next Task.
  temp = temp->next;

  // While there are tasks to display..
  while(temp)
  {
    // Print out the current task.
    std::cout << "\n\tTask #" << ++cnt << " => " << temp->task << std::endl;
    // Set the traversal pointer to the next Task.
    temp = temp->next;
  }

  return;
}



/*******************************************
// displayTask, not to be confused with
// displayTasks, is used to print out a
// single task to the user.
// 
// *****************************************/
void TaskList::displayTask(void)
{
        // The user's selection for which task to print.
  short task_selection = 0,
        // The number of tasks in the list.
        task_size_sp   = (getSize()-1), // -1 because of label.
        // Counter for traversing the list.
        task_counter   = 0;

  // Temporary Task pointer for traversing the list.
  Task *temp = NULL;

  // If the list is empty, or if there is only a label..
  if(!head || !head->next)
    // Alert the user that there are no tasks to be displayed.
    std::cout << "\nThere are no tasks to display."  << std::endl;

  // Otherwise, if there is no Task after the next Task..
  else if(!head->next->next)
    // Alert the user that there is only one Task.
    std::cout << "\nThere is only one task to display.\n"  << std::endl
                 // Print the label.
              << head->task << "\n\n\tTask #1 "
                 // Print the only task.
              << head->next->task << std::endl;

  // Otherwise (there are options)..
  else
  {
    // While the user's entry for which task to print is invalid..
    while(task_selection < 1 || task_selection > task_size_sp)
    {
      // Print out the range of tasks.
      std::cout << "\nThere are " << task_size_sp << " tasks to display."  << std::endl
                  // Tell the user to enter the number of the task that they want displayed.
                << "Enter a number from 1 to " << task_size_sp << " to display a specific task." << std::endl
                  // Prompt for entry.
                << "Task # ";

      // Read in the user's selection
      std::cin >> task_selection;
      std::cin.ignore(100,'\n');

      // If the task is invalid
      if(task_selection < 1 && task_selection > task_size_sp)
        // Alert the user that the task was invalid. Try again.
        std::cout << "Invalid entry! " << std::endl;
    }

    // Initialized to head->next because head contains the label.
    temp = head->next;

    // While temp does not point to the user's selection
    while(temp->next && task_selection > ++task_counter)
      // set temp to the next Task.
      temp = temp->next;

    // Print out the user's selected Task description.
    std::cout << "\n  Task #" << task_selection << " => " << temp->task << std::endl;
  }

  return;
}



/*******************************************
// readInTaskFile is used to open a file,
// for which the name is passed in, and
// load its contents into the TaskList
// object. If the file does not exist, then
// the user is asked to initialize a new
// Task with a label. New files are not
// actually created until a user ends the
// program and confirms changes.
// 
// *****************************************/
bool TaskList::readInTaskFile(const char filename[])
{
  // Open the file with the passed filename for input.
  std::ifstream infile(filename);
  // Contains the current line being read in the file.
  char linebuffer[LINE_BUFFER_SIZE + 1];
  // True when the input file is empty.
  bool empty = infile.peek() == std::ifstream::traits_type::eof();

  // If the file is empty..
  if(empty)
  {

    // Alert the user that the file is empty, and to enter a new label.
    std::cout << "\nFile is empty. Please enter a title for the new task list."
                 // Prompt the user for a new TaskList label.
              << std::endl << "\nTitle: ";

    // Read in the user's new label
    std::cin.get(linebuffer, LINE_BUFFER_SIZE, '\n');
    std::cin.ignore(100,'\n');

    // Initialize a new TaskList with the new label.
    pushTask(linebuffer);
    // Close the file.
    infile.close();
    // Return true to changes.
    return true;
  }

  // Until the end of the file..
  while(!infile.eof())
  {
    // Read in a line until the '~' delimiter.
    infile.get(linebuffer, LINE_BUFFER_SIZE, '~');
    infile.ignore(100,'\n');

    // If the line starts with the opening delimiter '%'..
    if(linebuffer[0] == '%')
    {
      // Read in the next line until the '~' delimiter.. This line contains an actual
      // Task description. Note that a separate delimiter is not necessary for the label
      // and the Tasks. The first line of info is simply set as the label.
      infile.get(linebuffer, LINE_BUFFER_SIZE, '~');
      infile.ignore(100,'\n');

      // Insert the current description.
      pushTask(linebuffer);
    }
  }

  // Close the input file.
  infile.close();
  // Return false to changes.
  return false;
}



/*******************************************
// Writes the contents of the linked list
// of tasks to the external file.
// 
// *****************************************/
void TaskList::writeToTaskFile(const char filename[])
{
  // If the list is empty, return. Currently, the program will work just fine without
  // this because files are only written when additions/changes have been made,
  // however, I'm putting it here anyway in case of future modifications.
  if(!head)
    return;

  // Open and empty the external file.
  std::ofstream outfile(filename, std::ofstream::trunc);
  // Create a pointer to the first task.
  Task *temp = head;
  // Write an opening delimiter to the external file.
  outfile << "%%~\n" << temp->task << "~" << std::endl;

  // While the list is not empty, set temp to the next task.
  while(temp = temp->next)
    // Write the current task, preceeded by a task delimiter, to the file.
    outfile << "%T~\n" << temp->task << '~' << std::endl;

  // Everything has been written. Close the external file.
  outfile.close();

  return;
}



/*******************************************
// Returns the number of tasks(including the
// label) in the list.
// 
// *****************************************/
short TaskList::getSize(void)
{
  return task_count;
}



/*******************************************
// Task constructor currently does nothing.
// Here for future use.
// 
// *****************************************/
TaskList::Task::Task(void)
{ return; }



/*******************************************
// Task copy constructor currently does
// nothing. Here for future use.
// 
// *****************************************/
TaskList::Task::Task(const Task &task_copy)
{ return; }



/*******************************************
// Task destructor is responsible for
// deallocating the array containing the
// user's task definition, as well as
// calling the destructor of the next task.
// 
// *****************************************/
TaskList::Task::~Task(void)
{

  // Delete the contents of the user's task definition.
  delete[] task;
  // Call the destructor of the next task node.
  delete next;
  return;
}
