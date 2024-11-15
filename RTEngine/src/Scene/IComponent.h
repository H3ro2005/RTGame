#pragma once

#include <Core/CommonDef.h>
#include <Renderer/RChar.h>
#include <vector>

namespace rt::engine {
    class Entity;
    
    using ComponentID = u32;

    inline u32 GenComponentID() noexcept
    {
        static u32 id = 0;
        return id++;
    }

    template <typename T>
    inline u32 GetComponentID() noexcept
    {
        static u32 id = GenComponentID();
        return id;
    }

    struct IComponent
    {
        friend class Entity;

    protected:
        Entity* m_Parent = nullptr;

    public:
        constexpr IComponent() = default;
        virtual ~IComponent() noexcept {}
    };
} // namespace rt::engine