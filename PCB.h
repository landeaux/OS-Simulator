/**
 * @file PCB.h
 * 
 * @brief Definition file for PCB (Process Control Block) class
 * 
 * @details Specifies all member methods of the PCB class
 * 
 * @version 1.03
 *          Adam Landis (24 April 2019)
 *          - Change name of setProgramCounter() and getProgramCounter() to 
 *            setPC() and getPC()
 *          - Add copy constructor, overloaded assignement operator, and 
 *            destructor
 * 
 * @version 1.02
 *          Adam Landis (14 March 2019)
 *          - Add data member pc.
 *          - Add setProgramCounter() and getProgramCounter() method prototypes.
 *          - Change all declarations and parameter types of "unsigned" to 
 *            "unsigned int".
 * 
 * @version 1.01
 *          Adam Landis (13 March 2019)
 *          - Add data member pid and method prototype getPID().
 *          - Add parameterized constructor to take in pid.
 * 
 * @version 1.00
 *          Adam Landis (6 March 2019)
 *          Initial development and testing of PCB class
 * 
 * @note None
 */
//
// Precompiler Directives //////////////////////////////////////////////////////
//
#ifndef PCB_H
#define PCB_H
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include <string>
//
// Global Constant Definitions /////////////////////////////////////////////////
// 
enum State {START, READY, RUNNING, WAIT, EXIT};
//
// Class Definition ////////////////////////////////////////////////////////////
// 
class PCB
{
public:
    PCB();
    PCB(unsigned int pid);
    PCB(const PCB &obj);
    PCB& operator=(const PCB &rhs);
    ~PCB();
    void setState(State state);
    void setPC(unsigned int pc);
    const State getState() const;
    const unsigned int getPID() const;
    const unsigned int getPC() const;
private:
    unsigned int pid;
    unsigned int pc;
    State state;
};
//
// Terminating Precompiler Directives
// 
#endif  // PCB_H
//