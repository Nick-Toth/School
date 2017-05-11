/*****************************************************************
// File Name:  Util.cpp
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 10th, 2017
// Class:      CS163
// Program #:  2
//
// Overview: This file contains some utilities that are important
// in several places in the second program. Specifically, in the
// Queue and in the testing file.
//
// ***************************************************************/

#include "Util.h"


/* ************************************************************
// Reverses a string.
//
// @param str: The string to be reversed.
//
// @return: The length of the string.
//
// ************************************************************/
int strrev(char str[])
{

        // Get the length of the passed in string.
  short len = strlen(str),
        // Index counter.
        index = 0;

  // Create a new char[] to hold the reversed string.
  char new_str[len + 1];

  // Copy the reverse of the passed string into the new string.
  while(index < len)
  {
    // Store the end of str in new_str.. etc..
    new_str[index] = str[len - index - 1];
    // Increment the string index counter.
    // +1 new string, -1 old string.
    ++index;
  }
  // Set the end of the new string to null term char.
  new_str[len] = '\0';

  // Copy the reversed string into the passed in string.
  strcpy(str, new_str);

  // Return the length.
  return len;
}



/* ************************************************************
// Returns the integer at the beginning of a string. If the
// string does not begin with an int, then 0 is returned. For
// example, given "123abc", this function returns 123. This is
// used to check row numbers in passenger seats.
//
// @param str: The string to be parsed.
//
// @return: 0 if the string does not begin with an int,
// otherwise whatever number the string begins with.
//
// ************************************************************/
int parseInt(const char str[])
{

        // Index counter.
  short index = 0,
        // Number of digits in the number.
        count = 0;

  // The parsed number to be returned.
  int parsed_num = 0;

  // While the string at the current index
  // is a digit, increment the count.
  while( isdigit(str[count]) ) ++count;

  // While index is less than the number
  // of digits in the number to be parsed..
  while(index < count)
  {
    // Add the current digit to the parsed number..
    parsed_num += (str[index] - '0') * pow(10, (count - index - 1));
    // Increment the index counter.
    ++index;
  }

  // Return the parsed number.
  return parsed_num;
}



/* ************************************************************
// Returns the alphabetic part of a passenger's seat number.
//
// @param str: The string to be parsed.
//
// @param location: The location where the alphabetic part of
// the passenger's seat number will be stored.
//
// @return: Returns the length of the parsed char[]. 0 if no
// char[] was found.
//
// ************************************************************/
int parseAlpha( const char str[],
                char location[] )
{

        // Index counter for str. Counts backwards while alpha.
  short s_index = strlen(str) - 1,
        // Index counter for location.
        l_index = 0;

  // Copy the alphabetic contents of str into location.
  while( isalpha(str[s_index]) )
  {
    // Add teh current string character.
    location[l_index] = str[s_index];
    // Increment the alphabetic location index coutner.
    ++l_index;
    // Decrement the alphabetic source index counter.
    --s_index;
  }

  // If no alphabetic chars exist, return 0.
  // This should never happen.
  if(l_index == 0) return 0;

  if(strlen(location) > 1)
    // Reverse the string. Return its length.
    return strrev(location);

  // Return the length.
  return 1;
}
