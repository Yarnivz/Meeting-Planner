//// File edited by Lucas Van Asbroeck
///  Original file:
//        ============================================================================
//         Name        : TicTacToeUtils.h
//         Author      : Serge Demeyer
//         Version     :
//         Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
//         Description : TicTactToe in C++, Ansi-style
//        ============================================================================

// Below are two lines serving as a so-called ``header guard''.
// Prevents from loading the same .h file multiple times
#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <iostream>

bool directory_exists(const std::string &dirname);

bool file_exists(const std::string &filename);

bool file_is_empty(const std::string &filename);

bool file_compare(const std::string &leftFileName, const std::string &rightFileName);


// Closing of the ``header guard''.
#endif