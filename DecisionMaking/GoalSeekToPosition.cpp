#include "GoalSeekToPosition.h"

GoalSeekToPosition::GoalSeekToPosition(Raven& agent, const X::Math::Vector2& destination)
    : Goal(agent), mDestination(destination)
{

}

void GoalSeekToPosition::Activate()
{
    mStatus = AI::Goal<Raven>::Status::Active;
    mAgent.SetSeek(true);
    mAgent.destination = mDestination;
}

AI::Goal<Raven>::Status GoalSeekToPosition::Process()
{
    ActivateIfInactive();

    if (X::Math::DistanceSqr(mAgent.position, mDestination) < 10.0f)
        mStatus = AI::Goal<Raven>::Status::Completed;

    return mStatus;
}

void GoalSeekToPosition::Terminate()
{
    mAgent.SetSeek(false);
}