/**
 * @file Process.cpp
 * 
 * @brief Implementation file for Process class
 * 
 * @details Implements all member methods of Process class
 * 
 * @version 1.02
 * 			Adam Landis (24 April 2019)
 * 			Update copy constructor and overloaded assignment operator to copy
 * 			data member instrVector.
 * 
 * @version 1.01
 *          Adam Landis (24 April 2019)
 *          Remove method getPID() - made inline in header file
 * 
 * @version 1.00
 *          Adam Landis (24 April 2019)
 *          Initial development and testing of Process class
 * 
 * @note Requries Process.h
 */ 
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include "Process.h"
//
// Class Member Implementation /////////////////////////////////////////////////
//

/**
 * @brief      Constructs the object (default).
 */
Process::Process() : pid(0) {}

/**
 * @brief      Constructs the Process object (parameterized).
 */
Process::Process(unsigned int pid) : pid(pid) {}

/**
 * @brief      Copy constructor
 *
 * @param[in]  obj   The object to be copied
 */
Process::Process(const Process &obj)
{
    this->pid = obj.pid;
    this->instrVector = obj.instrVector;
}

/**
 * @brief      Overloaded assignment operator
 *
 * @param[in]  rhs   The right hand side object to be copied
 *
 * @return     The copy of rhs Process
 */
Process& Process::operator=(const Process &rhs)
{
    this->pid = rhs.pid;
    this->instrVector = rhs.instrVector;

    return *this;
}

/**
 * @brief      Destroys the object.
 */
Process::~Process() {}
