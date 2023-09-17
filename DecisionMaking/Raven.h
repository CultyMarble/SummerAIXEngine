#pragma once
#include <AI.h>

class Raven : public AI::Agent
{
public:
    Raven(AI::AIWorld& world);

    void Load();
    void Unload();

    void Update(float deltaTime);
    void Render();

    void ShowDebug(bool debug);

    void SetSeek(bool active) { mSeekBehavior->SetActive(active); }
    void SetWander(bool active) { mWanderBehavior->SetActive(active); }

private:
    std::unique_ptr<AI::PerceptionModule> mPerceptionModule{};
    std::unique_ptr<AI::SteeringModule> mSteeringModule{};

    AI::SeekBehavior* mSeekBehavior = nullptr;
    AI::WanderBehavior* mWanderBehavior = nullptr;

    std::array<X::TextureId, 32> mTextureIDs{};
};