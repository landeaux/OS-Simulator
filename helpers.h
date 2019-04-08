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
#include <string>
#include <fstream>
//
// Function Prototypes
// 
bool isValidFileExtension(const std::string& filename, const std::string& ext);
bool isEmptyFile(std::ifstream& file);
bool isPositiveInteger(const std::string& str);
//
// Terminating Precompiler Directives
// 
#endif  // HELPERS_H
//