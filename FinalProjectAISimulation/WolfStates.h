#pragma once
#include <AI.h>

#include "Wolf.h"
#include "VisualSensor.h"

class WanderToFindPreyState : public AI::State<Wolf>
{
public:
    void Enter(Wolf& agent) override {}

    void Update(Wolf& agent, float deltaTime) override
    {
        const auto& memoryRecords = agent.mPerceptionModule->GetMemoryRecords();

        if (memoryRecords.empty())
            return;

        for (auto& memory : memoryRecords)
        {
            auto pos = memory.GetProperty<X::Math::Vector2>("lastSeenPosition");
            agent.destination = pos;

            break;
        }

        agent.ChangeState(Wolf::State::SeekAndEat);
    }

    void Exit(Wolf& agent) override
    {
        agent.SetWander(false);
        agent.SetSeek(true);
    }
};

class SeekAndEatState : public AI::State<Wolf>
{
public:
    void Enter(Wolf& agent) override {}

    void Update(Wolf& agent, float deltaTime) override
    {
        if (X::Math::Distance(agent.position, agent.destination) <= 3.0f)
        {
            agent.ChangeState(Wolf::State::WanderToFindPrey);
        }
    }

    void Exit(Wolf& agent) override {}
};