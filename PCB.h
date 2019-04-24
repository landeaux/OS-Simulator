/**
 * @file PCB.h
 * 
 * @brief Definition file for PCB (Process Control Block) class
 * 
 * @details Specifies all member methods of the PCB class
 * 
 * @version 1.04
 *          Adam Landis (24 April 2019)
 *          - Add data members numInstr and numIOInstr along with their setters 
 *            and getters
 *          - Make all setters and getters inline
 *          - Change parameterized constructor signature to include numInstr and
 *            numIOInstr
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
    PCB(unsigned int pid, unsigned int numInstr, unsigned int numIOInstr);
    PCB(const PCB &obj);
    PCB& operator=(const PCB &rhs);
    ~PCB();

    /************************      Setter methods     *************************/
    /**
     * @brief      Sets the pc.
     *
     * @param[in]  pc    The program counter value to set
     */
    inline void setPC(unsigned int pc) { this->pc = pc; }

    /**
     * @brief      Sets the state.
     *
     * @param[in]  state  The state value to set
     */
    inline void setState(State state) { this->state = state; }

    /**************************************************************************/

    /************************      Getter methods     *************************/
    /**
     * @brief      Gets the pid.
     *
     * @return     The pid.
     */
    inline const unsigned int getPID() const { return this->pid; }

    /**
     * @brief      Gets the program counter (pc).
     *
     * @return     The program counter (pc).
     */
    inline const unsigned int getPC() const { return this->pc; }

    /**
     * @brief      Gets the number of instructions.
     *
     * @return     The number of instrucitons.
     */
    inline const unsigned int getNumInstr() const { return this->numInstr; }

    /**
     * @brief      Gets the number IO instructions.
     *
     * @return     The number of IO instructions.
     */
    inline const unsigned int getNumIOInstr() const { return this->numIOInstr; }

    /**
     * @brief      Gets the state.
     *
     * @return     The state.
     */
    inline const State getState() const { return this->state; }

    /**************************************************************************/
private:
    unsigned int pid, pc, numInstr, numIOInstr;
    State state;
};
//
// Terminating Precompiler Directives
// 
#endif  // PCB_H
//