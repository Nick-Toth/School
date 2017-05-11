/*****************************************************************
// File Name:  Program1_NToth.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 10th, 2017
// Class:      CS163
// Program #:  2
//
// Overview: This is the Passenger declaration file for use in the
// airport management assignment. The Passenger struct is used as
// the data type for the Queue and Stack used in this same
// assignment. The implementation of Passenger functions, and more
// information can be found in the Passenger.cpp file.
//
// ***************************************************************/

#ifndef PASSENGER_H
#define PASSENGER_H

#include <iostream>
#include <cstring>



/* *****************************************
// The Passenger struct is the primary data
// type that the associated Stack and Queue
// will be dealing with. Passengers have a
// name, a seat number, a boarding number,
// and a set of fundamental operations for
// managing that data. The implementations
// of the Passenger functions can be found
// in the Passenger.cpp file.
//
// *****************************************/
struct Passenger
{

         // The name of the passenger.
  char * name,
         // The passengers seat number. e.g. 11B
       * seat_num;

  // The passenger's boarding number.
  long long boarding_num;

  // Default constructor for setting data members to NULL.
  Passenger(void);
  // Constructor for allocating memory for,
  // and initializing data members.
  Passenger(char name[], char seat_num[]);
  // Copy constructor for making passenger copies.
  Passenger(const Passenger & passenger);

  // Destructor for deallocating data members.
  ~Passenger(void);

  // Display information about the passenger.
  bool display(void) const;

  // Store the passenger's name in name_loc.
  int getName(char name_loc[]) const;

  // Store the passenger's seat number in seat_num_loc.
  int getSeat(char seat_num_loc[]) const;

  // Store the passenger's boarding number in board_num_loc.
  int getBoardingNum(void) const;

  // Copy the contents of the passenger parameter.
  int copyPassenger(const Passenger & passenger);

  // Allocate memory for the passenger's name and seat number,
  // set to the contents of the passed name and seat number.
  int setupPassenger(const char name[], const char seat_num[]);

};
#endif // PASSENGER_H