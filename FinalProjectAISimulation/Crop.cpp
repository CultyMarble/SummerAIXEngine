#include "Crop.h"
#include "TypeID.h"

Crop::Crop(AI::AIWorld& world)
    :AI::Entity(world, TypeID::CropID)
{

}

void Crop::Initialize()
{
    mTextureId = X::LoadTexture("crop_01.png");
    position = X::RandomVector2({ 500.0f, 100.0f }, { 780.0f, 710.0f });
    position.x += X::Random(0, 1) == 0 ? -300.0f : 300.0f;
}

void Crop::Render()
{
    X::DrawSprite(mTextureId, position);
}