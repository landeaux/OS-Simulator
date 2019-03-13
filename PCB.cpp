/**
 * @file PCB.cpp
 * 
 * @brief Implementation file for PCB (Process Control Block) class
 * 
 * @details Implements all member methods of PCB class
 * 
 * @version 1.00
 *          Adam Landis (6 March 2019)
 *          Initial development and testing of PCB class
 * 
 * @note Requries PCB.h
 */ 
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include "PCB.h"
//
// Class Member Implementation /////////////////////////////////////////////////
//
/**
 * @brief      Constructs the PCB object.
 */
PCB::PCB(unsigned pid)
{
    this->state = START;
    this->pid = pid;
}

/**
 * @brief      Sets the process state.
 *
 * @param[in]  state  The process state
 */
void PCB::setState(State state)
{
    this->state = state;
}

/**
 * @brief      Gets the process state.
 *
 * @return     The process state.
 */
const State PCB::getState() const
{
    return this->state;
}

/**
 * @brief      Gets the pid.
 *
 * @return     The pid.
 */
const unsigned PCB::getPID() const
{
    return this->pid;
}