#include "frontends/pbrt/material_manager.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/materials/mock_material.h"

TEST(MaterialManager, GetMissing) {
  iris::pbrt_frontend::MaterialManager material_manager;
  EXPECT_EXIT(material_manager.Get("test"),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: No material defined with name: test");
}

TEST(MaterialManager, Get) {
  auto material = iris::MakeReferenceCounted<iris::materials::MockMaterial>();
  iris::pbrt_frontend::MaterialManager material_manager;
  material_manager.Put("test", material);
  EXPECT_EQ(material.Get(), material_manager.Get("test").Get());
}

TEST(MaterialManager, Clear) {
  auto material = iris::MakeReferenceCounted<iris::materials::MockMaterial>();
  iris::pbrt_frontend::MaterialManager material_manager;
  material_manager.Clear();
  EXPECT_EXIT(material_manager.Get("test"),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: No material defined with name: test");
}