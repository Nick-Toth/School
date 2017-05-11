/*****************************************************************
// File Name:  Queue.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 10th, 2017
// Class:      CS163
// Program #:  2
//
// Overview: This is the implementation file for the Queue ADT for
// use in the airport management assignment. The objective of this
// queue is to manage the check-in line and gate line at the
// airport, such that passengers are loaded efficiently based on
// their boarding number (which is based on their seat number).
// For information regarding specific functionality, see the
// function header comments in this file.
//
// ***************************************************************/

#include "Queue.h"



/* *****************************************
// Initialize any and all queue members.
//
// *****************************************/
Queue :: Queue(void)
{
  // Set the rear queue pointer to NULL.
  qptr = NULL;
  // Set the number of passengers to 0.
  num_passengers = 0;
}




/* *****************************************
// Copy constructor for potential future
// copy constructions.
//
// *****************************************/
Queue :: Queue(const Queue & queue)
{ return; }



/* *****************************************
// Calls the private eraseQueue function on
// the head front (qptr->next) of the list.
//
// *****************************************/
Queue :: ~Queue(void)
{
  // If the queue is empty, return.
  if(!qptr) return;

  // Call the private eraseQueue function
  // on the front of the queue.
  eraseQueue(qptr->next);

  return;
}



/* *****************************************
// Calls the private eraseQueue function.
// Removes every node in the queue
// recursively.
//
// @return: The number of nodes removed.
//
// *****************************************/
int Queue :: eraseQueue(void)
{
  // If the queue is empty, return.
  if(!qptr) return 0;

  // Call the private eraseQueue function
  // on the front of the queue. Returned
  // the number of removed nodes.
  return eraseQueue(qptr->next);
}



/* *****************************************
// Removes every node in the queue. Called
// by the queue's destructor for recursive
// deallocation.
//
// @Return: The number of nodes removed.
//
// *****************************************/
int Queue :: eraseQueue(QNode *& front)
{

  //Set the number of passengers to 0.
  num_passengers = 0;

  // If front points to the tail..
  if(front == qptr)
  {
    // Delete & nullify the tail.
    delete qptr;
    qptr = NULL;

    // Exit.
    return 1;
  }

  // Call the eraseQueue function on the next node.
  // Store 1 plus the returned value in an int.
  int removed = 1 + eraseQueue(front->next);

  // Delete and nullify the front.
  delete front;
  front = NULL;

  // Return the total number of removed nodes.
  return removed;
}



/* *****************************************
// Create a new node with the contents of
// the given passenger, and insert it at
// the front of the queue.
//
// @param passenger: The passenger to be
// copied into the new Node.
//
// @return: 0 - Successful enqueue, size > 1
//          1 - Copy failed.
//          2 - Successful enqueue, size = 1
//
// *****************************************/
int Queue :: enqueue(const Passenger & passenger)
{

  // Allocate memory for the new node.
  QNode * new_node = new QNode;

       // Stores the seat number of the new passenger.
  char new_psngr_seat[5],
       // Stores the seat number of the rear passenger.
       rear_psngr_seat[5];

      // Copy the passed passenger's data into the new passenger.
  int copy_returned = new_node->passenger.copyPassenger(passenger),
      // Store the passenger's seat number in new_psngr_seat.
      get_new_seat_ret = passenger.getSeat(new_psngr_seat),
      // Stores the returned value of the isFrontOf function.
      is_front = 0;

  // If the copy failed, or the new seat is
  // not in front of the rear seat..
  if(copy_returned == 1 || is_front != 0)
  {
    // Delete and nullify the new node.
    delete new_node;
    new_node = NULL;

    // Report failure.
    return 1;
  }

  // Increment the passenger counter.
  ++num_passengers;

  // If the boarding number has not been initialized,
  // Initialize the boarding number.
  if(new_node->passenger.boarding_num == 0)
    // Generate a boarding number.
    generateBoardingNum(new_node->passenger);

  // If the rear is not null, and the new seat is valid..
  if(qptr && get_new_seat_ret == 0)
  {
    // Get the seat number of the rear seat.
    qptr->passenger.getSeat(rear_psngr_seat);
    // Check if the new seat is to the front/
    // isle of the rear passenger's seat.
    is_front = isFrontOf(new_psngr_seat, rear_psngr_seat);
  }

  // If the list is empty..
  if(!qptr)
  {
    // Set the front and rear to the new node.
    qptr = new_node;
    qptr->next = qptr;
    // Set the rear to the new node.
    qptr = qptr->next;

    // Return code 2.
    return 2;
  }

  // Set the new node to point to the current
  // front of the queue.
  new_node->next = qptr->next;

  // Set the new node as the front of the queue.
  qptr->next = new_node;

  // Set the rear to the new node.
  qptr = qptr->next;

  // Report success.
  return 0;
}



/* *****************************************
// Removes the Node at the rear of the queue.
//
// @return: 0 - Success, queue not empty.
//          1 - Empty queue.
//          2 - Success, queue empty.
//
// *****************************************/
int Queue :: dequeue(void)
{
  // If the queue is empty, return code 1.
  if(!qptr) return 1;

  // Store the front node.
  QNode * temp = qptr->next;

  // If the front node is also the rear node..
  if(temp == qptr)
  {
    // Delete and nullify the last node.
    delete qptr;
    qptr = NULL;
    // Return code 2.
    return 2;
  }

  // Set the front node to its successor.
  qptr->next = temp->next;

  // Delete the previous front node.
  delete temp;

  // Report success.
  return 0;
}



/* *****************************************
// Copies the contents of the Node at the
// rear of the queue, and removes it.
//
// @param passenger: The location where the
// contents of the rear node will be copied.
//
// @return: 0 - Success, queue not empty.
//          1 - Failure. Could not copy.
//          2 - Failure, empty queue.
//          3 - Success, queue empty.
//
// *****************************************/
int Queue :: dequeue(Passenger & passenger)
{
  // Copy's return value.
  short copy_returned = 0;

  // If the queue is empty, return code 2.
  if(!qptr) return 2;

  // Store the front node.
  QNode * temp = qptr->next;

  // If the front node is also the rear node..
  if(temp == qptr)
  {
    // Copy the passenger data from the node being
    // deallocated into the given passenger.
    copy_returned = passenger.copyPassenger(temp->passenger);

    // Delete and nullify the last node.
    delete qptr;
    qptr = NULL;

    // Return code 3.
    return 3;
  }

  // Copy the passenger data from the node being
  // deallocated into the given passenger.
  copy_returned = passenger.copyPassenger(temp->passenger);

  // Set the front node to its successor.
  qptr->next = temp->next;
  // Delete the previous front node.
  delete temp;

  // Return whatever the copy operation returned.
  // See header for details.
  return copy_returned;
}



/* *****************************************
// Stores the contents of the rear of the
// queue in the passed passenger.
//
// @param passenger: The location where the
// contents of the front passenger in the
// queue is copied.
//
// @return: 0 - Copy succeeded.
//          1 - Copy failed.
//
// *****************************************/
int Queue :: peek(Passenger & passenger) const
{
  // If the queue is empty, return error code 1.
  if(!qptr) return 1;

  // Otherwise, copy the contents of the rear
  // passenger into the given passengr object.
  // Return the code returned for the passenger
  // copy operation. 1 for failure, 0 for success.
  return passenger.copyPassenger(qptr->passenger);
}



/* *****************************************
// Checks whether the list is empty.
//
// @return: true if the list is empty.
//
// *****************************************/
bool Queue :: isEmpty(void) const
{
  // If the Queue is empty, return true.
  if(!qptr) return true;

  // Otherwise, return false.
  return false;
}



/* *****************************************
// Checks whether the queue is empty, and,
// if it isn't, calls the private display
// function with the first node.
//
// @return: true if the list is not empty.
//
// *****************************************/
bool Queue :: display(void) const
{
  // If the queue is empty, return false.
  if(!qptr) return false;

  // Display alert that the queue will be printed.
  std::cout << "\n  Displaying passengers in the queue.." << std::endl;

  // Print the front node (at least), and return true.
  return display(qptr->next, 1);
}



/* *****************************************
// The private display function is initially
// called by the public display function,
// and subsequently calls itself until the
// end of the queue is reached. Note that the
// public display function checks if the
// list is empty, so there is no reason to
// check here, and this function will always
// return true.
//
// @param front: The current node to print.
//
// @param cnt: The number (Starting at 1)
// of the current node (for printing).
//
// @return: true. If the list is empty, then
// false is returned by the public display.
//
// *****************************************/
bool Queue :: display(const QNode * front, short cnt) const
{
  // Display the passenger in the front node.
  std::cout << "\n    Passenger #" << cnt << std::endl;

  // Display the current passenger.
  front->passenger.display();

  // If the front node is the last node, return true.
  if(front == qptr) return true;

  // Display the next node.
  return display(front->next, ++cnt);
}



/* *****************************************
// Checks whether seat_num_a is closer to
// the front / isle than seat_num_b. Used
// in checking for correct order of
// passengers.
//
// @param seat_num_a: Seat number to be
// compared with seat_num_b.
//
// @param seat_num_b: Seat number to be
// compared with seat_num_a.
//
// @return: 0 - a is in front of b.
//          1 - b is in front of a.
//          2 - a is in the same seat as b.
//          3 - Invalid seats. Missing
//              alphabetic characters.
//          4 - Invalid seats. Missing
//              numeric characters.
//
// *****************************************/
int Queue :: isFrontOf( const char * seat_num_a,
                        const char * seat_num_b )
{

  // Create arrays for storing the alphabetic
  // characters in the seat numbers.
  char seat_a_alpha[5],
       seat_b_alpha[5];

      // Store the numeric parts of the seat numbers.
  int seat_a_row = parseInt(seat_num_a),
      seat_b_row = parseInt(seat_num_b),
      // Store the alphabetic parts of the seat numbers.
      parse_a_ch_res = parseAlpha(seat_num_a, seat_a_alpha),
      parse_b_ch_res = parseAlpha(seat_num_b, seat_b_alpha);

  // If one of the seats did not have an
  // alphabetic component, return code 3.
  if(parse_a_ch_res == 0 || parse_b_ch_res == 0) return 3;

  // If one of the seats did not have a
  // numeric component, return code 4.
  if(seat_a_row == 0 || seat_b_row == 0) return 4;

  // If a is in a row closer to the front than b, return 0.
  if(seat_a_row < seat_b_row) return 0;

  // If a is in the same row as b..
  if(seat_a_row == seat_b_row)
  {
    // If a is closer to the isle than b, return 0.
    if(*seat_a_alpha > *seat_b_alpha) return 0;

    // If a and b are the same seat, return code 2.
    if(*seat_a_alpha == *seat_b_alpha) return 2;
  }
  // Otherwise (different row side) if seat a is closer to the
  // isle than seat b (could not have been added yet), return 0.
  if(seat_a_row > seat_b_row) return 0;

  // b is closer to the front/isle than a - return code 1.
  return 1;
}



/* ************************************************************
// Generates a passenger's boarding number based on their seat
// number using the cantor pairing function.
//
// cantor a b = 1/2 * (a+b) * (a+b+1) + b
//
// where a is the numeric part of the seat number, and b is the
// alphabetic part of the seat number.
//
// @param seat_num: The passenger's seat number to be used in
// calculating their boarding number.
//
// @return: The boarding number.
//
// ************************************************************/
int Queue :: generateBoardingNum(Passenger & passenger)
{

  // Get the passengers seat number.
  char seat_num[strlen(passenger.seat_num) + 1];

  strcpy(seat_num, passenger.seat_num);

  // Location for storing the alphabetic part of the seat number.
  char alpha[strlen(seat_num) + 1];

      // Get the numeric part of the seat number.
  int numeric = parseInt(seat_num),
      // Stores the numeric representation of the seat number.
      alpha_num = 1;

  // Get the alphabetic part of the seat number.
  parseAlpha(seat_num, alpha);

  // Index counter.
  unsigned int g_index = 0;

           // The boarding number.
  long long b_num = 0,
           // First boarding number.
           last_num = (num_passengers > 1 )? qptr->passenger.boarding_num : 1;
  
  // Calculate alpha num - 'A'
  for(; g_index < strlen(alpha); ++g_index)
    alpha_num += ('A' - alpha[g_index] + 1);

  // Calculate the boarding number.
  b_num = (cantor_pair(numeric, alpha_num) * static_cast<long long>( pow(num_passengers, 2.71))) + last_num;

  // Set the new passenger's boarding number.
  passenger.boarding_num = b_num;

  return 0;
}



/* ************************************************************
// Cantor pairing function - used in generating boarding
// numbers
//
// cantor a b = 1/2 * (a+b) * (a+b+1) + b
//
// where a is the numeric part of the seat number, and b is the
// alphabetic part of the seat number.
//
// @param seat_num:
// @return: The boarding number.
//
// ************************************************************/
long long Queue :: cantor_pair(int num_one, int num_two)
{
  long long cantor_num = static_cast<long long>(0.5 * (num_one + num_two) * (num_one + num_one + 1) + num_two);

  return cantor_num;
}


/* ************************************************************
// Returns the number of passengers in the queue.
// ************************************************************/
int Queue :: getNumPassengers(void)
{ return num_passengers; }
