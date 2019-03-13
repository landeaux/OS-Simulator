/**
 * @file Timer.h
 * 
 * @brief Definition file for Timer class
 * 
 * @details Specifies all member methods of the Timer class
 * 
 * @version 1.00
 *          Adam Landis (13 March 2019)
 *          Initial development and testing of Timer class
 * 
 * @note None
 */
//
// Precompiler Directives //////////////////////////////////////////////////////
//
#ifndef TIMER_H
#define TIMER_H
//
// Header Files ////////////////////////////////////////////////////////////////
// 
#include <chrono>
//
// Typedefs
//
typedef std::chrono::time_point<std::chrono::high_resolution_clock> timepoint;
//
// Class Definition ////////////////////////////////////////////////////////////
// 
class Timer
{
public:
    Timer();
    ~Timer();
    void startTimer();
    void stopTimer();
    float getDuration();
private:
    timepoint startTime, endTime;
    std::chrono::duration<float> duration;
    bool isRunning;
};
//
// Terminating Precompiler Directives
// 
#endif  // TIMER_H
//