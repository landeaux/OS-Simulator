/**
 * @file Process.cpp
 * 
 * @brief Implementation file for Process class
 * 
 * @details Implements all member methods of Process class
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

    return *this;
}

/**
 * @brief      Destroys the object.
 */
Process::~Process() {}

/**
 * @brief      Gets the pid.
 *
 * @return     The pid.
 */
const unsigned int Process::getPID() const
{
    return this->pid;
}
