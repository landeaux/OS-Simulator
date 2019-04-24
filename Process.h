/**
 * @file Process.h
 * 
 * @brief Definition file for Process class
 * 
 * @details Specifies all member methods of the Process class
 * 
 * @version 1.01
 *          Adam Landis (24 April 2019)
 *          - Add data member instrVector to hold all instructions for the 
 *            process
 * 
 * @version 1.00
 *          Adam Landis (24 April 2019)
 *          Initial development and testing of Process class
 * 
 * @note None
 */
//
// Precompiler Directives //////////////////////////////////////////////////////
//
#ifndef Process_H
#define Process_H
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include <vector> // for instruction vector

#include "MetadataInstruction.h"
//
// Global Constant Definitions /////////////////////////////////////////////////
// 
///// None
//
// Class Definition ////////////////////////////////////////////////////////////
// 
class Process
{
public:
    Process();
    Process(unsigned int pid);
    Process(const Process &obj);
    Process& operator=(const Process &rhs);
    ~Process();
    const unsigned int getPID() const;
private:
    unsigned int pid;
    std::vector<MetadataInstruction> instrVector;
};
//
// Terminating Precompiler Directives
// 
#endif  // Process_H
//
