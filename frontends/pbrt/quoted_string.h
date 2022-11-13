#ifndef _FRONTENDS_PBRT_QUOTED_STRING_
#define _FRONTENDS_PBRT_QUOTED_STRING_

#include <optional>
#include <string_view>

namespace iris::pbrt_frontend {

std::optional<std::string_view> Unquote(std::string_view quoted_string);

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_QUOTED_STRING_