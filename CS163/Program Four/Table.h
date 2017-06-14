/*****************************************************************
// File Name:  Table.h
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       June 2nd, 2017
// Class:      CS163
// Program #:  4
// 
// Overview: This is the header file for the Table, implemented
// as a binary search tree, for use in the fourth programming
// assignment. This file also contains the declaration for the
// Node struct, which is used as the tree node. More information
// for the Tree and Node can be found in the Table.cpp file. For
// more information about the Events, which serve as the Nodes'
// data, see Event.cpp/h.
//
// ***************************************************************/


#ifndef TABLE_H
#define TABLE_H

#include "Event.h" // Includes iostream, cstring.


/* ********************************************
// Node structure for use by the Table class.
//
// ********************************************/
struct Node
{
  // The event.
  Event data;

  // Children of the node.
  Node * left,
       * right;

  // Default constructor sets pointers to NULL.
  Node(void);

  // Destructor deallocates event
  // data, sets pointers to NULL. 
  ~Node(void);
};



/* ********************************************
// A table implemented as a binary search tree.
//
// ********************************************/
class Table
{
  public:

    // Initializes the tree members.
    Table(void);

    // Copy constructor for future use.
    Table(const Table & table);

    // Deallocate all dynamic memory in the tree.
    ~Table(void);

    // Inserts a new event into the tree by name.
    int insert(const Event & new_event);

    // Call the private retrieve function on root.
    bool retrieve( char * name,
                   Event & event_match ) const;

    // Calls the private display function on root.
    int display(void) const;

    // Calls the private removeEventName function.
    bool removeEventName(char * name);

    // Calls the private removeEventKey function.
    int removeEventKey(const char * keyword);

    // Return the height of the tree.
    int getHeight(void) const;

  private:

    // Inserts a new event into the tree by name.
    int insert( Node *& new_node,
                Node *& root);

    // Find an event by name and copy
    // the event info into event_match.
    bool retrieve( char * name,
                   Event & event_match,
                   Node * root ) const;

    // Displays all events in alphabetical order.
    int display(Node * root) const;

    // Removes the event with the given name.
    bool removeEventName( char * name,
                          Node *& root );

    // Removes all events which contain the specified keyword.
    int removeEventKey( const char * keyword,
                        Node *& root);

    // Deallocate and nullify - postorder.
    int deleteTree(Node *& root);

    // Calculate and return the height of the tree.
    int calcHeight(Node * root) const;

    // Return true if the node param is a leaf.
    bool isLeaf(Node * node) const;

    // Return a pointer to the char[] with greater
    // alphabetical prescedence. e.g. (a,b) => a,
    // (ng, nc) => nc, etc..
    char * min(char * first, char * second) const;

                 // The current height of the tree.
    unsigned int height,
                 // Number of nodes in the tree.
                 current_size;

    // The root of the tree.
    Node * root;

};
#endif // TABLE_H
