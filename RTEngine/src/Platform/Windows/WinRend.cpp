#include "WinRend.h"

#include <algorithm>
#include <cstring>
#include <exception>
#include <stdexcept>

namespace rt::engine::sys {
    [[nodiscard]] static CHAR_INFO FromRChar(const RChar& info) noexcept
    {
        CHAR_INFO ch{};
        ch.Char.UnicodeChar = info.ch;

        if (info.attrib == RChar::Attrib::Default)
            ch.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        else
        {
            if (info.attrib & RChar::Attrib::BgRed)
                ch.Attributes |= BACKGROUND_RED;
            if (info.attrib & RChar::Attrib::BgBlue)
                ch.Attributes |= BACKGROUND_BLUE;
            if (info.attrib & RChar::Attrib::BgGreen)
                ch.Attributes |= BACKGROUND_GREEN;
            if (info.attrib & RChar::Attrib::FgRed)
                ch.Attributes |= FOREGROUND_RED;
            if (info.attrib & RChar::Attrib::FgBlue)
                ch.Attributes |= FOREGROUND_BLUE;
            if (info.attrib & RChar::Attrib::FgGreen)
                ch.Attributes |= FOREGROUND_GREEN;
        }

        return ch;
    }
    [[nodiscard]] static inline RChar FromNativeChar(const CHAR_INFO& info) noexcept
    {
        RChar ch;
        ch.ch = info.Char.UnicodeChar;

        if (info.Attributes & BACKGROUND_RED)
            ch.attrib |= RChar::Attrib::BgRed;
        if (info.Attributes & BACKGROUND_BLUE)
            ch.attrib |= RChar::Attrib::BgBlue;
        if (info.Attributes & BACKGROUND_GREEN)
            ch.attrib |= RChar::Attrib::BgGreen;
        if (info.Attributes & FOREGROUND_RED)
            ch.attrib |= RChar::Attrib::FgRed;
        if (info.Attributes & FOREGROUND_BLUE)
            ch.attrib |= RChar::Attrib::FgBlue;
        if (info.Attributes & FOREGROUND_GREEN)
            ch.attrib |= RChar::Attrib::FgGreen;

        return ch;
    }

    [[nodiscard]] static constexpr RChar DefaultRChar() noexcept
    {
        return RChar{ ' ', RChar::Attrib::Default };
    }

    void WinRend::Init(const u16 width, const u16 height)
    {
        m_Width  = width;
        m_Height = height;

        m_Framebuffer = new CHAR_INFO[GetWidth() * GetHeight()]{};

        m_Handle = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(m_Handle, &m_BufferInfo);

        for (auto i = 0; i < GetWidth() * GetHeight(); ++i)
        {
            m_Framebuffer[i] = FromRChar(GetClearChar());
        }

        m_Initialized = true;
    }

    void WinRend::RenderChar(const RChar& ch, const iVec2& pos)
    {
        if (!m_Initialized)
            throw std::runtime_error("Renderer not initialized. Initialize it first.");
        if (pos.x >= 0 && pos.x < GetWidth() && pos.y >= 0 && pos.y < GetHeight())
        {
            m_Framebuffer[pos.y * GetWidth() + pos.x] = FromRChar(ch);
        }
        else
        {
            throw std::invalid_argument("Out of bounds.");
        }
    }

    [[nodiscard]] RChar WinRend::At(const iVec2& pos)
    {
        if (!m_Initialized)
            throw std::runtime_error("Renderer not initialized. Initialize it first.");
        if (pos.x >= 0 && pos.x < GetWidth() && pos.y >= 0 && pos.y < GetHeight())
        {
            return FromNativeChar(m_Framebuffer[pos.y * GetWidth() + pos.x]);
        }
        else
        {
            throw std::invalid_argument("Out of bounds.");
        }
    }

    void WinRend::RenderFrame() noexcept
    {
        if (!m_Initialized)
            return;

        auto write_region = SMALL_RECT{ 0, 0, static_cast<i16>(GetWidth() - 1), static_cast<i16>(GetHeight() - 1) };
        WriteConsoleOutput(m_Handle, m_Framebuffer,
                           COORD{ static_cast<i16>(GetWidth()), static_cast<i16>(GetHeight()) }, COORD{ 0, 0 },
                           &write_region);
    }

    void WinRend::Clear() noexcept
    {
        if (!m_Initialized)
            return;
        std::fill(m_Framebuffer, m_Framebuffer + (GetWidth() * GetHeight()), FromRChar(GetClearChar()));
    }

    WinRend::~WinRend() noexcept
    {
        if (m_Initialized)
            delete[] m_Framebuffer;
    }
} // namespace rt::engine::sys
