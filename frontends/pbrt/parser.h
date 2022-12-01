#ifndef _FRONTENDS_PBRT_PARSER_
#define _FRONTENDS_PBRT_PARSER_

#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "frontends/pbrt/matrix_manager.h"
#include "frontends/pbrt/renderable.h"
#include "frontends/pbrt/spectrum_manager.h"
#include "frontends/pbrt/texture_manager.h"
#include "frontends/pbrt/tokenizer.h"
#include "iris/integrator.h"
#include "iris/light_scene.h"

namespace iris::pbrt_frontend {

class Parser {
 public:
  Parser(std::unique_ptr<SpectrumManager> spectrum_manager)
      : spectrum_manager_(std::move(spectrum_manager)) {
    spectrum_manager_->Clear();
  }

  std::optional<Renderable> ParseFrom(
      const std::filesystem::path& search_root, Tokenizer& tokenizer,
      std::optional<std::filesystem::path> file_path = std::nullopt);

 private:
  std::optional<std::string_view> PeekToken();
  std::string_view NextToken();

  bool Include();
  bool Integrator();
  bool PixelFilter();
  bool WorldBegin();
  bool WorldEnd();

  struct TokenizerEntry {
    Tokenizer* tokenizer;
    std::unique_ptr<Tokenizer> owned_tokenizer;
    std::filesystem::path search_path;
    std::optional<std::filesystem::path> file_path;
    std::unique_ptr<std::ifstream> file;
  };

  std::vector<TokenizerEntry> tokenizers_;
  std::unique_ptr<SpectrumManager> spectrum_manager_;
  std::unique_ptr<MatrixManager> matrix_manager_;
  std::unique_ptr<TextureManager> texture_manager_;

  std::unique_ptr<iris::Integrator> integrator_;
  std::unique_ptr<iris::LightScene::Builder> light_scene_builder_;

  bool integrator_encountered_ = false;
  bool pixel_filter_encountered_ = false;
  bool world_begin_encountered_ = false;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_PARSER_