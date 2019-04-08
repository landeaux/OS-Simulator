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

/**
 * Checks if a given string is a non-negative integer or not.
 *
 * Credit: Code adapted from https://stackoverflow.com/a/2845275
 *
 * @param   str
 *          The string to check
 *
 * @return  true if string is a positive integer, false otherwise
 */
bool isNonNegativeInteger(const std::string& str)
{
    if(str.empty() || (!isdigit(str[0])))
    {
        return false;
    }

    char * p;
    strtol(str.c_str(), &p, 10);

    return (*p == 0);
}

/**
 * Takes a given string and strips ' ' characters from it
 *
 * @param   str
 *          The string to strip ' ' characters from
 *
 * @return  the input string without ' ' characters in it
 */
std::string stripSpaces(const std::string& str)
{
    std::string result;
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] != ' ')
        {
            result += str[i];
        }
    }

    return result;
}

/**
 * Wrapper function for strtol which takes a std::string and converts it to an
 * unsigned long.
 *
 * @param   str
 *          The string to convert
 *
 * @return
 */
unsigned long strToUnsignedLong(const std::string& str)
{
    return (unsigned long) strtol(str.c_str(), nullptr, 10);
}

/**
 * Logs data to monitor (terminal)
 *
 * @param   logData
 *          The data to log to monitor (string)
 */
void logToMonitor(std::string& logData)
{
    std::cout << logData;
}

/**
 * Logs data to an output file
 *
 * @param   logData
 *          The data to log to file (string)
 *
 * @param   logFile
 *          The filename of the file to log the output to (string)
 *
 * @param   mode
 *          The openmode for the file. Default: ios_base::out
 *
 * @return  None
 */
void logToFile(std::string& logData, std::string& logFilename,
               std::ios_base::openmode mode)
{
    std::ofstream logFile;

    if (logFilename.empty())
    {
        throw std::string("Error: cannot log to file - filename missing");
    }

    logFile.open(logFilename, mode);

    if (!logFile)
    {
        throw std::string("Error: cannot log to file - unable to open log file");
    }

    logFile << logData;

    logFile.close();
}

/**
 * @brief      Generates a random number between 0 and max value for unsigned
 *
 * @return     The generated random number (unsigned)
 */
unsigned genRandNum()
{
    // Get a random seed from the OS entropy device
    std::random_device rd;

    // Use the 64-bit Mersenne Twister 19937 generator and seed it with entropy
    std::mt19937_64 eng(rd());  
                                
    // Define the distribution
    std::uniform_int_distribution<unsigned> distr;

    return distr(eng);
}

/**
 * @brief      Converts an unsigned integer to a string in hexidecimal format
 *
 * @param[in]  num   The unsigned integer to convert
 *
 * @return     The resultant hex string after conversion
 */
std::string uintToHexStr(unsigned num)
{
    std::stringstream stream;
    stream.width(8);
    stream.fill('0');
    stream << std::hex << num;
    return "0x" + std::string(stream.str());
}