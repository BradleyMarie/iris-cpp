#ifndef _IRIS_GEOMETRY_TRIANGLE_MESH_
#define _IRIS_GEOMETRY_TRIANGLE_MESH_

#include <array>
#include <memory>
#include <optional>
#include <span>
#include <variant>
#include <vector>

#include "iris/emissive_material.h"
#include "iris/geometry.h"
#include "iris/hit_allocator.h"
#include "iris/integer.h"
#include "iris/material.h"
#include "iris/normal_map.h"
#include "iris/point.h"
#include "iris/ray.h"
#include "iris/reference_countable.h"
#include "iris/reference_counted.h"
#include "iris/sampler.h"
#include "iris/texture_coordinates.h"
#include "iris/vector.h"

namespace iris {
namespace geometry {

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
    ReferenceCounted<NormalMap> back_normal_map);

}  // namespace geometry
}  // namespace iris

#endif  // _IRIS_GEOMETRY_TRIANGLE_MESH_