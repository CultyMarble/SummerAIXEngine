#pragma once
#include <AI.h>

class SCV
{
public:
    enum LocationType
    {
        CommandCenter,
        MineralDeposit,
        BuildingBlueprint,
        RandomPositionAwayFromEnemy,
    };

    enum States
    {
        GoToCommandCenterAndDepositMineral,
        GoToDepositAndMineForMineral,
        GoToBlueprintAndContructBuilding,
        RunwayFromEnemy,
    };

    SCV();
    void Initialize();
    void Terminate();
    void Update(float deltaTime);
    void ChangeState(States newState);

    // Check States
    bool IsHoldingFullAmountOfMineral() const;
    bool IsDoneConstructBuilding() const;
    bool IsInDanger() const;
    bool IsEnoughMineralToContructBuilding() const;

    int GetMineralAmountInCommandCenter() const;
    int GetMineralCarried() const;
    int GetBuildingHealth() const;

    // Action Functions
    void SetLocation(LocationType newLocation);
    LocationType GetLocationType() const;

    void AddMineralCarried();

    void AddMineralToCommandCenter();
    void ReduceMineralToCommandCenter();

    void IncreaseBuildingHealth();
    void ResetBuildingHealth();

    void ToggleIsInDanger();

    // Output Text
    std::string GetLocationTypeString();
    std::string GetMineralCarriedString();
    std::string GetMineralInCommandCenterString();
    std::string GetInDangerStateString();
    std::string GetBuildingHealthString();

private:
    AI::StateMachine<SCV>* mStateMachine = nullptr;

    LocationType mLocation = {};        // SCV's current location

    int mMineralCarried = {};           // # of mineral in miner's pocket
    bool mIsInDanger = {};              // In danger = Is being Attacked my enemy

    int mMineralInCommandCenter = {};   // # of mineral in Command Center
    int mBuildingHealth = {};           // % of building health
};