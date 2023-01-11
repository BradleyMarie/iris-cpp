#include "iris/geometry/triangle_mesh.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/emissive_materials/mock_emissive_material.h"
#include "iris/materials/mock_material.h"
#include "iris/normal_maps/mock_normal_map.h"
#include "iris/testing/hit_allocator.h"

static const iris::face_t FRONT_FACE = 0u;
static const iris::face_t BACK_FACE = 1u;

iris::ReferenceCounted<iris::Material> back_material =
    iris::MakeReferenceCounted<iris::materials::MockMaterial>();
iris::ReferenceCounted<iris::Material> front_material =
    iris::MakeReferenceCounted<iris::materials::MockMaterial>();
iris::ReferenceCounted<iris::EmissiveMaterial> front_emissive_material =
    iris::MakeReferenceCounted<
        iris::emissive_materials::MockEmissiveMaterial>();
iris::ReferenceCounted<iris::EmissiveMaterial> back_emissive_material =
    iris::MakeReferenceCounted<
        iris::emissive_materials::MockEmissiveMaterial>();
iris::ReferenceCounted<iris::NormalMap> front_normal_map =
    iris::MakeReferenceCounted<iris::normal_maps::MockNormalMap>();
iris::ReferenceCounted<iris::NormalMap> back_normal_map =
    iris::MakeReferenceCounted<iris::normal_maps::MockNormalMap>();

iris::ReferenceCounted<iris::Geometry> SimpleTriangle() {
  auto triangles = iris::geometry::AllocateTriangleMesh(
      {iris::Point(0.0, 0.0, 0.0), iris::Point(1.0, 0.0, 0.0),
       iris::Point(0.0, 1.0, 0.0)},
      {{0, 1, 2}}, std::vector<iris::Vector>(),
      {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, back_material, front_material,
      front_emissive_material, back_emissive_material, front_normal_map,
      back_normal_map);
  return triangles.front();
}

TEST(Triangle, ComputeSurfaceNormal) {
  auto triangle = SimpleTriangle();

  auto front_normal = triangle->ComputeSurfaceNormal(iris::Point(0.0, 0.0, 0.0),
                                                     FRONT_FACE, nullptr);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), front_normal);

  auto back_normal = triangle->ComputeSurfaceNormal(iris::Point(0.0, 0.0, 0.0),
                                                    BACK_FACE, nullptr);
  EXPECT_EQ(iris::Vector(0.0, 0.0, -1.0), back_normal);
}