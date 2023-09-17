#include "Precompiled.h"
#include "SteeringModule.h"

namespace AI
{
    SteeringModule::SteeringModule(Agent& agent)
        : mAgent(agent)
    {

    }

    X::Math::Vector2 SteeringModule::Calculate()
    {
        X::Math::Vector2 totalForce{};

        for (auto& behavior : mBehaviors)
        {
            if (behavior->IsActive())
            {
                totalForce += behavior->Calculate(mAgent) * behavior->GetWeight();
            }
        }

        return totalForce;
    }
}