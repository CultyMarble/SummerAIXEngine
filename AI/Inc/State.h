#pragma once

namespace AI
{
    template <class AgentType>
    class State
    {
    public:
        virtual ~State() = default; // So pointer can delete the derived first before go up and delete parent
        virtual void Enter(AgentType& agent) = 0; // & mean pass by reference
        virtual void Update(AgentType& agent, float deltaTime) = 0;
        virtual void Exit(AgentType& agent) = 0;
    };
}