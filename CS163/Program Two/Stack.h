/*****************************************************************
// File Name:  Stack.h
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 10th, 2017
// Class:      CS163
// Program #:  2
//
// Overview: This is the declaration file for the Stack ADT for
// use in the airport management assignment. The purpose of the
// stack is to manage passengers as they board the plane. See the
// declaration comments and the Stack.cpp file for more information.
//
// ***************************************************************/

#ifndef STACK_H
#define STACK_H

#include "Passenger.h"
#include "Util.h"

/* *******************************************
// Node definition for the Stack. Each instance
// of the stack contains two linked lists of
// arrays, representing each side of the plane.
// Nodes represent rows (on one side) and the
// array represents the seats for that row.
//
// *******************************************/
struct Row
{
  // Array of passengers in this row.
  Passenger * passengers;

  // Pointer to the next Row for the same side.
  Row * next;
};



/* *******************************************
// The stack is used to represent the plane in
// the airport management assignment. Each
// stack contains two sides (left rows and
// right rows), where each side is a linked
// list of arrays, and each row is an array.
//
// *******************************************/
class Stack
{
  public:

    // Initializes the stack's data member(s).
    Stack(short seats = 0);
    // Copies the passed Stack's data.
    Stack(const Stack & stack);
    // Calls the eraseStack function on head.
    ~Stack(void);

    // Insert a new Passenger at the head of the stack.
    int push(const Passenger & passenger);

    // Remove the passenger
    int pop(void);

    // Copy the head passenger into the
    int pop(Passenger & passenger);

    // Stores the passenger
    int peek(Passenger & passenger);

    // True if head is NULL.
    bool isEmpty(void) const;

    // Display each half of the stack.
    bool display(void);

    // Set the number of passengers in the stack.
    bool setNumPassengers(short count);


  private:

    // Deallocates memory for the right
    // or left side of the stack.
    int eraseHalf(Row *& row);

    // Sets the passed row pointer to the
    // row pointer for the current side.
    short getCurrentRow(Row *& row);

    // Sets the top counter for a specific
    // side, given a pointer to that side.
    bool setSideCounter(Row * row);

    // Updates the head for a specific side.
    int updateHead(Row *& old_head);

    // Displays half of the seats.
    bool display(Row * row);

    // Return the opposite head of old_head.
    Row * getOther(Row * old_head);

        // Pointer to the left head Row node
    Row * l_row_head,
        // Pointer to the right head Row node.
        * r_row_head;

          // The top index of the left head Row node.
    short l_top_index,
          // The top index of the right head Row node.
          r_top_index,
          // The number of seats in a row (1 side).
          row_size,
          // The max number of rows.
          num_passengers;

    // Used to switch between sides in the pop function.
    // true => right, false => left.
    bool side_switch;

};
#endif // STACK_H
