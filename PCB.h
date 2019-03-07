#ifndef PCB_H
#define PCB_H

#include <string>

enum State {START, READY, RUNNING, WAIT, EXIT};

class PCB
{
public:
    PCB();
    void setState(State state);
    const State getState() const;
private:
    State state;
};

#endif