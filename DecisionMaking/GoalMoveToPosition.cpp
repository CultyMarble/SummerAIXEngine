#include "GoalMoveToPosition.h"
#include "GoalSeekToPosition.h"

GoalMoveToPosition::GoalMoveToPosition(Raven& agent, const X::Math::Vector2& destination)
    : GoalComposite(agent), mDestination(destination)
{

}

void GoalMoveToPosition::Activate()
{
    mStatus = AI::Goal<Raven>::Status::Active;
    RemoveAllSubgoals();

    // last thing to I need to do
    // to first thing I need to do

    // if we have path
    //int pathLength = path.size();
    //for (int i = 0; i < pathLength; ++path)
    //{
    //    if (i == pathLength - 1)
    //    {
    //        AddSubgoal(new GoalStopAtPosition(mAgent, path[i].position));
    //    }
    //    else
    //    {
    //        AddSubgoal(new GoalSeekToPosition(mAgent, path[i].position));
    //    }
    //}

    if (X::Math::DistanceSqr(mAgent.position, mDestination) > 1000.0f)
    {
        AddSubgoal(new GoalSeekToPosition(mAgent, mDestination));
    }
}

AI::Goal<Raven>::Status GoalMoveToPosition::Process()
{
    ActivateIfInactive();
    // mStatus = ProcessSubgoals();
    ReactivateIfFailed();
    return mStatus;
}

void GoalMoveToPosition::Terminate()
{
    RemoveAllSubgoals();
}