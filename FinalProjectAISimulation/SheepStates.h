#pragma once
#include <AI.h>

#include "Sheep.h"
#include "VisualSensor.h"

class DayDreamState : public AI::State<Sheep>
{
public:
    void Enter(Sheep& agent) override {}

    void Update(Sheep& agent, float deltaTime) override
    {
        if (agent.mHunger <= 50.0f)
            agent.ChangeState(Sheep::State::WanderToFindCrop);
    }

    void Exit(Sheep& agent) override
    {
        agent.SetFlee(true);
        agent.SetWander(true);
    }
};

class WanderToFindCropState : public AI::State<Sheep>
{
public:
    void Enter(Sheep& agent) override {}

    void Update(Sheep& agent, float deltaTime) override
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

        agent.ChangeState(Sheep::State::SeekAndEatCrop);
    }

    void Exit(Sheep& agent) override
    {
        agent.SetWander(false);
        agent.SetSeek(true);
    }
};

class SeekAndEatCropState : public AI::State<Sheep>
{
public:
    void Enter(Sheep& agent) override {}

    void Update(Sheep& agent, float deltaTime) override
    {
        if (X::Math::Distance(agent.position, agent.destination) <= 1.0f)
        {
            agent.mHunger = 75.0;

            agent.ChangeState(Sheep::State::FindPlaceToDayDream);
        }
    }

    void Exit(Sheep& agent) override
    {
        const float screenWidth = static_cast<float>(X::GetScreenWidth());
        const float screenHeight = static_cast<float>(X::GetScreenHeight());

        agent.destination = X::RandomVector2({ 100.0f, 100.0f }, { screenWidth - 100.0f,screenHeight - 100.0f });
    }
};

class FindPlaceToDayDreamState : public AI::State<Sheep>
{
public:
    void Enter(Sheep& agent) override {}

    void Update(Sheep& agent, float deltaTime) override
    {
        if (agent.mHunger <= 50.0f)
        {
            agent.ChangeState(Sheep::State::WanderToFindCrop);
        }
        else if (X::Math::Distance(agent.position, agent.destination) <= 1.0f)
        {
            agent.ChangeState(Sheep::State::DayDream);
        }
    }

    void Exit(Sheep& agent) override
    {
        agent.destination = X::Math::Vector2::Zero();

        agent.SetSeek(false);
        agent.SetFlee(false);
    }
};