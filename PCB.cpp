/**
 * @file PCB.cpp
 * 
 * @brief Implementation file for PCB (Process Control Block) class
 * 
 * @details Implements all member methods of PCB class
 * 
 * @version 1.02
 *          Adam Landis (14 March 2019)
 *          - Implement setProgramCounter() and getProgramCounter() methods.
 *          - Change all declarations and parameter types of "unsigned" to 
 *            "unsigned int".
 * 
 * @version 1.01
 *          Adam Landis (13 March 2019)
 *          Add method getPID() implementation
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
 * @brief      Constructs the object (default).
 */
PCB::PCB() {}

/**
 * @brief      Constructs the PCB object (parameterized).
 */
PCB::PCB(unsigned int pid)
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
 * @brief      Sets the program counter.
 *
 * @param[in]  pc    The address to set the program counter to (unsigned int)
 */
void PCB::setProgramCounter(unsigned int pc)
{
    this->pc = pc;
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
const unsigned int PCB::getPID() const
{
    return this->pid;
}


const unsigned int PCB::getProgramCounter() const
{
    return this->pc;
}
