/*****************************************************************
// File Name:  Util.h
// Created By: Nick G. Toth
// E-Mail:     ntoth@pdx.edu
// Date:       May 10th, 2017
// Class:      CS163
// Program #:  2
//
// Overview: This file contains headers for some utilities that
// are important in several places in the second program. More
// info in the Util.cpp file.
//
// ***************************************************************/

#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <cstring>
#include <locale>

// Reverse a string.
int strrev(char str[]);

// Parse an number from the beginning of a string.
// Used to get the numeric part of a boarding number.
int parseInt(const char str[]);

// Parse a string from the end of a string. Used to
// get the alphabetic part of a boarding number.
int parseAlpha( const char str[],
                char location[] );

#endif // UTIL_H
