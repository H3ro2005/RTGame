#pragma once

#include "CommonDef.h"

namespace rt::engine {
    class Input
    {
    public:
        enum class Key
        {
            W,
            A,
            S,
            D,
            Space,
            Enter,
            Esc,
            Num1,
            Num2,
            Num3,
            Num4,
            Num5,
            Num6,
            Num7,
            Num8,
            Num9,
            Num0,
            Up,
            Down,
            Left,
            Right
        };

    public:
        [[nodiscard]] static bool IsKeyDown(const Key key) noexcept;
    };
} // namespace rt::engine
