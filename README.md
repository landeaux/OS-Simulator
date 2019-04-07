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
./sim2 <filename>
```

## Mutex and Semaphores

Version 3.0 introduces the use of pthread mutex and semaphores for mutual exclusion and resource management. All mutex and semaphore statements can be found in __main.cpp__. The mutex is defined on __line 166__, initialized on __line 1082__, and the mutex lock and unlock statements are found on __lines 1218 and 1238__, respectively. The semaphores are defined on __line 167__, initialized on __lines 1084 - 1085__, and the sem_wait() and sem_post() calls are made on __lines 1266 and 1270__, respectively.

## Pthreads

Version 2.0 of the OS-Simulator introduces pthreads for a threaded wait() timer function. The location of the created threads is in __main.cpp__ at __line 1052__ (in the startSimulation() function).

## PCB (Process Control Block)

Version 2.0 of the OS-Simulator also introduces the use of a PCB object to simulate a process control block. The class specification and implementation can be found in __PCB.h__ and __PCB.cpp__, respectively. A PCB* object is defined in __main.cpp__ at the beginning of the __startSimulation()__ function (__line 1018__) and allocated a new PCB object when a new process is created (__line 1036__).