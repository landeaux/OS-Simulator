/**
 * @file PCB.cpp
 * 
 * @brief Implementation file for PCB (Process Control Block) class
 * 
 * @details Implements all member methods of PCB class
 * 
 * @version 1.05
 *          - Update copy contructor and overloaded assignment operator to copy
 *            numInstr and numIOInstr data member values
 *          - Implement overloaded stream insertion operator << method
 * 
 * @version 1.04
 *          Adam Landis (24 April 2019)
 *          - Remove all setters and getters - made them inline
 *          - Change constructors to initialize new data members numInstr and 
 *            numIOInstr
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
PCB::PCB() : pid(0), pc(0), numInstr(0), numIOInstr(0), state(START) {}

/**
 * @brief      Constructs the PCB object (parameterized).
 */
PCB::PCB(unsigned int pid, unsigned int numInstr, unsigned int numIOInstr) : 
    pid(pid), 
    pc(0), 
    numInstr(numInstr), 
    numIOInstr(numIOInstr), 
    state(START)
{}

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
    this->numInstr = obj.numInstr;
    this->numIOInstr = obj.numIOInstr;
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
    this->numInstr = rhs.numInstr;
    this->numIOInstr = rhs.numIOInstr;

    return *this;
}

/**
 * @brief      Destroys the object.
 */
PCB::~PCB() {}

/**
 * @brief      Overloaded stream insertion operator for output
 *
 * @param      out   The output stream
 * @param[in]  pcb   The pcb
 *
 * @return     The output stream
 */
std::ostream & operator << (std::ostream &out, const PCB &pcb)
{
    out << "PID: " << pcb.pid << std::endl;
    out << "PC: " << pcb.pc << std::endl;
    out << "State: " << pcb.state << std::endl;
    out << "NumInstr: " << pcb.numInstr << std::endl;
    out << "NumIOInstr: " << pcb.numIOInstr << std::endl;

    return out;
}