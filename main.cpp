// Program Information /////////////////////////////////////////////////////////

/**
 * @file main.cpp
 *
 * @brief Driver program for OS Simulator
 *
 * @details This program is the driver program for OS Simulator classes
 * 
 * @version 3.06
 *          Adam Landis (8 April 2019)
 *          - Move all Metadata-related instructions and data into Metadata.h
 *            and Metadata.cpp files for Metadata class
 *          - Move all Simulation-related instructions and data into Simulation.h
 *            and Simulation.cpp files for Simulation class
 * 
 * @version 3.05
 *          Adam Landis (8 April 2019)
 *          Update to utilize new Config class and moved all helper functions to
 *          their own helpers.cpp file.
 * 
 * @version 3.04
 *          Adam Landis (7 April 2019)
 *          - Add semaphores for all I/O resources
 * 
 * @version 3.03
 *          Adam Landis (7 April 2019)
 *          - Add support for semaphores
 *          - Change implementation of startSimulation() to initialize 
 *            semaphores for resource management
 *          - Add threaded function executeIOInstruction() to execute IO 
 *            instructions and utilize semaphores for resource managemnent and 
 *            mutual exclusion
 * 
 * @version 3.02
 *          Adam Landis (6 April 2019)
 *          - Add executeMemInstruction() for memory allocation (and soon,  
 *            blocking) functionality
 *          - Add uintToHex() function to convert an unsigned to a 
 *            hexidecimal string
 *          - Add mutex locks for memory instructions
 * 
 * @version 3.01
 *          Adam Landis (6 April 2019)
 *          - Change implementation of parseMetadataFile() to calc and set 
 *            waitTime for each metadata instruction before pushing it onto 
 *            queue.
 *          - Change implementation of startSimulation() to get wait_time from 
 *            each metadata instruction instead of calculating it every time in
 *            the loop.
 * 
 * @version 3.00
 *          Adam Landis (5 April 2019)
 *          Add support for configuration settings.
 * 
 * @version 2.05
 *          Adam Landis (15 March 2019)
 *          Change implementation of startSimulation() to change PCB state at
 *          appropriate moments
 * 
 * @version 2.04
 *          Adam Landis (14 March 2019)
 *          - Add non-threaded version of wait() function for non I/O operations
 *          - Add genRandNum() function to generate random numbers for memory 
 *            allocation purposes
 *          - Change implementation of startSimulation to properly log all 
 *            timestamped operations
 * 
 * @version 2.03
 *          Adam Landis (13 March 2019)
 *          - Add Timer class
 *          - Include pthread.h
 *          - Add threaded wait() function
 *          - Change implementation of startSimulation() to test these new 
 *            additions
 * 
 * @version 2.02
 *          Adam Landis (9 March 2019)
 *          Add MetadataInstruction Class and remove metadataInstruction struct.
 *          Modify implementation of functions which used struct to use class 
 *          instead.
 *
 * @version 2.01
 *          Adam Landis (6 March 2019)
 *          Add PCB class            
 *
 * @version 1.05
 *          Adam Landis (18 February 2019)
 *          Add logging functionality for config data and metadata and
 *          implemented helper functions related to logging
 *
 * @version 1.04
 *          Adam Landis (17 February 2019)
 *          Remove ability to take in an arbitrary number of config files - not
 *          required
 *
 * @version 1.03
 *          Adam Landis (17 February 2019)
 *          Add functionality to parse metadata files and add helper functions
 *
 * @version 1.02
 *          Adam Landis (16 February 2019)
 *          Add ability to take in an arbitrary number of config files
 *
 * @version 1.01
 *          Adam Landis (15 February 2019)
 *          Implement parsing of config file and error checking
 *
 * @version 1.00
 *          Adam Landis (14 February 2019)
 *          Initial development
 *
 * @note None
 */ 

// Program Description/Support /////////////////////////////////////////////////
/*
 * This program runs from the terminal and takes one argument for the filename
 * of the configuration file to be used to initialize the OS simulator. This
 * config file contains the filename of a meta data file for use in loading the
 * operations to perform in the OS simulator.
 */
// Header Files ////////////////////////////////////////////////////////////////
//
#include <iostream>     // cout
#include <string>       // string

#include "Simulation.h"
//
// Main Function Implementation ////////////////////////////////////////////////
//
int main(int argc, char *argv[])
{
    try
    {
        if (argc == 1)
        {
            throw std::string("Error: missing argument for configuration file");
        }

        std::string configFilename = argv[1];

        Simulation mySim(configFilename);

        mySim.startSimulation();
    }
    catch (std::string& e)
    {
        std::cout << e << std::endl;
        exit(1);
    }

    return 0;
}