#pragma once
#include "SteeringBehavior.h"

namespace AI
{
    class PursuitBehavior : public SteeringBehavior
    {
    public:
        X::Math::Vector2 Calculate(Agent& agent, float deltaTime);
    };

    class EvadeBehavior : public SteeringBehavior
    {
    public:
        X::Math::Vector2 Calculate(Agent& agent,float deltaTime);

        float panicDistance = 100.0f;
    };
}