#include "iris/scene.h"

#include "googletest/include/gtest/gtest.h"

class TestScene : public iris::Scene {
 public:
  void Trace(const iris::Ray& ray, iris::Intersector& intersector) const {
    EXPECT_FALSE(true);
  }

  virtual iris::Scene::const_iterator begin() const { return end(); }
};

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