#include "iris/scene.h"

#include "googletest/include/gtest/gtest.h"

class TestScene : public iris::Scene {
 public:
  void Trace(const iris::Ray& ray,
             iris::Intersector& intersector) const override {
    EXPECT_FALSE(true);
  }

  virtual iris::Scene::const_iterator begin() const { return end(); }
};

class TestSceneBuilder : public iris::Scene::Builder {
 public:
  TestSceneBuilder(std::function<std::unique_ptr<iris::Scene>(
                       std::vector<std::pair<size_t, size_t>>,
                       std::vector<std::unique_ptr<iris::Geometry>>,
                       std::vector<iris::Matrix>)>
                       validation_function)
      : validation_function_(validation_function) {}

  std::unique_ptr<iris::Scene> Build(
      std::vector<std::pair<size_t, size_t>> geometry_and_matrix,
      std::vector<std::unique_ptr<iris::Geometry>> geometry,
      std::vector<iris::Matrix> matrices) override;

  std::function<std::unique_ptr<iris::Scene>(
      std::vector<std::pair<size_t, size_t>>,
      std::vector<std::unique_ptr<iris::Geometry>>, std::vector<iris::Matrix>)>
      validation_function_;
};

std::unique_ptr<iris::Scene> TestSceneBuilder::Build(
    std::vector<std::pair<size_t, size_t>> geometry_and_matrix,
    std::vector<std::unique_ptr<iris::Geometry>> geometry,
    std::vector<iris::Matrix> matrices) {
  return validation_function_(std::move(geometry_and_matrix),
                              std::move(geometry), std::move(matrices));
}

class TestGeometry : public iris::Geometry {
 private:
  iris::Hit* Trace(const iris::Ray& ray,
                   iris::HitAllocator& hit_allocator) const override {
    EXPECT_FALSE(true);
    return nullptr;
  }

 public:
  iris::Vector ComputeSurfaceNormal(
      const iris::Point& hit_point, iris::face_t face,
      const void* additional_data) const override {
    EXPECT_FALSE(true);
    return iris::Vector(1.0, 0.0, 0.0);
  }

  std::span<const iris::face_t> GetFaces() const override {
    static const iris::face_t faces[] = {1};
    EXPECT_FALSE(true);
    return faces;
  }
};

TEST(SceneBuilder, Build) {
  auto geom0 = std::make_unique<TestGeometry>();
  auto geom1 = std::make_unique<TestGeometry>();
  auto geom2 = std::make_unique<TestGeometry>();
  auto geom3 = std::make_unique<TestGeometry>();
  auto matrix0 = iris::Matrix::Identity();
  auto matrix1 = iris::Matrix::Translation(1.0, 2.0, 3.0).value();
  auto matrix2 = iris::Matrix::Scalar(1.0, 2.0, 3.0).value();

  auto geom0_ptr = geom0.get();
  auto geom1_ptr = geom1.get();
  auto geom2_ptr = geom2.get();
  auto geom3_ptr = geom3.get();

  iris::Scene* allocated_scene = nullptr;
  std::function<std::unique_ptr<iris::Scene>(
      std::vector<std::pair<size_t, size_t>> geometry_and_matrix,
      std::vector<std::unique_ptr<iris::Geometry>> geometry,
      std::vector<iris::Matrix>)>
      func = [&](std::vector<std::pair<size_t, size_t>> geometry_and_matrix,
                 std::vector<std::unique_ptr<iris::Geometry>> geometry,
                 std::vector<iris::Matrix> matrices) {
        EXPECT_EQ(4u, geometry_and_matrix.size());
        EXPECT_EQ(0u, geometry_and_matrix[0].first);
        EXPECT_EQ(1u, geometry_and_matrix[0].second);
        EXPECT_EQ(1u, geometry_and_matrix[1].first);
        EXPECT_EQ(2u, geometry_and_matrix[1].second);
        EXPECT_EQ(2u, geometry_and_matrix[2].first);
        EXPECT_EQ(2u, geometry_and_matrix[2].second);
        EXPECT_EQ(3u, geometry_and_matrix[3].first);
        EXPECT_EQ(0u, geometry_and_matrix[3].second);

        EXPECT_EQ(4u, geometry.size());
        EXPECT_EQ(geom0_ptr, geometry[0].get());
        EXPECT_EQ(geom1_ptr, geometry[1].get());
        EXPECT_EQ(geom2_ptr, geometry[2].get());
        EXPECT_EQ(geom3_ptr, geometry[3].get());

        EXPECT_EQ(3u, matrices.size());
        EXPECT_EQ(matrix0, matrices[0]);
        EXPECT_EQ(matrix1, matrices[1]);
        EXPECT_EQ(matrix2, matrices[2]);

        auto result = std::make_unique<TestScene>();
        allocated_scene = result.get();
        return result;
      };

  std::unique_ptr<iris::Scene::Builder> builder =
      std::make_unique<TestSceneBuilder>(func);

  builder->Add(std::move(geom0), matrix1);
  builder->Add(std::move(geom1), matrix2);
  builder->Add(std::move(geom2), matrix2);
  builder->Add(std::move(geom3), matrix0);

  EXPECT_EQ(allocated_scene, builder->Build().get());
  EXPECT_NE(nullptr, allocated_scene);

  // Reuse builder
  geom0 = std::make_unique<TestGeometry>();
  geom1 = std::make_unique<TestGeometry>();
  geom2 = std::make_unique<TestGeometry>();
  geom3 = std::make_unique<TestGeometry>();

  geom0_ptr = geom0.get();
  geom1_ptr = geom1.get();
  geom2_ptr = geom2.get();
  geom3_ptr = geom3.get();
  allocated_scene = nullptr;

  builder->Add(std::move(geom0), matrix1);
  builder->Add(std::move(geom1), matrix2);
  builder->Add(std::move(geom2), matrix2);
  builder->Add(std::move(geom3), matrix0);

  EXPECT_EQ(allocated_scene, builder->Build().get());
  EXPECT_NE(nullptr, allocated_scene);
}

TEST(SceneConstIteratorTest, Dereference) {
  auto* geom = reinterpret_cast<const iris::Geometry*>(1);
  auto matrix = iris::Matrix::Identity();
  iris::Scene::const_iterator iter(
      [&]() -> std::optional<iris::Scene::const_iterator::value_type> {
        return std::make_pair(std::cref(*geom), &matrix);
      });
  EXPECT_EQ(geom, &(*iter).first);
  EXPECT_EQ(&matrix, (*iter).second);
}

TEST(SceneConstIteratorTest, PreIncrement) {
  auto* geom0 = reinterpret_cast<const iris::Geometry*>(1);
  auto* geom1 = reinterpret_cast<const iris::Geometry*>(2);
  auto matrix0 = iris::Matrix::Identity();

  std::vector<std::pair<const iris::Geometry*, const iris::Matrix*>> geometry(
      {{geom0, &matrix0}, {geom1, nullptr}});

  auto geom_iter = geometry.cbegin();
  iris::Scene::const_iterator iter(
      [geom_iter, &geometry]() mutable
      -> std::optional<iris::Scene::const_iterator::value_type> {
        if (geom_iter == geometry.end()) {
          return std::nullopt;
        }
        auto current = geom_iter++;
        return iris::Scene::const_iterator::value_type(*current->first,
                                                       current->second);
      });

  EXPECT_EQ(geom0, &(*iter).first);
  EXPECT_EQ(&matrix0, (*iter).second);

  ++iter;
  EXPECT_EQ(geom1, &(*iter).first);
  EXPECT_EQ(nullptr, (*iter).second);

  ++iter;
  EXPECT_EQ(nullptr, &(*iter).first);
  EXPECT_EQ(nullptr, (*iter).second);

  ++iter;
  EXPECT_EQ(nullptr, &(*iter).first);
  EXPECT_EQ(nullptr, (*iter).second);
}

TEST(SceneConstIteratorTest, PostIncrement) {
  auto* geom0 = reinterpret_cast<const iris::Geometry*>(1);
  auto* geom1 = reinterpret_cast<const iris::Geometry*>(2);
  auto matrix0 = iris::Matrix::Identity();

  std::vector<std::pair<const iris::Geometry*, const iris::Matrix*>> geometry(
      {{geom0, &matrix0}, {geom1, nullptr}});

  auto geom_iter = geometry.cbegin();
  iris::Scene::const_iterator iter(
      [geom_iter, &geometry]() mutable
      -> std::optional<iris::Scene::const_iterator::value_type> {
        if (geom_iter == geometry.end()) {
          return std::nullopt;
        }
        auto current = geom_iter++;
        return iris::Scene::const_iterator::value_type(*current->first,
                                                       current->second);
      });

  auto first = iter++;
  EXPECT_EQ(geom0, &(*first).first);
  EXPECT_EQ(&matrix0, (*first).second);

  auto second = iter++;
  EXPECT_EQ(geom1, &(*second).first);
  EXPECT_EQ(nullptr, (*second).second);

  auto third = iter++;
  EXPECT_EQ(nullptr, &(*third).first);
  EXPECT_EQ(nullptr, (*third).second);

  auto fourth = iter++;
  EXPECT_EQ(nullptr, &(*fourth).first);
  EXPECT_EQ(nullptr, (*fourth).second);
}

TEST(SceneConstIteratorTest, Equals) {
  auto* geom0 = reinterpret_cast<const iris::Geometry*>(1);
  auto* geom1 = reinterpret_cast<const iris::Geometry*>(2);
  auto matrix0 = iris::Matrix::Identity();

  std::vector<std::pair<const iris::Geometry*, const iris::Matrix*>> geometry(
      {{geom0, &matrix0}, {geom1, nullptr}});

  auto geom_iter = geometry.cbegin();
  iris::Scene::const_iterator iter0(
      [geom_iter, &geometry]() mutable
      -> std::optional<iris::Scene::const_iterator::value_type> {
        if (geom_iter == geometry.end()) {
          return std::nullopt;
        }
        auto current = geom_iter++;
        return iris::Scene::const_iterator::value_type(*current->first,
                                                       current->second);
      });
  iris::Scene::const_iterator iter1(
      [geom_iter, &geometry]() mutable
      -> std::optional<iris::Scene::const_iterator::value_type> {
        if (geom_iter == geometry.end()) {
          return std::nullopt;
        }
        auto current = geom_iter++;
        return iris::Scene::const_iterator::value_type(*current->first,
                                                       current->second);
      });

  EXPECT_EQ(iter0, iter1);
}

TEST(SceneConstIteratorTest, NotEquals) {
  auto* geom0 = reinterpret_cast<const iris::Geometry*>(1);
  auto* geom1 = reinterpret_cast<const iris::Geometry*>(2);
  auto matrix0 = iris::Matrix::Identity();

  std::vector<std::pair<const iris::Geometry*, const iris::Matrix*>> geometry(
      {{geom0, &matrix0}, {geom1, nullptr}});

  auto geom_iter = geometry.cbegin();
  iris::Scene::const_iterator iter0(
      [geom_iter, &geometry]() mutable
      -> std::optional<iris::Scene::const_iterator::value_type> {
        if (geom_iter == geometry.end()) {
          return std::nullopt;
        }
        auto current = geom_iter++;
        return iris::Scene::const_iterator::value_type(*current->first,
                                                       current->second);
      });
  iris::Scene::const_iterator iter(
      [geom_iter, &geometry]() mutable
      -> std::optional<iris::Scene::const_iterator::value_type> {
        if (geom_iter == geometry.end()) {
          return std::nullopt;
        }
        auto current = geom_iter++;
        return iris::Scene::const_iterator::value_type(*current->first,
                                                       current->second);
      });

  auto first = iter++;
  EXPECT_EQ(geom0, &(*first).first);
  EXPECT_EQ(&matrix0, (*first).second);

  EXPECT_NE(first, iter);
}

TEST(SceneTest, End) {
  TestScene scene;

  auto iter = scene.end();
  EXPECT_EQ(nullptr, &(*iter).first);
  EXPECT_EQ(nullptr, (*iter).second);

  ++iter;
  EXPECT_EQ(nullptr, &(*iter).first);
  EXPECT_EQ(nullptr, (*iter).second);
}