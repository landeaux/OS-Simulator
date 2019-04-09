# OS Simulator

This program runs from the terminal and takes one argument for the filename of the configuration file to be used to initialize the OS simulator. This config file contains the filename of a meta data file for use in loading the operations to perform in the OS simulator.

## Installation

To build the program, run the following command from the program directory:
```
make
```

## Running the program

To run the program, use the following command from the program directory, and in place of "\<filename>\", provide a configuration file as an argument. (NOTE: the configuration file must have the extension ".conf"):
```
./sim3 <filename>
```

## Mutex and Semaphores

Version 3.0 introduces the use of pthread mutex and semaphores for mutual exclusion and resource management. All mutex and semaphore statements can be found in __Simulation.cpp__. The mutex is defined on __line 21__, initialized on __line 67__, and the mutex lock and unlock statements are found on __lines 217 and 239__, respectively. The semaphores are defined on __line 22__, initialized on __lines 69 - 73__, and the sem_wait() and sem_post() calls are made on __lines 257 and 261__, respectively.

## Pthreads

Version 2.0 of the OS-Simulator introduces pthreads for a threaded wait() timer function. The location for created threads is in __Simulation.cpp__ at __line 129__ of the Simulation::startSimulation() function.

## PCB (Process Control Block)

Version 2.0 of the OS-Simulator also introduces the use of a PCB object to simulate a process control block. The class specification and implementation can be found in __PCB.h__ and __PCB.cpp__, respectively. A PCB* object is defined in __Simulation.cpp__ at the beginning of the __Simulation::startSimulation()__ function (__line 55__) and allocated a new PCB object when a new process is created (__line 90__).