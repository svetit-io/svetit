#pragma once

#include <string_view>

namespace svetit::headers {

inline constexpr std::string_view kUserId{"X-User"};
inline constexpr std::string_view kSessionId{"X-Session"};
inline constexpr std::string_view kSpaceId{"X-Space-Id"};
inline constexpr std::string_view kSpaceRole{"X-Space-Role"};

} // namespace svetit::headers
