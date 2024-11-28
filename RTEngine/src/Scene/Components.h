#pragma once

#include <Behaviour/Behaviour.h>
#include <Core/CommonDef.h>
#include <Renderer/RChar.h>
#include <Scene/Scene.h>
#include <Utils/Utils.h>

#include "IComponent.h"

#include <codecvt>
#include <filesystem>
#include <fstream>
#include <locale>
#include <memory>
#include <vector>
#include <iostream>

namespace rt::engine {
    using Sprite = std::vector<std::vector<RChar>>;

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
        u16    width;
        u16    height;

    public:
        void LoadFromFile(const std::string& path)
        {
            // Open the file
            std::ifstream file{ path, std::ios::binary };
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
                    sprite.push_back(std::move(currentLine));
                    currentLine.clear();
                }
                else
                {
                    // Add character to the current line
                    currentLine.emplace_back(ch);
                }
            }

            // Add the last line if it wasn't followed by a newline
            if (!currentLine.empty())
            {
                sprite.push_back(std::move(currentLine));
            }
        }
    };

    struct SpriteAnimationComponent : public IComponent
    {
    public:
        std::vector<Sprite> frames;
        u16                 current_frame = 0;

    public:
        void LoadFromFile(const std::string& path)
        {
            std::ifstream fs{ path };
            if (fs.is_open())
            {
                std::string content((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

                // Use a UTF-8 to wchar_t converter
                std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

                // Convert the file content to a wide string
                std::wstring wide_content = converter.from_bytes(content);

                std::vector<std::wstring> tokens = utils::WSplitBy(wide_content, ' ');

                std::cout << "asd" << std::endl;
            }
        }
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
