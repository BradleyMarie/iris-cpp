#ifndef _IRIS_FRAMEBUFFER_
#define _IRIS_FRAMEBUFFER_

#include <array>
#include <cassert>
#include <cstddef>
#include <vector>

#include "iris/color.h"
#include "iris/float.h"

namespace iris {

class Framebuffer final {
 public:
  Framebuffer(std::pair<size_t, size_t> dimensions)
      : pixels_(dimensions.first,
                std::vector<std::pair<std::array<visual_t, 3>, Color::Space>>(
                    dimensions.second, {{0.0, 0.0, 0.0}, Color::CIE_XYZ})) {
    assert(dimensions.first != 0);
    assert(dimensions.second != 0);
  }

  Color Get(size_t y, size_t x) const {
    const auto& entry = pixels_.at(y).at(x);
    return Color(entry.first[0], entry.first[1], entry.first[2], entry.second);
  }

  void Set(size_t y, size_t x, const Color& color) {
    pixels_.at(y).at(x) = std::make_pair(
        std::array<visual_t, 3>({color.x, color.y, color.z}), color.space);
  }

  std::pair<size_t, size_t> Size() const {
    return std::make_pair(pixels_.size(), pixels_.at(0).size());
  }

 private:
  std::vector<std::vector<std::pair<std::array<visual_t, 3>, Color::Space>>>
      pixels_;
};

}  // namespace iris

#endif  // _IRIS_FRAMEBUFFER_