#include "iris/geometry/triangle_mesh.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/emissive_materials/mock_emissive_material.h"
#include "iris/materials/mock_material.h"
#include "iris/normal_maps/mock_normal_map.h"
#include "iris/random/mock_random.h"
#include "iris/testing/hit_allocator.h"

typedef std::array<iris::geometric_t, 3> AdditionalData;
static const iris::face_t FRONT_FACE = 0u;
static const iris::face_t BACK_FACE = 1u;

iris::ReferenceCounted<iris::Material> front_material =
    iris::MakeReferenceCounted<iris::materials::MockMaterial>();
iris::ReferenceCounted<iris::Material> back_material =
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
      {{0, 1, 2}}, {}, {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, front_material,
      back_material, front_emissive_material, back_emissive_material,
      front_normal_map, back_normal_map);
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

TEST(Triangle, ComputeTextureCoordinatesNone) {
  auto triangles = iris::geometry::AllocateTriangleMesh(
      {iris::Point(0.0, 0.0, 0.0), iris::Point(1.0, 0.0, 0.0),
       iris::Point(0.0, 1.0, 0.0)},
      {{0, 1, 2}}, {}, {}, back_material, front_material,
      front_emissive_material, back_emissive_material, front_normal_map,
      back_normal_map);

  auto texture_coordinates = triangles.front()->ComputeTextureCoordinates(
      iris::Point(0.0, 0.0, 0.0), FRONT_FACE, nullptr);
  EXPECT_FALSE(texture_coordinates);
}

TEST(Triangle, ComputeTextureCoordinates) {
  auto triangle = SimpleTriangle();

  AdditionalData additional_data0({1.0, 0.0, 0.0});
  auto texture_coordinates0 = triangle->ComputeTextureCoordinates(
      iris::Point(0.0, 0.0, 0.0), FRONT_FACE, &additional_data0);
  EXPECT_EQ(0.0, texture_coordinates0->uv[0]);
  EXPECT_EQ(0.0, texture_coordinates0->uv[1]);

  AdditionalData additional_data1({0.0, 1.0, 0.0});
  auto texture_coordinates1 = triangle->ComputeTextureCoordinates(
      iris::Point(0.0, 0.0, 0.0), FRONT_FACE, &additional_data1);
  EXPECT_EQ(1.0, texture_coordinates1->uv[0]);
  EXPECT_EQ(0.0, texture_coordinates1->uv[1]);

  AdditionalData additional_data2({0.0, 0.0, 1.0});
  auto texture_coordinates2 = triangle->ComputeTextureCoordinates(
      iris::Point(0.0, 0.0, 0.0), FRONT_FACE, &additional_data2);
  EXPECT_EQ(0.0, texture_coordinates2->uv[0]);
  EXPECT_EQ(1.0, texture_coordinates2->uv[1]);
}

TEST(Triangle, ComputeShadingNormalNone) {
  auto triangles = iris::geometry::AllocateTriangleMesh(
      {iris::Point(0.0, 0.0, 0.0), iris::Point(1.0, 0.0, 0.0),
       iris::Point(0.0, 1.0, 0.0)},
      {{0, 1, 2}}, {}, {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, back_material,
      front_material, front_emissive_material, back_emissive_material,
      iris::ReferenceCounted<iris::NormalMap>(),
      iris::ReferenceCounted<iris::NormalMap>());

  auto front_normal =
      triangles.front()->ComputeShadingNormal(FRONT_FACE, nullptr);
  EXPECT_EQ(nullptr, std::get<const iris::NormalMap*>(front_normal));

  auto back_normal =
      triangles.front()->ComputeShadingNormal(BACK_FACE, nullptr);
  EXPECT_EQ(nullptr, std::get<const iris::NormalMap*>(back_normal));
}

TEST(Triangle, ComputeShadingNormalFromMap) {
  auto triangle = SimpleTriangle();

  auto front_normal = triangle->ComputeShadingNormal(FRONT_FACE, nullptr);
  EXPECT_EQ(front_normal_map.Get(),
            std::get<const iris::NormalMap*>(front_normal));

  auto back_normal = triangle->ComputeShadingNormal(BACK_FACE, nullptr);
  EXPECT_EQ(back_normal_map.Get(),
            std::get<const iris::NormalMap*>(back_normal));
}

TEST(Triangle, ComputeShadingNormalFromNormals) {
  auto triangles = iris::geometry::AllocateTriangleMesh(
      {iris::Point(0.0, 0.0, 0.0), iris::Point(1.0, 0.0, 0.0),
       iris::Point(0.0, 1.0, 0.0)},
      {{0, 1, 2}},
      {iris::Vector(1.0, 0.0, 0.0), iris::Vector(0.0, 1.0, 0.0),
       iris::Vector(0.0, 0.0, 1.0)},
      {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, back_material, front_material,
      front_emissive_material, back_emissive_material,
      iris::ReferenceCounted<iris::NormalMap>(),
      iris::ReferenceCounted<iris::NormalMap>());

  AdditionalData additional_data0({1.0, 0.0, 0.0});
  auto front_normal0 =
      triangles.front()->ComputeShadingNormal(FRONT_FACE, &additional_data0);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0), std::get<iris::Vector>(front_normal0));

  auto back_normal0 =
      triangles.front()->ComputeShadingNormal(BACK_FACE, &additional_data0);
  EXPECT_EQ(iris::Vector(-1.0, 0.0, 0.0), std::get<iris::Vector>(back_normal0));

  AdditionalData additional_data1({0.0, 1.0, 0.0});
  auto front_normal1 =
      triangles.front()->ComputeShadingNormal(FRONT_FACE, &additional_data1);
  EXPECT_EQ(iris::Vector(0.0, 1.0, 0.0), std::get<iris::Vector>(front_normal1));

  auto back_normal1 =
      triangles.front()->ComputeShadingNormal(BACK_FACE, &additional_data1);
  EXPECT_EQ(iris::Vector(0.0, -1.0, 0.0), std::get<iris::Vector>(back_normal1));

  AdditionalData additional_data2({0.0, 0.0, 1.0});
  auto front_normal2 =
      triangles.front()->ComputeShadingNormal(FRONT_FACE, &additional_data2);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), std::get<iris::Vector>(front_normal2));

  auto back_normal2 =
      triangles.front()->ComputeShadingNormal(BACK_FACE, &additional_data2);
  EXPECT_EQ(iris::Vector(0.0, 0.0, -1.0), std::get<iris::Vector>(back_normal2));
}

TEST(Triangle, GetMaterial) {
  auto triangle = SimpleTriangle();

  auto front = triangle->GetMaterial(FRONT_FACE, nullptr);
  EXPECT_EQ(front_material.Get(), front);

  auto back = triangle->GetMaterial(BACK_FACE, nullptr);
  EXPECT_EQ(back_material.Get(), back);
}

TEST(Triangle, IsEmissive) {
  auto triangles = iris::geometry::AllocateTriangleMesh(
      {iris::Point(0.0, 0.0, 0.0), iris::Point(1.0, 0.0, 0.0),
       iris::Point(0.0, 1.0, 0.0)},
      {{0, 1, 2}},
      {iris::Vector(1.0, 0.0, 0.0), iris::Vector(0.0, 1.0, 0.0),
       iris::Vector(0.0, 0.0, 1.0)},
      {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}}, back_material, front_material,
      front_emissive_material, iris::ReferenceCounted<iris::EmissiveMaterial>(),
      iris::ReferenceCounted<iris::NormalMap>(),
      iris::ReferenceCounted<iris::NormalMap>());
  EXPECT_TRUE(triangles.front()->IsEmissive(FRONT_FACE));
  EXPECT_FALSE(triangles.front()->IsEmissive(BACK_FACE));
}

TEST(Triangle, GetEmissiveMaterial) {
  auto triangle = SimpleTriangle();

  auto front = triangle->GetEmissiveMaterial(FRONT_FACE, nullptr);
  EXPECT_EQ(front_emissive_material.Get(), front);

  auto back = triangle->GetEmissiveMaterial(BACK_FACE, nullptr);
  EXPECT_EQ(back_emissive_material.Get(), back);
}

TEST(Triangle, SampleFace) {
  auto triangle = SimpleTriangle();

  iris::random::MockRandom rng0;
  EXPECT_CALL(rng0, NextGeometric()).WillRepeatedly(testing::Return(0.0));

  iris::Sampler sampler0(rng0);
  auto point0 = triangle->SampleFace(FRONT_FACE, sampler0);
  EXPECT_EQ(iris::Point(0.0, 0.0, 0.0), point0.value());

  iris::random::MockRandom rng1;
  EXPECT_CALL(rng1, NextGeometric()).WillRepeatedly(testing::Return(1.0));

  iris::Sampler sampler1(rng1);
  auto point1 = triangle->SampleFace(FRONT_FACE, sampler1);
  EXPECT_EQ(iris::Point(0.0, 0.0, 0.0), point1.value());

  iris::random::MockRandom rng2;
  {
    testing::InSequence s;
    EXPECT_CALL(rng2, NextGeometric()).WillOnce(testing::Return(1.0));
    EXPECT_CALL(rng2, NextGeometric()).WillOnce(testing::Return(0.0));
  }

  iris::Sampler sampler2(rng2);
  auto point2 = triangle->SampleFace(FRONT_FACE, sampler2);
  EXPECT_EQ(iris::Point(1.0, 0.0, 0.0), point2.value());

  iris::random::MockRandom rng3;
  {
    testing::InSequence s;
    EXPECT_CALL(rng3, NextGeometric()).WillOnce(testing::Return(0.0));
    EXPECT_CALL(rng3, NextGeometric()).WillOnce(testing::Return(1.0));
  }

  iris::Sampler sampler3(rng3);
  auto point3 = triangle->SampleFace(FRONT_FACE, sampler3);
  EXPECT_EQ(iris::Point(0.0, 1.0, 0.0), point3.value());

  iris::random::MockRandom rng4;
  EXPECT_CALL(rng4, NextGeometric()).WillRepeatedly(testing::Return(0.25));

  iris::Sampler sampler4(rng4);
  auto point4 = triangle->SampleFace(FRONT_FACE, sampler4);
  EXPECT_EQ(iris::Point(0.25, 0.25, 0.0), point4.value());

  iris::random::MockRandom rng5;
  EXPECT_CALL(rng5, NextGeometric()).WillRepeatedly(testing::Return(0.75));

  iris::Sampler sampler5(rng5);
  auto point5 = triangle->SampleFace(FRONT_FACE, sampler5);
  EXPECT_EQ(iris::Point(0.25, 0.25, 0.0), point5.value());
}

TEST(Triangle, ComputeArea) {
  auto triangle = SimpleTriangle();
  EXPECT_EQ(0.5, triangle->ComputeArea(FRONT_FACE));
}

TEST(Triangle, GetFaces) {
  auto triangle = SimpleTriangle();
  auto faces = triangle->GetFaces();
  ASSERT_EQ(2, faces.size());
  EXPECT_EQ(FRONT_FACE, faces[0]);
  EXPECT_EQ(BACK_FACE, faces[1]);
}