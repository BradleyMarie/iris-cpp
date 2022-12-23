#include "iris/file/pfm_writer.h"

#include <fstream>
#include <sstream>

#include "googletest/include/gtest/gtest.h"

TEST(PfmWriterTest, Black) {
  iris::Framebuffer framebuffer(std::make_pair(5, 7));

  std::stringstream output;
  iris::file::WritePfm(framebuffer, iris::Color::CIE_XYZ, output);

  std::ifstream reference("iris/file/test_data/black.pfm",
                          std::ofstream::binary);
  std::ostringstream reference_string;
  reference_string << reference.rdbuf();

  EXPECT_EQ(reference_string.str(), output.str());
}

TEST(PfmWriterTest, White) {
  iris::Framebuffer framebuffer(std::make_pair(5, 7));
  for (size_t y = 0; y < 5; y++) {
    for (size_t x = 0; x < 7; x++) {
      iris::Color color(1.0, 1.0, 1.0, iris::Color::CIE_XYZ);
      framebuffer.Set(y, x, color);
    }
  }

  std::stringstream output;
  iris::file::WritePfm(framebuffer, iris::Color::CIE_XYZ, output);

  std::ifstream reference("iris/file/test_data/white.pfm",
                          std::ofstream::binary);
  std::ostringstream reference_string;
  reference_string << reference.rdbuf();

  EXPECT_EQ(reference_string.str(), output.str());
}