#include "Precompiled.h"
#include "AIWorld.h"

namespace AI
{
    void AIWorld::Initialize()
    {

    }

    void AIWorld::Update()
    {

    }

    void AIWorld::Register(Entity* entity)
    {
        XASSERT(std::find(mEntities.begin(), mEntities.end(), entity) == mEntities.end(), "AIWorld: entity already added!!!");
        mEntities.push_back(entity);
    }

    void AIWorld::Unregister(Entity* entity)
    {
        auto iter = std::find(mEntities.begin(), mEntities.end(), entity);
        if (iter != mEntities.end())
        {
            mEntities.erase(iter);
        }
    }

    void AIWorld::AddObstable(const X::Math::Circle& obstacle)
    {
        mObstacles.push_back(obstacle);
    }

    void AIWorld::AddWall(const X::Math::LineSegment& wall)
    {
        mWalls.push_back(wall);
    }

    bool AIWorld::HasLineOfSight(const X::Math::LineSegment& lineSegment) const
    {
        for (auto& wall : mWalls)
        {
            if (X::Math::Intersect(lineSegment, wall))
            {
                return false;
            }
        }

        for (auto& obstacle : mObstacles)
        {
            if (X::Math::Intersect(lineSegment, obstacle))
            {
                return false;
            }
        }

        return true;
    }

    EntityPtrs AIWorld::GetEntitiesInRange(const X::Math::Circle& range, uint32_t typeID)
    {
        // GO OVER PARTITION GRIDS
        EntityPtrs entities;
        float radiusSqr = range.radius * range.radius;

        for (auto& entity : mEntities)
        {
            if (typeID == 0 || entity->GetTypeID() == typeID)
            {
                auto disVector = entity->position - range.center;
                if (X::Math::MagnitudeSqr(disVector) <= radiusSqr)
                {
                    entities.push_back(entity);
                }
            }
        }

        return entities;
    }
}