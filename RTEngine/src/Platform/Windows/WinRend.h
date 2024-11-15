#include <Core/CommonDef.h>
#include <Renderer/Renderer.h>

#include <windows.h>

namespace rt::engine::sys {
    class WinRend : public IRenderer
    {
    private:
        CHAR_INFO*                 m_Framebuffer = nullptr;
        HANDLE                     m_Handle      = nullptr;
        CONSOLE_SCREEN_BUFFER_INFO m_BufferInfo{};
        bool m_Initialized = false;

    public:
        WinRend() = default;
        ~WinRend() noexcept override;

    public:
        void                Init(const u16 width, const u16 height) override;
        void                RenderChar(const RChar& ch, const iVec2& pos) override;
        [[nodiscard]] RChar At(const iVec2& pos) override;
        void                RenderFrame() noexcept override;
        void                Clear() noexcept override;
    };
} // namespace rt::engine::sys
