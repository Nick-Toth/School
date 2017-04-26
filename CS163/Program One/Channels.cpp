/*****************************************************************
// File Name:  Channels.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       4/19/2017
// Class:      CS163
// Program #:  1
//
// Overview: This is the implementation file for the Channels list
// class. The Channels list is a linked list of Channel objects,
// each of which contains a linked list of messages. This file also
// contains the definitions for structs used by the Channels challs.
// These structs include User, Message and Channel.
//
// ***************************************************************/

// Include Channels, Channel, Message and User declarations.
#include "Channels.h"


// **************************************  |
// User Struct Function Implementations *  |
// **************************************  V


/**********************************************************
// User constructor allocates memory to store a user's
// name and email address.
//
// ********************************************************/
User :: User()
{
    // Allocate MAX_NAME_LEN to store the user's name.
    user_name = new char[MAX_NAME_LEN + 1];
    // Allocate MAX_EMAIL_LEN to store the user's email address.
    user_email = new char[MAX_EMAIL_LEN + 1];
}



/**********************************************************
// User destructor deallocates memory used to store the
// user's name and email address.
//
// ********************************************************/
User :: ~User()
{
    delete[] user_name; // Deallocate the memory storing the user's name.
    delete[] user_email; // Deallocate the memory storing the user's email.
    return;
}




// *****************************************  |
// Message Struct Function Implementations *  |
// *****************************************  V


/**********************************************************
// Message constructor allocates memory used to store a
// user, and sets the remaining Message members to NULL.
//
// ********************************************************/
// Message constructor.
Message :: Message()
{
    // Allocate memory for the message's poster.
    poster = new User;

    // Initialize remaining pointers to NULL.
    next = NULL;
    message = NULL;
    time_stamp = NULL;
}


/**********************************************************
// Message destructor deallocates memory used to store a
// the char[] message, the char[] timestamp, and the User.
//
// ********************************************************/
Message :: ~Message()
{
    delete[] message; // Deallocate the char[] message
    delete[] time_stamp; // Deallocate the char[] timestamp

    // Deallocate the user *As mentioned elsewhere, the User in Message
    // objects is currently allocated separately from Users in Channel
    // usergroups. In the future, it would be nice to make the Message
    // user point to the user in a Channel's usergroup.
    delete poster;
}



/**********************************************************
// This function is called by the display channel function.
// The count argument is used to print messages with their
// position in the list.
// ********************************************************/
bool Message :: printMessage(short count) const
{
    // If the message object is incomplete..
    if(!poster || !time_stamp || !message)
        // report error.
        return false;

    // If the print message is not called with -1, print out
    // the counter. This is done so that printMessage can be
    // used to print individual messages, as well as sets,
    // without overloading.
    if(count != -1)
        std::cout << "\n    Message: #" << count << std::endl;

    // Print out the contents of the message, including the
    // date and poster.
    std::cout << "    By User:  " << poster->user_name << std::endl
                << "    Date:     " << time_stamp << std::endl
                << "    Message:  " << message << std::endl;

    // Report success.
    return true;
}




// *****************************************  |
// Channel Struct Function Implementations *  |
// *****************************************  V


/**********************************************************
// The channel constructor sets the usr_cnt to 0, and sets
// every dynamic member to NULL. The dynamic members are all
// allocated in the Channels class.
//
// ********************************************************/
Channel :: Channel()
{
    // Initialize the number of users in the usergroup to 0.
    usr_cnt = 0;

    // Initialize all class field pointers to NULL.
    channel_name = NULL;
    description = NULL;
    usergroup = NULL;
    msg_head = NULL;
    msg_tail = NULL;
    next = NULL;
}



/**********************************************************
// The Channel destructor deallocates all local dynamic
// memory.
//
// ********************************************************/
Channel :: ~Channel()
{
    // Deallocate Channel array memory.
    // Call the destructors of each user in the usergroup.
    delete[] usergroup;
    usergroup = NULL;
    // Erase the Channel's title.
    delete[] channel_name;
    channel_name = NULL;
    // Erase the description of the Channel.
    delete[] description;
    description = NULL;

    // Create a temporary pointer for traversing
    // the message list to deallocate memory.
    Message *msg_temp = NULL;

    // While there is at least one message..
    while(msg_head)
    {
        // Store the next Message pointer.
        msg_temp = msg_head->next;
        // Delete the message stored by head.
        delete msg_head;
        // Set head to the next Message pointer.
        msg_head = msg_temp;
    }

    msg_head = NULL;
    msg_tail = NULL;
}



/**********************************************************
// The public displayMessages function calls the private
// displayMessages function, as well as in the Channels
// class's dispChanMsgs function. The public function
// exists to print metadata about the channel being
// printed, to hide the msg_head, and to initialize the
// message counter. False is returned ONLY from the public
// displayMessages function, i.e. false is only returned
// if there are no messages in the list.
//
// ********************************************************/
bool Channel :: displayMessages() const
{
    // If the Channel object is incomplete or empty, report error.
    if(!channel_name || !msg_head)
        return false;

    // Otherwise, print channel name to the console.
    std::cout << "\n  Displaying Channel: " << channel_name
                << std::endl << std::endl // More whitespace.
                << "  Channel Description: " << description
                << std::endl; // More whitespace.

    // Return / print out the messages contained in the Channel object.
    return displayMessages(msg_head, 1);
}



/**********************************************************
// The private displayMessages function is called by the
// public displayMessages. With each call, this function
// first checks that the message exists. When the message
// does not exist, true is returned. While the message does
// exist, it is printed using the Message struct's print
// function, and displayMessages calls itself with the next
// message pointer.
//
// ********************************************************/
bool Channel :: displayMessages( Message * head,
                                 short count ) const
{
    // Once the messages are all printed, return true.
    // Head is first checked in the public displayMessages
    // function, which is why there is no need to return
    // false if head is NULL.
    if(!head)
        return true;

    // Print out the message using the Message struct's
    // print functions, and then call this function with
    // the next message object. The printMessage function
    // checks that the Message object is complete i.e. both
    // functions must be returned to report a potential message
    // error.
    return head->printMessage(count) && displayMessages(head->next, ++count);
}




// *****************************************  |
// Channels Class Function Implementations *  |
// *****************************************  V


/**********************************************************
// The Channels list constructor initializes the head and
// tail Channel pointers to NULL.
//
// ********************************************************/
Channels :: Channels()
{
    chan_head = NULL;
    chan_tail = NULL;
}



/**********************************************************
// Channel copy constructor for potential future use.
//
// ********************************************************/
Channels :: Channels(const Channels &channels)
{ return; }



/**********************************************************
// Channels destructor traverses the Channel list and
// calls the destructor of each one.
//
// ********************************************************/
Channels :: ~Channels()
{
    // Create a temporary pointer for traversing the Channel list to deallocate memory.
    Channel *chan_temp = NULL;

    // While there is at least one Channel
    while(chan_head)
    {
        // Store the next Channel pointer.
        chan_temp = chan_head->next;
        // Delete the Channel stored by head.
        delete chan_head;
        // Set head to the next Channel pointer
        chan_head = chan_temp;
    }

    // Set both list pointers to NULL.
    chan_head = NULL;
    chan_tail = NULL;
}



/**********************************************************
// The insertChannel function is used to create and insert
// new Channel objects into the Channels list. The new
// Channel is allocated memory using the associated
// parameters, namely chan_name, chan_description, and
// u_group_size, and then initialized by copying over the
// contents of the parameters. Assuming a channel by the
// same name does not already exist, the new channel is
// inserted and true is returned. If a channel by the same
// name does exist, false is returned and the new message
// is deleted.
//
// ********************************************************/
bool Channels :: insertChannel( const char * chan_name,
                                const char * chan_description,
                                const short u_group_size,
                                const User * usergroup )
{
    // Index counter for adding users to a Channel usergroup.
    short u_index = 0;

    // Create a new Channel node.
    Channel * new_chan = new Channel;

    // Allocate memory for the new Channel's name.
    new_chan->channel_name = new char[strlen(chan_name) + 1];
    // Allocate memory for the new Channel's description.
    new_chan->description = new char[strlen(chan_description) + 1];

    // Initialize new channel fields.
    // Copy over the name from the passed
    // name to the new Channel name.
    strcpy(new_chan->channel_name, chan_name);
    // Copy over the description from the passed
    // description to the new Channel description.
    strcpy(new_chan->description, chan_description);

    // Set the number of users for the new Channel.
    new_chan->usr_cnt = u_group_size;
    // Allocate memory for the new Channel's usergroup.
    new_chan->usergroup = new User[u_group_size];

    // While there are unadded users..
    while(u_group_size > u_index)
    {
		// Copy over the username from the passed usergroup to the new Channel usergroup.
        strcpy(new_chan->usergroup[u_index].user_name, usergroup[u_index].user_name);
		// Copy over the email from the passed usergroup to the new Channel usergroup.
        strcpy(new_chan->usergroup[u_index].user_email, usergroup[u_index].user_email);
        // Increment the usergroup index counter.
		++u_index;
    }

    // Temporary Channel pointer for traversal.
    Channel *temp_chan = chan_head;

    // If the list is empty..
    if(!chan_head)
    {
        // Set the head and tail Channel pointers to the new channel (created externally).
        chan_head = new_chan;
        chan_tail = new_chan;
    }
    // Otherwise (the list is not empty)..
    else
    {
	      // Iterate through the list, and check that the name of the new Channel does
        // not exist somewhere else in the Channel list.
        while(temp_chan->next && strcmp(temp_chan->channel_name, new_chan->channel_name))
            temp_chan = temp_chan->next;

        // If a Channel by the same name does exist..
        if(temp_chan && !strcmp(temp_chan->channel_name, new_chan->channel_name))
            // report failure to calling function.
            return false;

        // The channel name is original, so add the new Channel to the back of the list.
        chan_tail->next = new_chan;
        // Update tail.
        chan_tail = chan_tail->next;
    }

    // Report success back to the calling function.
    return true;
}



/**********************************************************
// The postMessage function is used to create and insert
// a new message into a specific channel. Only some of the
// Message components are passed in, and those remaining are
// initialized by this function. The room_id is the name of
// the channel to which the new message should be printed.
// The user_name is used to check that the user posting the
// new message exists in the specified channel's usergroup.
// If the user does exist, then the user_name is coppied over
// to the new Message object's User, and then that User's
// email is coppied over to from the same User in the
// Channel's usergroup. The passed message is used to
// allocate memory for the new Message, and its contents are
// then coppied over.
//
// ********************************************************/
bool Channels :: postMessage( const char * room_id,
                              const char * user_name,
                              const char * message )
{
    // If there are no channels..
    if(!chan_head)
        // Alert the calling function of the failure.
        return false;

    // Stores userInChannel() result. If the user who created
    // new_msg is in the usergroup for the specified room,
    // then user_check == true, and the message can be added.
    bool user_check = false;

    // New message object to be initialized using the passed data
    // and some more data created in this function.
    Message * new_msg = new Message;

    // Copy over the given username to the username in the new message.
    strcpy(new_msg->poster->user_name, user_name);

    // Allocate memory for the new message.
    new_msg->message = new char[ strlen(message) + 1];
    // Copy over the passed message to the new message.
    strcpy(new_msg->message, message);

    // Create a pointer to the first channel for traversal.
    Channel *temp_chan = chan_head;

    // Add a time stamp to the new message.
    new_msg->time_stamp = createTimestamp();

    // While temp_chan exists, and its title is different
    // from the one given by the user, keep traversing.
    while(temp_chan && strcmp(temp_chan->channel_name, room_id))
        temp_chan = temp_chan->next;

    // If temp_chan stopped on anything but the last
    // Channel in the list, then its title is, of
    // course, the same as the room id.
    if(temp_chan)
    {
        // Check if the User who posted the new Message
        // exists in the temp_chan Channel's usergroup.
        user_check = userInChannel(new_msg->poster , temp_chan);

        // If the user exists in the usergroup, insert the message.
        if(user_check)
        {
            // If the list of messages is currently empty..
            if(!temp_chan->msg_head)
            {
                // Set the head and the tail pointers both to the new message.
                temp_chan->msg_head = new_msg;
                temp_chan->msg_tail = new_msg;
            }
            // If the list is not empty..
            else
            {
                // Insert the new message at the back of the list.
                temp_chan->msg_tail->next = new_msg;
                temp_chan->msg_tail = temp_chan->msg_tail->next;
            }
        }
    }

    // If the user does not exist in the usergroup, or temp_chan is not defined..
    if(!temp_chan || !user_check)
	{
        // Delete the new message
	    delete new_msg;	

        // Report the failure.
        return false;
    }
    // Report the success.
    return true;
}



/**********************************************************
// dispChanMsgs is a public function which, given the name
// of some channel, displays the contents of the specified
// channel. Channels are traversed, and once a match is
// found, the channel is printed, and true is returned.
// If no match could be found, false is returned.
//
// ********************************************************/
bool Channels :: dispChanMsgs(const char * channel_name) const
{
    // Create a pointer to the first channel for traversal.
    Channel *temp_chan = chan_head;

    // While temp_chan exists, and its title is different
    // from the one given by the user, keep traversing.
    while(temp_chan && strcmp(temp_chan->channel_name, channel_name))
        temp_chan = temp_chan->next;

    // If temp_chan stopped on anything but the last
    // Channel in the list, then its title is, of
    // course, the same as the room id.
    if(temp_chan)
    {
        // Print the message contents of the selected channel.
        temp_chan->displayMessages();
        // Report success.
        return true;
    }

    // The channel was not found Report the failure.
    return false;
}



/**********************************************************
// dispChanWithKey is a public function which, using the
// ssMatch function, checks for a given keyword in every
// channel description and every message until a match is
// found. If a match is found, the channel is printed, and
// true is returned. Otherwise, false is returned.
//
// ********************************************************/
bool Channels :: dispChanWithKey(const char * keyword) const
{
    // Channel pointer for traversing the channel list.
    Channel *temp_chan = chan_head;

    // Message pointer for traversing the messages in each channel.
    Message *temp_msg = NULL;

    // Loop control variable set to true if a keyword match is
    // found in a description or message. Also returned to the
    // calling routine after the search is completed.
    bool is_match = false;

    // While a match has not been found, and there is at least
    // one channel to search..
    while(temp_chan && !is_match)
    {
        // Check for a match to keyword in the current Channel's description.
        is_match = ssMatch(keyword, temp_chan->description);

        // Update temp_msg to the current Message in the current Channel.
        temp_msg = temp_chan->msg_head;

        // If a match was not found in the current Channel desctiption,
        // then while a match is not found, and while there are unsearched
        // messages, search for a match in the current Channel's message list.
        while(temp_msg && !is_match)
        {
            // Check for a match to keyword in the current Message of the current Channel.
            is_match = ssMatch(keyword, temp_msg->message);

            // Set the temporary message pointer to the next message in the list.
            temp_msg = temp_msg->next;
        }

        // If a match was not found in the current Channel description,
        // or the current Channel messages.. It's important that temp_chan is
        // only updated if a match is not found, as it is used to print the
        // channel after the loop completes (assuming a match is found).
        if(!is_match)
            // Set the channel pointer to the next channel.
            temp_chan = temp_chan->next;
    }

    // If a match has been found, print the Channel.
    if(is_match)
    {
        // Print alert that a match has been found, as well as metadata
        // for the Channel in which it was found.
        std::cout << std::endl
                  << "  Keyword Match Found in " << temp_chan->channel_name << '!'
                  << std::endl;

        // Display the messages for the Channel in which the match was found.
        dispChanMsgs(temp_chan->channel_name);
    }

    // Return the result to the calling routine.
    return is_match;
}



/**********************************************************
// ssMatch is a private function which uses an
// istringstream to traverse channel descriptions and
// messages in search of some keyword. The passed keyword
// is compared with every word in the passed string until
// a match is found. If no match is found, then false is
// returned.
//
// ********************************************************/
bool Channels :: ssMatch( const char * keyword,
                          const char * string ) const
{
    // Loop control variable for traversing the string in
    // search of a specific keyword.
    bool is_match = false;

    // Create a buffer for storing individual words in the
    // given string.The memory allocated to the buffer is
    // (at least currently -- in the test file) equal to
    // the memory allocated to the keyword[].
    char *ss_buffer = new char[MAX_KEYWORD_LEN + 1];

    // Create a string stream with the string given string.
    std::istringstream string_stream(string);

    // Search for keyword in current message in the string stream.
    while((string_stream >> ss_buffer) && !is_match)
        // If the current word (extracted from the string stream)
        // is the same as the specified keyword..
        if(!strcmp(ss_buffer, keyword))
            // Set the loop control variable to true, thereby ending
            // the search and returning
            is_match = true;

    // Clear any remaining text in the string stream.
    string_stream.clear();

    // Deallocate the memory used to store
    // current words in the string.
    delete[] ss_buffer;

    // Return the result to the calling routine.
    return is_match;
}



/**********************************************************
// This function is used used to display the names of all
// of the channels in the list which contain the passed
// user_name in their usergroup. true is returned if the
// user is found in at least one channel.
//
// ********************************************************/
bool Channels :: dispChanWithUsr(const char * user_name) const
{
    // Index counter for the usergroup array.
    short member_cnt = 0;

    // Search channel descriptions
    Channel *temp_chan = chan_head;

    // Loop control variable. Set to true if the user
    // exists in the Channel's usergroup. Note that this
    // is reset to false after a user is found i.e. This
    // should not be returned.
    bool user_match = false,
         // once user_match has been set to true, user_matched
         // is set to true and NEVER changes. This is used to
         // return a success message to the calling routine.
         user_matched = false;

    // While there are unsearched usergroups..
    while(temp_chan)
    {
        // Loop through the users in temp_chan
        // and try to match some_user.
        do
        {
            // Set user_match to the result of the username of the specified user
            // compared with the user at the current index of the usergroup.
            user_match = !strcmp(user_name, temp_chan->usergroup[member_cnt++].user_name);
        }
        // Continue until the array is exhausted, or a match is found.
        while((member_cnt < temp_chan->usr_cnt) && !user_match);

        // If a match has been found, print the Channel.
        if(user_match)
        {
            // Print alert that a match has been found, as well as metadata
            // for the Channel in which it was found.
            std::cout << std::endl
                    << "\n  User match found in Channel: " << temp_chan->channel_name
                    << std::endl << std::endl;

            // Reset user_match to false to check the next channel.
            user_match = false;

            // Set/preserve user_matched to/as true.
            // Yes, this is done redundantly.. I know. :<
            user_matched = true;
        }

        // Update temp_chan to the next Channel.
        temp_chan = temp_chan->next;

        // Reset Channel member counter to 0.
        member_cnt = 0;
    }

    // Return the result.
    return user_matched;
}



/**********************************************************
// The public removeOldMsgs function takes a timestamp
// of the form dd/mm/yyyy, and calls the private
// removeOldMsgs with the first channel, and an initial
// removed messages counter. Once all old messages have
// been removed from all channels, the total number of
// removed messages is returned.
//
// ********************************************************/
short Channels :: removeOldMsgs(const char * time_stamp)
{
    return removeOldMsgs(time_stamp, chan_head, 0);
}



/**********************************************************
// The pivate removeOldMsgs function takes a timestamp of
// the form dd/mm/yyyy, a reference to a channel poininter,
// and a counter. For every message in the passed channel,
// if the timestamp of that message is older than the passed
// timestamp (given by timeStampCmp), then delete it. Because
// messages are added to the end of the list, stop once a
// newer message is encountered. After the old messages have
// been deleted, return a recursive call with the next channel
// pointer and the updated deleted message counter. Return
// the deleted message counter once the channel is NULL.
//
// ********************************************************/
short Channels :: removeOldMsgs( const char * time_stamp,
                                 Channel *& head,
                                 short deleted )
{
    // If chan_head has not been defined..
    if(!head)
        // Return the number of deleted messages.
        return deleted;

    // Temporary pointer for traversing messages.
    Message *msg_temp = head->msg_head;

    // While the message exists, its timestamp is defined, and the timestamp is
    // older than the specified time_stamp..
    while(msg_temp && msg_temp->time_stamp && timeStampCmp(msg_temp->time_stamp, time_stamp))
    {
        // Store the next message.
        msg_temp = head->msg_head->next;
        // Delete the old message.
        delete head->msg_head;
        // Update the message head to point to the next message.
        head->msg_head = msg_temp;

        // Increment the deleted message counter.
        ++deleted;
    }

    // If the current list of messages is empty..
    if(!head->msg_head)
        // Ensure that the current message tail points to NULL.
        head->msg_tail = NULL;

    // Call this function again with the same time stamp, the next
    // channel in the list, and the updated deleted message counter.
    return removeOldMsgs(time_stamp, head->next, deleted);
}



/**********************************************************
// Compare two time stamps. Return true if time_stamp_1
// is older than time_stamp_2.
//
// ********************************************************/
bool Channels :: timeStampCmp( const char * time_stamp_1,
                               const char * time_stamp_2 ) const
{
          // Short integer cast values in the time stamp arrays.
    short current_digit_1 = 0,
          current_digit_2 = 0,
          // The offset from the current index of the ords
          // array used to compare indices in the time stamps.
          offset = 0,
          // The current index of the ords array to be used in
          // comparing the indices of the time_stamps.
          index  = 0,
          // An array of section separaters for time stamps.
          // [0] => year, [1] => month, [2] => day.
          * ords = new short[3];

    // The time stamp's year starts at index 6.
    ords[0] = 6;
    // The time stamp's month starts at index 3.
    ords[1] = 3;
    // The time stamp's day starts at index 0.
    ords[2] = 0;

         // Loop control variable for comparing time stamps.
    bool age_confirmed = false,
         // Set to true when age is confirmed.
         is_older = false;

    // Until the relative age of the timestamps has been confirmed..
    while(!age_confirmed)
    {
        // Case the current index (by offset from current ord,
        // not by iteration) of each timestamp to short int, store.
        current_digit_1 = static_cast<short>( time_stamp_1[ ords[ index ] + offset ] - '0' );
        current_digit_2 = static_cast<short>( time_stamp_2[ ords[ index ] + offset ] - '0' );

        // If the current numbers of the indices of the time stamps
        // are not the same..
        if( current_digit_1 > current_digit_2 || current_digit_1 < current_digit_2  )
        {
            // Set confirmed to true so that the loop can discontinue.
            age_confirmed = true;
            // If the first is not older than the second, set isolder
            // to false. Otherwise, set to true.
            is_older = !(current_digit_1 > current_digit_2);
        }
        // Otherwise..
        else
            // Increment the offset to check the next offset/section
            // of the timestamps.
            ++offset;

        // If the current section of the timestamps has been fully
        // checked, reset the offset and increment the ords index
        // counter.
        if( (offset == 2 && index == 1) || (offset == 4 && index == 0) )
        {
            // Reset the index offset.
            offset = 0;
            // Increment the ords index counter.
            ++index;
        }
        // Otherwise, if the timestamps are the same..
        else if(offset == 2 && index == 2)
        {
            // Set confirmed to true so that the loop can discontinue.
            age_confirmed = true;
            // The first timestamp is not older than the second.
            is_older = false;
        }
    }

    // Delete the ords array.
    delete[] ords;
    // Return the result of the compared timestamps.
    return is_older;
}



/**********************************************************
// This is the public displayChannelNames function. This
// function calls the private function with the first
// channel, and an initial counter.
//
// ********************************************************/
bool Channels :: displayChannelNames() const
{
    // If the list of channels is empty, report failure.
    if(!chan_head)
        return false;

    // Display a header before printing the list.
    std::cout << "\n  Displaying all channel names: " << std::endl;

    // Call the private display function with the first channel.
    return displayChannelNames(chan_head, 1);
}



/**********************************************************
// This is the private displayChannelNames function. After
// being called by the public displayChannelNames function,
// this channel prints out the name of the channel that was
// passed in as an argument, and returns displayChannelNames
// given the next channel pointer. Once the channel is NULL,
// all Channels have been printed.
//
// ********************************************************/
bool Channels :: displayChannelNames( const Channel * head,
                                      short count ) const
{
    // If head is NULL, report success. No need to return false
    // because actual head is checked in public function.
    if(!head)
        return true;

    // Print out the channel name.
    std::cout << "\n    Channel #" << count << " => " << head->channel_name << std::endl;

    // Call display with the next Channel.
    return displayChannelNames(head->next, ++count);
}



/**********************************************************
// The userInChannel function is a private member of the
// Chanels class that is used in checking whether a
// specific user exists in the usergroup of some spefific
// Channel. The function is called with a reference to a
// User pointer, which is the user that is being checked
// for in the supplied Channel. The user is passed by
// reference because if the user exists, its email member
// needs to be updated. Finally, if the user exists, the
// function returns true. Otherwise, the function returns
// false.
//
// ********************************************************/
bool Channels :: userInChannel( User *& some_user,
                                const Channel * temp_chan ) const
{
    // Index counter for the usergroup array.
    short member_cnt = 0;

    // Loop control variable. Set to true if the user
    // exists in the Channel's usergroup.
    bool user_match = false;

    // If temp_chan is NULL, or its usergroup is NULL..
    if(!temp_chan || !temp_chan->usergroup)
        // Report that the user does not exist in the
        // (maybe) Channel's usergroup.
        return false;

    // Loop through the users wqin temp_chan and try to match some_user.
    do
        // Set user_match to the result of the username of the specified user
        // compared with the user at the current index of the usergroup.
        user_match = !strcmp(some_user->user_name, temp_chan->usergroup[member_cnt++].user_name);
    // Continue until the array is exhausted, or a match is found.
    while((member_cnt < temp_chan->usr_cnt) && !user_match);

    // If a match was found..
    if(user_match)
        // The Message struct currently contains its own User object,
        // but when a message is created, only a name is read in. Because
        // a match has been found, the email from the User match in the
        // Channel object can be coppied over to the User in the message.
        // At some point, I would like to change this so that the user in
        // a message points to the user in the Channel's usergroup.
        strcpy(some_user->user_email, temp_chan->usergroup[member_cnt-1].user_email);

    // Return the result.
    return user_match;
}



/**********************************************************
// The create time stamp function is called every time a
// new message is created. Note that the Message struct's
// timestamp pointer is allocated here. Timestamps are
// really date stamps in the form Day/Month/Year.
//
// ********************************************************/
char * Channels :: createTimestamp() const
{
    // Create a char array to hold the time stamp
    char *time_stamp = new char[MAX_TIME_LEN + 1];
    // Get the time/date.
    time_t t = time(0);

    // Store the day/month/year in the time stamp.
    strftime(time_stamp, MAX_TIME_LEN, "%d/%m/%Y", localtime(&t));

   // Return the time stamp.
    return time_stamp;
}
