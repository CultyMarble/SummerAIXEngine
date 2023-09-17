#pragma once
#include <AI.h>
#include "Miner.h"

class GoHomeAndSleepUntilRestedState : public AI::State<Miner>
{
public:
    void Enter(Miner& agent) override
    {
        agent.SetLocation(Miner::LocationType::Home);
    }

    void Update(Miner& agent, float deltaTime) override
    {
        agent.ResetRest();

        if (agent.IsWealthy() == false)
        {
            agent.ChangeState(MinerStates::EnterMineAndDigForNugget);
        }

        // Stay Home Forever!!!
    }

    void Exit(Miner& agent) override
    {
        // Do nothing
    }
};

class EnterMineAndDigForNuggetState : public AI::State<Miner>
{
public:
    void Enter(Miner& agent) override
    {
        agent.SetLocation(Miner::LocationType::Mine);
    }

    void Update(Miner& agent, float deltaTime) override
    {
        agent.IncreaseFatique();
        agent.AddGoldCarried(1);

        if (agent.IsPocketFull())
        {
            agent.ChangeState(MinerStates::VisitBankAndDepositGold);
        }
        else if (agent.IsThirsty())
        {
            agent.ChangeState(MinerStates::QuenchThirst);
        }
    }

    void Exit(Miner& agent) override
    {
        // Do nothing
    }
};

class QuenchThirstState : public AI::State<Miner>
{
public:
    void Enter(Miner& agent) override
    {
        agent.SetLocation(Miner::LocationType::Saloon);
    }

    void Update(Miner& agent, float deltaTime) override
    {
        agent.ResetFatigue();

        if (agent.IsThirsty() == false)
        {
            agent.ChangeState(MinerStates::EnterMineAndDigForNugget);
        }
    }

    void Exit(Miner& agent) override
    {
        // Do nothing
    }
};

class VisitBankAndDepositGoldState : public AI::State<Miner>
{
public:
    void Enter(Miner& agent) override
    {
        agent.SetLocation(Miner::LocationType::Bank);
    }

    void Update(Miner& agent, float deltaTime) override
    {
        agent.AddGoldToBank();

        if (agent.IsWealthy())
        {
            agent.ChangeState(MinerStates::GoHomeAndSleepUntilRested);
        }
        else
        {
            agent.ChangeState(MinerStates::EnterMineAndDigForNugget);
        }
    }

    void Exit(Miner& agent) override
    {
        // Do nothing
    }
};