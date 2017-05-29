/*****************************************************************
// File Name:  Event.h
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 24nd, 2017
// Class:      CS163
// Program #:  3
//
// Overview: This is the declaration file for the Event struct,
// which serves as the main collection of data for programming
// assignment #3. See function comments in the Event.cpp file for
// more information.
//
// ***************************************************************/


#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <cstring>


// This is only here because of the 5 keyword limit - 
// so that this program could be quickly modified for
// more keywords in the future.
const unsigned short MAX_KEYWORDS = 5;


struct Event
{
    // Default constructor for setting all data to NULL.
    Event(void);
    // Copy constructor - calls the copy function.
    Event(const Event & event);
    // Deallocate all event data.
    ~Event(void);

    // Display all information about the event.
    // Return -1 if empty. Otherwise, return the
    // number of keywords.
    int display(void) const;

    // Copy the contents of another event.
    int copy(const Event & event);

    // Return the number of non void keywords.
    int keywordCount(void) const;

           // The event title.
    char * name,
           // The location of the event.
         * location,
           // That date in which the event will take place.
           // yyyy-mm-dd
         * date,
           // The event start time.
           // (24 hour time. 0 - 2359 )
         * start_time,
           // A brief description of the event.
         * description,
           // Array of char pointers. For i (0 - MAX_KEYWORDS),
           // keywords[i] = a single keyword.
         * keywords[MAX_KEYWORDS];
};
#endif // EVENT_H
