/**
 * @file Simulation.h
 * 
 * @brief Specification file for Simulation class
 * 
 * @details Specifies all member methods of the Simulation class
 * 
 * @version 1.02
 *          Adam Landis (24 April 2019)
 *          Add method prototype createProcesses()
 * 
 * @version 1.01
 *          Adam Landis (24 April 2019)
 *          Add data members readyQueue, waitQueue, and processVector
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
#include <queue>        // for ready and wait queues
#include <vector>       // for vector of processes

#include "helpers.h"
#include "Config.h"
#include "MetadataInstruction.h"
#include "Metadata.h"
#include "Process.h"
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
    void createProcesses();
private:
    std::string configFilename;
    Config *config;
    Metadata *metadata;
    std::queue<PCB> readyQueue;
    std::queue<PCB> waitQueue;
    std::vector<Process> processVector;
};
//
// Terminating Precompiler Directives
// 
#endif  // SIMULATION_H
//