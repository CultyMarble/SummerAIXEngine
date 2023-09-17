#include "Precompiled.h"
#include "PerceptionModule.h"

using namespace AI;

PerceptionModule::PerceptionModule(Agent& agent, ImportanceCalculator calculator)
    :mAgent(agent), ComputeImportance(calculator)
{

}

void PerceptionModule::Update(float deltaTime)
{
    // Update all the sensors
    for (auto& sensor : mSensors)
        sensor->Update(mAgent, mMemoryRecords, deltaTime);

    // remove old memory
    mMemoryRecords.erase(
        std::remove_if(
            mMemoryRecords.begin(),
            mMemoryRecords.end(),
            [span = mMemorySpan](const auto& record)
            {
                return record.lastRecordedTime + span < X::GetTime();
            }), mMemoryRecords.end());

    // calculate importance on remaining records
    for (auto& record : mMemoryRecords)
        record.importance = ComputeImportance(mAgent, record);

    // sort records by importance
    mMemoryRecords.sort(
        [](const auto& a, const auto& b)
        {
            return a.importance > b.importance;
        });
}