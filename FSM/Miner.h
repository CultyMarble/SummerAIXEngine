#pragma once
#include <AI.h>

enum MinerStates
{
    GoHomeAndSleepUntilRested,
    EnterMineAndDigForNugget,
    QuenchThirst,
    VisitBankAndDepositGold,
};

class Miner
{
public:
    enum LocationType
    {
        Home,
        Mine,
        Saloon,
        Bank,
    };

    Miner();
    void Initialize();
    void Terminate();
    void Update(float deltaTime);
    void ChangeState(MinerStates newState);

    // Check States
    bool IsThirsty() const;
    bool IsPocketFull() const;
    bool IsWealthy() const;
    bool IsRested() const;

    // Action Functions
    void SetLocation(LocationType newLocation);
    LocationType GetLocationType() const;

    void IncreaseFatique();
    void AddGoldCarried(int amount);
    void AddGoldToBank();

    void ResetFatigue();
    void ResetRest();

private:
    AI::StateMachine<Miner>* mStateMachine = nullptr;

    LocationType mLocation = {};    // Miner's current location
    int mGoldCarried = {};          // # of nuggets in miner's pocket
    int mMoneyInBank = {};          // Amount of money in bank
    int mThirst = {};               // Higher value = Thirstier miner
    int mFatigue = {};              // Higher value = miner more tired
};