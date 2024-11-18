#include <iostream>

#include <Behaviour/Behaviour.h>
#include <Core/Input.h>
#include <NetNative.h>
#include <Platform/Windows/WinRend.h>
#include <Renderer/RChar.h>
#include <Renderer/Renderer.h>
#include <Scene/Components.h>
#include <Scene/Entity.h>
#include <Scene/Scene.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <vector>

class GameManager : public rt::engine::Behaviour
{
private:
    rt::engine::Entity m_FaceGuy = rt::engine::Entity::None();

protected:
    void OnInit() override
    {
        using namespace rt;
        using namespace rt::engine;

        // Initilization goes here.
    }
    void OnUpdate(const rt::f32 dt) override
    {
        using namespace rt;
        using namespace rt::engine;

        // Update goes here...
        FaceGuyMovement(dt);
    }
    void FaceGuyMovement(const rt::f32 dt) {}
};

int main()
{
    using namespace rt;
    using namespace rt::engine;
    using namespace std::chrono_literals;
    using clock = std::chrono::high_resolution_clock;

    constexpr auto WIDTH  = 100;
    constexpr auto HEIGHT = 50;

    // TODO: Write an Init method instead of initializating in ctor.
    auto* rend = rt::engine::IRenderer::Get();
    rend->Init(WIDTH, HEIGHT);
    rend->SetClearChar(' ');
    rend->Clear();

    auto scene = rt::engine::Scene{ rend };

    scene.CreateEntity().AddComponent<BehaviourComponent>().Attach<GameManager>();
    scene.OnInit();

    bool is_running = true;

    auto tp1 = clock::now();
    auto tp2 = tp1;
    while (is_running)
    {
        tp2          = clock::now();
        const f32 dt = std::chrono::duration<f32>(tp2 - tp1).count();
        tp1          = tp2;

        rend->Clear();

        scene.OnUpdate(dt);
        rend->RenderFrame();
        std::this_thread::sleep_for(33ms);
    }

    rend->Clear();
    return 0;
}
