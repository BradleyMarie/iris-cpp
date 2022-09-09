#include "iris/ray_tracer.h"

#include <limits>

#include "iris/internal/ray_tracer.h"

namespace iris {

std::optional<RayTracer::Result> RayTracer::Trace(const Ray& ray) {
  auto* hit =
      ray_tracer_.Trace(ray, minimum_distance_,
                        std::numeric_limits<geometric>::infinity(), scene_);
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

  const Bsdf* bsdf = nullptr;
  if (auto material =
          hit->geometry->GetMaterial(hit->front, hit->additional_data)) {
    bsdf = material->Compute(texture_coordinates);
  }

  const Spectrum* spectrum = nullptr;
  if (auto* emissive_material = hit->geometry->GetEmissiveMaterial(
          hit->front, hit->additional_data)) {
    spectrum = emissive_material->Compute(texture_coordinates);
  }

  Vector model_surface_normal = hit->geometry->ComputeSurfaceNormal(
      model_hit_point, hit->front, hit->additional_data);

  auto shading_normal_variant =
      hit->geometry->ComputeShadingNormal(hit->front, hit->additional_data);
  Vector model_shading_normal =
      std::holds_alternative<const NormalMap*>(shading_normal_variant)
          ? std::get<const NormalMap*>(shading_normal_variant)
                ? std::get<const NormalMap*>(shading_normal_variant)
                      ->Compute(texture_coordinates, model_surface_normal)
                : model_surface_normal
          : std::get<Vector>(shading_normal_variant);

  if (!hit->model_to_world) {
    return RayTracer::Result{bsdf, spectrum, world_hit_point,
                             model_surface_normal, model_shading_normal};
  }

  return RayTracer::Result{
      bsdf, spectrum, world_hit_point,
      Normalize(
          hit->model_to_world->InverseTransposeMultiply(model_surface_normal)),
      Normalize(
          hit->model_to_world->InverseTransposeMultiply(model_shading_normal))};
}

}  // namespace iris