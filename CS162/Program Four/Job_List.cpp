/*****************************************************************
// File Name:  Job_List.cpp
// Created By: Nicholas Toth
// E-Mail:     ntoth@pdx.edu
// Date:       3/10/2017
//
// Overview: This is the implementation file for the Job_List 
// class. Essentially, this file is responsible for defining the
// operations that can be performed on each of a list of some  
// user's job/internship related information.
//
// ***************************************************************/

#include "Job_List.h"


/*******************************************************
// Job_List conteuctor without auto load for file.
//
// *****************************************************/
Job_List::Job_List(void)
{
  // Initialize job array. 
  jobs = NULL;
  // Job contains no entries.
  counter = 0;
  // Job list takes up no memory yet.
  bytes = 0;

  return;
}



/*******************************************************
// Job_List constructor for automatically opening some 
// file (filename) and loading/initializing it's contents.
//
// *****************************************************/
Job_List::Job_List(const char filename[])
{
  // Call the default job constructor to setup class fields.
  Job_List();
  // Load/create the passed file.
  Load(filename);

  return; 
}



/*******************************************************
// Job_List copy constructor for constructing copies.
//
// *****************************************************/
Job_List::Job_List(const Job_List &j_list)
{ return; }



/*******************************************************
// Job_List destructor deallocates job array contents.
//
// *****************************************************/
Job_List::~Job_List(void)
{
  delete[] jobs;
}



/*******************************************************
// Function for creating a new job object. Also used to 
// initialize a job when a new file is created.
// 
// *****************************************************/
void Job_List::Read(void)
{
  // Index counter for traversing job data.
  short index = 0;
  
  // If the current file is not new..
  if(jobs[0].isactive)
  {
    // Temporary job array for resizing jobs array.
    Job *temp = new Job[counter];

    // put the contents of jobs into temp.
    for(; index < counter; ++index)
      temp[index] = jobs[index];
  
    // Reset jobs array to size + 1.
    delete[] jobs;
    jobs = new Job[counter + 1];

    // put the contents of temp back into jobs.
    for(index = 0; index < counter; ++index)
      jobs[index] = temp[index];
    
    // Set the new job to active.
    jobs[counter].isactive = true;

    // Delete the temporary job array.
    delete[] temp;
  }

  // Set the current job to active.
  jobs[0].isactive = true;

  // For every field in the Job struct..
  for(index = 0; index < 6; ++index)
    // Set the field for the new job.
    editField(counter, index);
  
  // Increment the job entry counter.
    ++counter;

  // Save the memory size of the jobs array.
  bytes = sizeof(*jobs);

  return;
}



/******************************************************* 
// Display all of the jobs containd in the array of 
// jobs. User is first prompted to select a job to 
// print incase there are several files open.
//
// *****************************************************/
void Job_List::Display(void)
{
  // Index counter for traversing the list of jobs/interviews.
  short index = 0;
  // Print some whitespace.
  std::cout << std::endl;
  // For every entry in the given list of jobs/interiews..
  for(; index < counter; ++index)
    // If the current entry is active..
    if(jobs[index].isactive)
      std::cout 
        // Print out the type & entry number(1 to 20).
        << "Entry #" << (index + 1) << std::endl << std::endl
        // Print out the company name.
        << "\tCompany name:  " << jobs[index].company_name << std::endl
        // Print out the job title.
        << "\tJob title:     " << jobs[index].job_title << std::endl
        // Print out the # of hours per day for the current job.
        << "\tHours per day: " << jobs[index].hours_per_day << std::endl
        // Print out the start date for the job.
        << "\tStart date:    " << jobs[index].start_date << std::endl
        // Print out the end date for the job.
        << "\tEnd date:      " << jobs[index].end_date << std::endl
        // Print out the user's personal thoughts about the job/interview.
        << "\tThoughts:      " << jobs[index].personal_thoughts << std::endl
        << std::endl;

  return;
}



/*******************************************************
// Edit is the function responsible for modifying a job
// in the jobs array. Once the user selects Edit from 
// the program's main menu, they will be asked to select
// a job object from the list of loaded files. Next, 
// the Edit function is called, which gets the user's
// selection for which job to edit, and which field to
// edit within that job. 
// 
// *****************************************************/
void Job_List::Edit(void)
{
       // Company name for the job that the user will edit.
  char *job_name = new char[MAX_ENTRY_SIZE],
       // The user's decision to try again if a job is not found.
       again = 'Y';

        // Index counter for displaying jobs & finding a job.
  short index = 0,
        // The user's selection for which job to edit.
        edit_job = -1,
	// The type of job data that the user will edit.
        edit_field = -1;
  
  // Print out the list of jobs for the user make a selection.
  std::cout << "Here is a list of all company names in your entries.\n" << std::endl;
  for(index = 0; index < counter; ++index)
    std::cout << "\t#" << (index+1) << " => " << jobs[index].company_name << std::endl;
  
  do
  {
    // Prompt the user to enter a company name for
    // some job in the jobs array.
    std::cout << "\nPlease enter the company name for"
              << " the job that you would like to edit." << std::endl
              << "\nEnter job name: ";

    // Read in the user's job selection.
    cin.get(job_name, MAX_ENTRY_SIZE, '\n');
    cin.ignore(100,'\n');
  
    // Find the job with the company name given by the user.
    for(index = 0; index < counter; ++index)
      if(!strcmp(jobs[index].company_name , job_name))
        // Save job index.
        edit_job = index;
    
    // If name entry does not correspond to a valid job..
    if(edit_job == -1)
    {
      // Alert the user that their entry was invalid.
      std::cout << "\nJob not found." << std::endl
                   // Prompt the user to try again.
                << "Would you like to try again?(Y/N): ";
      // Read in the user's decision.
      cin >> again;
      cin.ignore(100,'\n');
      again = toupper(again);
    }
  }

  // Do until entry is valid or user cancels.
  while(edit_job < 0 && again == 'Y');

  if(again == 'Y')
  {
    // Get field to edit. 
    edit_field = jobSelectMenu();
    // Edit selected field.
    editField(edit_job, edit_field);
  
    // If the user did not select to try again.
    if(edit_field != 6)
      // Alert the user that their changes were written.
      std::cout << "Changes written." << std::endl;

    // Update the size of the jobs memory.
    bytes = sizeof(*jobs);
  }

  // Deallocate job name memory.
  delete[] job_name;

  return;
}



/*******************************************************
// jobSelectMenu is a menu controlled for getting the 
// user's selection for which field to edit after 
// selecting Edit from the main menu. After a selection
// is made, a number is returned which represents the
// user's selection. This function is only called 
// internally.
//
// *****************************************************/
short Job_List::jobSelectMenu()
{
  // The user's selection from the menu.
  short selection = -1;

  // Print out the menu options
  std::cout << "\nPlease select an option from the following menu" << std::endl
       // Prompt the user to update the company name.
       << "\n  [1] => Edit company name." << std::endl
       // Prompt the user to update the job title.
       << "  [2] => Edit job title." << std::endl
       // Prompt the user to update the job hours.
       << "  [3] => Edit hours." << std::endl
       // Prompt the user to update the start date.
       << "  [4] => Edit start date." << std::endl
       // Prompt the user to update the end date.
       << "  [5] => Edit end date" << std::endl
       // Prompt the user to update their personal thoughts.
       << "  [6] => Edit personal thoughts." << std::endl 
       // Prompt the user to cancel editing.
       << "  [7] => Cancel edit." << std::endl;

  //While the user's selection is invalid (or first attempt..
  while(selection < 1 || selection > 7)
  {
    // Prompt the user to enter their menu selection.
    cout << "\nEnter the number corresponding to your selection: ";
    // Read in the user's selection
    cin >> selection;
    cin.ignore(100,'\n');
    // If the user has entered an invalid selection..
    if(selection < 1 || selection > 6)
      // Alert the user that their entry was invalid.
      cout << "\nInvalid entry! Please enter a number from 1 through 7." << endl;
  }
  // Return the selection of the user to main.
  // Subtract 1 because options start at 1, not 0.
  return selection - 1;
}



/*******************************************************
// editField is called when editing a single field of 
// a single job object. This occurs when a new job is 
// created (each field is filled one at a time), and 
// when the user decides to edit some field in a pre-
// existing job object. edit_job is the index of the 
// job in jobs that will be modified. edit_field 
// represents the field that will be modified in a job.
// This function is only called internally.
//
// *****************************************************/
void Job_List::editField(short edit_job, short edit_field)
{ 
  // Points to the field in some job that will be edited.
  char *job_field;

  // Print some whitespace.
  std::cout << std::endl;

  switch(edit_field)
  {
    // Change company name.
    case 0:
      // Prompt the user to enter the company name for the job.
      std::cout << "Please enter the company name for your new job entry." << std::endl
                << "Company Name: ";
      job_field = jobs[edit_job].company_name;
      break;
    // Change job title.
    case 1:
      // Prompt the user to enter the job title for the job.
      std::cout << "Please enter the job title for your new job." << std::endl
                << "Job Title: ";
      job_field = jobs[edit_job].job_title;
      break;
    // Change job hours per day.
    case 2:
      // Prompt the user to enter the number of working hours per day for the job.
      std::cout << "Please enter the number of hours per day for your new job." << std::endl
                << "Hours: ";
      job_field = jobs[edit_job].hours_per_day;
      break;
    // Change job start date.
    case 3:
      // Prompt the user to enter the start date for the job.
      std::cout << "Please enter the start date for your new job." << std::endl
                << "Start Date: ";
      job_field = jobs[edit_job].start_date;
      break;
    // Change job end date.
    case 4:
      // Prompt the user to enter the end date for the job.
      std::cout << "Please enter the end date for your new job." << std::endl
                << "End Date: ";
      job_field = jobs[edit_job].end_date;
      break;
    // Change personal thoughts about job.
    case 5:
      // Prompt the user to enter personal thoughts about the job.
      std::cout << "Please enter any personal thoughts for your new job." << std::endl
                << "Personal Thoughts: ";
      job_field = jobs[edit_job].personal_thoughts;
      break;
    // Cancel edit.
    case 6:
      std::cout << "Edit canceled. Returning to menu." << std::endl;
      break;
  }

  // If the user did not select to cancel..
  if(edit_field != 6)
  {
    // Read in the new information. Store in corresponding field.
    cin.get(job_field, MAX_ENTRY_SIZE, '\n');
    cin.ignore(100, '\n');
  }

  return;
}



/*******************************************************
// Called when the user chooses to load a preexisting 
// file, or to create a new file. If a file with the 
// given file name exists in the same directory as this
// program, that file is opened, and its contents are 
// written to the jobs array. If a new file is opened, 
// the user will be asked to initialize the first job 
// object in the jobs list.
//
// *****************************************************/
bool Job_List::Load(const char filename[])
{
  // Open the external file containing the information for any
  // saved jobs/interviews.
  ifstream infile(filename);
 
  // True when the input file is empty.
  bool empty = infile.peek() == std::ifstream::traits_type::eof();

  // If the file is empty..
  if(empty)
  {
    // Alert the user that the file is empty and
    // that a new file will need to be initialized.
    std::cout << "File is empty. Please initialize.." << std::endl;
    // Set the jobs array to contain a single job.
    jobs = new Job[1];
    // Initialize the new job.
    Read();
    // Close the input file.
    infile.close();
    // Return to the program's main menu.
    return true;
  }
    
  // Contains the current line being read in the file.
  char linebuffer[LINE_BUFFER_SIZE + 1];
  // Updated with each char denoting the current input type
  char current_entry_type = ' ';
  // Pointer to the current entry in the current Job 
  // for transfer from the input file.
  char *location;

  // Counter for the number of entries in the external file.
  short entry_num = 0;
  
  // temp is a buffer for the jobs array.
  Job *temp = new Job[MAX_LOAD_SIZE];

  // While the file contains things..
  while(!infile.eof())
  {
    // Get the first unread line of text from the input file.
    infile.get(linebuffer, LINE_BUFFER_SIZE, '~');
    infile.ignore(100,'\n');

    // If the line denotes a new job..
    if(linebuffer[0] == '%' && strlen(linebuffer) == 1)
      temp[entry_num].isactive = true;

    // If the line denotes the end of the current job/interview entry..
    else if(linebuffer[0] == '%' && linebuffer[1] == '\\')
      // Increment the number of entries.
      ++entry_num;

    // If the line denotes a new data entry for current job/interview..
    else if(linebuffer[0] == '%' && strlen(linebuffer) == 2)
      // Store the char denoting the type of data contained in the file.
      current_entry_type = linebuffer[1];
    
    // Otherwise (The line contains the user's data)
    else
    {
      // Switch the input type from the delimiter.
      // Set location pointer to the correct data.
      switch(current_entry_type)
      {
        // The data is a company name.
        case 'N':
          location = temp[entry_num].company_name;
          break;
        // The data is a job title.
        case 'T':
          location = temp[entry_num].job_title;
          break;
        // The data is some number of hours per day.
        case 'H':
          location = temp[entry_num].hours_per_day;
          break;
        // The data is a start date.
        case 'S':
          location = temp[entry_num].start_date;
          break;
        // The data is an end date.
        case 'E':
          location = temp[entry_num].end_date;
          break;
        // The data is the user's personal thoughts
        case 'P':
          location = temp[entry_num].personal_thoughts;
          break;
      }
      // Copy the contents of the file data to the decided location of the list.
      strcpy(location, linebuffer);
    }
  }

  // Close the file from which the user's data has been read.
  infile.close();

  // Set the Job_List object's size in number of entries.
  counter = entry_num;
  // Allocate memory for jobs equal to the number of entries made
  // as well as one additional empty entry.
  jobs = new Job[counter + 1];

  // For every job in the loaded jobs..
  for(entry_num = 0; entry_num < counter; ++entry_num)
    // Transfer the jobs from the file into the Job_List's array of jobs.
    jobs[entry_num] = temp[entry_num];
  
  // Set the Job_List object's size in bytes.
  bytes = sizeof(*jobs);
  //std::cout << "\n\t\nJ_BYTES: " << bytes << std::endl;
  
  // Deallocate the job buffer.
  delete[] temp;

  // False is returned because the file is not new.
  return false;
}



/*******************************************************
// Save is called after the user confirms any changes
// made during the life of the program. filename is the
// name of the file to which the contents of this class
// are written. It's worth noting that the file is 
// cleared before the class is written.
//
// *****************************************************/
void Job_List::Save(const char filename[])
{
  // Open the file corresponding to the type of data being written.
  ofstream outfile(filename, ofstream::trunc);

  // Index counter for traversing the jobs/interviews.
  short index = 0;

  // For every entry in the given list of jobs/interviews..
  for(; index < counter; ++index)
      //If the current entry is active..
    if(jobs[index].isactive)
              // For the current job/interview..
              // Write a new entry delimiter
      outfile << "%~"    << endl
              // Write the contents of the company name.
              << "%N~\n" << jobs[index].company_name      << '~' << endl
              // Write the contents of the job title.
              << "%T~\n" << jobs[index].job_title         << '~' << endl
              // Write the contetns of the daily # of hours worked.
              << "%H~\n" << jobs[index].hours_per_day     << '~' << endl
              // Write the contents of the job's start date.
              << "%S~\n" << jobs[index].start_date        << '~' << endl
              // Write the content of the job's end date.
              << "%E~\n" << jobs[index].end_date          << '~' << endl
              // Write the contents of the user's personal thoughts.
              << "%P~\n" << jobs[index].personal_thoughts << '~' << endl
              // Write an end of entry delimiter.
              << "%\\~"  << endl;

  // Everything has been written. Close the external file.
  outfile.close();

  return;
}
