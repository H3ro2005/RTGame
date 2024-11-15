#pragma once

#include <Core/CommonDef.h>
#include <memory>

#include "RChar.h"

namespace rt::engine {
    class IRenderer
    {
    protected:
        u16     m_Width;
        u16     m_Height;
        RChar   m_ClearChar = RChar{ 0, RChar::Attrib::Default };

    private:
        static std::unique_ptr<IRenderer> s_Instance;

    public:
        IRenderer() noexcept = default;
        virtual ~IRenderer() noexcept {};

    public:
        [[nodiscard]] inline auto GetWidth() const noexcept { return m_Width; }
        [[nodiscard]] inline auto GetHeight() const noexcept { return m_Height; }
        inline void               SetClearChar(const RChar newClearChar) noexcept { m_ClearChar = newClearChar; }
        [[nodiscard]] inline RChar GetClearChar() const noexcept { return m_ClearChar; }

    public:
        virtual void                Init(const u16 width, const u16 height)       = 0;
        virtual void                RenderChar(const RChar& ch, const iVec2& pos) = 0;
        [[nodiscard]] virtual RChar At(const iVec2& pos)                          = 0;
        virtual void                RenderFrame() noexcept                        = 0;
        virtual void                Clear() noexcept                        = 0;
    
    public:
        [[nodiscard]] static IRenderer* Get() noexcept;
    };
} // namespace rt::engine
