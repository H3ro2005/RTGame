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
#include <iostream>
#include <locale>
#include <memory>
#include <vector>

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
        struct Animation
        {
            std::string         name;
            iVec2               dim;
            u16                 delta;
            std::vector<Sprite> frames;
        };

    public:
        std::vector<Animation> anims;
        u32                    currentFrame = 0;
        u16                    currentDelta = 0;
        Animation*             activeAnim   = nullptr;

    public:
        void SetActiveAnim(const std::string_view anim)
        {
            const auto it = std::find_if(anims.begin(), anims.end(),
                                         [anim](const Animation& e) { return e.name == anim; });
            if (it != anims.end())
            {
                activeAnim   = &(*it);
                currentFrame = 0;
                currentDelta = activeAnim->delta;
            }
            else
                throw std::runtime_error("Animation does not exist.");
        }
        void Reset() { currentDelta = activeAnim->delta; }
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

                const auto anims = utils::WSplitBy(wide_content, '@');

                for (const auto& anim : anims)
                {
                    const auto anim_spec = utils::WSplitBy(anim, ':');

                    if (!anim_spec.empty())
                    {
                        // Parse animation parameters.
                        Animation anim;
                        for (const auto arg : utils::WSplitBy(anim_spec[0], ';'))
                        {
                            if (arg.starts_with(L"anim="))
                            {
                                anim.name = std::string{ std::next(std::find(arg.begin(), arg.end(), '"')),
                                                         std::prev(std::find(arg.rbegin(), arg.rend(), '"').base()) };
                            }
                            else if (arg.starts_with(L"dim="))
                            {
                                anim.dim.x = std::stoi(
                                    std::wstring{ std::next(std::find(arg.begin(), arg.end(), '[')),
                                                  std::prev(std::find(arg.rbegin(), arg.rend(), ',').base()) });
                                anim.dim.y = std::stoi(
                                    std::wstring{ std::next(std::find(arg.begin(), arg.end(), ',')),
                                                  std::prev(std::find(arg.rbegin(), arg.rend(), ']').base()) });
                            }
                            else if (arg.starts_with(L"len="))
                            {
                                anim.frames.resize(std::stoi(utils::WSplitBy(arg, '=')[1]));
                            }
                            else if (arg.starts_with(L"delta="))
                            {
                                anim.delta = std::stoi(utils::WSplitBy(arg, '=')[1]);
                            }
                        }

                        // Parse the actual animation based on the parameters provided.
                        i32 x = 0, y = 0, i = 0, j = 0;
                        anim.frames[i].resize(anim.dim.y, std::vector<RChar>(anim.dim.x));
                        for (const auto c : anim_spec[1])
                        {
                            if (c != '\n')
                            {
                                anim.frames[i][y][x] = RChar{ c };

                                if (x == anim.dim.x - 1)
                                {
                                    x = 0;
                                    ++y;
                                }
                                if (y == anim.dim.y)
                                {
                                    x = 0;
                                    y = 0;
                                    if (i + 1 < anim.frames.size())
                                        anim.frames[++i].resize(anim.dim.y, std::vector<RChar>(anim.dim.x));
                                    else
                                        break;
                                }
                                else
                                    ++x;
                            }
                        }

                        this->anims.push_back(std::move(anim));
                    }
                }
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
