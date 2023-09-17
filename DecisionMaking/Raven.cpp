#include "Raven.h"
#include "TypeIds.h"

extern float wanderJitter;
extern float wanderRadius;
extern float wanderDistance;

extern float viewRange;
extern float viewAngle;

namespace
{
    float ComputeImportance(const AI::Agent& agent, const AI::MemoryRecord& record)
    {
        float distance{};
        float distanceScore{};

        Types entityType = static_cast<Types>(record.GetProperty<int>("type"));
        switch (entityType)
        {
        case Types::Invalid:
            return 0;
            break;
        case Types::PeonId:
            distance = X::Math::Distance(agent.position, record.GetProperty<X::Math::Vector2>("lastSeenPosition"));
            distanceScore = std::max(1000.0f - distance, 0.0f);
            return distanceScore;
            break;
        case Types::MineralId:
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

Raven::Raven(AI::AIWorld& world)
    : Agent(world, Types::PeonId)
{

}

void Raven::Load()
{
    // Perception
    mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, ComputeImportance);
    mPerceptionModule->SetMemorySpan(3.0f);

    // Steering
    mSteeringModule = std::make_unique<AI::SteeringModule>(*this);

    mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
    mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehavior>();

    mWanderBehavior->SetActive(true);

    for (int i = 0; i < mTextureIDs.size(); ++i)
    {
        char name[128];
        sprintf_s(name, "interceptor_%02i.png", i + 1);
        mTextureIDs[i] = X::LoadTexture(name);
    }

    float spriteWidth = static_cast<float>(X::GetSpriteWidth(mTextureIDs[0]));
    radius = (spriteWidth * 0.5f) + 30.0f;
}

void Raven::Unload()
{

}

void Raven::Update(float deltaTime)
{
    mPerceptionModule->Update(deltaTime);

    if (mWanderBehavior->IsActive())
    {
        mWanderBehavior->Setup(wanderRadius, wanderDistance, wanderJitter);
    }

    const auto force = mSteeringModule->Calculate();
    const auto acceleration = force / mass;

    velocity += acceleration * deltaTime;

    if (X::Math::MagnitudeSqr(velocity) > 1.0f)
    {
        heading = X::Math::Normalize(velocity);
    }

    position += velocity * deltaTime;

    const auto screenWidth = X::GetScreenWidth();
    const auto screenHeight = X::GetScreenHeight();

    if (position.x <= 0.0f)
    {
        position.x += screenWidth;
    }
    if (position.x >= screenWidth)
    {
        position.x -= screenWidth;
    }
    if (position.y <= 0.0f)
    {
        position.y += screenHeight;
    }
    if (position.y >= screenHeight)
    {
        position.y -= screenHeight;
    }

    const auto& memoryRecords = mPerceptionModule->GetMemoryRecords();
    for (auto& memory : memoryRecords)
    {
        auto pos = memory.GetProperty<X::Math::Vector2>("lastSeenPosition");
        X::DrawScreenLine(position, pos, X::Colors::Red);

        std::string score = std::to_string(memory.importance);
        X::DrawScreenText(score.c_str(), pos.x, pos.y, 12.0f, X::Colors::Wheat);
    }
}

void Raven::Render()
{
    const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
    const float percent = angle / X::Math::kTwoPi;
    const int frame = static_cast<int>(percent * mTextureIDs.size()) % mTextureIDs.size();

    X::DrawSprite(mTextureIDs[frame], position);
}

void Raven::ShowDebug(bool debug)
{
    mSeekBehavior->ShowDebug(debug);
    mWanderBehavior->ShowDebug(debug);
}