#include "iris/light_scene.h"

#include "iris/internal/visibility_tester.h"

namespace iris {
namespace {

class AreaLight final : public Light {
 public:
  AreaLight(const Geometry& geometry, const Matrix* model_to_world, face_t face,
            visual_t surface_area) noexcept
      : geometry_(geometry),
        model_to_world_(model_to_world),
        face_(face),
        surface_area_(surface_area) {}

  std::optional<SampleResult> Sample(
      const Point& hit_point, Random& rng, VisibilityTester& tester,
      SpectralAllocator& allocator) const override;

  const Spectrum* Emission(const Ray& to_light, VisibilityTester& tester,
                           SpectralAllocator& allocator,
                           visual_t* pdf = nullptr) const override;

 private:
  const Geometry& geometry_;
  const Matrix* model_to_world_;
  const face_t face_;
  visual_t surface_area_;
};

std::optional<Light::SampleResult> AreaLight::Sample(
    const Point& hit_point, Random& rng, VisibilityTester& tester,
    SpectralAllocator& allocator) const {
  auto sampled_point = geometry_.SampleFace(face_, rng);
  if (!sampled_point) {
    return std::nullopt;
  }

  auto to_light = Normalize(*sampled_point - hit_point);

  visual_t pdf;
  auto* emission = Emission(Ray(hit_point, to_light), tester, allocator, &pdf);
  if (!emission) {
    return std::nullopt;
  }

  return Light::SampleResult{*emission, to_light, pdf};
}

const Spectrum* AreaLight::Emission(const Ray& to_light,
                                    VisibilityTester& tester,
                                    SpectralAllocator& allocator,
                                    visual_t* pdf) const {
  internal::VisibilityTester& internal_tester =
      static_cast<internal::VisibilityTester&>(tester);

  auto result = internal_tester.Visible(to_light, geometry_, model_to_world_,
                                        face_, surface_area_, pdf);

  if (!result) {
    return nullptr;
  }

  return &result->emission;
}

}  // namespace

void LightScene::Builder::Add(std::unique_ptr<Light> light) {
  lights_.push_back(std::move(light));
}

std::unique_ptr<LightScene> LightScene::Builder::Build(const Scene& scene) {
  for (const auto& [geometry, model_to_world] : scene) {
    for (face_t face : geometry.GetFaces()) {
      if (!geometry.IsEmissive(face)) {
        continue;
      }

      auto surface_area = geometry.ComputeArea(face);
      if (!surface_area) {
        continue;
      }

      lights_.push_back(std::make_unique<AreaLight>(geometry, model_to_world,
                                                    face, *surface_area));
    }
  }

  auto result = Build(std::move(lights_));
  lights_.clear();

  return result;
}

}  // namespace iris