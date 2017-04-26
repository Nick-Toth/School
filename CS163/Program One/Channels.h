/*****************************************************************
// File Name:  Channels.h
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       4/19/2017
// Class:      CS163
// Program #:  1
//
// Overview: This is the header file for the Channels list class.
// The Channels list is a linked list of Channel objects, each of
// which contains a linked list of messages. The structs that are
// used to create these lists, namely User, Message, and Channel,
// are declared in this file, and defined in the Channels.cpp file.
//
// ***************************************************************/

#include <iostream>
#include <cstring>
#include <ctime> // Used to create timestamps.

// The stringstream library is used to stream individual words
// in the channel descriptions and messages. See ssMatch().
#include <sstream>


// ******************************  |
// Program Array Size Constants *  |
// ******************************  V

const short MAX_DESCRIPTION_LEN = 512, // The maximum length for a Channel description.
            MAX_KEYWORD_LEN     = 56,  // The maximum length for a keyword to be searched.
            MAX_EMAIL_LEN       = 128, // The maximum length for a user's email address.
            MAX_NAME_LEN        = 128, // The maximum length for a user's name.
            MAX_TIME_LEN        = 11,  // The maximum length for a time stamp.
            MAX_MSG_LEN         = 512; // The maximum length for a message.


// *******************************************  |
// Structure declarations for Channels class *  |
// *******************************************  V


/**********************************************************
// User structure declaration. Users are stored in each
// Channel's usergroup, as well as each message, so that
// channels can set access limits. Implementation and
// function comments in Channels.cpp.
//
// ********************************************************/
struct User
{
    // User constructor.
    User();

    // User destructor 
    ~User();

           // Pointer to the user name of this User.
    char * user_name,
           // Pointer to the email address of this User.
         * user_email;
};



/**********************************************************
// Message structure declaration. Each Message is stored
// in a linked list in a specific channel. New messages are
// stored at the end of the list, which allows for quick
// insertion using the tail pointer, as well as quick
// removal of old messages by traversing from head until the
// first newer message. Implementation and function comments
// in Channels.cpp.
//
// ********************************************************/
struct Message
{
    // Message constructor.
    Message();

    // Message destructor.
    ~Message();

    // Displays message & metadata.
    bool printMessage(short count) const;

    // The user who posted this message.
    User * poster;

           // The actual message posted by the user.
    char * message,
           // The date (dd/mm/yyyy) that themessage was posted.
         * time_stamp;

    // Pointer to the next message
    Message * next;
};



/**********************************************************
// Channel structure declaration. Each Channel is stored
// in a linked list in the Channels class. New Channels,
// like Messages, are inserted at the end of the list.
// Implementation and function comments in Channels.cpp.
//
// ********************************************************/
struct Channel
{
    // Channel constructor.
    Channel();

    // Channel destructor.
    ~Channel();

    // Calls the private displayMessages function with initial arguments.
    bool displayMessages() const;

    // An array of users who can post messages to the Channel.
    User * usergroup;

    // The number of users in the channel. Used to traverse
    // the usergroup in search of a specific user.
    short usr_cnt;

           // The name of the channel.
    char * channel_name,
           // A brief description of the Channel's purpose.
         * description;

              // The first (oldest) message posted to the Channel.
    Message * msg_head,
              // The last (newest) message posted to the Channel.
            * msg_tail;

    // The next (potential) Channel in the Channels list.
    Channel * next;

  private:

    // Prints out all of the messages
    bool displayMessages( Message * head,
                          short count ) const;
};



/*******************************************
// Declaration for a list of Channel objects,
// and a set of functions for managing that
// list.
//
// *****************************************/
class Channels
{
    public:

        // Constructor for initializing Channel objects.
        Channels();

        // Copy constructor for potential future modification of this program.
        Channels(const Channels & channels);

        // Destructor for deallocating Channel memory.
        ~Channels();

        // Add a new channel object to the channels list.
        bool insertChannel( const char * chan_name,
                            const char * chan_description,
                            const short u_group_size,
                            const User * usergroup );

        // Post a new message to the channel with the specified id.
        bool postMessage( const char * room_id,
                          const char * user_name,
                          const char * message );

        // Display all of the messages in some channel.
        bool dispChanMsgs(const char * channel_name) const;

        // Display all channels whose description or messages contain the specified keyword.
        bool dispChanWithKey(const char * keyword) const;

        // Display all channels with the specified user.
        bool dispChanWithUsr(const char * user_name) const;

        // Remove messages with timestamps from before the specified time_stamp.
        short removeOldMsgs(const char * time_stamp);

        // Display a list of the names of channels in the list.
        bool displayChannelNames() const;

    private:

        // Private remove message function for recursive removal.
        short removeOldMsgs( const char * time_stamp,
                             Channel *& head,
                             short deleted );

        // Private display channel function for recursive printing.
        bool displayChannelNames( const Channel * head,
                                  short count ) const;

        // Check if a user exists in a Channel's usergroup. If the user does
        // exist, some_user's email is updated with the email in temp_chan
        // for reasons explained in userInChannel's implementation.
        bool userInChannel( User *& some_user,
                            const Channel * temp_chan ) const;

        // Use a string stream to check whether a char[] contains some keyword.
        bool ssMatch( const char * keyword,
                      const char * string ) const;

        // Create a time stamp to add to messages.
        char * createTimestamp() const;

        // Compare two time stamps. Return true if time_stamp_1 is older than time_stamp_2.
        bool timeStampCmp( const char * time_stamp_1,
                           const char * time_stamp_2 ) const;

                // The first Channel object in the list.
        Channel * chan_head,
                // The last Channel object in the list.
                * chan_tail;
};
