/*****************************************************************
// File Name:  Program1_NToth.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 10th, 2017
// Class:      CS163
// Program #:  2
//
// Overview: This is the Passenger struct's implementation file.
// As mentioned in the Passenger header file, Passengers are the
// primary data in the queue and stack for the airport management
// assignment. Please see the Passenger.h file for the Passenger
// declaration, and more information. If you're looking for more
// information regarding specific functionality, the function
// comments in this file are a good place to start.
//
// ***************************************************************/

#include "Passenger.h"


/* ********************************************
// Default constructor sets the passenger's
// data pointers to NULL, boarding number to 0.
//
// ********************************************/
Passenger :: Passenger(void)
{
  // Set passenger data pointers to NULL.
  name = NULL;
  seat_num = NULL;

  // Initialize the boarding number to 0.
  boarding_num = 0;
}



/* ********************************************
// Allocates memory for and initializes the
// passenger's name and seat number.
//
// @param name: The name to be copied into the
// memory pointed to by the passenger's name.
//
// @param seat_num: The seat number to be copied
// into the memory pointed to by the passenger's
// seat_num.
//
// ********************************************/
Passenger :: Passenger(char name[], char seat_num[])
{
  // Allocate proportional memory to the passenger's
  // name as the passed name.
  this->name = new char[strlen(name) + 1];
  // Allocate proportional memory to the passenger's
  // seat number as the passed seat number.
  this->seat_num = new char[strlen(seat_num) + 1];

  // Copy the contents of the passed name to
  // the passenger's name.
  strcpy(this->name, name);
  // Copy the contents of the passed seat number to
  // the passenger's seat number
  strcpy(this->seat_num, seat_num);

  return;
}



/* ********************************************
// Calls the allocating constructor (above this
// copy constructor) with the name and seat
// number of the passenger to be copied.
//
// @param passenger: The passenger to be copied.
//
// ********************************************/
Passenger :: Passenger(const Passenger & passenger)
{
  // Initialie the name and seat number using
  // the ( char* , char* ) constructor.
  Passenger(passenger.name, passenger.seat_num);

  // Initialize the boarding number.
  boarding_num = passenger.boarding_num;

  return;
}



/* ********************************************
// Deallocates the passenger's name and seat
// number.
//
// ********************************************/
Passenger :: ~Passenger(void)
{
  // If the name is not NULL..
  if(name)
    // Deallocate it.
    delete[] name;

  // If the seat number is not NULL..
  if(seat_num)
    // Deallocate it.
    delete[] seat_num;

  // Set pointers to NULL.
  name = NULL;
  seat_num = NULL;

  return;
}



/* ********************************************
// Display the passenger's name, seat number
// and boarding number.
//
// @return: true if seat_num and name not NULL.
//
// ********************************************/
bool Passenger :: display(void) const
{
  // If the name or seat number are NULL, report error.
  if(!name || !seat_num) return false;

  // Print out the name, seat number and boarding_num of the passenger.
  std::cout << std::endl << "  "
            << "Displaying Passenger Info" << std::endl << "    "
            << "Name => " << name << std::endl << "    "
            << "Seat => " << seat_num << std::endl << "    "
            << "Boarding Number => " << boarding_num << std::endl;

  // Report success.
  return true;
}



/* ********************************************
// Retrieves the passenger's name.
//
// @param name_loc: The location where the
// passenger's name will bo copied.
//
// @return: 0 - copied successfully.
//          1 - passenger's name is NULL.
//          2 - name location too small.
//
// ********************************************/
int Passenger :: getName(char name_loc[]) const
{
  // If the name is NULL, report error #1.
  if(!name) return 1;

  // If the name location is not large enough to
  // store the passenger's name, report error #2.
  if(strlen(name_loc) < strlen(name)) return 2;

  // Copy the contents of the passenger's
  // name into name_loc.
  strcpy(name_loc, name);

  // Report success.
  return 0;
}



/* ********************************************
// Retrieves the passenger's seat number.
//
// @param seat_num_loc: The location where the
// passenger's seat number will bo copied.
//
// @return: 0 - copied successfully.
//          1 - passenger's seat is NULL.
//
// ********************************************/
int Passenger :: getSeat(char seat_num_loc[]) const
{
  // If the seat number is NULL, report error #1.
  if(!seat_num) return 1;

  // Copy the contents of the passenger's
  // seat number into seat_num_loc.
  strcpy(seat_num_loc, seat_num);

  // Report success.
  return 0;
}



/* ********************************************
// Returns the passenger's boarding number.
//
// ********************************************/
int Passenger :: getBoardingNum(void) const
{ return boarding_num; }



/* ********************************************
// Copies the name, seat number and boarding
// number of a given passenger into this
// Passenger.
//
// @param passenger: The passenger to be copied
// into this passenger.
//
// @return: 0 - copied successfully.
//          1 - copy failed.
//
// ********************************************/
int Passenger :: copyPassenger(const Passenger & passenger)
{

  // If the passenger is incomplete,
  // return code 1 :: Incomplete passenger.
  if(!passenger.name || !passenger.seat_num)
    return 1;

  // If any fields have already been allocated,
  // deallocate them..
  if(name) delete name;
  if(seat_num) delete seat_num;

  // Allocate proportional memory to
  // name as the given passenger's name.
  name = new char[strlen(passenger.name) + 1];

  // Allocate proportional memory to seat_num
  // as the given passenger's seat_num.
  seat_num = new char[strlen(passenger.seat_num) + 1];

  // Copy the contents of the given passenger's
  // name to the passenger's name.
  strcpy(name, passenger.name);

  // Copy the contents of the passed seat number to
  // the passenger's seat number
  strcpy(seat_num, passenger.seat_num);

  // Initialize the boarding number to the
  // given passenger's boarding number.
  boarding_num = passenger.boarding_num;

  // Report success.
  return 0;
}



/* ********************************************
// Allocates memory for and initialize the
// passenger with a given name and seat number.
//
// @param name: The name to be copied into
// the Passenger's name field.
//
// @param seat_num: The seat number to be
// copied into the Passenger's seat number
// field.
//
// ********************************************/
int Passenger :: setupPassenger( const char name[],
                                 const char seat_num[] )
{
  // If any fields have already been
  // allocated, deallocate them..
  if(this->name) delete this->name;
  if(this->seat_num) delete this->seat_num;

  // Allocate proportional memory to the
  // passenger's name as the passed name.
  this->name = new char[strlen(name) + 1];
  // Allocate proportional memory to the passenger's
  // seat number as the passed seat number.
  this->seat_num = new char[strlen(seat_num) + 1];

  // Copy the contents of the passed
  // name to the passenger's name.
  strcpy(this->name, name);
  // Copy the contents of the passed seat
  // number to the passenger's seat number
  strcpy(this->seat_num, seat_num);

  // Report success.
  return 0;
}