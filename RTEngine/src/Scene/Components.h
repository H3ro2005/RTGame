#pragma once

#include <Behaviour/Behaviour.h>
#include <Core/CommonDef.h>
#include <Renderer/RChar.h>
#include <Scene/Scene.h>

#include "IComponent.h"

#include <codecvt>
#include <filesystem>
#include <fstream>
#include <locale>
#include <memory>
#include <vector>

namespace rt::engine {
    using Sprite      = std::vector<std::vector<RChar>>;

    struct TransformComponent : public IComponent
    {
    public:
        iVec2 pos;
        iVec2 scale;
        f32   rot = 0.0f;
    };

    struct SpriteComponent : public IComponent
    {
    public:
        Sprite sprite;

    public:
        void LoadFromFile(const std::string& path)
        {
            // Open the file
            std::ifstream file(path);
            if (!file.is_open())
            {
                throw std::runtime_error("Failed to open file");
            }

            // Read the entire file content into a string
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

            // Use a UTF-8 to wchar_t converter
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

            // Convert the file content to a wide string
            std::wstring wideContent = converter.from_bytes(content);

            // Create a 2D vector from the wide string
            std::vector<RChar> currentLine;

            for (const wchar_t ch : wideContent)
            {
                if (ch == L'\n')
                {
                    // Push the current line and start a new one
                    sprite.push_back(currentLine);
                    currentLine.clear();
                }
                else
                {
                    // Add character to the current line
                    currentLine.emplace_back(ch, RChar::Attrib::Default);
                }
            }

            // Add the last line if it wasn't followed by a newline
            if (!currentLine.empty())
            {
                sprite.push_back(currentLine);
            }
        }
    };

    struct SpriteAnimationComponent : public IComponent
    {
    public:
        std::vector<Sprite> frames;
        u16                 current_frame = 0;
    };

    struct BehaviourComponent : public IComponent
    {
    public:
        std::vector<std::unique_ptr<Behaviour>> behaviours;

    public:
        template <typename T, typename... TArgs>
        T& Attach(TArgs&&... args) noexcept
        {
            behaviours.push_back(std::make_unique<T>());
            auto* ptr = behaviours.back().get();

            ptr->m_Parent = *m_Parent;
            return *reinterpret_cast<T*>(ptr);
        }
    };
} // namespace rt::engine
