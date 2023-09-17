#include <XEngine.h> // <> for external includes, "" for internal includes
#include <ImGui/Inc/imgui.h>
#include "AI.h"

#include "TypeID.h"

#include "Sheep.h"
#include "Wolf.h"
#include "Crop.h"

//===========================================================================
AI::AIWorld aiWorld;

Wolf testingWolf(aiWorld);

std::vector<std::unique_ptr<Sheep>> sheeps;
std::vector<std::unique_ptr<Crop>> crops;

bool showDebug = true;

float wanderJitter = 7.5f;
float wanderRadius = 25.0f;
float wanderDistance = 50.0f;

float viewRange = 200;
float viewAngle = 45.0f;

AI::ArriveBehavior::Deceleration deceleration = AI::ArriveBehavior::Deceleration::Normal;

int activeBehavior = 0;

void SpawnSheep()
{
    auto& sheep = sheeps.emplace_back(std::make_unique<Sheep>(aiWorld));
    sheep->Load();
    sheep->ShowDebug(showDebug);

    const float screenWidth = static_cast<float>(X::GetScreenWidth());
    const float screenHeight = static_cast<float>(X::GetScreenHeight());
    sheep->position = X::RandomVector2({ 100.0f, 100.0f }, { screenWidth - 100.0f,screenHeight - 100.0f });
}

void SpawnWolf()
{
    testingWolf.Load();
    testingWolf.ShowDebug(showDebug);

    const float screenWidth = static_cast<float>(X::GetScreenWidth());
    const float screenHeight = static_cast<float>(X::GetScreenHeight());
    testingWolf.position = X::RandomVector2({ 100.0f, 100.0f }, { screenWidth - 100.0f,screenHeight - 100.0f });
}

void KillSheep()
{
    if (sheeps.size() == 0)
        return;

    auto& sheep = sheeps.back();
    sheep->Unload();
    sheeps.pop_back();
}

void GameInit()
{
    aiWorld.Initialize();

    SpawnWolf();

    for (int i = 0; i < 3; ++i)
        SpawnSheep();

    for (int i = 0; i < 15; ++i)
    {
        auto& crop = crops.emplace_back(std::make_unique<Crop>(aiWorld));
        crop->Initialize();
    }
}

bool GameLoop(float deltaTime)
{
    ImGui::Begin("Simulation Controller", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Checkbox("ShowDebug", &showDebug))
    {
        for (auto& sheep : sheeps)
            sheep->ShowDebug(showDebug);
    }
    ImGui::Text("Sheep Behaviour:");
    ImGui::Text("Sheep will Daydream until hunger drop below 50");
    ImGui::Text("Then it will wander to look for crop");
    ImGui::Text("If it finds a crop it will seek to its position");
    ImGui::Text("After eating the crop, sheep will wander to find for a location to DayDream");
    ImGui::Text("The sheep always evade the wolf");

    ImGui::End();

    //==================================================
    // UPDATE LOGIC

    aiWorld.Update(); // Do nothing

    for (auto& sheep : sheeps)
    {
        auto neighbors = aiWorld.GetEntitiesInRange({ sheep->position, 100.0f }, TypeID::SheepID);
        sheep->neighbors.clear();

        for (auto& n : neighbors)
        {
            if (n != sheep.get())
            {
                sheep->neighbors.push_back(static_cast<AI::Agent*>(n));
            }
        }
    }

    for (auto& sheep : sheeps)
    {
        sheep->Update(deltaTime);
        sheep->fleeTarget = testingWolf.position;
    }

    testingWolf.Update(deltaTime);

    // Set Predator

    //==================================================
    // DRAW

    for (auto& crop : crops)
        crop->Render();

    for (auto& sheep : sheeps)
        sheep->Render();

    testingWolf.Render();

    const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
    return quit;
}

void GameCleanup()
{
    for (auto& sheep : sheeps)
        sheep->Unload();

    testingWolf.Unload();
}

//===========================================================================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    X::Start("Final Project");
    GameInit();

    X::Run(GameLoop);

    GameCleanup();
    X::Stop();
    return 0;
}