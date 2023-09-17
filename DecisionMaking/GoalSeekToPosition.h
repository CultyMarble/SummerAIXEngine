#pragma once
#include "AI.h"
#include "Raven.h"

class GoalSeekToPosition : public AI::Goal<Raven>
{
public:
    using Status = AI::Goal<Raven>::Status;

    GoalSeekToPosition(Raven& agent, const X::Math::Vector2& destination);

    void Activate();
    Status Process();
    void Terminate();

private:
    X::Math::Vector2 mDestination = X::Math::Vector2::Zero();
};