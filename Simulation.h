/**
 * @file Simulation.h
 * 
 * @brief Specification file for Simulation class
 * 
 * @details Specifies all member methods of the Simulation class
 * 
 * @version 1.00
 *          Adam Landis (8 April 2019)
 *          Initial development and testing of Simulation class
 * 
 * @note None
 */
//
// Precompiler Directives //////////////////////////////////////////////////////
//
#ifndef SIMULATION_H
#define SIMULATION_H
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include <string>
#include <iomanip>      // setprecision, fixed
#include <pthread.h>    // for threads
#include <semaphore.h>  // for semaphores

#include "helpers.h"
#include "Config.h"
#include "MetadataInstruction.h"
#include "Metadata.h"
#include "PCB.h"
#include "Timer.h"
//
// Free Function Prototypes ////////////////////////////////////////////////////
//
void* wait(void* param);
void wait(float duration);
void executeMemInstruction(
    MetadataInstruction instr, 
    unsigned &nextBlockPtr, 
    unsigned blockSize, 
    unsigned &memAddr,
    unsigned sysMem);
void* executeIOInstruction(void* param);
//
// Class Definition ////////////////////////////////////////////////////////////
// 
class Simulation
{
public:
    Simulation(const std::string& configFilename);

    void startSimulation();
private:
    std::string configFilename;
    Config *config;
    Metadata *metadata;
};
//
// Terminating Precompiler Directives
// 
#endif  // SIMULATION_H
//