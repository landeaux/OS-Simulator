// Program Information /////////////////////////////////////////////////////////

/**
 * @file main.cpp
 *
 * @brief Driver program for OS Simulator
 *
 * @details This program is the driver program for OS Simulator classes
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
#include <iomanip>      // setprecision, fixed
#include <fstream>      // ifstream, ofstream
#include <string>       // string, to_string
#include <queue>        // for the metadata
#include <pthread.h>    // for threads
#include <semaphore.h>  // for semaphores

#include "helpers.h"
#include "Config.h"
#include "PCB.h"
#include "MetadataInstruction.h"
#include "Metadata.h"
#include "Timer.h"
#include "Simulation.h"
//
// Global Constant Definitions /////////////////////////////////////////////////
//
const std::string METADATA_HEADER = "Start Program Meta-Data Code:";
const std::string METADATA_FOOTER = "End Program Meta-Data Code.";
const std::string METADATA_CODES = "SAPIOM";
const std::string METADATA_DESCRIPTORS[] = {
        "begin",
        "finish",
        "hard drive",
        "keyboard",
        "scanner",
        "monitor",
        "run",
        "allocate",
        "projector",
        "block",
};
//
// Global Variable Definitions /////////////////////////////////////////////////
//
Config *config;
Metadata *metadata;
pthread_mutex_t mutex;                          // mutex for memory mgmt
sem_t semHD, semProj, semKB, semMon, semScan;   // semaphores for resource mgmt
//
// Free Function Prototypes ////////////////////////////////////////////////////
//
void startSimulation(metadataQueue mdQueue);
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

        std::string configFilename = argv[1], metadataFilename;

        Simulation mySim(configFilename);

        config = new Config(configFilename);

        metadataFilename = config->getSettingVal("File Path");

        if (metadataFilename.empty())
        {
            throw std::string("Error: 'File Path' missing from config file");
        }

        metadata = new Metadata(metadataFilename, config);

        startSimulation(metadata->getMetadataQueue());
    }
    catch (std::string& e)
    {
        std::cout << e << std::endl;
        exit(1);
    }

    return 0;
}
//
// Free Function Implementation ////////////////////////////////////////////////
//

/**
 * @brief      Starts the OS simulation.
 *
 * @param[in]  mdQueue  The queue of metadata instructions
 */
void startSimulation(metadataQueue mdQueue)
{
    Timer myTimer;
    float duration;
    PCB* pcb;
    unsigned numHD, countHD = 0,
             numProj, countProj = 0,
             sysMem, memBlockSize, nextBlockPtr = 0,
             pid = 0;

    numHD   = (unsigned) strToUnsignedLong(config->getSettingVal("Hard drive quantity"));
    numProj = (unsigned) strToUnsignedLong(config->getSettingVal("Projector quantity"));

    sysMem       = (unsigned) strToUnsignedLong(config->getSettingVal("System memory"));
    memBlockSize = (unsigned) strToUnsignedLong(config->getSettingVal("Memory block size"));

    pthread_mutex_init(&mutex, NULL);

    sem_init(&semHD,   0, numHD);
    sem_init(&semProj, 0, numProj);
    sem_init(&semKB,   0, 1);
    sem_init(&semMon,  0, 1);
    sem_init(&semScan, 0, 1);

    std::cout << std::setprecision(6) << std::fixed;

    myTimer.startTimer();

    while (!mdQueue.empty())
    {
        MetadataInstruction instr = mdQueue.front();
        char code = instr.getCode();
        std::string descriptor = instr.getDescriptor();
        float wait_time = instr.getWaitTime();
        unsigned memAddr;
        std::string data;
        
        if (code == 'A' && descriptor == "begin")
        {
            pcb = new PCB(++pid);
            pcb->setState(START);
        }
        else if (code == 'P')
        {
            pcb->setState(RUNNING);
        }

        duration = myTimer.getDuration() / 1000.0f;
        data = std::to_string(duration) + " - " + instr.genLogString(true, pid);

        if (descriptor == "hard drive")
        {
            data += std::to_string(countHD++ % numHD);
        }
        else if (descriptor == "projector")
        {
            data += std::to_string(countProj++ % numProj);
        }

        data += "\n";

        config->logData(data);

        if (code == 'I' || code == 'O')
        {
            pcb->setState(WAIT);

            pthread_t tid;
            // pthread_create(&tid, NULL, executeIOInstruction, (void*)&instr);
            pthread_create(&tid, NULL, wait, (void*)&wait_time);
            pthread_join(tid, NULL);
        }
        else if (code == 'M')
        {
            executeMemInstruction(instr, nextBlockPtr, memBlockSize, memAddr, sysMem);
        }
        else
        {
            wait(wait_time);
        }

        if (code == 'A' && descriptor == "finish")
        {
            pcb->setState(EXIT);
            delete pcb;
        }
        else if (code != 'S')
        {
            pcb->setState(READY);
        }

        if (   code != 'S' && 
             !(code == 'A' && descriptor == "finish") )
        {
            duration = myTimer.getDuration() / 1000.0f;
            data = std::to_string(duration) + " - " + instr.genLogString(false, pid);

            if (code == 'M' && descriptor == "allocate")
            {
                data += " ";
                data += uintToHexStr(memAddr);
            }

            data += "\n";
            
            config->logData(data);
        }

        mdQueue.pop();
    }

    config->logData("\n");
}

/**
 * @brief      Thread that waits for a specified amount of time in milliseconds.
 *
 * @param      param  The duration parameter
 *
 * @return     None
 */
void* wait(void* param)
{
    float duration = *((float*)param);
    wait(duration);
    pthread_exit(0);
}

/**
 * @brief      Non-threaded version of wait() - waits for a specified amount of
 *             time in milliseconds.
 *
 * @param      duration  The duration to wait for
 *
 * @return     None
 */
void wait(float duration)
{
    Timer myTimer;

    myTimer.startTimer();
    while (myTimer.getDuration() < duration);
}

/**
 * @brief      Executes a memory instruction
 *
 * @param[in]  instr         The instruction
 * @param      nextBlockPtr  The next block pointer
 * @param[in]  blockSize     The block size
 * @param      memAddr       The memory address
 * @param[in]  sysMem        The system memory
 */
void executeMemInstruction(
    MetadataInstruction instr, 
    unsigned &nextBlockPtr, 
    unsigned blockSize, 
    unsigned &memAddr,
    unsigned sysMem)
{
    std::string descriptor = instr.getDescriptor();

    pthread_mutex_lock(&mutex);

    if (descriptor == "allocate")
    {
        if (sysMem - nextBlockPtr >= blockSize)
        {
            memAddr = nextBlockPtr;
            nextBlockPtr += blockSize;
        }
        else
        {
            memAddr = 0;
            nextBlockPtr = blockSize;
        }
    }
    else if (descriptor == "block")
    {
        /* For now, do nothing */
    }

    wait(instr.getWaitTime());
    
    pthread_mutex_unlock(&mutex);
}

/**
 * @brief      Executes an IO instruction
 *
 * @param      param  The parameter holding the instruction
 *
 * @return     None
 */
void* executeIOInstruction(void* param)
{
    MetadataInstruction instr = *((MetadataInstruction*)param);
    sem_t *semPtr;
    std::string descriptor = instr.getDescriptor();

    semPtr = instr.getSemPtr();

    sem_wait(semPtr);

    wait(instr.getWaitTime());

    sem_post(semPtr);

    return 0;
}
