#include <Core/Input.h>

#include <conio.h>
#include <windows.h>

namespace rt::engine {
    [[nodiscard]] u8 FromInputKey(const Input::Key key) noexcept
    {
        switch (key)
        {
            using enum Input::Key;

            case W: return 0x57;
            case A: return 0x41;
            case S: return 0x53;
            case D: return 0x44;
            case Space: return VK_SPACE;
            case Enter: return VK_RETURN;
            case Up: return VK_UP;
            case Down: return VK_DOWN;
            case Left: return VK_LEFT;
            case Right: return VK_RIGHT;
            default: return 0;
        }
    }
    [[nodiscard]] bool Input::IsKeyDown(const Key key) noexcept
    {
        return GetAsyncKeyState(FromInputKey(key)) & 0x01;
    }
} // namespace rt::engine
