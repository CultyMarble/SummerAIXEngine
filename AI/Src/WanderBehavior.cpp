#include "Precompiled.h"
#include "WanderBehavior.h"
#include "Agent.h"

namespace AI
{
    X::Math::Vector2 WanderBehavior::Calculate(Agent& agent)
    {
        // Apply random jitter to wander target
        X::Math::Vector2 newWanderTarget = mLocalWanderTarget + (X::RandomUnitCircle() * mWanderJitter);

        // Snap the new position back to the wander circle
        newWanderTarget = X::Math::Normalize(newWanderTarget) * mWanderRadius;
        mLocalWanderTarget = newWanderTarget;

        // Project target in front of agent
        newWanderTarget += X::Math::Vector2(0.0f, mWanderDistance);

        // transform target into world space
        const auto worldTransform = agent.GetWorldTransform();
        const auto worldWanderTarget = X::Math::TransformCoord(newWanderTarget, worldTransform);

        // Check if we are at the target
        const auto agentDist = worldWanderTarget - agent.position;
        const float distToDest = X::Math::Magnitude(agentDist);

        if (distToDest <= 0.01f)
            return X::Math::Vector2::Zero();

        // Seek to WanderTarget
        const auto desiredVelocity = (agentDist / distToDest) * agent.maxSpeed;
        const auto seekForce = desiredVelocity - agent.velocity;

        if (IsDebug())
        {
            const auto wanderCenter = X::Math::TransformCoord({ 0.0f, mWanderDistance }, worldTransform);
            X::DrawScreenCircle(wanderCenter, mWanderRadius, X::Colors::Yellow);
            X::DrawScreenDiamond(worldWanderTarget, 3.0f, X::Colors::Red);
            X::DrawScreenLine(agent.position, worldWanderTarget, X::Colors::Green);
        }

        return seekForce;
    }

    void WanderBehavior::Setup(float radius, float distance, float jitter)
    {
        mWanderRadius = radius;
        mWanderDistance = distance;
        mWanderJitter = jitter;
    }
}