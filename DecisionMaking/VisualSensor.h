#pragma once
#include <AI.h>
#include "TypeIds.h"

class VisualSensor : public AI::Sensor
{
public:
    void Update(AI::Agent& agent, AI::MemoryRecords& memory, float deltaTime) override;

    Types targetType = Types::Invalid;
    float viewRange = 0.0f;
    float viewHalfAngle = 0.0f;
};