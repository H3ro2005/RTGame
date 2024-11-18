#include "Scene.h"

#include "Components.h"
#include "Entity.h"

#include <Behaviour/Behaviour.h>

#include <iostream>
#include <limits>

namespace rt::engine {
    Scene::Scene(IRenderer* renderer)
        : m_Renderer(renderer)
        , m_Rd()
        , m_Gen(m_Rd())
    {
    }

    void Scene::OnInit()
    {
        for (auto& [x, y] : m_Registry)
        {
            auto entity = Entity{ x, this };
            if (entity.HasComponent<BehaviourComponent>())
            {
                auto& bc = entity.GetComponent<BehaviourComponent>();
                for (auto& e : bc.behaviours)
                {
                    e->OnInit();
                }
            }
        }
    }

    void Scene::OnUpdate(const f32 dt)
    {
        for (auto& [x, y] : m_Registry)
        {
            auto  entity    = Entity{ x, this };
            auto& transform = entity.GetComponent<TransformComponent>();
            if (entity.HasComponent<SpriteComponent>())
            {
                auto& sc = entity.GetComponent<SpriteComponent>();
                for (u16 i = 0; i < sc.sprite.size(); ++i)
                {
                    for (u16 j = 0; j < sc.sprite[i].size(); ++j)
                    {
                        m_Renderer->RenderChar(sc.sprite[i][j], iVec2{ static_cast<u16>(transform.pos.x + j),
                                                                       static_cast<u16>(transform.pos.y + i) });
                    }
                }
            }
            if (entity.HasComponent<BehaviourComponent>())
            {
                auto& bc = entity.GetComponent<BehaviourComponent>();
                for (auto& e : bc.behaviours)
                {
                    e->OnUpdate(dt);
                }
            }
        }
    }

    Entity Scene::CreateEntity(const std::string_view tag) noexcept
    {
        std::uniform_int_distribution<u32> dist{ 0, std::numeric_limits<u32>::max() };
        const auto                         entity = Entity{ dist(m_Gen), this };
        m_Registry[entity.m_UUID];
        m_Registry[entity.m_UUID][GetComponentID<TransformComponent>()] = std::make_unique<TransformComponent>();
        return entity;
    }

    void Scene::DeleteEntity(const Entity entity) noexcept
    {
        if (auto it = m_Registry.find(entity); it != m_Registry.end())
        {
            m_Registry.erase(it);
        }
    }
} // namespace rt::engine
