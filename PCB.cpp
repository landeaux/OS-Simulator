#include "PCB.h"

PCB::PCB(): state(START) {}

void PCB::setState(State state)
{
    this->state = state;
}

const State PCB::getState() const
{
    return this->state;
}