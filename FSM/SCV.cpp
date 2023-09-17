#include "SCVStates.h"

SCV::SCV() {}

void SCV::Initialize()
{
    mLocation = LocationType::CommandCenter;

    mMineralCarried = 0;
    mIsInDanger = false;

    mMineralInCommandCenter = 0;
    mBuildingHealth = 0;

    mStateMachine = new AI::StateMachine<SCV>(*this);

    mStateMachine->AddState<GoToCommandCenterAndDepositMineralState>();
    mStateMachine->AddState<GoToDepositAndMineForMineralState>();
    mStateMachine->AddState<GoToBlueprintAndContructBuildingState>();
    mStateMachine->AddState<RunwayFromEnemyState>();

    mStateMachine->ChangeState((int)SCV::States::GoToDepositAndMineForMineral);
}

void SCV::Terminate()
{
    delete mStateMachine;
    mStateMachine = nullptr;
}

void SCV::Update(float deltaTime)
{
    mStateMachine->Update(deltaTime);

}

void SCV::ChangeState(States newState)
{
    mStateMachine->ChangeState((int)newState);
}


bool SCV::IsHoldingFullAmountOfMineral() const
{
    return mMineralCarried == 50;
}

bool SCV::IsDoneConstructBuilding() const
{
    return mBuildingHealth == 100;
}

bool SCV::IsInDanger() const
{
    return mIsInDanger;
}

bool SCV::IsEnoughMineralToContructBuilding() const
{
    return mMineralInCommandCenter >= 200;
}


int SCV::GetMineralAmountInCommandCenter() const
{
    return mMineralInCommandCenter;
}

int SCV::GetMineralCarried() const
{
    return mMineralCarried;
}

int SCV::GetBuildingHealth() const
{
    return mBuildingHealth;
}


void SCV::SetLocation(LocationType newLocation)
{
    mLocation = newLocation;
}

SCV::LocationType SCV::GetLocationType() const
{
    return mLocation;
}


void SCV::AddMineralCarried()
{
    mMineralCarried += 10;
}

void SCV::AddMineralToCommandCenter()
{
    mMineralInCommandCenter += mMineralCarried;
    mMineralCarried = 0;
}

void SCV::ReduceMineralToCommandCenter()
{
    mMineralInCommandCenter = 0;
}

void SCV::IncreaseBuildingHealth()
{
    mBuildingHealth += 5;
}

void SCV::ResetBuildingHealth()
{
    mBuildingHealth = 0;
}

void SCV::ToggleIsInDanger()
{
    mIsInDanger = !mIsInDanger;

    if (mIsInDanger)
    {
        std::cout << "I am being attacked! Drop all tasks and start running!" << std::endl;
    }
    else
    {
        std::cout << "I am safe! Gotta get back to work!" << std::endl;
    }
}

std::string SCV::GetLocationTypeString()
{
    switch (mLocation)
    {
    case SCV::CommandCenter:
        return "Current Location: CommandCenter";
        break;
    case SCV::MineralDeposit:
        return "Current Location: MineralDeposit";
        break;
    case SCV::BuildingBlueprint:
        return "Current Location: BuildingBlueprint";
        break;
    case SCV::RandomPositionAwayFromEnemy:
        return "Current Location: RandomPositionAwayFromEnemy";
        break;
    default:
        return "Current Location: Invalid";
        break;
    }
}

std::string SCV::GetMineralCarriedString()
{
    std::string toReturn = "Mineral Carrying: ";
    toReturn += std::to_string(mMineralCarried);

    return toReturn;
}

std::string SCV::GetMineralInCommandCenterString()
{
    std::string toReturn = "Mineral In Command Centre: ";
    toReturn += std::to_string(mMineralInCommandCenter);

    return toReturn;
}

std::string SCV::GetInDangerStateString()
{
    if (IsInDanger())
    {
        return "I am in danger! Cannot work, gotta run away!";
    }

    return "I am safe, I can work!";
}

std::string SCV::GetBuildingHealthString()
{
    std::string toReturn = "Building Health: ";
    toReturn += std::to_string(mBuildingHealth);

    return toReturn;
}