#pragma once
#include "AI.h"
#include "Raven.h"

class RavenStrategy : public AI::Strategy<Raven>
{
public:
    float CalculateDesirability(Raven& agent) const override
    {
        return 100.0f;
    }

    //std::unique_ptr<AI::Goal<Raven>> CreateGoal() const override
    //{
    //    return std::make_unique<
    //}

};