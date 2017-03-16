/* ******************************************************************
// File Name:  Program1_NToth.cpp
// Created By: Nicholas Toth
// E-Mail:     ntoth@pdx.edu
// Date:       January 31st, 2017
// Class:      CS162
// Program #:  1
//
// Program Overview: Essentially, this program is meant to aid
// some user in analyzing their exercise habits. Given the
// user's height, pace and one or more data entries representing
// some measure of exercise progress (minutes exercised or steps
// taken), the program will calculate and output the number of
// miles a user has exercised for each data entry, as well as
// the average time spent exercising for a given session.
//
// ****************************************************************/

#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdlib>

using namespace std;


/* ****************************************************************
// Contains all exercise data entered by the user, as well as the
// types of data entered.
// ****************************************************************/
struct Data
{

    /* ****************************************************************
    // Constructor takes and sets exercise entries, size, and the
    // order of the exercise entry types.
    // ****************************************************************/
    Data(short data_size, char* entry_order): data_size(data_size),
                                              entry_order(entry_order)
    {
        data_entries = new int[data_size];
    }



    /* ****************************************************************
    // Copy constructor. For constructing copies.
    // ****************************************************************/
    Data(const Data &data)
    { }



    /* ****************************************************************
    // Destructor destructs Data object using destruction.
    // Destructively.
    // ****************************************************************/
    ~Data(void)
    {
        // First the exercise entries.
        delete[] data_entries;
        // Then the order of entry types.
        delete[] entry_order;
    }

    // Processed data entries.
    int *data_entries;
    // Number of entries. i.e. days.
    short data_size;
    // Order of entry types. i.e. Minutes &| Steps.
    char* entry_order;



    /* ****************************************************************
    // Print out the exercise entries made by the user.
    // Exists for debugging only.
    // ****************************************************************/
    void printData(void)
    {
        // General index counter for traversing data entries.
        short index = 0;

        cout << "/*/\n/*/\tDATA:" << endl;
        // For all data entries..
        for(index = 0; index < data_size; ++index)
        {
            // Print minutes.
            if(entry_order[index] == 'M')
                cout << "/*/\tM: " << data_entries[index] << endl;
            // Print steps.
            else
                cout << "/*/\tS: " << data_entries[index] << endl;
        }

        cout << "/*/\n/*/\tEND OF DATA." << endl;
        return;
    }
};


// The number of feet in a mile, except.. multiplied by 12. :O
const double IN_PER_MILE = 63360.0;

// The size of the height entry buffer is 10.
const unsigned short HEIGHT_BUFFER_SIZE = 10;

// Front end.
void greetUser(void); // Prints out greeting message.
void endProgram(void); // Prompts for restart/quit.
void getRawHeight(char* raw_height); // Gets height input.
short getPace(void); // Gets pace input.
short getDays(void); // Gets number of data entries.
void getRawData(char* raw_data, short days, int buffer_size); // Gets data input.
void printHalError(const char* ERROR_MESSAGE); // HAL corrects user error.
void displaySeparator(void); // Displays a line of "/*/".
void printResults(Data* data, short pace, double stride_len); // Prints out exercise results.
// Back end.
short parseRawHeight(char* raw_height); // Processes user height entry.
void parseRawData(char* raw_data, Data* data); // Processes user data entry/entries.
Data* createData(char* raw_data, short days); // Creates Data object for user info.
double strideLength(short height); // Calculates the length of the user's stride.
double stepsPerMile(double stride_len); // Calculates the user's number of steps per mile.
double milesExercised(int steps, double stride_len); // Calculates miles exercised with step data.
double milesExercised(short time, short pace); // Calculates miles exercised with time data.



int main(void)
{
    short height = 0, // User's height in inches.
          pace   = 0, // User's exercise pace in minutes per mile.
          days   = 0; // The number of days of data to be entered.

    // Initialized with the number of characters allowed for 
    // each entry. Multiplied by the number of entries later.
    int data_buf_size = 10; 
    
    // User's stride length in inches.
    double stride_len = 0.0;

        // User's unparsed height submission
    char *raw_height = new char[HEIGHT_BUFFER_SIZE + 1],
        // User's unparsed data submission(s)
         *raw_data;

    Data* data; // User's exercise data entries.

    // Read in & parse general user information.
    greetUser(); // Print greeting to console.
    getRawHeight(raw_height); // Read in the user's height entry.
    height = parseRawHeight(raw_height); // Parse and set height. Delete raw_height.
    pace = getPace(); // Read in the user's pace in minutes per mile.
    days = getDays(); // Read in the User's number of days exercised.

    // Calculate buffer size for data entry.
    data_buf_size *= static_cast<int>(days);

    // Allocate 10 buffer chars for each day.
    raw_data = new char[data_buf_size + 1];

    // Read in the user's exercise data entry/entries.
    getRawData(raw_data, days, data_buf_size);
    // Create a new data object for user's specific entry/entries
    data = createData(raw_data, days);
    // Parse data entries, store in data object. Delete raw_data.
    parseRawData(raw_data, data);

    // Calculate the user's approximate stride length.
    stride_len = strideLength(height);

    // Print out the user's number of miles exercised for each entry,
    // as well as the average amount of time spent exercising.
    printResults(data, pace, stride_len);

    // Deallocate data.
    delete data;
    data = NULL;

    endProgram(); // Prompt the user to restart or quit.

    return 0; // Fin.
}



/* ****************************************************************
// Prompts for and reads in the users height entry in feet and or
// inches. This function does check for invalid entries (negative,
// decimal), however, the numeric data is extracted by a separate
// function, namely, parseRawHeight.
// ****************************************************************/
void getRawHeight(char* raw_height)
{
    short ch = 0; // Index counter for error checking user input.

    // Prompt user to enter their height in feet and or inches.
    cout << "/*/\n/*/\tPlease enter your height in feet and or inches.\n/*/" << endl
         << "/*/\tFollow the number of feet with a single apostrophe." << endl
         << "/*/\tFollow the number of inches with a double apostrophe.\n/*/" << endl
         << "/*/\tFor example:" << endl
         << "/*/\tIf your height is five feet and six inches, enter 5\'6\" or 66\"\n/*/" << endl
         << "/*/\tEnter your height: ";

    // Read in user height.
    cin.get(raw_height, HEIGHT_BUFFER_SIZE, '\n');
    cin.get();

    // Verify that the user's height is a positive whole number.
    // For every character in the user's height input..
    for(; ch < HEIGHT_BUFFER_SIZE - 1; ++ch) // -1 in case user ends entry with .
    {
        // If the user enters a negative or a decimal..
        if(raw_height[ch] == '-' || raw_height[ch] == '.')
        {   // Specifically, if the user entered a negative
            if(raw_height[ch] == '-')
                // Print out an error message for negative entry.
                printHalError("Surely your height is not a negative number. Try again.");
            // Otherwise if the user has entered a decimal..
            else if(raw_height[ch] == '.')
                // Print out an error message for decimal entry.
                 printHalError("Whole numbers only, please. Try again.");

            // In either error case, try to get
            // the user's height once again.
            getRawHeight(raw_height);
        }
        if(raw_height[ch] == ' ')
            // Convert it to a unit symbol. The parser will simply ignore this.
            raw_height[ch] = '\'';
    }

    return;
}



/* ****************************************************************
// Processes the height entered by the user. Because the program
// allows a user to enter their height in feet and or inches,
// this function is necessary to detect unit types, extract
// numeric values, and return the total height in inches.
// ****************************************************************/
short parseRawHeight(char* raw_height)
{
    unsigned short height = 0, // Total height in inches to be returned.
          height_to_add   = 0, // Height to be added for each parsed number.
          parsed_num      = 0, // Number parsed from input -- updated every inner-loop iteration.
          current_index   = 0, // Outer-loop control. Goes to inner-loop when \' or \" is
                               // parsed from raw_height.
          buffer_index    = 0, // Inner-loop control. Goes from index after last \' or \" to
                               // current_index. These indices are the numbers in raw_height.
          i_aftr_lst      = 0; // Used to set the buffer index.

    char current_char = ' ', // Contains the character at current_index of raw_height.
         buffer_char  = ' '; // Contains the character at buffer_index of raw_height.

    // For every character in raw_height
    for(; current_index < strlen(raw_height); ++current_index)
    {
        // Store the current character for flow control.
        current_char = raw_height[current_index];
        // If the current character is a unit symbol
        if(current_char == '\"' || current_char == '\'')
        {
            // For every character from the index after the last unit symbol,
            // to one less than the current index of the outer-loop
            for(buffer_index = i_aftr_lst; buffer_index < current_index; ++buffer_index)
            {
                // Store the character for this iteration of the buffering segment of raw_height.
                buffer_char = raw_height[buffer_index];

                // Subtract '0' from the buffer_char to get the number contained in the raw height.
                parsed_num = static_cast<short>(buffer_char - '0');

                // Add the parsed number to the total number for the current entry.
                // So that the number is added to its proper decimal place, multiply
                // the parsed number by ten to the power of the one greater than
                // buffer_index subtracted from the current index.
                height_to_add += parsed_num * pow(10,current_index - (buffer_index+1));
            }

            // If the current character is the foot unit symbol
            if(current_char == '\'')
                // Multiply the parsed number by twelve, and add it to the final height.
                height += height_to_add * 12;
    
            // If the current character is the inch unit symbol
            else
                // Add the parsed number to the final height.
                height += height_to_add;

            // Reset height to be added for the next parsed number.
            height_to_add = 0;
            // Store the index after the current index to set the next buffering segment.
            i_aftr_lst = current_index + 1; 

        }
    }

    // Deallocate raw_height.
    delete[] raw_height;
    raw_height = NULL; 

    // Print user height (in inches) to the console.
    cout << "/*/\n/*/\tHeight given (Inches): " << height << "\n/*/" << endl;
    displaySeparator();

    // Return the parsed height.
    return height;
}



/* ****************************************************************
// Prompts for and reads in the users pace in miles per minute.
// ****************************************************************/
short getPace(void)
{
    // User's pace entry
    short pace = 0;

    // Prompt the user to enter their pace as a whole number in minutes per mile.
    cout << "/*/\n/*/\tPlease enter your pace as a"
         << " whole number in minutes per mile.\n/*/" << endl
         << "/*/\tFor example:" << endl
         << "/*/\tIf you run a 5 minute mile, simply enter the number 5.\n/*/" << endl
         << "/*/\tEnter your pace: ";
    
    // Read in user pace.
    cin >> pace;
    cin.ignore(100,'\n');

    // If the user enters a negative pace..
    if(pace < 0)
    {
        // Print out an error message for negative entry.
        printHalError("Pace must be a whole number greater than 0");
        // Try to get the user's pace once again.
        return getPace();
    }

    // Print out the user's pace.
    cout << "/*/\n/*/\tPace given (Minutes per mile): " << pace << "\n/*/" << endl;
    displaySeparator();

    // Return, the user's pace.
    return pace;
}



/* ****************************************************************
// Prompts for and reads in the number of days for which the user
// would like to enter exercise data.
// ****************************************************************/
short getDays(void)
{
    // User's entry for number of days exercised.
    short days = 0;

    // Prompt the user to enter the number of days for which they
    // would like to enter exercise data.
    cout << "/*/\n/*/\tYou can now to enter your exercise data." << endl
         << "/*/\n/*/\tTo get started, please enter the number of days" << endl
         << "/*/\tworth of data would you like to enter? " << endl
         << "/*/\n/*/\tFor example:" << endl
         << "/*/\tIf you want to process data from seventeen days, simply enter" << endl
         << "/*/\tthe number 17." << endl
         << "/*/\n/*/\tEnter the number of days worth of data that you would" << endl
         << "/*/\tlike to process: ";

    // Read in days exercised
    cin >> days;
    cin.ignore(100,'\n');

    // If the user enters a negative number of days..
    if(days < 0)
    {
        // Print out an error message for negative entry.
        printHalError("Days exercised must be a whole number greater than 0.");
        // Try to get the number of days exercised once again.
        return getDays();
    }

    // If no data to enter.
    if(days == 0)
        // Prompt to restart or quit.
        endProgram();

    // Print out the user's entry for days exercised.
    cout << "/*/\n/*/\tDays exercised: " << days << "\n/*/" << endl;
    displaySeparator();

    // Return the number of days exercised.
    return days; 
}



/* ****************************************************************
// Prompts for and reads in user exercise data. This function
// does check for invalid entries (negative, decimal), however,
// the numeric data is extracted by a separate function, namely,
// parseRawData.
// ****************************************************************/
void getRawData(char* raw_data, short days, int buffer_size)
{
    // Index counter for error checking user input.
    short ch = 0;

    // Print rules for exercise data entry.
    // This part of the prompt will appear regardless
    // of the number of days entered by the user.
    cout << "/*/\n/*/\tFinally, you can now submit your exercise data." << endl
         << "/*/\n/*/\tIf you would like to submit your time spent exercising," << endl
         << "/*/\tenter the number of minutes exercised, followed by the letter M." << endl
         << "/*/\n/*/\tFor example:" << endl
         << "/*/\tIf you ran for 2 hours, enter 120M." << endl
         << "/*/\n/*/\tIf you would like to submit your number of steps taken," << endl
         << "/*/\tenter the number of steps exercised, followed by the letter S." << endl
         << "/*/\n/*/\tFor example:" << endl
         << "/*/\tIf you ran for 300 steps, enter 300S." << endl
         << "/*/\tIf you ran for 26 minutes, enter 26M." << endl;

    // If the user is going to enter data for more than one day..
    if(days > 1)
        // Print out additional rules for data entry.
        cout << "/*/\n/*/\tYou have chosen to enter " << days << " days worth of data." << endl
             << "/*/\n/*/\tNote that you may optionally mix the types of data entered." << endl
             << "/*/\n/*/\tFor example, a week's worth of data will look similar to the following:" << endl
             << "/*/\t300S 20M 60M 250S 35M 45M 600S" << endl;

    // Prompt the user for entry/entries.
    cout << "/*/\n/*/\tEnter your data: ";
    
    // Read in the user's exercise data.
    cin.get(raw_data , buffer_size, '\n');
    cin.get();

    // Verify that each datum is a positive whole number.
    // For every character in the user's data entries..
    for(ch = 0; ch < buffer_size-1; ++ch) // -1 in case user ends entry with .
    {
        // If the user enters either a negative or a decimal..
        if(raw_data[ch] == '-' || raw_data[ch] == '.')
        {   // Print out error message for invalid data entry.
            printHalError("All entries must be positive, whole numbers. Try again.");
            // Try to get the user's data once again.
            getRawData(raw_data, days, buffer_size);
        }
    }

    return;
}



/* ****************************************************************
// Processes the exercise data entries made by the user. By this
// point, all user entries have been made, and the Data object has
// been created. This function extracts the numeric data from the
// user data, and adds it to the Data object.
// ****************************************************************/
void parseRawData(char* raw_data, Data* data)
{
    short current_index = 0, // Index counter for character in raw_data.
          buffer_index  = 0, // Index counter for reading numbers leading
                             // up to some entry type 'S' or 'M', from the
                             // last entry scanned.
          i_aftr_lst    = 0, // The index after the last entry scanned.
          parsed_num    = 0, // Set to the digit at some index of raw_data.
          data_index    = 0; // Tracking next empty step data index.

    // Total numeric datum to be added to Data object.
    int data_to_add   = 0;

    char current_char = ' ', // The current character being scanned by the outer loop.
         buffer_char  = ' '; // The current character being scanned by the inner loop.

    // For every character in the set of data entries..
    for(; current_index < strlen(raw_data); ++current_index)
    {
        // Save the character at the index corresponding to the outer loop iteration.
        current_char = raw_data[current_index];

        // If the current character is an entry type symbol..
        if(current_char == 'S' || current_char == 'M')
        {
            // For every character leading up to the current outer loop character,
            // from the index after the last read entry..
            for(buffer_index = i_aftr_lst; buffer_index < current_index; ++buffer_index)
            {
                // Save the character at the index corresponding to the inner loop iteration.
                buffer_char = raw_data[buffer_index];

                // If a space is detected, continue. Allows the user to separate entries
                // with a space. I know we're not really supposed to use continue.
                // I did it anyway.
                if(buffer_char == ' ') continue; 

                // Subtract '0' from the buffer_char to get the number contained in raw input.
                parsed_num = static_cast<short>(buffer_char - '0');

                // Add the parsed number to the total number for the current entry.
                // So that the number is added to its proper decimal place, multiply
                // the parsed number by ten to the power of the one greater than
                // buffer_index subtracted from the current index.
                data_to_add += parsed_num * pow(10,current_index - (buffer_index+1));
            }
            // Add the parsed data to the Data object.
            data -> data_entries[data_index++] = data_to_add;

            // Resed / update fields.
            data_to_add = 0;
            i_aftr_lst = current_index + 1;
        }
    }

    // Deallocate raw_data.
    delete[] raw_data;
    raw_data = NULL;

    return;
}



/* ****************************************************************
// Creates an array containing the order of entry types given
// by the user. Creates a new data object with entry type order,
// as well as the number of entries (days). At this point, the
// data has been entered, but not parsed. i.e. The actual exercise
// data is not given to the Data object in this function.
// ****************************************************************/
Data* createData(char* raw_data, short days)
{
    short entry_counter = 0, // Tracking number of entries from 0 to days - 1
          ch            = 0; // General index counter for traversing raw_data.

    // Contains the order of exercise data entry types.
    char *entry_order = new char[days+1];

    // Save number of entries for time and steps. Save order of entries.
    // For every character in raw_data..
    for(ch = 0; ch < strlen(raw_data); ++ch)
    {
        // If entry is in seconds exists.
        if(raw_data[ch] == 'S')
            // Save 'S' to index equal to total entries counted.
            entry_order[entry_counter++] = 'S';
        // If entry is in minutes exists.
        if(raw_data[ch] == 'M')
            // Save 'M' to index equal to total entries counted.
            entry_order[entry_counter++] = 'M';
    }

    // Create and return a new Data object according to input specifications.
    return new Data(days, entry_order);
}



/* ****************************************************************
// Calculates the user's approximate stride length, based on their
// height.
// ****************************************************************/
double strideLength(short height)
{
    return height * 0.42;
}



/* ****************************************************************
// Calculates the number of steps taken by a user, based on their
// stride length.
// ****************************************************************/
double stepsPerMile(double stride_len)
{
    return IN_PER_MILE/stride_len;
}



/* ****************************************************************
// Calculates the number of miles a user has exercised.
// Overloaded to calculate based on entry type.
// ****************************************************************/

// Calculation for steps taken entry.
double milesExercised(int steps, double stride_len)
{
    return static_cast<double>(steps)/stepsPerMile(stride_len);
}
// Calculation for minutes exercised entry.
double milesExercised(short time, short pace)
{
    return static_cast<double>(time)/static_cast<double>(pace);
}



/* ****************************************************************
// Prints a message to the user regarding an entry error.
// ****************************************************************/
void printHalError(const char* ERROR_MESSAGE)
{
    cout << "/*/\n/*/\tI'm sorry, Dave. I'm afraid I can't do that." << endl
         << "/*/\t" << ERROR_MESSAGE << "\n/*/" << endl;
    displaySeparator();

    return;
}



/* ****************************************************************
// Used to separate sections of the program's interface.
// Function exists purely because I don't want to type
// out the actual cout statement over and over.
// ****************************************************************/
void displaySeparator(void)
{
    cout << "/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/"
         << "*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/" << endl;
    return;
}



/* ****************************************************************
// Displays a header, greeting the user at run time.
// ****************************************************************/
void greetUser(void)
{
    // Line of '/*/'.
    displaySeparator();

         // Blank line, starting with '/*/'.
    cout << "/*/" << endl
         // Message, starting with '/*/'.
         << "/*/\tHello! Welcome to your Personal Exercise Distance Calculator." << endl
         // Blank line, starting with '/*/'.
         << "/*/" << endl;

    // Line of '/*/'.
    displaySeparator();
    return;
}



/* ****************************************************************
// Calculates, prints out the user's exercise data results.
// Data object is used to get exercise data, which is feed into
// milesExercised with either pace or stride length.
// ****************************************************************/
void printResults(Data* data, short pace, double stride_len)
{
    short data_index = 0, // Index counter for traversing exercise data.
          avg_time   = 0; // Average time spent exercising.

    int data_val = 0; // Reset for, to each data entry.

    double miles_ex_ea = 0.0, // Miles reset each exercise entry.
           total_miles = 0.0, // Cumulative miles exercised.
           time_sum    = 0.0, // Total amount of time exercised.
           step_sum    = 0.0; // Total number of steps taken.

    // Contains either 'Minutes' or 'Steps', depending on data.
    // Used to print out the entry type to the user.
    const char *type_label;

    // Print out miles exercised for each entry.
    // Print output format.
    cout << "/*/" << endl;
    displaySeparator();
    cout << "/*/\n/*/\tExercise results (Entry #: Entry-Value Type -> # Miles):\n/*/" << endl;

    // For each user entry..
    for(;data_index < data->data_size; ++data_index)
    {
        // Get data_indexth value from data object.
        data_val = data->data_entries[data_index];

        // If entry type is in minutes..
        if(data->entry_order[data_index] == 'M')
        {
            // Calculate miles exercised from minutes.
            miles_ex_ea = milesExercised(static_cast<short>(data_val), pace);
            // Add individual time to total time.
            time_sum += data_val;
            // Set output label to "Minutes".
            type_label = "Minutes";
        }
         // If entry type is in steps..
        if(data->entry_order[data_index] == 'S')
        {
            // Calculate miles exercised from steps.
            miles_ex_ea = milesExercised(data_val, stride_len);
            // Add individual steps to total steps.
            step_sum += data_val;
            // Set output label to "Steps".
            type_label = "Steps";
        }

        // Print out individual entry results.
        cout << "/*/\t  #" << data_index + 1 // Add one to start displaying at 1.
             << ": " << data_val
             << " " << type_label
             << " -> " << miles_ex_ea
             << " Miles" << endl;

        // Add local miles to total miles.
        total_miles += miles_ex_ea;
    }

    // Average time from mins.
    time_sum =  time_sum/data->data_size;
    // Average time from steps.
    step_sum = pace * milesExercised((step_sum/data->data_size), stride_len);

    // Total average exercise time
    avg_time = (time_sum + step_sum) / 2; 

    // Display average number of minutes per exercise.
    cout << "/*/\n/*/\tYou've spent an average of " << time_sum
         << " minutes exercising per session." << endl;

    return;
}



/* ****************************************************************
// Everything has been leading up to this point. This is it, the
// proverbial 'final countdown'. There's no telling what decision
// a user will make. Brace yourself.
// ****************************************************************/
void endProgram(void)
{
    // User's decision for program termination.
    char response = 'n';

    // Prompt user to terminate/restart the program.
    cout << "/*/" << endl;
    displaySeparator();
    cout << "/*/\n/*/\tWould you like to quit or restart? Q/R: ";

    // Read in the user's response.
    cin >> response;
    cin.ignore(100, '\n');

    // Convert to uppercase letter.
    response = toupper(response);

    // If the user enters 'Q', terminate the program.
    if(response == 'Q')
    {
        cout << "/*/\n/*/\tGoodbye!\n/*/" << endl;
        displaySeparator();
        exit(0);
    }
    // If the user enters 'R', restart the program.
    if(response == 'R')
    {
        cout << "/*/\n/*/" << endl;
        main();
    }
    // Otherwise, try to get the user's termination decision once again.
    else
    {
        printHalError("Invalid entry. Please enter either Q(quit) or R(restart)");
        endProgram();
    }
    return;
}
