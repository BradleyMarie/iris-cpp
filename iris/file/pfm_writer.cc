#include "iris/file/pfm_writer.h"

#include <sstream>

namespace iris {
namespace file {
namespace {

constexpr float ByteOrderMark() {
  union {
    float value;
    char bytes[4];
  } value = {-0.0f};

  return (value.bytes[0] == 1) ? -1.0 : 1.0;
}

}  // namespace

void WritePfm(const Framebuffer& framebuffer, Color::Space color_space,
              std::ostream& output) {
  auto size = framebuffer.Size();

  std::stringstream header_builder;
  header_builder << "PF\n"
                 << size.second << " " << size.first << "\n"
                 << ByteOrderMark() << "\n";
  auto header = header_builder.str();

  output.write(header.c_str(), header.size());

  for (size_t y = 0; y < size.first; y++) {
    for (size_t x = 0; x < size.second; x++) {
      auto color = framebuffer.Get(y, x).ConvertTo(color_space);
      for (size_t index = 0; index < 3; index++) {
        float component = static_cast<float>(color[index]);
        output.write(reinterpret_cast<char*>(&component), 4);
        static_assert(sizeof(float) == 4);
      }
    }
  }
}

}  // namespace file
}  // namespace iris