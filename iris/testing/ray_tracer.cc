#include "iris/testing/ray_tracer.h"

#include <cmath>
#include <memory>

#include "iris/internal/arena.h"
#include "iris/internal/ray_tracer.h"
#include "iris/scene.h"

namespace iris {
namespace testing {
namespace {

class BxdfMaterial : public Material {
 public:
  BxdfMaterial(const Bxdf* bxdf) : bxdf_(bxdf) {}

  const Bxdf* Evaluate(const TextureCoordinates& texture_coordinates,
                       SpectralAllocator& spectral_allocator,
                       BxdfAllocator& bxdf_allocator) const override {
    return bxdf_;
  }

 private:
  const Bxdf* bxdf_;
};

class SpectrumMaterial : public EmissiveMaterial {
 public:
  SpectrumMaterial(const Spectrum* spectrum) : spectrum_(spectrum) {}

  const Spectrum* Evaluate(
      const TextureCoordinates& texture_coordinates,
      SpectralAllocator& spectral_allocator) const override {
    return spectrum_;
  }

 private:
  const Spectrum* spectrum_;
};

class AlwaysHitsGeometry : public Geometry {
 public:
  AlwaysHitsGeometry(geometric_t distance, const Vector& world_surface_normal,
                     const Vector& world_shading_normal, const Bxdf* bxdf,
                     const Spectrum* spectrum)
      : distance_(distance),
        world_surface_normal_(world_surface_normal),
        world_shading_normal_(world_shading_normal),
        material_(bxdf),
        emissive_material_(spectrum) {
    assert(std::isfinite(distance) && distance > 0.0);
  }

  Hit* Trace(const Ray& ray, HitAllocator& hit_allocator) const override {
    return &hit_allocator.Allocate(nullptr, distance_, 1, 1);
  }

  Vector ComputeSurfaceNormal(const Point& hit_point, face_t face,
                              const void* additional_data) const override {
    return world_surface_normal_;
  }

  std::variant<Vector, const NormalMap*> ComputeShadingNormal(
      face_t face, const void* additional_data) const override {
    return world_shading_normal_;
  }

  const Material* GetMaterial(face_t face,
                              const void* additional_data) const override {
    return &material_;
  }

  const EmissiveMaterial* GetEmissiveMaterial(
      face_t face, const void* additional_data) const override {
    return &emissive_material_;
  }

  std::span<const face_t> GetFaces() const override {
    static const face_t faces[] = {1};
    return faces;
  }

 private:
  const visual_t distance_;
  const Vector world_surface_normal_;
  const Vector world_shading_normal_;
  const BxdfMaterial material_;
  const SpectrumMaterial emissive_material_;
};

class NoHitsScene final : public Scene {
 public:
  void Trace(const Ray& ray, Intersector& intersector) const override {}
};

class HitScene final : public Scene {
 public:
  HitScene(std::span<const RayTracerPathNode> trace_results) noexcept {
    for (const auto& trace_result : trace_results) {
      geometry_.push_back(std::make_unique<AlwaysHitsGeometry>(
          trace_result.distance, trace_result.surface_normal,
          trace_result.shading_normal, trace_result.bxdf,
          trace_result.emission));
    }
  }

  void Trace(const Ray& ray, Intersector& intersector) const override {
    if (geometry_index_ != geometry_.size()) {
      intersector.Intersect(*geometry_[geometry_index_++]);
    }
  }

 private:
  std::vector<std::unique_ptr<Geometry>> geometry_;
  mutable size_t geometry_index_ = 0;
};

}  // namespace

RayTracer& GetNoHitsRayTracer() {
  static NoHitsScene no_hits_scene;
  thread_local internal::RayTracer internal_ray_tracer;
  thread_local internal::Arena arena;
  thread_local RayTracer ray_tracer(no_hits_scene, nullptr, 0.0,
                                    internal_ray_tracer, arena);
  return ray_tracer;
}

void ScopedNoHitsRayTracer(const EnvironmentalLight& environmental_light,
                           std::function<void(RayTracer&)> callback) {
  static NoHitsScene no_hits_scene;
  internal::RayTracer internal_ray_tracer;
  internal::Arena arena;
  RayTracer ray_tracer(no_hits_scene, &environmental_light, 0.0,
                       internal_ray_tracer, arena);
  callback(ray_tracer);
}

void ScopedHitsRayTracer(const EnvironmentalLight* environmental_light,
                         std::span<const RayTracerPathNode> trace_results,
                         std::function<void(RayTracer&)> callback) {
  internal::RayTracer internal_ray_tracer;
  internal::Arena arena;
  HitScene scene(trace_results);
  RayTracer ray_tracer(scene, environmental_light, 0.0, internal_ray_tracer,
                       arena);
  callback(ray_tracer);
}

}  // namespace testing
}  // namespace iris