/*****************************************************************
// File Name:  Event.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       June 2nd, 2017
// Class:      CS163
// Program #:  4
//
// Overview: This is the declaration file for the Event struct,
// which serves as the main collection of data for programming
// assignment #4. See function comments for more information.
//
// ***************************************************************/

#include "Event.h"


/* ********************************************
// Sets all event pointers to NULL.
//
// ********************************************/
Event :: Event(void)
{
  // Set every Event pointer (except the keywords) to NULL.
  name = NULL;
  location = NULL;
  date = NULL;
  start_time = NULL;
  description = NULL;
	isNULL = true;

  // For every keyword in the keyword array..
  for(short index = 0; index < MAX_KEYWORDS; ++index)
    // Set the current keyword to NULL.
    keywords[index] = NULL;
}



/* ********************************************
// Copies the contents of a preexisting event
// object using the copy function.
//
// @param event: The event object to be copied.
//
// ********************************************/
Event :: Event(const Event & event)
{
  // Call the copy function with the event parameter.
  copy(event);

  return;
}



/* ********************************************
// Deallocates the memory for all event fields.
//
// ********************************************/
Event :: ~Event(void)
{
	if(!isNULL)
		deleteEvent();
}



/* ********************************************
// Displays the contents of the event.
//
// @return: The number of keywords in the event.
//
// ********************************************/
int Event :: display(void) const
{
  // Index counter for printing keywords.
  // Also, returned.
  short index = 0;

  // If the name is NULL, return -1.
  // It is assumed that if the name is not NULL,
  // the other data is also not NULL.
	if(!isNULL)
	{
  	// Display everything except the event keywords.
  	std::cout << "\n\tDisplaying Event.." << std::endl << "\n\t  "
  	          << "Event Name: " << name << std::endl << "\t  "
  	          << "Event Location: " << location << std::endl << "\t  "
  	          << "Event Date: " << date << std::endl << "\t  "
  	          << "Event Start Time: " << start_time << std::endl << "\t  "
  	          << "Event Description: " << description << std::endl << "\t  ";

  	// If there is at least one keyword..
  	if(keywords[0])
  	{
  	  // Display a keywords label.
  	  std::cout << "Event Keywords: ";

  	  // From 0 to MAX_KEYWORDS..
  	  for(; index < MAX_KEYWORDS; ++index)
  	    // If the current keyword is not NULL, display it.
  	    if(keywords[index]) std::cout << keywords[index] << " ";
  	}

  // Display white space.
  std::cout << std::endl;
	}

  // Return the number of keywords.
  return index;
}



/* ********************************************
// Copys a preexisting event object.
//
// @param event: The event object to be copied.
// 
// @return: -2 if event copy location isn't NULL.
//          -1 if the event param is NULL.
//          Otherwise, returns the number of
//          copied keywords.
// 
// ********************************************/
int Event :: copy(const Event & event)
{
  // Index counter for adding keywords.
  short index = 0;

	if(!isNULL)
	  // Delete any dynamic memory used
	  // for the event being copied into.
	  deleteEvent();

  // If the name in the passed event is NULL, return -1.
  // It is assumed that if the name is not NULL, the
  // other data is also not NULL.
  if(!event.name) return -1;

  // Allocate memory for all event data (except keywords).
  name = new char[strlen(event.name) + 1];
  location = new char[strlen(event.location) + 1];
  date = new char[strlen(event.date) + 1];
  start_time = new char[strlen(event.start_time) + 1];
  description = new char[strlen(event.description) + 1];

  // Copy the passed event's data (except keywords).
  strcpy(name, event.name);
  strcpy(location, event.location);
  strcpy(date, event.date);
  strcpy(start_time, event.start_time);
  strcpy(description, event.description);

  // For every keyword in the keyword array..
  while(index < MAX_KEYWORDS && event.keywords[index])
  {
    // Allocate memory for the current keyword.
    keywords[index] = new char[strlen(event.keywords[index]) + 1];
    // Copy the contents of the passed in event.
    strcpy(keywords[index], event.keywords[index]);
    // Increment the index counter.
    ++index;
  }

	// Change isNULL.
	isNULL = false;

  // Return the number of copied keywords.
  return index;
}



/* ********************************************
// Checks the number of non-NULL keywords.
//
// @return: 0 - All keywords are NULL.
//          ∞ - The number of non-NULL keywords.
//
// ********************************************/
int Event :: keywordCount(void) const
{
  // The number of non-NULL keywords to be returned.
  int count = 0;

  // Count the number of non-NULL keywords.
  while(count < MAX_KEYWORDS && keywords[count] != NULL) ++count;

  // Return the number of non-NULL keywords.
  return count;
}


// Deletes all of the event data.
int Event :: deleteEvent(void)
{
  // If the name is not NULL..
  if(name)
  {
    // Deallocate and nullify it.
    delete[] name;
    name = NULL;
  }

  // If the location is not NULL..
  if(location)
  {
    // Deallocate and nullify it.
    delete[] location;
    location = NULL;
  }

  // If the date is not NULL..
  if(date)
  {
    // Deallocate and nullify it.
    delete[] date;
    date = NULL;
  }

  // If the start time is not NULL..
  if(start_time)
  {
    // Deallocate and nullify it.
    delete[] start_time;
    start_time = NULL;
  }

  // If the description is not NULL..
  if(description)
  {
    // Deallocate and nullify it.
     delete[] description;
     description = NULL;
  }

  // For every keyword in the keywords array..
  for(short index = 0; index < MAX_KEYWORDS; ++index)
    // If the current keyword is not NULL..
    if(keywords[index])
    {
      // Deallocate and nullify it.
      delete[] keywords[index];
      keywords[index] = NULL;
    }

	// Reset isNULL.
	isNULL = true;

  return 0;
}