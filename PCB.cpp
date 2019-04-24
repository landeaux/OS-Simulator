/**
 * @file PCB.cpp
 * 
 * @brief Implementation file for PCB (Process Control Block) class
 * 
 * @details Implements all member methods of PCB class
 * 
 * @version 1.03
 *          Adam Landis (24 April 2019)
 *          - Change name of setProgramCounter() and getProgramCounter() to 
 *            setPC() and getPC().
 *          - Implement copy constructor, overloaded assignement operator, and 
 *            destructor.
 *          - Change default constructor to instantiate default parameters via 
 *            initialization list.
 *          - Change copy constructor to instantate parameters via 
 *            initialization list and instantiate pc to 0.
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
PCB::PCB() : pid(0), pc(0), state(START) {}

/**
 * @brief      Constructs the PCB object (parameterized).
 */
PCB::PCB(unsigned int pid) : pid(pid), pc(0), state(START) {}

/**
 * @brief      Copy constructor
 *
 * @param[in]  obj   The object to be copied
 */
PCB::PCB(const PCB &obj)
{
    this->pid = obj.pid;
    this->pc = obj.pc;
    this->state = obj.state;
}

/**
 * @brief      Overloaded assignment operator
 *
 * @param[in]  rhs   The right hand side object to be copied
 *
 * @return     The copy of rhs PCB
 */
PCB& PCB::operator=(const PCB &rhs)
{
    this->pid = rhs.pid;
    this->pc = rhs.pc;
    this->state = rhs.state;

    return *this;
}

/**
 * @brief      Destroys the object.
 */
PCB::~PCB() {}

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
void PCB::setPC(unsigned int pc)
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

/**
 * @brief      Gets the program counter pc.
 *
 * @return     The value of the program counter pc.
 */
const unsigned int PCB::getPC() const
{
    return this->pc;
}
