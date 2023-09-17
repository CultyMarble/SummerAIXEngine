#pragma once
#include <AI.h>

class VisualSensor;

class Wolf : public AI::Agent
{
public:
    enum State
    {
        WanderToFindPrey,
        SeekAndEat,
    };

    Wolf(AI::AIWorld& world);

    void Load();
    void Unload();

    void Update(float deltaTime);
    void Render();

    void ChangeState(State newState);

    void ShowDebug(bool debug);

    void SetSeek(bool active) { mSeekBehavior->SetActive(active); }
    void SetWander(bool active) { mWanderBehavior->SetActive(active); }

    // Perception Module
    std::unique_ptr<AI::PerceptionModule> mPerceptionModule{};

private:
    // State Machine
    AI::StateMachine<Wolf>* mStateMachine = nullptr;

    // Steering Module
    std::unique_ptr<AI::SteeringModule> mSteeringModule{};

    AI::SeekBehavior* mSeekBehavior = nullptr;
    AI::WanderBehavior* mWanderBehavior = nullptr;

    AI::SeparationBehavior* mSeparationBehavior = nullptr;

    VisualSensor* mVisualSensor = nullptr;

    std::array<X::TextureId, 4> mTextureIDs{};
};