/**
 * @file PCB.cpp
 * 
 * @brief Implementation file for PCB (Process Control Block) class
 * 
 * @details Implements all member methods of PCB class
 * 
 * @version 1.00
 *          Adam Landis (6 March 2019)
 *          Initial development and testing of PCB class
 * 
 * @note Requries PCB.h
 */ 
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include "Timer.h"
//
// Class Member Implementation /////////////////////////////////////////////////
//
/**
 * @brief      Constructs the Timer object.
 */
Timer::Timer() : isRunning(false) {}

/**
 * @brief      Destroys the Timer object.
 */
Timer::~Timer() {}

/**
 * @brief      Starts the timer.
 */
void Timer::startTimer()
{
    if (!this->isRunning)
    {
        this->isRunning = true;
        this->startTime = std::chrono::high_resolution_clock::now();
    }
}

/**
 * @brief      Stops the timer.
 */
void Timer::stopTimer()
{
    if (this->isRunning)
    {
        this->endTime = std::chrono::high_resolution_clock::now();
        this->duration = endTime - startTime;
        this->isRunning = false;
    }
}

/**
 * @brief      Gets the duration since start time in milliseconds.
 *
 * @return     The duration in ms (float).
 */
float Timer::getDuration()
{
    if (this->isRunning)
    {
        std::chrono::duration<float> current_duration;
        timepoint time_now;
        time_now = std::chrono::high_resolution_clock::now();
        current_duration = time_now - this->startTime;
        return current_duration.count() * 1000.0f;
    }

    return this->duration.count() * 1000.0f;
}
