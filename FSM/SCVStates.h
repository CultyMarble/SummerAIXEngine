#pragma once
#include <AI.h>
#include "SCV.h"

class GoToCommandCenterAndDepositMineralState : public AI::State<SCV>
{
public:
    void Enter(SCV& agent) override
    {
        agent.SetLocation(SCV::LocationType::CommandCenter);
        std::cout << std::endl;
        std::cout << "EnterState: GoToCommandCenterAndDepositMineral | Location: CommandCenter" << std::endl;
    }

    void Update(SCV& agent, float deltaTime) override
    {
        agent.AddMineralToCommandCenter();
        std::cout << "Update: AddMineralToCommandCenter" << std::endl;
        std::cout << "Update: Current Amount of Mineral: " << agent.GetMineralAmountInCommandCenter() << std::endl;

        if (agent.IsInDanger())
        {
            agent.ChangeState(SCV::States::RunwayFromEnemy);
            std::cout << "ChangeState: RunwayFromEnemy" << std::endl;
            return;
        }

        if (agent.IsEnoughMineralToContructBuilding())
        {
            std::cout << "Checked: Have enought mineral to construct building!" << std::endl;
            agent.ChangeState(SCV::GoToBlueprintAndContructBuilding);
            agent.ReduceMineralToCommandCenter();
        }
        else
        {
            std::cout << "Checked: Dont have enough mineral for building! Need 300!" << std::endl;
            agent.ChangeState(SCV::GoToDepositAndMineForMineral);
        }
    }

    void Exit(SCV& agent) override
    {
        // Do nothing
    }
};

class GoToDepositAndMineForMineralState : public AI::State<SCV>
{
public:
    void Enter(SCV& agent) override
    {
        agent.SetLocation(SCV::LocationType::MineralDeposit);
        std::cout << std::endl;
        std::cout << "EnterState: GoToDepositAndMineForMineral | Location: MineralDeposit" << std::endl;
    }

    void Update(SCV& agent, float deltaTime) override
    {
        agent.AddMineralCarried();
        std::cout << "Update: MineralCarriedAmount: "<< agent.GetMineralCarried() << std::endl;

        if (agent.IsInDanger())
        {
            agent.ChangeState(SCV::States::RunwayFromEnemy);
            std::cout << "ChangeState: RunwayFromEnemy" << std::endl;
            return;
        }

        if (agent.IsHoldingFullAmountOfMineral())
        {
            std::cout << "Checked: Hold full amount of mineral!" << std::endl;
            agent.ChangeState(SCV::States::GoToCommandCenterAndDepositMineral);
        }
    }

    void Exit(SCV& agent) override
    {
        // Do nothing
    }
};

class GoToBlueprintAndContructBuildingState : public AI::State<SCV>
{
public:
    void Enter(SCV& agent) override
    {
        agent.SetLocation(SCV::LocationType::BuildingBlueprint);
        std::cout << std::endl;
        std::cout << "EnterState: GoToBlueprintAndContructBuilding | Location: BuildingBlueprint" << std::endl;
    }

    void Update(SCV& agent, float deltaTime) override
    {
        agent.IncreaseBuildingHealth();
        std::cout << "Update: CurrentBuildingHealth: " << agent.GetBuildingHealth() << "%" << std::endl;

        if (agent.IsInDanger())
        {
            agent.ChangeState(SCV::States::RunwayFromEnemy);
            std::cout << "ChangeState: RunwayFromEnemy" << std::endl;
            return;
        }

        if (agent.IsDoneConstructBuilding())
        {
            std::cout << "Checked: Building done! Go back to mine more mineral!" << std::endl;
            agent.ResetBuildingHealth();

            if (agent.IsHoldingFullAmountOfMineral())
            {
                agent.ChangeState(SCV::States::GoToCommandCenterAndDepositMineral);
            }
            else
            {
                agent.ChangeState(SCV::States::GoToDepositAndMineForMineral);
            }
        }
    }

    void Exit(SCV& agent) override
    {
        // Do nothing
    }
};

class RunwayFromEnemyState : public AI::State<SCV>
{
public:
    void Enter(SCV& agent) override
    {
        agent.SetLocation(SCV::LocationType::RandomPositionAwayFromEnemy);
        std::cout << std::endl;
        std::cout << "EnterState: RunwayFromEnemy | Location: RandomPositionAwayFromEnemy" << std::endl;
    }

    void Update(SCV& agent, float deltaTime) override
    {
        std::cout << "Update: Runway!!!" << std::endl;

        if (agent.IsInDanger())
            return;

        if (agent.GetBuildingHealth() != 0)
        {
            agent.ChangeState(SCV::States::GoToBlueprintAndContructBuilding);
        }
        else if (agent.IsHoldingFullAmountOfMineral())
        {
            agent.ChangeState(SCV::States::GoToCommandCenterAndDepositMineral);
        }
        else
        {
            agent.ChangeState(SCV::States::GoToDepositAndMineForMineral);
        }
    }

    void Exit(SCV& agent) override
    {
        // Do nothing
    }
};