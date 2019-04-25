/**
 * @file Process.h
 * 
 * @brief Definition file for Process class
 * 
 * @details Specifies all member methods of the Process class
 * 
 * @version 1.04
 *          Adam Landis (24 April 2019)
 *          Add overloaded stream insertion operator << method signature.
 * 
 * @version 1.03
 *          Adam Landis (24 April 2019)
 *          Add getter method to get instrVector 
 * 
 * @version 1.02
 *          Adam Landis (24 April 2019)
 *          - Add setter and getter methods for instrVector data member.
 *          - Make getter method for getPID() to be inline.
 * 
 * @version 1.01
 *          Adam Landis (24 April 2019)
 *          Add data member instrVector to hold all instructions for the process
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
#include <vector> // for instruction vector
#include <iostream>

#include "MetadataInstruction.h"
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

    friend std::ostream & operator << (std::ostream &out, const Process &process);

    /************************      Setter methods     *************************/
    /**
     * @brief      Sets the instr vector.
     *
     * @param[in]  instrVector  The instr vector to copy
     */
    inline void setInstrVector(std::vector<MetadataInstruction> instrVector)
    {
        this->instrVector = instrVector;
    }
    /**************************************************************************/

    /************************      Getter methods     *************************/
    /**
     * @brief      Gets the pid.
     *
     * @return     The pid.
     */
    inline const unsigned int getPID() const { return this->pid; }

    /**
     * @brief      Gets the instruction vector.
     *
     * @return     The instruction vector.
     */
    inline const std::vector<MetadataInstruction> getInstrVector() const
    {
        return this->instrVector;
    }

    /**
     * @brief      Gets the instruciton from instrVector located at index idx.
     *
     * @param[in]  idx   The index
     *
     * @return     The instr.
     */
    inline const MetadataInstruction getInstr(unsigned idx) const
    { 
        return this->instrVector[idx];
    }
    /**************************************************************************/

private:
    unsigned int pid;
    std::vector<MetadataInstruction> instrVector;
};
//
// Terminating Precompiler Directives
// 
#endif  // Process_H
//
