/**
 * @file Simulation.cpp
 * 
 * @brief Implementation file for Simulation class
 * 
 * @details Implements all member methods of Simulation class
 * 
 * @version 1.01
 *          Adam Landis (24 April 2019)
 *          Add global constant CPU_SCHEDULING_CODES
 * 
 * @version 1.00
 *          Adam Landis (8 March 2019)
 *          Initial development and testing of Simulation class
 * 
 * @note Requries Simulation.h
 */ 
//
// Header Files ////////////////////////////////////////////////////////////////
//
#include "Simulation.h"
//
// Global Constant Definitions /////////////////////////////////////////////////
// 
const std::string CPU_SCHEDULING_CODES[3] = { "FIFO", "PS", "SJF" };
//
// Global Variable Definitions /////////////////////////////////////////////////
//
pthread_mutex_t mutex;                          // mutex for memory mgmt
sem_t semHD, semProj, semKB, semMon, semScan;   // semaphores for resource mgmt
//
// Class Member Implementation /////////////////////////////////////////////////
//

/**
 * @brief      Constructs the object.
 *
 * @param[in]  configFilename  The configuration filename
 */
Simulation::Simulation(const std::string& configFilename): configFilename(configFilename)
{
    this->config = new Config(configFilename);

    std::string metadataFilename = this->config->getSettingVal("File Path");

    if (metadataFilename.empty())
    {
        throw std::string("Error: 'File Path' missing from config file");
    }

    this->metadata = new Metadata(metadataFilename, this->config);
    this->metadata->setSemPtrs(&semHD, &semProj, &semKB, &semMon, &semScan);
}

/**
 * @brief      Starts the OS simulation.
 */
void Simulation::startSimulation()
{
    metadataQueue mdQueue = this->metadata->getMetadataQueue();
    Timer myTimer;
    float duration;
    PCB* pcb;
    unsigned numHD, countHD = 0,
             numProj, countProj = 0,
             sysMem, memBlockSize, nextBlockPtr = 0,
             pid = 0;

    numHD   = (unsigned) strToUnsignedLong(this->config->getSettingVal("Hard drive quantity"));
    numProj = (unsigned) strToUnsignedLong(this->config->getSettingVal("Projector quantity"));

    sysMem       = (unsigned) strToUnsignedLong(this->config->getSettingVal("System memory"));
    memBlockSize = (unsigned) strToUnsignedLong(this->config->getSettingVal("Memory block size"));

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

        this->config->logData(data);

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
            
            this->config->logData(data);
        }

        mdQueue.pop();
    }

    this->config->logData("\n");
}

//
// Free Function Implementation ////////////////////////////////////////////////
//

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