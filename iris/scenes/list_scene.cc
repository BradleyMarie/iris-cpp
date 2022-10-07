#include "iris/scenes/list_scene.h"

#include <algorithm>

namespace iris {
namespace scenes {

std::unique_ptr<Scene> ListScene::Builder::Build(
    std::vector<std::pair<size_t, size_t>> geometry_and_matrix,
    std::vector<std::unique_ptr<Geometry>> geometry,
    std::vector<Matrix> matrices) {
  std::vector<std::pair<const Geometry*, const Matrix*>>
      geometry_ptr_and_matrix;
  for (const auto& entry : geometry_and_matrix) {
    const Matrix* matrix =
        entry.second != 0 ? &matrices.at(entry.second) : nullptr;
    geometry_ptr_and_matrix.emplace_back(geometry.at(entry.first).get(),
                                         matrix);
  }

  geometry_ptr_and_matrix.shrink_to_fit();

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
        return std::make_pair(std::cref(*geometry_and_matrix_[current].first),
                              geometry_and_matrix_[current].second);
      });
}

void ListScene::Trace(const Ray& ray, Intersector& intersector) const {
  for (const auto& entry : geometry_and_matrix_) {
    intersector.Intersect(*entry.first, entry.second);
  }
}

}  // namespace scenes
}  // namespace iris