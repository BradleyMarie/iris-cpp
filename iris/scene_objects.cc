#include "iris/scene_objects.h"

#include <limits>
#include <set>

#include "absl/log/check.h"
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
      const Point& hit_point, Sampler sampler, VisibilityTester& tester,
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
    const Point& hit_point, Sampler sampler, VisibilityTester& tester,
    SpectralAllocator& allocator) const {
  auto sampled_point = geometry_.SampleFace(face_, sampler);
  if (!sampled_point) {
    return std::nullopt;
  }

  auto world_sample_point = model_to_world_
                                ? model_to_world_->Multiply(*sampled_point)
                                : *sampled_point;

  auto to_light = Normalize(world_sample_point - hit_point);

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

void SceneObjects::Builder::Add(ReferenceCounted<Geometry> geometry,
                                const Matrix& matrix) {
  if (!geometry) {
    return;
  }

  auto insertion_result = matrix_to_transform_index_.insert(
      {matrix, matrix_to_transform_index_.size()});
  if (insertion_result.second) {
    matrices_.push_back(matrix);
  }

  CHECK(insertion_result.first->second <=
        std::numeric_limits<uintptr_t>::max());
  uintptr_t matrix_index =
      static_cast<uintptr_t>(insertion_result.first->second);

  geometry_.push_back(
      std::make_pair(geometry, reinterpret_cast<const Matrix*>(matrix_index)));
}

void SceneObjects::Builder::Add(ReferenceCounted<Light> light) {
  if (!light) {
    return;
  }

  lights_.push_back(std::move(light));
}

SceneObjects SceneObjects::Builder::Build() {
  assert((std::set<std::pair<ReferenceCounted<Geometry>, const Matrix*>>(
              geometry_.begin(), geometry_.end())
              .size() == geometry_.size()));

  SceneObjects result(std::move(*this));

  matrix_to_transform_index_.clear();
  matrices_.clear();
  geometry_.clear();
  lights_.clear();

  matrix_to_transform_index_.insert({Matrix::Identity(), 0});
  matrices_.push_back(Matrix::Identity());

  return result;
}

SceneObjects::SceneObjects(Builder&& builder)
    : geometry_(std::move(builder.geometry_)),
      lights_(std::move(builder.lights_)),
      matrices_(std::move(builder.matrices_)) {
  matrices_.shrink_to_fit();
  geometry_.shrink_to_fit();

  for (auto& entry : geometry_) {
    if (entry.second) {
      entry.second = &matrices_.at(reinterpret_cast<uintptr_t>(entry.second));
    }

    for (face_t face : entry.first->GetFaces()) {
      if (!entry.first->IsEmissive(face)) {
        continue;
      }

      lights_.push_back(iris::MakeReferenceCounted<AreaLight>(
          std::cref(*entry.first), entry.second, face,
          entry.first->ComputeArea(face).value()));
    }
  }

  lights_.shrink_to_fit();
}

}  // namespace iris