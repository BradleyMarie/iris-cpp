#include "frontends/pbrt/materials/matte.h"

#include "iris/materials/matte_material.h"

namespace iris::pbrt_frontend::materials {
namespace {

static const std::unordered_map<std::string_view, Parameter::Type>
    g_parameters = {
        {"Kd", Parameter::REFLECTOR_TEXTURE},
        {"sigma", Parameter::FLOAT_TEXTURE},
};

class MatteObjectBuilder
    : public ObjectBuilder<
          std::shared_ptr<
              ObjectBuilder<iris::ReferenceCounted<Material>, TextureManager&>>,
          TextureManager&> {
 public:
  MatteObjectBuilder() noexcept : ObjectBuilder(g_parameters) {}

  std::shared_ptr<
      ObjectBuilder<iris::ReferenceCounted<Material>, TextureManager&>>
  Build(const std::unordered_map<std::string_view, Parameter>& parameters,
        TextureManager& texture_manager) const override;
};

class NestedMatteObjectBuilder
    : public ObjectBuilder<iris::ReferenceCounted<Material>, TextureManager&> {
 public:
  NestedMatteObjectBuilder(
      iris::ReferenceCounted<iris::textures::PointerTexture2D<
          iris::Reflector, iris::SpectralAllocator>>
          diffuse,
      iris::ReferenceCounted<iris::textures::ValueTexture2D<iris::visual>>
          sigma)
      : ObjectBuilder(g_parameters),
        diffuse_(std::move(diffuse)),
        sigma_(std::move(sigma)),
        default_(iris::MakeReferenceCounted<iris::materials::MatteMaterial>(
            diffuse_, sigma_)) {}

  iris::ReferenceCounted<Material> Build(
      const std::unordered_map<std::string_view, Parameter>& parameters,
      TextureManager& texture_manager) const override;

 private:
  iris::ReferenceCounted<iris::textures::PointerTexture2D<
      iris::Reflector, iris::SpectralAllocator>>
      diffuse_;
  iris::ReferenceCounted<iris::textures::ValueTexture2D<iris::visual>> sigma_;
  iris::ReferenceCounted<Material> default_;
};

std::shared_ptr<
    ObjectBuilder<iris::ReferenceCounted<Material>, TextureManager&>>
MatteObjectBuilder::Build(
    const std::unordered_map<std::string_view, Parameter>& parameters,
    TextureManager& texture_manager) const {
  auto diffuse_texture = texture_manager.AllocateUniformReflectorTexture(0.5);
  auto sigma_texture = texture_manager.AllocateUniformFloatTexture(0.5);

  auto kd = parameters.find("Kd");
  if (kd != parameters.end()) {
    diffuse_texture = kd->second.GetReflectorTextures(1).front();
  }

  auto sigma = parameters.find("sigma");
  if (sigma != parameters.end()) {
    sigma_texture = sigma->second.GetFloatTextures(1).front();
  }

  return std::make_unique<NestedMatteObjectBuilder>(std::move(diffuse_texture),
                                                    std::move(sigma_texture));
}

iris::ReferenceCounted<Material> NestedMatteObjectBuilder::Build(
    const std::unordered_map<std::string_view, Parameter>& parameters,
    TextureManager& texture_manager) const {
  if (parameters.empty()) {
    return default_;
  }

  auto diffuse_texture = diffuse_;
  auto sigma_texture = sigma_;

  auto kd = parameters.find("Kd");
  if (kd != parameters.end()) {
    diffuse_texture = kd->second.GetReflectorTextures(1).front();
  }

  auto sigma = parameters.find("sigma");
  if (sigma != parameters.end()) {
    sigma_texture = sigma->second.GetFloatTextures(1).front();
  }

  return iris::MakeReferenceCounted<iris::materials::MatteMaterial>(
      std::move(diffuse_texture), std::move(sigma_texture));
}

}  // namespace

const std::unique_ptr<
    const ObjectBuilder<std::shared_ptr<ObjectBuilder<
                            iris::ReferenceCounted<Material>, TextureManager&>>,
                        TextureManager&>>
    g_matte_builder = std::make_unique<MatteObjectBuilder>();

}  // namespace iris::pbrt_frontend::materials