#include "Mineral.h"
#include "TypeIds.h"

Mineral::Mineral(AI::AIWorld& world)
    :AI::Entity(world, TypeID::MineralId)
{

}

void Mineral::Initialize()
{
    mTextureId = X::LoadTexture("minerals.png");
    position = X::RandomVector2({ 500.0f, 100.0f }, { 780.0f, 710.0f });
    position.x += X::Random(0, 1) == 0 ? -300.0f : 300.0f;
}

void Mineral::Render()
{
    X::DrawSprite(mTextureId, position);
}