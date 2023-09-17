#include <XEngine.h> // <> for external includes, "" for internal includes
#include <ImGui/Inc/imgui.h>
#include "AI.h"

#include "Peon.h"
#include "Mineral.h"
#include "TypeIds.h"

//===========================================================================
AI::AIWorld aiWorld;
std::vector<std::unique_ptr<Peon>> peons;
std::vector<std::unique_ptr<Mineral>> minerals;

bool showDebug = true;

float wanderJitter = 7.5f;
float wanderRadius = 30.0f;
float wanderDistance = 50.0f;

float viewRange = 300.0f;
float viewAngle = 45.0f;

int activeBehavior = 0;

void SpawnPeon()
{
    auto& peon = peons.emplace_back(std::make_unique<Peon>(aiWorld));
    peon->Load();
    peon->ShowDebug(showDebug);

    const float screenWidth = static_cast<float>(X::GetScreenWidth());
    const float screenHeight = static_cast<float>(X::GetScreenHeight());
    peon->position = X::RandomVector2({ 100.0f, 100.0f }, { screenWidth - 100.0f,screenHeight - 100.0f });
}

void KillPeon()
{
    if (peons.size() == 0)
        return;

    auto& peon = peons.back();
    peon->Unload();
    peons.pop_back();
}

void GameInit()
{
    aiWorld.Initialize();

    SpawnPeon();

    for (int i = 0; i < 10; ++i)
    {
        auto& mineral = minerals.emplace_back(std::make_unique<Mineral>(aiWorld));
        mineral->Initialize();
    }

    // Create Obstacles
    aiWorld.AddObstable({ 230.0f, 300.0f, 50.0f });
    X::Math::Vector2 topLeft(500.0f, 100.0f);
    X::Math::Vector2 topRight(600.0f, 100.0f);
    X::Math::Vector2 botLeft(500, 600.0f);
    X::Math::Vector2 botRight(600.0f, 600.0f);

    aiWorld.AddWall({ topLeft, topRight });
    aiWorld.AddWall({ topRight, botRight });
    aiWorld.AddWall({ botLeft, botRight });
    aiWorld.AddWall({ botLeft, topLeft });
}

bool GameLoop(float deltaTime)
{
    ImGui::Begin("Steering Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::Button("Spawn"))
    {
        SpawnPeon();
    }

    if (ImGui::Button("Kill"))
    {
        KillPeon();
    }

    if (ImGui::Checkbox("ShowDebug", &showDebug))
    {
        for (auto& peon : peons)
        {
            peon->ShowDebug(showDebug);
        }
    }

    const char* behaviors[] =
    {
        "Wander",
        "Seek",
    };

    if (ImGui::Combo("Active Behavior##", &activeBehavior, behaviors, std::size(behaviors)))
    {
        for (auto& peon : peons)
        {
            peon->SetWander(activeBehavior == 0);
            peon->SetSeek(activeBehavior == 1);
        }
    }

    if (ImGui::CollapsingHeader("Wander##Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Jitter##", &wanderJitter, 0.1f, 0.1f, 10.0f);
        ImGui::DragFloat("Radius##", &wanderRadius, 1.0f, 1.0f, 100.0f);
        ImGui::DragFloat("Distance##", &wanderDistance, 5.0f, 5.0f, 500.0f);
    }

    if (ImGui::CollapsingHeader("Visual##Sensor", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("View##Range", &viewRange, 1.0f, 100.0f, 1000.0f);
        ImGui::DragFloat("View##Angle", &viewAngle, 1.0f, 10.0f, 180.0f);
    }

    ImGui::End();

    //==================================================
    // UPDATE LOGIC

    aiWorld.Update();

    for (auto& peon : peons)
    {
        peon->Update(deltaTime);
    }

    //==================================================
    // DRAW

    for (auto& mineral : minerals)
        mineral->Render();

    for (auto& peon : peons)
        peon->Render();

    auto& obstacles = aiWorld.GetObstacles();
    for (auto& obstacle : obstacles)
        X::DrawScreenCircle(obstacle.center, obstacle.radius, X::Colors::Gray);

    auto& walls = aiWorld.GetWalls();
    for (auto& wall : walls)
        X::DrawScreenLine(wall.from, wall.to, X::Colors::Gray);

    const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
    return quit;
}

void GameCleanup()
{
    for (auto& peon : peons)
    {
        peon->Unload();
    }
}

//===========================================================================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    X::Start("Perception");
    GameInit();

    X::Run(GameLoop);

    GameCleanup();
    X::Stop();
    return 0;
}