/**
 * @file MetadataInstruction.h
 * 
 * @brief Definition file for MetadataInstruction class
 * 
 * @details Specifies all member methods of the MetadataInstruction class
 * 
 * @version 1.04
 *          Adam Landis (7 April 2019)
 *          - Add inclusion of semaphore.h header
 *          - Add setter/getter func prototypes for new data member semPtr
 * 
 * @version 1.03
 *          Adam Landis (6 April 2019)
 *          - Add overloaded assignment operator prototype
 *          - Add setWaitTime() and getWaitTime() method prototypes
 *          - Add data member waitTime
 * 
 * @version 1.02
 *          Adam Landis (13 March 2019)
 *          Add genLogString() method prototype
 * 
 * @version 1.01
 *          Adam Landis (9 March 2019)
 *          Add toString() method prototype
 * 
 * @version 1.00
 *          Adam Landis (8 March 2019)
 *          Initial development and testing of MetadataInstruction class
 * 
 * @note None
 */
//
// Precompiler Directives //////////////////////////////////////////////////////
//
#ifndef METADATA_INSTRUCTION_H
#define METADATA_INSTRUCTION_H
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include <string>
#include <semaphore.h>
//
// Class Definition ////////////////////////////////////////////////////////////
// 
class MetadataInstruction
{
public:
    MetadataInstruction(char code, std::string descriptor, unsigned long numCycles);
    MetadataInstruction(const MetadataInstruction &obj);
    MetadataInstruction& operator=(const MetadataInstruction &rhs);
    ~MetadataInstruction();
    void setWaitTime(unsigned long cycleTime);
    void setSemPtr(sem_t *semPtr);
    const float getWaitTime() const;
    sem_t *getSemPtr();
    const char getCode() const;
    const std::string getDescriptor() const;
    const unsigned long getNumCycles() const;
    const std::string toString() const;
    const std::string genLogString(bool isStart, unsigned pid);
private:
    char code;
    std::string descriptor;
    unsigned long numCycles;
    std::string logString;
    float waitTime;
    sem_t *semPtr;
};
//
// Terminating Precompiler Directives
// 
#endif  // METADATA_INSTRUCTION_H
//