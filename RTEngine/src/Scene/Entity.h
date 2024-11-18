#pragma once

#include <Core/CommonDef.h>

#include "IComponent.h"

#include "Scene.h"

namespace rt::engine {
    // Forward declarations.
    class Behaviour;

    struct Entity
    {
        friend class Scene;
        friend class Behaviour;

    private:
        u32    m_UUID   = 0;
        Scene* m_Parent = nullptr;

    private:
        Entity(const u32 uuid, Scene* parent) noexcept
            : m_UUID(uuid)
            , m_Parent(parent)
        {
        }

    public:
        [[nodiscard]] static inline Entity None() noexcept { return Entity{ 0, nullptr }; }

    public:
        template <typename T, typename... TArgs>
        inline T& AddComponent(TArgs&&... args)
        {
            const auto id = GetComponentID<T>();
            if (!HasComponent<T>())
            {
                std::unique_ptr<IComponent> ptr     = std::make_unique<T>(std::forward<TArgs>(args)...);
                ptr->m_Parent                       = this;
                m_Parent->m_Registry.at(m_UUID)[id] = std::move(ptr);
                return *reinterpret_cast<T*>(m_Parent->m_Registry.at(m_UUID)[id].get());
            }
            else
                throw std::runtime_error("Entity already has component.");
        }
        template <typename T>
        inline bool HasComponent() noexcept
        {
            return m_Parent->m_Registry.at(m_UUID)[GetComponentID<T>()] != nullptr;
        }
        template <typename T>
        inline T& GetComponent()
        {
            return *reinterpret_cast<T*>(m_Parent->m_Registry.at(m_UUID).at(GetComponentID<T>()).get());
        }

    public:
        [[nodiscard]] inline      operator bool() const noexcept { return m_UUID != 0 && m_Parent; }
        [[nodiscard]] inline bool operator==(const Entity& other) const noexcept
        {
            return m_UUID == other.m_UUID && m_Parent == other.m_Parent;
        }
    };
} // namespace rt::engine
