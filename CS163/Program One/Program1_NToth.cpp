/*****************************************************************
// File Name:  Program1_NToth.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       4/19/2017
// Class:      CS163
// Program #:  1
//
// Overview: This is the test file for the Channels chat program.
// Essentially, this program creates a linked list of Channel
// objects each of which contains a linked list of messages. When
// the program is loaded, a menu appears with a list of options for
// testing the Channels functionality. This functionality includes
// creating new Channels, posting new Messages to specific
// preexisting Channels (assuming the User posting the new message 
// is a member of the Channel they're trying to post in), deleting
// messages in all channels that were created before a specific
// date, multiple methods of printing, and more.
//
// ***************************************************************/

#include "Channels.h"

using namespace std;

// ***************  |
// Menu handlers *  |
// ***************  V

// Handler for new channel menu selection.
void handleNewChan( Channels *& chan_lst,
                    char *& chan_name,
                    char *& chan_description );

// Handler for new message menu selection.
void handleNewMsg( Channels *& chan_lst,
                   char *& channel_name,
                   char *& user_name,
                   char *& message );

// Handler for display all messages in channel menu selection.
void handleDisplayAllMsgs( Channels *& chan_lst,
                           char *& channel_name );

// Handler for search for keyword menu selection.
void handleSearchKey( Channels *& chan_lst,
                      char *& keyword );

// Handler for search for user menu selection.
void handleSearchUser( Channels *& chan_lst,
                       char *& user_name );

// Handler for remove old messages menu selection.
void handleRemoveOld( Channels *& chan_lst,
                      char *& time_stamp );

// *******************  |
// Testing functions *  |
// *******************  V

// Print out the main program menu, read in & return selection.
short menuController(void);

// Prompt user to press enter to continue.
void continueMenu(void);

// Get a channel name (for searching & setting!). Note that in
// search cases, a list of options will need to be printed
// prior to the getChannelSelection call using the Channels
// class's displayChannelNames function.
void getChannelSelection(char *& channel_name);

// Prompt for and read in a user name (searching & setting).
void getUserName(char *& user_name);

// Create a new user object.
User * createUser(void);

// Prompt for and read in a new message.
void getMessage(char *& message);

// Prompt for and read in a keyword to search for.
void getKeyword(char *& keyword);

// Prompt for and read in a time stamp from the user.
void getTimestamp(char *& time_stamp);



int main(void)
{
    // Loop control variable set to true when the user selects quit from the menu.
    bool quit = false;

    // Stores the user's menu selection.
    short selection = 0;

           // Variable for temporarily storing channel names.
    char * channel_name = new char[MAX_NAME_LEN + 1],
           // Variable for temporarily storing channel descriptions.
         * channel_description = new char[MAX_DESCRIPTION_LEN +1],
           // Variable for temporarily storing time stamps.
         * time_stamp = new char[MAX_TIME_LEN + 1],
           // Variable for temporarily storing user names.
         * user_name = new char[MAX_NAME_LEN + 1],
           // Variable for temporarily storing messages.
         * message = new char[MAX_MSG_LEN + 1],
           // Variable for temporarily storing keywords.
         * keyword = new char[MAX_KEYWORD_LEN + 1];

    // The linked list of Channel objects.
    Channels *chan_lst = new Channels;

    // Print program header.
    cout << "\n ---------------- Chat Channel Program ---------------- " << endl;

    // Main program loop.
    // Until the user selects 7 (quit) from the program menu..
    while(!quit)
    {
        // Print the program's main menu, read in the user's selections.
        selection = menuController();

        // Execute the user's selection from the program menu.
        switch(selection)
        {
            // User has selected create a new channel.
            case 1:
                // Call the new Channel handler.
                handleNewChan(chan_lst, channel_name, channel_description);
                break;

            // User has selected post a new message to a channel.
            case 2:
                handleNewMsg(chan_lst, channel_name, user_name, message);
                break;

            // User has selected display all messages in a channel.
            case 3:
                handleDisplayAllMsgs(chan_lst, channel_name);
                break;

            // User has selected search all messages for keyword.
            case 4:
                handleSearchKey(chan_lst, keyword);
                break;

            // User has selected search all channels with user.
            case 5:
                handleSearchUser(chan_lst, user_name);
                break;

            // User has selected remove old messages.
            case 6:
                handleRemoveOld(chan_lst, time_stamp);
                break;

            // User has selected quit the program.
            default:
                quit = true;
                break;
        }

        // If anything other than quit has been selected from the menu..
        if(!quit)
            // Prompt the user to press enter to continue, wait, continue.
            continueMenu();
    }

    // Call the Channels destructor for chan_lst.
    delete chan_lst;
    chan_lst = NULL;

    // Deallocate the char arrays used for searching.
    delete[] channel_name;
    channel_name = NULL;
    delete[] channel_description;
    channel_description = NULL;
    delete[] time_stamp;
    time_stamp = NULL;
    delete[] user_name;
    user_name = NULL;
    delete[] message;
    message = NULL;
    delete[] keyword;
    keyword = NULL;

    // Quit. Politely.
    cout << "\n  Goodbye!\n" << endl;

    return 0; // Fin.
}



/*******************************************
// This is the function used to create a new
// channel whenever the user selects create
// new channel from the menu. The chan_lst
// parameter's insert channel function is
// called with the information created using
// this function. The char *& arguments,
// chan_name and chan_description, are used
// for all functions which read in channel
// names/descriptions and pass them to the
// chan_lst.
//
// *****************************************/
void handleNewChan( Channels *& chan_lst,
		    char *& chan_name,
		    char *& chan_description )
{
    short num_users = 0, // Total number of useers to be added.
          user_cnt  = 0; // Index counter for inserting new users into new Channel's User array.

    // Array of users that can post to the new channel.
    User *usergroup;

    // Get the channel name.
    getChannelSelection(chan_name);

    // Get the channel description.

    // Prompt the user to enter a description of the channel.
    cout << endl //Print some whitespace.
         << "  Enter a description of the channel." << endl
         << "\n    Description: ";

    // Read in the user's channel description.
    cin.get(chan_description, MAX_DESCRIPTION_LEN, '\n');
    cin.ignore(100,'\n');
    cout << endl; // Glorious whitespace.

    // Get the number of users to be added.
    while(num_users < 1)
    {
        // Prompt the user to enter the number of users.
        cout << "\n  Enter the number of users to be added." << endl
            << "\n    Total Users: ";

        // Read in the total number of users to be added to the channel.
        cin >> num_users;
        cin.ignore(100,'\n');
        cout << endl; // Summore whitespace.

        // If the number entered is less than 1, the user will have to try again.
        if(num_users < 1)
            cout << "\n  Invalid Entry. Total users must be at least 1." << endl;
    }

    // Allocate memory for new users.
    usergroup = new User[num_users];

    // Get the users to add.
    do
    {
        // Print out the current user being created.
        cout << "\n  Creating user #" << (user_cnt + 1) << endl;

        // Prompt the user to enter a username.
        cout << "\n    Enter the new user's username: ";
        cin.get(usergroup[user_cnt].user_name, MAX_NAME_LEN, '\n');
        cin.ignore(100,'\n');

        // Prompt the user to enter an email.
        cout << "\n    Enter the new user's email: ";
        cin.get(usergroup[user_cnt].user_email, MAX_EMAIL_LEN, '\n');
        cin.ignore(100,'\n');
        cout << endl; // WHITESPACE!!

      // Continue until num_users Users have been created.
    } while(num_users > ++user_cnt);

    // Try to create, insert a new channel into chan_lst.
    // If it fails..
    if(!chan_lst->insertChannel( chan_name,
                                 chan_description,
                                 num_users,
                                 usergroup) )

        // Print an alert that the new channel could not be created.
        cout << "\n  Error creating new channel!" << endl
             << "  Returning to the main menu." << endl;

    // Set the first character in chan_name and in
    // chan_description to null terminating char for reuse.
    *chan_name = '\0';
    *chan_description = '\0';

    // Delete the memory allocated to the usergroup.
    delete[] usergroup;

    return;
}



/*******************************************
// This is the function used to create a new
// message whenever the user selects post a
// new message from the menu. The chan_lst
// parameter's post message function is
// called with the information created using
// this function. The char *& arguments,
// chan_name, user_name and message are used
// for all functions which read in channel
// names/user names/ descriptions and pass
// them to the chan_lst.
//
// *****************************************/
void handleNewMsg( Channels *& chan_lst,
                   char *& channel_name,
                   char *& user_name,
                   char *& message )
{
    // Loop control variable for message insertion.
    bool success = false;
    // User confirmation variable for retrying when a channel cannot be found by name.
    char selection = 'N';

    // Print an alert to the console that the user will need to enter
    // the name of a channel to post the new message to.
    cout << "\n  Which channel would you like to post your message to?" << endl
         << "  Here is a list of your options: " << endl << endl;

    // Print out a list of existing channels.
    // If no channels exist..
    if(!chan_lst->displayChannelNames())
        // Alert the user.
        cout << "\n  Error :: No channels exist." << endl
             << "  Returning to the main menu!" << endl;

    // If channels exist..
    else
    {
        // Try to get information for a new channel
        // until all info has been entered successfully.
        do
        {
            // Get a channel to post to from the user.
            getChannelSelection(channel_name);

            // Prompt the user to enter a username.
            cout << "\n  Enter your username: ";
            // Read in the user's username.
            cin.get(user_name, MAX_NAME_LEN, '\n');
            cin.ignore(100,'\n');

            // Read in a message from the user.
             getMessage(message);

            // Post the message.
            success = chan_lst->postMessage(channel_name, user_name, message);

            // If the message was posted successfully..
            if(success)
                // Print an alert that the message was posted successfully.
                cout << "\n    Message posted successfully!" << endl;
            // If the message was not posted successfully..
            else
            {
                // Ask the user if they want to try again.
                do
                {
                    // Prompt for the user to reenter the name.
                    cout << "\n  Error :: Access denied." << endl
                         << "  Try again? (Y/N): ";

                    // Read in user's selection.
                    cin >> selection;
                    cin.ignore(100, '\n');

                    // Convert entry to uppercase.
                    selection = toupper(selection);

                    // If the user selects N..
                    if(selection == 'N')
                    {
                        // Alert the user that the program is returning to the main menu.
                        cout << "\n  Returning to main menu." << endl;

                        // Set success to true so the function can end.
                        success = true;
                    }
                    // If the user has not entered a valid response..
                    if(selection != 'N' && selection != 'Y')
                        // Alert them to try again.
                        cout << "\n    Invalid entry. Please enter Y or N..." << endl;
                }
                // Loop until a valid response is entered.
                while( (selection != 'Y' && selection != 'N') );
            }
                // Set first char of info buffers to
                // null terminating char for reuse.
                *channel_name = '\0';
                *user_name = '\0';
                *message = '\0';
        }
        // Keep trying until the message is posted successfully,
        // or the user enters N to the try again prompt.
        while(!success);
    }

    return;
}



/*******************************************
// This is the function used to display all
// messages in a specific channel. The user
// is asked to enter a channel name, which is
// saved to the channel_name parameter, and
// the chan_lst parameter's print function is
// called with that username.
//
// *****************************************/
void handleDisplayAllMsgs( Channels *& chan_lst,
                           char *& channel_name )
{
    // Print an alert to the console that the user will need to enter
    // the name of a channel to display
    cout << "\n  Which channel would you like to display?" << endl
         << "  Here is a list of your options: " << endl << endl;

    // Print out a list of existing channels.
    // If no channels exist..
    if(!chan_lst->displayChannelNames())
        // Alert the user.
        cout << "\n  Error :: No channels exist." << endl
             << "  Returning to the main menu!" << endl;

    // If channel(s) do exist..
    else
    {
        // Read in a channel selection from the user.
        getChannelSelection(channel_name);

        // Try to display the messages for the specified channel.
        // If the channel does not exist..
        if(!chan_lst->dispChanMsgs(channel_name))
            // Alert the user that their entry was invalid, and return to the menu.
            cout << "\n  Either no channel by that name exists, or the channel is empty." << std::endl;
    }
    // Set first char of channel_name to null
    // terminating char for reuse.
    *channel_name = '\0';

    return;
}



/*******************************************
// This is the function used to display the
// names of channels in chan_lst which
// contain a keyword, saved to the keyword
// parameter, in their descriptions or some
// message.
//
// *****************************************/
void handleSearchKey( Channels *& chan_lst,
                      char *& keyword )
{
    // Get a keyword from the console.
    getKeyword(keyword);

    // Search for the keyword in chan_lst, and store the result.
    bool match_found = chan_lst->dispChanWithKey(keyword);

    // If a match is not found..
    if(!match_found)
        // Print an alert that the message could not be found.
        cout << "\n  The keyword: " << keyword
             << "  is nowhere to be found!\n" << endl;

    // Set first char of keyword to null
    // terminating char for reuse.
    *keyword = '\0';
    return;
}



/*******************************************
// This is the function used to search the
// channels in chan_lst for a specific user.
// The name of the user to be searched in
// read into the user_name parameter, and
// passed to the chan_lst's search user
// function.
//
// *****************************************/
void handleSearchUser( Channels *& chan_lst,
                       char *& user_name )
{
    // Prompt for and read in a username.
    getUserName(user_name);

    bool matched = chan_lst->dispChanWithUsr(user_name);

    // If the username could not be found in any usergroups..
    if(!matched)
        // Print an alert that the user could not be found.
        cout << "\n  The User: " << user_name
             << "  does not exist in any usergroups!\n" << endl;

    // Set first char of user_name to
    // null terminating char for reuse.
    *user_name = '\0';
    return;
}



/*******************************************
// This is the function used to remove all
// messages in the chan_lst that are older
// than a time_stamp. That time stamp is
// read into the time_stamp parameter, and
// passed into the chan_lst's remove old
// function.
//
// *****************************************/
void handleRemoveOld( Channels *& chan_lst,
                      char *& time_stamp )
{
    // Read in a timestamp to the time_stamp array.
    getTimestamp(time_stamp);

    // Attempt to remove messages older than the specified time.
		// Store the result.
    short removed_msgs = chan_lst->removeOldMsgs(time_stamp);

    // If no messages were older than the specified time..
    if(removed_msgs == 0)
        // Print an alert that no messages were removed.
        cout << "\n  There are no old messages to remove." << endl;
    // Otherwise..
    else
        // Print out the number of removed messages.
        cout << "\n  " << removed_msgs << " Messages have been removed." << endl;

    // Set first char of time_stamp to
    // null terminating char for reuse.
    *time_stamp = '\0';

 		return;
}



/*******************************************
// Print the program testing menu, read in
// a selection, return selection to main.
//
// *****************************************/
short menuController(void)
{
          // The number of selections in the menu.
    short max_selections = 7,
          // The user's menu selection.
          selection      = -1;

    // Print out the program menu.
    cout << "\n  Make a selection from the menu." << endl << endl
         // If the user enters 1, create, add a new channel.
         << "    [1] => Create a new channel." << endl
         // If the user enters 2, create, post a new message.
         << "    [2] => Post a new message to a channel." << endl
         // If the user enters 3, display a channel's messages
         << "    [3] => Display all messages in a channel. " << endl
         // If the user enters 4, search channels for a keyword.
         << "    [4] => Search all channels for a keyword." << endl
         // If the user enters 5, search channel's usergroups for a user.
         << "    [5] => Search all channel usergroups for a user." << endl
         // If the user enters 6, remove old messages from all channels.
         << "    [6] => Remove old messages." << endl
         // If the user enters 7, quit.
         << "    [7] => Quit the program." << endl;

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
            // Alert the user that they will need to try again in the range 1, max_selections.
            cout << "\n  Invalid entry! Please enter a number from 1 through "
								 << max_selections << '.' << endl;
    }

    // Return the user's selection.
    return selection;
}



/*******************************************
// Prompt's user to press enter to continue
// so they can read output before going back
// to the main menu.
//
// *****************************************/
void continueMenu(void)
{
  // Prompt user to press enter.
  cout << "\n  Press enter to continue...";
  // Wait until user continues.
  cin.ignore(100,'\n');
  return;
}



/*******************************************
// Read in a channel name from the user to
// the channel_name parameter.
//
// *****************************************/
void getChannelSelection(char *& channel_name)
{
    // Prompt the user to enter their channel selection.
    cout << "\n  Enter the channel name." << endl
         << "\n    Name: ";

    // Read in the user's channel selection.
    cin.get(channel_name, MAX_NAME_LEN, '\n');
    cin.ignore(100,'\n');

    // Print whitespace.
    cout << endl;
    return;
}



/*******************************************
// Create and return a new user object.
//
// *****************************************/
User * createUser(void)
{
    // Create a new User.
    User *new_user = new User;

    // Prompt the user to enter a username.
    cout << "\n  Enter the new user's username: ";
    // Read in the User's name.
    cin.get(new_user->user_name, MAX_NAME_LEN, '\n');
    cin.ignore(100,'\n');

    // Prompt the user to enter an email.
    cout << "\n  Enter the new user's email: ";
    // Read in the User's email.
    cin.get(new_user->user_email, MAX_EMAIL_LEN, '\n');
    cin.ignore(100,'\n');

    // Return the new user.
    return new_user;
}



/*******************************************
// Read in a new message from the user,
// store in the message parameter.
//
// *****************************************/
void getMessage(char *& message)
{
    // Prompt the user to enter a new message
    cout << "\n  Enter a new message: ";

    // Read in the user's message.
    cin.get(message, MAX_MSG_LEN, '\n');
    cin.ignore(100,'\n');

    return;
}



/*******************************************
// Read in a keyword from the user, store in
// the keyword parameter.
//
// *****************************************/
void getKeyword(char *& keyword)
{
    // Prompt the user to enter a keyword to search for.
    cout << "\n  Enter a keyword to search: ";

    // Read in the user's keyword.
    cin.get(keyword, MAX_KEYWORD_LEN, '\n');
    cin.ignore(100,'\n');

    return;
}



/*******************************************
// Read in a user_name from the user, store
// in the user_name parameter.
//
// *****************************************/
void getUserName(char *& user_name)
{
    // Prompt the user to enter a username to search for.
    cout << "\n  Enter a username to search: ";

    // Read in the user's username.
    cin.get(user_name, MAX_NAME_LEN, '\n');
    cin.ignore(100,'\n');

    return;
}



/*******************************************
// Read in a time_stamp from the user, store
// in the time_stamp parameter.
//
// *****************************************/
void getTimestamp(char *& time_stamp)
{
    // Index counter for checking that a timestamp contains
    // all valid characters
    unsigned short index = 0;

    // Loop control variable set to true if the user enter a
    // non digit character where there should be a digit for
    // the timestamp.
    bool invalid_date = false;

    // Prompt the user to enter a time stamp.
    cout << "\n  Enter a timestamp using the format DD/MM/YYYY. " << endl
        << "  Please be sure to add include a zero for single digit months and days." << endl
        << "  For example, if you want to remove anything from before " << endl
        << "  January 11th, 2017, you should enter 11/01/2017." << endl;

    // Init time_stamp to '\0' for loop conditional.
    *time_stamp = '\0';

    // While the entry has not been read in,
    // or it was not complete (includes zeroes)..
    while(strlen(time_stamp) < MAX_TIME_LEN - 1 || invalid_date)
    {
        // Reset invalid_date to false.
        invalid_date = false;

		// Prompt the user to enter a number.
        cout << "\n    Enter time stamp: ";

        // Read in the user's timestamp.
        cin.get(time_stamp, MAX_TIME_LEN, '\n');
        cin.ignore(100,'\n');

        // For every character in the timestamp
        while(index < strlen(time_stamp))
        {
            // If the current character is not a digit (unless the current
            // index is 2 or 5, which should contain forward slashes)..
            if(index != 2 && index != 5 && !isdigit(time_stamp[index]))
            {
                // Print an alert that the entry was invalid.
                cout << "\n    Invalid entry. Please use numbers.. " << endl;
                // Set loop control to true, so that the user can try
                // entering a new date.
                invalid_date = true;
            }
            // Increment the index counter.
            ++index;
        }

        // If the date does not contain invalid characters,
        // but does have an invalid size..
        if(!invalid_date && strlen(time_stamp) < MAX_TIME_LEN - 1)
            // Alert the user that they must try again.
            cout << "\n    Invalid entry. Please include the zeroes. " << endl;

        // Reset index to false.
        index = 0;
    }
    return;
}
