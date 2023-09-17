#pragma once
#include "AI.h"
#include "Raven.h"

class GoalWander : public AI::Goal<Raven>
{
public:
    using Status = AI::Goal<Raven>::Status;

    GoalWander(Raven& agent);

    void Activate();
    Status Process();
    void Terminate();
};