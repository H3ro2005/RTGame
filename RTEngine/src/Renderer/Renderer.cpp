#include "Renderer.h"

#ifdef _WIN32
#include <Platform/Windows/WinRend.h>
#endif

namespace rt::engine {
    std::unique_ptr<IRenderer> IRenderer::s_Instance = nullptr;
    IRenderer*        IRenderer::Get() noexcept
    {
        if (!s_Instance)
#ifdef _WIN32
            s_Instance = std::make_unique<sys::WinRend>();
#else defined(__linux__) || defined(__APPLE__)
            s_Instance = std::make_unique<sys::WinRend>();
#endif
        return s_Instance.get();
    }
} // namespace rt::engine