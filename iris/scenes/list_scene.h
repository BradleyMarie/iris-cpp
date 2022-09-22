#ifndef _IRIS_SCENES_LIST_SCENE_
#define _IRIS_SCENES_LIST_SCENE_

#include <memory>

#include "iris/geometry.h"
#include "iris/intersector.h"
#include "iris/matrix.h"
#include "iris/ray.h"
#include "iris/scene.h"

namespace iris {
namespace scenes {

class ListScene final : public Scene {
 public:
  class Builder final : public Scene::Builder {
   public:
    static std::unique_ptr<Scene::Builder> Create() {
      return std::make_unique<Builder>();
    }

   private:
    std::unique_ptr<Scene> Build(
        std::vector<std::pair<size_t, size_t>> geometry_and_matrix,
        std::vector<std::unique_ptr<Geometry>> geometry,
        std::vector<Matrix> matrices) override;
  };

  ListScene(std::vector<std::pair<size_t, size_t>> geometry_and_matrix,
            std::vector<std::unique_ptr<Geometry>> geometry,
            std::vector<Matrix> matrices) noexcept
      : geometry_and_matrix_(std::move(geometry_and_matrix)),
        geometry_(std::move(geometry)),
        matrices_(std::move(matrices)) {}

  const_iterator begin() const override;

  void Trace(const Ray& ray, Intersector& intersector) const override;

 private:
  const std::vector<std::pair<size_t, size_t>> geometry_and_matrix_;
  const std::vector<std::unique_ptr<Geometry>> geometry_;
  const std::vector<Matrix> matrices_;
};

}  // namespace scenes
}  // namespace iris

#endif  // _IRIS_SCENES_LIST_SCENE_