#pragma once
#include "Goal.h"

namespace AI
{
    template<class AgentType>
    class GoalComposite : public Goal<AgentType>
    {
    public:
        using Status = Goal<AgentType>::Status;

        GoalComposite(AgentType& agent)
            : Goal<AgentType>(agent)
        {

        }

        virtual ~GoalComposite()
        {
            RemoveAllSubgoals();
        }

    protected:
        void AddSubgoal(Goal<AgentType>* goal)
        {
            mSubgoals.push_front(goal);
        }

        void RemoveAllSubgoals()
        {
            for (auto& goal : mSubgoals)
            {
                goal->Terminate();
                delete goal;
            }

            mSubgoals.clear();
        }

        Status ProcessSubgoals()
        {
            while (!mSubgoals.empty())
            {
                auto& goal = mSubgoals.front();
                if (goal->mStatus != Status::Completed &&
                    goal->mStatus != Status::Failed)
                {
                    break;
                }

                goal->Terminate();
                mSubgoals.pop_front();
            }

            if (!mSubgoals.empty())
            {
                Status status = mSubgoals.front()->Process();
                if (status == Status::Completed && mSubgoals.size() > 1)
                {
                    return Status::Active;
                }

                return status;
            }

            return Status::Completed;
        }

        using Subgoals = std::list<Goal<AgentType>*>;
        Subgoals mSubgoals;
    };
}