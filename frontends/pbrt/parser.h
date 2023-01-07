#ifndef _FRONTENDS_PBRT_PARSER_
#define _FRONTENDS_PBRT_PARSER_

#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "frontends/pbrt/material_manager.h"
#include "frontends/pbrt/matrix_manager.h"
#include "frontends/pbrt/renderable.h"
#include "frontends/pbrt/spectrum_manager.h"
#include "frontends/pbrt/texture_manager.h"
#include "frontends/pbrt/tokenizer.h"
#include "iris/camera.h"
#include "iris/image_sampler.h"
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

  bool AreaLightSource();
  bool AttributeBegin();
  bool AttributeEnd();
  bool Camera();
  bool Film();
  bool Include();
  bool Integrator();
  bool MakeNamedMaterial();
  bool Material();
  bool PixelFilter();
  bool Sampler();
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

  struct AttributeEntry {
    std::shared_ptr<
        ObjectBuilder<iris::ReferenceCounted<iris::Material>, TextureManager&>>
        material;
    std::pair<iris::ReferenceCounted<iris::EmissiveMaterial>,
              iris::ReferenceCounted<iris::EmissiveMaterial>>
        emissive_material;
  };

  std::vector<AttributeEntry> attributes_;
  std::unique_ptr<SpectrumManager> spectrum_manager_;
  std::unique_ptr<MaterialManager> material_manager_;
  std::unique_ptr<MatrixManager> matrix_manager_;
  std::unique_ptr<TextureManager> texture_manager_;

  std::function<std::unique_ptr<iris::Camera>(const std::pair<size_t, size_t>&)>
      camera_;
  std::pair<size_t, size_t> image_dimensions_;
  std::unique_ptr<iris::ImageSampler> image_sampler_;
  std::unique_ptr<iris::Integrator> integrator_;
  std::unique_ptr<iris::LightScene::Builder> light_scene_builder_;
  std::function<void(Framebuffer&, std::ofstream&)> write_function_;

  bool camera_encountered_ = false;
  bool film_encountered_ = false;
  bool sampler_encountered_ = false;
  bool integrator_encountered_ = false;
  bool pixel_filter_encountered_ = false;
  bool world_begin_encountered_ = false;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_PARSER_