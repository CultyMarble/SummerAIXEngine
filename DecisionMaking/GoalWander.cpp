#include "GoalWander.h"

GoalWander::GoalWander(Raven& agent)
    : Goal(agent)
{

}

void GoalWander::Activate()
{
    mStatus = AI::Goal<Raven>::Status::Active;

    mAgent.SetWander(true);
}

AI::Goal<Raven>::Status GoalWander::Process()
{
    ActivateIfInactive();

    return mStatus;
}

void GoalWander::Terminate()
{
    mAgent.SetWander(false);
}