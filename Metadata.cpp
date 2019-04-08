/**
 * @file Metadata.cpp
 * 
 * @brief Implementation file for Metadata class
 * 
 * @details Implements all member methods of Metadata class
 * 
 * @version 1.00
 *          Adam Landis (8 March 2019)
 *          Initial development and testing of Metadata class
 * 
 * @note Requries Metadata.h
 */ 
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include "Metadata.h"
#include "helpers.h"
//
// Class Member Implementation /////////////////////////////////////////////////
//

/**
 * @brief      Constructs the object.
 *
 * @param[in]  filename  The metadata filename
 */
Metadata::Metadata(const std::string& filename): filename(filename)
{
    
}