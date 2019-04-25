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

## CPU Scheduling Algorithms

This version of the OS Simulator introduces CPU Scheduling algorithms. These algorithm can be found in __Simulation.cpp__ in the function __Simulation::sortReadyQueue__ on __line 398__.