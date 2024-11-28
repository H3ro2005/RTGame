#pragma once

#include <Core/CommonDef.h>
#include <string>
#include <string_View>
#include <vector>
#include <sstream>

namespace rt::utils {
    [[nodiscard]] std::vector<std::wstring> WSplitBy(const std::wstring& str, const wchar_t dl) noexcept;
} // namespace rt::utils
