#pragma once
#include <AI.h>

class VisualSensor;

class Sheep : public AI::Agent
{
public:
    enum State
    {
        DayDream,
        WanderToFindCrop,
        SeekAndEatCrop,
        FindPlaceToDayDream,
    };

    Sheep(AI::AIWorld& world);

    void Load();
    void Unload();

    void Update(float deltaTime);
    void Render();

    void ChangeState(State newState);

    void ShowDebug(bool debug);

    void SetFlee(bool active) { mFleeBehavior->SetActive(active); }
    void SetSeek(bool active) { mSeekBehavior->SetActive(active); }
    void SetWander(bool active) { mWanderBehavior->SetActive(active); }

    float mHunger = {};

    // Perception Module
    std::unique_ptr<AI::PerceptionModule> mPerceptionModule{};


private:
    // State Machine
    AI::StateMachine<Sheep>* mStateMachine = nullptr;

    // Steering Module
    std::unique_ptr<AI::SteeringModule> mSteeringModule{};

    AI::FleeBehavior* mFleeBehavior = nullptr;
    AI::SeekBehavior* mSeekBehavior = nullptr;
    AI::WanderBehavior* mWanderBehavior = nullptr;

    AI::AlignmentBehavior* mAlignmentBehavior = nullptr;
    AI::CohesionBehavior* mCohesionBehavior = nullptr;
    AI::SeparationBehavior* mSeparationBehavior = nullptr;

    VisualSensor* mVisualSensor = nullptr;

    std::array<X::TextureId, 4> mTextureIDs{};
};