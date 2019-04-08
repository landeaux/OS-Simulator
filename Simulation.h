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

#include "Config.h"
#include "Metadata.h"
#include "PCB.h"
#include "Timer.h"
//
// Class Definition ////////////////////////////////////////////////////////////
// 
class Simulation
{
public:
    Simulation(const std::string& configFilename);
private:
    std::string configFilename;
};
//
// Terminating Precompiler Directives
// 
#endif  // SIMULATION_H
//