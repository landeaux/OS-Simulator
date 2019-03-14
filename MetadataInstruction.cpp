/**
 * @file MetadataInstruction.cpp
 * 
 * @brief Implementation file for MetadataInstruction class
 * 
 * @details Implements all member methods of MetadataInstruction class
 * 
 * @version 1.02
 *          Adam Landis
 *          Add genLogString() method implementation
 * 
 * @version 1.01
 *          Adam Landis
 *          Add toString() method implementation
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
    std::string result;

    std::string code = std::string(1, this->code);
    std::string descriptor = this->descriptor;
    std::string numCycles = std::to_string(this->numCycles);

    result = code + "{" + descriptor + "}" + numCycles;

    return result;
}

const std::string MetadataInstruction::genLogString(bool isStart, unsigned pid)
{
    std::string result;

    if (this->code == 'S')
    {
        result += "Simulator program ";
        result += (this->descriptor == "begin") ? "starting" : "ending";
    }
    else if (this->code == 'A')
    {
        result += "OS: ";
        
        if (this->descriptor == "begin")
        {
            result += isStart ? "preparing" : "starting";
        }
        else if (this->descriptor == "finish")
        {
            result += "removing";
        }

        result += " process " + std::to_string(pid);
    }
    else
    {
        result += "Process " + std::to_string(pid) + ": ";

        if (this->code == 'M')
        {
            if (this->descriptor == "allocate")
            {
                result += isStart ? "allocating memory" : "memory allocated at";
            }
            else if (this->descriptor == "block")
            {
                result += isStart ? "start" : "end";
                result += " memory blocking";
            }
        }
        else
        {
            
            result += isStart ? "start" : "end";
            
            if (this->code == 'P')
            {
                result += " processing action";
            }
            else
            {
                result += " " + this->descriptor + " ";
                
                if (this->code == 'I' || this->code == 'O')
                {
                    result += (this->code == 'I') ? "input" : "output";
                }
            }
        }
    }
        

    return result;
}