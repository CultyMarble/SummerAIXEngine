#pragma once
#include <AI.h>
#include "TypeID.h"

class VisualSensor : public AI::Sensor
{
public:
    void Update(AI::Agent& agent, AI::MemoryRecords& memory, float deltaTime) override;

    TypeID targetType = TypeID::InvalID;
    float viewRange = 0.0f;
    float viewHalfAngle = 0.0f;
};