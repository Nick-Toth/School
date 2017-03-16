/*****************************************************************
// File Name:  Program3_NToth.cpp
// Created By: Nicholas Toth
// E-Mail:     ntoth@pdx.edu
// Date:       3/2/2017
// Class:      CS162
// Program #:  3
//
// Program Overview: The purpose of this program is to help the
// user keep track of jobs they are interested in, and interviews
// they have had. The user may store up to 20 each of jobs and
// interviews. When a user enters a job, they will be prompted
// for each of six fields to fill. For a new interview, the user
// will be asked to select an existing job from the job list
// to transfer over to the interviews list. Once a job has been
// transferred to the interviews list, the user will be asked
// to update their personal thoughts. In addition to adding
// jobs and interviews, the user will be allowed to print each
// list from the menu. Finally, the user's data will be saved to
// one of two external files (jobs.txt & interviews.txt).
//
// ***************************************************************/

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;


                     // The maximum size of a given line in the exernal files.
const unsigned short LINE_BUFFER_SIZE = 255,
                     // The maximum size of a given entry for a given Job/Interview.
                     MAX_ENTRY_SIZE   = 255,
                     // Max number of jobs / interviews ea.
                     MAX_ENTRIES      = 20;

           // The name of the external file containing job data
const char INTERVIEWS_FILENAME[] = "interviews.txt",
           // The name of the external file containing interview data
           JOBS_FILENAME[]       = "jobs.txt";



/*******************************************************
// Contains the data for a given job/interview.
// *****************************************************/
struct Info
{
    // Active when containing info
    bool isactive;
    // Name of the company for a given job/interview.
    char company_name[MAX_ENTRY_SIZE + 1];
    // The name of the position for a given job/interview.
    char job_title[MAX_ENTRY_SIZE + 1];
    // The number of hours worked per day for a given job.
    char hours_per_day[MAX_ENTRY_SIZE + 1];
    // The begining date for a given job.
    char start_date[MAX_ENTRY_SIZE + 1];
    // The ending date for a given job.
    char end_date[MAX_ENTRY_SIZE + 1];
    // Personal thoughts about a given job/interview.
    // This is the only thing that is updated when a job isactive
    // moved to the interview list.
    char personal_thoughts[MAX_ENTRY_SIZE + 1];
};



// FRONT END
// Greet user.
void beginProgram(void);
// Print out program options, read in selection.
short menuController(void);
// Gets a new job from the user.. Handles entries == 20.
short getNewJob(Info (&jobs)[MAX_ENTRIES], short job_cnt, bool &changes);
// Fills a job object with information from the user.
void fillJob(Info (&jobs)[MAX_ENTRIES], short job_cnt, short location);
// Gets a new interview from the user by selecting a job to transfer.. Handles entries == 20.
short getNewInterview(Info (&jobs)[MAX_ENTRIES], short &job_cnt, Info (&interviews)[MAX_ENTRIES], short interview_cnt, bool &changes);
// Transfers job contents to new interview. Prompts/reads in personal thoughts.
void fillInterview(Info (&jobs)[MAX_ENTRIES], short &job_cnt, Info (&interviews)[MAX_ENTRIES], short interview_cnt, short location);
// Prints out the contents of the job or interview array.
void printOutData(Info data[MAX_ENTRIES], short entry_cnt, const char type[]);
// Prompts the user to press enter to return to the menu.
void continueMenu(void);

// BACK END
// Reads in data from the external files. Writes data to respective object.
short readInData(Info data[MAX_ENTRIES], const char filename[]);
// Removes a job from the jobs list (user selects which job).
short removeJob(Info (&jobs)[MAX_ENTRIES], short job_cnt);
// Removes an interview from the interview list (user selects which interview).
short removeInterview(Info (&interviews)[MAX_ENTRIES], short interview_cnt);
// Enters specific data into the job/interview arrays from their respective external files.
void enterSpecificData(Info data[MAX_ENTRIES], short &entry_num, const char entry_type, char info[MAX_ENTRY_SIZE]);
// Asks the user to confirm any changes.
bool confirmWrite(void);
// Writes the data to an external file when the program is killed.
void writeInfo(Info data[MAX_ENTRIES], short entry_cnt, const char filename[]);
// Shifts down jobs/interviews after removal.
void shiftDown(Info data[MAX_ENTRIES], short entry_cnt, short openIndex);



int main(void)
{
        // Total current number of interviews entered.
  short interview_count = 0,
        // Menu option selected by the user.
        menu_selection = -1,
        // Total current number of jobs entered.
        job_count = 0;

       // True when the user has made changes. Used to determine
       // whether or not to write to the files on program termination.
  bool changes = false,
       // True when the user selects 4(quit) from the program menu.
       quit    = false;

  // Create array to contain all job entries.
  Info jobs[MAX_ENTRIES];
  // Create array to contain all interview entries.
  Info interviews[MAX_ENTRIES];

  // READ IN EXTERNAL DATA (jobs & interviews)
  // Read in jobs from external jobs file
  job_count = readInData(jobs, JOBS_FILENAME);
  // Read in interviews from external interviews file
  interview_count = readInData(interviews, INTERVIEWS_FILENAME);

  // Greets user.
  beginProgram();

  // While the user has not selected to quit the program..
  while(!quit)
  {
    // Print out the menu, read in the user's selected operation.
    menu_selection = menuController();
    // Execute user's selection.
    switch(menu_selection)
    {
      case 0: // Add a new job.
        job_count = getNewJob(jobs, job_count, changes);
        break;
      case 1: // Add a new interview.
        interview_count = getNewInterview(jobs, job_count, interviews, interview_count, changes);
        break;
      case 2: // Print out jobs.
        printOutData(jobs, job_count, "Job");
        break;
      case 3: // Print out interviews.
        printOutData(interviews, interview_count, "Interview");
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
      // Write the interviews to the interviews file.
      writeInfo(interviews, interview_count, INTERVIEWS_FILENAME);
      // Write the jobs to the jobs file.
      writeInfo(jobs, job_count, JOBS_FILENAME);
      // Alert the user that the changes have been made.
      cout << "\nChanges written!" << endl;
  }

  // The program is being terminated.
  cout << "\nGoodbye!\n" << endl;
  return 0; // Fin.
}



/*******************************************************
// Reads in any preexisting user information for jobs/
// interviews from the given file name. Saves the info
// to the data array.
// *****************************************************/
short readInData(Info data[MAX_ENTRIES], const char filename[])
{
  // Open the external file containing the information for any
  // saved jobs/interviews.
  ifstream infile(filename);

  // Contains the current line being read in the file.
  char linebuffer[LINE_BUFFER_SIZE + 1];
  // Updated with each char denoting the current input type
  char current_entry_type = ' ';

  // Counter for the number of entries in the external file.
  short entry_num = 0;

  // While the file contains things..
  while(!infile.eof())
  {
    // Get the first unread line of text from the input file.
    infile.get(linebuffer, LINE_BUFFER_SIZE, '~');
    infile.ignore(100,'\n');

    // If the line denotes a new job..
    if(linebuffer[0] == '%' && strlen(linebuffer) == 1)
      //cout << "New job" << endl;
      data[entry_num].isactive = true;

    // If the line denotes the end of the current job/interview entry..
    else if(linebuffer[0] == '%' && linebuffer[1] == '\\')
      // Increment the number of entries.
      ++entry_num;

    // If the line denotes a new data entry for current job/interview..
    else if(linebuffer[0] == '%' && strlen(linebuffer) == 2)
      // Store the char denoting the type of data contained in the file.
      current_entry_type = linebuffer[1];

    // Otherwise (the line contains the user's job/interview data)..
    else
      // Enter the data for the current line.
      enterSpecificData(data, entry_num, current_entry_type, linebuffer);
  }

  // Close the file from which the user's data has been read.
  infile.close();
  // Return the number of entries for the current type of data.
  return entry_num;
}



/*******************************************************
// Removes a job in the event that the job list is full,
// and the user elects to delete an entry. Returns the
// index of the new empty job object.
// *****************************************************/
short removeJob(Info (&jobs)[MAX_ENTRIES], short job_cnt)
{
  // The user's selection for which job to remove.
  short remove = 0;

  // Print out the list of jobs so that the user can
  // select a job to remove from the list.
  cout << "\nHere is a list of your job entries:" << endl;
  printOutData(jobs, job_cnt, "Job");

  do
  {
    // Prompt the user to select a job to remove.
    cout << "\nPlease enter the number corresponding to the entry you\'d like to remove: ";
    // Read in the user's selection.
    cin >> remove;
    cin.ignore(100,'\n');

    //If the user's entry is invalid.
    if(remove < 1 || remove > 20)
      // Print out that they user's entry is invalid.
      cout << "\nInvalid Entry! Number must be from 1 through 20." << endl;
  }

  // While the user's entry is invalid..
  while(remove < 1 || remove > 20);
  // Decrement the user's selection because jobs listed 1-20, not 0-19.
  --remove;
  // Reset the selected job object.
  jobs[remove] = Info();

  // Return the index of the removed job.
  return remove;
}



/*******************************************************
// Removes an interview in the event that the list of
// interviews is full, and the user elects to delete an
// entry. Returns the index of the new empty interview
// object.
// *****************************************************/
short removeInterview(Info (&interviews)[MAX_ENTRIES], short interview_cnt)
{
  // User's selection for which interview to remove.
  short remove = 0;

  // Print out the list of interviews so that the user can
  // select an interview to remove from the list.
  cout << "\nHere is a list of your interview entries:" << endl;
  printOutData(interviews, interview_cnt, "Interview");

  do
  {
    // Prompt the user to select an interview to remove.
    cout << "\nPlease enter the number corresponding to the entry you\'d like to remove: ";
    // Read in the user's selection & convert to uppercase.
    cin >> remove;
    cin.ignore(100,'\n');
    remove = toupper(remove);

    // If the user's entry is invalid..
    if(remove < 1 || remove > 20)
      // Alert the user that they have made an invalid entry.
      cout << "\nInvalid Entry! Number must be from 1 through 20." << endl;
  }
  // While the user's entry is invalid..
  while(remove < 1 || remove > 20);

  --remove;
  // Reset the selected interview object.
  interviews[remove] = Info();

  // Return the index of the removed interview.
  return remove;
}



/*******************************************************
// Currently, the only purpose of this function is to
// greet the user. Obviously this could have simply been
// accomplished in main, but I figure that having this
// function will allow for easier modifications in the
// future.
// *****************************************************/
void beginProgram(void)
{
  cout << "\nWelcome to your personal job & interview log." << endl;

  return;
}



/*******************************************************
// Prints out the program menu and reads in a selection
// from the user. Selection returned to main.
// *****************************************************/
short menuController(void)
{
  // The user's selection from the menu.
  short selection = -1;

  // Print out the menu options
  cout << "\nPlease select an option from the following menu" << endl
       // The user will enter 0 to add a new job.
       << "\n  [0] => Add a new job." << endl
       // The user will enter 1 to add a new interview.
       << "  [1] => Add a new interview." << endl
       // The user will enter 2 to ptint out the contents
       // of the job array.
       << "  [2] => Print out job records." << endl
       // The user will enter 3 to print out the contents
       // of the interviews array.
       << "  [3] => Print out interview records" << endl
       // The user will enter 4 to terminate the program.
       // If changes were made, the user will subsequently
       // be prompted to confirm those changes.
       << "  [4] => Quit the program" << endl;

  //While the user's selection is invalid (or first attempt..
  while(selection < 0 || selection > 4)
  {
    // Prompt the user to enter their menu selection.
    cout << "\nEnter the number corresponding to your selection: ";
    // Read in the user's selection
    cin >> selection;
    cin.ignore(100,'\n');
    // If the user has entered an invalid selection..
    if(selection < 0 || selection > 4)
      // Alert the user that their entry was invalid.
      cout << "\nInvalid entry! Please enter a number from 0 through 4." << endl;
  }
  // Return the selection of the user to main.
  return selection;
}



/*******************************************************
// Prompt the user to fill in a job object. jobs -> job
// entries. job_cnt -> # of jobs created. changes is
// made true if a job is added.
// *****************************************************/
short getNewJob(Info (&jobs)[MAX_ENTRIES], short job_cnt, bool &changes)
{
  // The user's entry for whether or not to remove a job in the
  // event that the job list is full.
  char confirmation = ' ';
  // The user's entry for which job job to remove in the event
  // that the job list is full.
  short remove = 0;

  // If the list of jobs is full..
  if(job_cnt > 19)
  {
    // Print out that the job list memory is full, and that they
    // may either select a job to delete, or return to the menu.
    cout << "\nJob memory full!" << endl
         << "You can either select a job to delete, or return to the menu." << endl;

    // While the user's entry is invalid (or first attempt)..
    while(confirmation != 'Y' && confirmation != 'N')
    {
      // Prompt the user to confirm whether or not they want to
      // remove a job.
      cout << "\nWould you like to select a job to delete?(Y/N): ";
      // Read in the user's response.
      cin >> confirmation;
      cin.ignore(100,'\n');
      confirmation = toupper(confirmation);

      // If the user's entry is invalid..
      if(confirmation != 'Y' && confirmation != 'N')
        // Print out that they user has made an invalid entry.
        cout << "\nInvalid entry! Please enter Y for Yes or N for No." << endl;
    }
    // If the user wants to select a job to remove..
    if(confirmation == 'Y')
      // Remove a job.
      remove = removeJob(jobs, job_cnt);
    // Otherwise
    else
      // Return the unchanges job count.
      return job_cnt;
  }
  // Confirm that changes have been made.
  changes = true;
  // If a job was deleted..
  if(confirmation == 'Y')
  {
    // Shift the jobs down from the empty job object to the end of the list.
    shiftDown(jobs,job_cnt, remove);
    // Fill the last job object in the list.
    fillJob(jobs, job_cnt, 19);
  }
  // Otherwise(no job was removed)..
  else
  {
    // Fill the next unused job object
    fillJob(jobs, job_cnt, job_cnt);
    // Increment the job count. Unnecessary in the prior condition
    // because a job is removed & added. Balances out.
    ++job_cnt;
  }

  // Return the updated job count.
  return job_cnt;
}


/*******************************************************
// Fills the next job object in the list of jobs with
// entries by the user. location = the location of the
// next empty job object.
// *****************************************************/
void fillJob(Info (&jobs)[MAX_ENTRIES], short job_cnt, short location)
{
  // Set the current job to active.
  jobs[location].isactive = true;

  // Prompt the user to enter the company name for the job.
  cout << "Please enter the company name for your new job entry." << endl
       << "Company Name: ";
  // Read in the user's entry for the company name.
  cin.get(jobs[location].company_name, MAX_ENTRY_SIZE, '\n');
  cin.ignore(100,'\n');

  // Prompt the user to enter the job title for the job.
  cout << "Please enter the job title for your new job." << endl
       << "Job Title: ";
  // Read in the user's entry for the job title.
  cin.get(jobs[location].job_title, MAX_ENTRY_SIZE, '\n');
  cin.ignore(100,'\n');

  // Prompt the user to enter the number of working hours per day for the job.
  cout << "Please enter the number of hours per day for your new job." << endl
       << "Hours: ";
  // Read in the user's entry for the job's hours.
  cin.get(jobs[location].hours_per_day, MAX_ENTRY_SIZE, '\n');
  cin.ignore(100,'\n');

  // Prompt the user to enter the start date for the job.
  cout << "Please enter the start date for your new job." << endl
       << "Start Date: ";
  // Read in the user's entry for the job's start date.
  cin.get(jobs[location].start_date, MAX_ENTRY_SIZE, '\n');
  cin.ignore(100,'\n');

  // Prompt the user to enter the end date for the job.
  cout << "Please enter the end date for your new job." << endl
       << "End Date: ";
  // Read in the user's entry for the job's end date.
  cin.get(jobs[location].end_date, MAX_ENTRY_SIZE, '\n');
  cin.ignore(100,'\n');

  // Prompt the user to enter personal thoughts about the job.
  cout << "Please enter any personal thoughts for your new job." << endl
       << "Personal Thoughts: ";
  // Read in the user's personal thoughts for the job.
  cin.get(jobs[location].personal_thoughts, MAX_ENTRY_SIZE, '\n');
  cin.ignore(100,'\n');

  return;
}



/*******************************************************
// Transfers a job into the interviews array and updates
// personal thoughts. Requires BOTH the jobs and
// interviews lists because a job will be selected for
// transfer into the interviews list. Location is the
// index in which a new interview will be placed.
// Changes is used to keep track of whether the list
// of interviews has been changed.
// *****************************************************/
short getNewInterview(Info (&jobs)[MAX_ENTRIES], short &job_cnt, Info (&interviews)[MAX_ENTRIES], short interview_cnt, bool &changes)
{
  // The user's entry for whether or not to remove an interview
  // in the event that they list of interviews is full.
  char confirmation = ' ';
  // The user's selection for which interview to remove
  // in the event that the list of interviews is full.
  short remove = 0;

  // If the list of interviews is full..
  if(interview_cnt > 19)
  {
    // Print out that the interview memory is full, and that they can
    // select an interview to remove, or return to the menu.
    cout << "\nInterview memory full!" << endl
         << "You can either select an interview to delete, or return to the menu." << endl;

    // While the user has not entered a valid entry for whether or
    // not to remove an interview..
    while(confirmation != 'Y' && confirmation != 'N')
    {
      // Prompt the user to make a selection
      cout << "\nWould you like to select an interview to delete?(Y/N): ";

      // Read in the user's response.
      cin >> confirmation;
      cin.ignore(100,'\n');
      confirmation = toupper(confirmation);

      // If the user's entry is invalid..
      if(confirmation != 'Y' && confirmation != 'N')
        // Tell the user to try again.
        cout << "\nInvalid entry! Please enter Y for Yes or N for No." << endl;
    }
    // If the user has confirmed that they want to
    // select an interview to remove..
    if(confirmation == 'Y')
      // Remove an interview and get the index of the removed interview.
      remove = removeInterview(interviews, interview_cnt);
    // Otherwise
    else
      // Return the unchanges number of interviews.
      return interview_cnt;
  }

  // An interview will be added, confirm changes.
  changes = true;
  // If an interview has been removed..
  if(confirmation == 'Y')
  {
    // Shift down the interview after the removed interview.
    shiftDown(interviews,interview_cnt, remove);
    // Fill the final (only empty) interview object.
    fillInterview(jobs, job_cnt, interviews, interview_cnt, 19);
  }
  // Otherwise, no interview was removed..
  else
  {
    // Fill the next open interview object.
    fillInterview(jobs, job_cnt, interviews, interview_cnt, interview_cnt);
    // Increment the number of interviews. This is not necessary
    // in the prior condition because an interview was removed and added
    // which balances out.
    ++interview_cnt;
  }

  // return the updated interview count.
  return interview_cnt;
}

/*******************************************************
// Fill the next interview object in the interviews list
// with the contents of a selected job. Read in updated
// personal thoughts from the user. Requires BOTH the
// jobs and interviews lists because a job will be
// selected for transfer into the interviews list.
// Location is the index in which a new interview will
// be placed.
// *****************************************************/
void fillInterview(Info (&jobs)[MAX_ENTRIES], short &job_cnt, Info (&interviews)[MAX_ENTRIES], short interview_cnt, short location)
{
  // The user's selection for which job to transfer.
  short job_to_trans = 0;

  // Print out the list of jobs for the user to select a job to move into the interviews list.
  cout << "Please select a job from the following list to transfer to the interviews list." << endl;
  printOutData(jobs, job_cnt, "Job");

  do
  {
    // Prompt the user to enter a number corresponding to the job they would like to transfer..
    cout << "\nPlease enter the number corresponding to the job you'd like to transfer to the interview list: ";
    // Read in the user's entry for which job to transfer.
    cin >> job_to_trans;
    cin.ignore(100,'\n');

    // If the user's entry for which job to transfer is incorrect..
    if(job_to_trans < 1 || job_to_trans > 20)
      // Prompt the user to try again.
      cout << "\nInvalid Entry! Number must be from 1 through 20." << endl;
  }
  // While the entry is invalid.
  while(job_to_trans < 1 || job_to_trans > 20);

  // Because the user was prompted to enter from 1 to 20, decrement
  // for array access (0-19).
  --job_to_trans;

  // Set the current interview to active..
  interviews[location].isactive = true;
  // Copy the company name from the job list to the interview list.
  strcpy(interviews[location].company_name, jobs[job_to_trans].company_name);
  // Copy the job title from the job list to the interview list.
  strcpy(interviews[location].job_title, jobs[job_to_trans].job_title);
  // Copy the hours/day from the job list to the interview list.
  strcpy(interviews[location].hours_per_day, jobs[job_to_trans].hours_per_day);
  // Copy the start date from the job list to the interview list.
  strcpy(interviews[location].start_date, jobs[job_to_trans].start_date);
  // Copy the end date from the job  list to the interview list.
  strcpy(interviews[location].end_date, jobs[job_to_trans].end_date);

  // Reset the transferred job.
  jobs[job_to_trans] = Info();
  // Shift the jobs down to prevent misc empties in the job list.
  shiftDown(jobs, job_cnt, job_to_trans);

  // Prompt for, read in the user's personal thoughts for the interview.
  cout << "Please enter any personal thoughts for your new interview entry." << endl
       << "Personal Thoughts: ";
  cin.get(interviews[location].personal_thoughts, MAX_ENTRY_SIZE, '\n');
  cin.ignore(100,'\n');

  // Decrement the number of job entries.
  --job_cnt;

  return;
}


/*******************************************************
// Fills the passed array with the contents of a file.
// data = array of jobs/interviews. entry_num = counter
// for entries. entry type = type of info.. i.e. 'C' ->
// company name. info = contents of the file for the
// current entry.
// *****************************************************/
void enterSpecificData(Info data[MAX_ENTRIES], short &entry_num, const char entry_type, char info[MAX_ENTRY_SIZE])
{
  // Switch the character representing the type of entry..
  switch(entry_type)
  {
    // If the current entry is an 'N' (Co. Name)..
    case 'N':
      // Copy the contents of the company name from the file
      // into the current entry of the list.
      strcpy(data[entry_num].company_name, info);
      break;

    // If the current entry is a 'T' (Job title)..
    case 'T':
      // Copy the contents of the job title from the file
      // into the current entry of the list
      strcpy(data[entry_num].job_title, info);
      break;

    // If the current entry is an 'H' (Hours/dat)..
    case 'H':
      // Copy the contents of the hours from the file
      // into the current entry of the list.
      strcpy(data[entry_num].hours_per_day, info);
      break;

    // If the current entry is an 'S' (Start date)..
    case 'S':
      // Copy the contents of the start date from the file
      // into the current entry of the list.
      strcpy(data[entry_num].start_date, info);
      break;

    // If the current entry is an 'E' (End date)..
    case 'E':
      // Copy the contents of the end date from the file
      // into the current entry of the list.
      strcpy(data[entry_num].end_date, info);
      break;

    // If the current entry is a 'P' (Personal thoughts)..
    case 'P':
      // Copy the contents of the user's personal thoughts
      // from the file into the current entry of the list.
      strcpy(data[entry_num].personal_thoughts, info);
      break;

    // The user messed with the file and created an invalid entry type.
    // Stupid user..
    default:
      // Tell the user that there was an error.
      cerr << "ERROR!!! CEASE ALL MOTOR FUNCTIONS!!! ACK!!" << endl;
      break;
  }
  return;
}



/*******************************************************
// Prints out the contents of the passed list (jobs/
// interviews). data = jobs/interviews. entry_cnt = the
// number of entries in the list. type contains the type
// of data that was passed, so that it can be printed in
// a label to the user.
// *****************************************************/
void printOutData(Info data[MAX_ENTRIES], short entry_cnt, const char type[])
{
  // Index counter for traversing the list of jobs/interviews.
  short index = 0;
  // Print some whitespace.
  cout << endl;
  // For every entry in the given list of jobs/interiews..
  for(; index < entry_cnt; ++index)
    // If the current entry is active..
    if(data[index].isactive)
              // Print out the type & entry number(1 to 20).
      cout << type << " entry #" << (index + 1) << endl << endl
              // Print out the company name.
           << "\tCompany name:  " << data[index].company_name << endl
              // Print out the job title.
           << "\tJob title:     " << data[index].job_title << endl
              // Print out the # of hours per day for the current job.
           << "\tHours per day: " << data[index].hours_per_day << endl
              // Print out the start date for the job.
           << "\tStart date:    " << data[index].start_date << endl
              // Print out the end date for the job.
           << "\tEnd date:      " << data[index].end_date << endl
              // Print out the user's personal thoughts about the job/interview.
           << "\tThoughts:      " << data[index].personal_thoughts << endl
           << endl;
  return;
}



/*******************************************************
// Prompts the user to return to the menu between
// operations. For example, suppose the user asks to
// print out the contents of the list of jobs.. it would
// look ugly if the jobs were printed, and the menu was
// printed immediately after.
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
// Prompts the user to confirm any changes made during
// the life of the program. Reads in their answer and
// returns true or false to main, which then decides
// whether to write or not.
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



/*******************************************************
// Called after the user confirms any changes made
// during the life of the program. Called separately
// for job & interview lists. data = list being written,
// entry_cnt = # of currently active entries. filename
// is the name of the file corresponding to the data
// currently being written.
// *****************************************************/
void writeInfo(Info data[MAX_ENTRIES], short entry_cnt, const char filename[])
{
  // Open the file corresponding to the type of data being written.
  ofstream outfile(filename, ofstream::trunc);

  // Index counter for traversing the jobs/interviews.
  short index = 0;

  // For every entry in the given list of jobs/interviews..
  for(; index < entry_cnt; ++index)
      //If the current entry is active..
    if(data[index].isactive)
              // For the current job/interview..
              // Write a new entry delimiter
      outfile << "%~"    << endl
              // Write the contents of the company name.
              << "%N~\n" << data[index].company_name      << '~' << endl
              // Write the contents of the job title.
              << "%T~\n" << data[index].job_title         << '~' << endl
              // Write the contetns of the daily # of hours worked.
              << "%H~\n" << data[index].hours_per_day     << '~' << endl
              // Write the contents of the job's start date.
              << "%S~\n" << data[index].start_date        << '~' << endl
              // Write the content of the job's end date.
              << "%E~\n" << data[index].end_date          << '~' << endl
              // Write the contents of the user's personal thoughts.
              << "%P~\n" << data[index].personal_thoughts << '~' << endl
              // Write an end of entry delimiter.
              << "%\\~"  << endl;

  // Everything has been written. Close the external file.
  outfile.close();

  return;
}



/*******************************************************
// Shifts back data when a datum is removed.
// Called by both remove job and remove interview, where
// data = whichever. entry_cnt = the number of entries
// in the given array of jobs/interviews. open_index is
// the first empty index in the array.
// *****************************************************/
void shiftDown(Info data[MAX_ENTRIES], short entry_cnt, short open_index)
{
  // Index counter for traversing the array of jobs/interviews.
  short index = open_index;

  // For every job/interview from the open index to the end -1..
  for(;index < MAX_ENTRIES - 1; ++index)
    // Shift the subsequent job/interview back 1.
    data[index] = data[index + 1];
  // Set the last element in the array to inactive.
  data[MAX_ENTRIES - 1].isactive = false;

  return;
}
