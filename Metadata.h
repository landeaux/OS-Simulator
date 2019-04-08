/**
 * @file Metadata.h
 * 
 * @brief Definition file for Metadata class
 * 
 * @details Specifies all member methods of the Metadata class
 * 
 * @version 1.00
 *          Adam Landis (7 April 2019)
 *          Initial development and testing of Metadata class
 * 
 * @note None
 */
//
// Precompiler Directives //////////////////////////////////////////////////////
//
#ifndef METADATA_H
#define METADATA_H
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include <string>
//
// Class/Struct Definitions ////////////////////////////////////////////////////
//
///// None
//
// Typedefs ////////////////////////////////////////////////////////////////////
//
///// None
//
// Class Definition ////////////////////////////////////////////////////////////
// 
class Metadata
{
public:
    Metadata(const std::string& filename);
private:
    std::string filename;
};
//
// Terminating Precompiler Directives
// 
#endif  // METADATA_H
//