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

## Pthreads

This version of the OS-Simulator uses pthreads for a threaded wait() timer function. The location of the created threads is in __main.cpp__ at __line 1052__ (in the startSimulation() function).

## PCB (Process Control Block)

This version of the OS-Simulator also uses a PCB object to simulate a process control block. The class specification and implementation can be found in __PCB.h__ and __PCB.cpp__, respectively. A PCB* object is defined in __main.cpp__ at the beginning of the __startSimulation()__ function (__line 1018__) and allocated a new PCB object when a new process is created (__line 1036__).