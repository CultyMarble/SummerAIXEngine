#include "Peon.h"
#include "TypeIds.h"

extern float wanderJitter;
extern float wanderRadius;
extern float wanderDistance;

Peon::Peon(AI::AIWorld& world)
    : Agent(world, Types::PeonId)
{

}

void Peon::Load()
{
    mSteeringModule = std::make_unique<AI::SteeringModule>(*this);

    // Individual
    mArriveBehavior = mSteeringModule->AddBehavior<AI::ArriveBehavior>();
    mFleeBehavior = mSteeringModule->AddBehavior<AI::FleeBehavior>();
    mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
    mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehavior>();

    // mFleeBehavior->SetActive(true);
    mWanderBehavior->SetActive(true);

    // Group
    mAlignmentBehavior = mSteeringModule->AddBehavior<AI::AlignmentBehavior>();
    mCohesionBehavior = mSteeringModule->AddBehavior<AI::CohesionBehavior>();
    mSeparationBehavior = mSteeringModule->AddBehavior<AI::SeparationBehavior>();

    mAlignmentBehavior->SetActive(true);
    mCohesionBehavior->SetActive(true);
    mSeparationBehavior->SetActive(true);

    for (int i = 0; i < mTextureIDs.size(); ++i)
    {
        char name[128];
        sprintf_s(name, "scv_%02i.png", i + 1);
        mTextureIDs[i] = X::LoadTexture(name);
    }

    float spriteWidth = X::GetSpriteWidth(mTextureIDs[0]);
    radius = (spriteWidth * 0.5f) + 10.0f;
}

void Peon::Unload()
{

}

void Peon::Update(float deltaTime)
{
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
}

void Peon::Render()
{
    const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
    const float percent = angle / X::Math::kTwoPi;
    const int frame = static_cast<int>(percent * mTextureIDs.size()) % mTextureIDs.size();

    X::DrawSprite(mTextureIDs[frame], position);
}

void Peon::ShowDebug(bool debug)
{
    mArriveBehavior->ShowDebug(debug);
    mFleeBehavior->ShowDebug(debug);
    mSeekBehavior->ShowDebug(debug);
    mWanderBehavior->ShowDebug(debug);

    mAlignmentBehavior->ShowDebug(debug);
    mCohesionBehavior->ShowDebug(debug);
    mSeparationBehavior->ShowDebug(debug);
}