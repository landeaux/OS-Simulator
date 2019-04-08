/**
 * @file helpers.h
 * 
 * @brief      Header file for helper functions used accross the program
 * 
 * @details    Header file for helper functions used accross the program. 
 *             Contains the function prototypes and includes any other headers
 *             as needed.
 *             
 * @version    1.00
 *             Adam Landis (8 April 2019)
 *             Move all helper function prototypes out of main and into 
 *             helpers.h.
 *             
 * @note       None.
 */
//
// Precompiler Directives //////////////////////////////////////////////////////
//
#ifndef HELPERS_H
#define HELPERS_H
//
// Header Files ////////////////////////////////////////////////////////////////
//
#include <iostream>
#include <string>
#include <fstream>
#include <random>       // for generating random numbers
#include <limits>       // for generating random numbers
#include <sstream>      // stringstream
//
// Function Prototypes
// 
bool isValidFileExtension(const std::string& filename, const std::string& ext);
bool isEmptyFile(std::ifstream& file);
bool isPositiveInteger(const std::string& str);
bool isNonNegativeInteger(const std::string& str);
std::string stripSpaces(const std::string& s);
unsigned long strToUnsignedLong(const std::string& str);
void logToMonitor(std::string& logData);
void logToFile(std::string& logData, std::string& logFilename,
               std::ios_base::openmode mode = std::ios_base::app);
unsigned genRandNum();
std::string uintToHexStr(unsigned num);
//
// Terminating Precompiler Directives
// 
#endif  // HELPERS_H
//