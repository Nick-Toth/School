/*****************************************************************
// File Name:  Stack.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 10th, 2017
// Class:      CS163
// Program #:  2
//
// Overview: This is the implementation file for the Stack ADT for
// use in the airport management assignment. The purpose of the
// stack is to manage passengers as they board the plane. For
// information regarding specific functionality, see the function
// header comments in this file.
//
// ***************************************************************/

#include "Stack.h"


/* ******************************************
// Initializes any and all Stack data members
// and pointers.
//
// @param seats: The number of seats in a
// given row.
//
// ******************************************/
Stack :: Stack(short seats)
{
  // Set all stack list pointers to NULL.
  l_row_head = NULL;
  r_row_head = NULL;

  // Init top indices to 0.
  l_top_index = 0;
  r_top_index = 0;

  // Set row size to the value of the seats parameter.
  row_size = seats;

  // Initialize side switch to true (right).
  side_switch = true;

  return;
}



/* ******************************************
// Copy constructor for potential future use.
//
// ******************************************/
Stack :: Stack(const Stack & stack)
{ return; }



/* ******************************************
// Calls the eraseHalf function on each half
// of the plane.
//
// ******************************************/
Stack :: ~Stack(void)
{
  // Delete the left side.
  eraseHalf(l_row_head);
  l_row_head = NULL;

  // Delete the right side.
  eraseHalf(r_row_head);
  r_row_head = NULL;
}



/* ******************************************
// Copies the contents of the given Passenger
// object into the Passenger at the top of
// the stack. If the current stack, which
// represents one side of one row of the
// plane, is full, then a new Row node is
// created and initialized with the given
// passenger.
//
// ******************************************/
int Stack :: push(const Passenger & passenger)
{
  // True if passenger goes in the right (even) side.
  bool right_side = parseInt(passenger.seat_num) % 2 == 0;
  // Get the side of the stack to put the passenger.
  Row * push_row = right_side? r_row_head : l_row_head;
  // Get the top for the current side.
  short top = right_side? r_top_index : l_top_index;

  // New row pointer -- If necessary.
  Row * new_row;

  // If the current row is NULL or full..
  if(!push_row || top > row_size - 1)
  {
    // Create a new row.
    new_row = new Row;
    // Set its next row to NULL.
    new_row->next = NULL;

    // Allocate memory for the new row's passengers.
    new_row->passengers = new Passenger[row_size];

    // Add the given passenger to the new row.
    new_row->passengers[0].copyPassenger(passenger);

    // Update the corresponding top index.
    if(right_side) r_top_index = 1;
    else l_top_index = 1;
  }

  // If the push_row is NULL, or the top is past max..
  if(!push_row)
  {
    // Set the corresponding side's top index to 1.
    if(right_side)
      r_row_head = new_row;
    else
      l_row_head = new_row;

    // Return code 1.
    return 1;
  }

  // If the current row is full..
  if(top > row_size - 1)
  {
    // Update the next row to the current head.
    new_row->next = push_row;

    // Update the right head pointer to the new row.
    if(right_side)
        // Update first as the new row.
        r_row_head = new_row;
    // Update the left head pointer to the new row
    else
        // Update first as the new row.
        l_row_head = new_row;

    // Return code 2.
    return 2;
  }

  // Add the given passenger to the top of their row.
  push_row->passengers[top].copyPassenger(passenger);

  // Increment the corresponding top index.
  if(right_side) ++r_top_index;
  else ++l_top_index;

  // Return code 0.
  return 0;
}



/* ******************************************
// Deletes the top passenger.
//
// @param passenger: The location to set the
// data from the top of the current row.
//
// @return: 0 - Pop successful.
//          1 - Empty stack.
//
// ******************************************/
int Stack :: pop(void)
{
  // Row pointer to the current row side.
  Row * current_row;
  // Get the current row, and its top index.
  short top = getCurrentRow(current_row);

  // If the current row is NULL, try the other side.
  if(!current_row)
  {
    // Update the side switch.
    side_switch = !side_switch;
    // Check the other side.
    top = getCurrentRow(current_row);
  }

  // If the other side is NULL..
  if(!current_row)
  {
    // Reset side_switch to its original value.
    side_switch = !side_switch;
    // Report error.
    return 1;
  }

  // Decrement the top index (to get the last added value).
  --top;

  if(top < 0) return 3;

  // Update the counter for the current side.
  setSideCounter(current_row);

  // If the top index is less than 0..
  if(top == 0)
  {
    // Store the current row.
    Row * temp = current_row;

    // If there is a row after current..
    if(temp->next)
      // Update the current row's head
      // pointer to the next row.
      updateHead(temp);

    // Delete and nullify the old row.
    delete temp;
    temp = NULL;
  }

  // Update the side switch.
  side_switch = !side_switch;

  return 0;
}



/* ******************************************
// Sets the given passenger to the top
// passenger of the current row, then
// deletes the top passenger.
//
// @param passenger: The location to set the
// data from the top of the current row.
//
// @return: 0 - Pop successful.
//          1 - Empty stack.
//          2 - Copy failed.
// 
// ******************************************/
int Stack :: pop(Passenger & passenger)
{

  // Row pointer to the current row side.
  Row * current_row;

  // Get the current row, and its top index.
  short top = getCurrentRow(current_row);

  // If the current row is NULL, try the other side.
  if(!current_row)
  {
    // Update the side switch.
    side_switch = !side_switch;
    // Check the other side.
    top = getCurrentRow(current_row);
  }

  // If the other side is NULL..
  if(!current_row)
  {
    // Reset side_switch to its original value.
    side_switch = !side_switch;
    // Report error.
    return 1;
  }

  // Decrement the top index (to get the last added value).
  --top;

  if(top < 0) return 3;

  // Get the top passenger.
  passenger.copyPassenger(current_row->passengers[top]);

  // Update the counter for the current side.
  setSideCounter(current_row);

  // If the top index is less than 0..
  if(top == 0)
  {
    // Store the current row.
    Row * temp = current_row;

    // If there is a row after current..
    if(temp->next)
      // Update the current row's head
      // pointer to the next row.
      updateHead(temp);

    // Delete and nullify the old row.
    delete temp;
    temp = NULL;
  }

  // Update the side switch.
  side_switch = !side_switch;

  return 0;
}



/* ******************************************
// Sets the given passenger to the top
// passenger of the current row.
//
// @param passenger: The location to set the
// data from the top of the current row.
//
// @return: 0 - Peek successful.
//          1 - Empty stack.
//          2 - Copy failed.
//
// ******************************************/
int Stack :: peek(Passenger & passenger)
{
  // Row pointer to the current row side.
  Row * current_row;
        // Get the current row, and its top index.
  short top = getCurrentRow(current_row);

  // If the current row is NULL, try the other side.
  if(!current_row)
  {
    // Change the side switch.
    side_switch = !side_switch;
    // Get the other side.
    top = getCurrentRow(current_row);
    // Reset the side switch.
    side_switch = !side_switch;
  }

  // If the other side is NULL..
  if(!current_row) return 1;

  // Decrement the top index (to get the last added value).
  --top;

  // If the top index is less than 0..
  if(top < 0)
  {
    // If the current row has a successor..
    if(current_row->next)
      // Copy the top passenger into the passenger parameter.
      passenger.copyPassenger(current_row->next->passengers[row_size-1]);
    // Otherwise, report empty stack.
    else return 1;
  }
  else
    // Get the top passenger.
    passenger.copyPassenger(current_row->passengers[top]);

  return 0;
}



/* ******************************************
// Updates the head pointer == to the old_head
// pointer to its next node.
//
// ******************************************/
int Stack :: updateHead(Row *& old_head)
{
  // If the old head pointer points to the right head pointer..
  if(old_head == r_row_head)
  {
    // Update it to the next right row.
    r_row_head = r_row_head->next;
    // Return code 0.
    return 0;
  }

  // If the old head pointer points to the left head pointer..
  else if(old_head == l_row_head)
  {
    // Update it to the next left row.
    l_row_head = l_row_head->next;
    // Return code 1.
    return 1;
  }

  // Report failure.
  else return 2;
}



/* ******************************************
// Checks whether or not the stack is empty.
//
// @return: True if the stack is empty.
//
// ******************************************/
bool Stack :: isEmpty(void) const
{
  // Check whether both of the head pointers
  // are NULL. If they are, return true.
  if(!l_row_head && !r_row_head) return true;

  // Stack isn't empty - return false.
  return false;
}



/* ******************************************
// Checks whether the list is empty, and, if
// it isn't, calls the private display
// function with the left and right nodes.
//
// @return: True if not empty.
//
// ******************************************/
bool Stack :: display(void)
{
  bool r_return = false;
  // If the stack is empty, return false.
  if(!r_row_head && !l_row_head) return false;

  // Display header message.
  std::cout << "\n\t Printing right half of the plane." << std::endl;
  r_return = display(r_row_head);

  // Display header message.
  std::cout << "\n\tPrinting left half of the plane." << std::endl;

  // Print the left and report success.
  return r_return && display(l_row_head);
}



/* ******************************************
// Prints the passengers on a half of the
// plane.
//
// @return: True if not empty.
//
// ******************************************/
bool Stack :: display(Row * row)
{
  

  // Passenger counters for traversal.
  short passenger_cnt = 0,
        seat_cnt = 0;

  // While there are passengers to print.
  while(row)
  {
    // Display the next passenger.
    row->passengers[seat_cnt].display();

    // Increment traversal counters.
    ++seat_cnt;
    ++passenger_cnt;

    // If the seat counter hits the max..
    if(seat_cnt == row_size)
    {
      // Update temp.
      row = row->next;
      // Reset the seat counter.
      seat_cnt = 0;
    }
  }

  // Report success.
  return true;

}



/* ******************************************
// Swaps the row sides. Returns the alternate
// side's head pointer.
// ******************************************/
Row * Stack :: getOther(Row * old_head)
{
  // If old head points to the right side, set it to the left side.
  if(old_head == r_row_head) return l_row_head;
  // Otherwise set it to the left side.
  return r_row_head;
}



/* ******************************************
// Private function for deallocating the
// memory for a row (passed in by the 
// destructor).
//
// ******************************************/
int Stack :: eraseHalf(Row *& row)
{
  // temp row pointer for storing
  // next node before deleting.
  Row * temp;

  // While there are passengers to print.
  while(row)
  {
    // Store the next row.
    temp = row->next;

		// Delete the passengers in the current row.
		delete[] row->passengers;

    // Delete the current row.
    delete row;
    row = NULL;

    // If there are still row(s) to delete..
    // set row to the next row.
    if(temp) row = temp;
  }

  // Nullify pointers.
  row = NULL;
  temp = NULL;
  // Report success.
  return 0;
}



/* ******************************************
// Sets the passed row pointer to the row
// pointer for the current side..
//
// @param row: The row to be updated with the
// row pointer for the current side (given by
// side_switch)
//
// @return: The top index of the current
// side stack.
//
// ******************************************/
short Stack :: getCurrentRow(Row *& row)
{
  // Top of the current row.
  short top = 0;

  // If the side switch is true,
  // set it to the right head pointer.
  if(side_switch)
  {
    row = r_row_head;
    top = r_top_index;
  }
  // Set row to the left head pointer.
  else
  {
    row = l_row_head;
    top = l_top_index;
  }

  // Return the top of the current row.
  return top;
}



/* ******************************************
// Decrement the top counter
// for a specific side.
//
// @param row: Pointer to the side whose top
// should be decremented.
//
// @return: true if row == the right side.
//
// ******************************************/
bool Stack :: setSideCounter(Row * row)
{
  // If the row points to the left side, decrement the left head.
  if(row == l_row_head) --l_top_index;
  // Otherwise, decrement the right head top.
  else --r_top_index;

  // Return the side of row.
  return (row == r_row_head);
}



/* ******************************************
// Initializes the passenger count.
// ******************************************/
bool Stack :: setNumPassengers(short count)
{
  num_passengers = count;
  return true;
}
