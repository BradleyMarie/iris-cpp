#include "iris/ray_tracer.h"

#include <limits>

#include "iris/bxdf_allocator.h"
#include "iris/internal/ray_tracer.h"
#include "iris/spectral_allocator.h"

namespace iris {
namespace {

std::pair<Vector, Vector> Transform(const Matrix& model_to_world,
                                    const std::pair<Vector, Vector>& vectors) {
  return std::make_pair(
      Normalize(model_to_world.InverseTransposeMultiply(vectors.first)),
      Normalize(model_to_world.InverseTransposeMultiply(vectors.second)));
}

std::pair<Vector, Vector> MaybeTransform(const Matrix* model_to_world,
                                         const Vector& surface_normal,
                                         const Vector& shading_normal) {
  auto vectors = std::make_pair(surface_normal, shading_normal);
  return model_to_world ? Transform(*model_to_world, vectors) : vectors;
}

std::optional<Bsdf> MakeBsdf(const iris::internal::Hit& hit,
                             const TextureCoordinates& texture_coordinates,
                             const Vector& world_surface_normal,
                             const Vector& world_shading_normal,
                             SpectralAllocator& spectral_allocator,
                             BxdfAllocator& bxdf_allocator) {
  auto* material = hit.geometry->GetMaterial(hit.front, hit.additional_data);
  if (!material) {
    return std::nullopt;
  }

  auto* bxdf = material->Evaluate(texture_coordinates, spectral_allocator,
                                  bxdf_allocator);
  if (!bxdf) {
    return std::nullopt;
  }

  return Bsdf(*bxdf, world_surface_normal, world_shading_normal);
}

}  // namespace

std::optional<RayTracer::Result> RayTracer::Trace(const Ray& ray) {
  auto* hit =
      ray_tracer_.Trace(ray, minimum_distance_,
                        std::numeric_limits<geometric_t>::infinity(), scene_);
  if (!hit) {
    return std::nullopt;
  }

  Point world_hit_point = ray.Endpoint(hit->distance);
  Point model_hit_point =
      hit->model_to_world
          ? hit->model_to_world->InverseMultiply(world_hit_point)
          : world_hit_point;

  TextureCoordinates texture_coordinates =
      hit->geometry
          ->ComputeTextureCoordinates(model_hit_point, hit->front,
                                      hit->additional_data)
          .value_or(TextureCoordinates{0.0, 0.0});

  SpectralAllocator spectral_allocator(arena_);

  const Spectrum* spectrum = nullptr;
  if (auto* emissive_material = hit->geometry->GetEmissiveMaterial(
          hit->front, hit->additional_data)) {
    spectrum =
        emissive_material->Evaluate(texture_coordinates, spectral_allocator);
  }

  Vector model_surface_normal = hit->geometry->ComputeSurfaceNormal(
      model_hit_point, hit->front, hit->additional_data);

  auto shading_normal_variant =
      hit->geometry->ComputeShadingNormal(hit->front, hit->additional_data);
  Vector model_shading_normal =
      std::holds_alternative<const NormalMap*>(shading_normal_variant)
          ? std::get<const NormalMap*>(shading_normal_variant)
                ? std::get<const NormalMap*>(shading_normal_variant)
                      ->Evaluate(texture_coordinates, model_surface_normal)
                : model_surface_normal
          : std::get<Vector>(shading_normal_variant);

  auto vectors = MaybeTransform(hit->model_to_world, model_surface_normal,
                                model_shading_normal);

  BxdfAllocator bxdf_allocator(arena_);
  auto bsdf = MakeBsdf(*hit, texture_coordinates, vectors.first, vectors.second,
                       spectral_allocator, bxdf_allocator);

  return RayTracer::Result{bsdf, spectrum, world_hit_point, vectors.first,
                           vectors.second};
}

}  // namespace iris