/* ***************************************************************
// File Name:  Queue.h
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 10th, 2017
// Class:      CS163
// Program #:  2
//
// Overview: This is the declaration file for the Queue ADT for
// use in the airport management assignment. The objective of
// this queue is to manage the check-in line and gate line at the
// airport, such that passengers are loaded efficiently based on
// their boarding number (which is based on their seat number).
// See the Queue declaration comments and the Queue.cpp file for
// more information.
//
// ***************************************************************/

#ifndef QUEUE_H
#define QUEUE_H

#include "Util.h"
#include "Passenger.h"

//#include <cmath>

/* *****************************************
// Node definition for linking Passengers
// in the Passenger Queue.
// *****************************************/
struct QNode
{
  // Passenger object.
  Passenger passenger;

  // Pointer to the next QNode / Passenger.
  QNode * next;
};



/* *****************************************
// Declaration for a queue of Passengers
// for use in managing passenger boarding
// protocols. Passenger queue methods are
// defined in the Queue.cpp file.
//
// *****************************************/
class Queue
{

  public:

    // Initializes the Queue's data member(s).
    Queue(void);
    // Copies the passed Queue's data.
    Queue(const Queue & queue);
    // Calls the eraseQueue function on the front node.
    ~Queue(void);

    // Inserts a new node at the rear of the Queue.
    int enqueue(const Passenger & passenger);

    // Removes the front node.
    int dequeue(void);
    // Stores the data in the front node's Passenger
    // object into the passed Passenger. Then removes
    // the front node.
    int dequeue(Passenger & passenger);

    // Stores the data in the front node's Passenger
    // object into the passed Passenger.
    int peek(Passenger & passenger) const;

    // Returns true if the list is empty.
    bool isEmpty(void) const;

    // Calls the private display
    // function with the front node.
    bool display(void) const;

    // Calls the private eraseQueue
    // function on the front node.
    int eraseQueue(void);
    
    // Returns the number of passengers
    // in the queue.
    int getNumPassengers(void);

  private:

    // Recursively traverses the queue
    // and prints every Passenger.
    bool display( const QNode * front,
                  short cnt) const;

    // True if seat_num_a is closer to
    // the front/isle than num_seat_b.
    int isFrontOf( const char * seat_num_a,
                   const char * seat_num_b );

    // Deallocatew the entire queue. Called by
    // the destructor with the front node.
    int eraseQueue(QNode *& front);

    // Creates a boarding number based
    // on a passenger's seat number.
    int generateBoardingNum(Passenger & passenger);

    // Cantor pair function.
    long long cantor_pair( int num_one,
                           int num_two);

    // Pointer to the rear node.
    QNode * qptr;
    // The number of passengers in the queue.
    int num_passengers;

};
#endif // QUEUE_H
