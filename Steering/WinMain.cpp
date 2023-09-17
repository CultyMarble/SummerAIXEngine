#include <XEngine.h> // <> for external includes, "" for internal includes
#include <ImGui/Inc/imgui.h>
#include "AI.h"

#include "Peon.h"
#include "TypeIds.h"

//===========================================================================
AI::AIWorld aiWorld;
std::vector<std::unique_ptr<Peon>> peons;

Peon targetPeon(aiWorld);

bool showDebug = true;

float wanderJitter = 7.5f;
float wanderRadius = 30.0f;
float wanderDistance = 50.0f;

AI::ArriveBehavior::Deceleration deceleration = AI::ArriveBehavior::Deceleration::Normal;

int activeBehavior = 0;

void SpawnPeon()
{
    auto& peon = peons.emplace_back(std::make_unique<Peon>(aiWorld));
    peon->Load();
    peon->ShowDebug(showDebug);
    peon->target = &targetPeon;

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

    targetPeon.Load();
    targetPeon.SetArrive(false);
    targetPeon.SetFlee(false);
    targetPeon.SetSeek(false);
    targetPeon.SetWander(true);

    SpawnPeon();
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
        "Flee",
        "Seek",
        "Wander",
        "Arrive",
    };

    if (ImGui::Combo("Active Behavior##", &activeBehavior, behaviors, std::size(behaviors)))
    {
        for (auto& peon : peons)
        {
            peon->SetFlee(activeBehavior == 0);
            peon->SetSeek(activeBehavior == 1);
            peon->SetWander(activeBehavior == 2);
            peon->SetArrive(activeBehavior == 3);
        }
    }

    if (ImGui::CollapsingHeader("Wander##Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Jitter##", &wanderJitter, 0.1f, 0.1f, 10.0f);
        ImGui::DragFloat("Radius##", &wanderRadius, 1.0f, 1.0f, 100.0f);
        ImGui::DragFloat("Distance##", &wanderDistance, 5.0f, 5.0f, 500.0f);
    }

    if (ImGui::CollapsingHeader("Arrive##Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static const char* decelerationSpeeds[] =
        {
            "Fast",
            "Normal",
            "Slow",
        };

        int decel = static_cast<int>(deceleration);
        if (ImGui::Combo("Deceleration##", &decel, decelerationSpeeds, std::size(decelerationSpeeds)))
        {
            deceleration = static_cast<AI::ArriveBehavior::Deceleration>(decel);
        }
    }

    ImGui::End();

    //==================================================
    // UPDATE LOGIC

    if (X::IsMousePressed(X::Mouse::LBUTTON))
    {
        const auto mouseX = static_cast<float>(X::GetMouseScreenX());
        const auto mouseY = static_cast<float>(X::GetMouseScreenY());
        const auto destination = X::Math::Vector2(mouseX, mouseY);

        for (auto& peon : peons)
        {
            peon->destination = destination;
        }
    }


    aiWorld.Update();

    for (auto& peon : peons)
    {
        auto neighbors = aiWorld.GetEntitiesInRange({ peon->position, 100.0f }, TypeID::SheepID);
        peon->neighbors.clear();

        for (auto& n : neighbors)
        {
            if (n != peon.get())
            {
                peon->neighbors.push_back(static_cast<AI::Agent*>(n));
            }
        }
    }

    targetPeon.Update(deltaTime);
    for (auto& peon : peons)
    {
        peon->Update(deltaTime);
    }

    //==================================================
    // DRAW

    targetPeon.Render();
    for (auto& peon : peons)
    {
        peon->Render();
    }

    const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
    return quit;
}

void GameCleanup()
{
    targetPeon.Unload();

    for (auto& peon : peons)
    {
        peon->Unload();
    }
}

//===========================================================================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    X::Start("Steering");
    GameInit();

    X::Run(GameLoop);

    GameCleanup();
    X::Stop();
    return 0;
}