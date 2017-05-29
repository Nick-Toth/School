/*****************************************************************
// File Name:  Table.h
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 24nd, 2017
// Class:      CS163
// Program #:  3
//
// Overview: This is the declaration file for the hash table for
// programming assignment #3. Essentially, this hash table is
// used to store pointers to Events (allocated in a linked list),
// using event keywords. See function comments in the Table.cpp
// file for more information.
//
// ***************************************************************/


#ifndef TABLE_H
#define TABLE_H

#include "Event.h" // Includes iostream, cstring.
#include <cmath> // sqrt for nextPrime.


// The default size of the hash table - If no
// size is passed to the table constructor.
const unsigned int DEFAULT_TABLE_SIZE = 2053;



/* ********************************************
// Linked list node for use in a chaining hash
// table of Events.
//
// ********************************************/
struct Node
{
  // Pointer to an event.
  Event * data;
  // Pointer to the next event in the chain.
  Node * next;

  // Default constructor sets pointers to NULL.
  Node(void);

  // Destructor deallocates event
  // data, sets pointers to NULL. 
  ~Node(void);
};



/* ********************************************
// Linked list node for dealing with event
// memory.
//
// ********************************************/
struct ENode
{
  // The event.
  Event data;

  // Pointer to the next ENode.
  ENode * next;

  // Default constructor.
  ENode(void):next(NULL){}

  // Destructor deallocates event
  // data, sets pointers to NULL. 
  ~ENode(void);
};



/* ********************************************
// A chaining hash table of Events.
//
// ********************************************/
class Table
{
  public:

    // Default constructor allocates memory
    // for the hash table. Sets pointers to NULL.
    Table(unsigned int size = DEFAULT_TABLE_SIZE);
    // Copy constructor for future use.
    Table(const Table & table);
    // Deallocate all events in the hash table.
    ~Table(void);

    // Inserts a new event into the hash table.
    int insert(const Event & new_event);

    // Searches for events with the specified keyword. Stores
    // results in the event_match array, up to the maximum size.
    int retrieve( const char * keyword,
                  Event event_match[],
                  int & found ) const;

    // Displays all events with the specified keyword.
    int display(const char * keyword) const;
    // Displays all events.
    int display(void) const;

    // Removes all events which are older than the specified date.
    int removeOld(const char * time_stamp);

  private:

    // Returns an integer index based on the given key.
    unsigned int hash(const char * key) const;

    // Returns whatever prime number exceeds the passed number.
    int nextPrime(unsigned int number) const;

    // Compare dates, return true if
    // fst_date is older than snd_date
    bool datecmp( const char * fst_date,
                  const char * snd_date ) const;

                 // Number of Event chains in the table.
    unsigned int table_size,
                 // The prime number to be used in the hash table.
                 hash_prime,
                 // Number of inserted Events (Not the number of nodes!!).
                 current_size;

    // Hash table of event pointers.
    Node ** hash_table;

    // List of events. (where the events are allocated).
    ENode * events;

};
#endif // TABLE_H
