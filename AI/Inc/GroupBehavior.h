#pragma once
#include "SteeringBehavior.h"

namespace AI
{
    class Agent;

    class SeparationBehavior : public SteeringBehavior
    {
    public:
        X::Math::Vector2 Calculate(Agent& agent) override;
    };

    class AlignmentBehavior : public SteeringBehavior
    {
    public:
        X::Math::Vector2 Calculate(Agent& agent) override;
    };

    class CohesionBehavior : public SteeringBehavior
    {
    public:
        X::Math::Vector2 Calculate(Agent& agent) override;
    };
}