#pragma once

#include <Core/CommonDef.h>

namespace rt::engine {
    struct RChar
    {
    public:
        enum Attrib : u16
        {
            Default   = 0,
            FgBlue    = 0x0001,
            FgGreen   = 0x0002,
            FgRed     = 0x0004,
            FgIntense = 0x0008,
            BgBlue    = 0x0010,
            BgGreen   = 0x0020,
            BgRed     = 0x0040,
            BgIntense = 0x0080
        };

    public:
        wchar_t ch;
        u16     attrib;

    public:
        constexpr RChar(const wchar_t ch = ' ', const u16 attrib = Attrib::Default) noexcept
            : ch(ch)
            , attrib(attrib)
        {
        }
    };

    [[nodiscard]] constexpr auto operator==(const RChar& lhv, const RChar& rhv) noexcept
    {
        return lhv.ch == rhv.ch;
    }
} // namespace rt::engine
