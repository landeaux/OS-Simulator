/**
 * @file MetadataInstruction.cpp
 * 
 * @brief Implementation file for MetadataInstruction (Process Control Block) class
 * 
 * @details Implements all member methods of MetadataInstruction class
 * 
 * @version 1.00
 *          Adam Landis (8 March 2019)
 *          Initial development and testing of MetadataInstruction class
 * 
 * @note Requries MetadataInstruction.h
 */ 
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include "MetadataInstruction.h"
//
// Class Member Implementation /////////////////////////////////////////////////
//

/**
 * @brief      Constructs the object.
 *
 * @param[in]  code        The code
 * @param[in]  descriptor  The descriptor
 * @param[in]  numCycles   The number cycles
 */
MetadataInstruction::MetadataInstruction(char code, 
                                         std::string descriptor, 
                                         unsigned long numCycles)
{
    this->code = code;
    this->descriptor = descriptor;
    this->numCycles = numCycles;
}

/**
 * @brief      Copy constructor.
 *
 * @param[in]  obj   The object to copy
 */
MetadataInstruction::MetadataInstruction(const MetadataInstruction &obj)
{
    this->code = obj.code;
    this->descriptor = obj.descriptor;
    this->numCycles = obj.numCycles;
}

/**
 * @brief      Destroys the object.
 */
MetadataInstruction::~MetadataInstruction() {}

/**
 * @brief      Gets the code.
 *
 * @return     The code.
 */
const char MetadataInstruction::getCode() const
{
    return this->code;
}

/**
 * @brief      Gets the descriptor.
 *
 * @return     The descriptor.
 */
const std::string MetadataInstruction::getDescriptor() const
{
    return this->descriptor;
}

/**
 * @brief      Gets the number of cycles.
 *
 * @return     The number cycles.
 */
const unsigned long MetadataInstruction::getNumCycles() const
{
    return this->numCycles;
}

/**
 * @brief      Returns a string representation of the object.
 *
 * @return     String representation of the object.
 */
const std::string MetadataInstruction::toString() const
{
    return "";
}
