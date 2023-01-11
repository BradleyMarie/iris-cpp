#include "iris/geometry/triangle_mesh.h"

namespace iris {
namespace geometry {
namespace {

std::array<geometric_t, 3> Subtract(const Point& point0, const Point& point1) {
  return std::array<geometric_t, 3>{point0.x - point1.x, point0.y - point1.y,
                                    point0.z - point1.z};
}

std::array<geometric_t, 3> PermuteXDominant(
    const std::array<geometric_t, 3>& point) {
  return std::array<geometric_t, 3>{point[2], point[1], point[0]};
}

std::array<geometric_t, 3> PermuteYDominant(
    const std::array<geometric_t, 3>& point) {
  return std::array<geometric_t, 3>{point[0], point[2], point[1]};
}

class Triangle final : public Geometry {
 public:
  struct SharedData {
    std::vector<Point> points;
    std::vector<Vector> normals;
    std::vector<std::pair<geometric, geometric>> uv;
    ReferenceCounted<Material> materials[2];
    ReferenceCounted<EmissiveMaterial> emissive_materials[2];
    ReferenceCounted<NormalMap> normal_maps[2];
  };

  typedef std::array<geometric_t, 3> AdditionalData;

  static const face_t FRONT_FACE = 0u;
  static const face_t BACK_FACE = 1u;

  Triangle(const std::array<uint32_t, 3>& vertices,
           std::shared_ptr<const SharedData> shared) noexcept
      : vertices_(vertices), shared_(std::move(shared)) {}

  virtual Vector ComputeSurfaceNormal(
      const Point& hit_point, face_t face,
      const void* additional_data) const override;

  virtual std::optional<TextureCoordinates> ComputeTextureCoordinates(
      const Point& hit_point, face_t face,
      const void* additional_data) const override;

  virtual std::variant<Vector, const NormalMap*> ComputeShadingNormal(
      face_t face, const void* additional_data) const override;

  virtual const Material* GetMaterial(
      face_t face, const void* additional_data) const override;

  virtual bool IsEmissive(face_t face) const override;

  virtual const EmissiveMaterial* GetEmissiveMaterial(
      face_t face, const void* additional_data) const override;

  virtual std::optional<Point> SampleFace(face_t face,
                                          Sampler& sampler) const override;

  virtual std::optional<visual_t> ComputeArea(face_t face) const override;

  virtual std::span<const face_t> GetFaces() const override;

 private:
  virtual Hit* Trace(const Ray& ray,
                     HitAllocator& hit_allocator) const override;

  Vector ComputeSurfaceNormal() const;

  const std::array<uint32_t, 3> vertices_;
  const std::shared_ptr<const SharedData> shared_;
};

Vector Triangle::ComputeSurfaceNormal() const {
  Vector v0_to_v1 =
      shared_->points[vertices_[1]] - shared_->points[vertices_[0]];
  Vector v0_to_v2 =
      shared_->points[vertices_[2]] - shared_->points[vertices_[0]];
  return CrossProduct(v0_to_v1, v0_to_v2);
}

Vector Triangle::ComputeSurfaceNormal(const Point& hit_point, face_t face,
                                      const void* additional_data) const {
  Vector surface_normal = ComputeSurfaceNormal();
  Vector normalized = Normalize(surface_normal);

  if (face == FRONT_FACE) {
    return normalized;
  }

  return -normalized;
}

std::optional<TextureCoordinates> Triangle::ComputeTextureCoordinates(
    const Point& hit_point, face_t face, const void* additional_data) const {
  if (shared_->uv.empty()) {
    return std::nullopt;
  }

  const auto* barycentric = static_cast<const AdditionalData*>(additional_data);

  geometric_t u = shared_->uv[vertices_[0]].first * (*barycentric)[0] +
                  shared_->uv[vertices_[1]].first * (*barycentric)[1] +
                  shared_->uv[vertices_[2]].first * (*barycentric)[2];
  geometric_t v = shared_->uv[vertices_[0]].second * (*barycentric)[0] +
                  shared_->uv[vertices_[1]].second * (*barycentric)[1] +
                  shared_->uv[vertices_[2]].second * (*barycentric)[2];

  return TextureCoordinates{{u, v}};
}

std::variant<Vector, const NormalMap*> Triangle::ComputeShadingNormal(
    face_t face, const void* additional_data) const {
  if (shared_->normal_maps[face]) {
    return shared_->normal_maps[face].Get();
  }

  if (shared_->normals.empty()) {
    return nullptr;
  }

  const auto* barycentric = static_cast<const AdditionalData*>(additional_data);
  Vector shading_normal = shared_->normals[vertices_[0]] * (*barycentric)[0] +
                          shared_->normals[vertices_[1]] * (*barycentric)[1] +
                          shared_->normals[vertices_[2]] * (*barycentric)[2];
  Vector normalized = Normalize(shading_normal);

  if (face == FRONT_FACE) {
    return normalized;
  }

  return -normalized;
}

const Material* Triangle::GetMaterial(face_t face,
                                      const void* additional_data) const {
  return shared_->materials[face].Get();
}

bool Triangle::IsEmissive(face_t face) const {
  return shared_->emissive_materials[face].Get();
}

const EmissiveMaterial* Triangle::GetEmissiveMaterial(
    face_t face, const void* additional_data) const {
  return shared_->emissive_materials[face].Get();
}

std::optional<Point> Triangle::SampleFace(face_t face, Sampler& sampler) const {
  geometric_t u = sampler.Next();
  geometric_t v = sampler.Next();

  if (u + v > (geometric_t)1.0) {
    u = (geometric_t)1.0 - u;
    v = (geometric_t)1.0 - v;
  }

  Vector v0_to_v1 =
      shared_->points[vertices_[1]] - shared_->points[vertices_[0]];
  Vector v0_to_v2 =
      shared_->points[vertices_[2]] - shared_->points[vertices_[0]];

  return shared_->points[vertices_[0]] + v0_to_v1 * u + v0_to_v2 * v;
}

std::optional<visual_t> Triangle::ComputeArea(face_t face) const {
  Vector surface_normal = ComputeSurfaceNormal();
  return surface_normal.Length() * 0.5;
}

std::span<const face_t> Triangle::GetFaces() const {
  static const face_t faces[2] = {FRONT_FACE, BACK_FACE};
  return faces;
}

Hit* Triangle::Trace(const Ray& ray, HitAllocator& hit_allocator) const {
  auto v0 = Subtract(shared_->points[vertices_[0]], ray.origin);
  auto v1 = Subtract(shared_->points[vertices_[1]], ray.origin);
  auto v2 = Subtract(shared_->points[vertices_[2]], ray.origin);

  auto dominant_axis = ray.direction.DominantAxis();

  geometric_t shear_x, shear_y, direction_z;
  switch (dominant_axis) {
    case Vector::X_AXIS:
      v0 = PermuteXDominant(v0);
      v1 = PermuteXDominant(v1);
      v2 = PermuteXDominant(v2);

      shear_x = -ray.direction.z / ray.direction.x;
      shear_y = -ray.direction.y / ray.direction.x;
      direction_z = ray.direction.x;
      break;
    case Vector::Y_AXIS:
      v0 = PermuteYDominant(v0);
      v1 = PermuteYDominant(v1);
      v2 = PermuteYDominant(v2);

      shear_x = -ray.direction.x / ray.direction.y;
      shear_y = -ray.direction.z / ray.direction.y;
      direction_z = ray.direction.y;
      break;
    case Vector::Z_AXIS:
      shear_x = -ray.direction.x / ray.direction.z;
      shear_y = -ray.direction.y / ray.direction.z;
      direction_z = ray.direction.z;
      break;
  }

  v0[0] += shear_x * v0[2];
  v0[1] += shear_y * v0[2];
  v1[0] += shear_x * v1[2];
  v1[1] += shear_y * v1[2];
  v2[0] += shear_x * v2[2];
  v2[1] += shear_y * v2[2];

  geometric_t b0 = v1[0] * v2[1] - v1[1] * v2[0];
  geometric_t b1 = v2[0] * v0[1] - v2[1] * v0[0];
  geometric_t b2 = v0[0] * v1[1] - v0[1] * v1[0];

  if constexpr (std::is_same<geometric_t, float>::value) {
    if (b0 == 0.0 || b1 == 0.0 || b2 == 0.0) {
      b0 = ((double)v1[0] * (double)v2[1] - (double)v1[1] * (double)v2[0]);
      b1 = ((double)v2[0] * (double)v0[1] - (double)v2[1] * (double)v0[0]);
      b2 = ((double)v0[0] * (double)v1[1] - (double)v0[1] * (double)v1[0]);
    }
  }

  if constexpr (std::is_same<geometric_t, double>::value) {
    if (b0 == 0.0 || b1 == 0.0 || b2 == 0.0) {
      b0 = ((long double)v1[0] * (long double)v2[1] -
            (long double)v1[1] * (long double)v2[0]);
      b1 = ((long double)v2[0] * (long double)v0[1] -
            (long double)v2[1] * (long double)v0[0]);
      b2 = ((long double)v0[0] * (long double)v1[1] -
            (long double)v0[1] * (long double)v1[0]);
    }
  }

  if ((b0 < 0.0 || b1 < 0.0 || b2 < 0.0) &&
      (b0 > 0.0 || b1 > 0.0 || b2 > 0.0)) {
    return nullptr;
  }

  geometric_t determinant = b0 + b1 + b2;
  if (determinant == 0.0) {
    return nullptr;
  }

  geometric_t shear_z = (geometric_t)1.0 / direction_z;
  v0[2] = v0[2] * shear_z;
  v1[2] = v1[2] * shear_z;
  v2[2] = v2[2] * shear_z;

  geometric_t distance = b0 * v0[2] + b1 * v1[2] + b2 * v2[2];

  geometric_t inverse_determinant = (geometric_t)1.0 / determinant;
  distance *= inverse_determinant;

  AdditionalData additional_data{b0 * inverse_determinant,
                                 b1 * inverse_determinant,
                                 b2 * inverse_determinant};

  geometric_t dp = DotProduct(ray.direction, Normalize(ComputeSurfaceNormal()));

  face_t front_face, back_face;
  if (dp < 0.0) {
    front_face = FRONT_FACE;
    back_face = BACK_FACE;
  } else {
    front_face = BACK_FACE;
    back_face = FRONT_FACE;
  }

  return &hit_allocator.Allocate(nullptr, distance, front_face, back_face,
                                 additional_data);
}

}  // namespace

std::vector<ReferenceCounted<Geometry>> AllocateTriangleMesh(
    std::vector<Point> points,
    const std::vector<std::array<uint32_t, 3>>& indices,
    std::vector<Vector> normals,
    std::vector<std::pair<geometric, geometric>> uv,
    ReferenceCounted<Material> back_material,
    ReferenceCounted<Material> front_material,
    ReferenceCounted<EmissiveMaterial> front_emissive_material,
    ReferenceCounted<EmissiveMaterial> back_emissive_material,
    ReferenceCounted<NormalMap> front_normal_map,
    ReferenceCounted<NormalMap> back_normal_map) {
  auto shared_data =
      std::make_shared<Triangle::SharedData>(Triangle::SharedData{
          std::move(points),
          std::move(normals),
          std::move(uv),
          {std::move(back_material), std::move(front_material)},
          {std::move(front_emissive_material),
           std::move(back_emissive_material)},
          {std::move(front_normal_map), std::move(back_normal_map)}});

  std::vector<ReferenceCounted<Geometry>> result;
  for (const auto& entry : indices) {
    assert(shared_data->normals.empty() ||
           entry[0] < shared_data->normals.size());
    assert(shared_data->normals.empty() ||
           entry[1] < shared_data->normals.size());
    assert(shared_data->normals.empty() ||
           entry[2] < shared_data->normals.size());
    assert(shared_data->uv.empty() || entry[0] < shared_data->uv.size());
    assert(shared_data->uv.empty() || entry[1] < shared_data->uv.size());
    assert(shared_data->uv.empty() || entry[2] < shared_data->uv.size());

    if (shared_data->points.at(entry[0]) == shared_data->points.at(entry[1]) ||
        shared_data->points.at(entry[1]) == shared_data->points.at(entry[2]) ||
        shared_data->points.at(entry[2]) == shared_data->points.at(entry[0])) {
      continue;
    }

    result.push_back(MakeReferenceCounted<Triangle>(entry, shared_data));
  }

  return result;
}

}  // namespace geometry
}  // namespace iris