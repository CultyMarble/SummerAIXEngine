#include "MinerStates.h"

Miner::Miner() {}

void Miner::Initialize()
{
    mLocation = LocationType::Home;
    mGoldCarried = 0;
    mMoneyInBank = 0;
    mThirst = 0;
    mFatigue = 0;

    mStateMachine = new AI::StateMachine<Miner>(*this);

    mStateMachine->AddState<GoHomeAndSleepUntilRestedState>();
    mStateMachine->AddState<EnterMineAndDigForNuggetState>();
    mStateMachine->AddState<QuenchThirstState>();
    mStateMachine->AddState<VisitBankAndDepositGoldState>();

    mStateMachine->ChangeState((int)MinerStates::GoHomeAndSleepUntilRested);
}

void Miner::Terminate()
{
    delete mStateMachine;
    mStateMachine = nullptr;
}

void Miner::Update(float deltaTime)
{
    mStateMachine->Update(deltaTime);
}

void Miner::ChangeState(MinerStates newState)
{
    mStateMachine->ChangeState((int)newState);
}

// Check States
bool Miner::IsThirsty() const
{
    return mThirst >= 5;
}
bool Miner::IsPocketFull() const
{
    return mGoldCarried >= 10;
}
bool Miner::IsWealthy() const
{
    return mMoneyInBank > 100;
}
bool Miner::IsRested() const
{
    return true;
}

// Action Functions
void Miner::SetLocation(LocationType newLocation)
{
    mLocation = newLocation;
}
Miner::LocationType Miner::GetLocationType() const
{
    return mLocation;
}

void Miner::IncreaseFatique()
{
    mThirst++;
}
void Miner::AddGoldCarried(int amount)
{
    mGoldCarried += amount;
}
void Miner::AddGoldToBank()
{
    mMoneyInBank += mGoldCarried;
    mGoldCarried = 0;
}

void Miner::ResetFatigue()
{

}
void Miner::ResetRest()
{

}