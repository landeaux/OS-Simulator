/**
 * @file Process.h
 * 
 * @brief Definition file for Process class
 * 
 * @details Specifies all member methods of the Process class
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
///// None
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
};
//
// Terminating Precompiler Directives
// 
#endif  // Process_H
//
