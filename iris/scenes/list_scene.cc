#include "iris/scenes/list_scene.h"

#include <algorithm>

namespace iris {
namespace scenes {

std::unique_ptr<Scene> ListScene::Builder::Build(
    std::vector<std::pair<size_t, size_t>> geometry_and_matrix,
    std::vector<std::unique_ptr<Geometry>> geometry,
    std::vector<Matrix> matrices) {
  std::vector<std::pair<const Geometry*, size_t>> geometry_ptr_and_matrix;
  for (const auto& entry : geometry_and_matrix) {
    geometry_ptr_and_matrix.emplace_back(geometry.at(entry.first).get(),
                                         entry.second);
  }

  return std::make_unique<ListScene>(std::move(geometry_ptr_and_matrix),
                                     std::move(geometry), std::move(matrices));
}

Scene::const_iterator ListScene::begin() const {
  size_t index = 0;
  return Scene::const_iterator(
      [this,
       index]() mutable -> std::optional<Scene::const_iterator::value_type> {
        if (index == geometry_and_matrix_.size()) {
          return std::nullopt;
        }

        size_t current = index++;
        bool has_matrix = geometry_and_matrix_[current].second != 0;
        return std::make_pair(
            std::cref(*geometry_and_matrix_[current].first),
            has_matrix ? &matrices_[geometry_and_matrix_[current].second]
                       : nullptr);
      });
}

void ListScene::Trace(const Ray& ray, Intersector& intersector) const {
  for (const auto& entry : geometry_and_matrix_) {
    if (entry.second != 0) {
      intersector.Intersect(*entry.first, matrices_[entry.second]);
    } else {
      intersector.Intersect(*entry.first);
    }
  }
}

}  // namespace scenes
}  // namespace iris