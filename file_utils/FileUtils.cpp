//// File edited by Lucas Van Asbroeck
///  Original file:
//        ============================================================================
//         Name        : TicTacToeUtils.cpp
//         Author      : Serge Demeyer
//         Version     :
//         Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
//         Description : TicTactToe in C++, Ansi-style
//        ============================================================================

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cstdio>
#include <string>

#include "FileUtils.h"

using namespace std;

/**
Auxiliary functions for file manipulation.
*/
bool directory_exists(const std::string &dirname) {
	struct stat st;
	return stat(dirname.c_str(), &st) == 0;
}

bool file_exists(const std::string &filename) {
	struct stat st;
	if (stat(filename.c_str(), &st) != 0) return false;
	ifstream f(filename.c_str());
	if (f.good()) {
		f.close();
		return true;
	} else {
		f.close();
		return false;
	}
}

bool file_is_empty(const std::string &filename) {
	struct stat st;
	if (stat(filename.c_str(), &st) != 0) return true; // File does not exist; thus it is empty
	return st.st_size == 0;
}

bool file_compare(const std::string &leftFileName, const std::string &rightFileName) {
	ifstream leftFile, rightFile;
	char leftRead, rightRead;
	bool result;

	// Open the two files.
	leftFile.open(leftFileName.c_str());
	if (!leftFile.is_open()) {
		return false;
	};
	rightFile.open(rightFileName.c_str());
	if (!rightFile.is_open()) {
		leftFile.close();
		return false;
	};

	result = true; // files exist and are open; assume equality unless a counterexamples shows up.
	while (result && leftFile.good() && rightFile.good()) {
		leftFile.get(leftRead);
		rightFile.get(rightRead);
		result = (leftRead == rightRead);
	};
	if (result) {
		// last read was still equal; are we at the end of both files ?
		result = (!leftFile.good()) && (!rightFile.good());
	};

	leftFile.close();
	rightFile.close();
	return result;
}