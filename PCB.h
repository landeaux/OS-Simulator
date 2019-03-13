/**
 * @file PCB.h
 * 
 * @brief Definition file for PCB (Process Control Block) class
 * 
 * @details Specifies all member methods of the PCB class
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
    PCB(unsigned pid);
    void setState(State state);
    const State getState() const;
    const unsigned getPID() const;
private:
    unsigned pid;
    State state;
};
//
// Terminating Precompiler Directives
// 
#endif  // PCB_H
//