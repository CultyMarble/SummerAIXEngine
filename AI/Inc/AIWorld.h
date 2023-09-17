#pragma once
#include "Entity.h"

namespace AI
{
    class AIWorld
    {
    public:
        using Obstacles = std::vector<X::Math::Circle>;
        using Walls = std::vector<X::Math::LineSegment>;

        void Initialize();
        void Update();

        void Register(Entity* entity);
        void Unregister(Entity* entity);

        void AddObstable(const X::Math::Circle& obstacle);
        void AddWall(const X::Math::LineSegment& wall);

        bool HasLineOfSight(const X::Math::LineSegment& lineSegment) const;

        EntityPtrs GetEntities() const { return mEntities; }
        const Obstacles& GetObstacles() const { return mObstacles; }
        const Walls& GetWalls() const { return mWalls; }

        EntityPtrs GetEntitiesInRange(const X::Math::Circle& range, uint32_t typeID);

        uint32_t GetNextId() const
        {
            XASSERT(mNextId < UINT32_MAX, "AIWorld: run out of Ids!!!");
            return mNextId++;
        }

    private:
        mutable uint32_t mNextId = 0;
        EntityPtrs mEntities;

        Obstacles mObstacles;
        Walls mWalls;
    };
}