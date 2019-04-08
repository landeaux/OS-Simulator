/**
 * @file helpers.cpp
 * 
 * @brief      Implementaion file for helper functions used accross the program
 * 
 * @details    Implementation file for helper functions used accross the 
 *             program. Contains the function implementations for functions
 *             defined in helpers.h.
 *             
 * @version    1.00
 *             Adam Landis (8 April 2019)
 *             Move all helper function bodies out of main and into 
 *             helpers.cpp
 *             
 * @note       Requires helpers.h
 */
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include "helpers.h"
//
// Function Implementations ////////////////////////////////////////////////////
//

/**
 * Checks if a filename contains a specified extension
 *
 * @param   filename
 *          The filename to validate
 *
 * @param   extension
 *          The extension to check for
 *
 * @return  success of validation (bool)
 */
bool isValidFileExtension(const std::string& filename, const std::string& ext)
{
    unsigned long flnLen = filename.length();
    unsigned long extLen = ext.length();
    size_t extLoc = filename.rfind('.' + ext);

    // if extension found in string and extension is at the end, return true
    return ((extLoc != std::string::npos) && (extLen + extLoc + 1 == flnLen));
}

/**
 * Check if a file is empty
 *
 * @param   file
 *          The file to check
 *
 * @return  true if file is empty, false otherwise
 */
bool isEmptyFile(std::ifstream& file)
{
    return file.peek() == std::ifstream::traits_type::eof();
}

/**
 * Checks if a given string is a positive integer or not.
 *
 * Credit: Code adapted from https://stackoverflow.com/a/2845275
 *
 * @param   str
 *          The string to check
 *
 * @return  true if string is a positive integer, false otherwise
 */
bool isPositiveInteger(const std::string& str)
{
    if(str.empty() || (!isdigit(str[0])))
    {
        return false;
    }

    char * p;
    long int num = strtol(str.c_str(), &p, 10);

    return (*p == 0 && num != 0);
}