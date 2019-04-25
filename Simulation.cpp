/**
 * @file Simulation.cpp
 * 
 * @brief Implementation file for Simulation class
 * 
 * @details Implements all member methods of Simulation class
 * 
 * @version 1.06
 *          Adam Landis (24 April 2019)
 *          Remove unneeded methods stubs for sortByPS(), sortBySJF(), and 
 *          sortByFCFS()
 * 
 * @version 1.05
 *          Adam Landis (24 April 2019)
 *          Implement method sortReadyQueue() and add method stubs for 
 *          sortByPS(), sortBySJF(), and sortByFCFS()
 * 
 * @version 1.04
 *          Adam Landis (24 April 2019)
 *          - Implement methods printReadyQueue(), printWaitQueue(), 
 *            printQueue(std::string), and printProcessVector() to print the 
 *            contents of readyQueue, waitQueue, and processVector, respectively
 *          - Add calls to end of createProcesses() to printQueue() and 
 *            printProcessVector()
 *           
 * @version 1.03
 *          Adam Landis (24 April 2019)
 *          Add code to createProcesses() to count numInstr and numIOInstr and 
 *          use to instantiate PCB object.
 * 
 * @version 1.02
 *          Adam Landis (24 April 2019)
 *          Implement basic, unfinished version of createProcesses() method
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
 * @brief      Creates processes for each process in metadata, along with its 
 *             PCB.
 */     
void Simulation::createProcesses()
{
    int i = 1;
    metadataQueue mdQueueCopy = this->metadata->getMetadataQueue();

    while (!mdQueueCopy.empty())
    {
        MetadataInstruction instr = mdQueueCopy.front();
        
        if (instr.toString() == "A{begin}0")
        {
            mdQueueCopy.pop();

            std::cout << "\nProcess found!\nThe instructions are...\n";
            
            // Create new Process
            unsigned int numInstr = 0, numIOInstr = 0;
            Process process(i);
            std::vector<MetadataInstruction> instrVector;

            instr = mdQueueCopy.front();

            while (instr.toString() != "A{finish}0")
            {
                instrVector.push_back(instr);

                std::cout << "\t" << instr.toString() << std::endl;
                std::cout << "\t\t" << instr.getCode() << std::endl;

                if (instr.getCode() == 'I' || instr.getCode() == 'O')
                {
                    numIOInstr++;
                    numInstr++;
                }
                else
                {
                    numInstr++;
                }

                mdQueueCopy.pop();
                instr = mdQueueCopy.front();
            }
            process.setInstrVector(instrVector);
            // END Create new Process

            this->processVector.push_back(process);

            // Create new PCB
            PCB pcb(i, numInstr, numIOInstr);
            // END Create new PCB

            pcb.setState(READY);
            this->readyQueue.push(pcb);
            i++;
        }

        mdQueueCopy.pop();
    }

    printQueue("ready");
    printQueue("wait");
    printProcessVector();
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
            pcb = new PCB(++pid, 0, 0);
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

/**
 * @brief      Sorts the readyQueue according to given algorithm (PS, SJF, or 
 *             FCFS)
 *
 * @param[in]  algo  The scheduling algorithm
 */
void Simulation::sortReadyQueue(std::string algo)
{
    std::vector<PCB> temp;

    unsigned size = this->readyQueue.size();

    for (unsigned i = 0; i < size; i++)
    {
        temp[i] = readyQueue.front();
        readyQueue.pop();
    }

    if (algo == "PS")
    {
        std::sort(temp.begin(), temp.end(), by_largestNumIOInstr());
    }
    else if (algo == "SJF")
    {
        std::sort(temp.begin(), temp.end(), by_smallestNumInstr());
    }
    else if (algo == "FCFS")
    {
        std::sort(temp.begin(), temp.end(), by_smallestPID());
    }

    for (unsigned i = 0; i < size; i++)
    {
        readyQueue.push(temp[i]);
    }
}

/**
 * @brief      Prints the contents of readyQueue
 */
void Simulation::printReadyQueue()
{
    std::queue<PCB> readyQueueCopy = this->readyQueue;
    while (!readyQueue.empty())
    {
        PCB tempPCB = this->readyQueue.front();

        std::cout << tempPCB << std::endl;

        readyQueue.pop();
    }
}

/**
 * @brief      Prints the contents of waitQueue
 */
void Simulation::printWaitQueue()
{
    std::queue<PCB> readyQueueCopy = this->readyQueue;
    while (!readyQueue.empty())
    {
        PCB tempPCB = this->readyQueue.front();

        std::cout << tempPCB << std::endl;

        readyQueue.pop();
    }
}

/**
 * @brief      Prints the contents of the specified queue ("wait" or "ready")
 *
 * @param[in]  queueType  The queue type
 */
void Simulation::printQueue(std::string queueType)
{
    std::cout << "\nPrinting the contents of " << queueType << "queue...\n\n";

    if (queueType == "ready")
    {
        this->printReadyQueue();
    }
    else if (queueType == "wait")
    {
        this->printWaitQueue();
    }
}

/**
 * @brief      Prints the contents of processVector
 */
void Simulation::printProcessVector()
{
    std::cout << "\nPrinting the contents of processVector...\n\n";
    for (unsigned k = 0; k < this->processVector.size(); k++)
    {
        Process tempProcess = processVector[k];

        std::cout << tempProcess << std::endl;
    }
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