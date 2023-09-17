#pragma once

#include "Goal.h"
#include "GoalComposite.h"
#include "Strategy.h"

namespace AI
{
    template<class AgentType>
    class DecisionModule
    {
    public:
        using GoalType = Goal<AgentType>;
        using StrategyType = Strategy<AgentType>;

        DecisionModule(AgentType& agent)
            : mAgent(agent)
        {

        }

        template<class NewStrategyType>
        void AddStrategy()
        {
            static_assert(std::is_base_of_v<StrategyType, NewStrategyType>, "DecisionModule: must be of 'StrategyType'");
            mStrategies.push_back(std::make_unique<StrategyType>());
        }

        void Update()
        {
            // What is the best goal at this time
            Arbitrate();

            // Process the goal
            if (mCurrentGoal != nullptr)
            {
                mCurrentGoal->Process();
            }

        }

    private:
        void Arbitrate()
        {
            StrategyType* bestStrategy = nullptr;
            float mostDesirable = 0.0f;

            for (auto& strategy : mStrategies)
            {
                float desirability = strategy->CalculateDesirability();
                if (desirability > mostDesirable)
                {
                    mostDesirable = desirability;
                    bestStrategy = strategy;
                }
            }

            if (mCurrentStrategy != bestStrategy)
            {
                mCurrentStrategy = bestStrategy;

                if (mCurrentGoal != nullptr)
                {
                    mCurrentGoal->Terminate();
                }
                if (mCurrentStrategy != nullptr)
                {
                    mCurrentGoal = mCurrentStrategy->CreateGoal();
                }
            }
        }

        using Strategies = std::vector<std::unique_ptr<StrategyType>>;

        Strategies mStrategies;
        StrategyType* mCurrentStrategy = nullptr;

        std::unique_ptr<GoalType> mCurrentGoal = nullptr;

        AgentType& mAgent;
    };
}