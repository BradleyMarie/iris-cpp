#ifndef _FRONTENDS_PBRT_FILM_RESULT_
#define _FRONTENDS_PBRT_FILM_RESULT_

#include <filesystem>
#include <fstream>
#include <functional>
#include <optional>
#include <string>

#include "iris/framebuffer.h"

namespace iris::pbrt_frontend::film {

struct Result {
  std::filesystem::path filename;
  std::pair<size_t, size_t> resolution;
  std::function<void(Framebuffer&, std::ofstream&)> write_function;
  std::array<geometric_t, 4> crop_window;
  geometric_t diagonal;
  std::optional<visual_t> max_sample_luminance;
};

}  // namespace iris::pbrt_frontend::film

#endif  // _FRONTENDS_PBRT_FILM_RESULT_