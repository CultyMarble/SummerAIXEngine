#include "TypeID.h"
#include "Sheep.h"
#include "SheepStates.h"

#include "VisualSensor.h"

extern float wanderJitter;
extern float wanderRadius;
extern float wanderDistance;

extern float viewRange;
extern float viewAngle;

namespace
{
    void WrapLocation(Sheep& agent)
    {
        const auto screenWidth = X::GetScreenWidth();
        const auto screenHeight = X::GetScreenHeight();

        if (agent.position.x <= 0.0f)
        {
            agent.position.x += screenWidth;
        }
        if (agent.position.x >= screenWidth)
        {
            agent.position.x -= screenWidth;
        }
        if (agent.position.y <= 0.0f)
        {
            agent.position.y += screenHeight;
        }
        if (agent.position.y >= screenHeight)
        {
            agent.position.y -= screenHeight;
        }
    }

    float ComputeImportance(const AI::Agent& agent, const AI::MemoryRecord& record)
    {
        float distance{};
        float distanceScore{};

        TypeID entityType = static_cast<TypeID>(record.GetProperty<int>("type"));
        switch (entityType)
        {
        case TypeID::InvalID:
            return 0;
            break;
        case TypeID::SheepID:
            distance = X::Math::Distance(agent.position, record.GetProperty<X::Math::Vector2>("lastSeenPosition"));
            distanceScore = std::max(1000.0f - distance, 0.0f);
            return distanceScore;
            break;
        case TypeID::CropID:
            distance = X::Math::Distance(agent.position, record.GetProperty<X::Math::Vector2>("lastSeenPosition"));
            distanceScore = std::max(10000.0f - distance, 0.0f);
            return distanceScore;
            break;
        default:
            break;
        }

        return 0;
    }
}

Sheep::Sheep(AI::AIWorld& world)
    : Agent(world, TypeID::SheepID)
{

}

void Sheep::Load()
{
    mHunger = X::RandomFloat(55.0f, 75.0f);

    // STATE MACHINE
    mStateMachine = new AI::StateMachine<Sheep>(*this);
    mStateMachine->AddState<DayDreamState>();
    mStateMachine->AddState<WanderToFindCropState>();
    mStateMachine->AddState<SeekAndEatCropState>();
    mStateMachine->AddState<FindPlaceToDayDreamState>();
    mStateMachine->ChangeState((int)State::DayDream);

    mSteeringModule = std::make_unique<AI::SteeringModule>(*this);

    // STEERING MODULE
    mFleeBehavior = mSteeringModule->AddBehavior<AI::FleeBehavior>();
    mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
    mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehavior>();

    mFleeBehavior->SetActive(true);
    mSeekBehavior->SetActive(false);
    mWanderBehavior->SetActive(false);

    mAlignmentBehavior = mSteeringModule->AddBehavior<AI::AlignmentBehavior>();
    mCohesionBehavior = mSteeringModule->AddBehavior<AI::CohesionBehavior>();
    mSeparationBehavior = mSteeringModule->AddBehavior<AI::SeparationBehavior>();

    mAlignmentBehavior->SetActive(false);
    mCohesionBehavior->SetActive(false);
    mSeparationBehavior->SetActive(true);

    // PERCEPTION MODULE
    mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, ComputeImportance);
    mPerceptionModule->SetMemorySpan(3.0f);
    mVisualSensor = mPerceptionModule->AddSensor<VisualSensor>();
    mVisualSensor->targetType = TypeID::CropID;

    // LOAD TEXTURE
    for (int i = 0; i < mTextureIDs.size(); ++i)
    {
        char name[128];
        sprintf_s(name, "sheep_%02i.png", i + 1);
        mTextureIDs[i] = X::LoadTexture(name);
    }

    float spriteWidth = X::GetSpriteWidth(mTextureIDs[0]);
    radius = (spriteWidth * 0.5f) + 10.0f;
}

void Sheep::Unload()
{

}

void Sheep::ChangeState(State newState)
{
    mStateMachine->ChangeState((int)newState);
}

void Sheep::Update(float deltaTime)
{
    // UPDATE CONDITION
    mHunger -= deltaTime * 2;

    // STATE MACHINE
    mStateMachine->Update(deltaTime);

    // PERCEPTION MODULE
    mVisualSensor->viewRange = viewRange;
    mVisualSensor->viewHalfAngle = viewAngle * X::Math::kDegToRad;

    mPerceptionModule->Update(deltaTime);

    // STEERING MODULE
    if (mWanderBehavior->IsActive())
        mWanderBehavior->Setup(wanderRadius, wanderDistance, wanderJitter);

    const auto force = mSteeringModule->Calculate();
    const auto acceleration = force / mass;

    velocity += acceleration * deltaTime;

    if (X::Math::MagnitudeSqr(velocity) > 1.0f)
        heading = X::Math::Normalize(velocity);

    position += velocity * deltaTime;

    WrapLocation(*this);

    const auto& memoryRecords = mPerceptionModule->GetMemoryRecords();
    for (auto& memory : memoryRecords)
    {
        auto pos = memory.GetProperty<X::Math::Vector2>("lastSeenPosition");
        X::DrawScreenLine(position, pos, X::Colors::Red); // Point to memory
    }
}

void Sheep::Render()
{
    const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
    const float percent = angle / X::Math::kTwoPi;
    const int frame = static_cast<int>(percent * mTextureIDs.size()) % mTextureIDs.size();

    X::DrawSprite(mTextureIDs[frame], position);

    // DRAW CONDITION
    X::DrawScreenText(std::to_string(mHunger).c_str(), position.x - 6.0f, position.y - 36.0f, 12.0, X::Colors::Yellow);
    X::DrawScreenText("Hunger:", position.x - 16.0f, position.y - 48.0f, 10.0, X::Colors::Yellow);
}

void Sheep::ShowDebug(bool debug)
{
    mFleeBehavior->ShowDebug(debug);
    mSeekBehavior->ShowDebug(debug);
    mWanderBehavior->ShowDebug(debug);

    mAlignmentBehavior->ShowDebug(debug);
    mCohesionBehavior->ShowDebug(debug);
    mSeparationBehavior->ShowDebug(debug);
}