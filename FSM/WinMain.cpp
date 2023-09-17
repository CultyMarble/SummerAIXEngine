#include <XEngine.h> // <> for external includes, "" for internal includes
#include <ImGui/Inc/imgui.h>
#include <AI.h>

#include "SCV.h"

SCV testSCV;

//===========================================================================
void GameInit()
{
    testSCV.Initialize();
}

bool GameLoop(float deltaTime)
{
    ImGui::Begin("FSM");
    ImGui::Text(testSCV.GetLocationTypeString().c_str());
    ImGui::Text(testSCV.GetMineralCarriedString().c_str());
    ImGui::Text(testSCV.GetMineralInCommandCenterString().c_str());
    ImGui::Text(testSCV.GetBuildingHealthString().c_str());
    ImGui::Text(testSCV.GetInDangerStateString().c_str());

    if (ImGui::Button("Update SCV Behavior"))
        testSCV.Update(deltaTime);

    if (ImGui::Button("Toggle Danger Mode"))
        testSCV.ToggleIsInDanger();

    ImGui::Text("Note: After being in danger, scv will continous the task he dropped off!");

    ImGui::End();

    const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
    return quit;
}

void GameCleanup()
{

}

//===========================================================================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    X::Start("FSM");
    GameInit();

    X::Run(GameLoop);

    GameCleanup();
    X::Stop();
    return 0;
}