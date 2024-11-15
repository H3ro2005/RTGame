#pragma once

#include <Scene/Entity.h>

namespace rt::engine {
    // Forward declarations.
    class Scene;

    class Behaviour
    {
        friend class Scene;
        friend class BehaviourComponent;

    private:
        Entity m_Parent = Entity{ 0, nullptr };

    public:
        Behaviour() = default;

    protected:
        virtual void OnInit()               = 0;
        virtual void OnUpdate(const f32 dt) = 0;

    protected:
        Entity CreateEntity(const std::string_view tag = "default") { return m_Parent.m_Parent->CreateEntity(tag); }

    protected:
        template <typename T, typename... TArgs>
        inline T& AddComponent(TArgs&&... args)
        {
            return m_Parent.AddComponent<T>(std::forward<TArgs>(args)...);
        }
        template <typename T>
        inline bool HasComponent() noexcept
        {
            return m_Parent.HasComponent<T>();
        }
        template <typename T>
        inline T& GetComponent()
        {
            return m_Parent.GetComponent<T>();
        }
    };
} // namespace rt::engine
