#pragma once
#include <XEngine.h>
#include <AI.h>

class Crop : public AI::Entity
{
public:
    Crop(AI::AIWorld& world);

    void Initialize();
    void Render();

private:
    X::TextureId mTextureId{};
};
