#include "frontends/pbrt/quoted_string.h"

namespace iris::pbrt_frontend {

std::optional<std::string_view> Unquote(std::string_view quoted_string) {
  if (quoted_string.size() < 2 || quoted_string.front() != '"' ||
      quoted_string.back() != '"') {
    return std::nullopt;
  }

  quoted_string.remove_prefix(1);
  quoted_string.remove_suffix(1);

  return quoted_string;
}

}  // namespace iris::pbrt_frontend