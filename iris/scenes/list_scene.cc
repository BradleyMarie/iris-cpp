#include "iris/scenes/list_scene.h"

namespace iris {
namespace scenes {

std::unique_ptr<Scene> ListScene::Builder::Build(
    std::vector<std::pair<size_t, size_t>> geometry_and_matrix,
    std::vector<std::unique_ptr<Geometry>> geometry,
    std::vector<Matrix> matrices) {
  std::vector<std::shared_ptr<Geometry>> shared_geometry;
  for (auto& geom : geometry) {
    shared_geometry.push_back(std::move(geom));
  }

  std::vector<std::shared_ptr<Geometry>> scene_geometry;
  std::vector<std::shared_ptr<Matrix>> scene_matrices;
  for (const auto& [geometry_index, matrix_index] : geometry_and_matrix) {
    scene_geometry.push_back(shared_geometry[geometry_index]);
    auto matrix = (matrix_index != 0u)
                      ? std::make_shared<Matrix>(matrices[matrix_index])
                      : nullptr;
    scene_matrices.push_back(std::move(matrix));
  }

  return std::make_unique<ListScene>(std::move(scene_geometry),
                                     std::move(scene_matrices));
}

Scene::const_iterator ListScene::begin() const {
  size_t index = 0;
  return Scene::const_iterator(
      [this,
       index]() mutable -> std::optional<Scene::const_iterator::value_type> {
        if (index == geometry_.size()) {
          return std::nullopt;
        }

        size_t current = index++;
        return std::make_pair(std::cref(*geometry_[current]),
                              matrices_[current].get());
      });
}

void ListScene::Trace(const Ray& ray, Intersector& intersector) const {
  for (size_t i = 0u; i < geometry_.size(); i++) {
    intersector.Intersect(*geometry_[i], matrices_[i].get());
  }
}

}  // namespace scenes
}  // namespace iris