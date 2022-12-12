#include "frontends/pbrt/parser.h"

#include <cstdlib>
#include <iostream>
#include <unordered_map>

#include "frontends/pbrt/build_objects.h"
#include "frontends/pbrt/cameras/parse.h"
#include "frontends/pbrt/integrators/parse.h"
#include "frontends/pbrt/materials/parse.h"
#include "frontends/pbrt/quoted_string.h"
#include "frontends/pbrt/samplers/parse.h"

namespace iris::pbrt_frontend {

bool Parser::Camera() {
  if (world_begin_encountered_) {
    std::cerr << "ERROR: Directive cannot be specified between WorldBegin and "
                 "WorldEnd: Camera"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (camera_encountered_) {
    std::cerr << "ERROR: Directive specified twice for a render: Camera"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  const auto& builder = cameras::Parse(*tokenizers_.back().tokenizer);
  camera_ =
      BuildObject(builder, *tokenizers_.back().tokenizer, *spectrum_manager_,
                  *texture_manager_, matrix_manager_->Get());

  camera_encountered_ = true;

  return true;
}

bool Parser::Include() {
  auto next = tokenizers_.back().tokenizer->Next();
  if (!next) {
    std::cerr << "ERROR: Too few parameters to directive: Include" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = Unquote(*next);
  if (!unquoted) {
    std::cerr << "ERROR: Parameter to Include must be a string" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::filesystem::path file_path(*unquoted);
  if (file_path.is_relative()) {
    file_path = tokenizers_.back().search_path / file_path;
  }

  file_path = std::filesystem::weakly_canonical(file_path);

  for (const auto& entry : tokenizers_) {
    if (entry.file_path && file_path == *entry.file_path) {
      std::cerr << "ERROR: Detected cyclic Include of file: " << *unquoted
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  std::filesystem::path search_path = file_path;
  search_path.remove_filename();

  auto file = std::make_unique<std::ifstream>(file_path);
  if (file->fail()) {
    std::cerr << "ERROR: Failed to open file: " << *unquoted << std::endl;
    exit(EXIT_FAILURE);
  }

  auto tokenizer = std::make_unique<Tokenizer>(*file);

  tokenizers_.emplace_back(tokenizer.get(), std::move(tokenizer),
                           std::filesystem::weakly_canonical(search_path),
                           file_path, std::move(file));

  return true;
}

bool Parser::Integrator() {
  if (world_begin_encountered_) {
    std::cerr << "ERROR: Directive cannot be specified between WorldBegin and "
                 "WorldEnd: Integrator"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (integrator_encountered_) {
    std::cerr << "ERROR: Directive specified twice for a render: Integrator"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  const auto& builder = integrators::Parse(*tokenizers_.back().tokenizer);
  auto object = BuildObject(builder, *tokenizers_.back().tokenizer,
                            *spectrum_manager_, *texture_manager_);
  integrator_ = std::move(object.integrator);
  light_scene_builder_ = std::move(object.light_scene_builder);

  integrator_encountered_ = true;

  return true;
}

bool Parser::MakeNamedMaterial() {
  materials::ParseNamed(*tokenizers_.back().tokenizer, *material_manager_,
                        *spectrum_manager_, *texture_manager_);
  return true;
}

bool Parser::Material() {
  if (!world_begin_encountered_) {
    std::cerr
        << "ERROR: Directive cannot be specified before WorldBegin: Material"
        << std::endl;
    exit(EXIT_FAILURE);
  }

  const auto& builder = materials::Parse(*tokenizers_.back().tokenizer);
  attributes_.back().material =
      BuildObject(builder, *tokenizers_.back().tokenizer, *spectrum_manager_,
                  *texture_manager_, *texture_manager_);

  return true;
}

bool Parser::PixelFilter() {
  if (world_begin_encountered_) {
    std::cerr << "ERROR: Directive cannot be specified between WorldBegin and "
                 "WorldEnd: PixelFilter"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (pixel_filter_encountered_) {
    std::cerr << "ERROR: Directive specified twice for a render: PixelFilter"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto next = tokenizers_.back().tokenizer->Next();
  if (!next) {
    std::cerr << "ERROR: Too few parameters to directive: PixelFilter"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = Unquote(*next);
  if (!unquoted) {
    std::cerr << "ERROR: Parameter to PixelFilter must be a string"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (*unquoted != "box") {
    std::cerr << "ERROR: Unsupported type for directive PixelFilter: "
              << *unquoted << std::endl;
    exit(EXIT_FAILURE);
  }

  pixel_filter_encountered_ = true;

  return true;
}

bool Parser::Sampler() {
  if (world_begin_encountered_) {
    std::cerr << "ERROR: Directive cannot be specified between WorldBegin and "
                 "WorldEnd: Sampler"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (sampler_encountered_) {
    std::cerr << "ERROR: Directive specified twice for a render: Sampler"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  const auto& builder = samplers::Parse(*tokenizers_.back().tokenizer);
  image_sampler_ = BuildObject(builder, *tokenizers_.back().tokenizer,
                               *spectrum_manager_, *texture_manager_);

  sampler_encountered_ = true;

  return true;
}

bool Parser::WorldBegin() {
  if (world_begin_encountered_) {
    std::cerr << "ERROR: Invalid WorldBegin directive" << std::endl;
    exit(EXIT_FAILURE);
  }

  matrix_manager_ = std::make_unique<MatrixManager>();

  world_begin_encountered_ = true;
  return true;
}

bool Parser::WorldEnd() {
  if (!world_begin_encountered_) {
    std::cerr << "ERROR: Invalid WorldEnd directive" << std::endl;
    exit(EXIT_FAILURE);
  }

  return false;
}

std::optional<std::string_view> Parser::PeekToken() {
  while (!tokenizers_.empty()) {
    if (tokenizers_.back().tokenizer->Peek()) {
      return tokenizers_.back().tokenizer->Peek();
    }

    tokenizers_.pop_back();
  }

  return std::nullopt;
}

std::string_view Parser::NextToken() {
  return tokenizers_.back().tokenizer->Next().value();
}

std::optional<Renderable> Parser::ParseFrom(
    const std::filesystem::path& search_root, Tokenizer& tokenizer,
    std::optional<std::filesystem::path> file_path) {
  attributes_.emplace_back(materials::Default());
  material_manager_ = std::make_unique<MaterialManager>();
  matrix_manager_ = std::make_unique<MatrixManager>();
  texture_manager_ = std::make_unique<TextureManager>();

  camera_encountered_ = false;
  integrator_encountered_ = false;
  pixel_filter_encountered_ = false;
  sampler_encountered_ = false;
  world_begin_encountered_ = false;

  static const std::unordered_map<std::string_view, bool (Parser::*)()>
      callbacks = {
          {"Camera", &Parser::Camera},
          {"Include", &Parser::Include},
          {"Integrator", &Parser::Integrator},
          {"MakeNamedMaterial", &Parser::MakeNamedMaterial},
          {"Material", &Parser::Material},
          {"PixelFilter", &Parser::PixelFilter},
          {"Sampler", &Parser::Sampler},
          {"WorldBegin", &Parser::WorldBegin},
          {"WorldEnd", &Parser::WorldEnd},
      };

  if (file_path) {
    *file_path = std::filesystem::weakly_canonical(*file_path);
  }

  tokenizers_.emplace_back(&tokenizer, nullptr,
                           std::filesystem::weakly_canonical(search_root),
                           file_path, nullptr);

  bool tokens_parsed = false;
  for (;;) {
    auto peeked_token = PeekToken();
    if (!peeked_token.has_value()) {
      if (tokens_parsed) {
        std::cerr << "ERROR: Final directive should be WorldEnd" << std::endl;
        exit(EXIT_FAILURE);
      }

      return std::nullopt;
    }

    tokens_parsed = true;

    if (matrix_manager_->TryParse(*tokenizers_.back().tokenizer)) {
      continue;
    }

    auto token = NextToken();

    auto iter = callbacks.find(token);
    if (iter == callbacks.end()) {
      std::cerr << "ERROR: Invalid directive: " << token << std::endl;
      exit(EXIT_FAILURE);
    }

    if (!(this->*iter->second)()) {
      break;
    }
  }

  camera_ = nullptr;
  attributes_.clear();
  spectrum_manager_->Clear();
  material_manager_.reset();
  matrix_manager_.reset();
  texture_manager_.reset();

  return std::nullopt;
}

}  // namespace iris::pbrt_frontend