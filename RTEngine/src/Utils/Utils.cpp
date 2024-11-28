#include "Utils.h"

namespace rt::utils {
    [[nodiscard]] std::vector<std::wstring> WSplitBy(const std::wstring& str, const wchar_t dl) noexcept
    {
        std::vector<std::wstring> split;
        std::wstringstream        ss{ str }; 
        std::wstring              token;

        while (std::getline(ss, token, dl)) {
            split.push_back(std::move(token));
        }

        return split;
    }
} // namespace rt::utils
