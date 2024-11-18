#pragma once

#include <Core/CommonDef.h>
#include <Renderer/Renderer.h>

#include <array>
#include <random>
#include <string_view>
#include <unordered_map>

namespace rt::engine {
    // Forward declarations.
    class Entity;
    class IComponent;

    class Scene
    {
        friend class Entity;

    public:
        static constexpr auto MAX_COMPONENT_COUNT = 100;

    private:
        std::random_device                                                                    m_Rd;
        std::mt19937_64                                                                       m_Gen;
        IRenderer*                                                                            m_Renderer = nullptr;
        std::unordered_map<u32, std::array<std::unique_ptr<IComponent>, MAX_COMPONENT_COUNT>> m_Registry;

    public:
        Scene(IRenderer* renderer);

    public:
        void   OnInit();
        void   OnUpdate(const f32 dt);
        Entity CreateEntity(const std::string_view tag = "default") noexcept;
        void   DeleteEntity(const Entity entity) noexcept;
    };
} // namespace rt::engine
